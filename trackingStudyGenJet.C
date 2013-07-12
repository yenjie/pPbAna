#include "utilities.h"
#include "HiForest/hiForest.h"

void trackingStudyGenJet(char *infname = "skim_jet_ptha120.root")
{
   HiForest *c = new HiForest(infname);
   c->tree->AddFriend("nt");

   TCut trackSelection = "(mtrkQual||mtrkAlgo<4)&&pNRec>0";
   TCut dijetSelection = "leadingJetPt>120&&subleadingJetPt>50&&acos(cos(leadingJetPhi-subleadingJetPhi))>7./8.*3.141593";
   TCut genParticleCut = "abs(pEta)<2.4";
   TCut weightCent = "(0.64661/(0.64661+0.00279303*hiBin+(-3.89902e-5)*hiBin*hiBin)*(hiBin<34)+(hiBin>=34)*0.64661/(-1.38822+0.123182*hiBin-0.0018135*hiBin*hiBin))";
TCut weight = "1./(0.757356-0.030163*pEta-0.0769554*pEta**2+0.0176546*pEta**3+0.0200791*pEta**4-0.00256533*pEta**5-0.00304287*pEta**6)";   TCanvas *cleadingJetPt = new TCanvas ("cleadingJetPt","",600,600);
   
   TH1F *hEffleadingJetPt = new TH1F("hEffleadingJetPt","",15,0,300);
   
   makeHistTitle(hEffleadingJetPt,"","Gen Leading Jet p_{T} (GeV/c)","Tracking Efficiency");
   
   TGraphAsymmErrors *g = getEfficiency(c->tree,"Max$(genpt*(abs(geneta)<2))",15,0,300,trackSelection,TCut("pPt>20")&&dijetSelection&&genParticleCut,weight*weightCent);
   TGraphAsymmErrors *g2 = getEfficiency(c->tree,"Max$(genpt*(abs(geneta)<2))",15,0,300,trackSelection,TCut("pPt>8")&&dijetSelection&&genParticleCut,weight*weightCent);
   TGraphAsymmErrors *g3 = getEfficiency(c->tree,"Max$(genpt*(abs(geneta)<2))",15,0,300,trackSelection,TCut("pPt>0")&&dijetSelection&&genParticleCut,weight*weightCent);

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
   cleadingJetPt->SaveAs("effFig/EffVsleadingGenJetPt-DijetSelection.gif");
   cleadingJetPt->SaveAs("effFig/EffVsleadingGenJetPt-DijetSelection.C");

   TCanvas *cleadingJetPtNoDijet = new TCanvas ("cleadingJetPtNoDijet","",600,600);

   TGraphAsymmErrors *gg = getEfficiency(c->tree,"Max$(genpt*(abs(geneta)<2))",15,0,300,trackSelection,TCut("pPt>20")&&genParticleCut,weight*weightCent);
   TGraphAsymmErrors *gg2 = getEfficiency(c->tree,"Max$(genpt*(abs(geneta)<2))",15,0,300,trackSelection,TCut("pPt>8")&&genParticleCut,weight*weightCent);
   TGraphAsymmErrors *gg3 = getEfficiency(c->tree,"Max$(genpt*(abs(geneta)<2))",15,0,300,trackSelection,TCut("pPt>0")&&genParticleCut,weight*weightCent);

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

   cleadingJetPtNoDijet->SaveAs("effFig/EffVsleadingGenJetPt-NoDijetSelection.gif");
   cleadingJetPtNoDijet->SaveAs("effFig/EffVsleadingGenJetPt-NoDijetSelection.C");

   
}
