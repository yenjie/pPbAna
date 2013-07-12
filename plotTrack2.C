#include "HiForest/hiForest.h"

#include "utilities.h"

void plotTrack2(char *infname)
{
   TCanvas *c = new TCanvas("c","",600,600);
   HiForest *h = new HiForest(infname,"",cPPb);
   
   TH1D *hEta = new TH1D("hEta","",100,-3,3);
   TH1D *hEtaHighPurity = new TH1D("hEtaHighPurity","",100,-3,3);
   
   double nEvt = h->tree->GetEntries("abs(vz[1])<15&&HLT_PAZeroBiasPixel_SingleTrack_v1");
   h->tree->Draw("trkEta>>hEta","Run==202792&&lumi>=317&&lumi<=1014&&abs(vz[1])<15&&HLT_PAZeroBiasPixel_SingleTrack_v1");
   h->tree->Draw("trkEta>>hEtaHighPurity","Run==202792&&lumi>=317&&lumi<=1014&&abs(vz[1])<15&&HLT_PAZeroBiasPixel_SingleTrack_v1&&highPurity&&abs(trkDz1/trkDzError1)<3&&abs(trkDz1/trkDzError1)<3&&trkPtError/trkPt<0.1");

   hEta->SetLineColor(2);
   hEta->Scale(1./nEvt);
   hEtaHighPurity->Scale(1./nEvt);
   hEta->Draw();
   hEtaHighPurity->Draw("same");
   makeHistTitle(hEta,"","Track #eta","Entries / Event",2);
   
   TLegend *leg = myLegend(0.36,0.20,0.72,0.39);
   
   leg->AddEntry(hEta,"All tracks","l");
   leg->AddEntry(hEtaHighPurity,"High purity tracks","l");
   leg->Draw();
   c->SaveAs(Form("CF/trkEta-%s.eps",infname));
   c->SaveAs(Form("CF/trkEta-%s.gif",infname));
   TCanvas *c2 = new TCanvas("c2","",600,600);

   TH1D *hN = new TH1D("hN","",100,0,300);
   TH1D *hNHighPurity = new TH1D("hNHighPurity","",100,0,300);
   
   h->tree->Draw("nTrk>>hN","Run==202792&&lumi>=317&&lumi<=1014&&abs(vz[1])<15&&HLT_PAZeroBiasPixel_SingleTrack_v1");
   h->tree->Draw("Sum$(highPurity&&abs(trkDz1/trkDzError1)<3&&abs(trkDz1/trkDzError1)<3&&trkPtError/trkPt<0.1)>>hNHighPurity","Run==202792&&lumi>=317&&lumi<=1014&&abs(vz[1])<15&&HLT_PAZeroBiasPixel_SingleTrack_v1");

   hN->SetLineColor(2);
   hN->SetAxisRange(0,hN->GetMaximum()*1.5,"Y");
   hN->Draw();
   hNHighPurity->Draw("same");
   makeHistTitle(hN,"","N_{Track}","Entries",2);
   
   TLegend *legN = myLegend(0.36,0.70,0.72,0.89);
   
   legN->AddEntry(hN,"All tracks","l");
   legN->AddEntry(hNHighPurity,"High purity tracks","l");
   legN->Draw();
   c2->SaveAs(Form("CF/nTrk-%s.eps",infname));
   c2->SaveAs(Form("CF/nTrk-%s.gif",infname));

}
