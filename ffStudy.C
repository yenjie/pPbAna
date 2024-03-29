#include "HiForest/hiForest.h"
#include "HiForest/TrackingCorrections2012.h"
#include "utilities.h"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>


void ffStudy(double tag=0, char *infName = "/d102/yjlee/hiForest2MC/Pythia80_HydjetDrum_mix01_HiForest2_v22_simTrack05.root")
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
   TFile *output = new TFile(Form("output-%.0f.root",tag),"recreate");
   
   // Output
   TTree * t = new TTree("t","gammajet");
   
   JetData data(t,1);

   HistoData histos_MergedGeneralCalo("MergedGeneral");
   HistoData histos2_MergedGeneral("MergedGeneral2");   // phi dependent corr
   
   TH1D *hWeight = new TH1D("hWeight","",1000,0,100);
   TH1D *hWeight2 = new TH1D("hWeight2","",1000,0,100);
   TH1D *hPt = new TH1D("hPt","",100,0,100);
   TH1D *hNoWPt = new TH1D("hNoWPt","",100,0,100);
   
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
      
      
      // Select leading and subleading jet
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

      // Select generator level leading and subleading jet
      for (int j=0;j<c->icPu5.ngen;j++) {
         if (fabs(c->icPu5.geneta[j])>2) continue;
         if (c->icPu5.genpt[j]>data.genleadingJetPt) {
	    data.genleadingJetPt = c->icPu5.genpt[j];
	    data.genleadingJetEta = c->icPu5.geneta[j];
	    data.genleadingJetPhi = c->icPu5.genphi[j];
	 }   
	 if (c->icPu5.genpt[j]>data.gensubleadingJetPt && c->icPu5.genpt[j] < data.genleadingJetPt) {
	    data.gensubleadingJetPt = c->icPu5.genpt[j];
	    data.gensubleadingJetEta = c->icPu5.geneta[j];
	    data.gensubleadingJetPhi = c->icPu5.genphi[j];
         }
	 if (c->icPu5.genpt[j]<data.gensubleadingJetPt) break;	 
      } 
      
      //if (data.subleadingJetPt<50||data.subleadingJetPt>80) continue;
      
      // MPT calculation
      data.mpt = 0;
      data.cormpt = 0;
      data.cormpt2 = 0;
      data.genMpt = 0;
      data.genPMpt = 0;
      
      for (int j=0;j<c->track.nTrk;j++) {
         if (fabs(c->track.trkEta[j])>2.4) continue;
	 if (fabs(c->track.trkPt[j])<0.5) continue;
	 double mptTrk = -c->track.trkPt[j]*cos(c->track.trkPhi[j]-data.leadingJetPhi);
	 data.mpt+=mptTrk;
      }

      for (int j=0;j<c->track.nTrk;j++) {
         if (fabs(c->track.trkEta[j])>2.4) continue;
	 if ((c->track.trkPt[j])<0.5) continue;
	 if (c->track.trkAlgo[j]>=4&&!(c->track.highPurity[j])) continue;
         
	 double dphi1 = acos(cos(c->track.trkPhi[j]-data.leadingJetPhi));
         double deta1 = fabs(c->track.trkEta[j]-data.leadingJetEta);
         double dphi2 = acos(cos(c->track.trkPhi[j]-data.subleadingJetPhi));
         double deta2 = fabs(c->track.trkEta[j]-data.subleadingJetEta);
	 
	 double dr1 = sqrt(dphi1*dphi1+deta1*deta1);
	 double dr2 = sqrt(dphi2*dphi2+deta2*deta2);
	 
	 double trkWt=1;
         double trkWt2=0;         

	 //cout <<trkWt<<endl;
	 double mptTrk = -c->track.trkPt[j]*cos(c->track.trkPhi[j]-data.leadingJetPhi)*trkWt;
	 data.cormpt+=mptTrk;
	 if (data.leadingJetPt>100&&dr1<0.5) trkWt = 0.5; 
	 if (data.subleadingJetPt>100&&dr2<0.5) trkWt = 0.5; 
	 
	 double ptWeight = 1;//c->track.trkPt[j];
	 histos_MergedGeneralCalo.hRecoPt->Fill(c->track.trkPt[j],ptWeight);
	 histos_MergedGeneralCalo.hCorrectedPt->Fill(c->track.trkPt[j],ptWeight*trkWt);
	 histos_MergedGeneralCalo.hRecoEta->Fill(c->track.trkEta[j],ptWeight);
	 histos_MergedGeneralCalo.hCorrectedEta->Fill(c->track.trkEta[j],ptWeight*trkWt);
	 histos_MergedGeneralCalo.hRecoPhi->Fill(c->track.trkPhi[j],ptWeight);
	 histos_MergedGeneralCalo.hCorrectedPhi->Fill(c->track.trkPhi[j],ptWeight*trkWt);
	 histos_MergedGeneralCalo.hRecoDR->Fill(dr1,ptWeight);
	 histos_MergedGeneralCalo.hCorrectedDR->Fill(dr1,ptWeight*trkWt);


	 if (data.leadingJetPt>=50&&dr1<0.5) {
            if (data.leadingJetPt>200) trkWt2 = c->trackCorrections[1]->GetCorr(c->track.trkPt[j],c->track.trkEta[j],c->track.trkPhi[j],c->evt.hiBin);
            else trkWt2 = c->trackCorrections[2]->GetCorr(c->track.trkPt[j],c->track.trkEta[j],c->track.trkPhi[j],c->evt.hiBin);
         } else if (data.subleadingJetPt>=50&&dr2<0.5) {
            if (data.subleadingJetPt>120) trkWt2 = c->trackCorrections[1]->GetCorr(c->track.trkPt[j],c->track.trkEta[j],c->track.trkPhi[j],c->evt.hiBin);
            else trkWt2 = c->trackCorrections[2]->GetCorr(c->track.trkPt[j],c->track.trkEta[j],c->track.trkPhi[j],c->evt.hiBin);        
         } else {
            trkWt2 = c->trackCorrections[0]->GetCorr(c->track.trkPt[j],c->track.trkEta[j],c->track.trkPhi[j],c->evt.hiBin);
         }

         trkWt2 = c->getTrackCorrectionPara(j);

	 histos2_MergedGeneral.hRecoPt->Fill(c->track.trkPt[j],ptWeight);
	 histos2_MergedGeneral.hCorrectedPt->Fill(c->track.trkPt[j],ptWeight*trkWt2);
	 histos2_MergedGeneral.hRecoEta->Fill(c->track.trkEta[j],ptWeight);
	 histos2_MergedGeneral.hCorrectedEta->Fill(c->track.trkEta[j],ptWeight*trkWt2);
	 histos2_MergedGeneral.hRecoPhi->Fill(c->track.trkPhi[j],ptWeight);
	 histos2_MergedGeneral.hCorrectedPhi->Fill(c->track.trkPhi[j],ptWeight*trkWt2);
	 histos2_MergedGeneral.hRecoDR->Fill(dr1,ptWeight);
	 histos2_MergedGeneral.hCorrectedDR->Fill(dr1,ptWeight*trkWt2);

	 double mptTrk2 = -c->track.trkPt[j]*cos(c->track.trkPhi[j]-data.leadingJetPhi)*trkWt2;
	 data.cormpt2+=mptTrk2;
	 
	 hWeight->Fill(trkWt);
	 hWeight2->Fill(trkWt2);
	 if (trkWt2>0.001) {
	    hPt->Fill(c->track.trkPt[j]);
         } else {
	    hPt->Fill(c->track.trkPt[j]);
	    hNoWPt->Fill(c->track.trkPt[j]);
         
         }
	 

      }

      for (int j=0;j<c->track.nParticle;j++) {
         if (fabs(c->track.pEta[j])>2.4) continue;
	 if (fabs(c->track.pPt[j])<0.5) continue;
	 double mptPTrk = -c->track.pPt[j]*cos(c->track.pPhi[j]-data.leadingJetPhi);
	 data.genPMpt+=mptPTrk;

	 double dphi1 = acos(cos(c->track.pPhi[j]-data.leadingJetPhi));
         double deta1 = fabs(c->track.pEta[j]-data.leadingJetEta);
	 
	 double dr1 = sqrt(dphi1*dphi1+deta1*deta1);
	 double ptWeight = 1;//c->track.pPt[j];
	 
	 histos_MergedGeneralCalo.hGenPt->Fill(c->track.pPt[j],ptWeight);
	 histos_MergedGeneralCalo.hGenEta->Fill(c->track.pEta[j],ptWeight);
	 histos_MergedGeneralCalo.hGenPhi->Fill(c->track.pPhi[j],ptWeight);
	 histos_MergedGeneralCalo.hGenDR->Fill(dr1,ptWeight);
	 histos2_MergedGeneral.hGenPt->Fill(c->track.pPt[j],ptWeight);
	 histos2_MergedGeneral.hGenEta->Fill(c->track.pEta[j],ptWeight);
	 histos2_MergedGeneral.hGenPhi->Fill(c->track.pPhi[j],ptWeight);
	 histos2_MergedGeneral.hGenDR->Fill(dr1,ptWeight);
	 double mptTrk = -c->track.pPt[j]*cos(c->track.pPhi[j]-data.genleadingJetPhi);
	 data.genMpt+=mptTrk;

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
