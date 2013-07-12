#include "HiForest/hiForest.h"
#include "utilities.h"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>
#include <TLorentzVector.h>

void phiAnalysis(char * tag= "0", char *infName = "/d102/yjlee/hiForest2MC/Pythia80_HydjetDrum_mix01_HiForest2_v22_simTrack05.root")
{
   // Define the input file and HiForest
   HiForest *c = new HiForest(infName,"",cPPb);
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


   TNtuple *nt = new TNtuple("nt","","m:eta:phi:pt:pt1:pt2:ch1:ch2:phi1:phi2:hiBin:N:highPurity1:highPurity2");
   TNtuple *ntEvt = new TNtuple("ntEvt","","N");
   nt->SetAutoFlush(30000);
   cout <<nt->GetAutoFlush()<<endl;
   TCanvas *cc = new TCanvas("cc","",600,600);   
//	 nt->SetCircular(1000);
   // Main loop

   TLorentzVector *v2 = new TLorentzVector;
   TLorentzVector *v = new TLorentzVector;
   TLorentzVector phi;

   for (int i=0;i<c->GetEntries()/1.;i++) {
      c->GetEntry(i);
      if (!c->selectEvent()) continue;
      if (i%1000==0){
         cout <<i<<" / "<<c->GetEntries()<<endl;
       } 
      int N=0;   
      for (int j=0;j<c->track.nTrk;j++) {
         if (!c->selectTrack(j)) continue;
         if (fabs(c->track.trkEta[j])>2.4) continue;
         if (fabs(c->track.trkPt[j])<0.4) continue;
         N++;
      }   
      ntEvt->Fill(N);
      for (int j=0;j<c->track.nTrk;j++) {
//         if (!c->selectTrack(j)) continue;
         if (fabs(c->track.trkPt[j])<5) continue; 
//      if (fabs(c->track.trkDxy1[j]/c->track.trkDxyError1[j])<1) continue;
         for (int k=j+1;k<c->track.nTrk;k++) {
            if (j==k) continue;
//            if (!c->selectTrack(k)) continue;
//            if (c->track.trkCharge[k]==c->track.trkCharge[j]) continue; 
     	    if (fabs(c->track.trkPt[k])<5) continue;
            v->SetPtEtaPhiM(c->track.trkPt[j],c->track.trkEta[j],c->track.trkPhi[j],0.493677);
            v2->SetPtEtaPhiM(c->track.trkPt[k],c->track.trkEta[k],c->track.trkPhi[k],0.493677);
//          v2->SetPtEtaPhiM(c->track.trkPt[k],c->track.trkEta[k],c->track.trkPhi[k],0.13957);
//          v->SetPtEtaPhiM(c->track.trkPt[j],c->track.trkEta[j],c->track.trkPhi[j],0.938272);
//          v->SetPtEtaPhiM(c->track.trkPt[j],c->track.trkEta[j],c->track.trkPhi[j],0.13957);
//          v2->SetPtEtaPhiM(c->track.trkPt[k],c->track.trkEta[k],c->track.trkPhi[k],0.13957);
  	    phi = (*v) + (*v2);
//         if ((phi.M())>5) {
            if ((phi.M())>1.2||phi.M()<0.0) {
//	       phi.Delete();
	       continue;
	    }    
	    nt->Fill(phi.M(),phi.Eta(),phi.Phi(),phi.Pt(),v->Pt(),v2->Pt(),c->track.trkCharge[j],c->track.trkCharge[k],v->Phi(),v2->Phi(),c->evt.hiBin,N,c->track.highPurity[j],c->track.highPurity[k]);
//	       phi.Delete();
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
