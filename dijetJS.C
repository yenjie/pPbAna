#include "HiForest/hiForest.h"
#include "HiForest/TrackingCorrections2012.h"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>
#include "analysisDataFormat.h"


void dijetJS(char * tag= "0", char *infName = "/d102/yjlee/hiForest2MC/Pythia80_HydjetDrum_mix01_HiForest2_v22_simTrack05.root",int hiBinL=0,int hiBinH=4, bool isMC = 1, bool isPP = 0)
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
      // Select leading and subleading jet
      for (int j=0;j<c->akPu3PF.nref;j++) {
         if (fabs(c->akPu3PF.jteta[j])>2) continue;
         if (c->akPu3PF.jtpt[j]>data.leadingJetPt) {
	    data.leadingJetPt = c->akPu3PF.jtpt[j];
	    data.leadingJetEta = c->akPu3PF.jteta[j];
	    data.leadingJetPhi = c->akPu3PF.jtphi[j];
	    data.leadingJetIt = j;
	 }   
	 if (c->akPu3PF.jtpt[j]>data.subleadingJetPt && c->akPu3PF.jtpt[j] < data.leadingJetPt) {
	    data.subleadingJetPt = c->akPu3PF.jtpt[j];
	    data.subleadingJetEta = c->akPu3PF.jteta[j];
	    data.subleadingJetPhi = c->akPu3PF.jtphi[j];
	    data.subleadingJetIt = j;
         }
	 if (c->akPu3PF.jtpt[j]<data.subleadingJetPt) break;	 
      } 
      
      if (data.leadingJetPt<100) continue;
      if (data.subleadingJetPt<50) continue;
      if (fabs(data.leadingJetEta)<0.5) continue;
      if (acos(cos(data.leadingJetPhi-data.subleadingJetPhi))<2./3.*3.1415926) continue;
      
      // Select generator level leading and subleading jet
      for (int j=0;j<c->akPu3PF.ngen;j++) {
         if (fabs(c->akPu3PF.geneta[j])>2) continue;
         if (c->akPu3PF.genpt[j]>data.genleadingJetPt) {
	    data.genleadingJetPt = c->akPu3PF.genpt[j];
	    data.genleadingJetEta = c->akPu3PF.geneta[j];
	    data.genleadingJetPhi = c->akPu3PF.genphi[j];
	 }   
	 if (c->akPu3PF.genpt[j]>data.gensubleadingJetPt && c->akPu3PF.genpt[j] < data.genleadingJetPt) {
	    data.gensubleadingJetPt = c->akPu3PF.genpt[j];
	    data.gensubleadingJetEta = c->akPu3PF.geneta[j];
	    data.gensubleadingJetPhi = c->akPu3PF.genphi[j];
         }
	 if (c->akPu3PF.genpt[j]<data.gensubleadingJetPt) break;	 
      } 
      
      
      for (int j=0;j<c->track.nTrk;j++) {
         if (fabs(c->track.trkEta[j])>2.4) continue;
	 if ((c->track.trkPt[j])<2) continue;
	 if (c->track.trkAlgo[j]>=4&&!(c->track.highPurity[j])) continue;
         
	 double dphi1 = acos(cos(c->track.trkPhi[j]-data.leadingJetPhi));
         double deta1 = fabs(c->track.trkEta[j]-data.leadingJetEta);
         double dreta1 = fabs(c->track.trkEta[j]+data.leadingJetEta);
         double dphi2 = acos(cos(c->track.trkPhi[j]-data.subleadingJetPhi));
         double deta2 = fabs(c->track.trkEta[j]-data.subleadingJetEta);
         double dreta2 = fabs(c->track.trkEta[j]+data.subleadingJetEta);
	 
	 double dr1 = sqrt(dphi1*dphi1+deta1*deta1);
	 double dr2 = sqrt(dphi2*dphi2+deta2*deta2);
	 double drr1 = sqrt(dphi1*dphi1+dreta1*dreta1);
	 double drr2 = sqrt(dphi2*dphi2+dreta2*dreta2);
	 
	 double trkWt=0;
         double trkWt2=0;         

         trkWt = c->getTrackCorrection(j);

	 double ptWeight = c->track.trkPt[j]/data.leadingJetPt;
	 histos_MergedGeneralCalo.hRecoPt ->Fill(c->track.trkPt[j],ptWeight);
	 histos_MergedGeneralCalo.hRecoEta->Fill(c->track.trkEta[j],ptWeight);
	 histos_MergedGeneralCalo.hRecoPhi->Fill(c->track.trkPhi[j],ptWeight);
	 histos_MergedGeneralCalo.hRecoDR ->Fill(dr1,ptWeight);
	 histos_MergedGeneralCalo.hCorrectedPt->Fill(c->track.trkPt[j],ptWeight*trkWt);
	 histos_MergedGeneralCalo.hCorrectedEta->Fill(c->track.trkEta[j],ptWeight*trkWt);
	 histos_MergedGeneralCalo.hCorrectedPhi->Fill(c->track.trkPhi[j],ptWeight*trkWt);
	 histos_MergedGeneralCalo.hCorrectedDR->Fill(dr1,ptWeight*trkWt);
	 histos_MergedGeneralCalo.hRecoJSEtaSub->Fill(dr1,ptWeight);
	 histos_MergedGeneralCalo.hCorrectedJSEtaSub->Fill(dr1,ptWeight*trkWt);
	 histos_MergedGeneralCalo.hRecoJSEtaSub->Fill(drr1,-ptWeight);
	 histos_MergedGeneralCalo.hCorrectedJSEtaSub->Fill(drr1,-ptWeight*trkWt);

         trkWt2 = c->getTrackCorrectionPara(j);
	 histos2_MergedGeneral.hRecoPt->Fill(c->track.trkPt[j],ptWeight);
	 histos2_MergedGeneral.hCorrectedPt->Fill(c->track.trkPt[j],ptWeight*trkWt2);
	 histos2_MergedGeneral.hRecoEta->Fill(c->track.trkEta[j],ptWeight);
	 histos2_MergedGeneral.hCorrectedEta->Fill(c->track.trkEta[j],ptWeight*trkWt2);
	 histos2_MergedGeneral.hRecoPhi->Fill(c->track.trkPhi[j],ptWeight);
	 histos2_MergedGeneral.hCorrectedPhi->Fill(c->track.trkPhi[j],ptWeight*trkWt2);
	 histos2_MergedGeneral.hRecoDR->Fill(dr1,ptWeight);
	 histos2_MergedGeneral.hCorrectedDR->Fill(dr1,ptWeight*trkWt2);
	 histos2_MergedGeneral.hRecoJSEtaSub->Fill(dr1,ptWeight);
	 histos2_MergedGeneral.hCorrectedJSEtaSub->Fill(dr1,ptWeight*trkWt2);
	 histos2_MergedGeneral.hRecoJSEtaSub->Fill(drr1,-ptWeight);
	 histos2_MergedGeneral.hCorrectedJSEtaSub->Fill(drr1,-ptWeight*trkWt2);

	 
	 hWeight->Fill(trkWt);
	 hWeight2->Fill(trkWt2);
	 if (trkWt2>0.001) {
	    hPt->Fill(c->track.trkPt[j]);
         } else {
	    hPt->Fill(c->track.trkPt[j]);
	    hNoWPt->Fill(c->track.trkPt[j]);
         }
      }

      for (int j=0;j<c->genparticle.mult;j++) {
         if (fabs(c->genparticle.eta[j])>2.4) continue;
	 if ((c->genparticle.pt[j])<2) continue;
	 if ((c->genparticle.sube[j]!=0)) continue;
	 if ((c->genparticle.chg[j]==0)) continue;
	 
	 
	 double dphi1 = acos(cos(c->genparticle.phi[j]-data.leadingJetPhi));
         double deta1 = fabs(c->genparticle.eta[j]-data.leadingJetEta);
         double dreta1 = fabs(c->genparticle.eta[j]+data.leadingJetEta);
	 
	 double dr1 = sqrt(dphi1*dphi1+deta1*deta1);
	 double drr1 = sqrt(dphi1*dphi1+dreta1*dreta1);
	 double ptWeight = c->genparticle.pt[j]/data.leadingJetPt;
	 
	 histos_MergedGeneralCalo.hGenPt->Fill(c->genparticle.pt[j],ptWeight);
	 histos_MergedGeneralCalo.hGenEta->Fill(c->genparticle.eta[j],ptWeight);
	 histos_MergedGeneralCalo.hGenPhi->Fill(c->genparticle.phi[j],ptWeight);
	 histos_MergedGeneralCalo.hGenDR->Fill(dr1,ptWeight);
	 histos_MergedGeneralCalo.hGenJSEtaSub->Fill(dr1,ptWeight);

	 histos2_MergedGeneral.hGenPt->Fill(c->genparticle.pt[j],ptWeight);
	 histos2_MergedGeneral.hGenEta->Fill(c->genparticle.eta[j],ptWeight);
	 histos2_MergedGeneral.hGenPhi->Fill(c->genparticle.phi[j],ptWeight);
	 histos2_MergedGeneral.hGenDR->Fill(dr1,ptWeight);
	 histos2_MergedGeneral.hGenJSEtaSub->Fill(dr1,ptWeight);

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
