#include "utilities.h"
#include "HiForest/hiForest.h"

void photonEff(char *infname = "/d102/yjlee/hiForest2/pPb/pPb.root")
{
   HiForest *c = new HiForest(infname);
   c->tree->AddFriend("ak5CaloJetAnalyzer/t");

   TCanvas *c1 = new TCanvas ("c1","",600,600);
   
   TH1F *hEff20 = new TH1F("hEff20","",10,0,50);
   
   makeHistTitle(hEff20,"","Leading Photon Candidate p_{T} (GeV/c)","Trigger Efficiency");
   
   TGraphAsymmErrors *g = getEfficiency(c->tree,"Max$(pt*(isEB&&sigmaIetaIeta<0.011&&(cr4+cc4+ct4PtCut20)<5))",10,0,50,"HLT_PAPhoton10_CaloIdVL_v1","phiEcalRecHitSpikeFilter&&pHBHENoiseFilter&&phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&(HLT_PAZeroBiasPixel_SingleTrack_v1||HLT_PAL1Tech53_MB_SingleTrack_v1)");
   TGraphAsymmErrors *g2 = getEfficiency(c->tree,"Max$(pt*(isEB&&sigmaIetaIeta<0.011&&(cr4+cc4+ct4PtCut20)<5))",10,0,50,"HLT_PAPhoton15_CaloIdVL_v1","phiEcalRecHitSpikeFilter&&pHBHENoiseFilter&&phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&(HLT_PAZeroBiasPixel_SingleTrack_v1||HLT_PAL1Tech53_MB_SingleTrack_v1)");
   TGraphAsymmErrors *g3 = getEfficiency(c->tree,"Max$(pt*(isEB&&sigmaIetaIeta<0.011&&(cr4+cc4+ct4PtCut20)<5))",10,0,50,"HLT_PAPhoton20_CaloIdVL_v1","phiEcalRecHitSpikeFilter&&pHBHENoiseFilter&&phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&(HLT_PAZeroBiasPixel_SingleTrack_v1||HLT_PAL1Tech53_MB_SingleTrack_v1)");

   hEff20->SetAxisRange(0,1.4,"Y");
   hEff20->Draw();
   
   g->Draw("p same");
   g2->SetLineColor(2);
   g2->SetMarkerColor(2);
   g3->SetLineColor(4);
   g3->SetMarkerColor(4);
   g2->Draw("p same");
   g3->Draw("p same");

   TLegend *leg = myLegend(0.3,0.7,0.9,0.9);
   leg->AddEntry(g,"HLT_PAPhoton10_CaloIdVL","pl");
   leg->AddEntry(g2,"HLT_PAPhoton15_CaloIdVL","pl");
   leg->AddEntry(g3,"HLT_PAPhoton20_CaloIdVL","pl");
   leg->Draw();
   TLine *l = new TLine(0,1,50,1);
   l->SetLineStyle(2);
   l->Draw();
}
