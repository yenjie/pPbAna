#include "HiForest/hiForest.h"
#include "utilities.h"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>


// Example of forest skim

void pPbSkim(char *title,char *infname = "/d100/yjlee/hiForest/PromptReco2011/HIHighPt/skim_Photon35/merged_HIData2011_HIHighPt_highPtExercise_photonSkim35GeVEB.root")
{
   // Define the input file and HiForest
   HiForest *c = new HiForest(infname,"",cPPb);
   c->SetOutputFile(Form("skim_pPb_%s.root",title));
   c->hasPFTree=0;
   c->hasPhotonTree=0;
   c->hasTowerTree=0;
   c->hasHbheTree=0;
   c->hasEbTree=0;
   c->hasGenpTree=0;
   c->hasGenParticleTree=0;
   
   c->hasAkPu2CaloJetTree=0;
   c->hasAkPu3CaloJetTree=0;
   c->hasAkPu4CaloJetTree=0;
   
   // Main loop
  
   for (int i=0;i<c->GetEntries();i++) {
      c->GetEntry(i);
      int nTrk=0;
      for (int j=0;j<c->track.nTrk;j++) {
         if (fabs(c->track.trkEta[j])>2.4) continue;
         if (fabs(c->track.trkPt[j])<0.4) continue;
         if (!(c->track.highPurity[j])) continue;
         nTrk++;
      }
      if (i % 1000 == 0) cout <<i<<" / "<<c->GetEntries()<<endl;
//      if (!(c->skim.phfPosFilter1&&c->skim.phfNegFilter1&&c->skim.phltPixelClusterShapeFilter&&c->skim.pprimaryvertexFilter)) continue;   // good selection
//      if (!(c->skim.phfPosFilter1&&c->skim.phfNegFilter1&&!c->skim.phltPixelClusterShapeFilter&&c->skim.pHBHENoiseFilter)) continue;
      if (fabs(c->evt.vz)>15) continue;
      if (!c->hlt.HLT_PAZeroBiasPixel_SingleTrack_v1) continue;
      
      
      if (!(c->skim.phltPixelClusterShapeFilter)) continue;  
      if (!(c->skim.phfPosFilter1&&c->skim.phfNegFilter1)) continue;  
      if ((c->skim.pprimaryvertexFilter)) continue;  
      if (c->evt.run!=202792) continue;   
//      if (nTrk<110) continue;      
//      if (c->track.nTrk==0) continue;
         c->FillOutput();
   }
   delete c;
}
