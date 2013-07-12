#include "HiForest/hiForest.h"

#include "utilities.h"

void plotTrack(char *infname)
{
   TCanvas *c = new TCanvas("c","",600,600);
   HiForest *h = new HiForest(infname,"",cPPb);
   
   TH1D *hEta = new TH1D("hEta","",100,-3,3);
   TH1D *hEtaHighPurity = new TH1D("hEtaHighPurity","",100,-3,3);
   
   h->tree->Draw("trkEta>>hEta","Run==202792&&lumi>=317&&lumi<=1014&&phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&pprimaryvertexFilter&&abs(vz[1])<15&&HLT_PAZeroBiasPixel_SingleTrack_v1");
   h->tree->Draw("trkEta>>hEtaHighPurity","Run==202792&&lumi>=317&&lumi<=1014&&phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&pprimaryvertexFilter&&abs(vz[1])<15&&HLT_PAZeroBiasPixel_SingleTrack_v1&&highPurity&&abs(trkDz1/trkDzError1)<3&&abs(trkDz1/trkDzError1)<3&&trkPtError/trkPt<0.1");

   hEta->SetLineColor(2);
   hEta->Draw();
   hEtaHighPurity->Draw("same");
   makeHistTitle(hEta,"","Track #eta","Entries",2);
   
   TLegend *leg = myLegend(0.36,0.20,0.72,0.39);
   
   leg->AddEntry(hEta,"All tracks","l");
   leg->AddEntry(hEtaHighPurity,"High purity tracks","l");
   leg->Draw();

   TCanvas *c2 = new TCanvas("c2","",600,600);

   TH1D *hPhi = new TH1D("hPhi","",100,-3.1415927,3.1415927);
   TH1D *hPhiHighPurity = new TH1D("hPhiHighPurity","",100,-3.1415927,3.1415927);
   
   h->tree->Draw("trkPhi>>hPhi","Run==202792&&lumi>=317&&lumi<=1014&&phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&pprimaryvertexFilter&&abs(vz[1])<15&&HLT_PAZeroBiasPixel_SingleTrack_v1");
   h->tree->Draw("trkPhi>>hPhiHighPurity","Run==202792&&lumi>=317&&lumi<=1014&&phltPixelClusterShapeFilter&&phfPosFilter1&&phfNegFilter1&&pprimaryvertexFilter&&abs(vz[1])<15&&HLT_PAZeroBiasPixel_SingleTrack_v1&&highPurity&&abs(trkDz1/trkDzError1)<3&&abs(trkDz1/trkDzError1)<3&&trkPtError/trkPt<0.1");

   hPhi->SetLineColor(2);
   hPhi->SetAxisRange(0,hPhi->GetMaximum()*1.5,"Y");
   hPhi->Draw();
   hPhiHighPurity->Draw("same");
   makeHistTitle(hPhi,"","Track #phi","Entries",2);
   
   TLegend *legPhi = myLegend(0.36,0.20,0.72,0.39);
   
   legPhi->AddEntry(hPhi,"All tracks","l");
   legPhi->AddEntry(hPhiHighPurity,"High purity tracks","l");
   legPhi->Draw();
}
