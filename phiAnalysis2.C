#include "HiForest/hiForest.h"
#include "utilities.h"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>
#include <TLorentzVector.h>

class pi0data
{
   public:
   float m;
   float eta;
   float phi;
   float pt;
   float pt1;
   float pt2;
   float ch1;
   float ch2;
   float phi1;
   float phi2;
   float hiBin;
   float N;
   float highPurity1;
   float highPurity2;
   float dxy1;
   float dxy2;
   float ptError1;
   float ptError2;
   float nhit1;
   float nhit2;
};

void phiAnalysis2(char * tag= "0", char *infName = "/d102/yjlee/hiForest2MC/Pythia80_HydjetDrum_mix01_HiForest2_v22_simTrack05.root")
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

   pi0data pi0;
   TTree *nt = new TTree("nt","pi0");
   nt->Branch("m",&pi0.m,"m/F");
   nt->Branch("eta",&pi0.eta,"eta/F");
   nt->Branch("phi",&pi0.phi,"phi/F");
   nt->Branch("pt1",&pi0.pt1,"pt1/F");
   nt->Branch("pt2",&pi0.pt2,"pt2/F");
   nt->Branch("pt",&pi0.pt,"pt/F");
   nt->Branch("ch1",&pi0.ch1,"ch1/F");
   nt->Branch("ch2",&pi0.ch2,"ch2/F");
   nt->Branch("phi1",&pi0.phi1,"phi1/F");
   nt->Branch("phi2",&pi0.phi2,"phi2/F");
   nt->Branch("hiBin",&pi0.hiBin,"hiBin/F");
   nt->Branch("N",&pi0.N,"N/F");
   nt->Branch("highPurity1",&pi0.highPurity1,"highPurity1/F");
   nt->Branch("highPurity2",&pi0.highPurity2,"highPurity2/F");
   nt->Branch("dxy1",&pi0.dxy1,"dxy1/F");
   nt->Branch("dxy2",&pi0.dxy2,"dxy2/F");
   nt->Branch("ptError1",&pi0.ptError1,"ptError1/F");
   nt->Branch("ptError2",&pi0.ptError2,"ptError2/F");
   nt->Branch("nhit1",&pi0.nhit1,"nhit1/F");
   nt->Branch("nhit2",&pi0.nhit2,"nhit2/F");

   TNtuple *ntEvt = new TNtuple("ntEvt","","N");
   nt->SetAutoFlush(30000);
   cout <<nt->GetAutoFlush()<<endl;
   TCanvas *cc = new TCanvas("cc","",600,600);   

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
            pi0.m=phi.M();
	    pi0.eta=phi.Eta();
	    pi0.phi=phi.Phi();
	    pi0.pt=phi.Pt();
	    pi0.pt1=v->Pt();
	    pi0.pt2=v2->Pt();
	    pi0.ch1=c->track.trkCharge[j];
	    pi0.ch2=c->track.trkCharge[k];
	    pi0.phi1=v->Phi();
	    pi0.phi2=v2->Phi();
	    pi0.hiBin=c->evt.hiBin;
	    pi0.N=N;
	    pi0.highPurity1=c->track.highPurity[j];
	    pi0.highPurity2=c->track.highPurity[k];
	    pi0.nhit1=c->track.trkNHit[j];
	    pi0.nhit2=c->track.trkNHit[k];
            pi0.dxy1=c->track.trkDz1[j]/c->track.trkDzError1[j];
            pi0.dxy2=c->track.trkDz1[k]/c->track.trkDzError1[k];
            pi0.ptError1=c->track.trkPtError[j]/c->track.trkPt[j];
            pi0.ptError2=c->track.trkPtError[k]/c->track.trkPt[k];
	    nt->Fill();
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
