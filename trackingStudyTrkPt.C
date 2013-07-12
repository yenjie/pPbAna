#include "utilities.h"
#include "HiForest/hiForest.h"

void trackingStudyTrkPt(char *infname = "skim_jet_ptha120.root")
{
   HiForest *c = new HiForest(infname);
   c->tree->AddFriend("nt");

   TCut trackSelection = "(mtrkQual||mtrkAlgo<4)&&pNRec>0";
//   TCut dijetSelection = "leadingJetPt>120&&subleadingJetPt>50&&acos(cos(leadingJetPhi-subleadingJetPhi))>7./8.*3.141593";
   TCut genParticleCut = "abs(pEta)<2.4";
   
   TCanvas *cpPt = new TCanvas ("cpPt","",600,600);
   
   TH1F *hEffpPt = new TH1F("hEffpPt","",10,0,100);
   
   makeHistTitle(hEffpPt,"","SimTrack p_{T} (GeV/c)","Tracking Efficiency");
   
   TGraphAsymmErrors *g = getEfficiency(c->tree,"pPt",10,0,100,trackSelection,TCut("50<leadingJetPt&&leadingJetPt<100")&&genParticleCut);
   TGraphAsymmErrors *g2 = getEfficiency(c->tree,"pPt",10,0,100,trackSelection,TCut("100<leadingJetPt&&leadingJetPt<120")&&genParticleCut);
   TGraphAsymmErrors *g3 = getEfficiency(c->tree,"pPt",10,0,100,trackSelection,TCut("120<leadingJetPt&&leadingJetPt<200")&&genParticleCut);

   hEffpPt->SetAxisRange(0.,1,"Y");
   hEffpPt->Draw();
   
   g->Draw("p same");
   g2->SetLineColor(2);
   g2->SetMarkerColor(2);
   g2->Draw("p same");
   
   g3->SetLineColor(4);
   g3->SetMarkerColor(4);
   g3->Draw("p same");

   TLegend *legpPt = myLegend(0.5,0.7,0.9,0.9);
   legpPt->AddEntry(g,"#hat{p}_{T} 120 GeV/c, No selection","");
   legpPt->AddEntry(g,"50 < leadingJetPt < 100 GeV/c","pl");
   legpPt->AddEntry(g2,"100 < leadingJetPt < 120 GeV/c","pl");
   legpPt->AddEntry(g3,"120 < leadingJetPt < 200 GeV/c","pl");
   
   legpPt->Draw();
   cpPt->SaveAs("effFig/EffVsTrackPt.gif");
   cpPt->SaveAs("effFig/EffVsTrackPt.C");

   
}
