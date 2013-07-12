#include "utilities.h"

#include <TMarker.h>
#include <TLine.h>


void drawMean(TH1 *h,double y)
{
  TMarker *m = new TMarker(h->GetMean(),y,24);
  m->SetMarkerColor(h->GetMarkerColor());  
  m->Draw();
  TLine *l = new TLine(h->GetMean()-h->GetMeanError(),y,h->GetMean()+h->GetMeanError(),y);
  l->SetLineColor(h->GetMarkerColor());
  l->Draw();
}

void plotAJ2(double leadingJetPtCut = 80, double subleadingJetPtCut = 30)
{
   TCanvas *c = new TCanvas("c","",1200,600);
   c->Divide(2,1);
   c->cd(1);
//   TFile *inf = new TFile ("output-ak5CaloRun210534.root");
   TFile *inf = new TFile ("output-ak5CaloRun210534Trig.root");
   TTree *ntPPb = (TTree*)inf->Get("t");
   
   TH1D *hPPb1 = new TH1D("hPPb1","",10,0,1);
   TH1D *hPPb2 = new TH1D("hPPb2","",10,0,1);
   TH1D *hPPb3 = new TH1D("hPPb3","",10,0,1);
   TH1D *hPPb4 = new TH1D("hPPb4","",10,0,1);
   
   ntPPb->Draw("Aj>>hPPb1",Form("N<60&&leadingJetEta<100&&leadingJetPt>%.0f&&subleadingJetPt>%.0f&&acos(cos(leadingJetPhi-subleadingJetPhi))>2./3*3.14159",leadingJetPtCut,subleadingJetPtCut));
   ntPPb->Draw("Aj>>hPPb2",Form("N>60&&leadingJetEta<100&&N<100&&leadingJetPt>%.0f&&subleadingJetPt>%.0f&&acos(cos(leadingJetPhi-subleadingJetPhi))>2./3*3.14159",leadingJetPtCut,subleadingJetPtCut));
   ntPPb->Draw("Aj>>hPPb3",Form("N>100&&leadingJetEta<100&&N<150&&leadingJetPt>%.0f&&subleadingJetPt>%.0f&&acos(cos(leadingJetPhi-subleadingJetPhi))>2./3*3.14159",leadingJetPtCut,subleadingJetPtCut));
   ntPPb->Draw("Aj>>hPPb4",Form("N>150&&leadingJetEta<100&&leadingJetPt>%.0f&&subleadingJetPt>%.0f&&acos(cos(leadingJetPhi-subleadingJetPhi))>2./3*3.14159",leadingJetPtCut,subleadingJetPtCut));

   hPPb1->Sumw2();
   hPPb1->Scale(1./hPPb1->GetEntries());
   hPPb2->Sumw2();
   hPPb2->Scale(1./hPPb2->GetEntries());
   hPPb3->Sumw2();
   hPPb3->Scale(1./hPPb3->GetEntries());
   hPPb4->Sumw2();
   hPPb4->Scale(1./hPPb4->GetEntries());
   cout <<hPPb1->GetEntries()<<endl;
   cout <<hPPb2->GetEntries()<<endl;
   cout <<hPPb3->GetEntries()<<endl;
   cout <<hPPb4->GetEntries()<<endl;
   cout <<hPPb1->GetMean()<<endl;
   cout <<hPPb2->GetMean()<<endl;
   cout <<hPPb3->GetMean()<<endl;
   cout <<hPPb4->GetMean()<<endl;



//   hPPb1->SetFillColor(4);
//   hPPb1->SetFillStyle(3335);   
   hPPb1->Draw("");
   hPPb2->SetLineColor(2);
   hPPb2->SetMarkerColor(2);
   hPPb3->SetLineColor(4);
   hPPb3->SetMarkerColor(4);
   hPPb4->SetLineColor(kGreen+2);
   hPPb4->SetMarkerColor(kGreen+2);
//   hPPb2->Draw("e same");
//   hPPb3->Draw("e same");
   hPPb4->Draw("e same");
   hPPb1->SetAxisRange(0,0.5/hPPb1->GetNbinsX()*10,"Y");

   hPPb1->SetXTitle("A_{J}");
   hPPb1->SetYTitle("Event Fraction");
   hPPb1->GetXaxis()->CenterTitle();
   hPPb1->GetYaxis()->CenterTitle();
      
   TLegend *leg1 = myLegend(0.45,0.8,0.88,0.99);
   leg1->AddEntry(hPPb1,"CMS Preliminary","");

   leg1->Draw();

   TLegend *leg = myLegend(0.35,0.55,0.9,0.85);
   leg->AddEntry(hPPb1,"p+Pb data #sqrt{s_{NN}} = 5.02 TeV N<60","pl");
//   leg->AddEntry(hPPb2,"p+Pb data #sqrt{s_{NN}} = 5.02 TeV 60<N<100","pl");
//   leg->AddEntry(hPPb3,"p+Pb data #sqrt{s_{NN}} = 5.02 TeV 100<N<150","pl");
   leg->AddEntry(hPPb4,"p+Pb data #sqrt{s_{NN}} = 5.02 TeV 150<N","pl");
   leg->AddEntry(hPPb1,Form("Leading Jet p_{T}>%.0f GeV/c",leadingJetPtCut),"");
   leg->AddEntry(hPPb1,Form("Subleading Jet p_{T}>%.0f GeV/c",subleadingJetPtCut),"");
   leg->AddEntry(hPPb1,Form("Dijet #Delta#phi > #frac{2}{3}#pi"),"");
   leg->Draw();

   drawMean(hPPb1,0.01);
//   drawMean(hPPb2,0.02);
//   drawMean(hPPb3,0.03);
   drawMean(hPPb4,0.04);

   c->cd(2)->SetLogy();
   TH1D *hPPbDphi1 = new TH1D("hPPbDphi1","",10,0,3.14159);
   TH1D *hPPbDphi2 = new TH1D("hPPbDphi2","",10,0,3.14159);
   TH1D *hPPbDphi3 = new TH1D("hPPbDphi3","",10,0,3.14159);
   TH1D *hPPbDphi4 = new TH1D("hPPbDphi4","",10,0,3.14159);
   
   ntPPb->Draw("acos(cos(leadingJetPhi-subleadingJetPhi))>>hPPbDphi1",Form("N<60&&leadingJetPt>%.0f&&subleadingJetPt>%.0f&&acos(cos(leadingJetPhi-subleadingJetPhi))>0*3.14159",leadingJetPtCut,subleadingJetPtCut));
   ntPPb->Draw("acos(cos(leadingJetPhi-subleadingJetPhi))>>hPPbDphi2",Form("N>60&&N<100&&leadingJetPt>%.0f&&subleadingJetPt>%.0f&&acos(cos(leadingJetPhi-subleadingJetPhi))>0*3.14159",leadingJetPtCut,subleadingJetPtCut));
   ntPPb->Draw("acos(cos(leadingJetPhi-subleadingJetPhi))>>hPPbDphi3",Form("N>100&&N<150&&leadingJetPt>%.0f&&subleadingJetPt>%.0f&&acos(cos(leadingJetPhi-subleadingJetPhi))>0*3.14159",leadingJetPtCut,subleadingJetPtCut));
   ntPPb->Draw("acos(cos(leadingJetPhi-subleadingJetPhi))>>hPPbDphi4",Form("N>150&&leadingJetPt>%.0f&&subleadingJetPt>%.0f&&acos(cos(leadingJetPhi-subleadingJetPhi))>0*3.14159",leadingJetPtCut,subleadingJetPtCut));

   hPPbDphi1->Sumw2();
   hPPbDphi1->Scale(1./hPPbDphi1->GetEntries());
   hPPbDphi2->Sumw2();
   hPPbDphi2->Scale(1./hPPbDphi2->GetEntries());
   hPPbDphi3->Sumw2();
   hPPbDphi3->Scale(1./hPPbDphi3->GetEntries());
   hPPbDphi4->Sumw2();
   hPPbDphi4->Scale(1./hPPbDphi4->GetEntries());
   cout <<hPPbDphi1->GetEntries()<<endl;
   cout <<hPPbDphi2->GetEntries()<<endl;
   cout <<hPPbDphi3->GetEntries()<<endl;
   cout <<hPPbDphi4->GetEntries()<<endl;
   cout <<hPPbDphi1->GetMean()<<endl;
   cout <<hPPbDphi2->GetMean()<<endl;
   cout <<hPPbDphi3->GetMean()<<endl;
   cout <<hPPbDphi4->GetMean()<<endl;



//   hPPbDphi1->SetFillColor(4);
//   hPPbDphi1->SetFillStyle(3335);   
   hPPbDphi1->Draw("");
   hPPbDphi2->SetLineColor(2);
   hPPbDphi2->SetMarkerColor(2);
   hPPbDphi3->SetLineColor(4);
   hPPbDphi3->SetMarkerColor(4);
   hPPbDphi4->SetLineColor(kGreen+2);
   hPPbDphi4->SetMarkerColor(kGreen+2);
//   hPPbDphi2->Draw("e same");
//   hPPbDphi3->Draw("e same");
   hPPbDphi4->Draw("e same");
   hPPbDphi1->SetAxisRange(0.002,2,"Y");

   hPPbDphi1->SetXTitle("#Delta #phi");
   hPPbDphi1->SetYTitle("Event Fraction");
   hPPbDphi1->GetXaxis()->CenterTitle();
   hPPbDphi1->GetYaxis()->CenterTitle();
      
   TLegend *leg2 = myLegend(0.2,0.8,0.63,0.99);
   leg2->AddEntry(hPPbDphi1,"CMS Preliminary","");

   leg2->Draw();

   TLegend *leg3 = myLegend(0.2,0.6,0.75,0.85);
   leg3->AddEntry(hPPbDphi1,"p+Pb data #sqrt{s_{NN}} = 5.02 TeV N<60","pl");
 //  leg3->AddEntry(hPPbDphi2,"p+Pb data #sqrt{s_{NN}} = 5.02 TeV 60<N<100","pl");
 //  leg3->AddEntry(hPPbDphi3,"p+Pb data #sqrt{s_{NN}} = 5.02 TeV 100<N<150","pl");
   leg3->AddEntry(hPPbDphi4,"p+Pb data #sqrt{s_{NN}} = 5.02 TeV 150<N","pl");
   leg3->AddEntry(hPPbDphi1,Form("Leading Jet p_{T}>%.0f GeV/c",leadingJetPtCut),"");
   leg3->AddEntry(hPPbDphi1,Form("Subleading Jet p_{T}>%.0f GeV/c",subleadingJetPtCut),"");
//   leg3->AddEntry(hPPbDphi1,Form("Dijet #Delta#phi > #frac{7}{8}#pi"),"");
   leg3->Draw();

   drawMean(hPPbDphi1,0.02);
//   drawMean(hPPbDphi2,0.03);
//   drawMean(hPPbDphi3,0.04);
   drawMean(hPPbDphi4,0.05);

   c->SaveAs(Form("aj/aj-%.0f-%.0f.gif",leadingJetPtCut,subleadingJetPtCut));
   c->SaveAs(Form("aj/aj-%.0f-%.0f.C",leadingJetPtCut,subleadingJetPtCut));
}
