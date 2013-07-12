
#include "HiForest/hiForest.h"
#include "HiForest/TrackingCorrectionsv6.h"
#include "utilities.h"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>


void dijetMPT(char * tag= "0", char *infName = "/d102/yjlee/hiForest2MC/Pythia80_HydjetDrum_mix01_HiForest2_v22_simTrack05.root")
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
   c->doTrackCorrections=1;
   
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

   TCanvas *cc = new TCanvas("cc","",600,600);   
   // Main loop
   TF1 *f = new TF1("f","[1]*(1+[2]*cos(x+[0])+[3]*cos(2*(x+[0]))+[4]*cos(3*(x+[0]))+[5]*cos(4*(x+[0]))+[6]*cos(5*(x+[0])))");
   for (int i=0;i<c->GetEntries()/1.;i++) {
      c->GetEntry(i);
      data.hiBin = c->evt.hiBin;
      data.hiHFplus = c->evt.hiHFplus;
//      if (!c->selectEvent()) continue;
//      if (!c->hlt.HLT_PAJet80_NoJetID_v1&&!c->hlt.HLT_PAPixelTrackMultiplicity140_Jet80_NoJetID_v1) continue;
      if (i % 1000 == 0) cout <<i<<" / "<<c->GetEntries()<<endl;
      data.leadingJetPt = -1;
      data.subleadingJetPt = -1;
      data.leadingJetIt = -1;
      data.subleadingJetIt = -1;
      data.genleadingJetPt = -1;
      data.gensubleadingJetPt = -1;
      
      data.N=0;
      
      for (int j=0;j<c->track.nTrk;j++) {
         if (!c->selectTrack(j)) continue;
         if (fabs(c->track.trkEta[j])>2.4) continue;
         if (fabs(c->track.trkPt[j])<0.4) continue;
         data.N++;
      }   

      // Select leading and subleading jet
      for (int j=0;j<c->ak5Calo.nref;j++) {
         if (fabs(c->ak5Calo.jteta[j])>2) continue;
//         if (c->ak5Calo.trackMax[j]<4) continue;
         if (c->ak5Calo.jtpt[j]>data.leadingJetPt) {
	    data.leadingJetPt = c->ak5Calo.jtpt[j];
	    data.leadingJetEta = c->ak5Calo.jteta[j];
	    data.leadingJetPhi = c->ak5Calo.jtphi[j];
	    data.leadingJetIt = j;
	 }   
	 if (c->ak5Calo.jtpt[j]>data.subleadingJetPt && c->ak5Calo.jtpt[j] < data.leadingJetPt) {
	    data.subleadingJetPt = c->ak5Calo.jtpt[j];
	    data.subleadingJetEta = c->ak5Calo.jteta[j];
	    data.subleadingJetPhi = c->ak5Calo.jtphi[j];
	    data.subleadingJetIt = j;
         }
	 if (c->ak5Calo.jtpt[j]<data.subleadingJetPt) break;	 
      } 

      // Select generator level leading and subleading jet
      for (int j=0;j<c->ak5Calo.ngen;j++) {
         if (fabs(c->ak5Calo.geneta[j])>2) continue;
         if (c->ak5Calo.genpt[j]>data.genleadingJetPt) {
	    data.genleadingJetPt = c->ak5Calo.genpt[j];
	    data.genleadingJetEta = c->ak5Calo.geneta[j];
	    data.genleadingJetPhi = c->ak5Calo.genphi[j];
	 }   
	 if (c->ak5Calo.genpt[j]>data.gensubleadingJetPt && c->ak5Calo.genpt[j] < data.genleadingJetPt) {
	    data.gensubleadingJetPt = c->ak5Calo.genpt[j];
	    data.gensubleadingJetEta = c->ak5Calo.geneta[j];
	    data.gensubleadingJetPhi = c->ak5Calo.genphi[j];
         }
	 if (c->ak5Calo.genpt[j]<data.gensubleadingJetPt) break;	 
      } 
      
      //if (data.subleadingJetPt<50||data.subleadingJetPt>80) continue;
//      if (data.leadingJetPt<30) continue;
//      if (data.subleadingJetPt<15) continue;
//      if (acos(cos(data.leadingJetPhi-data.subleadingJetPhi))<1007./8.*3.1415926) continue;
      
      t->Fill();
   }
  // t->Write();
   histos_MergedGeneralCalo.calcEff();
   histos2_MergedGeneral.calcEff();
   output->Write();
   output->Close();
}
