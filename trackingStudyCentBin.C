#include "utilities.h"
#include "HiForest/hiForest.h"

void trackingStudyCentBin(char *infname = "skim_jet_pthat80Sim05.root")
{
   HiForest *c = new HiForest(infname);
   c->tree->AddFriend("nt");

   TCut trackSelection = "(mtrkQual||mtrkAlgo<4)&&pNRec>0";
   TCut dijetSelection = "leadingJetPt>120&&subleadingJetPt>50&&acos(cos(leadingJetPhi-subleadingJetPhi))>7./8.*3.141593";
   TCut genParticleCut = "abs(pEta)<2.4";
   
   TCanvas *chiBin = new TCanvas ("chiBin","",600,600);
   
   TH1F *hEffhiBin = new TH1F("hEffhiBin","",40,0,40);
   
   makeHistTitle(hEffhiBin,"","Centrality Bin","Tracking Efficiency");
   
   TCut weightCent = "1";//"(0.782188/(0.782188+0.00529868*hiBin+(-2.9114e-4)*hiBin**2+(9.7019e-6)*hiBin**3+(-1.24787e-7)*hiBin**4))";
//   TCut weightCent = "1";//" (0.642662/(0.642662+(5.17863e-3)*hiBin+(-3.4125e-4)*hiBin*hiBin+(1.26925e-5)*hiBin*hiBin*hiBin+(-1.67013e-7)*hiBin*hiBin*hiBin*hiBin)*(hiBin<=37)+(hiBin>37)*00.642662/(1.47942+(6.31446e-3)*hiBin-(-7.30847e-4)*hiBin*hiBin))";
   TCut weightEta =  "0.73854/(0.73854+(-0.0248947)*pEta+(-0.0519076)*pEta**2+0.0123081*pEta**3-0.00919964*pEta**4+(-0.00156727)*pEta**5+(0.000785894)*pEta**5)";

   TGraphAsymmErrors *g = getEfficiency(c->tree,"hiBin",40,0,40,trackSelection,TCut("pPt>20")&&dijetSelection&&genParticleCut,weightCent*weightEta);
   TGraphAsymmErrors *g2 = getEfficiency(c->tree,"hiBin",40,0,40,trackSelection,TCut("pPt>8")&&dijetSelection&&genParticleCut,weightCent*weightEta);
   TGraphAsymmErrors *g3 = getEfficiency(c->tree,"hiBin",40,0,40,trackSelection,TCut("pPt>0")&&dijetSelection&&genParticleCut,weightCent*weightEta);

   hEffhiBin->SetAxisRange(0.5,0.8,"Y");
   hEffhiBin->Draw();
   
   g->Draw("p same");
   g2->SetLineColor(2);
   g2->SetMarkerColor(2);
   g2->Draw("p same");
   
   g3->SetLineColor(4);
   g3->SetMarkerColor(4);
   g3->Draw("p same");

   TLegend *leghiBin = myLegend(0.5,0.7,0.9,0.9);
   leghiBin->AddEntry(g,"#hat{p}_{T} 80 GeV/c, Dijet selection","");
   leghiBin->AddEntry(g,"simTrack p_{T} > 20 GeV/c","pl");
   leghiBin->AddEntry(g2,"simTrack p_{T} > 8 GeV/c","pl");
   leghiBin->AddEntry(g3,"simTrack p_{T} > 0.5 GeV/c","pl");
   
   leghiBin->Draw();
   chiBin->SaveAs("effFig/EffVshiBin-DijetSelection.gif");
   chiBin->SaveAs("effFig/EffVshiBin-DijetSelection.C");

   TCanvas *chiBinNoDijet = new TCanvas ("chiBinNoDijet","",600,600);

   TGraphAsymmErrors *gg = getEfficiency(c->tree,"hiBin",40,0,40,trackSelection,TCut("pPt>20")&&genParticleCut,weightCent*weightEta);
   TGraphAsymmErrors *gg2 = getEfficiency(c->tree,"hiBin",40,0,40,trackSelection,TCut("pPt>8")&&genParticleCut,weightCent*weightEta);
   TGraphAsymmErrors *gg3 = getEfficiency(c->tree,"hiBin",40,0,40,trackSelection,TCut("pPt>0")&&genParticleCut,weightCent*weightEta);

   hEffhiBin->SetAxisRange(0.5,1.0,"Y");
   hEffhiBin->Draw();

   gg->SetMarkerStyle(24);
   gg2->SetMarkerStyle(24);
   gg3->SetMarkerStyle(24);
   gg->Draw("p same");
   gg2->SetLineColor(2);
   gg2->SetMarkerColor(2);
   gg2->Draw("p same");
   
   gg3->SetLineColor(4);
   gg3->SetMarkerColor(4);
   gg3->Draw("p same");
   
   TLegend *leghiBin2 = myLegend(0.2,0.7,0.6,0.9);
   leghiBin2->AddEntry(gg,"#hat{p}_{T} 80 GeV/c, No selection","");
   leghiBin2->AddEntry(gg,"simTrack p_{T} > 20 GeV/c","pl");
   leghiBin2->AddEntry(gg2,"simTrack p_{T} > 8 GeV/c","pl");
   leghiBin2->AddEntry(gg3,"simTrack p_{T} > 0.5 GeV/c","pl");
   
   leghiBin2->Draw();

   chiBinNoDijet->SaveAs("effFig/EffVshiBin-NoDijetSelection.gif");
   chiBinNoDijet->SaveAs("effFig/EffVshiBin-NoDijetSelection.C");

   gg3->Fit("pol4");   
}
