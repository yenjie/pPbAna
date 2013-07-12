#include "utilities.h"
#include <TStyle.h>
void normalize(TH1F *hData,bool setError=0)
{
   int bin = hData->FindBin(0.2999);
   double sum = 0,sumErr =0;
   for (int j=1;j<=bin;j++) {
      sum += hData->GetBinContent(j);
      if (setError) hData->SetBinError(j,sqrt(hData->GetBinContent(j)));
   }
   hData->Scale(1./sum);
   divideBinWidth(hData);
}

void plotJS()
{
   gStyle->SetErrorX(0);

   const int nBin = 4;
   TFile *inf[nBin];
/*
   inf[0] = new TFile("jetShape2/output-JS_pthat100_dijetSelection_genp-0-4.root");
   inf[1] = new TFile("jetShape2/output-JS_pthat100_dijetSelection_genp-4-12.root");
   inf[2] = new TFile("jetShape2/output-JS_pthat100_dijetSelection_genp-12-20.root");
   inf[3] = new TFile("jetShape2/output-JS_pthat100_dijetSelection_genp-20-40.root");
*/
   inf[0] = new TFile("jetShape3/output-JS_data-0-4.root");
   inf[1] = new TFile("jetShape3/output-JS_data-4-12.root");
   inf[2] = new TFile("jetShape3/output-JS_data-12-20.root");
   inf[3] = new TFile("jetShape3/output-JS_data-20-40.root");

//   inf[4] = new TFile("jetShape2/output-JS_pthat100_dijetSelection_genp-20-40.root");
//   inf[4] = new TFile("jetShape2/output-JS_ppdata-0-40.root");
   char *title[nBin] = {"0-10%", "10-30%", "30-50%", "50-100%"};

   int color[nBin] = {kBlack, kBlue, kRed, kGreen+2};

   TH1F *hData[nBin];
   TH1F *hDataGen[nBin];
   TH1F *hDataRatio[nBin];
   TH1F *hDataGenRatio[nBin];
   TH1F *hDataRecoRatio[nBin];
   TCanvas *c = new TCanvas("c","",600,600);
   c->SetLogy();
   TH1F *hJSTmp = new TH1F("hJSTmp","",100,0,0.3);
   hJSTmp->SetAxisRange(0.005,20,"Y");
   makeHistTitle(hJSTmp,"","#Delta R","Differential Jet shape");
   hJSTmp->Draw();
  
   TLegend *legJS = myLegend(0.3,0.2,0.7,0.5);
   
   for (int i=0;i<nBin;i++) {
      if (inf[i]->IsZombie()) continue;
      hDataGen[i]= (TH1F*)inf[i]->Get("hGenJSEtaSub_MergedGeneral");
      hData[i]= (TH1F*)inf[i]->Get("hCorrectedJSEtaSub_MergedGeneral");
      normalize(hDataGen[i]);
      hDataGen[i]->SetLineColor(color[i]);
      hDataGen[i]->SetMarkerColor(color[i]);
      hDataGen[i]->Draw("same");
      normalize(hData[i]);
      hData[i]->SetLineColor(color[i]);
      hData[i]->SetMarkerColor(color[i]);
      hData[i]->Draw("p same");
      legJS->AddEntry(hData[i],Form("Reco JS %s",title[i]),"p");
      legJS->AddEntry(hDataGen[i],Form("Gen JS %s",title[i]),"l");
   }      

   legJS->Draw();
   
   TCanvas *cRatio = new TCanvas("cRatio","",600,600);
   c->SetLogy();
   TH1F *hJSRatioTmp = new TH1F("hJSRatioTmp","",100,0,0.3);
   makeHistTitle(hJSRatioTmp,"","#Delta R","Ratio");
   hJSRatioTmp->Draw();
   TLegend *legJSRatio = myLegend(0.2,0.2,0.7,0.5);
   hJSRatioTmp->SetAxisRange(0.6,1.35,"Y");
   for (int i=0;i<nBin;i++) {
      if (inf[i]->IsZombie()) continue;
      hDataRatio[i] = (TH1F*)hData[i]->Clone();
      hDataRatio[i]->Divide(hDataGen[i]);
      hDataRatio[i]->Draw("p same");
      legJSRatio->AddEntry(hData[i],Form("Reco / Gen JS %s",title[i]),"p");
   }
   TLine *l = new TLine(0,1,0.3,1);
   l->SetLineStyle(2);
   l->Draw();
   legJSRatio->Draw();
      

   TCanvas *cGenRatio = new TCanvas("cGenRatio","Gen / Gen (50-100%)",600,600);
   c->SetLogy();
   TH1F *hJSGenRatioTmp = new TH1F("hJSGenRatioTmp","",100,0,0.3);
   hJSGenRatioTmp->SetAxisRange(0.6,1.35,"Y");
   makeHistTitle(hJSGenRatioTmp,"","#Delta R","Ratio");
   TLegend *legJSGenRatio = myLegend(0.2,0.2,0.7,0.5);
   hJSGenRatioTmp->Draw();
   for (int i=0;i<nBin;i++) {
      if (inf[i]->IsZombie()) continue;
      hDataGenRatio[i] = (TH1F*)hDataGen[i]->Clone();
      hDataGenRatio[i]->Divide(hDataGen[nBin-1]);
      hDataGenRatio[i]->Draw("hist same");
      legJSGenRatio->AddEntry(hDataGenRatio[i],Form("Gen %s / Gen %s",title[i],title[nBin-1]),"l");
   }   
   legJSGenRatio->Draw();
   l->Draw();


   TCanvas *cRecoRatio = new TCanvas("cRecoRatio","Reco / Reco (50-100%)",600,600);
   c->SetLogy();
   TH1F *hJSRecoRatioTmp = new TH1F("hJSRecoRatioTmp","",100,0,0.3);
   hJSRecoRatioTmp->SetAxisRange(0.6,1.5,"Y");
   TLegend *legJSRecoRatio = myLegend(0.2,0.2,0.7,0.5);
   makeHistTitle(hJSRecoRatioTmp,"","#Delta R","Ratio");
   hJSRecoRatioTmp->Draw();
   for (int i=0;i<nBin;i++) {
      if (inf[i]->IsZombie()) continue;
      hDataRecoRatio[i] = (TH1F*)hData[i]->Clone();
      hDataRecoRatio[i]->Divide(hData[nBin-1]);
      hDataRecoRatio[i]->Draw("same");
      legJSRecoRatio->AddEntry(hDataRecoRatio[i],Form("Reco %s / Reco %s",title[i],title[nBin-1]),"l");
   }   
   legJSRecoRatio->Draw();
   l->Draw();

   TCanvas *cCentRatio = new TCanvas("cRecoRatio","Reco / Reco (50-100%)",600,600);
   c->SetLogy();
   TH1F *hJSRecoRatioTmpCent0 = new TH1F("hJSRecoRatioTmpCent0","",100,0,0.3);
   hJSRecoRatioTmpCent0->SetAxisRange(0.6,1.5,"Y");
   hJSRecoRatioTmpCent0->Draw();
   makeHistTitle(hJSRecoRatioTmpCent0,"","#Delta R","Jet shape ratio");
   
   double sysJetAxis[6] = {0.03,0.09,0.02,0.06,0.04,0.07};
   double sysEff[6]     = {0.02,0.05,0.05,0.05,0.05,0.1};
   
   hDataRecoRatio[0]->Draw("p same");
   for (int i=1;i<=6;i++)
   {
      double val = hDataRecoRatio[0]->GetBinContent(i);
      double sys = sqrt(sysJetAxis[i-1]*sysJetAxis[i-1]+sysEff[i-1]*sysEff[i-1]);
      TBox *b = new TBox(i*0.05-0.025-0.01,val*(1-sys),i*0.05-0.025+0.01,val*(1+sys));
      b->Draw();
   }   
   l->Draw();
   hDataRecoRatio[0]->Draw("p same");
   TLegend *legJSRecoRatioCent0 = myLegend(0.2,0.2,0.7,0.5);
   legJSRecoRatioCent0->AddEntry(hDataRecoRatio[0],"0-10% / 50-100%", "p");
   legJSRecoRatioCent0->Draw();
   TLegend *legCMS = myLegend(0.13,0.82,0.62,0.95);
   legCMS->AddEntry(hDataRecoRatio[0],"CMS Preliminary", "");
   legCMS->Draw();
}
