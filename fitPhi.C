#include "utilities.h"
TF1 *fit(TTree *nt,int nMin,int nMax)
{   
   static int count=0;
   count++;
   TCanvas *c= new TCanvas(Form("c%d",count),"",600,600);
   TH1D *h = new TH1D(Form("h%d",count),"",300,0.8995,1.1995);
   TH1D *hB = new TH1D(Form("hB%d",count),"",300,0.8995,1.1995);
//   TF1 *f = new TF1(Form("f%d",count),"[0]*TMath::BreitWigner(x,[1],[2])+[3]+[4]*x+[5]*x*x+[6]*x*x*x+[7]*x*x*x*x");
   TF1 *f = new TF1(Form("f%d",count),"[0]*TMath::BreitWigner(x,[1],[2])+[3]+[4]*x+[5]*x*x+[6]*x*x*x+[7]*x*x*x*x");
   f->SetParameters(1,1.02,0.010,1,1,1);
   f->FixParameter(1,1.02);
   f->FixParameter(2,5e-3);
   f->SetRange(1,1.06);
   //nt->Draw(Form("m>>h%d",count),Form("ch1!=ch2&&pt1>0&&pt2>0&&pt>25&&pt<300&&N<%d&&N>=%d&&(!highPurity1||!highPurity2)",nMax,nMin));
//nt->Draw(Form("m>>h%d",count),Form("ch1!=ch2&&pt1>0&&pt2>0&&pt>30&&pt<300&&N<%d&&N>=%d&&(highPurity1&&highPurity2&&abs(dxy1)<3&&abs(dxy2)<3&&ptError1<0.1&&ptError2<0.1)&&nhit1>5&&nhit2>5",nMax,nMin));
nt->Draw(Form("m>>h%d",count),Form("ch1!=ch2&&pt1>0&&pt2>0&&pt>20&&abs((pt1-pt2)/(pt1+pt2))<200&&pt<300&&N<%d&&N>=%d",nMax,nMin));
nt->Draw(Form("m>>hB%d",count),Form("ch1==ch2&&pt1>0&&pt2>0&&pt>20&&abs((pt1-pt2)/(pt1+pt2))<200&&pt<300&&N<%d&&N>=%d",nMax,nMin),"same");
   h->Fit(Form("f%d",count),"","",1.01,1.03);
   h->Fit(Form("f%d",count),"","",1.01,1.03);
   h->Fit(Form("f%d",count),"LL","",1,1.035);
   h->Fit(Form("f%d",count),"LL","",1,1.04);
   f->ReleaseParameter(1);
   f->ReleaseParameter(2);
   h->Fit(Form("f%d",count),"LL","",0.995,1.05);
   h->Fit(Form("f%d",count),"","",0.995,1.07);
   h->Fit(Form("f%d",count),"LL m","",1,1.07);
   
   TF1 *background = new TF1("background","[0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x");
   background->SetParameter(0,f->GetParameter(3));
   background->SetParameter(1,f->GetParameter(4));
   background->SetParameter(2,f->GetParameter(5));
   background->SetParameter(3,f->GetParameter(6));
   background->SetParameter(4,f->GetParameter(7));
   background->SetLineColor(4);
   background->SetRange(0.995,1.06);
   background->SetLineStyle(2);
   TF1 *mass = new TF1("mass","[0]*TMath::BreitWigner(x,[1],[2])");
   mass->SetParameters(f->GetParameter(0),f->GetParameter(1),f->GetParameter(2));
   mass->SetParError(0,f->GetParError(0));
   mass->SetParError(1,f->GetParError(1));
   mass->SetParError(2,f->GetParError(2));
   mass->SetRange(1,1.06);
   mass->SetLineColor(2);
   mass->SetLineStyle(2);
   cout <<mass->Integral(0,1.2)<<" "<<mass->IntegralError(0,1.2)<<endl;
   h->SetMarkerStyle(24);
   h->SetStats(0);
   h->Draw("e");
   h->SetXTitle("M_{KK} (GeV/c^{2})");
   h->SetYTitle("Entries / (1 MeV/c^{2})");
   h->GetXaxis()->CenterTitle();
   h->GetYaxis()->CenterTitle();
   h->SetTitleOffset(1.65,"Y");
   h->SetAxisRange(0.985,1.07,"X");
   h->SetAxisRange(0,h->GetMaximum()*1.2,"Y");
   background->Draw("same");   
   mass->Draw("same");
   mass->SetLineStyle(2);
   mass->SetFillStyle(3004);
   mass->SetFillColor(2);
   f->Draw("same");
   
   TLegend *leg = myLegend(0.50,0.78,0.86,0.92);
   leg->AddEntry(h,"CMS Preliminary","pl");
   leg->AddEntry(h,"p+Pb #sqrt{s_{NN}}= 5.02 TeV","");
   leg->AddEntry(h,Form("%d < N_{trk}^{offline} < %d",nMin,nMax),"");
   leg->Draw();
   TLegend *leg2 = myLegend(0.44,0.23,0.89,0.40);
   leg2->AddEntry(h,"#phi meson p_{T}> 30 GeV/c","");
   leg2->AddEntry(h,Form("M_{#phi}=%.2f #pm %.2f MeV/c^{2}",f->GetParameter(1)*1000.,f->GetParError(1)*1000.),"");
   leg2->AddEntry(h,Form("N_{#phi}=%.0f #pm %.0f",f->GetParameter(0)*1000.,f->GetParError(0)*1000.),"");
   leg2->Draw();
   c->Write();
   c->SaveAs(Form("phiFigure/phiMass-%d.C",count));
   c->SaveAs(Form("phiFigure/phiMass-%d.gif",count));
   c->SaveAs(Form("phiFigure/phiMass-%d.eps",count));
   return f;
}

void fitPhi(char *infname)
{

   TFile *inf = new TFile(infname);
   TTree *nt = (TTree*) inf->Get("nt");
   TTree *ntEvt = (TTree*) inf->Get("ntEvt");

   TFile *outf = new TFile("phiHistos.root","recreate");

   const int nBins = 1;
   Float_t bins[nBins+1] = {0,10000};
//   const int nBins = 1;
//   Float_t bins[nBins+1] = {100,110};

   TH1D *hYield = new TH1D("hYield","",nBins,bins);
   TH1D *hMass = new TH1D("hMass","",nBins,bins);
   TH1D *hMult = new TH1D("hMult","",500,0,500);
   for (int i=1;i<=nBins;i++)
   {   
     ntEvt->Project("hMult","N",Form("N>=%.0f&&N<%.0f",bins[i-1],bins[i]));
     Float_t nEntry = hMult->GetEntries();
     TF1 *f = fit(nt,bins[i-1],bins[i]);
     cout <<nEntry<<endl; 
     hYield->SetBinContent(i,f->GetParameter(0)*1000./hMult->GetMean()/nEntry);
     hYield->SetBinError(i,f->GetParError(0)*1000./hMult->GetMean()/nEntry);
     hMass->SetBinContent(i,f->GetParameter(1)*1.);
     hMass->SetBinError(i,f->GetParError(1)*1.);
   }  

   TCanvas *cSummary = new TCanvas("cSummary","Summray",600,600,600,600);
   hYield->SetXTitle("N_{trk}^{offline}");
   hYield->SetYTitle("N_{#phi} / N_{trk}^{offline}");
   hYield->GetXaxis()->CenterTitle();
   hYield->GetYaxis()->CenterTitle();
   hYield->SetTitleOffset(1.65,"Y");
   hYield->Draw(); 
   cSummary->SaveAs("phiFigure/phiYieldFraction.C");
   cSummary->SaveAs("phiFigure/phiYieldFraction.gif");
   cSummary->SaveAs("phiFigure/phiYieldFraction.eps");

   TCanvas *cMass = new TCanvas("cMass","Mass",600,600,600,600);
   hMass->SetXTitle("N_{trk}^{offline}");
   hMass->SetYTitle("M_{#phi} (GeV/c^{2})");
   hMass->GetXaxis()->CenterTitle();
   hMass->GetYaxis()->CenterTitle();
   hMass->SetTitleOffset(1.65,"Y");
   hMass->SetAxisRange(1.018,1.022,"Y");
   hMass->Draw(); 

   TLine *l = new TLine(0,1.019445,200,1.019445);
   l->SetLineStyle(2);
   l->Draw("same");

   cMass->SaveAs("phiFigure/phiMass.C");
   cMass->SaveAs("phiFigure/phiMass.gif");
   cMass->SaveAs("phiFigure/phiMass.eps");
   outf->Write();

}
