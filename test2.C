#include "HiForest/hiForest.h"
#include "HiForest/TrackingCorrections2012.h"
#include "utilities.h"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>
#include <TLorentzVector.h>

void test2(char * tag= "0", char *infName = "/d102/yjlee/hiForest2MC/Pythia80_HydjetDrum_mix01_HiForest2_v22_simTrack05.root")
{
   // Define the input file and HiForest
   HiForest *c = new HiForest(infName);
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
   c->InitTree();
   
   // Output file
   TFile *output = new TFile(Form("output-%s.root",tag),"recreate");
   
   // Output
   TTree * t = new TTree("t","gammajet");
   
   JetData data(t,1);

   HistoData histos_MergedGeneralCalo("MergedGeneral");
   HistoData histos2_MergedGeneral("MergedGeneral2");   // phi dependent corr
   
   TH1D *hWeight = new TH1D("hWeight","",1000,0,100);
   TH1D *hWeight2 = new TH1D("hWeight2","",1000,0,100);
   TH1D *hPt = new TH1D("hPt","",100,0,100);
   TH1D *hNoWPt = new TH1D("hNoWPt","",100,0,100);


   TNtuple *nt = new TNtuple("nt","","m:eta:phi:pt:pt1:pt2:phi1:phi2:dxy1:dxy2:hiBin");
   TCanvas *cc = new TCanvas("cc","",600,600);   
   // Main loop
   for (int i=0;i<c->GetEntries()/10.;i++) {
      c->GetEntry(i);
      if (i%1==0) cout <<i<<" / "<<c->GetEntries()<<endl;
   for (int j=0;j<c->track.nTrk;j++) {
//      if (fabs(c->track.trkEta[j])>2.4) continue;
      if (fabs(c->track.trkPt[j])<3) continue; 
      if (c->track.trkAlgo[j]>=4&&!(c->track.highPurity[j])) continue;
//      if (fabs(c->track.trkDxy1[j]/c->track.trkDxyError1[j])<1) continue;
      for (int k=0;k<c->track.nTrk;k++) {
         if (j==k) continue;
	 if (fabs(c->track.trkPt[k])<3) continue;
         if (c->track.trkAlgo[k]>=4&&!(c->track.highPurity[k])) continue;
//         if (fabs(c->track.trkDxy1[k]/c->track.trkDxyError1[k])<1) continue;
         TLorentzVector *v2 = new TLorentzVector;
         TLorentzVector *v = new TLorentzVector;
         v->SetPtEtaPhiM(c->track.trkPt[j],c->track.trkEta[j],c->track.trkPhi[j],0.105658);
         v2->SetPtEtaPhiM(c->track.trkPt[k],c->track.trkEta[k],c->track.trkPhi[k],0.105658);
	 TLorentzVector *phi = new TLorentzVector((*v) + (*v2));
         if ((phi->M())>100) {
	    delete v,v2,phi;
	    continue;
	 }    nt->Fill(phi->M(),phi->Eta(),phi->Phi(),phi->Pt(),v->Pt(),v2->Pt(),v->Phi(),v2->Phi(),c->track.trkDxy1[j],c->track.trkDxy1[k],c->evt.hiBin);
         delete v,v2,phi;
   }
      }

      //cout <<data.mpt<<endl;
      t->Fill();
   }
  // t->Write();
   histos_MergedGeneralCalo.calcEff();
   histos2_MergedGeneral.calcEff();
   output->Write();
   output->Close();
}
