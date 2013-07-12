#include "utilities.h"
#include "HiForest/hiForest.h"

void trackingStudyJet(char *infname = "skim_jet_ptha120.root")
{
   HiForest *c = new HiForest(infname);
   c->tree->AddFriend("nt");

   TCut trackSelection = "(mtrkQual||mtrkAlgo<4)&&pNRec>0";
   TCut dijetSelection = "leadingJetPt>120&&subleadingJetPt>50&&acos(cos(leadingJetPhi-subleadingJetPhi))>7./8.*3.141593";
   TCut genParticleCut = "abs(pEta)<0.8";
   
   TCanvas *cleadingJetPt = new TCanvas ("cleadingJetPt","",600,600);
   
   TH1F *hEffleadingJetPt = new TH1F("hEffleadingJetPt","",15,0,300);
   
   makeHistTitle(hEffleadingJetPt,"","Leading Jet p_{T} (GeV/c)","Tracking Efficiency");
   
   TGraphAsymmErrors *g = getEfficiency(c->tree,"leadingJetPt",15,0,300,trackSelection,TCut("pPt>20")&&dijetSelection&&genParticleCut);
   TGraphAsymmErrors *g2 = getEfficiency(c->tree,"leadingJetPt",15,0,300,trackSelection,TCut("pPt>8")&&dijetSelection&&genParticleCut);
   TGraphAsymmErrors *g3 = getEfficiency(c->tree,"leadingJetPt",15,0,300,trackSelection,TCut("pPt>0")&&dijetSelection&&genParticleCut);

   hEffleadingJetPt->SetAxisRange(0.6,0.9,"Y");
   hEffleadingJetPt->Draw();
   
   g->Draw("p same");
   g2->SetLineColor(2);
   g2->SetMarkerColor(2);
   g2->Draw("p same");
   
   g3->SetLineColor(4);
   g3->SetMarkerColor(4);
   g3->Draw("p same");

   TLegend *legleadingJetPt = myLegend(0.5,0.7,0.9,0.9);
   legleadingJetPt->AddEntry(g,"#hat{p}_{T} 120 GeV/c, Dijet selection","");
   legleadingJetPt->AddEntry(g,"simTrack p_{T} > 20 GeV/c","pl");
   legleadingJetPt->AddEntry(g2,"simTrack p_{T} > 8 GeV/c","pl");
   legleadingJetPt->AddEntry(g3,"simTrack p_{T} > 0.5 GeV/c","pl");
   
   legleadingJetPt->Draw();
   cleadingJetPt->SaveAs("effFig/EffVsleadingJetPt-DijetSelection.gif");
   cleadingJetPt->SaveAs("effFig/EffVsleadingJetPt-DijetSelection.C");

   TCanvas *cleadingJetPtNoDijet = new TCanvas ("cleadingJetPtNoDijet","",600,600);

   TGraphAsymmErrors *gg = getEfficiency(c->tree,"leadingJetPt",15,0,300,trackSelection,TCut("pPt>20")&&genParticleCut);
   TGraphAsymmErrors *gg2 = getEfficiency(c->tree,"leadingJetPt",15,0,300,trackSelection,TCut("pPt>8")&&genParticleCut);
   TGraphAsymmErrors *gg3 = getEfficiency(c->tree,"leadingJetPt",15,0,300,trackSelection,TCut("pPt>0")&&genParticleCut);

   hEffleadingJetPt->SetAxisRange(0.6,0.9,"Y");
   hEffleadingJetPt->Draw();

   gg->SetMarkerStyle(24);
   gg2->SetMarkerStyle(24);
   gg3->SetMarkerStyle(24);
   gg->Draw("p same");
   gg2->SetLineColor(2);
   gg2->SetMarkerColor(2);
   gg2->Draw("p same");
   
   gg3->SetLineColor(4);
   gg3->SetMarkerColor(4);
   gg3->Draw("p same");
   
   TLegend *legleadingJetPt2 = myLegend(0.5,0.7,0.9,0.9);
   legleadingJetPt2->AddEntry(gg,"#hat{p}_{T} 120 GeV/c, No selection","");
   legleadingJetPt2->AddEntry(gg,"simTrack p_{T} > 20 GeV/c","pl");
   legleadingJetPt2->AddEntry(gg2,"simTrack p_{T} > 8 GeV/c","pl");
   legleadingJetPt2->AddEntry(gg3,"simTrack p_{T} > 0.5 GeV/c","pl");
   
   legleadingJetPt2->Draw();

   cleadingJetPtNoDijet->SaveAs("effFig/EffVsleadingJetPt-NoDijetSelection.gif");
   cleadingJetPtNoDijet->SaveAs("effFig/EffVsleadingJetPt-NoDijetSelection.C");

   
}
