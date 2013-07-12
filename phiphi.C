#include "HiForest/hiForest.h"
#include "HiForest/TrackingCorrections2012.h"
#include "utilities.h"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>
#include <TLorentzVector.h>

void phiphi(char * tag= "0", char *infName = "/d102/yjlee/hiForest2MC/Pythia80_HydjetDrum_mix01_HiForest2_v22_simTrack05.root")
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


   TNtuple *nt = new TNtuple("nt","","xM:xEta:xPhi:phiM:phiEta:phiPhi:pt:pt1:pt2:phi1:phi2:dxy1:dxy2:hiBin");
   TCanvas *cc = new TCanvas("cc","",600,600);   
   // Main loop
   for (int i=0;i<c->GetEntries()/1.;i++) {
      c->GetEntry(i);
      if (i%10==0) cout <<i<<" / "<<c->GetEntries()<<endl;
      int nPhi =0;
   for (int j=0;j<c->track.nTrk;j++) {
      if (fabs(c->track.trkPt[j])<5) continue; 
      if (c->track.trkAlgo[j]>=4&&!(c->track.highPurity[j])) continue;
      for (int k=0;k<c->track.nTrk;k++) {
         if (j==k) continue;
	 if (fabs(c->track.trkPt[k])<5) continue;
         if (c->track.trkAlgo[k]>=4&&!(c->track.highPurity[k])) continue;
         TLorentzVector *v2 = new TLorentzVector;
         TLorentzVector *v = new TLorentzVector;
         v->SetPtEtaPhiM(c->track.trkPt[j],c->track.trkEta[j],c->track.trkPhi[j],0.493677);
         v2->SetPtEtaPhiM(c->track.trkPt[k],c->track.trkEta[k],c->track.trkPhi[k],0.493677);
	 TLorentzVector *phi = new TLorentzVector((*v) + (*v2));
         if ((phi->M())>1.035||phi->M()<1.005) {
	    delete v,v2,phi;
	    continue;
	 }
         for (int l=0;l<c->track.nTrk;l++) {
            if (l==k) continue;
            if (l==j) continue;
   	    if (fabs(c->track.trkPt[l])<3) continue;
            if (c->track.trkAlgo[l]>=4&&!(c->track.highPurity[l])) continue;
            TLorentzVector *v3 = new TLorentzVector;
	    v3->SetPtEtaPhiM(c->track.trkPt[l],c->track.trkEta[l],c->track.trkPhi[l],0.13957);
	    TLorentzVector *X = new TLorentzVector((*phi)+(*v3));
	    
	    nt->Fill(X->M(),X->Eta(),X->Phi(),phi->M(),phi->Eta(),phi->Phi(),phi->Pt(),v->Pt(),v2->Pt()
,v->Phi(),v2->Phi(),c->track.trkDxy1[j],c->track.trkDxy1[k],c->evt.hiBin);
            delete v3,X;
	 }
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
