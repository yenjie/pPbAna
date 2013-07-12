#include "utilities.h"

TH1D *accumulate(TH1D* h,bool reverse = 0)
{
   TH1D *hAcc = (TH1D*)h->Clone(Form("%s_acc",h->GetName()));
   if (!reverse) {
      for (int i=1;i<=hAcc->GetNbinsX();i++)
      {
         hAcc->SetBinContent(i,hAcc->GetBinContent(i-1)+hAcc->GetBinContent(i));
      }
   } else {
      for (int i=hAcc->GetNbinsX();i>=1;i--)
      {
         hAcc->SetBinContent(i,hAcc->GetBinContent(i+1)+hAcc->GetBinContent(i));
      }
   } 
   return hAcc;
}

void plotFF(char *infname)
{
   TFile *inf = new TFile(infname);
   TCanvas *c = new TCanvas("c","",600,600);

   TH1D *hPt = (TH1D*) inf->Get("hPt");   
   hPt->Scale(1./hPt->Integral(0,1000));
   TH1D *hPtAcc = accumulate(hPt);
   hPtAcc->SetXTitle("Particle p_{T} GeV/c");
   hPtAcc->SetYTitle("Accumulated Fraction");
   hPtAcc->Draw();

   TCanvas *c2 = new TCanvas("c2","",600,600);
 
   TH1D *hXi = (TH1D*) inf->Get("hXi");   
   hXi->Rebin(2);
   hXi->Scale(1./hXi->Integral(0,1000));
   TH1D *hXiAcc = accumulate(hXi,1);
   hXiAcc->SetXTitle("#xi");
   hXiAcc->SetYTitle("Accumulated Fraction");
   hXi->SetXTitle("#xi");
   hXiAcc->Draw();
   
   TH1D *hMod = (TH1D*) hXi->Clone("hMod");
   
   double modify2[11] = {1.23,1.25,1.05,0.891,0.817,0.801,0.891,1.1,1.46,1.71,2.17};
   double modify[11] = {0.899,0.956,0.899,0.809,0.817,0.965,1.23,1.46,1.7,1.89,2.09};
   
   double sum = 0;
   double sum2 = 0;
   for (int i=7;i<11;i++) 
   {
      cout <<modify2[i]<<" "<< hXi->GetBinContent(i+1)<<endl;
      sum+=(modify2[i]-1)*hXi->GetBinContent(i+1);
      sum2+=(modify2[i]-1+0.1)*hXi->GetBinContent(i+1);
   }   
   
   cout <<sum<<endl;
   cout <<sum2<<endl;
}
