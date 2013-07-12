#include "utilities.h"
#include "HiForest/hiForest.h"

void trackingStudyCentBin(char *infname = "skim_jet_ptha120.root")
{
   HiForest *c = new HiForest(infname);
   c->tree->AddFriend("nt");

   TCut trackSelection = "(mtrkQual||mtrkAlgo<4)&&pNRec>0";
   TCut dijetSelection = "leadingJetPt>120&&subleadingJetPt>50&&acos(cos(leadingJetPhi-subleadingJetPhi))>7./8.*3.141593";
   TCut genParticleCut = "abs(pEta)<2.4";
   
   TCanvas *chiBin = new TCanvas ("chiBin","",600,600);
   
   TH1F *hEffhiBin = new TH1F("hEffhiBin","",40,0,40);
   
   makeHistTitle(hEffhiBin,"","Centrality Bin","Tracking Efficiency");
   
   TGraphAsymmErrors *g = getEfficiency(c->tree,"hiBin",40,0,40,trackSelection,TCut("pPt>20")&&dijetSelection&&genParticleCut);
   TGraphAsymmErrors *g2 = getEfficiency(c->tree,"hiBin",40,0,40,trackSelection,TCut("pPt>8")&&dijetSelection&&genParticleCut);
   TGraphAsymmErrors *g3 = getEfficiency(c->tree,"hiBin",40,0,40,trackSelection,TCut("pPt>0")&&dijetSelection&&genParticleCut);

   hEffhiBin->SetAxisRange(0.5,0.8,"Y");
   hEffhiBin->Draw();
   
   g->Draw("p same");
   g2->SetLineColor(2);
   g2->SetMarkerColor(2);
   g2->Draw("p same");
   
   g3->SetLineColor(4);
   g3->SetMarkerColor(4);
   g3->Draw("p same");

   TLegend *leghiBin = myLegend(0.5,0.7,0.9,0.9);
   leghiBin->AddEntry(g,"#hat{p}_{T} 120 GeV/c, Dijet selection","");
   leghiBin->AddEntry(g,"simTrack p_{T} > 20 GeV/c","pl");
   leghiBin->AddEntry(g2,"simTrack p_{T} > 8 GeV/c","pl");
   leghiBin->AddEntry(g3,"simTrack p_{T} > 0.5 GeV/c","pl");
   
   leghiBin->Draw();
   chiBin->SaveAs("effFig/EffVshiBin-DijetSelection.gif");
   chiBin->SaveAs("effFig/EffVshiBin-DijetSelection.C");

   TCanvas *chiBinNoDijet = new TCanvas ("chiBinNoDijet","",600,600);

   TGraphAsymmErrors *gg = getEfficiency(c->tree,"hiBin",40,0,40,trackSelection,TCut("pPt>20")&&genParticleCut);
   TGraphAsymmErrors *gg2 = getEfficiency(c->tree,"hiBin",40,0,40,trackSelection,TCut("pPt>8")&&genParticleCut);
   TGraphAsymmErrors *gg3 = getEfficiency(c->tree,"hiBin",40,0,40,trackSelection,TCut("pPt>0")&&genParticleCut);

   hEffhiBin->SetAxisRange(0.5,0.8,"Y");
   hEffhiBin->Draw();

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
   
   TLegend *leghiBin2 = myLegend(0.5,0.7,0.9,0.9);
   leghiBin2->AddEntry(gg,"#hat{p}_{T} 120 GeV/c, No selection","");
   leghiBin2->AddEntry(gg,"simTrack p_{T} > 20 GeV/c","pl");
   leghiBin2->AddEntry(gg2,"simTrack p_{T} > 8 GeV/c","pl");
   leghiBin2->AddEntry(gg3,"simTrack p_{T} > 0.5 GeV/c","pl");
   
   leghiBin2->Draw();

   chiBinNoDijet->SaveAs("effFig/EffVshiBin-NoDijetSelection.gif");
   chiBinNoDijet->SaveAs("effFig/EffVshiBin-NoDijetSelection.C");

   
}
