#include "utilities.h"
#include "HiForest/hiForest.h"

void trackingStudyAroundJet(char *infname = "skim_jet_ptha120.root")
{
   HiForest *c = new HiForest(infname);
   c->tree->AddFriend("nt");

   TCut trackSelection = "(mtrkQual||mtrkAlgo<4)&&pNRec>0";
   TCut dijetSelection = "leadingJetPt>120&&subleadingJetPt>50&&acos(cos(leadingJetPhi-subleadingJetPhi))>7./8.*3.141593";
   TCut genParticleCut = "abs(pEta)<2.4";
   
   TCanvas *cleadingJetPt = new TCanvas ("cleadingJetPt","",600,600);
   
   TH1F *hEffleadingJetPt = new TH1F("hEffleadingJetPt","",10,0,0.3);
   
   makeHistTitle(hEffleadingJetPt,"","Leading Jet p_{T} (GeV/c)","Tracking Efficiency");
   
   TGraphAsymmErrors *g = getEfficiency(c->tree,"acos(cos(pPhi-leadingJetPhi))",10,0,0.3,trackSelection,TCut("pPt>20")&&dijetSelection&&genParticleCut);
   TGraphAsymmErrors *g2 = getEfficiency(c->tree,"acos(cos(pPhi-leadingJetPhi))",10,0,0.3,trackSelection,TCut("pPt>8")&&dijetSelection&&genParticleCut);
   TGraphAsymmErrors *g3 = getEfficiency(c->tree,"acos(cos(pPhi-leadingJetPhi))",10,0,0.3,trackSelection,TCut("pPt>0")&&dijetSelection&&genParticleCut);

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
   legleadingJetPt->AddEntry(g,"#hat{p}_{T} 170 GeV/c, Dijet selection","");
   legleadingJetPt->AddEntry(g,"simTrack p_{T} > 20 GeV/c","pl");
   legleadingJetPt->AddEntry(g2,"simTrack p_{T} > 8 GeV/c","pl");
   legleadingJetPt->AddEntry(g3,"simTrack p_{T} > 0 GeV/c","pl");
   
   legleadingJetPt->Draw();
   cleadingJetPt->SaveAs("effFig/EffVsleadingJetDphi-DijetSelection.gif");
   cleadingJetPt->SaveAs("effFig/EffVsleadingJetDphi-DijetSelection.C");

   
}
