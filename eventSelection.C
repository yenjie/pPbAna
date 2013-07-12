#include "utilities.h"
#include "HiForest/hiForest.h"

void eventSelection(char *infname = "/d102/yjlee/hiForest2/pPb/pPb.root")
{
   HiForest *c = new HiForest(infname,"",cPPb);
   c->tree->AddFriend("ak5CaloJetAnalyzer/t");

   TCanvas *c1 = new TCanvas ("c1","",600,600);
   
   TH1F *hEff20 = new TH1F("hEff20","",100,0,200);
   
   makeHistTitle(hEff20,"","N","Selection Efficiency");
   
   TGraphAsymmErrors *g = getEfficiency(c->tree,"Sum$(trkPt>0.4&&abs(trkEta)<2.4&&highPurity&&abs(trkDz1/trkDzError1)<3&&abs(trkDz1/trkDzError1)<3&&trkPtError/trkPt<0.1)",100,0,200,"phltPixelClusterShapeFilter","Run==202792&&lumi>=317&&lumi<=1014&&HLT_PAZeroBiasPixel_SingleTrack_v1");
   TGraphAsymmErrors *g2 = getEfficiency(c->tree,"Sum$(trkPt>0.4&&abs(trkEta)<2.4&&highPurity&&abs(trkDz1/trkDzError1)<3&&abs(trkDz1/trkDzError1)<3&&trkPtError/trkPt<0.1)",100,0,200,"phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1","Run==202792&&lumi>=317&&lumi<=1014&&HLT_PAZeroBiasPixel_SingleTrack_v1");
   TGraphAsymmErrors *g3 = getEfficiency(c->tree,"Sum$(trkPt>0.4&&abs(trkEta)<2.4&&highPurity&&abs(trkDz1/trkDzError1)<3&&abs(trkDz1/trkDzError1)<3&&trkPtError/trkPt<0.1)",100,0,200,"phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&pprimaryvertexFilter","Run==202792&&lumi>=317&&lumi<=1014&&HLT_PAZeroBiasPixel_SingleTrack_v1");

   hEff20->SetAxisRange(0,1.4,"Y");
   hEff20->Draw();
   
   g->Draw("p same");
   g2->SetLineColor(2);
   g2->SetMarkerColor(2);
   g3->SetLineColor(4);
   g3->SetMarkerColor(4);
   g2->Draw("p same");
   g3->Draw("p same");

   TLegend *leg = myLegend(0.2231544,0.4125874,0.9110738,0.6118881);
   leg->AddEntry(g,"Vertex Compatibility","pl");
   leg->AddEntry(g2,"Vertex Compatibility+HF1x1","pl");
   leg->AddEntry(g3,"Vertex Compatibility+HF1x1+Primary Vertex","pl");
   leg->Draw();
   TLine *l = new TLine(0,1,200,1);
   l->SetLineStyle(2);
   l->Draw();
}
