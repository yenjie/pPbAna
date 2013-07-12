#include "utilities.h"
#include <TView.h>

void plotCF(char *infname)
{
   TCanvas *c = new TCanvas("c","",600,600);
   c->SetTheta(60);
   c->SetPhi(45);

   TFile *inf = new TFile (infname);
   
   TH1D *hSig = (TH1D*)inf->Get("hSig");
   TH1D *hBck = (TH1D*)inf->Get("hBck");

   hSig->Divide(hBck);
   hSig->Draw("surf1");      
   hSig->GetXaxis()->CenterTitle();
   hSig->GetYaxis()->CenterTitle();
   hSig->SetTitleOffset(1.5,"Y");  
   hSig->SetTitleOffset(1.5,"X");  
   hSig->SetTitleOffset(1.5,"Z");  
   hSig->SetNdivisions(103,"Z");
  
   hSig->SetXTitle("#Delta#eta");
   hSig->SetYTitle("#Delta#phi");
   hSig->SetZTitle("Ratio");

   c->SaveAs(Form("CF/%s.eps",infname));
   c->SaveAs(Form("CF/%s.gif",infname));
      

}
