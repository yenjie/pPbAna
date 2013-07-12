#include "HiForest/hiForest.h"
#include "HiForest/TrackingCorrections2012.h"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>
#include "analysisDataFormat.h"


void jetFF(char * tag= "0", char *infName = "/d102/yjlee/hiForest2MC/Pythia80_HydjetDrum_mix01_HiForest2_v22_simTrack05.root",int hiBinL=0,int hiBinH=40, bool isMC = 1, bool isPP = 0)
{
   // Define the input file and HiForest
   HiForest *c = new HiForest(infName,"forest",isPP);
   c->hasPFTree=0;
   c->hasPhotonTree=0;
   c->hasTowerTree=0;
   c->hasHbheTree=0;
   c->hasEbTree=0;
   c->hasGenpTree=0;
   c->hasAkPu2CaloJetTree=0;
   c->hasAkPu3CaloJetTree=0;
   c->hasAkPu4CaloJetTree=0;
   c->doTrackCorrections=1;
   c->InitTree();
   
   // Output file
   TFile *output = new TFile(Form("output-%s-%d-%d.root",tag,hiBinL,hiBinH),"recreate");
   
   // Output
   TTree * t = new TTree("t","gammajet");
   
   JetData data(t,1);

   HistoData histos_MergedGeneralCalo("MergedGeneral");
   HistoData histos2_MergedGeneral("MergedGeneral2");   // phi dependent corr
   
   TH1D *hWeight = new TH1D("hWeight","",1000,0,100);
   TH1D *hWeight2 = new TH1D("hWeight2","",1000,0,100);
   TH1D *hPt = new TH1D("hPt","",100,0,100);
   TH1D *hXi = new TH1D("hXi","",28,0,7);
   TH1D *hXi1 = new TH1D("hXi1","",28,0,7);
   TH1D *hXi2 = new TH1D("hXi2","",28,0,7);
   TH1D *hNoWPt = new TH1D("hNoWPt","",100,0,100);
   
   TH1D *hJS = new TH1D("hJS","",6,0,0.3);
   
   // Main loop
   for (int i=0;i<c->GetEntries();i++) {
      c->GetEntry(i);
      data.hiBin = c->evt.hiBin;
      if (i % 1000 == 0) cout <<i<<" / "<<c->GetEntries()<<endl;
      data.leadingJetPt = -1;
      data.subleadingJetPt = -1;
      data.leadingJetIt = -1;
      data.subleadingJetIt = -1;
      data.genleadingJetPt = -1;
      data.gensubleadingJetPt = -1;
      
      // Event selection
      if (fabs(c->evt.vz)>15) continue;
      if (c->evt.hiBin<hiBinL) continue;
      if (c->evt.hiBin>=hiBinH) continue;
      if (!isMC&&!c->selectEvent()) continue;
      
      // Select generator level leading and subleading jet
      for (int n=0;n<c->akPu3PF.ngen;n++) {
         if (fabs(c->akPu3PF.geneta[n])>2) continue;
         if (c->akPu3PF.genpt[n]<100) continue;
	 
         for (int j=0;j<c->genparticle.mult;j++) {
            if (fabs(c->genparticle.eta[j])>2.4) continue;
	    if ((c->genparticle.sube[j]!=0)) continue;
	    double dphi1 = acos(cos(c->genparticle.phi[j]-c->akPu3PF.genphi[n]));
            double deta1 = fabs(c->genparticle.eta[j]-c->akPu3PF.geneta[n]);
	    double dr1 = sqrt(dphi1*dphi1+deta1*deta1);
	    if ((dr1)<0.3) {
	       hPt->Fill(c->genparticle.pt[j],c->genparticle.pt[j]);
	       hXi->Fill(-log(c->genparticle.pt[j]/c->akPu3PF.genpt[n]),c->genparticle.pt[j]);
	       if (c->genparticle.pt[j]>1) hXi1->Fill(-log(c->genparticle.pt[j]/c->akPu3PF.genpt[n]),c->genparticle.pt[j]);
	       if (c->genparticle.pt[j]>2) hXi2->Fill(-log(c->genparticle.pt[j]/c->akPu3PF.genpt[n]),c->genparticle.pt[j]);
	    }
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
