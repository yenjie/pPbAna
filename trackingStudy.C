#include "utilities.h"
#include "HiForest/hiForest.h"

void trackingStudy(char *infname = "skim_jet_ptha120.root")
{
   HiForest *c = new HiForest(infname);
   c->tree->AddFriend("nt");

   TCut trackSelection = "(mtrkQual||mtrkAlgo<4)&&pNRec>0";
   TCut dijetSelection = "leadingJetPt>120&&subleadingJetPt>50&&acos(cos(leadingJetPhi-subleadingJetPhi))>7./8.*3.141593";
   TCut genParticleCut = "abs(pEta)<2.4";
   
   TCanvas *cAj = new TCanvas ("cAj","",600,600);
   
   TH1F *hEffAj = new TH1F("hEffAj","",5,0,0.5);
   
   makeHistTitle(hEffAj,"","A_{J}","Tracking Efficiency");
   
   TGraphAsymmErrors *g = getEfficiency(c->tree,"Aj",5,0,0.5,trackSelection,TCut("pPt>20")&&dijetSelection&&genParticleCut);
   TGraphAsymmErrors *g2 = getEfficiency(c->tree,"Aj",5,0,0.5,trackSelection,TCut("pPt>8")&&dijetSelection&&genParticleCut);
   TGraphAsymmErrors *g3 = getEfficiency(c->tree,"Aj",5,0,0.5,trackSelection,TCut("pPt>0")&&dijetSelection&&genParticleCut);

   hEffAj->SetAxisRange(0.5,0.8,"Y");
   hEffAj->Draw();
   
   g->Draw("p same");
   g2->SetLineColor(2);
   g2->SetMarkerColor(2);
   g2->Draw("p same");
   
   g3->SetLineColor(4);
   g3->SetMarkerColor(4);
   g3->Draw("p same");

   TLegend *legAj = myLegend(0.5,0.7,0.9,0.9);
   legAj->AddEntry(g,"#hat{p}_{T} 120 GeV/c, Dijet selection","");
   legAj->AddEntry(g,"simTrack p_{T} > 20 GeV/c","pl");
   legAj->AddEntry(g2,"simTrack p_{T} > 8 GeV/c","pl");
   legAj->AddEntry(g3,"simTrack p_{T} > 0.5 GeV/c","pl");
   
   legAj->Draw();
   cAj->SaveAs("effFig/EffVsAj-DijetSelection.gif");
   cAj->SaveAs("effFig/EffVsAj-DijetSelection.C");

   TCanvas *cAjNoDijet = new TCanvas ("cAjNoDijet","",600,600);

   TGraphAsymmErrors *gg = getEfficiency(c->tree,"Aj",5,0,0.5,trackSelection,TCut("pPt>20")&&genParticleCut);
   TGraphAsymmErrors *gg2 = getEfficiency(c->tree,"Aj",5,0,0.5,trackSelection,TCut("pPt>8")&&genParticleCut);
   TGraphAsymmErrors *gg3 = getEfficiency(c->tree,"Aj",5,0,0.5,trackSelection,TCut("pPt>0")&&genParticleCut);

   hEffAj->SetAxisRange(0.5,0.8,"Y");
   hEffAj->Draw();

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
   
   TLegend *legAj2 = myLegend(0.5,0.7,0.9,0.9);
   legAj2->AddEntry(gg,"#hat{p}_{T} 120 GeV/c, No selection","");
   legAj2->AddEntry(gg,"simTrack p_{T} > 20 GeV/c","pl");
   legAj2->AddEntry(gg2,"simTrack p_{T} > 8 GeV/c","pl");
   legAj2->AddEntry(gg3,"simTrack p_{T} > 0.5 GeV/c","pl");
   
   legAj2->Draw();

   cAjNoDijet->SaveAs("effFig/EffVsAj-NoDijetSelection.gif");
   cAjNoDijet->SaveAs("effFig/EffVsAj-NoDijetSelection.C");

   
}
