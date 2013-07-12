#include "utilities.h"
#include "HiForest/hiForest.h"

void jetRcp(char *infname = "/d102/yjlee/hiForest2/pPb/pPb.root")
{
   HiForest *c = new HiForest(infname,"",cPPb);

   TCanvas *c1 = new TCanvas ("c1","",600,600);

   Float_t bin[10] ={20,25,30,35,40,50,60,80,100,200};
   
   TH1F *hJetCent = new TH1F("hJetCent","",9,bin);
   TH1F *hJetPeri = new TH1F("hJetPeri","",9,bin);
   
   makeHistTitle(hJetCent,"","AK5 Calo Jet p_{T} (GeV/c)","Entries");
   
   c->tree->Draw("ak5Calo.jtpt>>hJetCent","abs(ak5Calo.jteta)<2&&pHBHENoiseFilter&&phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&HLT_PAZeroBiasPixel_SingleTrack_v1&&hiHFplus>45.8");
   hJetCent->Sumw2();
   cout <<c->tree->GetEntries("pHBHENoiseFilter&&phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&HLT_PAak5CaloJet40_NoJetID_v1&&HLT_PAZeroBiasPixel_SingleTrack_v1&&hiHFplus>45.8")<<endl;
//   hJetCent->Scale(1./c->tree->GetEntries("pHBHENoiseFilter&&phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&HLT_PAak5CaloJet20_NoJetID_v1&&hiHFplus>45.8"));
   
   c->tree->Draw("ak5Calo.jtpt>>hJetPeri","abs(ak5Calo.jteta)<2&&pHBHENoiseFilter&&phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&HLT_PAZeroBiasPixel_SingleTrack_v1&&hiHFplus>2.53&&hiHFplus<15.7");
   hJetPeri->Sumw2();
   cout <<c->tree->GetEntries("pHBHENoiseFilter&&phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&HLT_PAak5CaloJet40_NoJetID_v1&&HLT_PAZeroBiasPixel_SingleTrack_v1&&hiHFplus>2.53&&hiHFplus<15.7")<<endl;
//   hJetPeri->Scale(1./c->tree->GetEntries("pHBHENoiseFilter&&phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&HLT_PAak5CaloJet20_NoJetID_v1&&hiHFplus<3.53&&hiHFplus<15.7"));


   hJetCent->Divide(hJetPeri);
   hJetCent->Scale(1./4.82*4.);
   hJetCent->Draw();
   TLegend *leg1 = myLegend(0.5,0.8,0.93,0.99);
   leg1->AddEntry(hJetCent,"CMS Preliminary","");
   leg1->Draw();

   TLegend *leg = myLegend(0.5,0.63,0.95,0.88);
   leg->AddEntry(hJetCent,"p+Pb data #sqrt{s_{NN}} = 5.02 TeV","pl");
   leg->AddEntry(hJetCent,"Jet |#eta|<2","");
   leg->Draw();

   TLine *l = new TLine(20,1,200,1);
   l->SetLineStyle(2);
   l->Draw();
}
