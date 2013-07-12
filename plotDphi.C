#include "utilities.h"

void plotDphi(double leadingJetPtCut = 50, double subleadingJetPtCut = 30)
{
   TCanvas *c = new TCanvas("c","",600,600);
   c->SetLogy()   ;
   TFile *infPP = new TFile ("output-Dijet30PPMC.root");
   TTree *ntPP = (TTree*)infPP->Get("t");
   TFile *inf = new TFile ("output-pPb.root");
   TTree *ntPPb = (TTree*)inf->Get("t");
   
   TH1D *hPP = new TH1D("hPP","",20,0,3.14159);
   TH1D *hPPb = new TH1D("hPPb","",20,0,3.14159);
   
   ntPP->Draw("acos(cos(leadingJetPhi-subleadingJetPhi))>>hPP",Form("leadingJetPt>%.0f&&subleadingJetPt>%.0f",leadingJetPtCut,subleadingJetPtCut));
   ntPPb->Draw("acos(cos(leadingJetPhi-subleadingJetPhi))>>hPPb",Form("leadingJetPt>%.0f&&subleadingJetPt>%.0f",leadingJetPtCut,subleadingJetPtCut));

//   hPP->Sumw2();
   hPPb->Sumw2();
   hPP->Scale(1./hPP->GetEntries());
   hPPb->Scale(1./hPPb->GetEntries());

   hPP->SetFillColor(4);
   hPP->SetFillStyle(3335);   
   hPP->Draw("h");
   hPPb->Draw("e same");
   hPP->SetAxisRange(0.0001,1,"Y");

   hPP->SetXTitle("#Delta#phi");
   hPP->SetYTitle("Event Fraction");
   hPP->GetXaxis()->CenterTitle();
   hPP->GetYaxis()->CenterTitle();
      
   TLegend *leg1 = myLegend(0.5,0.8,0.93,0.99);
   leg1->AddEntry(hPP,"CMS Preliminary","");
   leg1->Draw();

   TLegend *leg = myLegend(0.19,0.57,0.64,0.82);
   leg->AddEntry(hPP,"PYTHIA dijet30 #sqrt{s} = 2.76 TeV","f");
   leg->AddEntry(hPPb,"p+Pb data #sqrt{s_{NN}} = 5.02 TeV","pl");
   leg->AddEntry(hPPb,Form("Leading Jet p_{T}>%.0f GeV/c",leadingJetPtCut),"");
   leg->AddEntry(hPPb,Form("Subleading Jet p_{T}>%.0f GeV/c",subleadingJetPtCut),"");
   leg->AddEntry(hPPb,Form("Dijet #Delta#phi > #frac{7}{8}#pi"),"");
   leg->Draw();
}