#include "hiForest.h"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>

// Example of forest skim

void noiseSkim(char *infname = "/d100/yjlee/hiForest/PromptReco2011/HIHighPt/skim_Photon35/merged_HIData2011_HIHighPt_highPtExercise_photonSkim35GeVEB.root")
{
   // Define the input file and HiForest
   HiForest *c = new HiForest(infname);
   c->SetOutputFile("skim_jet.root");

   // Main loop
   for (int i=0;i<c->GetEntries();i++)
   {
      c->GetEntry(i);
      if (i%1000==0) cout <<i<<" / "<<c->GetEntries()<<endl;
      
      if (c->akPu3PF.jtpt[0]>=98||c->akPu2PF.jtpt[0]>=98&&c->akPu4PF.jtpt[0]>98){
         c->FillOutput(); // Write output forest
      }
   }

   delete c;
}
