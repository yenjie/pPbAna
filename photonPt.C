#include "utilities.h"
#include "HiForest/hiForest.h"

void photonPt(char *infname = "/d102/yjlee/hiForest2/pPb/pPb.root")
{
   HiForest *c = new HiForest(infname,"",cPPb);

   TCanvas *c1 = new TCanvas ("c1","",600,600);
   c1->SetLogy();
   
   TH1F *hEff20 = new TH1F("hEff20","",8,10,50);
   
   makeHistTitle(hEff20,"","Photon Candidate p_{T} (GeV/c)","Entries");
   
   c->tree->Draw("pt>>hEff20","isEB&&sigmaIetaIeta<0.011&&(cr4+cc4+ct4PtCut20)<5&&pHBHENoiseFilter&&phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&(HLT_PAZeroBiasPixel_SingleTrack_v1||HLT_PAL1Tech53_MB_SingleTrack_v1)");
   hEff20->Sumw2();

   TLegend *leg1 = myLegend(0.5,0.8,0.93,0.99);
   leg1->AddEntry(hEff20,"CMS Preliminary","");
   leg1->Draw();

   TLegend *leg = myLegend(0.45,0.55,0.9,0.8);
   leg->AddEntry(hEff20,"p+Pb data #sqrt{s_{NN}} = 5.02 TeV","pl");
   leg->AddEntry(hEff20,"|#eta|<1.479","");
   leg->Draw();

   TLine *l = new TLine(0,1,100,1);
   l->SetLineStyle(2);
 //  l->Draw();
}
