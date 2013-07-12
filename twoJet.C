
#include "HiForest/hiForest.h"
#include "HiForest/TrackingCorrectionsv6.h"
#include "utilities.h"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>

#define PI 3.1415926536

void twoJet(char * tag= "0", char *infName = "/d102/yjlee/hiForest2MC/Pythia80_HydjetDrum_mix01_HiForest2_v22_simTrack05.root")
{
   // Define the input file and HiForest
   HiForest *c = new HiForest(infName,"",cPPb);
   HiForest *c2 = new HiForest(infName,"",cPPb);
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
   c->doTrackCorrections=1;
   c2->hasPFTree=0;
   c2->hasPhotonTree=0;
   c2->hasTowerTree=0;
   c2->hasHbheTree=0;
   c2->hasEbTree=0;
   c2->hasGenpTree=0;
   c2->hasGenParticleTree=0;   
   c2->hasAkPu2CaloJetTree=0;
   c2->hasAkPu3CaloJetTree=0;
   c2->hasAkPu4CaloJetTree=0;
   c2->doTrackCorrections=1;
   
   // Output file
   TFile *output = new TFile(Form("output-%s.root",tag),"recreate");
   
   // Output
   TTree * t = new TTree("t","gammajet");
   
   TH2D *hSig = new TH2D("hSig","",20,-4,4,20,-PI/2.,3./2.*PI);
   TH2D *hBck = new TH2D("hBck","",20,-4,4,20,-PI/2.,3./2.*PI);

   TCanvas *cc = new TCanvas("cc","",600,600);   
    
   int scale = 1.;

   // Main loop
   for (int i=0;i<c->GetEntries()/scale;i++) {
      c->GetEntry(i);
      if (i % 1000 == 0) cout <<i<<" / "<<c->GetEntries()<<endl;
      
      // Event selection
      if (!(c->skim.phfPosFilter1&&c->skim.phfNegFilter1&&c->skim.phltPixelClusterShapeFilter&&c->skim.pHBHENoiseFilter&&c->skim.pprimaryvertexFilter)) continue;
      if (fabs(c->evt.vz)>15) continue;  
      if (c->ak5Calo.nref<=1) continue;
      int nTrk=0;
/*
      for (int j=0;j<c->ak5Calo.nref;j++) {
         if (fabs(c->ak5Calo.jteta[j])>2.4) continue;
         if (fabs(c->ak5Calo.jtPt[j])<0.4) continue;
         if (!(c->track.highPurity[j])) continue;
 
         nTrk++;
      }
//      if (nTrk<180) continue;
*/
      int selected = -1;
      for (int i2=i+1;i2<i+c2->GetEntries();i2++) {
         c2->GetEntry(i2%c->GetEntries());
         if (c->ak5Calo.nref<=1) continue;

         if (i!=(i2%c->GetEntries())&&fabs(c2->evt.vz-c->evt.vz)<1&&fabs(c2->track.nTrk-c->track.nTrk)<10) {
            selected = i2;
            break;
         }
      }
      cout <<i<<" "<<selected<<" "<<c->ak5Calo.nref<<" "<<c2->ak5Calo.nref<<" "<<c->evt.vz<<" "<<c2->evt.vz<<" "<<nTrk<<endl;


      for (int j=0;j<c->ak5Calo.nref;j++) {
         if (fabs(c->ak5Calo.jteta[j])>2.4) continue;
	 if (fabs(c->ak5Calo.jtpt[j])<1||fabs(c->ak5Calo.jtpt[j])>4) continue;
         for (int j2=j+1;j2<c->ak5Calo.nref;j2++) {
            if (fabs(c->ak5Calo.jteta[j2])>2.4) continue;
   	    if (fabs(c->ak5Calo.jtpt[j2])<1||fabs(c->ak5Calo.jtpt[j2])>4) continue;
            double deta = fabs(c->ak5Calo.jteta[j]-c->ak5Calo.jteta[j2]);
            double dphi = deltaPhi(c->track.trkPhi[j],c->track.trkPhi[j2]);
            double dphi1 = dphi;
            double dphi2 = -dphi;
            if (dphi1<-PI/2) dphi1 = 2*PI+dphi1;
            if (dphi2<-PI/2) dphi2 = 2*PI+dphi2;
            hSig->Fill(deta,dphi1);
            hSig->Fill(-deta,dphi1);
            hSig->Fill(-deta,dphi2);
            hSig->Fill(deta,dphi2);
         }
         for (int j2=0;j2<c2->ak5Calo.nref;j2++) {
            if (fabs(c2->ak5Calo.jteta[j2])>2.4) continue;
            double deta = fabs(c->ak5Calo.jteta[j]-c2->ak5Calo.jteta[j2]);
            double dphi = deltaPhi(c->ak5Calo.jtphi[j],c2->ak5Calo.jtphi[j2]);
            double dphi1 = dphi;
            double dphi2 = -dphi;
            if (dphi1<-PI/2) dphi1 = 2*PI+dphi1;
            if (dphi2<-PI/2) dphi2 = 2*PI+dphi2;
            hBck->Fill(deta,dphi1);
            hBck->Fill(-deta,dphi1);
            hBck->Fill(-deta,dphi2);
            hBck->Fill(deta,dphi2);
         }
      }
      
      
   }


   output->Write();
   output->Close();
}
