#include "utilities.h"
#include "HiForest/hiForest.h"

void trackingStudyTrkEta(char *infname = "skim_jet_pthat80Sim05.root")
{
   HiForest *c = new HiForest(infname);
   c->tree->AddFriend("nt");

   TCut trackSelection = "(mtrkQual||mtrkAlgo<4)&&pNRec>0";
   TCut genParticleCut = "abs(pEta)<2.8&&pPt>0";

   TCut weightCent = "1";//"(0.64661/(0.64661+0.00279303*hiBin+(-3.89902e-5)*hiBin*hiBin)*(hiBin<34)+(hiBin>=34)*0.64661/(-1.38822+0.123182*hiBin-0.0018135*hiBin*hiBin))";

   TCut weight = "1";//"1./(0.757356-0.030163*pEta-0.0769554*pEta**2+0.0176546*pEta**3+0.0200791*pEta**4-0.00256533*pEta**5-0.00304287*pEta**6)";    
   TCanvas *cpPt = new TCanvas ("cpPt","",600,600);
   
   TH1F *hEffpPt = new TH1F("hEffpPt","",28,-2.8,2.8);
   
   makeHistTitle(hEffpPt,"","SimTrack #eta","Tracking Efficiency");
   
   TGraphAsymmErrors *g = getEfficiency(c->tree,"pEta",28,-2.8,2.8,trackSelection,TCut("pPt>0.5")&&genParticleCut,weight*weightCent);
   TGraphAsymmErrors *g1 = getEfficiency(c->tree,"pEta",28,-2.8,2.8,trackSelection,TCut("pPt>0.5&&pPt<1")&&genParticleCut,weight*weightCent);
   TGraphAsymmErrors *g2 = getEfficiency(c->tree,"pEta",28,-2.8,2.8,trackSelection,TCut("pPt>1&&pPt<2")&&genParticleCut,weight*weightCent);
   TGraphAsymmErrors *g3 = getEfficiency(c->tree,"pEta",28,-2.8,2.8,trackSelection,TCut("pPt>2&&pPt<4")&&genParticleCut,weight*weightCent);
   TGraphAsymmErrors *g4 = getEfficiency(c->tree,"pEta",28,-2.8,2.8,trackSelection,TCut("pPt>4&&pPt<8")&&genParticleCut,weight*weightCent);
   TGraphAsymmErrors *g5 = getEfficiency(c->tree,"pEta",28,-2.8,2.8,trackSelection,TCut("pPt>8")&&genParticleCut,weight*weightCent);

   hEffpPt->SetAxisRange(0.,1,"Y");
   hEffpPt->Draw();
   
   g->Draw("p same");
   g1->SetLineColor(2);
   g1->SetMarkerColor(2);
   g1->Draw("p same");
   g2->SetLineColor(3);
   g2->SetMarkerColor(3);
   g2->Draw("p same");
   
   g3->SetLineColor(4);
   g3->SetMarkerColor(4);
   g3->Draw("p same");

   g4->SetLineColor(46);
   g4->SetMarkerColor(46);
   g4->Draw("p same");

   g5->SetLineColor(kOrange);
   g5->SetMarkerColor(kOrange);
   g5->Draw("p same");

   TLegend *legpPt = myLegend(0.2,0.7,0.6,0.9);
   legpPt->AddEntry(g,"#hat{p}_{T} 80 GeV/c, No selection","");
   legpPt->AddEntry(g,"Simtrack p_{T} > 0.5 GeV/c","pl");
   legpPt->AddEntry(g1,"Simtrack 0.5 > p_{T} > 1 GeV/c","pl");
   legpPt->AddEntry(g2,"Simtrack 1 > p_{T} > 2 GeV/c","pl");
   legpPt->AddEntry(g3,"Simtrack 2 > p_{T} > 4 GeV/c","pl");
   legpPt->AddEntry(g4,"Simtrack 4 > p_{T} > 8 GeV/c","pl");
   legpPt->AddEntry(g5,"Simtrack p_{T} > 8 GeV/c","pl");
   
   legpPt->Draw();
   cpPt->SaveAs("effFig/EffVsTrackPt.gif");
   cpPt->SaveAs("effFig/EffVsTrackPt.C");

   g->Fit("pol6")   ;
}
