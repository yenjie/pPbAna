#include "HiForest/hiForest.h"
#include "utilities.h"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>


// Example of forest skim

void jetSkim(char *title,char *infname = "/d100/yjlee/hiForest/PromptReco2011/HIHighPt/skim_Photon35/merged_HIData2011_HIHighPt_highPtExercise_photonSkim35GeVEB.root")
{
   // Define the input file and HiForest
   HiForest *c = new HiForest(infname);
   c->SetOutputFile(Form("skim_jet_%s.root",title));
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
   c->outf->cd();
   TTree * nt = new TTree("nt","dijet");
   nt->SetMaxTreeSize(4000000000);
  
   JetData data(nt,1);
   
   for (int i=0;i<c->GetEntries()/3.;i++) {
      c->GetEntry(i);
      data.hiBin = c->evt.hiBin;
      if (i % 1000 == 0) cout <<i<<" / "<<c->GetEntries()<<endl;
      data.leadingJetPt = -1;
      data.subleadingJetPt = -1;
      data.leadingJetIt = -1;
      data.subleadingJetIt = -1;
      
      if (fabs(c->evt.vz)>15) continue;
      
      for (int j=0;j<c->icPu5.nref;j++) {
         if (fabs(c->icPu5.jteta[j])>2) continue;
         if (c->icPu5.jtpt[j]>data.leadingJetPt) {
	    data.leadingJetPt = c->icPu5.jtpt[j];
	    data.leadingJetEta = c->icPu5.jteta[j];
	    data.leadingJetPhi = c->icPu5.jtphi[j];
	    data.leadingJetIt = j;
	 }   
	 if (c->icPu5.jtpt[j]>data.subleadingJetPt && c->icPu5.jtpt[j] < data.leadingJetPt) {
	    data.subleadingJetPt = c->icPu5.jtpt[j];
	    data.subleadingJetEta = c->icPu5.jteta[j];
	    data.subleadingJetPhi = c->icPu5.jtphi[j];
	    data.subleadingJetIt = j;
         }
	 if (c->icPu5.jtpt[j]<data.subleadingJetPt) break;	 
      }
//      if (data.leadingJetPt>120) {
         c->FillOutput();
         nt->Fill();
//      }	
   }
   nt->Write();   
   delete c;
}
