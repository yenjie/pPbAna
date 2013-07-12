#include "utilities.h"

void plotMpt2(char *infname)
{
   TFile *inf = new TFile(infname);
   
   TTree *t = (TTree*)inf->Get("t");
   
   TCanvas *c = new TCanvas("c","",600,600);
   TProfile *hRecoMpt = new TProfile("hRecoMpt","",10,0,200);
   TProfile *hCorMpt = new TProfile("hCorMpt","",10,0,200);
   TProfile *hCor2Mpt = new TProfile("hCor2Mpt","",10,0,200);
   TProfile *hGenMpt = new TProfile("hGenMpt","",10,0,200);
   TProfile *hGenPMpt = new TProfile("hGenPMpt","",10,0,200);
   
   TCut sel = "leadingJetPt>120&&subleadingJetPt>50&&acos(cos(leadingJetPhi-subleadingJetPhi))>7./8*3.1415926";
   
   t->Draw("mpt:leadingJetPt-subleadingJetPt>>hRecoMpt",sel,"prof");
   t->Draw("cormpt:leadingJetPt-subleadingJetPt>>hCorMpt",sel,"prof");
   t->Draw("cormpt2:leadingJetPt-subleadingJetPt>>hCor2Mpt",sel,"prof");
   t->Draw("genMpt:leadingJetPt-subleadingJetPt>>hGenMpt",sel,"prof");
   t->Draw("genPMpt:leadingJetPt-subleadingJetPt>>hGenPMpt",sel,"prof");

   hRecoMpt->SetLineColor(2);
   hRecoMpt->SetMarkerColor(2);
   hCorMpt->SetLineColor(4);
   hCorMpt->SetMarkerColor(4);
   hGenMpt->SetLineColor(1);
   hGenPMpt->SetLineStyle(2);

   hGenMpt->SetAxisRange(-100,100,"Y");
   makeHistTitle(hGenMpt,"","#Delta p_{T}","#slash{p}_{T}^{||}");
   hGenMpt->Draw("hist");
   hRecoMpt->Draw("same");
   hCorMpt->Draw("same");
   hCor2Mpt->Draw("same");
   hGenPMpt->Draw("same hist");
   
   TLegend *leg = myLegend(0.4,0.6,0.9,0.92);
   
   leg->AddEntry(hGenMpt,"Gen #slash{p}_{T}^{||}","l");
   leg->AddEntry(hGenPMpt,"Gen #slash{p}_{T}^{||} with reco jet axis","l");
   leg->AddEntry(hRecoMpt,"Reco #slash{p}_{T}^{||}","pl");
   leg->AddEntry(hCorMpt,"Corrected #slash{p}_{T}^{||} with jet p_{T} para","pl");
   leg->AddEntry(hCor2Mpt,"Corrected #slash{p}_{T}^{||} with track #phi para","pl");
   leg->Draw();   
}
