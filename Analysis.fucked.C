#include <iostream>
#include "Analysis.h"
#include "TGraph.h"
#include "TMarker.h"
#include "TCanvas.h"
#include "TText.h"
#include "TBox.h"
#include "TMatrixDSym.h"
#include "TMatrixDSymEigen.h"
#include "TGraphAsymmErrors.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#define REALDATA 0   // REALDATA is eq to zero for MC and 1 for real data

#define KEEP_EVENT 0 //used in Pre_OPS_OR
#define SKIP_EVENT 1

#define JETPT1_CUT 40000
#define JETPT2_CUT 40000
#define JETPT3_CUT 20000
#define JETPT4_CUT 20000
#define NJETMIN    3
#define NELEMIN    0
#define NMUOMIN    0
#define NMUGMIN    0
#define NTAUMIN    0
#define NLEPMIN    3
#define NSLOWMUOMIN    1

#define ETMISS_CUT 30000

//for background only use NHIGGSMIN  0 and NHIGGSMAX  0
//for signal only use NHIGGSMIN  1 and NHIGGSMAX  2
//for everything use NHIGGSMIN  0 and NHIGGSMAX  2
#define NHIGGSMIN  1
#define NHIGGSMAX  2


// require trigger?
#define REQTRIGGER true
//#define REQTRIGGER false
using namespace std;

Analysis::Analysis(D3PDSelector * inst)
{
  //Constructor. The constructor is called from SlaveBegin in the D3PDSelector
  
  m_sel = inst; //MANDATORY. The D3PDSelector Object must be known to the Analysis Class.
  
  cout << " In constructor" << endl;
  bbcounter=0;
  
  
  //The selector class needs to know which are the prefixes for the particles objects.
  m_sel->SetElectronContainerPrefix("el");
  m_sel->SetMuonContainerPrefix("mu_muid");
  m_sel->SetLowBetaMuonContainerPrefix("mu_lowbeta");
  m_sel->SetMuGirlMuonContainerPrefix("mu_mugirl");
  m_sel->SetJetContainerPrefix("jet_akt4topoem");
  //	m_sel->SetPhotonContainerPrefix("ph");
	
  // Let's create a histogram and register it with the D3PD Class
  cout << " Creating histos" << endl;
  meff = new TH1D("meff4","meff4",2000,0,4000);  
  
  hNJets = new TH1D("hNJets","N jets",30,0,30);
  hNOkJets = new TH1D("hNOkJets","N ok jets",30,0,30);
  
  hCutFlow = new TH1D("hCutFlow", "cut flow", 12, 0.0, 12.0);

  hJet1pT = new TH1D("hJet1pT", "jet1pT", 250, 0, 1300);
  hJet2pT = new TH1D("hJet2pT", "jet2pT", 250, 0, 1300);
  hJet3pT = new TH1D("hJet3pT", "jet3pT", 250, 0, 800);
  hJet4pT = new TH1D("hJet4pT", "jet4pT", 250, 0, 400);
  
  hJet1Eta = new TH1D("hJet1Eta", "jet1Eta", 100, -3.0, 3.0);
  hJet2Eta = new TH1D("hJet2Eta", "jet2Eta", 100, -3.0, 3.0);
  hJet3Eta = new TH1D("hJet3Eta", "jet3Eta", 100, -3.0, 3.0);
  hJet4Eta = new TH1D("hJet4Eta", "jet4Eta", 100, -3.0, 3.0);
  
  hJet1Phi = new TH1D("hJet1Phi", "jet1Phi", 100, -3.5, 3.5);
  hJet2Phi = new TH1D("hJet2Phi", "jet2Phi", 100, -3.5, 3.5);
  hJet3Phi = new TH1D("hJet3Phi", "jet3Phi", 100, -3.5, 3.5);
  hJet4Phi = new TH1D("hJet4Phi", "jet4Phi", 100, -3.5, 3.5);
  
  hJet1Mass = new TH1D("hJet1Mass", "jet1Mass", 250, 0.0, 200000.0);
  hJet2Mass = new TH1D("hJet2Mass", "jet2Mass", 250, 0.0, 100000.0);
  hJet3Mass = new TH1D("hJet3Mass", "jet3Mass", 250, 0.0, 60000.0);
  hJet4Mass = new TH1D("hJet4Mass", "jet4Mass", 250, 1000.0, 40000.0);
  
  hJet1FlavorWeight = new TH1D("hJet1FlavorWeight", "jet1FlavorWeight", 100, -25., 25.);
  hJet2FlavorWeight = new TH1D("hJet2FlavorWeight", "jet2FlavorWeight", 100, -25., 25.);
  hJet3FlavorWeight = new TH1D("hJet3FlavorWeight", "jet3FlavorWeight", 100, -25., 25.);
  hJet4FlavorWeight = new TH1D("hJet4FlavorWeight", "jet4FlavorWeight", 100, -25., 25.);
  
  hJet1dRTruthB = new TH1D("hJet1dRTruthB", "hJet1dRTruthB", 100, 0.0, 7.0);
  hJet2dRTruthB = new TH1D("hJet2dRTruthB", "hJet2dRTruthB", 100, 0.0, 7.0);
  hJet3dRTruthB = new TH1D("hJet3dRTruthB", "hJet3dRTruthB", 100, 0.0, 7.0);
  hJet4dRTruthB = new TH1D("hJet4dRTruthB", "hJet4dRTruthB", 100, 0.0, 7.0);
  
  hdRJet1Jet2 = new TH1D("hdRJet1Jet2", "dRJet1Jet2", 100, 0.0, 7.0);
  hdRJet1Jet3 = new TH1D("hdRJet1Jet3", "dRJet1Jet3", 100, 0.0, 7.0);
  hdRJet1Jet4 = new TH1D("hdRJet1Jet4", "dRJet1Jet4", 100, 0.0, 7.0);
  hdRJet2Jet3 = new TH1D("hdRJet2Jet3", "dRJet2Jet3", 100, 0.0, 7.0);
  hdRJet2Jet4 = new TH1D("hdRJet2Jet4", "dRJet2Jet4", 100, 0.0, 7.0);
  hdRJet3Jet4 = new TH1D("hdRJet3Jet4", "dRJet3Jet4", 100, 0.0, 7.0);
  
  // Electrons
  hRecoElectrons = new TH1D("hRecoElectrons","Number of Reco Electrons ",30,0,30);
  hRecoOkElectrons = new TH1D("hRecoOkElectrons","Number of ok Reco Electrons ",30,0,30);
  hRecoEleInvMass = new TH1D("hRecoEleInvMass","Reco Mass first 2 ee ",100,0,200);
  
  hElectron1pT = new TH1D("hElectron1pT", "Electron 1 pT", 250, 0, 1000);
  hElectron2pT = new TH1D("hElectron2pT", "Electron 2 pT", 250, 0, 600);
  hElectron3pT = new TH1D("hElectron3pT", "Electron 3 pT", 250, 0, 600);
  
  hElectron1Eta = new TH1D("hElectron1Eta", "Electron 1 Eta", 100, -3.0, 3.0);
  hElectron2Eta = new TH1D("hElectron2Eta", "Electron 2 Eta", 100, -3.0, 3.0);
  hElectron3Eta = new TH1D("hElectron3Eta", "Electron 3 Eta", 100, -3.0, 3.0);
  
  hElectron1Phi = new TH1D("hElectron1Phi", "Electron 1 Phi", 100, -3.5, 3.5);
  hElectron2Phi = new TH1D("hElectron2Phi", "Electron 2 Phi", 100, -3.5, 3.5);
  hElectron3Phi = new TH1D("hElectron3Phi", "Electron 3 Phi", 100, -3.5, 3.5);  
  
  hElectron1Mass = new TH1D("hElectron1Mass", "Electron 1 Mass", 250, 0.0, 150);
  hElectron2Mass = new TH1D("hElectron2Mass", "Electron 2 Mass", 250, 0.0, 150);
  hElectron3Mass = new TH1D("hElectron3Mass", "Electron 3 Mass", 250, 0.0, 150);
  
  hdRElectron1Electron2 = new TH1D("hdRElectron1Electron2", "dR Electron 1 - Electron 2", 100, 0.0 , 7.0);
  hdRElectron1Electron3 = new TH1D("hdRElectron1Electron3", "dR Electron 1 - Electron 3", 100, 0.0 , 7.0);
  hdRElectron1Muon1 = new TH1D("hdRElectron1Muon1", "dR Electron 1 - Muon 2", 100, 0.0 , 7.0);
  hdRElectron1Muon2 = new TH1D("hdRElectron1Muon2", "dR Electron 1 - Muon 2", 100, 0.0 , 7.0);
  //  hdRElectron1Tau1 = new TH1D("hdRElectron1Tau1", "dR Electron 1 - Tau 2", 100, 0.0 , 7.0);
  //  hdRElectron1Tau2 = new TH1D("hdRElectron1Tau2", "dR Electron 1 - Tau 2", 100, 0.0 , 7.0);
  
  hdRElectron2Electron3 = new TH1D("hdRElectron2Electron3", "dR Electron 2 - Electron 3", 100, 0.0 , 7.0);
  hdRElectron2Muon1 = new TH1D("hdRElectron2Muon1", "dR Electron 2 - Muon 2", 100, 0.0 , 7.0);
  hdRElectron2Muon2 = new TH1D("hdRElectron2Muon2", "dR Electron 2 - Muon 2", 100, 0.0 , 7.0);
  //  hdRElectron2Tau1 = new TH1D("hdRElectron2Tau1", "dR Electron 2 - Tau 2", 100, 0.0 , 7.0);
  //  hdRElectron2Tau2 = new TH1D("hdRElectron2Tau2", "dR Electron 2 - Tau 2", 100, 0.0 , 7.0);
  
  hElectronTruthType = new TH1D("hElectronTruthType", "Truth MC PDG ID for the Electron", 100, -50.0 , 50.0);
  hElectronBSTruthType = new TH1D("hElectronBSTruthType", "Truth MC PDG ID for the Electron - 1000000 ( Sleptons) ", 100, -50.0 , 50.0);
  
  //Muons
  hRecoMuons = new TH1D("hRecoMuons","Number of Reco Muons ",10,0,10);
  hRecoOkMuons = new TH1D("hRecoOkMuons","Number of Reco OK Muons ",10,0,10);
  
  hMuon1pT = new TH1D("hMuon1pT", "Muon 1 pT", 250, 0, 1000);
  hMuon2pT = new TH1D("hMuon2pT", "Muon 2 pT", 250, 0, 600);
  hMuon3pT = new TH1D("hMuon3pT", "Muon 3 pT", 250, 0, 600);
  
  hMuon1Eta = new TH1D("hMuon1Eta", "Muon 1 Eta", 100, -3.0, 3.0);
  hMuon2Eta = new TH1D("hMuon2Eta", "Muon 2 Eta", 100, -3.0, 3.0);
  hMuon3Eta = new TH1D("hMuon3Eta", "Muon 3 Eta", 100, -3.0, 3.0);
  
  hMuon1Phi = new TH1D("hMuon1Phi", "Muon 1 Phi", 100, -3.5, 3.5);
  hMuon2Phi = new TH1D("hMuon2Phi", "Muon 2 Phi", 100, -3.5, 3.5);
  hMuon3Phi = new TH1D("hMuon3Phi", "Muon 3 Phi", 100, -3.5, 3.5);
  
  hMuon1Mass = new TH1D("hMuon1Mass", "Muon 1 Mass", 100, 0.0, 150.0);
  hMuon2Mass = new TH1D("hMuon2Mass", "Muon 2 Mass", 100, 0.0, 150.0);
  hMuon3Mass = new TH1D("hMuon3Mass", "Muon 3 Mass", 100, 0.0, 150.0);
  
  hMuon1Beta = new TH1D("hMuon1Beta", "Muon 1 Beta", 100, 0.0, 1.0);
  hMuon2Beta = new TH1D("hMuon2Beta", "Muon 2 Beta", 100, 0.0, 1.0);
  hMuon3Beta = new TH1D("hMuon3Beta", "Muon 3 Beta", 100, 0.0, 1.0);
  
  hdRMuon1Muon2 = new TH1D("hdRMuon1Muon2", "dR Muon 1 - Muon 2", 100, 0.0 , 7.0);
  hdRMuon1Muon3 = new TH1D("hdRMuon1Muon3", "dR Muon 1 - Muon 3", 100, 0.0 , 7.0);
  //  hdRMuon1Tau1 = new TH1D("hdRMuon1Tau1", "dR Muon 1 - Tau 2", 100, 0.0 , 7.0);
  //  hdRMuon1Tau2 = new TH1D("hdRMuon1Tau2", "dR Muon 1 - Tau 2", 100, 0.0 , 7.0);
  
  hdRMuon2Muon3 = new TH1D("hdRMuon2Muon3", "dR Muon 2 - Muon 2", 100, 0.0 , 7.0);
  //  hdRMuon2Tau1 = new TH1D("hdRMuon2Tau1", "dR Muon 2 - Tau 2", 100, 0.0 , 7.0);
  //  hdRMuon2Tau2 = new TH1D("hdRMuon2Tau2", "dR Muon 2 - Tau 2", 100, 0.0 , 7.0);
  
  hMuonTruthType = new TH1D("hMuonTruthType", "Truth MC PDG ID for the Muon", 100, -50.0 , 50.0);
  hMuonBSTruthType = new TH1D("hMuonBSTruthType", "Truth MC PDG ID for the Muon - 1000000 ( Sleptons) ", 100, -50.0 , 50.0);

  //Slow Muons
  hRecoMuLowBetaMuons = new TH1D("hRecoMuLowBetaMuons","Number of Reco LowBetaMuons ",10,0,10);
  hRecoOkMuLowBetaMuons = new TH1D("hRecoOkMuLowBetaMuons","Number of Reco OK LowBetaMuons ",10,0,10);
  
  hMuLowBetaMuon1pT = new TH1D("hMuLowBetaMuon1pT", "LowBetaMuon 1 pT", 250, 0, 1000);
  hMuLowBetaMuon2pT = new TH1D("hMuLowBetaMuon2pT", "LowBetaMuon 2 pT", 250, 0, 600);
  hMuLowBetaMuon3pT = new TH1D("hMuLowBetaMuon3pT", "LowBetaMuon 3 pT", 250, 0, 600);
  
  hMuLowBetaMuon1Eta = new TH1D("hMuLowBetaMuon1Eta", "LowBetaMuon 1 Eta", 100, -3.0, 3.0);
  hMuLowBetaMuon2Eta = new TH1D("hMuLowBetaMuon2Eta", "LowBetaMuon 2 Eta", 100, -3.0, 3.0);
  hMuLowBetaMuon3Eta = new TH1D("hMuLowBetaMuon3Eta", "LowBetaMuon 3 Eta", 100, -3.0, 3.0);
  
  hMuLowBetaMuon1Phi = new TH1D("hMuLowBetaMuon1Phi", "LowBetaMuon 1 Phi", 100, -3.5, 3.5);
  hMuLowBetaMuon2Phi = new TH1D("hMuLowBetaMuon2Phi", "LowBetaMuon 2 Phi", 100, -3.5, 3.5);
  hMuLowBetaMuon3Phi = new TH1D("hMuLowBetaMuon3Phi", "LowBetaMuon 3 Phi", 100, -3.5, 3.5);
  
  hMuLowBetaMuon1Mass = new TH1D("hMuLowBetaMuon1Mass", "LowBetaMuon 1 Mass", 100, 0.0, 170.0);
  hMuLowBetaMuon2Mass = new TH1D("hMuLowBetaMuon2Mass", "LowBetaMuon 2 Mass", 100, 0.0, 170.0);
  hMuLowBetaMuon3Mass = new TH1D("hMuLowBetaMuon3Mass", "LowBetaMuon 3 Mass", 100, 0.0, 170.0);
    
  hMuLowBetaMuon1MassBeta = new TH1D("hMuLowBetaMuon1MassBeta", "LowBetaMuon 1 Mass from Beta", 100, 0.0, 170.0);
  hMuLowBetaMuon2MassBeta = new TH1D("hMuLowBetaMuon2MassBeta", "LowBetaMuon 2 Mass from Beta", 100, 0.0, 170.0);
  hMuLowBetaMuon3MassBeta = new TH1D("hMuLowBetaMuon3MassBeta", "LowBetaMuon 3 Mass from Beta", 100, 0.0, 170.0);
  
  hMuLowBetaMuon1Beta = new TH1D("hMuLowBetaMuon1Beta", "LowBetaMuon 1 Beta", 100, 0.0, 1.0);
  hMuLowBetaMuon2Beta = new TH1D("hMuLowBetaMuon2Beta", "LowBetaMuon 2 Beta", 100, 0.0, 1.0);
  hMuLowBetaMuon3Beta = new TH1D("hMuLowBetaMuon3Beta", "LowBetaMuon 3 Beta", 100, 0.0, 1.0);

  hMuLowBetaMuonTruthType = new TH1D("hMuLowBetaMuonTruthType", "Truth MC PDG ID for the LowBetaMuon", 100, -50.0 , 50.0);
  hMuLowBetaMuonBSTruthType = new TH1D("hMuLowBetaMuonBSTruthType", "Truth MC PDG ID for the LowBetaMuon - 1000000 ( Sleptons) ", 100, -50.0 , 50.0);
  
  //MuGirl Muons
  hRecoMuGirlMuons = new TH1D("hRecoMuGirlMuons","Number of Reco MuGirlMuons ",10,0,10);
  hRecoOkMuGirlMuons = new TH1D("hRecoOkMuGirlMuons","Number of Reco OK MuGirlMuons ",10,0,10);
  
  hMuGirlMuon1pT = new TH1D("hMuGirlMuon1pT", "MuGirlMuon 1 pT", 250, 0, 1000);
  hMuGirlMuon2pT = new TH1D("hMuGirlMuon2pT", "MuGirlMuon 2 pT", 250, 0, 600);
  hMuGirlMuon3pT = new TH1D("hMuGirlMuon3pT", "MuGirlMuon 3 pT", 250, 0, 600);
  
  hMuGirlMuon1Eta = new TH1D("hMuGirlMuon1Eta", "MuGirlMuon 1 Eta", 100, -3.0, 3.0);
  hMuGirlMuon2Eta = new TH1D("hMuGirlMuon2Eta", "MuGirlMuon 2 Eta", 100, -3.0, 3.0);
  hMuGirlMuon3Eta = new TH1D("hMuGirlMuon3Eta", "MuGirlMuon 3 Eta", 100, -3.0, 3.0);
  
  hMuGirlMuon1Phi = new TH1D("hMuGirlMuon1Phi", "MuGirlMuon 1 Phi", 100, -3.5, 3.5);
  hMuGirlMuon2Phi = new TH1D("hMuGirlMuon2Phi", "MuGirlMuon 2 Phi", 100, -3.5, 3.5);
  hMuGirlMuon3Phi = new TH1D("hMuGirlMuon3Phi", "MuGirlMuon 3 Phi", 100, -3.5, 3.5);
  
  hMuGirlMuon1Mass = new TH1D("hMuGirlMuon1Mass", "MuGirlMuon 1 Mass", 100, 0.0, 150.0);
  hMuGirlMuon2Mass = new TH1D("hMuGirlMuon2Mass", "MuGirlMuon 2 Mass", 100, 0.0, 150.0);
  hMuGirlMuon3Mass = new TH1D("hMuGirlMuon3Mass", "MuGirlMuon 3 Mass", 100, 0.0, 150.0);
    
  hMuGirlMuon1MassBeta = new TH1D("hMuGirlMuon1MassBeta", "MuGirlMuon 1 Mass from Beta", 100, 0.0, 150.0);
  hMuGirlMuon2MassBeta = new TH1D("hMuGirlMuon2MassBeta", "MuGirlMuon 2 Mass from Beta", 100, 0.0, 150.0);
  hMuGirlMuon3MassBeta = new TH1D("hMuGirlMuon3MassBeta", "MuGirlMuon 3 Mass from Beta", 100, 0.0, 150.0);
  
  hMuGirlMuon1Beta = new TH1D("hMuGirlMuon1Beta", "MuGirlMuon 1 Beta", 100, 0.0, 1.0);
  hMuGirlMuon2Beta = new TH1D("hMuGirlMuon2Beta", "MuGirlMuon 2 Beta", 100, 0.0, 1.0);
  hMuGirlMuon3Beta = new TH1D("hMuGirlMuon3Beta", "MuGirlMuon 3 Beta", 100, 0.0, 1.0);

  hMuGirlMuonTruthType = new TH1D("hMuGirlMuonTruthType", "Truth MC PDG ID for the MuGirlMuon", 100, -50.0 , 50.0);
  hMuGirlMuonBSTruthType = new TH1D("hMuGirlMuonBSTruthType", "Truth MC PDG ID for the MuGirlMuon - 1000000 ( Sleptons) ", 100, -50.0 , 50.0);
    
 
  //Taus
  //  hRecoTaus = new TH1D("hRecoTaus","Number of Reco Taus ",20,0,20);
  //  hRecoOkTaus = new TH1D("hRecoOkTaus","Number of Reco OK Taus ",20,0,20);

  //  hTau1pT = new TH1D("hTau1pT", "Tau 1 pT", 250, 0, 1000);
  //  hTau2pT = new TH1D("hTau2pT", "Tau 2 pT", 250, 0, 600);
  //  hTau3pT = new TH1D("hTau3pT", "Tau 3 pT", 250, 0, 600);
  
  //  hTau1Eta = new TH1D("hTau1Eta", "Tau 1 Eta", 100, -3.0, 3.0);
  //  hTau2Eta = new TH1D("hTau2Eta", "Tau 2 Eta", 100, -3.0, 3.0);
  //  hTau3Eta = new TH1D("hTau3Eta", "Tau 3 Eta", 100, -3.0, 3.0);
  
  //  hTau1Phi = new TH1D("hTau1Phi", "Tau 1 Phi", 100, -3.5, 3.5);
  //  hTau2Phi = new TH1D("hTau2Phi", "Tau 2 Phi", 100, -3.5, 3.5);
  //  hTau3Phi = new TH1D("hTau3Phi", "Tau 3 Phi", 100, -3.5, 3.5);
  
  //  hTau1Mass = new TH1D("hTau1Mass", "Tau 1 Mass", 100, 0.0, 150.0);
  //  hTau2Mass = new TH1D("hTau2Mass", "Tau 2 Mass", 100, 0.0, 150.0);
  //  hTau3Mass = new TH1D("hTau3Mass", "Tau 3 Mass", 100, 0.0, 150.0);
  
  //  hdRTau1Tau2 = new TH1D("hdRTau1Tau2", "dR Tau 1 - Tau 2", 100, 0.0 , 7.0);
  //  hdRTau1Tau3 = new TH1D("hdRTau1Tau3", "dR Tau 1 - Tau 3", 100, 0.0 , 7.0);
  //  hdRTau2Tau3 = new TH1D("hdRTau2Tau3", "dR Tau 2 - Tau 3", 100, 0.0 , 7.0);
  
  
  hRecoOkLeptons = new TH1D("hRecoOkLeptons","Number of Reco OK Leptons ",20,0,20);

  // MET quantities
  hMET = new TH1D("hMET","Missing ET",250,0.0,1000);
  hMETPhi = new TH1D("hMETPhi","Missing ET - Phi",100,-3.5,3.5);
  // Reco Masses of Higgs and Z from b-partons
  hNHiggs = new TH1D("hNHiggs","Number of Higgs",10,0,10);
  lshMassHiggs = new TH1D("lshMassHiggs","Reco Mass Higgs ",200,0,200);
  lshMassZ = new TH1D("lshMassZ","Reco Mass Z ",200,0,200);
  
  // Parton b's PT spectrum from Higgs1 and Higgs2
  lshPTb1  = new TH1D("lshPTb1", "b from first Higgs PT ", 200, 0, 1000);
  lshPTb2  = new TH1D("lshPTb2", "bbar from first Higgs PT ", 200, 0, 1000);
  lshPTb3  = new TH1D("lshPTb3", "b from second Higgs PT ", 200, 0, 1000);
  lshPTb4  = new TH1D("lshPTb4", "bbar from second Higgs PT ", 200, 0, 1000);
  
  // PT of matched jets
  lshJet1Match  = new TH1D("lshJet1Match", "Jet1 Matched PT - bjet pT", 200, 0, 1000);
  lshJet2Match  = new TH1D("lshJet2Match", "Jet2 Matched PT - bbarjet pT", 200, 0, 1000);
  
  // Matched and unmatched reco Higgs masses from jets
  lshJetsMass34 = new TH1D("lshJetsMass34","Reco Mass 3rd and 4th jet ",100,0,100);
  lshJetsNewMass34 = new TH1D("lshJetsNewMass34","Reco Mass 3rd and 4th jet with matching ",100,0,100);
  // other invarian masses
  lshRecoJJInvMass = new TH1D("lshRecoJJInvMass","Reco Mass 3rd and 4th Jets ",100,0.0,100);
  lshRecoBBInvMass = new TH1D("lshRecoBBInvMass","Reco Mass 2 b-jets ",100,0.0,100);
  lshRecoEleInvMass = new TH1D("lshRecoEleInvMass","Reco Mass first 2 ee ",100,0.,1);
  hdRMinDilep = new TH1D("hdRMinDilep","Dilepton min dR",100,0.0,7.0);
  hdRMinDilepInvMass= new TH1D("hdRMinDilepInvMass","Dilepton Inv Mass with min dR",100,0.0,500.0);
  h2dInvMassMindRDilep= new TH2D("h2dInvMassMindRDilep","Dilepton min dR vs Inv Mass",100,0.0,500.0,100,0.0,7.0);
  hdRDilep= new TH1D("hdRDilep","Dilepton dR - no dR restriction",100,0.,7.0);
  hDilepInvMass= new TH1D("hDilepInvMass","Dilepton Inv Mass - no min dR",100,0.0,500.0);
  h2dInvMassdRDilep= new TH2D("h2dInvMassdRDilep","Dilepton dR vs Inv Mass - no min dR restriction",100,0.0,500.0,100,0.0,7.0);

  hdRMinLeptons = new TH1D("hdRMinLeptons","Leptons min dR",100,0.,7.0);
  hMassMindRLeptons = new TH1D("hMassMindRLeptons","Leptons min dR Mass",100,0.0,500.0);
  h2dMassMindRLeptons = new TH2D("h2dMassMindRLeptons","min dR Leptons vs Inv Mass",100,0.0,500.0,100,0.0,7.0);
  hdRLeptons = new TH1D("hdRLeptons","Leptons dR",100,0.,7.0);
  hMassdRLeptons = new TH1D("hMassdRLeptons","Leptons dR Mass",100,0.0,500.0);
  h2dMassdRLeptons = new TH2D("h2dMassdRLeptons","dR Leptons vs Inv Mass",100,0.0,500.0,100,0.0,7.0);
  //bTagInfo
  
  lshJetFlavor1 = new TH1D("lshJetFlavor1", "Flavor of 1st jet - NHiggs == 1 ", 200, -100, 100);
  lshJetFlavor2 = new TH1D("lshJetFlavor2", "Flavor of 2nd jet - NHiggs == 1 ", 200, -100, 100);
  lshJetFlavor3 = new TH1D("lshJetFlavor3", "Flavor of 3rd jet - NHiggs == 1 ", 200, -100, 100);
  lshJetFlavor4 = new TH1D("lshJetFlavor4", "Flavor of 4th jet - NHiggs == 1 ", 200, -100, 100);
  lshJetFlavor5 = new TH1D("lshJetFlavor5", "Flavor of 5th jet - NHiggs == 1 ", 200, -100, 100);
  lshJetFlavor6 = new TH1D("lshJetFlavor6", "Flavor of 6th jet - NHiggs == 1 ", 200, -100, 100);
  lshJetFlavorMatch1 = new TH1D("lshJetFlavorMatch1", "Flavor of 1st matched jet ", 200, -100, 100);
  lshJetFlavorMatch2 = new TH1D("lshJetFlavorMatch2", "Flavor of 2nd matched jet ", 200, -100, 100);
  
  lshJetWeight1 = new TH1D("lshJetWeight1", "Weight of 1st jet - NHiggs == 1 ", 100, -50, 50);
  lshJetWeight2 = new TH1D("lshJetWeight2", "Weight of 2nd jet - NHiggs == 1 ", 100, -50, 50);
  lshJetWeight3 = new TH1D("lshJetWeight3", "Weight of 3rd jet - NHiggs == 1 ", 100, -50, 50);
  lshJetWeight4 = new TH1D("lshJetWeight4", "Weight of 4th jet - NHiggs == 1 ", 100, -50, 50);
  lshJetWeight5 = new TH1D("lshJetWeight5", "Weight of 5th jet - NHiggs == 1 ", 100, -50, 50);
  lshJetWeight6 = new TH1D("lshJetWeight6", "Weight of 6th jet - NHiggs == 1 ", 100, -50, 50);
  lshJetWeightMatch1 = new TH1D("lshJetWeightMatch1", "Weight of 1st matched jet ", 200, -100, 100);
  lshJetWeightMatch2 = new TH1D("lshJetWeightMatch2", "Weight of 2nd matched jet ", 200, -100, 100);
  
  
  // Parton Level quantities
  
  lshMinus = new TH1D("lshMinus","Number of same sign leptons Minus ",20,0,20);
  lshEMinus = new TH1D("lshEMinus","Number of same sign ele Minus ",20,0,20);
  lshMuMinus = new TH1D("lshMuMinus","Number of same sign mu Minus ",20,0,20);
  //  lshTauMinus = new TH1D("lshTauMinus","Number of same sign tau Minus ",20,0,20);
  
  lshPlus = new TH1D("lshPlus","Number of same sign leptons Plus ",20,0,20);
  lshEPlus = new TH1D("lshEPlus","Number of same sign ele Plus ",20,0,20);
  lshMuPlus = new TH1D("lshMuPlus","Number of same sign mu Plus ",20,0,20);
  //  lshTauPlus = new TH1D("lshTauPlus","Number of same sign tau Plus ",20,0,20);
  
  lshLepton = new TH1D("lshLepton","Number of leptons ",20,0,20);
  
  
  
  // sleptons masses
  lsh4 = new TH1D("lsh4","Mass mu2 ",200,0,1000);
  lsh44 = new TH1D("lsh44","Mass mu1 ",200,0,1000);
  lsh5 = new TH1D("lsh5","Mass eR",200,0,1000);
  lsh55 = new TH1D("lsh55","Mass eR1",200,0,1000);
  lsh6 = new TH1D("lsh6","Mass ta1 ",200,0,1000);
  lsh66 = new TH1D("lsh66","Mass ta2 ",200,0,1000);
  
  lshMassChi0e = new TH1D("lshMassChi0e","Reco Mass Chi0 ele ",200,0,1000);
  lshMassChi0m = new TH1D("lshMassChi0m","Reco Mass Chi0 mu ",200,0,1000);
  lshMassChi0t = new TH1D("lshMassChi0t","Reco Mass Chi0 tau ",200,0,1000);
  lshChi02Mass = new TH1D("lshChi02Mass","HEPG Mass Chi02 ",200,0,1000);
  lshChi01Mass = new TH1D("lshChi01Mass","HEPG Mass Chi01 ",200,0,1000);
  lshChi03Mass = new TH1D("lshChi03Mass","HEPG Mass Chi03 ",200,0,1000);
  
  
  // TRIGGER STUDIES
  hn_sel_pt = new TH1D("hn_sel_pt", "histo hn_sel_pt", 40,0.0,500.0);
  hn_sel_L1_pt = new TH1D("hn_sel_L1_pt", "histo hn_sel_L1_pt", 40,0.0,500.0);
  hn_sel_L2_pt = new TH1D("hn_sel_L2_pt", "histo hn_sel_L2_pt", 40,0.0,500.0);
  hn_sel_EF_pt = new TH1D("hn_sel_EF_pt", "histo hn_sel_EF_pt", 40,0.0,500.0);
  hn_ratio_pt_L1SEL = new TH1D("hn_ratio_pt_L1SEL", "histo hn_ratio_pt_L1SEL", 40,0.0,500.0);
  hn_ratio_pt_L1L2 = new TH1D("hn_ratio_pt_L1L2", "histo hn_ratio_pt_L1L2", 40,0.0,500.0);
  hn_ratio_pt_L2EF = new TH1D("hn_ratio_pt_L2EF", "histo hn_ratio_pt_L2EF", 40,0.0,500.0);
  hn_slow_sel_pt = new TH1D("hn_slow_sel_pt", "histo hn_slow_sel_pt", 40,0.0,500.0);
  hn_slow_sel_L1_pt = new TH1D("hn_slow_sel_L1_pt", "histo hn_slow_sel_L1_pt", 40,0.0,500.0);
  hn_slow_sel_L2_pt = new TH1D("hn_slow_sel_L2_pt", "histo hn_slow_sel_L2_pt", 40,0.0,500.0);
  hn_slow_sel_EF_pt = new TH1D("hn_slow_sel_EF_pt", "histo hn_slow_sel_EF_pt", 40,0.0,500.0);
  hn_slow_ratio_pt_L1SEL = new TH1D("hn_slow_ratio_pt_L1SEL", "histo hn_slow_ratio_pt_L1SEL", 40,0.0,500.0);
  hn_slow_ratio_pt_L1L2 = new TH1D("hn_slow_ratio_pt_L1L2", "histo hn_slow_ratio_pt_L1L2", 40,0.0,500.0);
  hn_slow_ratio_pt_L2EF = new TH1D("hn_slow_ratio_pt_L2EF", "histo hn_slow_ratio_pt_L2EF", 40,0.0,500.0);
  
  hn_sel_eta = new TH1D("hn_sel_eta", "histo hn_sel_eta", 40,-3.5,3.5);
  hn_sel_L1_eta = new TH1D("hn_sel_L1_eta", "histo hn_sel_L1_eta", 40,-3.5,3.5);
  hn_sel_L2_eta = new TH1D("hn_sel_L2_eta", "histo hn_sel_L2_eta", 40,-3.5,3.5);
  hn_sel_EF_eta = new TH1D("hn_sel_EF_eta", "histo hn_sel_EF_eta", 40,-3.5,3.5);
  hn_ratio_eta_L1SEL = new TH1D("hn_ratio_eta_L1SEL", "histo hn_ratio_eta_L1SEL", 40,-3.5,3.5);
  hn_ratio_eta_L1L2 = new TH1D("hn_ratio_eta_L1L2", "histo hn_ratio_eta_L1L2", 40,-3.5,3.5);
  hn_ratio_eta_L2EF = new TH1D("hn_ratio_eta_L2EF", "histo hn_ratio_eta_L2EF", 40,-3.5,3.5);
  hn_slow_sel_eta = new TH1D("hn_slow_sel_eta", "histo hn_slow_sel_eta", 40,-3.5,3.5);
  hn_slow_sel_L1_eta = new TH1D("hn_slow_sel_L1_eta", "histo hn_slow_sel_L1_eta", 40,-3.5,3.5);
  hn_slow_sel_L2_eta = new TH1D("hn_slow_sel_L2_eta", "histo hn_slow_sel_L2_eta", 40,-3.5,3.5);
  hn_slow_sel_EF_eta = new TH1D("hn_slow_sel_EF_eta", "histo hn_slow_sel_EF_eta", 40,-3.5,3.5);
  hn_slow_ratio_eta_L1SEL = new TH1D("hn_slow_ratio_eta_L1SEL", "histo hn_slow_ratio_eta_L1SEL", 40,-3.5,3.5);
  hn_slow_ratio_eta_L1L2 = new TH1D("hn_slow_ratio_eta_L1L2", "histo hn_slow_ratio_eta_L1L2", 40,-3.5,3.5);
  hn_slow_ratio_eta_L2EF = new TH1D("hn_slow_ratio_eta_L2EF", "histo hn_slow_ratio_eta_L2EF", 40,-3.5,3.5);
  
  hn_sel_phi = new TH1D("hn_sel_phi", "histo hn_sel_phi", 40,-3.0,3.0);
  hn_sel_L1_phi = new TH1D("hn_sel_L1_phi", "histo hn_sel_L1_phi", 40,-3.0,3.0);
  hn_sel_L2_phi = new TH1D("hn_sel_L2_phi", "histo hn_sel_L2_phi", 40,-3.0,3.0);
  hn_sel_EF_phi = new TH1D("hn_sel_EF_phi", "histo hn_sel_EF_phi", 40,-3.0,3.0);
  hn_ratio_phi_L1SEL = new TH1D("hn_ratio_phi_L1SEL", "histo hn_ratio_phi_L1SEL", 40,-3.0,3.0);
  hn_ratio_phi_L1L2 = new TH1D("hn_ratio_phi_L1L2", "histo hn_ratio_phi_L1L2", 40,-3.0,3.0);
  hn_ratio_phi_L2EF = new TH1D("hn_ratio_phi_L2EF", "histo hn_ratio_phi_L2EF", 40,-3.0,3.0);
  hn_slow_sel_phi = new TH1D("hn_slow_sel_phi", "histo hn_slow_sel_phi", 40,-3.0,3.0);
  hn_slow_sel_L1_phi = new TH1D("hn_slow_sel_L1_phi", "histo hn_slow_sel_L1_phi", 40,-3.0,3.0);
  hn_slow_sel_L2_phi = new TH1D("hn_slow_sel_L2_phi", "histo hn_slow_sel_L2_phi", 40,-3.0,3.0);
  hn_slow_sel_EF_phi = new TH1D("hn_slow_sel_EF_phi", "histo hn_slow_sel_EF_phi", 40,-3.0,3.0);
  hn_slow_ratio_phi_L1SEL = new TH1D("hn_slow_ratio_phi_L1SEL", "histo hn_slow_ratio_phi_L1SEL", 40,-3.0,3.0);
  hn_slow_ratio_phi_L1L2 = new TH1D("hn_slow_ratio_phi_L1L2", "histo hn_slow_ratio_phi_L1L2", 40,-3.0,3.0);
  hn_slow_ratio_phi_L2EF = new TH1D("hn_slow_ratio_phi_L2EF", "histo hn_slow_ratio_phi_L2EF", 40,-3.0,3.0);
  
  hn_sel_eta_phi = new TH2D("hn_sel_eta_phi", "histo hn_sel_eta_phi", 14,-3.5,3.5,14,-3.0,3.0);
  hn_sel_L1_eta_phi = new TH2D("hn_sel_L1_eta_phi", "histo hn_sel_L1_eta_phi", 14,-3.5,3.5,14,-3.0,3.0);
  hn_sel_L2_eta_phi = new TH2D("hn_sel_L2_eta_phi", "histo hn_sel_L2_eta_phi", 14,-3.5,3.5,14,-3.0,3.0);
  hn_sel_EF_eta_phi = new TH2D("hn_sel_EF_eta_phi", "histo hn_sel_EF_eta_phi", 14,-3.5,3.5,14,-3.0,3.0);
  hn_ratio_eta_phi_L1SEL = new TH2D("hn_ratio_eta_phi_L1SEL", "histo hn_ratio_eta_phi_L1SEL", 14,-3.5,3.5,14,-3.0,3.0);
  hn_ratio_eta_phi_L1L2 = new TH2D("hn_ratio_eta_phi_L1L2", "histo hn_ratio_eta_phi_L1L2", 14,-3.5,3.5,14,-3.0,3.0);
  hn_ratio_eta_phi_L2EF = new TH2D("hn_ratio_eta_phi_L2EF", "histo hn_ratio_eta_phi_L2EF", 14,-3.5,3.5,14,-3.0,3.0);
  hn_slow_sel_eta_phi = new TH2D("hn_slow_sel_eta_phi", "histo hn_slow_sel_eta_phi", 14,-3.5,3.5,14,-3.0,3.0);
  hn_slow_sel_L1_eta_phi = new TH2D("hn_slow_sel_L1_eta_phi", "histo hn_slow_sel_L1_eta_phi", 14,-3.5,3.5,14,-3.0,3.0);
  hn_slow_sel_L2_eta_phi = new TH2D("hn_slow_sel_L2_eta_phi", "histo hn_slow_sel_L2_eta_phi", 14,-3.5,3.5,14,-3.0,3.0);
  hn_slow_sel_EF_eta_phi = new TH2D("hn_slow_sel_EF_eta_phi", "histo hn_slow_sel_EF_eta_phi", 14,-3.5,3.5,14,-3.0,3.0);
  hn_slow_ratio_eta_phi_L1SEL = new TH2D("hn_slow_ratio_eta_phi_L1SEL", "histo hn_slow_ratio_eta_phi_L1SEL", 14,-3.5,3.5,14,-3.0,3.0);
  hn_slow_ratio_eta_phi_L1L2 = new TH2D("hn_slow_ratio_eta_phi_L1L2", "histo hn_slow_ratio_eta_phi_L1L2", 14,-3.5,3.5,14,-3.0,3.0);
  hn_slow_ratio_eta_phi_L2EF = new TH2D("hn_slow_ratio_eta_phi_L2EF", "histo hn_slow_ratio_eta_phi_L2EF", 14,-3.5,3.5,14,-3.0,3.0);
  
  cout << " In constructor - half" << endl;
  
  hn_sel_phi->Sumw2();
  hn_sel_L1_phi->Sumw2();
  hn_sel_L2_phi->Sumw2();
  hn_sel_EF_phi->Sumw2();
  hn_ratio_phi_L1SEL->Sumw2();
  hn_ratio_phi_L1L2->Sumw2();
  hn_ratio_phi_L2EF->Sumw2();
  hn_slow_sel_phi->Sumw2();
  hn_slow_sel_L1_phi->Sumw2();
  hn_slow_sel_L2_phi->Sumw2();
  hn_slow_sel_EF_phi->Sumw2();
  hn_slow_ratio_phi_L1SEL->Sumw2();
  hn_slow_ratio_phi_L1L2->Sumw2();
  hn_slow_ratio_pt_L2EF->Sumw2();
  
  hn_sel_pt->Sumw2();
  hn_sel_L1_pt->Sumw2();
  hn_sel_L2_pt->Sumw2();
  hn_sel_EF_pt->Sumw2();
  hn_ratio_pt_L1SEL->Sumw2();
  hn_ratio_pt_L1L2->Sumw2();
  hn_ratio_pt_L2EF->Sumw2();
  hn_slow_sel_pt->Sumw2();
  hn_slow_sel_L1_pt->Sumw2();
  hn_slow_sel_L2_pt->Sumw2();
  hn_slow_sel_EF_pt->Sumw2();
  hn_slow_ratio_pt_L1SEL->Sumw2();
  hn_slow_ratio_pt_L1L2->Sumw2();
  hn_slow_ratio_pt_L2EF->Sumw2();
  
  hn_sel_eta->Sumw2();
  hn_sel_L1_eta->Sumw2();
  hn_sel_L2_eta->Sumw2();
  hn_sel_EF_eta->Sumw2();
  hn_ratio_eta_L1SEL->Sumw2();
  hn_ratio_eta_L1L2->Sumw2();
  hn_ratio_eta_L2EF->Sumw2();
  hn_slow_sel_eta->Sumw2();
  hn_slow_sel_L1_eta->Sumw2();
  hn_slow_sel_L2_eta->Sumw2();
  hn_slow_sel_EF_eta->Sumw2();
  hn_slow_ratio_eta_L1SEL->Sumw2();
  hn_slow_ratio_eta_L1L2->Sumw2();
  hn_slow_ratio_eta_L2EF->Sumw2();
  
  
  
  //Creating the histogram. Remember the name you give it (meff4), you will later retrieve it in the static method postAnalysis(TSelectorList* m_sel)
  
  //Registering the histogram with the D3PDSelector
  // WHY?
  // When using Proof, the D3PD Selecter adds this Object to the OutputList
  // Proof automatically merges these Objects at the end, and you get the Ouput list in the static method   postAnalysis(TSelectorList* fOutput)
  cout << " Adding TObj" << endl;
  
  m_sel->AddTObject(meff);
  m_sel->AddTObject(hNJets);
  m_sel->AddTObject(hNOkJets);
  m_sel->AddTObject(hCutFlow);
  m_sel->AddTObject(hJet1pT);
  m_sel->AddTObject(hJet2pT);
  m_sel->AddTObject(hJet3pT);
  m_sel->AddTObject(hJet4pT);
  m_sel->AddTObject(hJet1Eta);
  m_sel->AddTObject(hJet2Eta);
  m_sel->AddTObject(hJet3Eta);
  m_sel->AddTObject(hJet4Eta);
  m_sel->AddTObject(hJet1Phi);
  m_sel->AddTObject(hJet2Phi);
  m_sel->AddTObject(hJet3Phi);
  m_sel->AddTObject(hJet4Phi);
  m_sel->AddTObject(hJet1Mass);
  m_sel->AddTObject(hJet2Mass);
  m_sel->AddTObject(hJet3Mass);
  m_sel->AddTObject(hJet4Mass);
  m_sel->AddTObject(hJet1FlavorWeight);
  m_sel->AddTObject(hJet2FlavorWeight);
  m_sel->AddTObject(hJet3FlavorWeight);
  m_sel->AddTObject(hJet4FlavorWeight);
  m_sel->AddTObject(hJet1dRTruthB);
  m_sel->AddTObject(hJet2dRTruthB);
  m_sel->AddTObject(hJet3dRTruthB);
  m_sel->AddTObject(hJet4dRTruthB);
  m_sel->AddTObject(hdRJet1Jet2);
  m_sel->AddTObject(hdRJet1Jet3);
  m_sel->AddTObject(hdRJet1Jet4);
  m_sel->AddTObject(hdRJet2Jet3);
  m_sel->AddTObject(hdRJet2Jet4);
  m_sel->AddTObject(hdRJet3Jet4);
  
  m_sel->AddTObject(hRecoElectrons);
  m_sel->AddTObject(hRecoOkElectrons);
  m_sel->AddTObject(hRecoEleInvMass);
  m_sel->AddTObject(hElectron1pT);
  m_sel->AddTObject(hElectron2pT);
  m_sel->AddTObject(hElectron3pT);
  m_sel->AddTObject(hElectron1Eta);
  m_sel->AddTObject(hElectron2Eta);
  m_sel->AddTObject(hElectron3Eta);
  m_sel->AddTObject(hElectron1Phi);
  m_sel->AddTObject(hElectron2Phi);
  m_sel->AddTObject(hElectron3Phi);
  m_sel->AddTObject(hElectron1Mass);
  m_sel->AddTObject(hElectron2Mass);
  m_sel->AddTObject(hElectron3Mass);
  m_sel->AddTObject(hdRElectron1Electron2);
  m_sel->AddTObject(hdRElectron1Electron3);
  m_sel->AddTObject(hdRElectron2Electron3);
  m_sel->AddTObject(hdRElectron1Muon1);
  m_sel->AddTObject(hdRElectron1Muon2);
  //  m_sel->AddTObject(hdRElectron1Tau1);
  //  m_sel->AddTObject(hdRElectron1Tau2);
  m_sel->AddTObject(hdRElectron2Muon1);
  m_sel->AddTObject(hdRElectron2Muon2);
  //  m_sel->AddTObject(hdRElectron2Tau1);
  //  m_sel->AddTObject(hdRElectron2Tau2);
  m_sel->AddTObject(hElectronTruthType);
  m_sel->AddTObject(hElectronBSTruthType);
  
  m_sel->AddTObject(hRecoMuons);
  m_sel->AddTObject(hRecoOkMuons);
  m_sel->AddTObject(hMuon1pT);
  m_sel->AddTObject(hMuon2pT);
  m_sel->AddTObject(hMuon3pT);
  m_sel->AddTObject(hMuon1Eta);
  m_sel->AddTObject(hMuon2Eta);
  m_sel->AddTObject(hMuon3Eta);
  m_sel->AddTObject(hMuon1Phi);
  m_sel->AddTObject(hMuon2Phi);
  m_sel->AddTObject(hMuon3Phi);
  m_sel->AddTObject(hMuon1Mass);
  m_sel->AddTObject(hMuon2Mass);
  m_sel->AddTObject(hMuon3Mass);
  m_sel->AddTObject(hMuon1Beta);
  m_sel->AddTObject(hMuon2Beta);
  m_sel->AddTObject(hMuon3Beta);
  m_sel->AddTObject(hdRMuon1Muon2);
  m_sel->AddTObject(hdRMuon1Muon3);
  m_sel->AddTObject(hdRMuon2Muon3);
  //  m_sel->AddTObject(hdRMuon1Tau1);
  //  m_sel->AddTObject(hdRMuon1Tau2);
  //  m_sel->AddTObject(hdRMuon2Tau1);
  //  m_sel->AddTObject(hdRMuon2Tau2);
  m_sel->AddTObject(hMuonTruthType);
  m_sel->AddTObject(hMuonBSTruthType);
  
  m_sel->AddTObject(hRecoMuLowBetaMuons);
  m_sel->AddTObject(hRecoOkMuLowBetaMuons);
  m_sel->AddTObject(hMuLowBetaMuon1pT);
  m_sel->AddTObject(hMuLowBetaMuon2pT);
  m_sel->AddTObject(hMuLowBetaMuon3pT);
  m_sel->AddTObject(hMuLowBetaMuon1Eta);
  m_sel->AddTObject(hMuLowBetaMuon2Eta);
  m_sel->AddTObject(hMuLowBetaMuon3Eta);
  m_sel->AddTObject(hMuLowBetaMuon1Phi);
  m_sel->AddTObject(hMuLowBetaMuon2Phi);
  m_sel->AddTObject(hMuLowBetaMuon3Phi);
  m_sel->AddTObject(hMuLowBetaMuon1Mass);
  m_sel->AddTObject(hMuLowBetaMuon2Mass);
  m_sel->AddTObject(hMuLowBetaMuon3Mass);
  m_sel->AddTObject(hMuLowBetaMuon1MassBeta);
  m_sel->AddTObject(hMuLowBetaMuon2MassBeta);
  m_sel->AddTObject(hMuLowBetaMuon3MassBeta);
  m_sel->AddTObject(hMuLowBetaMuon1Beta);
  m_sel->AddTObject(hMuLowBetaMuon2Beta);
  m_sel->AddTObject(hMuLowBetaMuon3Beta);
  m_sel->AddTObject(hMuLowBetaMuonTruthType);
  m_sel->AddTObject(hMuLowBetaMuonBSTruthType);
  
  m_sel->AddTObject(hRecoMuGirlMuons);
  m_sel->AddTObject(hRecoOkMuGirlMuons);
  m_sel->AddTObject(hMuGirlMuon1pT);
  m_sel->AddTObject(hMuGirlMuon2pT);
  m_sel->AddTObject(hMuGirlMuon3pT);
  m_sel->AddTObject(hMuGirlMuon1Eta);
  m_sel->AddTObject(hMuGirlMuon2Eta);
  m_sel->AddTObject(hMuGirlMuon3Eta);
  m_sel->AddTObject(hMuGirlMuon1Phi);
  m_sel->AddTObject(hMuGirlMuon2Phi);
  m_sel->AddTObject(hMuGirlMuon3Phi);
  m_sel->AddTObject(hMuGirlMuon1Mass);
  m_sel->AddTObject(hMuGirlMuon2Mass);
  m_sel->AddTObject(hMuGirlMuon3Mass);
  m_sel->AddTObject(hMuGirlMuon1MassBeta);
  m_sel->AddTObject(hMuGirlMuon2MassBeta);
  m_sel->AddTObject(hMuGirlMuon3MassBeta);
  m_sel->AddTObject(hMuGirlMuon1Beta);
  m_sel->AddTObject(hMuGirlMuon2Beta);
  m_sel->AddTObject(hMuGirlMuon3Beta);
  m_sel->AddTObject(hMuGirlMuonTruthType);
  m_sel->AddTObject(hMuGirlMuonBSTruthType);
  
  //  m_sel->AddTObject(hRecoTaus);
  //  m_sel->AddTObject(hRecoOkTaus);
  //  m_sel->AddTObject(hTau1pT);
  //  m_sel->AddTObject(hTau2pT);
  //  m_sel->AddTObject(hTau3pT);
  //  m_sel->AddTObject(hTau1Eta);
  //  m_sel->AddTObject(hTau2Eta);
  //  m_sel->AddTObject(hTau3Eta);
  //  m_sel->AddTObject(hTau1Phi);
  //  m_sel->AddTObject(hTau2Phi);
  //  m_sel->AddTObject(hTau3Phi);
  //  m_sel->AddTObject(hTau1Mass);
  //  m_sel->AddTObject(hTau2Mass);
  //  m_sel->AddTObject(hTau3Mass);
  //  m_sel->AddTObject(hdRTau1Tau2);
  //  m_sel->AddTObject(hdRTau1Tau3);
  //  m_sel->AddTObject(hdRTau2Tau3);
  
  m_sel->AddTObject(hRecoOkLeptons);
  m_sel->AddTObject(hMET);
  m_sel->AddTObject(hMETPhi);  
  
  m_sel->AddTObject(hNHiggs);
  m_sel->AddTObject(lshMassHiggs);
  m_sel->AddTObject(lshMassZ);
  
  m_sel->AddTObject(lshPTb1);
  m_sel->AddTObject(lshPTb2);
  m_sel->AddTObject(lshPTb3);
  m_sel->AddTObject(lshPTb4);
  m_sel->AddTObject(lshJet1Match);
  m_sel->AddTObject(lshJet2Match);
  m_sel->AddTObject(lshJetsMass34);
  m_sel->AddTObject(lshJetsNewMass34);
  m_sel->AddTObject(lshRecoJJInvMass);
  m_sel->AddTObject(lshRecoBBInvMass);
  m_sel->AddTObject(lshRecoEleInvMass);

  m_sel->AddTObject(hdRMinDilep);
  m_sel->AddTObject(hdRMinDilepInvMass);
  m_sel->AddTObject(h2dInvMassMindRDilep);
  m_sel->AddTObject(hdRDilep);
  m_sel->AddTObject(hDilepInvMass);
  m_sel->AddTObject(h2dInvMassdRDilep);

  m_sel->AddTObject(hdRMinLeptons);
  m_sel->AddTObject(hMassMindRLeptons);
  m_sel->AddTObject(h2dMassMindRLeptons);
  m_sel->AddTObject(hdRLeptons);
  m_sel->AddTObject(hMassdRLeptons);
  m_sel->AddTObject(h2dMassdRLeptons);
	
  m_sel->AddTObject(lshJetFlavor1);
  m_sel->AddTObject(lshJetFlavor2);
  m_sel->AddTObject(lshJetFlavor3);
  m_sel->AddTObject(lshJetFlavor4);
  m_sel->AddTObject(lshJetFlavor5);
  m_sel->AddTObject(lshJetFlavor6);
  m_sel->AddTObject(lshJetFlavorMatch1);
  m_sel->AddTObject(lshJetFlavorMatch2);
  m_sel->AddTObject(lshJetWeight1);
  m_sel->AddTObject(lshJetWeight2);
  m_sel->AddTObject(lshJetWeight3);
  m_sel->AddTObject(lshJetWeight4);
  m_sel->AddTObject(lshJetWeight5);
  m_sel->AddTObject(lshJetWeight6);
  m_sel->AddTObject(lshJetWeightMatch1);
  m_sel->AddTObject(lshJetWeightMatch2);
  m_sel->AddTObject(lshMinus);
  m_sel->AddTObject(lshEMinus);
  m_sel->AddTObject(lshMuMinus);
  //	m_sel->AddTObject(lshTauMinus);
  m_sel->AddTObject(lshPlus);
  m_sel->AddTObject(lshEPlus);
  m_sel->AddTObject(lshMuPlus);
  //	m_sel->AddTObject(lshTauPlus);
  m_sel->AddTObject(lshLepton);
  m_sel->AddTObject(lsh4);
  m_sel->AddTObject(lsh44);
  m_sel->AddTObject(lsh5);
  m_sel->AddTObject(lsh55);
  m_sel->AddTObject(lsh6);
  m_sel->AddTObject(lsh66);
  m_sel->AddTObject(lshMassChi0e);
  m_sel->AddTObject(lshMassChi0m);
  m_sel->AddTObject(lshMassChi0t);
  m_sel->AddTObject(lshChi02Mass);
  m_sel->AddTObject(lshChi01Mass);
  m_sel->AddTObject(lshChi03Mass);
  
  
  //TRIGGER STUDIES
  m_sel->AddTObject(hn_sel_pt);
  m_sel->AddTObject(hn_sel_L1_pt);
  m_sel->AddTObject(hn_sel_L2_pt);
  m_sel->AddTObject(hn_sel_EF_pt);
  m_sel->AddTObject(hn_ratio_pt_L1SEL);
  m_sel->AddTObject(hn_ratio_pt_L1L2);
  m_sel->AddTObject(hn_ratio_pt_L2EF);
  m_sel->AddTObject(hn_slow_sel_pt);
  m_sel->AddTObject(hn_slow_sel_L1_pt);
  m_sel->AddTObject(hn_slow_sel_L2_pt);
  m_sel->AddTObject(hn_slow_sel_EF_pt);
  m_sel->AddTObject(hn_slow_ratio_pt_L1SEL);
  m_sel->AddTObject(hn_slow_ratio_pt_L1L2);
  m_sel->AddTObject(hn_slow_ratio_pt_L2EF);
  
  m_sel->AddTObject(hn_sel_eta);
  m_sel->AddTObject(hn_sel_L1_eta);
  m_sel->AddTObject(hn_sel_L2_eta);
  m_sel->AddTObject(hn_sel_EF_eta);
  m_sel->AddTObject(hn_ratio_eta_L1SEL);
  m_sel->AddTObject(hn_ratio_eta_L1L2);
  m_sel->AddTObject(hn_ratio_eta_L2EF);
  m_sel->AddTObject(hn_slow_sel_eta);
  m_sel->AddTObject(hn_slow_sel_L1_eta);
  m_sel->AddTObject(hn_slow_sel_L2_eta);
  m_sel->AddTObject(hn_slow_sel_EF_eta);
  m_sel->AddTObject(hn_slow_ratio_eta_L1SEL);
  m_sel->AddTObject(hn_slow_ratio_eta_L1L2);
  m_sel->AddTObject(hn_slow_ratio_eta_L2EF);
  
  m_sel->AddTObject(hn_sel_phi);
  m_sel->AddTObject(hn_sel_L1_phi);
  m_sel->AddTObject(hn_sel_L2_phi);
  m_sel->AddTObject(hn_sel_EF_phi);
  m_sel->AddTObject(hn_ratio_phi_L1SEL);
  m_sel->AddTObject(hn_ratio_phi_L1L2);
  m_sel->AddTObject(hn_ratio_phi_L2EF);
  m_sel->AddTObject(hn_slow_sel_phi);
  m_sel->AddTObject(hn_slow_sel_L1_phi);
  m_sel->AddTObject(hn_slow_sel_L2_phi);
  m_sel->AddTObject(hn_slow_sel_EF_phi);
  m_sel->AddTObject(hn_slow_ratio_phi_L1SEL);
  m_sel->AddTObject(hn_slow_ratio_phi_L1L2);
  m_sel->AddTObject(hn_slow_ratio_phi_L2EF);
  
  m_sel->AddTObject(hn_sel_eta_phi);
  m_sel->AddTObject(hn_sel_L1_eta_phi);
  m_sel->AddTObject(hn_sel_L2_eta_phi);
  m_sel->AddTObject(hn_sel_EF_eta_phi);
  m_sel->AddTObject(hn_ratio_eta_phi_L1SEL);
  m_sel->AddTObject(hn_ratio_eta_phi_L1L2);
  m_sel->AddTObject(hn_ratio_eta_phi_L2EF);
  m_sel->AddTObject(hn_slow_sel_eta_phi);
  m_sel->AddTObject(hn_slow_sel_L1_eta_phi);
  m_sel->AddTObject(hn_slow_sel_L2_eta_phi);
  m_sel->AddTObject(hn_slow_sel_EF_eta_phi);
  m_sel->AddTObject(hn_slow_ratio_eta_phi_L1SEL);
  m_sel->AddTObject(hn_slow_ratio_eta_phi_L1L2);
  m_sel->AddTObject(hn_slow_ratio_eta_phi_L2EF);
  
  //resetting cutflow variables
  for (int cf = 0; cf < 10 ; cf++)
    {
      m_cutflow[cf] = 0.0;
    }
  m_cutname[0]="no cut";
  m_cutname[1]="After Selection";
  m_cutname[2]="no cut";
  m_cutname[3]="NJET MIN";
  m_cutname[4]="Jets pT cut";
  m_cutname[5]="NLeptons";
  m_cutname[6]="N Low Beta Muons";
  m_cutname[7]="no cut";
  m_cutname[8]="Trigger";
  m_cutname[9]="No cut";

  //0: no cut
  //1: crack veto
  //2: == 1 lepton
  //3: >= 4 jets
  //4: > jet_pt 40, 30, 30, 30
  //5: > EtMiss > 30
  //6: EtMiss > 0.2 Meff
  //7: ST> 0.2
  //8: dPhiMin > 0.2
  //9: MT> 100
  eventsProcessed=0;
  eventsPassed=0;
  nOneHiggsPerEvent=0;
  nTwoHiggsPerEvent=0;
  triggerPassed["L1_MU10"]=0;
  triggerPassed_1Higgs["L1_MU10"]=0;
  triggerPassed_2Higgs["L1_MU10"]=0;
  triggerPassed["L2_mu40_slow"]=0;
  triggerPassed_1Higgs["L2_mu40_slow"]=0;
  triggerPassed_2Higgs["L2_mu40_slow"]=0;
  triggerPassed["EF_mu40_slow"]=0;
  triggerPassed_1Higgs["EF_mu40_slow"]=0;
  triggerPassed_2Higgs["EF_mu40_slow"]=0;
  triggerPassed["mu40_slow"]=0;
  triggerPassed_1Higgs["mu40_slow"]=0;
  triggerPassed_2Higgs["mu40_slow"]=0;
  
  m_evs = new TCanvas("events", "events", 1920, 1080);
  m_evs->Divide(4,3);
  counter = 0;
}

unsigned int Analysis::Pre_OPS_OR(Long64_t entry)
{
  // Method executed from Process in D3PDSelector, before OPS and OR
  
  //gettign mc event weight
  double mc_event_weight;
  if (!REALDATA)
    {
      std::vector<double> mcevt_weight = m_sel->GetRawVector< std::vector<double> >( "mcevt_weight" );
      mc_event_weight = mcevt_weight.at(0);
    }
  else
    {
      mc_event_weight = 1.0;
    }
  
  m_cutflow[0] += (float)mc_event_weight; //cutflow entry for no cut 
  hCutFlow->Fill(0);

  
  if (entry == 7076)
    {
      drawEtaPhi(entry,1); //for event 189, we draw a eta/phi plot before OPS. 
      //Drawing at this stage only works if we are not using PROOF
    }
  
  
  return KEEP_EVENT; 
  
}



unsigned int Analysis::Between_OPS_and_OR(Long64_t entry) 
{
  //Method called Between OPS and OR
  if (entry == 7076)
    {
      drawEtaPhi(entry,2);//for event 189, we draw a eta/phi plot between OPS and OR
      //Drawing at this stage only works if we are not using PROOF
    }
  
  return KEEP_EVENT;
}


void Analysis::doAnalysis(Long64_t entry)
{
  
  eventsProcessed++;  
  Int_t debug=1;
  if (debug >4) std::cout << "Entry " << entry << " in Analysis " << std::endl;
  //Finally the Analysis method, called just after OR
  //First, I'll show you how to get variables. What are the available variables and their type?
  //You can either check with a TBrowser, but this code also outputs all variables at the beginning of the Analysis when running without proof!
  
  //This version of the D3PD reader was designed to support multiple type of e.g. jet objects
  //In the constructor of this class, you told the Selector which prefix to use for jet etc.
  //data needs to be fetched specifiying the type, eg. ELE, MUO, JET, PHO. internally, the selector will translate to the correct prefix. The advantage is, that when the prefix changes, not the whole code needs to be be changed.
  //if you don't want to take advantage of this thing (but then you loose the built-in overlap removal and ops), you can always get the data via GetRawVal(..) and GetRawVector(...)
  
  
  //What is the smartest way to get the number of electrons? (See RECOMMENDED)
  //Since the number of electron candidates is different before OPS, between OPS and OR, and after OR, we need to be careful. 
  //First, lets get the original number of electrons before OPS
  unsigned int n_el_b = m_sel->GetSizeByType("ELE"); //RECOMMENDED
  unsigned int n_mu_b = m_sel->GetSizeByType("MUO"); //RECOMMENDED
  unsigned int n_mugirl_b = m_sel->GetSizeByType("MUGIRL"); //RECOMMENDED
  unsigned int n_mulowbeta_b = m_sel->GetSizeByType("MULOWBETA"); //RECOMMENDED
  //  unsigned int n_ph_b = m_sel->GetSizeByType("PHO"); //RECOMMENDED
  //  unsigned int n_tau_b = m_sel->GetSizeByType("TAU"); //RECOMMENDED
  unsigned int n_jet_b = m_sel->GetSizeByType("JET"); //RECOMMENDED
  
  
  if (debug >4) std::cout << "#el(before) " << n_el_b <<  std::endl;
  if (debug >4) std::cout << "#mu(before) " << n_mu_b <<  std::endl;
  if (debug >4) std::cout << "#mugirl(before) " << n_mugirl_b <<  std::endl;
  if (debug >4) std::cout << "#mulowbeta(before) " << n_mulowbeta_b <<  std::endl;
  if (debug >4) std::cout << "#jet(before) " << n_jet_b <<  std::endl;
  //  if (debug >4) std::cout << "#ph(before) " << n_ph_b <<  std::endl;
  //  if (debug >4) std::cout << "#tau(before) " << n_tau_b <<  std::endl;
  
    
  if (entry == 7076)
    {
      drawEtaPhi(entry,3); //for event 189, we draw a eta/phi plot after OPS and OR
    }
    
  if (debug >4) std::cout << " Now sorting by pt " << std::endl;
  //also sort needs to know what kind of variable we are sorting by, here e.g. float
  m_sel->SortFinBy<float>("JET", "_pt"); //sort descending
  m_sel->SortFinBy<float>("MUO", "_pt"); //sort descending
  m_sel->SortFinBy<float>("ELE", "_pt"); //sort descending
  //  m_sel->SortFinBy<float>("TAU", "_pt"); //sort descending
  m_sel->SortFinBy<float>("MUGIRL", "_pt"); //sort descending
  m_sel->SortFinBy<float>("MULOWBETA", "_pt"); //sort descending
  
  
  //we need to reget the vectors
  unsigned int n_jet_f=m_sel->GetFinSizeByType("JET");
  std::vector<TLorentzVector> jet_f;
  //  std::vector<float> jet_flavorweight_f = m_sel->GetFinVectorByType< std::vector<float> >("JET", "_flavor_weight");
  //  std::vector<float> jet_flavor_truth_trueflav;
  //  std::vector<float> jet_TruthdRminB_f;
  //  if (!REALDATA){
  //    jet_flavor_truth_trueflav = m_sel->GetFinVectorByType< std::vector<float> >("JET", "_flavor_truth_trueflav");
  //    jet_TruthdRminB_f = m_sel->GetFinVectorByType< std::vector<float> >("JET", "_flavor_truth_dRminToB");
  //  }
  std::vector<float> jet_m_f = m_sel->GetFinVectorByType< std::vector<float> >("JET", "_m");
  
  for ( unsigned int i = 0; i < n_jet_f; i++){
    jet_f.push_back(m_sel->GetFinFourVecByType("JET",i));
  }  
  
  
  unsigned int n_el_f=m_sel->GetFinSizeByType("ELE");
  std::vector<TLorentzVector> el_f;
  std::vector<TLorentzVector> lep_f;
  std::vector<float> el_charge = m_sel->GetFinVectorByType< std::vector<float> >("ELE", "_charge");
  
  std::vector<int> el_truth_type;
  std::vector<int> mu_truth_type;
  std::vector<int> mugirl_truth_type;
  std::vector<int> mulowbeta_truth_type;
  std::vector<int> el_truth_barcode;
  std::vector<int> mu_truth_barcode;
  std::vector<int> mugirl_truth_barcode;
  std::vector<int> mulowbeta_truth_barcode;	
  if (!REALDATA) {
    el_truth_type = m_sel->GetFinVectorByType< std::vector<int> >("ELE", "_truth_type");
    mu_truth_type = m_sel->GetFinVectorByType< std::vector<int> >("MUO", "_truth_type");
    mugirl_truth_type = m_sel->GetFinVectorByType< std::vector<int> >("MUGIRL", "_truth_type");
    mulowbeta_truth_type = m_sel->GetFinVectorByType< std::vector<int> >("MULOWBETA", "_truth_type");
    el_truth_barcode = m_sel->GetFinVectorByType< std::vector<int> >("ELE", "_truth_barcode");
    mu_truth_barcode = m_sel->GetFinVectorByType< std::vector<int> >("MUO", "_truth_barcode");
    mugirl_truth_barcode = m_sel->GetFinVectorByType< std::vector<int> >("MUGIRL", "_truth_barcode");
    mulowbeta_truth_barcode = m_sel->GetFinVectorByType< std::vector<int> >("MULOWBETA", "_truth_barcode");
  }  
  std::vector<string> lep_mytype;
  std::vector<float> lep_charge;
  std::vector<int> lep_truth_type;
  std::vector<int> lep_truth_barcode;
  
  for ( unsigned int i = 0; i < n_el_f; i++){
    el_f.push_back(m_sel->GetFinFourVecByType("ELE",i));
    lep_f.push_back(m_sel->GetFinFourVecByType("ELE",i));
    lep_charge.push_back(el_charge.at(i));
    lep_mytype.push_back("el");
    if (!REALDATA){
      lep_truth_type.push_back(el_truth_type.at(i));
      lep_truth_barcode.push_back(el_truth_barcode.at(i));
    }
  }  
  
  unsigned int n_mu_f=m_sel->GetFinSizeByType("MUO");
  std::vector<TLorentzVector> mu_f;
  std::vector<float> mu_beta_f = m_sel->GetFinVectorByType< std::vector<float> >("MUO", "_beta");  
  std::vector<float> mu_charge = m_sel->GetFinVectorByType< std::vector<float> >("MUO", "_charge");
  for ( unsigned int i = 0; i < n_mu_f; i++){
    mu_f.push_back(m_sel->GetFinFourVecByType("MUO",i));
    lep_f.push_back(m_sel->GetFinFourVecByType("MUO",i));
    lep_charge.push_back(mu_charge.at(i));
    lep_mytype.push_back("mu");
    
    if (!REALDATA){
      lep_truth_type.push_back(mu_truth_type.at(i));
      lep_truth_barcode.push_back(mu_truth_barcode.at(i));
    }
  }
  
  
  unsigned int n_mugirl_f=m_sel->GetFinSizeByType("MUGIRL");
  std::vector<TLorentzVector> mugirl_f;
  std::vector<float> mugirl_beta_f = m_sel->GetFinVectorByType< std::vector<float> >("MUGIRL", "_beta");  
  std::vector<float> mugirl_charge = m_sel->GetFinVectorByType< std::vector<float> >("MUGIRL", "_charge");  
  for ( unsigned int i = 0; i < n_mugirl_f; i++){
    mugirl_f.push_back(m_sel->GetFinFourVecByType("MUGIRL",i));
    //    lep_f.push_back(m_sel->GetFinFourVecByTypeMass("MUGIRL",i));  
    //    lep_charge.push_back(mugirl_charge.at(i));
    
  }
  
  unsigned int n_mulowbeta_f=m_sel->GetFinSizeByType("MULOWBETA");
  std::vector<TLorentzVector> mulowbeta_f;
  std::vector<float> mulowbeta_beta_f = m_sel->GetFinVectorByType< std::vector<float> >("MULOWBETA", "_beta");  
  std::vector<float> mulowbeta_charge = m_sel->GetFinVectorByType< std::vector<float> >("MULOWBETA", "_charge");  
  for ( unsigned int i = 0; i < n_mulowbeta_f; i++){
    mulowbeta_f.push_back(m_sel->GetFinFourVecByType("MULOWBETA",i));
    lep_f.push_back(m_sel->GetFinFourVecByTypeMass("MULOWBETA",i));  
    lep_charge.push_back(mulowbeta_charge.at(i));
    lep_mytype.push_back("mulowb");
    
    if (!REALDATA){
      lep_truth_type.push_back(mulowbeta_truth_type.at(i));
      lep_truth_barcode.push_back(mulowbeta_truth_barcode.at(i));
    }
    
  }
  
  
  //  //  unsigned int n_tau_f=m_sel->GetFinSizeByType("TAU");
  //  std::vector<TLorentzVector> tau_f;
  //  std::vector<float> tau_charge = m_sel->GetFinVectorByType< std::vector<float> >("TAU", "_charge");
  
  //  for ( unsigned int i = 0; i < n_tau_f; i++){
  //    tau_f.push_back(m_sel->GetFinFourVecByTypeMass("TAU",i));  /// The energy of the taus is not well defined
  // if you want to remove the taus from the definition of leptons comment out this line
  //    lep_f.push_back(m_sel->GetFinFourVecByTypeMass("TAU",i));  
  //    lep_charge.push_back(tau_charge.at(i));
  //  }
  unsigned int n_lep_f = lep_f.size();
  
  // MET variables
  float met = m_sel->GetScalarVal<float>("MET_RefFinal_et");
  float met_phi = m_sel->GetScalarVal<float>("MET_RefFinal_phi");
  
  if (debug >4) std::cout << "Vectors are full " <<  std::endl;
  bool  L1_MU10 = m_sel->GetScalarVal<bool>("L1_MU10");
  bool  L2_mu40_slow=m_sel->GetScalarVal<bool>("L2_mu40_slow");
  //  if (!REALDATA) L2_mu40_slow=m_sel->GetScalarVal<bool>("L2_mu40_slow");
  bool  EF_mu40_slow;
  if (!REALDATA)  EF_mu40_slow=m_sel->GetScalarVal<bool>("EF_mu40_slow");
  //Let's do some useful stuff. Let's calculate meff4
  
  //First we get the pt container for the jets (Notice: We now get the Fin-type values)
  
  
  
  //  for (int q =0; q < jet_f.size(); q++)
  //  {
  //  	std::cout << "jet_pT " << q << ": " << jet_f.at(1).Pt() << std::endl;
  //  	std::cout << "jet_Eta " << q << ": " << jet_f.at(q).Eta() << std::endl;    
  //  }
  //  std::cout << std::endl;    
  
  //Final comment: with the "Get..BYType", you are bound to the Objects defined (JET, ELE, MUO, PHO). 
  //You can always get the raw vectors from the ntuple with the follwoing methods
  
  //  std::vector< float > el_eta = m_sel->GetRawVector< std::vector<float> >("el_eta");
  //NOTE: Don't forget to give full contianer name and template type argument!
  
  /*  if (!REALDATA)
      {
      //GetRawVector can be used for any type, also vect of vect of int
      vector<vector<int> > out = m_sel->GetRawVector< vector<vector<int> > >("mc_children");
      std::cout << "printin children info" << std::endl;
      for (unsigned int z = 0; z < out.size(); z++)
      {
      vector<int> la = out.at(z);
      for (unsigned int x = 0; x< la.size(); x++)
      {
      std::cout << "[" << z << "][" << x << "]:" << la.at(x) << std::endl;
      }
      } 
      }
  */
  //Now we perform cutflow study
  //MC Weight 
  double mc_event_weight;
  //  if (!REALDATA)
  if (0)
    {
      std::vector<double> mcevt_weight = m_sel->GetRawVector< std::vector<double> >( "mcevt_weight" );
      mc_event_weight = mcevt_weight.at(0);
    }
  else
    {
      mc_event_weight = 1.0;
    }
  //end Weight
  m_cutflow[1]+= (float)mc_event_weight; //events after crack veto
  hCutFlow->Fill(1);

  if (debug >4) std::cout << "Got event weight " <<  std::endl;

  
  m_cutflow[2]+= (float)mc_event_weight; // exactly one lepton < 20 GeV
  hCutFlow->Fill(2);

  //N Jet cuts
  if (n_jet_f < NJETMIN) return ;
  
  m_cutflow[3]+= (float)mc_event_weight; // four jets required
  hCutFlow->Fill(3);

  if (debug >4) std::cout << "selection number of jets " <<  std::endl;
  if (debug >4)  std::cout << "number of jets " << n_jet_f << std::endl;
  
  if ( n_jet_f > 0) 
    if (jet_f.at(0).Pt() < JETPT1_CUT && NJETMIN > 0 ) return ;
  if (debug >4)  std::cout << "checked jet 1" << std::endl;
  if ( n_jet_f > 1) 
    if (jet_f.at(1).Pt() < JETPT2_CUT && NJETMIN > 1 ) return ;
  if (debug >4)  std::cout << "checked jet 2" << std::endl;
  if ( n_jet_f > 2) 
    if ( jet_f.at(2).Pt() < JETPT3_CUT && NJETMIN > 2 ) return ;
  if (debug >4) std::cout << "checked jet 3" << std::endl;
  if ( n_jet_f > 3) 
    if ( jet_f.at(3).Pt() < JETPT4_CUT && NJETMIN > 3 ) return ;
  if (debug >4) std::cout << "checked jet 4 " <<  std::endl;
  
  m_cutflow[4]+= (float)mc_event_weight; //jet pt cuts
  hCutFlow->Fill(4);

  

  //  if (debug >4) std::cout << "NLeptons:  "<<n_lep_f << "     nel: " << n_el_f <<"     nmu: " << n_mu_f <<"     nlowbeta: " << n_mulowbeta_f <<"     nmug: " << n_mugirl_f << std::endl;
  if (debug >4) std::cout << "NLeptons:  "<<n_lep_f << "     nel: " << n_el_f <<"     nmu: " << n_mu_f <<"     nlowbeta: " << n_mulowbeta_f <<"     nmug: " << n_mugirl_f << "  NJETS: "<< n_jet_f <<std::endl;

  if ( n_lep_f < NLEPMIN ) return;
  
  //  if ( n_el_f <  NELEMIN ) return ;
  m_cutflow[5]+= (float)mc_event_weight; //MET cut
  hCutFlow->Fill(5);

  
  if ( n_mulowbeta_f < NSLOWMUOMIN ) return ;
  m_cutflow[6]+= (float)mc_event_weight; //MET cut
  hCutFlow->Fill(6);

   
  m_cutflow[7]+= (float)mc_event_weight; //ST cut
  hCutFlow->Fill(7);

  
  //  if ( deltaR(0,jet1.Phi(),0,met_phi) < 0.2 ) return ;
  //  if ( deltaR(0,jet2.Phi(),0,met_phi) < 0.2 ) return ;
  //  if ( deltaR(0,jet3.Phi(),0,met_phi) < 0.2 ) return ;
  //Require triggers
  if (REQTRIGGER) {
    if (!L1_MU10) return;
    if (!REALDATA)  // AT THE MOMENT REQUIRE THE LEVEL2 TRIGGER ONLY FOR MC AS IT IS NOT OPERATIONAL YET.
      if (!L2_mu40_slow) return;
  }
  
  m_cutflow[8]+= (float)mc_event_weight; //dRPhiMEt cut
  hCutFlow->Fill(8);

  
  //Transverse Mass between lepton and MET
  //  float MT = pow(2*(  met*leppt - metx*leppx - mety*leppy   ),0.5);
  //      if (MT < 100000.) return SKIP_EVENT;
  //  if (MT < 100000.) return;
  
  drawEtaPhi(entry, 3);
  
  if (debug >4) std::cout << "Finished additional cuts " <<  std::endl;
  
  eventsPassed++;
  
  // END OF CUT FLOW STUDY
  
  if (n_jet_f >= 4) //if we have enough jets...
    {
      //we sum four jet pt's...
      float ptsum = jet_f.at(0).Pt() + jet_f.at(1).Pt() + jet_f.at(2).Pt() + jet_f.at(3).Pt();
      //get the missing et...
    
      //sum the two...
      float MEFF = ptsum + met;
      MEFF/=1000;
    
      //..and fill the histogram
      meff->Fill(MEFF);
    }

  unsigned int higgsPerEvent=0;
  unsigned int HiggsWithB = 0;
  unsigned int HiggsWithBbar = 0;
  unsigned int bJetIndex=0;
  unsigned int bbarJetIndex=0;
  
  //  if (!REALDATA){
  if (0){
    
    // b-partons with Higgs mother PT
    Float_t PTb1,PTb2,PTb3,PTb4;
    Float_t Phib1,Phib2,Phib3,Phib4;
    // four momentum of b and bbar from Higgs or Z's
    Float_t bPX,bPY,bPZ,bE,bbarPX,bbarPY,bbarPZ,bbarE;
    Float_t bzPX,bzPY,bzPZ,bzE,bbarzPX,bbarzPY,bbarzPZ,bbarzE;
    Float_t ptb,ptbbar,ptbZ,ptbbarZ;

    // count how many same sign leptons are in the event
    Int_t nMinus = 0;
    Int_t nEMinus = 0;
    Int_t nMuMinus = 0;
    //     Int_t nTauMinus = 0;
    
    Int_t nPlus  = 0;
    Int_t nEPlus  = 0;
    Int_t nMuPlus  = 0;
    //    Int_t nTauPlus  = 0;
    
    Int_t nLeptons = 0;
    
    // Higgs Children momenta
    
    Float_t mom1_pt=-9999.,mom1_eta=-9999.,mom1_phi=-9999.;
    //  Float_t mom1_pz=-9999.,mom1_m=-9999.,mom1_pdgId=-9999.,mom1_barcode=-9999.,mom1_status=-9999.; 
    Float_t mom2_pt=-9999.,mom2_eta=-9999.,mom2_phi=-9999.;
    //  Float_t mom2_pz=-9999.,mom2_m=-9999.,mom2_pdgId=-9999.,mom2_barcode=-9999.,mom2_status=-9999.; 
    Float_t mom3_pt=-9999.,mom3_eta=-9999.,mom3_phi=-9999.;
    //  Float_t mom3_pz=-9999.,mom3_m=-9999.,mom3_pdgId=-9999.,mom3_barcode=-9999.,mom3_status=-9999.; 
    Float_t mom4_pt=-9999.,mom4_eta=-9999.,mom4_phi=-9999.;
    //  Float_t mom4_pz=-9999.,mom4_m=-9999.,mom4_pdgId=-9999.,mom4_barcode=-9999.,mom4_status=-9999.; 
    
    // Neutralinos Masses from leptons-sleptons
    Float_t mass1 = -999;
    Float_t mass2 = -999 ;
    Float_t mass3 = -999 ;
    
    unsigned int mc_n = m_sel->GetScalarVal<Int_t>("mc_n");
    if (debug >4) std::cout << "nmc particles " << mc_n << std::endl;
    for (unsigned int j = 0 ; j < mc_n ; j++) {
      std::vector< int > mc_barcode = m_sel->GetRawVector< std::vector<int> >("mc_barcode");
      if(fabs(mc_barcode[j]) < 200000){
        std::vector< int > mc_pdgId = m_sel->GetRawVector< std::vector<int> >("mc_pdgId");
        std::vector< int > mc_status = m_sel->GetRawVector< std::vector<int> >("mc_status");
        vector<vector<int> > mc_children= m_sel->GetRawVector< vector<vector<int> > >("mc_children");
        vector<vector<int> > mc_parents= m_sel->GetRawVector< vector<vector<int> > >("mc_parents");
        std::vector< float > mc_pt = m_sel->GetRawVector< std::vector<float> >("mc_pt");
	//        std::vector< float > mc_px = m_sel->GetRawVector< std::vector<float> >("mc_px");
	//        std::vector< float > mc_py = m_sel->GetRawVector< std::vector<float> >("mc_py");
	//        std::vector< float > mc_pz = m_sel->GetRawVector< std::vector<float> >("mc_pz");
	//        std::vector< float > mc_E = m_sel->GetRawVector< std::vector<float> >("mc_E");
        std::vector< float > mc_m = m_sel->GetRawVector< std::vector<float> >("mc_m");
        
        if (fabs(mc_pdgId[j]) == 25 ) {
          if ( mc_status[j] == 2){// Higgs decays, so istat code 2
            higgsPerEvent++;
            
            Int_t	HiggsChildren = mc_children[j].size();
            if (HiggsChildren > 1){
	      
              for (Int_t k = 0; k < HiggsChildren; k++){
                // total momentum components summing all children....
                if(isBarcodePdgId(mc_children[j][k],5)){
                  if (debug >4) cout << "Children is b!!  " << endl;
                  HiggsWithB++ ;
                  if (higgsPerEvent == 1) {
                    mom1_pt=findMCpT(mc_children[j][k]);
                    mom1_eta=findMCeta(mc_children[j][k]);
                    mom1_phi=findMCphi(mc_children[j][k]);
                    PTb1 = mom1_pt/1000.;
                    Phib1= mom1_phi;
                  }
                  if (higgsPerEvent == 2) {
                    mom3_pt=findMCpT(mc_children[j][k]);
                    mom3_eta=findMCeta(mc_children[j][k]);
                    mom3_phi=findMCphi(mc_children[j][k]);
                    PTb3 = mom3_pt/1000;
                    Phib3= mom3_phi;
                  }
                } // if (isBarcodePdgId(mc_children[j][k],5))
                else if (isBarcodePdgId(mc_children[j][k],-5)) {
                  //		    cout << "Children is bbar!! " <<endl;
                  if (debug >4) cout << "Children is bbar!!  " << endl;
                  HiggsWithBbar++;
                  if (higgsPerEvent == 1){
                    mom2_pt=findMCpT(mc_children[j][k]);
                    mom2_eta=findMCeta(mc_children[j][k]);
                    mom2_phi=findMCphi(mc_children[j][k]);
                    PTb2 = mom2_pt/1000.;
                    Phib2= mom2_phi;
                  }
                  if (higgsPerEvent == 2){
                    mom4_pt=findMCpT(mc_children[j][k]);
                    mom4_eta=findMCeta(mc_children[j][k]);
                    mom4_phi=findMCphi(mc_children[j][k]);
                    PTb4 = mom2_pt/1000.;
                    Phib4= mom2_phi;
                  }
                } // else if (isBarcodePdgId(mc_children[j][k],-5))
                else if (mc_children[j][k] !=  5 && mc_children[j][k] > 0 ){
                  //std::cout << hepjda1[j][k] << std::endl; //it will not work in current code
                  //lshHiggsChild1a->Fill(hepjda1[j][k]); //it will not work in current code
                } // if (mc_children[j][k] !=  5 && mc_children[j][k] > 0 )
                else if (mc_children[j][k] !=  -5 && mc_children[j][k] < 0 ){
                  //std::cout << hepjda1[j][k] << std::endl; //it will not work in current code
                  //lshHiggsChild2a->Fill(hepjda1[j][k]);//it will not work in current code
                } // if (mc_children[j][k] !=  5 && mc_children[j][k] > 0 )
              } //  for (Int_t k = 0; k < HiggsChildren; k++)
            } // if (HiggsChildren > 1)
            
          }//if ( mc_status[j] == 2)
        }//if (fabs(mc_pdgId[j]) == 25 )
        
	//        if (debug >4) cout << "Processing bs" << endl;
        
        if (fabs(mc_pdgId[j]) == 5) {
          if ( mc_status[j] == 2){ // b's are not with istat 1, select 2 as from btagging old work... 
            for (unsigned int s = 0; s < mc_parents[j].size(); s++) { // loop on all parents
              if (mc_pdgId[j] == 5) {
                if (isBarcodePdgId(mc_parents[j][s],25)){
                  // ID of parents is Higgs
                  if (debug >4) cout << "b's mother is higgs  " << endl;
		  ptb=mc_pt[j];
		  //                  bPX = mc_px[j]; bPY = mc_py[j]; bPZ =mc_pz[j]; bE = mc_E[j]; 
                } // (isBarcodePdgId(mc_parents[j][s],25))
                
                if (isBarcodePdgId(mc_parents[j][s],23)){ // ID of parents  is Z
                  if (debug >4) cout << "b's mother is Z  " << endl;
		  ptbZ=mc_pt[j];
                  //bzPX = mc_px[j]; bzPY = mc_py[j]; bzPZ =mc_pz[j]; bzE = mc_E[j]; 
                } // (isBarcodePdgId(mc_parents[j][s],23))
                
              } //if (mc_pdgId[j] == 5)
              
              
              else  {
                if (isBarcodePdgId(mc_parents[j][s],25)){ // ID of mother is Higgs
                  if (debug >4) cout << "bbar's mother is higgs  " << endl;
                  ptbbar=mc_pt[j];
		  
		  //                  bbarPX = mc_px[j]; bbarPY = mc_py[j]; bbarPZ =mc_pz[j]; bbarE = mc_E[j];
                } // if (isBarcodePdgId(mc_parents[j][s],25)) 
                
                if (isBarcodePdgId(mc_parents[j][s],23)){ // ID of mother
                  if (debug >4) cout << "bbar's mother is Z  " << endl;
                  ptbbarZ=mc_pt[j];
		  //                  bbarzPX = mc_px[j]; bbarzPY = mc_py[j]; bbarzPZ =mc_pz[j]; bbarzE = mc_E[j];
                } // if (isBarcodePdgId(mc_parents[j][s],23))
              }//else 
            }//for (Int_t s = 0; s < mc_parents[j].size(); s++) 
          } // if ( mc_status[j] == 2) 
        } // if (fabs(mc_pdgId[j]) == 5)
        
        
        // Leptons      
        if ( (mc_pdgId[j] == 11 ||  mc_pdgId[j] == 13 ||  mc_pdgId[j] == 15)) {
          if (mc_status[j] == 1   ) {nMinus++; nLeptons++;}
          if (mc_status[j] == 1 && mc_pdgId[j] == 11 ) nEMinus++; 
          if (mc_status[j] == 1 && mc_pdgId[j] == 13 ) nMuMinus++;
	  //          if (mc_pdgId[j] == 15 && mc_status[j] == 2) nTauMinus++;// taus are decayed by TAUOLA
        } //if ( fabs(mc_pdgId[j]) == 11 || 13 || 15) 
        
        if ( (mc_pdgId[j] == -11 ||  mc_pdgId[j] == -13 ||  mc_pdgId[j] == -15) ) {
          if (mc_status[j] == 1){ nPlus++; nLeptons++ ;};
          if (mc_status[j] == 1 && mc_pdgId[j] == -11 ) nEPlus++; 
          if (mc_status[j] == 1 && mc_pdgId[j] == -13 ) nMuPlus++;
	  //          if (mc_pdgId[j] == -15 && mc_status[j] == 2) nTauPlus++; // taus are decayed by TAUOLA
        } //if ( fabs(mc_pdgId[j]) == 11 || 13 || 15 ) 
        
	//        if (debug >4) cout << "Processing sleptons  " << endl;
        
        // sleptons
        if (fabs(mc_pdgId[j]) == 2000011 && mc_status[j] == 1 ) lsh5->Fill(mc_m[j]/1000.0);
        if (fabs(mc_pdgId[j]) == 1000011 && mc_status[j] == 2 ) lsh55->Fill(mc_m[j]/1000.0);
        
        if (fabs(mc_pdgId[j]) == 2000013 && mc_status[j] == 1 ) lsh4->Fill(mc_m[j]/1000.0);
        if (fabs(mc_pdgId[j]) == 1000013 && mc_status[j] == 2 ) lsh44->Fill(mc_m[j]/1000.0);
        
        if (fabs(mc_pdgId[j]) == 1000015 && mc_status[j] == 1 ) lsh6->Fill(mc_m[j]/1000.0);
        if (fabs(mc_pdgId[j]) == 2000015 && mc_status[j] == 2 ) lsh66->Fill(mc_m[j]/1000.0);
        
        
	//        if (debug >4) cout << "Processing neutralino masses  " << endl;
        
        // Reco neutralino masses
        if ( fabs(mc_pdgId[j]) == 2000011 &&  mc_status[j] == 1 ) { //eR
          if (mc_pdgId[j] == 2000011 ) {
            Int_t foundEle = 0;
            for (unsigned int k = 0 ; k < mc_n ; k++) {
              if ( mc_pdgId[k] == -11 && mc_status[k] == 1) {
                foundEle++;
                if (foundEle == 1)
                  mass1 = 77.0;
		// sqrt((mc_E[j]+mc_E[k])*(mc_E[j]+mc_E[k])
		//                               -  (mc_px[j]+mc_px[k])*(mc_px[j]+mc_px[k])
		//                               -  (mc_py[j]+mc_py[k])*(mc_py[j]+mc_py[k])
		//                               -  (mc_pz[j]+mc_pz[k])*(mc_pz[j]+mc_pz[k]) )/1000.0;
              }// if ( mc_pdgId[j] == -11 && mc_status[j] == 1)
            }// for (Int_t k = 0 ; k < mc_n ; k++)
          } // if (mc_pdgId[j] == 2000011 )
          else if (mc_pdgId[j] == -2000011 ){
            Int_t foundEle = 0;
            for (unsigned int  k = 0 ; k < mc_n ; k++) {
              if ( mc_pdgId[k] == 11 && mc_status[k] == 1) {
                foundEle++;
                if (foundEle == 1)
                  mass1 = 22.0;
		//sqrt((mc_E[j]+mc_E[k])*(mc_E[j]+mc_E[k])
		//                               -  (mc_px[j]+mc_px[k])*(mc_px[j]+mc_px[k])
		//                               -  (mc_py[j]+mc_py[k])*(mc_py[j]+mc_py[k])
		//                               -  (mc_pz[j]+mc_pz[k])*(mc_pz[j]+mc_pz[k]) )/1000.0;
              }// if ( mc_pdgId[j] == -11 && mc_status[j] == 1)
            }// for (Int_t k = 0 ; k < mc_n ; k++)
          } // else if (mc_pdgId[j] == -2000011 )
        } // if ( fabs(mc_pdgId[j]) == 2000011 )
        
	//        if (debug >4) cout << "Processing muons " << endl;
        
        // Muons
        if ( fabs(mc_pdgId[j]) == 2000013 &&  mc_status[j] == 1 ) { //mu2
          //std::cout << "Found eR " << std::setw(15) << mc_pdgId[j] << std::endl;
          if (mc_pdgId[j] == 2000013 ) {
            Int_t foundEle = 0;
            for (unsigned int  k = 0 ; k < mc_n ; k++) {
              if ( mc_pdgId[k] == -13 && mc_status[k] == 1) {
                foundEle++; //std::cout << "Found ele " << std::setw(15) << mc_pdgId[k] << std::endl;
                if (foundEle == 1)
                  mass2 = 33;
		//sqrt((mc_E[j]+mc_E[k])*(mc_E[j]+mc_E[k])
		//                               -  (mc_px[j]+mc_px[k])*(mc_px[j]+mc_px[k])
		//                               -  (mc_py[j]+mc_py[k])*(mc_py[j]+mc_py[k])
		//                               -  (mc_pz[j]+mc_pz[k])*(mc_pz[j]+mc_pz[k]) )/1000.0;
                
              }// if ( mc_pdgId[j] == -13 && mc_status[j] == 1)
            }// for (Int_t k = 0 ; k < mc_n ; k++)
          } // if (mc_pdgId[j] == 2000013 )
          
          else if (mc_pdgId[j] == -2000013 ){
            Int_t foundEle = 0;
            for (unsigned int k = 0 ; k < mc_n ; k++) {
              
              if ( mc_pdgId[k] == 13 && mc_status[k] == 1) {
                foundEle++;
                if (foundEle == 1)
                  mass2 = 44;
		//sqrt((mc_E[j]+mc_E[k])*(mc_E[j]+mc_E[k])
     		//                               -  (mc_px[j]+mc_px[k])*(mc_px[j]+mc_px[k])
		//                          -  (mc_py[j]+mc_py[k])*(mc_py[j]+mc_py[k])
		//                               -  (mc_pz[j]+mc_pz[k])*(mc_pz[j]+mc_pz[k]) )/1000.0;
                
              }// if ( mc_pdgId[j] == -13 && mc_status[j] == 1)
              
            }// for (Int_t k = 0 ; k < mc_n ; k++)
          } // else if (mc_pdgId[j] == -2000013 )
        } // if ( fabs(mc_pdgId[j]) == 2000013 )
        
	//        if (debug >4) cout << "Processing taus  " << endl;
        
        if ( fabs(mc_pdgId[j]) == 1000015 &&  mc_status[j] == 1 ) { //sta1
          if (mc_pdgId[j] == 1000015 ) {
            Int_t foundEle = 0;
            for (unsigned int k = 0 ; k < mc_n ; k++) {
              if ( mc_pdgId[k] == -15 && mc_status[k] == 2) { // tau is decayed by tauola
                foundEle++;
                if (foundEle == 1)
                  mass3 = 66;
		//sqrt((mc_E[j]+mc_E[k])*(mc_E[j]+mc_E[k])
		//                               -  (mc_px[j]+mc_px[k])*(mc_px[j]+mc_px[k])
		//                               -  (mc_py[j]+mc_py[k])*(mc_py[j]+mc_py[k])
		//                               -  (mc_pz[j]+mc_pz[k])*(mc_pz[j]+mc_pz[k]) )/1000.0;
              }// if ( mc_pdgId[j] == -11 && mc_status[j] == 1)
            }// for (Int_t k = 0 ; k < mc_n ; k++)
          } // if (mc_pdgId[j] == 1000015 )
          else if (mc_pdgId[j] == -1000015 ){
            Int_t foundEle = 0;
            for (unsigned int k = 0 ; k < mc_n ; k++) {
              if ( mc_pdgId[k] == 15 && mc_status[k] == 2) {
                foundEle++;
                if (foundEle == 1)
                  mass3 = 77 ;
		//sqrt((mc_E[j]+mc_E[k])*(mc_E[j]+mc_E[k])
		//                               -  (mc_px[j]+mc_px[k])*(mc_px[j]+mc_px[k])
		//                               -  (mc_py[j]+mc_py[k])*(mc_py[j]+mc_py[k])
		//                               -  (mc_pz[j]+mc_pz[k])*(mc_pz[j]+mc_pz[k]) )/1000.0;
              }// if ( mc_pdgId[j] == -11 && mc_status[j] == 1)
            }// for (Int_t k = 0 ; k < mc_n ; k++)
          } // else if (mc_pdgId[j] == -1000015 )
        } // if ( fabs(mc_pdgId[j]) == 1000015 )
        
	//        if (debug >4) cout << "Neutraliinos masses  " << endl;
        
        // HEPG  Neutralinos Masses
        if (mc_pdgId[j] == 1000023 && mc_status[j] == 3) {
          //std::cout << " Found chi02 with istat code " << std::setw(15) << mc_status[j]  << std::endl;
          lshChi02Mass->Fill(mc_m[j]/1000.0);
        } // if (mc_pdgId[j] == 1000023 )
        if (mc_pdgId[j] == 1000022 && mc_status[j] == 3) { // not present
          //std::cout << " Found chi01 with istat code " << std::setw(15) << mc_status[j]  << std::endl;
          lshChi01Mass->Fill(mc_m[j]/1000.0);
        } // if (mc_pdgId[j] == 1000023 )
        if (mc_pdgId[j] == 1000025 &&  mc_status[j] == 3 ) { // not present
          //std::cout << " Found chi03 with istat code " << std::setw(15) << mc_status[j]  << std::endl;
          lshChi03Mass->Fill(mc_m[j]/1000.0);
        } // if (mc_pdgId[j] == 1000023 )
        
        //END OF CURRENT EDIT
      }//if(fabs(mc_barcode[j]) < 200000)
    } // for (unsigned int j = 0 ; j < mc_n ; j++)
    
    
    // Leptons Multiplicities
    lshMinus->Fill(nMinus);
    lshEMinus->Fill(nEMinus);
    lshMuMinus->Fill(nMuMinus);
    //    lshTauMinus->Fill(nTauMinus);
    
    lshPlus->Fill(nPlus);
    lshEPlus->Fill(nEPlus);
    lshMuPlus->Fill(nMuPlus);
    //    lshTauPlus->Fill(nTauPlus);
    lshLepton->Fill(nLeptons);
    
    // Reco Masses of Neutralinos 
    lshMassChi0e->Fill(mass1);
    lshMassChi0m->Fill(mass2);
    lshMassChi0t->Fill(mass3);  
    
    
    // Higgs and Z Mass reconstructed from daughter b's
    if (debug >4) cout << "Z and Higgs masses  " << endl;
    
    Float_t MassHiggs = -999;
    Float_t MassZ = -999;
    //if (nSleptons ==2 ){ gli staus sono stati trasformati in muons...
    //    Float_t ptb    = sqrt(bPX*bPX       +bPY*bPY)/1000.;
    //    Float_t ptbbar = sqrt(bbarPX*bbarPX +bbarPY*bbarPY)/1000.0;
    if (ptb > 25 && ptbbar > 25){ 
      MassHiggs = 110.;
      MassZ = 80.;
      //      MassHiggs = sqrt((bE+bbarE)*(bE+bbarE)
      //                       -  (bPX+bbarPX)*(bPX+bbarPX)
      //                       -  (bPY+bbarPY)*(bPY+bbarPY)
      //                       -  (bPZ+bbarPZ)*(bPZ+bbarPZ) )/1000.0;
      //      MassZ = sqrt((bzE+bbarzE)*(bzE+bbarzE)
      //                   -  (bzPX+bbarzPX)*(bzPX+bbarzPX)
      //                   -  (bzPY+bbarzPY)*(bzPY+bbarzPY)
      //                   -  (bzPZ+bbarzPZ)*(bzPZ+bbarzPZ) )/1000.0;
    } // if (ptb > 25 && ptbbar > 25)
    //}// if (nSleptons <=2 && nSleptons >0) 
    
    if (MassHiggs > 10)
      lshMassHiggs->Fill(MassHiggs);
    if (MassZ > 10)
      lshMassZ->Fill(MassZ);
    
    // Jet Matching to b's and Higgs mass reconstruction
    // more compact copy
    Float_t NewMass34 = -999;
    Float_t * jetPbdR;
    Float_t * jetPbbardR;
    jetPbdR= new Float_t[n_jet_f];
    jetPbbardR= new Float_t[n_jet_f];
    if (debug >4) cout << "Bbar Jets after   " << higgsPerEvent << endl;
    
    if (higgsPerEvent == 1 ){ // first Higgs only case
      if (debug >4) cout << "Calculating dR to the bquark" << endl;
      
      for (unsigned int s = 0 ; s < n_jet_f ; s++){
        // deltaR calculate the dR to the b-jets 
        // This cannot be done for real data, it can be done for MC as we have the parton information
        // Now DeltaR of jets with b's
        jetPbdR[s] =  DeltaR(jet_f.at(s).Eta(),jet_f.at(s).Phi(),mom1_eta,mom1_phi);
        // Now DeltaR of jets with bbar's
        jetPbbardR[s] =  DeltaR(jet_f.at(s).Eta(),jet_f.at(s).Phi(),mom2_eta,mom2_phi);
      } // for (Int_t s = 0 ; s < n_jet_f ; s++)
      
      // Pick the jet with the lowest DeltaR to the b
      unsigned int minbdR = jetPbdR[0]; // deltaR value of the first jet
      unsigned int minbbardR = jetPbbardR[0]; // deltaR value of the first jet
      if (debug >4) cout << "Selecting bs" << endl;
      for (unsigned int  s = 0; s < n_jet_f; s++) {
        if (jetPbdR[s] < minbdR) {
          minbdR = jetPbdR[s];
          bJetIndex=s;
        } // if (jetdR[s] < minbdR)
        
        if (jetPbbardR[s] < minbbardR) {
          minbbardR = jetPbbardR[s];
          bbarJetIndex=s;
        } // if (jetdR[s] < mindR)
        
      }// (Int_t s = 0; s < n_jet_f; s++)
      // what if the both picked the same jet to be the closest...
      if (debug >4) cout << "Checkinf if bs index is the same  bjet: " << bJetIndex << "  bbar: " << bbarJetIndex << endl;
      if (bJetIndex == bbarJetIndex) {
        
        if (debug >4) cout << " SAME JET PICKED FOR b and bbar !!!!!!!!!" << std::endl;
        if (minbdR <= minbbardR ) { // if deltaRb smallest
          if (debug >4) cout << "Reasigning bjet" << endl;
          // redo the procedure for bbar but removing the index for the bjet
          minbbardR = jetPbbardR[0];
          Int_t bbarJetIndex=0;
          for (unsigned int s = 0; s < n_jet_f; s++) {
            if (jetPbbardR[s] < minbbardR && s != bJetIndex) {
              minbbardR = jetPbbardR[s];
              bbarJetIndex=s;
            } // if (jetdR[s] < mindR)
          }// (Int_t s = 0; s < n_jet_f; s++)
          
        }//if (minbdR <= minbbardR )
        else{
          // redo the procedure for b but removing the index for the bbar jet
          if (debug >4) cout << "Reasigning bbarjet" << endl;
          minbdR = jetPbdR[0];
          Int_t bJetIndex=0;
          for (unsigned int s = 0; s < n_jet_f; s++) {
            if (jetPbdR[s] < minbdR && s != bbarJetIndex ) {
              minbdR = jetPbdR[s];
              bJetIndex=s;
            } // if (jetdR[s] < mindR)
          }// (Int_t s = 0; s < n_jet_f; s++)
          
        } // else
        
      }// if (bJetIndex == bbarJetIndex)
      
      if (debug >4) cout << "Assing bs pt values" << endl;
      // PT od the two matched jets
      Float_t ptb    = jet_f.at(bJetIndex).Pt()/1000.;
      Float_t ptbbar = jet_f.at(bbarJetIndex).Pt()/1000.;
      
      if (debug >4) cout << "Fill the bbar histos" << endl;
      lshJet1Match->Fill(ptb);
      lshJet2Match->Fill(ptbbar);
      
      if (!REALDATA){
	//        lshJetFlavorMatch1->Fill(jet_flavor_truth_trueflav[bJetIndex]);
	//        lshJetFlavorMatch2->Fill(jet_flavor_truth_trueflav[bbarJetIndex]);
      }
      //      lshJetWeightMatch1->Fill(jet_flavorweight_f[bJetIndex]);
      //      lshJetWeightMatch2->Fill(jet_flavorweight_f[bbarJetIndex]);
      
      if (debug >4) cout << "Filling mass34" << endl;
      if (ptb > 25.0 && ptbbar > 25.0){ 
        if (debug >4) cout << "Inside mass34 if" << endl;
        //if (ptb > 25 && ptbbar > 25 && HiggsFromStau2 == 0 ){ 
        NewMass34 = (jet_f.at(bJetIndex).M()+jet_f.at(bbarJetIndex).M())/1000.0;
        if (debug >4) cout << "new mass is " << NewMass34<< endl;
        lshJetsNewMass34->Fill(NewMass34);
      }  // if (ptb > 25.0 && ptbbar > 25.0)
      
      if (debug >4) cout << "Filling Jet Histos" << endl;
      
      if (!REALDATA){
	//        if (n_jet_f >0 )lshJetFlavor1->Fill(jet_flavor_truth_trueflav[0]);
	//        if (n_jet_f >1 )lshJetFlavor2->Fill(jet_flavor_truth_trueflav[1]);
	//        if (n_jet_f >2 )lshJetFlavor3->Fill(jet_flavor_truth_trueflav[2]);
	//        if (n_jet_f >3 )lshJetFlavor4->Fill(jet_flavor_truth_trueflav[3]);
	//        if (n_jet_f >4 )lshJetFlavor5->Fill(jet_flavor_truth_trueflav[4]);
	//        if (n_jet_f >5 )lshJetFlavor6->Fill(jet_flavor_truth_trueflav[5]);
      }
      //      if (n_jet_f >0 )lshJetWeight1->Fill(jet_flavorweight_f[0]);
      //      if (n_jet_f >1 )lshJetWeight2->Fill(jet_flavorweight_f[1]);
      //      if (n_jet_f >2 )lshJetWeight3->Fill(jet_flavorweight_f[2]);
      //      if (n_jet_f >3 )lshJetWeight4->Fill(jet_flavorweight_f[3]);
      //      if (n_jet_f >4 )lshJetWeight5->Fill(jet_flavorweight_f[4]);
      //      if (n_jet_f >5 )lshJetWeight6->Fill(jet_flavorweight_f[5]);
      
    } //if (higgsPerEvent == 1 )
    
    
    //if (higgsPerEvent > 0 ) return;    
    //Truth types for mc only

    
    for ( unsigned int i = 0; i < n_el_f; i++){
      if (el_truth_type[i]>-50 && el_truth_type[i]<50)
        hElectronTruthType->Fill(el_truth_type[i]);
      else if (el_truth_type[i]<-1000000 )
        hElectronBSTruthType->Fill(el_truth_type[i]+1000000);
      else if (el_truth_type[i]>1000000 )
        hElectronBSTruthType->Fill(el_truth_type[i]-1000000);
    } 
    
    for (unsigned int i = 0; i < n_mu_f; i++){
      if (mu_truth_type[i]>-50 && mu_truth_type[i]<50)
        hMuonTruthType->Fill(mu_truth_type[i]);
      else if (mu_truth_type[i]<-1000000 )
        hMuonBSTruthType->Fill(mu_truth_type[i]+1000000);
      else if (mu_truth_type[i]>1000000 )
        hMuonBSTruthType->Fill(mu_truth_type[i]-1000000);      
    }  
    
    for (unsigned int i = 0; i < n_mulowbeta_f; i++){
      if (mulowbeta_truth_type[i]>-50 && mulowbeta_truth_type[i]<50)
        hMuLowBetaMuonTruthType->Fill(mulowbeta_truth_type[i]);
      else if (mulowbeta_truth_type[i]<-1000000 )
        hMuLowBetaMuonBSTruthType->Fill(mulowbeta_truth_type[i]+1000000);
      else if (mulowbeta_truth_type[i]>1000000 )
        hMuLowBetaMuonBSTruthType->Fill(mulowbeta_truth_type[i]-1000000);      
    }  
    
    for (unsigned int i = 0; i < n_mugirl_f; i++){
      if (mugirl_truth_type[i]>-50 && mugirl_truth_type[i]<50)
        hMuGirlMuonTruthType->Fill(mugirl_truth_type[i]);
      else if (mugirl_truth_type[i]<-1000000 )
        hMuGirlMuonBSTruthType->Fill(mugirl_truth_type[i]+1000000);
      else if (mugirl_truth_type[i]>1000000 )
        hMuGirlMuonBSTruthType->Fill(mugirl_truth_type[i]-1000000);      
    }  
    
    //Separate signal from background if required 
    // Signal NO higgs per event
    // BKG 1 or more higgs per event
    //    if (higgsPerEvent < NHIGGSMIN) return;
    //    if (higgsPerEvent > NHIGGSMAX) return;
    
    m_cutflow[9]+= (float)mc_event_weight; //MT cut
    hCutFlow->Fill(9);

    
  } //if (!REALDATA)
  hNHiggs->Fill(higgsPerEvent);
  

  hNJets->Fill(n_jet_b);
  hNOkJets->Fill(n_jet_f);
  

  
  if ( n_jet_f > 0) hJet1pT->Fill(jet_f.at(0).Pt()/1000.0);
  if ( n_jet_f > 1) hJet2pT->Fill(jet_f.at(1).Pt()/1000.0);
  if ( n_jet_f > 2) hJet3pT->Fill(jet_f.at(2).Pt()/1000.0);
  if ( n_jet_f > 3) hJet4pT->Fill(jet_f.at(3).Pt()/1000.0);
  
  if ( n_jet_f > 0) hJet1Eta->Fill(jet_f.at(0).Eta());
  if ( n_jet_f > 1) hJet2Eta->Fill(jet_f.at(1).Eta());
  if ( n_jet_f > 2) hJet3Eta->Fill(jet_f.at(2).Eta());
  if ( n_jet_f > 3) hJet4Eta->Fill(jet_f.at(3).Eta());
  
  if ( n_jet_f > 0) hJet1Phi->Fill(jet_f.at(0).Phi());
  if ( n_jet_f > 1) hJet2Phi->Fill(jet_f.at(1).Phi());
  if ( n_jet_f > 2) hJet3Phi->Fill(jet_f.at(2).Phi());
  if ( n_jet_f > 3) hJet4Phi->Fill(jet_f.at(3).Phi());
  
  if ( n_jet_f > 0) hJet1Mass->Fill(jet_f.at(0).M());
  if ( n_jet_f > 1) hJet2Mass->Fill(jet_f.at(1).M());
  if ( n_jet_f > 2) hJet3Mass->Fill(jet_f.at(2).M());
  if ( n_jet_f > 3) hJet4Mass->Fill(jet_f.at(3).M());

  //  if ( n_jet_f > 0) cout << "Jet1 mass: " <<jet_f.at(0).M() << endl;
  //  if ( n_jet_f > 1) cout << "Jet2 mass: " <<jet_f.at(1).M() << endl;
  //  if ( n_jet_f > 2) cout << "Jet3 mass: " <<jet_f.at(2).M() << endl;
  //  if ( n_jet_f > 3) cout << "Jet4 mass: " <<jet_f.at(3).M() << endl;
  
  //  if ( n_jet_f > 0) hJet1FlavorWeight->Fill(jet_flavorweight_f.at(0));
  //  if ( n_jet_f > 1) hJet2FlavorWeight->Fill(jet_flavorweight_f.at(1));
  //  if ( n_jet_f > 2) hJet3FlavorWeight->Fill(jet_flavorweight_f.at(2));
  //  if ( n_jet_f > 3) hJet4FlavorWeight->Fill(jet_flavorweight_f.at(3));
  
  if (!REALDATA){
    //    if ( n_jet_f > 0) hJet1dRTruthB->Fill(jet_TruthdRminB_f.at(0));
    //    if ( n_jet_f > 1) hJet2dRTruthB->Fill(jet_TruthdRminB_f.at(1));
    //    if ( n_jet_f > 2) hJet3dRTruthB->Fill(jet_TruthdRminB_f.at(2));
    //    if ( n_jet_f > 3) hJet4dRTruthB->Fill(jet_TruthdRminB_f.at(3));  
  }
  if ( n_jet_f > 1) hdRJet1Jet2->Fill(DeltaR(jet_f.at(0).Eta(),jet_f.at(0).Phi(),jet_f.at(1).Eta(),jet_f.at(1).Phi()));
  if ( n_jet_f > 2) hdRJet1Jet3->Fill(DeltaR(jet_f.at(0).Eta(),jet_f.at(0).Phi(),jet_f.at(2).Eta(),jet_f.at(2).Phi()));
  if ( n_jet_f > 3) hdRJet1Jet4->Fill(DeltaR(jet_f.at(0).Eta(),jet_f.at(0).Phi(),jet_f.at(3).Eta(),jet_f.at(3).Phi()));
  if ( n_jet_f > 2) hdRJet2Jet3->Fill(DeltaR(jet_f.at(1).Eta(),jet_f.at(1).Phi(),jet_f.at(2).Eta(),jet_f.at(2).Phi()));
  if ( n_jet_f > 3) hdRJet2Jet4->Fill(DeltaR(jet_f.at(1).Eta(),jet_f.at(1).Phi(),jet_f.at(3).Eta(),jet_f.at(3).Phi()));
  if ( n_jet_f > 3) hdRJet3Jet4->Fill(DeltaR(jet_f.at(2).Eta(),jet_f.at(2).Phi(),jet_f.at(3).Eta(),jet_f.at(3).Phi()));
  
  // Filling b bar invariant mass
  //  if (!REALDATA) {
  if (0) {
    double BBInvMass = (jet_f.at(bbarJetIndex).M()+jet_f.at(bJetIndex).M())/1000.0;
    lshRecoBBInvMass->Fill(BBInvMass);
    if (debug >4) cout << "BBInvMass mass " << BBInvMass << endl;
  }
  if (n_jet_f>3) {
    double JJInvMass = (jet_f.at(2).M()+jet_f.at(3).M())/1000.0;
    lshRecoJJInvMass->Fill(JJInvMass);
    if (debug >4) cout << "RecoJJ mass " << JJInvMass << endl;
    
  }
  if (n_el_f > 1 ) {
    double EleInvMass=(el_f.at(0).M()+el_f.at(1).M())/1000.0;
    lshRecoEleInvMass->Fill(el_f.at(0).M()+el_f.at(1).M());
    if (debug >4) cout << "EleInvMass mass " << EleInvMass << endl;
  }
  
  hRecoMuons->Fill(n_mu_b);
  hRecoOkMuons->Fill(n_mu_f);
  hRecoMuLowBetaMuons->Fill(n_mulowbeta_b);
  hRecoOkMuLowBetaMuons->Fill(n_mulowbeta_f);
  hRecoMuGirlMuons->Fill(n_mugirl_b);
  hRecoOkMuGirlMuons->Fill(n_mugirl_f);
  hRecoElectrons->Fill(n_el_b);
  hRecoOkElectrons->Fill(n_el_f);
  //  hRecoTaus->Fill(n_tau_b);
  //  hRecoOkTaus->Fill(n_tau_f);
  hRecoOkLeptons->Fill(n_lep_f);
  
  if (n_el_f > 0 ) hElectron1pT->Fill(el_f.at(0).Pt()/1000.);
  if (n_el_f > 1 ) hElectron2pT->Fill(el_f.at(1).Pt()/1000.);
  if (n_el_f > 2 ) hElectron3pT->Fill(el_f.at(2).Pt()/1000.);
  
  if (n_el_f > 0 ) hElectron1Eta->Fill(el_f.at(0).Eta());
  if (n_el_f > 1 ) hElectron2Eta->Fill(el_f.at(1).Eta());
  if (n_el_f > 2 ) hElectron3Eta->Fill(el_f.at(2).Eta());
  
  if (n_el_f > 0 ) hElectron1Phi->Fill(el_f.at(0).Phi());
  if (n_el_f > 1 ) hElectron2Phi->Fill(el_f.at(1).Phi());
  if (n_el_f > 2 ) hElectron3Phi->Fill(el_f.at(2).Phi());
    
  if (n_el_f > 0 ) hElectron1Mass->Fill(el_f.at(0).M());
  if (n_el_f > 1 ) hElectron2Mass->Fill(el_f.at(1).M());
  if (n_el_f > 2 ) hElectron3Mass->Fill(el_f.at(2).M());
  
  if (n_el_f > 1 ) hdRElectron1Electron2->Fill(DeltaR(el_f.at(0).Eta(),el_f.at(0).Phi(),el_f.at(1).Eta(),el_f.at(1).Phi()));
  if (n_el_f > 2 ) hdRElectron1Electron3->Fill(DeltaR(el_f.at(0).Eta(),el_f.at(0).Phi(),el_f.at(2).Eta(),el_f.at(2).Phi()));
  if (n_el_f > 2 ) hdRElectron2Electron3->Fill(DeltaR(el_f.at(1).Eta(),el_f.at(1).Phi(),el_f.at(2).Eta(),el_f.at(2).Phi()));
  
  if (debug >4) cout << "Starting to fill dR e mu " << endl;
  if (n_el_f > 0 && n_mu_f > 0 ) hdRElectron1Muon1->Fill(DeltaR(el_f.at(0).Eta(),el_f.at(0).Phi(),mu_f.at(0).Eta(),mu_f.at(0).Phi()));
  if (n_el_f > 0 && n_mu_f > 1 ) hdRElectron1Muon2->Fill(DeltaR(el_f.at(0).Eta(),el_f.at(0).Phi(),mu_f.at(1).Eta(),mu_f.at(1).Phi()));
  if (n_el_f > 1 && n_mu_f > 0 ) hdRElectron2Muon1->Fill(DeltaR(el_f.at(1).Eta(),el_f.at(1).Phi(),mu_f.at(0).Eta(),mu_f.at(0).Phi()));
  if (n_el_f > 1 && n_mu_f > 1 ) hdRElectron2Muon2->Fill(DeltaR(el_f.at(1).Eta(),el_f.at(1).Phi(),mu_f.at(1).Eta(),mu_f.at(1).Phi()));
  
  //  if (debug >4) cout << "Starting to fill dR e tau " << endl;
  //  if (n_el_f > 0 && n_tau_f > 0 ) hdRElectron1Tau1->Fill(DeltaR(el_f.at(0).Eta(),el_f.at(0).Phi(),tau_f.at(0).Eta(),tau_f.at(0).Phi()));
  //  if (n_el_f > 0 && n_tau_f > 1 ) hdRElectron1Tau2->Fill(DeltaR(el_f.at(0).Eta(),el_f.at(0).Phi(),tau_f.at(1).Eta(),tau_f.at(1).Phi()));
  //  if (n_el_f > 1 && n_tau_f > 0 ) hdRElectron2Tau1->Fill(DeltaR(el_f.at(1).Eta(),el_f.at(1).Phi(),tau_f.at(0).Eta(),tau_f.at(0).Phi()));
  //  if (n_el_f > 1 && n_tau_f > 1 ) hdRElectron2Tau2->Fill(DeltaR(el_f.at(1).Eta(),el_f.at(1).Phi(),tau_f.at(1).Eta(),tau_f.at(1).Phi()));
  
  if (n_mu_f > 0 ) hMuon1pT->Fill(mu_f.at(0).Pt()/1000.);
  if (n_mu_f > 1 ) hMuon2pT->Fill(mu_f.at(1).Pt()/1000.);
  if (n_mu_f > 2 ) hMuon3pT->Fill(mu_f.at(2).Pt()/1000.);
  
  if (n_mu_f > 0 ) hMuon1Eta->Fill(mu_f.at(0).Eta());
  if (n_mu_f > 1 ) hMuon2Eta->Fill(mu_f.at(1).Eta());
  if (n_mu_f > 2 ) hMuon3Eta->Fill(mu_f.at(2).Eta());
  
  if (n_mu_f > 0 ) hMuon1Phi->Fill(mu_f.at(0).Phi());
  if (n_mu_f > 1 ) hMuon2Phi->Fill(mu_f.at(1).Phi());
  if (n_mu_f > 2 ) hMuon3Phi->Fill(mu_f.at(2).Phi());  

  if (n_mu_f > 0 ) hMuon1Mass->Fill(mu_f.at(0).M());
  if (n_mu_f > 1 ) hMuon2Mass->Fill(mu_f.at(1).M());
  if (n_mu_f > 2 ) hMuon3Mass->Fill(mu_f.at(2).M());
  if (debug >4) if (n_mu_f > 0 ) cout << "            filling mass muon 0: " << mu_f.at(0).M() << endl;
  if (debug >4) if (n_mu_f > 1 ) cout << "            filling mass muon 1: " << mu_f.at(1).M() << endl;
	
  
  if (n_mu_f > 0 ) hMuon1Beta->Fill(mu_beta_f.at(0));
  if (n_mu_f > 1 ) hMuon2Beta->Fill(mu_beta_f.at(1));
  if (n_mu_f > 2 ) hMuon3Beta->Fill(mu_beta_f.at(2));

  if (debug >4) cout << "Starting to fill dR mus " << endl;
  if (n_mu_f > 1 ) hdRMuon1Muon2->Fill(DeltaR(mu_f.at(0).Eta(),mu_f.at(0).Phi(),mu_f.at(1).Eta(),mu_f.at(1).Phi()));
  if (n_mu_f > 2 ) hdRMuon1Muon3->Fill(DeltaR(mu_f.at(0).Eta(),mu_f.at(0).Phi(),mu_f.at(2).Eta(),mu_f.at(2).Phi()));
  if (n_mu_f > 2 ) hdRMuon2Muon3->Fill(DeltaR(mu_f.at(1).Eta(),mu_f.at(1).Phi(),mu_f.at(2).Eta(),mu_f.at(2).Phi()));
  
  //   if (debug >4) cout << "Starting to fill mu  tau " << endl;
  //   if (n_mu_f > 0 && n_tau_f > 0 ) hdRMuon1Tau1->Fill(DeltaR(mu_f.at(0).Eta(),mu_f.at(0).Phi(),tau_f.at(0).Eta(),tau_f.at(0).Phi()));
  //   if (n_mu_f > 0 && n_tau_f > 1 ) hdRMuon1Tau2->Fill(DeltaR(mu_f.at(0).Eta(),mu_f.at(0).Phi(),tau_f.at(1).Eta(),tau_f.at(1).Phi()));
  //   if (n_mu_f > 1 && n_tau_f > 0 ) hdRMuon2Tau1->Fill(DeltaR(mu_f.at(1).Eta(),mu_f.at(1).Phi(),tau_f.at(0).Eta(),tau_f.at(0).Phi()));
  //   if (n_mu_f > 1 && n_tau_f > 1 ) hdRMuon2Tau2->Fill(DeltaR(mu_f.at(1).Eta(),mu_f.at(1).Phi(),tau_f.at(1).Eta(),tau_f.at(1).Phi()));
  
  if (debug >4) cout << "Starting to fill mulowbeta " << endl;
  if (n_mulowbeta_f > 0 ) hMuLowBetaMuon1pT->Fill(mulowbeta_f.at(0).Pt()/1000.);
  if (n_mulowbeta_f > 1 ) hMuLowBetaMuon2pT->Fill(mulowbeta_f.at(1).Pt()/1000.);
  if (n_mulowbeta_f > 2 ) hMuLowBetaMuon3pT->Fill(mulowbeta_f.at(2).Pt()/1000.);
  
  if (n_mulowbeta_f > 0 ) hMuLowBetaMuon1Eta->Fill(mulowbeta_f.at(0).Eta());
  if (n_mulowbeta_f > 1 ) hMuLowBetaMuon2Eta->Fill(mulowbeta_f.at(1).Eta());
  if (n_mulowbeta_f > 2 ) hMuLowBetaMuon3Eta->Fill(mulowbeta_f.at(2).Eta());
  
  if (n_mulowbeta_f > 0 ) hMuLowBetaMuon1Phi->Fill(mulowbeta_f.at(0).Phi());
  if (n_mulowbeta_f > 1 ) hMuLowBetaMuon2Phi->Fill(mulowbeta_f.at(1).Phi());
  if (n_mulowbeta_f > 2 ) hMuLowBetaMuon3Phi->Fill(mulowbeta_f.at(2).Phi());
  
  if (n_mulowbeta_f > 0 ) hMuLowBetaMuon1Mass->Fill(mulowbeta_f.at(0).M());
  if (n_mulowbeta_f > 1 ) hMuLowBetaMuon2Mass->Fill(mulowbeta_f.at(1).M());
  if (n_mulowbeta_f > 2 ) hMuLowBetaMuon3Mass->Fill(mulowbeta_f.at(2).M());
  if (debug >4) if (n_mulowbeta_f > 0 ) cout << "            filling LOW BETA mass muon 0: " << mulowbeta_f.at(0).M() << endl;
  if (debug >4) if (n_mulowbeta_f > 1 ) cout << "            filling LOW BETA mass muon 1: " << mulowbeta_f.at(1).M() << endl;

  if (debug >4) cout << "Starting to fill mugirl Pt" << endl;
  if (n_mugirl_f > 0 ) hMuGirlMuon1pT->Fill(mugirl_f.at(0).Pt()/1000.);
  if (n_mugirl_f > 1 ) hMuGirlMuon2pT->Fill(mugirl_f.at(1).Pt()/1000.);
  if (n_mugirl_f > 2 ) hMuGirlMuon3pT->Fill(mugirl_f.at(2).Pt()/1000.);
  
  if (debug >4) cout << "Starting to fill mugirl Eta" << endl;
  //  if (n_mugirl_f > 0 ) hMuGirlMuon1Eta->Fill(mugirl_f.at(0).Eta());
  //  if (n_mugirl_f > 1 ) hMuGirlMuon2Eta->Fill(mugirl_f.at(1).Eta());
  //  if (n_mugirl_f > 2 ) hMuGirlMuon3Eta->Fill(mugirl_f.at(2).Eta());
  
  if (debug >4) cout << "Starting to fill mugirl Phi" << endl;
  if (n_mugirl_f > 0 ) hMuGirlMuon1Phi->Fill(mugirl_f.at(0).Phi());
  if (n_mugirl_f > 1 ) hMuGirlMuon2Phi->Fill(mugirl_f.at(1).Phi());
  if (n_mugirl_f > 2 ) hMuGirlMuon3Phi->Fill(mugirl_f.at(2).Phi());
  
  if (debug >4) cout << "Starting to fill mugirl Mass" << endl;
  if (n_mugirl_f > 0 ) hMuGirlMuon1Mass->Fill(mugirl_f.at(0).M());
  if (n_mugirl_f > 1 ) hMuGirlMuon2Mass->Fill(mugirl_f.at(1).M());
  if (n_mugirl_f > 2 ) hMuGirlMuon3Mass->Fill(mugirl_f.at(2).M());

  //calculate the mass of  the slow muon based on the beta 
  std::vector<float> staumass;
  std::vector<float> staueta = m_sel->GetFinVectorByType< std::vector<float> >("MULOWBETA", "_eta");
  std::vector<float> stauphi = m_sel->GetFinVectorByType< std::vector<float> >("MULOWBETA", "_phi");
  std::vector<float> staupt = m_sel->GetFinVectorByType< std::vector<float> >("MULOWBETA", "_pt");

  std::vector<TVector3> slepfake;
  std::vector<TLorentzVector> stau_f; 
  std::vector<float> stau_charge = m_sel->GetFinVectorByType< std::vector<float> >("MULOWBETA", "_charge");
  TLorentzVector tempStau;
  TVector3 tempSlepton;    
  for ( unsigned int i = 0; i < n_mulowbeta_f; i++){
    //cout << "bg i= " << i  << "   " << n_mulowbeta_f<< endl;
    if (debug >4)  cout << "THe pt values are Px: " << mulowbeta_f.at(i).Px() << "   Py: " << mulowbeta_f.at(i).Py() << "  Pz: " << mulowbeta_f.at(i).Pz() << endl;
    tempSlepton.SetXYZ(mulowbeta_f.at(i).Px()/1000.0,mulowbeta_f.at(i).Py()/1000.0,mulowbeta_f.at(i).Pz()/1000.0);
    slepfake.push_back(tempSlepton);
    staumass.push_back(sqrt(1-mulowbeta_beta_f.at(i)*mulowbeta_beta_f.at(i))*slepfake.at(i).Mag()/mulowbeta_beta_f.at(i));
    if (debug >4) cout << "Mass of the current stau: " << staumass.at(i) <<  "   Beta: " << mulowbeta_beta_f.at(i) << "  Pmag:" << slepfake.at(i).Mag() <<endl;
    tempStau.SetPtEtaPhiM(staupt.at(i),staueta.at(i),stauphi.at(i),staumass.at(i));
    stau_f.push_back(tempStau);
    if (debug >4) cout << "Mass of the current stau: " << staumass.at(i) << "  v2: "<< stau_f.at(i).M()<<  "   Beta: " << mulowbeta_beta_f.at(i) << "  Pmag:" << slepfake.at(i).Mag() <<endl;
    //cout << "here i= " << i  << "   " << n_mulowbeta_f<< endl;
    
  }
  //     cout << "here 2:" << endl;
  // find the closest dr to the slowbetamuon
  std::vector<float> dRDilep,dilepmass;
  float dRmin=100.0;
  int dRminIndex=0;
  float dRminJET=100.0;
  int dRminIndexJET=0;
  std::vector<TLorentzVector> neutralino_f;
  if(n_mulowbeta_f > 0 ){
    //     if (debug >0) cout << "n_mulowbeta_f:    " << n_mulowbeta_f<< endl;
    //cout << endl;
    for ( unsigned int i = 0; i < n_lep_f; i++){
      dRDilep.push_back(DeltaR(lep_f.at(i).Eta(),lep_f.at(i).Phi(),stau_f.at(0).Eta(),stau_f.at(0).Phi()));
      dilepmass.push_back(lep_f.at(i).M()+stau_f.at(0).M());
      if (debug >4) cout << "dR:    " << dRDilep.at(i) <<  " i: "<< i << "  Charges stau " <<stau_charge.at(0) <<   "  Charges lep " << lep_charge.at(i) << "  My type lep: "<<  lep_mytype.at(i) << "   Lep truth type:"<< lep_truth_type.at(i)<<"   Lep barcode:"<< lep_truth_barcode.at(i)<<endl;
      if (lep_charge.at(i)*stau_charge.at(0) < 0.0 ){
	if (dRDilep.at(i) < dRmin && dRDilep.at(i)>0.0 ){
	  dRmin=dRDilep.at(i);
	  dRminIndex=i;
	  neutralino_f=stau_f.at(0)+lep_f.at(i);
	}
      }//if (lep_charge.at(i)*stau_charge.at(0) < 0.0 ){
    }//for ( unsigned int i = 0; i < n_lep_f; i++)
		 
    if (debug >4) cout << " min dR " << dRmin << "   dR index " << dRminIndex <<  endl;
    if (dRmin<10.0 ) {
      hdRMinDilep->Fill(dRDilep.at(dRminIndex));
      hdRMinDilepInvMass->Fill(dilepmass.at(dRminIndex));
      h2dInvMassMindRDilep->Fill(dilepmass.at(dRminIndex),dRDilep.at(dRminIndex));
      for (unsigned int kk=0; kk<n_jet_f; kk++) {
	float jetdR=DeltaR(neutralino_f.Eta(),neutralino_f.Phi(),jet_f.at(kk).Eta(),jet_f.at(kk).Phi())
	  if (jetdR<dRminJET) {
	    dRminJET=jetdR;
	    dRminIndexJET=kk;
	  }
      }
      for ( unsigned int i = 0; i < n_lep_f; i++){
	if ( dRDilep.at(i) >0.1 ) {
	  if (  dRminIndex != i ) {
	    //cout << "dr " << dRDilep.at(i) << "   i " << i << "    dRminIndex: "<< dRminIndex << "   dRmin "<< dRmin <<endl;
	    hdRDilep->Fill(dRDilep.at(i));
	    hDilepInvMass->Fill(dilepmass.at(i));
	    h2dInvMassdRDilep->Fill(dilepmass.at(i),dRDilep.at(i));
	  }
	}//if (dRdilep.at(i)>0.1)
      }//for n_lep_f
    }//if (dRmin< 10.0)
  }//if(n_mulowbeta_f > 0 )
  
  /* if(debug >0 ){
  //     if (debug >0) cout << "n_mulowbeta_f:    " << n_mulowbeta_f<< endl;
  //cout << endl;
  for ( unsigned int i = 0; i < n_el_f; i++){
  cout << "Electron [" << i << "]    truth_type: "<< el_truth_type.at(i) << "   barcode: " << el_truth_barcode.at(i)<<  endl; 				
  }
  for ( unsigned int i = 0; i < n_mu_f; i++){
  cout << "    MUon [" << i << "]    truth_type: "<< mu_truth_type.at(i) << "   barcode: " << mu_truth_barcode.at(i)<<  endl; 				
  }
  for ( unsigned int i = 0; i < n_mulowbeta_f; i++){
  cout << "LowBMuon [" << i << "]    truth_type: "<< mulowbeta_truth_type.at(i) << "   barcode: " << mulowbeta_truth_barcode.at(i)<<  endl; 				
  }
  for ( unsigned int i = 0; i < n_mugirl_f; i++){
  cout << "MuGirlMu [" << i << "]    truth_type: "<< mugirl_truth_type.at(i) << "   barcode: " << mugirl_truth_barcode.at(i)<<  endl; 				
  }
  }//if(debug >0 ) */
				
				
  //   //Loop over all leptons and find the opposite sign pair with the smallest dR
  dRmin=100.0;
  std::vector< std::vector<float> >dRLeptons;
  std::vector< std::vector<float> >Mass2Leptons;
  unsigned int dRminIIndex=0;
  unsigned int dRminJIndex=0;
  for ( unsigned int i =0 ; i < n_lep_f ; i++){
    dRLeptons.push_back(std::vector<float>());
    Mass2Leptons.push_back(std::vector<float>());
    for ( unsigned int j =i+1 ; j < n_lep_f ; j++){
      dRLeptons[i].push_back(DeltaR(lep_f.at(i).Eta(),lep_f.at(i).Phi(),lep_f.at(j).Eta(),lep_f.at(j).Phi()));
      Mass2Leptons[i].push_back(lep_f.at(i).M()+lep_f.at(j).M());
			 
      //			 if(lep_charge.at(i)*lep_charge.at(j) < 0.0)
      //				 cout << "Bingo" << endl;
      if ( lep_charge.at(i)*lep_charge.at(j) < 0.0) {
	if ( dRLeptons[i][j-i-1] < dRmin ){
	  dRmin=dRLeptons[i][j-i-1];
	  dRminIIndex=i;
	  dRminJIndex=j-1-i;
	}
      }//if ( lep_charge.at(i)*lep_charge.at(j) < 0.0)
    }
    
  }//for ( unsigned int i =0 ; i < n_lep_f ; i++)

  //	cout <<" [dRminIIndex]  "<< dRminIIndex<<  "  [dRminJIndex]  "<< dRminJIndex << "  dRmin  "<< dRmin <<endl;
  if (dRmin < 10.0) {
    hdRMinLeptons->Fill(dRmin);
    hMassMindRLeptons->Fill(Mass2Leptons[dRminIIndex][dRminJIndex]);
    h2dMassMindRLeptons->Fill(Mass2Leptons[dRminIIndex][dRminJIndex],dRmin);

    for ( unsigned int i =0 ; i < n_lep_f ; i++){
      for ( unsigned int j =i+1 ; j < n_lep_f ; j++){
	if ( ( dRminIIndex != i || dRminJIndex !=(j-1-i) ) && dRLeptons[i][j-1-i] >0.1 ) {
	  //	cout <<" [dRminIIndex]  "<< dRminIIndex<<  "  [dRminJIndex]  "<< dRminJIndex << "  dRmin  "<< dRmin <<endl;
	  hdRLeptons->Fill(dRLeptons[i][j-1-i]);
	  hMassdRLeptons->Fill(Mass2Leptons[i][j-1-i]);
	  h2dMassdRLeptons->Fill(Mass2Leptons[i][j-1-i],dRLeptons[i][j-1-i]);


	}
      }
    }
  }
  //   }//hMuLowBetaMuon1Mass

  if (n_mulowbeta_f > 0 ) hMuLowBetaMuon1MassBeta->Fill(staumass.at(0));
  if (n_mulowbeta_f > 1 ) hMuLowBetaMuon2MassBeta->Fill(staumass.at(1));
  if (n_mulowbeta_f > 2 ) hMuLowBetaMuon3MassBeta->Fill(staumass.at(2));
  
  if (n_mulowbeta_f > 0 ) hMuLowBetaMuon1Beta->Fill(mulowbeta_beta_f.at(0));
  if (n_mulowbeta_f > 1 ) hMuLowBetaMuon2Beta->Fill(mulowbeta_beta_f.at(1));
  if (n_mulowbeta_f > 2 ) hMuLowBetaMuon3Beta->Fill(mulowbeta_beta_f.at(2));

  if (n_mugirl_f > 0 ) hMuGirlMuon1Beta->Fill(mugirl_beta_f.at(0));
  if (n_mugirl_f > 1 ) hMuGirlMuon2Beta->Fill(mugirl_beta_f.at(1));
  if (n_mugirl_f > 2 ) hMuGirlMuon3Beta->Fill(mugirl_beta_f.at(2));

  //   for ( unsigned int i = 0; i < n_mulowbeta_f; i++){
    

  //   }

  //   if (debug >4) std::cout << "THe beta value for the 1st low beta muon is :" << mulowbeta_beta_f.at(0) << endl;

  //   if (n_tau_f > 0 ) hTau1pT->Fill(tau_f.at(0).Pt()/1000.);
  //   if (n_tau_f > 1 ) hTau2pT->Fill(tau_f.at(1).Pt()/1000.);
  //   if (n_tau_f > 2 ) hTau3pT->Fill(tau_f.at(2).Pt()/1000.);
  
  //   if (n_tau_f > 0 ) hTau1Eta->Fill(tau_f.at(0).Eta());
  //   if (n_tau_f > 1 ) hTau2Eta->Fill(tau_f.at(1).Eta());
  //   if (n_tau_f > 2 ) hTau3Eta->Fill(tau_f.at(2).Eta());
  
  //   if (n_tau_f > 0 ) hTau1Phi->Fill(tau_f.at(0).Phi());
  //   if (n_tau_f > 1 ) hTau2Phi->Fill(tau_f.at(1).Phi());
  //   if (n_tau_f > 2 ) hTau3Phi->Fill(tau_f.at(2).Phi());

  //   if (n_tau_f > 0 ) hTau1Mass->Fill(tau_f.at(0).M());
  //   if (n_tau_f > 1 ) hTau2Mass->Fill(tau_f.at(1).M());
  //   if (n_tau_f > 2 ) hTau3Mass->Fill(tau_f.at(2).M());

  //   if (n_tau_f > 1 ) hdRTau1Tau2->Fill(DeltaR(tau_f.at(0).Eta(),tau_f.at(0).Phi(),tau_f.at(1).Eta(),tau_f.at(1).Phi()));
  //   if (n_tau_f > 2 ) hdRTau1Tau3->Fill(DeltaR(tau_f.at(0).Eta(),tau_f.at(0).Phi(),tau_f.at(2).Eta(),tau_f.at(2).Phi()));
  //   if (n_tau_f > 2 ) hdRTau2Tau3->Fill(DeltaR(tau_f.at(1).Eta(),tau_f.at(1).Phi(),tau_f.at(2).Eta(),tau_f.at(2).Phi()));  
  
  // MET histograms
  if (debug >4 ) std::cout << "Value of the met" << met << endl;
  hMET->Fill(met/1000.0);
  hMETPhi->Fill(met_phi);
  


  //TRIGGERS STUDY
  if (!REALDATA){
    if(higgsPerEvent==1)
      nOneHiggsPerEvent++;
    if(higgsPerEvent==2)
      nTwoHiggsPerEvent++;
        
    if(L1_MU10)
      triggerPassed["L1_MU10"]++;
    if(L1_MU10&&higgsPerEvent==1)
      triggerPassed_1Higgs["L1_MU10"]++;
    if(L1_MU10&&higgsPerEvent==2)
      triggerPassed_2Higgs["L1_MU10"]++;
    
    if(L2_mu40_slow)
      triggerPassed["L2_mu40_slow"]++;
    if(L2_mu40_slow&&higgsPerEvent==1)
      triggerPassed_1Higgs["L2_mu40_slow"]++;
    if(L2_mu40_slow&&higgsPerEvent==2)
      triggerPassed_2Higgs["L2_mu40_slow"]++;
    
    
    
    if(EF_mu40_slow)
      triggerPassed["EF_mu40_slow"]++;
    if(EF_mu40_slow&&higgsPerEvent==1)
      triggerPassed_1Higgs["EF_mu40_slow"]++;
    if(EF_mu40_slow&&higgsPerEvent==2)
      triggerPassed_2Higgs["EF_mu40_slow"]++;
    
    if(L1_MU10 && L2_mu40_slow )
      triggerPassed["mu40_slow"]++;
    if(L1_MU10 && L2_mu40_slow && higgsPerEvent==1)
      triggerPassed_1Higgs["mu40_slow"]++;
    if(L1_MU10 && L2_mu40_slow && higgsPerEvent==2)
      triggerPassed_2Higgs["mu40_slow"]++;
    
    
    if (n_mu_f > 0 ) {      
      hn_sel_pt->Fill(mu_f.at(0).Pt()/1000.0);
      hn_sel_eta->Fill(mu_f.at(0).Eta());
      hn_sel_phi->Fill(mu_f.at(0).Phi());
      if(L1_MU10){
	hn_sel_L1_pt->Fill(mu_f.at(0).Pt()/1000.0);
	hn_sel_L1_eta->Fill(mu_f.at(0).Eta());
	hn_sel_L1_phi->Fill(mu_f.at(0).Phi());

        if(L2_mu40_slow){ 
          hn_sel_L2_pt->Fill(mu_f.at(0).Pt()/1000.0);
          hn_sel_L2_eta->Fill(mu_f.at(0).Eta());
          hn_sel_L2_phi->Fill(mu_f.at(0).Phi());
          if(EF_mu40_slow){
            hn_sel_EF_pt->Fill(mu_f.at(0).Pt()/1000.0);
          }  // if(EF_mu40_slow)
        } //if(L2_mu40_slow)
      } // if(L1_MU10)
    } // (n_mu_f > 0 )
    //    

    if (n_mulowbeta_f > 0) {
      
      hn_slow_sel_pt->Fill(mulowbeta_f.at(0).Pt()/1000.0);
      hn_slow_sel_eta->Fill(mulowbeta_f.at(0).Eta());
      hn_slow_sel_phi->Fill(mulowbeta_f.at(0).Phi());
      
      if(L1_MU10){ 
        hn_slow_sel_L1_pt->Fill(mulowbeta_f.at(0).Pt()/1000.0);
        hn_slow_sel_L1_eta->Fill(mulowbeta_f.at(0).Eta());
        hn_slow_sel_L1_phi->Fill(mulowbeta_f.at(0).Phi());
        if(L2_mu40_slow){
          hn_slow_sel_L2_pt->Fill(mulowbeta_f.at(0).Pt()/1000.0);
          hn_slow_sel_L2_eta->Fill(mulowbeta_f.at(0).Eta());
          hn_slow_sel_L2_phi->Fill(mulowbeta_f.at(0).Phi());
          if(EF_mu40_slow){
            hn_slow_sel_EF_pt->Fill(mulowbeta_f.at(0).Pt()/1000.0);
            hn_slow_sel_EF_eta->Fill(mulowbeta_f.at(0).Eta());
            hn_slow_sel_EF_phi->Fill(mulowbeta_f.at(0).Phi());
          } //if(EF_mu40_slow)
        } //if(L2_mu40_slow)
      } //if(L1_MU10)
    }// if (n_mulowbeta_f > 0)    
  }// if (!REALDATA)
  
}// end of the doAnalysis


void Analysis::finalizeEvent(Long64_t entry)
{
  //  std::cout << "entry " << entry <<  "  eventsProcessed" << std::endl;
  //  if (eventsProcessed%500 == 0 || entry ==1 ){

  if ( entry ==1 || eventsProcessed%100 == 0){
    std::cout << "Cutflow after Event " << entry << std::endl;
    for (int cf = 0; cf < 10 ; cf++)
      {
	std::cout << "Cut " << cf << ": " <<  m_cutflow[cf] <<     "      -> "<< m_cutname[cf] << std::endl;
      }

    if (entry == 1 ) {
      bbcounter ++;
      std::cout <<  "     evt proc: " << eventsProcessed << "  #files processed " << bbcounter <<std::endl;
    }
    if (!REALDATA){
      cout << "Summary " << endl;
      cout << "Evt Processed: " << eventsProcessed << "   Passed: " << eventsPassed << "   OneHiggsPerEvent: " << nOneHiggsPerEvent << "   TwoHiggsPerEvent: " << nTwoHiggsPerEvent <<  endl;
    
      cout << "Trigger Print out TRIGGER NAME:mu40_slow" << endl;
      cout << "L1:  L1_MU10   --   L2:  L2_mu40_slow   --   EF: EF_mu40_slow" << endl;
      printf(" ALL EVENTS  -  L1: %5d   L2: %5d   EF: %5d   Comb: %5d \n ", triggerPassed["L1_MU10"],triggerPassed["L2_mu40_slow"],triggerPassed["EF_mu40_slow"],triggerPassed["mu40_slow"]);
      printf("with 1Higgs  -  L1: %5d   L2: %5d   EF: %5d   Comb: %5d \n ", triggerPassed_1Higgs["L1_MU10"],triggerPassed_1Higgs["L2_mu40_slow"],triggerPassed_1Higgs["EF_mu40_slow"],triggerPassed_1Higgs["mu40_slow"]);
      printf("with 2Higgs  -  L1: %5d   L2: %5d   EF: %5d   Comb: %5d \n ", triggerPassed_2Higgs["L1_MU10"],triggerPassed_2Higgs["L2_mu40_slow"],triggerPassed_2Higgs["EF_mu40_slow"],triggerPassed_2Higgs["mu40_slow"]);
    
      cout << "Percentages" << endl;
      printf("ALL  L1: %3.2f   L2: %3.2f   EF: %3.2f     mu40_slow: %3.2f \n",triggerPassed["L1_MU10"]*100.0/eventsPassed,triggerPassed["L2_mu40_slow"]*100.0/eventsPassed,triggerPassed["EF_mu40_slow"]*100.0/eventsPassed,triggerPassed["mu40_slow"]*100.0/eventsPassed);
      printf("1 H  L1: %3.2f   L2: %3.2f   EF: %3.2f     mu40_slow: %3.2f \n",triggerPassed_1Higgs["L1_MU10"]*100.0/eventsPassed,triggerPassed_1Higgs["L2_mu40_slow"]*100.0/eventsPassed,triggerPassed_1Higgs["EF_mu40_slow"]*100.0/eventsPassed,triggerPassed_1Higgs["mu40_slow"]*100.0/eventsPassed);
      printf("2 H  L1: %3.2f   L2: %3.2f   EF: %3.2f     mu40_slow: %3.2f \n",triggerPassed_2Higgs["L1_MU10"]*100.0/eventsPassed,triggerPassed_2Higgs["L2_mu40_slow"]*100.0/eventsPassed,triggerPassed_2Higgs["EF_mu40_slow"]*100.0/eventsPassed,triggerPassed_2Higgs["mu40_slow"]*100.0/eventsPassed);
    
      cout << "Percentages Relative" << endl;
      printf("ALL  L1: %3.2f   L2: %3.2f   EF: %3.2f     mu40_slow: %3.2f \n",triggerPassed["L1_MU10"]*100.0/eventsPassed,triggerPassed["L2_mu40_slow"]*100.0/eventsPassed,triggerPassed["EF_mu40_slow"]*100.0/eventsPassed,triggerPassed["mu40_slow"]*100.0/eventsPassed);
      printf("1 H  L1: %3.2f   L2: %3.2f   EF: %3.2f     mu40_slow: %3.2f \n",triggerPassed_1Higgs["L1_MU10"]*100.0/nOneHiggsPerEvent,triggerPassed_1Higgs["L2_mu40_slow"]*100.0/nOneHiggsPerEvent,triggerPassed_1Higgs["EF_mu40_slow"]*100.0/nOneHiggsPerEvent,triggerPassed_1Higgs["mu40_slow"]*100.0/nOneHiggsPerEvent);
      printf("2 H  L1: %3.2f   L2: %3.2f   EF: %3.2f     mu40_slow: %3.2f \n",triggerPassed_2Higgs["L1_MU10"]*100.0/nTwoHiggsPerEvent,triggerPassed_2Higgs["L2_mu40_slow"]*100.0/nTwoHiggsPerEvent,triggerPassed_2Higgs["EF_mu40_slow"]*100.0/nTwoHiggsPerEvent,triggerPassed_2Higgs["mu40_slow"]*100.0/nTwoHiggsPerEvent);
    }
  }
  return;
  
  
}


/*static*/ void Analysis::postAnalysis (TSelectorList* fOutput)
{
  //This method is declared as static in the header file
  //Why?
  //This comes from the fact that PROOF calls the method Terminate (which then calls this method) on the master thread.
  //However, when running PROOF, the code above usually runs in Slave Threads. 
  //The master thread (which is running when the computer gets here), doesn't have acces to the member variables in the slave threads.
  
  //All you can do here is get Objects from fOutput-List and Draw them or save them to a file
  TFile *fOut = new TFile("llp_hist.root","RECREATE");
  fOut->cd();
  
  //Lets get our meff4 plot from above (which at this stage will be merged)
  TH1D *dada = (TH1D*)fOutput->FindObject("meff4")->Clone();
  
  //We create a Canvas
  new TCanvas("Show","Show", 800, 600);
  
  //And eventually we draw the meff plot.
  //  dada->Draw();
  
  TH1D *hNJets = (TH1D*)fOutput->FindObject("hNJets")->Clone();
  hNJets->Write();
  
  TH1D *hNOkJets = (TH1D*)fOutput->FindObject("hNOkJets")->Clone();
  hNOkJets->Write();
  
  TH1D *hCutFlow = (TH1D*)fOutput->FindObject("hCutFlow")->Clone();
  hCutFlow->Write();
  
  TH1D *hJet1pT = (TH1D*)fOutput->FindObject("hJet1pT")->Clone();
  hJet1pT->Write();
  
  TH1D *hJet2pT = (TH1D*)fOutput->FindObject("hJet2pT")->Clone();
  hJet2pT->Write();
  
  TH1D *hJet3pT = (TH1D*)fOutput->FindObject("hJet3pT")->Clone();
  hJet3pT->Write();
  
  TH1D *hJet4pT = (TH1D*)fOutput->FindObject("hJet4pT")->Clone();
  hJet4pT->Write();
  
  TH1D *hJet1Eta = (TH1D*)fOutput->FindObject("hJet1Eta")->Clone();
  hJet1Eta->Write();
  
  TH1D *hJet2Eta = (TH1D*)fOutput->FindObject("hJet2Eta")->Clone();
  hJet2Eta->Write();
  
  TH1D *hJet3Eta = (TH1D*)fOutput->FindObject("hJet3Eta")->Clone();
  hJet3Eta->Write();
  
  TH1D *hJet4Eta = (TH1D*)fOutput->FindObject("hJet4Eta")->Clone();
  hJet4Eta->Write();
  
  TH1D *hJet1Phi = (TH1D*)fOutput->FindObject("hJet1Phi")->Clone();
  hJet1Phi->Write();
  
  TH1D *hJet2Phi = (TH1D*)fOutput->FindObject("hJet2Phi")->Clone();
  hJet2Phi->Write();
  
  TH1D *hJet3Phi = (TH1D*)fOutput->FindObject("hJet3Phi")->Clone();
  hJet3Phi->Write();  
  
  TH1D *hJet4Phi = (TH1D*)fOutput->FindObject("hJet4Phi")->Clone();
  hJet4Phi->Write();
  
  TH1D *hJet1Mass = (TH1D*)fOutput->FindObject("hJet1Mass")->Clone();
  hJet1Mass->Write();
  
  TH1D *hJet2Mass = (TH1D*)fOutput->FindObject("hJet2Mass")->Clone();
  hJet2Mass->Write();
  
  TH1D *hJet3Mass = (TH1D*)fOutput->FindObject("hJet3Mass")->Clone();
  hJet3Mass->Write();  
  
  TH1D *hJet4Mass = (TH1D*)fOutput->FindObject("hJet4Mass")->Clone();
  hJet4Mass->Write();  
  
  TH1D *hJet1FlavorWeight= (TH1D*)fOutput->FindObject("hJet1FlavorWeight")->Clone();
  hJet1FlavorWeight->Write();
  
  TH1D *hJet2FlavorWeight= (TH1D*)fOutput->FindObject("hJet2FlavorWeight")->Clone();
  hJet2FlavorWeight->Write();
  
  TH1D *hJet3FlavorWeight= (TH1D*)fOutput->FindObject("hJet3FlavorWeight")->Clone();
  hJet3FlavorWeight->Write();
  
  TH1D *hJet4FlavorWeight= (TH1D*)fOutput->FindObject("hJet4FlavorWeight")->Clone();
  hJet4FlavorWeight->Write();
  
  TH1D *hJet1dRTruthB= (TH1D*)fOutput->FindObject("hJet1dRTruthB")->Clone();
  hJet1dRTruthB->Write();
  
  TH1D *hJet2dRTruthB= (TH1D*)fOutput->FindObject("hJet2dRTruthB")->Clone();
  hJet2dRTruthB->Write();
  
  TH1D *hJet3dRTruthB= (TH1D*)fOutput->FindObject("hJet3dRTruthB")->Clone();
  hJet3dRTruthB->Write();
  
  TH1D *hJet4dRTruthB= (TH1D*)fOutput->FindObject("hJet4dRTruthB")->Clone();
  hJet4dRTruthB->Write();
  
  TH1D *hdRJet1Jet2 = (TH1D*)fOutput->FindObject("hdRJet1Jet2")->Clone();
  hdRJet1Jet2->Write();
  
  TH1D *hdRJet1Jet3 = (TH1D*)fOutput->FindObject("hdRJet1Jet3")->Clone();
  hdRJet1Jet3->Write();
  
  TH1D *hdRJet1Jet4 = (TH1D*)fOutput->FindObject("hdRJet1Jet4")->Clone();
  hdRJet1Jet4->Write();
  
  TH1D *hdRJet2Jet3 = (TH1D*)fOutput->FindObject("hdRJet2Jet3")->Clone();
  hdRJet2Jet3->Write();
  
  TH1D *hdRJet2Jet4 = (TH1D*)fOutput->FindObject("hdRJet2Jet4")->Clone();
  hdRJet2Jet4->Write();
  
  TH1D *hdRJet3Jet4 = (TH1D*)fOutput->FindObject("hdRJet3Jet4")->Clone();
  hdRJet3Jet4->Write();
  
  TH1D *hRecoOkElectrons = (TH1D*)fOutput->FindObject("hRecoOkElectrons")->Clone();
  hRecoOkElectrons->Write();
  
  TH1D *hRecoElectrons = (TH1D*)fOutput->FindObject("hRecoElectrons")->Clone();
  hRecoElectrons->Write();
  
  //  cout << "electron pT" << endl;  
  TH1D *hElectron1pT = (TH1D*)fOutput->FindObject("hElectron1pT")->Clone();
  hElectron1pT->Write();
  
  TH1D *hElectron2pT = (TH1D*)fOutput->FindObject("hElectron2pT")->Clone();
  hElectron2pT->Write();
  
  TH1D *hElectron3pT = (TH1D*)fOutput->FindObject("hElectron3pT")->Clone();
  hElectron3pT->Write();
  
  //cout << "electron eta" << endl;  
  TH1D *hElectron1Eta = (TH1D*)fOutput->FindObject("hElectron1Eta")->Clone();
  hElectron1Eta->Write();
  
  TH1D *hElectron2Eta = (TH1D*)fOutput->FindObject("hElectron2Eta")->Clone();
  hElectron2Eta->Write();
  
  TH1D *hElectron3Eta = (TH1D*)fOutput->FindObject("hElectron3Eta")->Clone();
  hElectron3Eta->Write();
  
  //cout << "electron Phi" << endl;  
  TH1D *hElectron1Phi = (TH1D*)fOutput->FindObject("hElectron1Phi")->Clone();
  hElectron1Phi->Write();
  
  TH1D *hElectron2Phi = (TH1D*)fOutput->FindObject("hElectron2Phi")->Clone();
  hElectron2Phi->Write();
  
  TH1D *hElectron3Phi = (TH1D*)fOutput->FindObject("hElectron3Phi")->Clone();
  hElectron3Phi->Write();
  
  TH1D *hElectron1Mass = (TH1D*)fOutput->FindObject("hElectron1Mass")->Clone();
  hElectron1Mass->Write();
  
  TH1D *hElectron2Mass = (TH1D*)fOutput->FindObject("hElectron2Mass")->Clone();
  hElectron2Mass->Write();
  
  TH1D *hElectron3Mass = (TH1D*)fOutput->FindObject("hElectron3Mass")->Clone();
  hElectron3Mass->Write();

  //cout << "electron dR" << endl;  
  TH1D *hdRElectron1Electron2 = (TH1D*)fOutput->FindObject("hdRElectron1Electron2")->Clone();
  hdRElectron1Electron2->Write();
  
  TH1D *hdRElectron1Electron3 = (TH1D*)fOutput->FindObject("hdRElectron1Electron3")->Clone();
  hdRElectron1Electron3->Write();
  
  TH1D *hdRElectron2Electron3 = (TH1D*)fOutput->FindObject("hdRElectron2Electron3")->Clone();
  hdRElectron2Electron3->Write();
  
  TH1D *hdRElectron1Muon1 = (TH1D*)fOutput->FindObject("hdRElectron1Muon1")->Clone();
  hdRElectron1Muon1->Write();
  
  TH1D *hdRElectron1Muon2 = (TH1D*)fOutput->FindObject("hdRElectron1Muon2")->Clone();
  hdRElectron1Muon2->Write();
  
  //  TH1D *hdRElectron1Tau1 = (TH1D*)fOutput->FindObject("hdRElectron1Tau1")->Clone();
  //  hdRElectron1Tau1->Write();
  
  //   TH1D *hdRElectron1Tau2 = (TH1D*)fOutput->FindObject("hdRElectron1Tau2")->Clone();
  //   hdRElectron1Tau2->Write();
  
  //   TH1D *hdRElectron2Muon1 = (TH1D*)fOutput->FindObject("hdRElectron2Muon1")->Clone();
  //   hdRElectron2Muon1->Write();
  
  //   TH1D *hdRElectron2Muon2 = (TH1D*)fOutput->FindObject("hdRElectron2Muon2")->Clone();
  //   hdRElectron2Muon2->Write();
  
  //   TH1D *hdRElectron2Tau1 = (TH1D*)fOutput->FindObject("hdRElectron2Tau1")->Clone();
  //   hdRElectron2Tau1->Write();
  
  //   TH1D *hdRElectron2Tau2 = (TH1D*)fOutput->FindObject("hdRElectron2Tau2")->Clone();
  //   hdRElectron2Tau2->Write();
  
  TH1D *hElectronTruthType = (TH1D*)fOutput->FindObject("hElectronTruthType")->Clone();
  hElectronTruthType->Write();
  
  TH1D *hElectronBSTruthType = (TH1D*)fOutput->FindObject("hElectronBSTruthType")->Clone();
  hElectronBSTruthType->Write();
  
  //  cout << "muon" << endl;  
  TH1D *hRecoMuons = (TH1D*)fOutput->FindObject("hRecoMuons")->Clone();
  hRecoMuons->Write();
  
  //  cout << "muon2" << endl;  
  TH1D *hRecoOkMuons = (TH1D*)fOutput->FindObject("hRecoOkMuons")->Clone();
  hRecoOkMuons->Write();
  
  //  cout << "muon pt" << endl;  
  TH1D *hMuon1pT = (TH1D*)fOutput->FindObject("hMuon1pT")->Clone();
  hMuon1pT->Write();
  
  TH1D *hMuon2pT = (TH1D*)fOutput->FindObject("hMuon2pT")->Clone();
  hMuon2pT->Write();
  
  TH1D *hMuon3pT = (TH1D*)fOutput->FindObject("hMuon3pT")->Clone();
  hMuon3pT->Write();
  
  //  cout << "muon eta" << endl;  
  TH1D *hMuon1Eta = (TH1D*)fOutput->FindObject("hMuon1Eta")->Clone();
  hMuon1Eta->Write();
  
  TH1D *hMuon2Eta = (TH1D*)fOutput->FindObject("hMuon2Eta")->Clone();
  hMuon2Eta->Write();
  
  TH1D *hMuon3Eta = (TH1D*)fOutput->FindObject("hMuon3Eta")->Clone();
  hMuon3Eta->Write();
  
  //  cout << "muon phi" << endl;  
  TH1D *hMuon1Phi = (TH1D*)fOutput->FindObject("hMuon1Phi")->Clone();
  hMuon1Phi->Write();
  
  TH1D *hMuon2Phi = (TH1D*)fOutput->FindObject("hMuon2Phi")->Clone();
  hMuon2Phi->Write();
  
  TH1D *hMuon3Phi = (TH1D*)fOutput->FindObject("hMuon3Phi")->Clone();
  hMuon3Phi->Write();
  
  TH1D *hMuon1Mass = (TH1D*)fOutput->FindObject("hMuon1Mass")->Clone();
  hMuon1Mass->Write();
  
  TH1D *hMuon2Mass = (TH1D*)fOutput->FindObject("hMuon2Mass")->Clone();
  hMuon2Mass->Write();
  
  TH1D *hMuon3Mass = (TH1D*)fOutput->FindObject("hMuon3Mass")->Clone();
  hMuon3Mass->Write();
  
  TH1D *hMuon1Beta = (TH1D*)fOutput->FindObject("hMuon1Beta")->Clone();
  hMuon1Beta->Write();
  
  TH1D *hMuon2Beta = (TH1D*)fOutput->FindObject("hMuon2Beta")->Clone();
  hMuon2Beta->Write();
  
  TH1D *hMuon3Beta = (TH1D*)fOutput->FindObject("hMuon3Beta")->Clone();
  hMuon3Beta->Write();

  //  cout << "muon dR" << endl;  
  TH1D *hdRMuon1Muon2 = (TH1D*)fOutput->FindObject("hdRMuon1Muon2")->Clone();
  hdRMuon1Muon2->Write();
  
  TH1D *hdRMuon1Muon3 = (TH1D*)fOutput->FindObject("hdRMuon1Muon3")->Clone();
  hdRMuon1Muon3->Write();
  
  TH1D *hdRMuon2Muon3 = (TH1D*)fOutput->FindObject("hdRMuon2Muon3")->Clone();
  hdRMuon2Muon3->Write();
  
  //   TH1D *hdRMuon1Tau1 = (TH1D*)fOutput->FindObject("hdRMuon1Tau1")->Clone();
  //   hdRMuon1Tau1->Write();
  
  //   TH1D *hdRMuon1Tau2 = (TH1D*)fOutput->FindObject("hdRMuon1Tau2")->Clone();
  //   hdRMuon1Tau2->Write();
  
  //   TH1D *hdRMuon2Tau1 = (TH1D*)fOutput->FindObject("hdRMuon2Tau1")->Clone();
  //   hdRMuon2Tau1->Write();
  
  //   TH1D *hdRMuon2Tau2 = (TH1D*)fOutput->FindObject("hdRMuon2Tau2")->Clone();
  //   hdRMuon2Tau2->Write();
  
  TH1D *hMuonTruthType = (TH1D*)fOutput->FindObject("hMuonTruthType")->Clone();
  hMuonTruthType->Write();
  
  TH1D *hMuonBSTruthType = (TH1D*)fOutput->FindObject("hMuonBSTruthType")->Clone();
  hMuonBSTruthType->Write();
  
  //  cout << "slow muon" << endl;  
  TH1D *hRecoMuLowBetaMuons = (TH1D*)fOutput->FindObject("hRecoMuLowBetaMuons")->Clone();
  hRecoMuLowBetaMuons->Write();
  
  //  cout << "slow LowBetaMuon2" << endl;  
  TH1D *hRecoOkMuLowBetaMuons = (TH1D*)fOutput->FindObject("hRecoOkMuLowBetaMuons")->Clone();
  hRecoOkMuLowBetaMuons->Write();
  
  //  cout << "LowBetaMuon pt" << endl;  
  TH1D *hMuLowBetaMuon1pT = (TH1D*)fOutput->FindObject("hMuLowBetaMuon1pT")->Clone();
  hMuLowBetaMuon1pT->Write();
  
  TH1D *hMuLowBetaMuon2pT = (TH1D*)fOutput->FindObject("hMuLowBetaMuon2pT")->Clone();
  hMuLowBetaMuon2pT->Write();
  
  TH1D *hMuLowBetaMuon3pT = (TH1D*)fOutput->FindObject("hMuLowBetaMuon3pT")->Clone();
  hMuLowBetaMuon3pT->Write();
  
  //  cout << "LowBetaMuon eta" << endl;  
  TH1D *hMuLowBetaMuon1Eta = (TH1D*)fOutput->FindObject("hMuLowBetaMuon1Eta")->Clone();
  hMuLowBetaMuon1Eta->Write();
  
  TH1D *hMuLowBetaMuon2Eta = (TH1D*)fOutput->FindObject("hMuLowBetaMuon2Eta")->Clone();
  hMuLowBetaMuon2Eta->Write();
  
  TH1D *hMuLowBetaMuon3Eta = (TH1D*)fOutput->FindObject("hMuLowBetaMuon3Eta")->Clone();
  hMuLowBetaMuon3Eta->Write();
  
  //  cout << "LowBetaMuon phi" << endl;  
  TH1D *hMuLowBetaMuon1Phi = (TH1D*)fOutput->FindObject("hMuLowBetaMuon1Phi")->Clone();
  hMuLowBetaMuon1Phi->Write();
  
  TH1D *hMuLowBetaMuon2Phi = (TH1D*)fOutput->FindObject("hMuLowBetaMuon2Phi")->Clone();
  hMuLowBetaMuon2Phi->Write();
  
  TH1D *hMuLowBetaMuon3Phi = (TH1D*)fOutput->FindObject("hMuLowBetaMuon3Phi")->Clone();
  hMuLowBetaMuon3Phi->Write();
  
  TH1D *hMuLowBetaMuon1Mass = (TH1D*)fOutput->FindObject("hMuLowBetaMuon1Mass")->Clone();
  hMuLowBetaMuon1Mass->Write();
  
  TH1D *hMuLowBetaMuon2Mass = (TH1D*)fOutput->FindObject("hMuLowBetaMuon2Mass")->Clone();
  hMuLowBetaMuon2Mass->Write();
  
  TH1D *hMuLowBetaMuon3Mass = (TH1D*)fOutput->FindObject("hMuLowBetaMuon3Mass")->Clone();
  hMuLowBetaMuon3Mass->Write();
  
  TH1D *hMuLowBetaMuon1MassBeta = (TH1D*)fOutput->FindObject("hMuLowBetaMuon1MassBeta")->Clone();
  hMuLowBetaMuon1MassBeta->Write();
  
  TH1D *hMuLowBetaMuon2MassBeta = (TH1D*)fOutput->FindObject("hMuLowBetaMuon2MassBeta")->Clone();
  hMuLowBetaMuon2MassBeta->Write();
  
  TH1D *hMuLowBetaMuon3MassBeta = (TH1D*)fOutput->FindObject("hMuLowBetaMuon3MassBeta")->Clone();
  hMuLowBetaMuon3MassBeta->Write();

  TH1D *hMuLowBetaMuon1Beta = (TH1D*)fOutput->FindObject("hMuLowBetaMuon1Beta")->Clone();
  hMuLowBetaMuon1Beta->Write();
  
  TH1D *hMuLowBetaMuon2Beta = (TH1D*)fOutput->FindObject("hMuLowBetaMuon2Beta")->Clone();
  hMuLowBetaMuon2Beta->Write();
  
  TH1D *hMuLowBetaMuon3Beta = (TH1D*)fOutput->FindObject("hMuLowBetaMuon3Beta")->Clone();
  hMuLowBetaMuon3Beta->Write();
  
  TH1D *hMuLowBetaMuonTruthType = (TH1D*)fOutput->FindObject("hMuLowBetaMuonTruthType")->Clone();
  hMuLowBetaMuonTruthType->Write();
  
  TH1D *hMuLowBetaMuonBSTruthType = (TH1D*)fOutput->FindObject("hMuLowBetaMuonBSTruthType")->Clone();
  hMuLowBetaMuonBSTruthType->Write();
  
  //  cout << "Mu Girl" << endl;  
  TH1D *hRecoMuGirlMuons = (TH1D*)fOutput->FindObject("hRecoMuGirlMuons")->Clone();
  hRecoMuGirlMuons->Write();
  
  //  cout << "slow MuGirlMuon2" << endl;  
  TH1D *hRecoOkMuGirlMuons = (TH1D*)fOutput->FindObject("hRecoOkMuGirlMuons")->Clone();
  hRecoOkMuGirlMuons->Write();
  
  //  cout << "MuGirlMuon pt" << endl;  
  TH1D *hMuGirlMuon1pT = (TH1D*)fOutput->FindObject("hMuGirlMuon1pT")->Clone();
  hMuGirlMuon1pT->Write();
  
  TH1D *hMuGirlMuon2pT = (TH1D*)fOutput->FindObject("hMuGirlMuon2pT")->Clone();
  hMuGirlMuon2pT->Write();
  
  TH1D *hMuGirlMuon3pT = (TH1D*)fOutput->FindObject("hMuGirlMuon3pT")->Clone();
  hMuGirlMuon3pT->Write();
  
  //  cout << "MuGirlMuon eta" << endl;  
  TH1D *hMuGirlMuon1Eta = (TH1D*)fOutput->FindObject("hMuGirlMuon1Eta")->Clone();
  hMuGirlMuon1Eta->Write();
  
  TH1D *hMuGirlMuon2Eta = (TH1D*)fOutput->FindObject("hMuGirlMuon2Eta")->Clone();
  hMuGirlMuon2Eta->Write();
  
  TH1D *hMuGirlMuon3Eta = (TH1D*)fOutput->FindObject("hMuGirlMuon3Eta")->Clone();
  hMuGirlMuon3Eta->Write();
  
  //  cout << "MuGirlMuon phi" << endl;  
  TH1D *hMuGirlMuon1Phi = (TH1D*)fOutput->FindObject("hMuGirlMuon1Phi")->Clone();
  hMuGirlMuon1Phi->Write();
  
  TH1D *hMuGirlMuon2Phi = (TH1D*)fOutput->FindObject("hMuGirlMuon2Phi")->Clone();
  hMuGirlMuon2Phi->Write();
  
  TH1D *hMuGirlMuon3Phi = (TH1D*)fOutput->FindObject("hMuGirlMuon3Phi")->Clone();
  hMuGirlMuon3Phi->Write();
  
  TH1D *hMuGirlMuon1Mass = (TH1D*)fOutput->FindObject("hMuGirlMuon1Mass")->Clone();
  hMuGirlMuon1Mass->Write();
  
  TH1D *hMuGirlMuon2Mass = (TH1D*)fOutput->FindObject("hMuGirlMuon2Mass")->Clone();
  hMuGirlMuon2Mass->Write();
  
  TH1D *hMuGirlMuon3Mass = (TH1D*)fOutput->FindObject("hMuGirlMuon3Mass")->Clone();
  hMuGirlMuon3Mass->Write();
  
  TH1D *hMuGirlMuon1MassBeta = (TH1D*)fOutput->FindObject("hMuGirlMuon1MassBeta")->Clone();
  hMuGirlMuon1MassBeta->Write();
  
  TH1D *hMuGirlMuon2MassBeta = (TH1D*)fOutput->FindObject("hMuGirlMuon2MassBeta")->Clone();
  hMuGirlMuon2MassBeta->Write();
  
  TH1D *hMuGirlMuon3MassBeta = (TH1D*)fOutput->FindObject("hMuGirlMuon3MassBeta")->Clone();
  hMuGirlMuon3MassBeta->Write();

  TH1D *hMuGirlMuon1Beta = (TH1D*)fOutput->FindObject("hMuGirlMuon1Beta")->Clone();
  hMuGirlMuon1Beta->Write();
  
  TH1D *hMuGirlMuon2Beta = (TH1D*)fOutput->FindObject("hMuGirlMuon2Beta")->Clone();
  hMuGirlMuon2Beta->Write();
  
  TH1D *hMuGirlMuon3Beta = (TH1D*)fOutput->FindObject("hMuGirlMuon3Beta")->Clone();
  hMuGirlMuon3Beta->Write();
  
  TH1D *hMuGirlMuonTruthType = (TH1D*)fOutput->FindObject("hMuGirlMuonTruthType")->Clone();
  hMuGirlMuonTruthType->Write();
  
  TH1D *hMuGirlMuonBSTruthType = (TH1D*)fOutput->FindObject("hMuGirlMuonBSTruthType")->Clone();
  hMuGirlMuonBSTruthType->Write();
  
  //   //  cout << "Taus" << endl;  
  //   TH1D *hRecoTaus = (TH1D*)fOutput->FindObject("hRecoTaus")->Clone();
  //   hRecoTaus->Write();
  
  //   TH1D *hRecoOkTaus = (TH1D*)fOutput->FindObject("hRecoOkTaus")->Clone();
  //   hRecoOkTaus->Write();
  
  //   //  cout << "Taus pt" << endl;  
  //   TH1D *hTau1pT = (TH1D*)fOutput->FindObject("hTau1pT")->Clone();
  //   hTau1pT->Write();
  
  //   TH1D *hTau2pT = (TH1D*)fOutput->FindObject("hTau2pT")->Clone();
  //   hTau2pT->Write();
  
  //   TH1D *hTau3pT = (TH1D*)fOutput->FindObject("hTau3pT")->Clone();
  //   hTau3pT->Write();
  
  //   //  cout << "Taus eta" << endl;  
  //   TH1D *hTau1Eta = (TH1D*)fOutput->FindObject("hTau1Eta")->Clone();
  //   hTau1Eta->Write();
  
  //   TH1D *hTau2Eta = (TH1D*)fOutput->FindObject("hTau2Eta")->Clone();
  //   hTau2Eta->Write();
  
  //   TH1D *hTau3Eta = (TH1D*)fOutput->FindObject("hTau3Eta")->Clone();
  //   hTau3Eta->Write();
  
  //   //  cout << "Taus phi" << endl;  
  //   TH1D *hTau1Phi = (TH1D*)fOutput->FindObject("hTau1Phi")->Clone();
  //   hTau1Phi->Write();
  
  //   TH1D *hTau2Phi = (TH1D*)fOutput->FindObject("hTau2Phi")->Clone();
  //   hTau2Phi->Write();
  
  //   TH1D *hTau3Phi = (TH1D*)fOutput->FindObject("hTau3Phi")->Clone();
  //   hTau3Phi->Write();
  
  //   TH1D *hTau1Mass = (TH1D*)fOutput->FindObject("hTau1Mass")->Clone();
  //   hTau1Mass->Write();
  
  //   TH1D *hTau2Mass = (TH1D*)fOutput->FindObject("hTau2Mass")->Clone();
  //   hTau2Mass->Write();
  
  //   TH1D *hTau3Mass = (TH1D*)fOutput->FindObject("hTau3Mass")->Clone();
  //   hTau3Mass->Write();
  
  TH1D *hRecoOkLeptons = (TH1D*)fOutput->FindObject("hRecoOkLeptons")->Clone();
  hRecoOkLeptons->Write();
  
  //   //  cout << "Taus dr" << endl;  
  //   TH1D *hdRTau1Tau2 = (TH1D*)fOutput->FindObject("hdRTau1Tau2")->Clone();
  //   hdRTau1Tau2->Write();
  
  //   //  cout << "Taus dr1" << endl;  
  //   TH1D *hdRTau1Tau3 = (TH1D*)fOutput->FindObject("hdRTau1Tau3")->Clone();
  //   hdRTau1Tau3->Write();
  
  //   //  cout << "Taus dr2" << endl;  
  //   TH1D *hdRTau2Tau3 = (TH1D*)fOutput->FindObject("hdRTau2Tau3")->Clone();
  //   hdRTau2Tau3->Write();
  
  //MET
  TH1D *hMET = (TH1D*)fOutput->FindObject("hMET")->Clone();
  hMET->Write();
  
  TH1D *hMETPhi = (TH1D*)fOutput->FindObject("hMETPhi")->Clone();
  hMETPhi->Write();
  
  TH1D *hNHiggs = (TH1D*)fOutput->FindObject("hNHiggs")->Clone();
  hNHiggs->Write();
  
  TH1D *lshMassHiggs = (TH1D*)fOutput->FindObject("lshMassHiggs")->Clone();
  lshMassHiggs->Write();
  
  TH1D *lshMassZ = (TH1D*)fOutput->FindObject("lshMassZ")->Clone();
  lshMassZ->Write();
  //  cout << "Electrons" << endl;  
  
  // Parton b's PT spectrum from Higgs1 and Higgs2
  TH1D *lshPTb1 = (TH1D*)fOutput->FindObject("lshPTb1")->Clone();
  lshPTb1->Write();
  TH1D *lshPTb2 = (TH1D*)fOutput->FindObject("lshPTb2")->Clone();
  lshPTb2->Write();
  TH1D *lshPTb3 = (TH1D*)fOutput->FindObject("lshPTb3")->Clone();
  lshPTb3->Write();
  TH1D *lshPTb4 = (TH1D*)fOutput->FindObject("lshPTb4")->Clone();
  lshPTb4->Write();
  
  // PT of matched jets
  TH1D *lshJet1Match = (TH1D*)fOutput->FindObject("lshJet1Match")->Clone();
  lshJet1Match->Write();
  TH1D *lshJet2Match = (TH1D*)fOutput->FindObject("lshJet2Match")->Clone();
  lshJet2Match->Write();
  
  // Matched and unmatched reco Higgs masses from jets
  TH1D *lshJetsMass34 = (TH1D*)fOutput->FindObject("lshJetsMass34")->Clone();
  lshJetsMass34->Write();
  TH1D *lshJetsNewMass34 = (TH1D*)fOutput->FindObject("lshJetsNewMass34")->Clone();
  lshJetsNewMass34->Write();
  
  TH1D *lshRecoJJInvMass = (TH1D*)fOutput->FindObject("lshRecoJJInvMass")->Clone();
  lshRecoJJInvMass->Write();
  TH1D *lshRecoBBInvMass = (TH1D*)fOutput->FindObject("lshRecoBBInvMass")->Clone();
  lshRecoBBInvMass->Write();
  TH1D *lshRecoEleInvMass = (TH1D*)fOutput->FindObject("lshRecoEleInvMass")->Clone();
  lshRecoEleInvMass->Write();

  TH1D *hdRMinDilep = (TH1D*)fOutput->FindObject("hdRMinDilep")->Clone();
  hdRMinDilep->Write();
  TH1D *hdRMinDilepInvMass = (TH1D*)fOutput->FindObject("hdRMinDilepInvMass")->Clone();
  hdRMinDilepInvMass->Write();
  TH2D *h2dInvMassMindRDilep = (TH2D*)fOutput->FindObject("h2dInvMassMindRDilep")->Clone();
  h2dInvMassMindRDilep->Write();
  TH1D *hdRDilep = (TH1D*)fOutput->FindObject("hdRDilep")->Clone();
  hdRDilep->Write();
  TH1D *hDilepInvMass = (TH1D*)fOutput->FindObject("hDilepInvMass")->Clone();
  hDilepInvMass->Write();
  TH2D *h2dInvMassdRDilep = (TH2D*)fOutput->FindObject("h2dInvMassdRDilep")->Clone();
  h2dInvMassdRDilep->Write();

  TH1D *hdRMinLeptons = (TH1D*)fOutput->FindObject("hdRMinLeptons")->Clone();
  hdRMinLeptons->Write();
  TH1D *hMassMindRLeptons = (TH1D*)fOutput->FindObject("hMassMindRLeptons")->Clone();
  hMassMindRLeptons->Write();
  TH2D *h2dMassMindRLeptons = (TH2D*)fOutput->FindObject("h2dMassMindRLeptons")->Clone();
  h2dMassMindRLeptons->Write();
  TH1D *hdRLeptons = (TH1D*)fOutput->FindObject("hdRLeptons")->Clone();
  hdRLeptons->Write();
  TH1D *hMassdRLeptons = (TH1D*)fOutput->FindObject("hMassdRLeptons")->Clone();
  hMassdRLeptons->Write();
  TH2D *h2dMassdRLeptons = (TH2D*)fOutput->FindObject("h2dMassdRLeptons")->Clone();
  h2dMassdRLeptons->Write();
  
  //bTagInfo
  
  TH1D *lshJetFlavor1 = (TH1D*)fOutput->FindObject("lshJetFlavor1")->Clone();
  lshJetFlavor1->Write();
  TH1D *lshJetFlavor2 = (TH1D*)fOutput->FindObject("lshJetFlavor2")->Clone();
  lshJetFlavor2->Write();
  TH1D *lshJetFlavor3 = (TH1D*)fOutput->FindObject("lshJetFlavor3")->Clone();
  lshJetFlavor3->Write();
  TH1D *lshJetFlavor4 = (TH1D*)fOutput->FindObject("lshJetFlavor4")->Clone();
  lshJetFlavor4->Write();
  TH1D *lshJetFlavor5 = (TH1D*)fOutput->FindObject("lshJetFlavor5")->Clone();
  lshJetFlavor5->Write();
  TH1D *lshJetFlavor6 = (TH1D*)fOutput->FindObject("lshJetFlavor6")->Clone();
  lshJetFlavor6->Write();
  TH1D *lshJetFlavorMatch1 = (TH1D*)fOutput->FindObject("lshJetFlavorMatch1")->Clone();
  lshJetFlavorMatch1->Write();
  TH1D *lshJetFlavorMatch2 = (TH1D*)fOutput->FindObject("lshJetFlavorMatch2")->Clone();
  lshJetFlavorMatch2->Write();
  
  TH1D *lshJetWeight1 = (TH1D*)fOutput->FindObject("lshJetWeight1")->Clone();
  lshJetWeight1->Write();
  TH1D *lshJetWeight2 = (TH1D*)fOutput->FindObject("lshJetWeight2")->Clone();
  lshJetWeight2->Write();
  TH1D *lshJetWeight3 = (TH1D*)fOutput->FindObject("lshJetWeight3")->Clone();
  lshJetWeight3->Write();
  TH1D *lshJetWeight4 = (TH1D*)fOutput->FindObject("lshJetWeight4")->Clone();
  lshJetWeight4->Write();
  TH1D *lshJetWeight5 = (TH1D*)fOutput->FindObject("lshJetWeight5")->Clone();
  lshJetWeight5->Write();
  TH1D *lshJetWeight6 = (TH1D*)fOutput->FindObject("lshJetWeight6")->Clone();
  lshJetWeight6->Write();
  TH1D *lshJetWeightMatch1 = (TH1D*)fOutput->FindObject("lshJetWeightMatch1")->Clone();
  lshJetWeightMatch1->Write();
  TH1D *lshJetWeightMatch2 = (TH1D*)fOutput->FindObject("lshJetWeightMatch2")->Clone();
  lshJetWeightMatch2->Write();
  
  
  // Parton Level quantities
  
  TH1D *lshMinus = (TH1D*)fOutput->FindObject("lshMinus")->Clone();
  lshMinus->Write();
  TH1D *lshEMinus = (TH1D*)fOutput->FindObject("lshEMinus")->Clone();
  lshEMinus->Write();
  TH1D *lshMuMinus = (TH1D*)fOutput->FindObject("lshMuMinus")->Clone();
  lshMuMinus->Write();
  //  TH1D *lshTauMinus = (TH1D*)fOutput->FindObject("lshTauMinus")->Clone();
  //  lshTauMinus->Write();
  
  TH1D *lshPlus = (TH1D*)fOutput->FindObject("lshPlus")->Clone();
  lshPlus->Write();
  TH1D *lshEPlus = (TH1D*)fOutput->FindObject("lshEPlus")->Clone();
  lshEPlus->Write();
  TH1D *lshMuPlus = (TH1D*)fOutput->FindObject("lshMuPlus")->Clone();
  lshMuPlus->Write();
  //  TH1D *lshTauPlus = (TH1D*)fOutput->FindObject("lshTauPlus")->Clone();
  //  lshTauPlus->Write();
  
  TH1D *lshLepton = (TH1D*)fOutput->FindObject("lshLepton")->Clone();
  lshLepton->Write();
  
  
  
  // sleptons masses
  TH1D *lsh4 = (TH1D*)fOutput->FindObject("lsh4")->Clone();
  lsh4->Write();
  TH1D *lsh44 = (TH1D*)fOutput->FindObject("lsh44")->Clone();
  lsh44->Write();
  TH1D *lsh5 = (TH1D*)fOutput->FindObject("lsh5")->Clone();
  lsh5->Write();
  TH1D *lsh55 = (TH1D*)fOutput->FindObject("lsh55")->Clone();
  lsh55->Write();
  TH1D *lsh6 = (TH1D*)fOutput->FindObject("lsh6")->Clone();
  lsh6->Write();
  TH1D *lsh66 = (TH1D*)fOutput->FindObject("lsh66")->Clone();
  lsh66->Write();
  
  TH1D *lshMassChi0e = (TH1D*)fOutput->FindObject("lshMassChi0e")->Clone();
  lshMassChi0e->Write();
  TH1D *lshMassChi0m = (TH1D*)fOutput->FindObject("lshMassChi0m")->Clone();
  lshMassChi0m->Write();
  TH1D *lshMassChi0t = (TH1D*)fOutput->FindObject("lshMassChi0t")->Clone();
  lshMassChi0t->Write();
  TH1D *lshChi02Mass = (TH1D*)fOutput->FindObject("lshChi02Mass")->Clone();
  lshChi02Mass->Write();
  TH1D *lshChi01Mass = (TH1D*)fOutput->FindObject("lshChi01Mass")->Clone();
  lshChi01Mass->Write();
  TH1D *lshChi03Mass = (TH1D*)fOutput->FindObject("lshChi03Mass")->Clone();
  lshChi03Mass->Write();
  
  
  // TRIGGER STUDIES
  TH1D *hn_sel_pt = (TH1D*)fOutput->FindObject("hn_sel_pt")->Clone();
  hn_sel_pt->Write();
  
  TH1D *hn_sel_L1_pt = (TH1D*)fOutput->FindObject("hn_sel_L1_pt")->Clone();
  hn_sel_L1_pt->Write();
  
  TH1D *hn_sel_L2_pt = (TH1D*)fOutput->FindObject("hn_sel_L2_pt")->Clone();
  hn_sel_L2_pt->Write();
  
  TH1D *hn_sel_EF_pt = (TH1D*)fOutput->FindObject("hn_sel_EF_pt")->Clone();
  hn_sel_EF_pt->Write();
  
  TH1D *hn_ratio_pt_L1SEL = (TH1D*)fOutput->FindObject("hn_ratio_pt_L1SEL")->Clone();
  hn_ratio_pt_L1SEL->Divide(hn_sel_L1_pt,hn_sel_pt);                                                                         
  hn_ratio_pt_L1SEL->Write();
  
  TH1D *hn_ratio_pt_L1L2 = (TH1D*)fOutput->FindObject("hn_ratio_pt_L1L2")->Clone();
  hn_ratio_pt_L1L2->Divide(hn_sel_L2_pt,hn_sel_L1_pt);                                                                       
  hn_ratio_pt_L1L2->Write();
  
  TH1D *hn_ratio_pt_L2EF = (TH1D*)fOutput->FindObject("hn_ratio_pt_L2EF")->Clone();
  hn_ratio_pt_L2EF->Divide(hn_sel_EF_pt,hn_sel_L2_pt);                                                                       
  hn_ratio_pt_L2EF->Write();
  
  TH1D *hn_slow_sel_pt = (TH1D*)fOutput->FindObject("hn_slow_sel_pt")->Clone();  
  hn_slow_sel_pt->Write();
  
  TH1D *hn_slow_sel_L1_pt = (TH1D*)fOutput->FindObject("hn_slow_sel_L1_pt")->Clone();
  hn_slow_sel_L1_pt->Write();
  
  TH1D *hn_slow_sel_L2_pt = (TH1D*)fOutput->FindObject("hn_slow_sel_L2_pt")->Clone();
  hn_slow_sel_L2_pt->Write();
  
  TH1D *hn_slow_sel_EF_pt = (TH1D*)fOutput->FindObject("hn_slow_sel_EF_pt")->Clone();
  hn_slow_sel_EF_pt->Write();
  
  TH1D *hn_sel_eta = (TH1D*)fOutput->FindObject("hn_sel_eta")->Clone();
  hn_sel_eta->Write();
  
  TH1D *hn_sel_L1_eta = (TH1D*)fOutput->FindObject("hn_sel_L1_eta")->Clone();
  hn_sel_L1_eta->Write();
  
  TH1D *hn_sel_L2_eta = (TH1D*)fOutput->FindObject("hn_sel_L2_eta")->Clone();
  hn_sel_L2_eta->Write();
  
  TH1D *hn_slow_sel_eta = (TH1D*)fOutput->FindObject("hn_slow_sel_eta")->Clone();
  hn_slow_sel_eta->Write();
  
  TH1D *hn_slow_sel_L1_eta = (TH1D*)fOutput->FindObject("hn_slow_sel_L1_eta")->Clone();
  hn_slow_sel_L1_eta->Write();
  
  TH1D *hn_slow_sel_L2_eta = (TH1D*)fOutput->FindObject("hn_slow_sel_L2_eta")->Clone();
  hn_slow_sel_L2_eta->Write();
  
  TH1D *hn_slow_sel_EF_eta = (TH1D*)fOutput->FindObject("hn_slow_sel_EF_eta")->Clone();
  hn_slow_sel_EF_eta->Write();
  
  TH1D *hn_slow_ratio_eta_L1SEL = (TH1D*)fOutput->FindObject("hn_slow_ratio_eta_L1SEL")->Clone();
  hn_slow_ratio_eta_L1SEL->Write();
  
  TH1D *hn_slow_ratio_eta_L1L2 = (TH1D*)fOutput->FindObject("hn_slow_ratio_eta_L1L2")->Clone();
  hn_slow_ratio_eta_L1L2->Write();
  
  TH1D *hn_slow_ratio_eta_L2EF = (TH1D*)fOutput->FindObject("hn_slow_ratio_eta_L2EF")->Clone();
  hn_slow_ratio_eta_L2EF->Write();
  
  TH1D *hn_sel_EF_eta = (TH1D*)fOutput->FindObject("hn_sel_EF_eta")->Clone();
  hn_sel_EF_eta->Write();
  
  TH1D *hn_ratio_eta_L1SEL = (TH1D*)fOutput->FindObject("hn_ratio_eta_L1SEL")->Clone();
  hn_ratio_eta_L1SEL->Write();
  
  TH1D *hn_ratio_eta_L1L2 = (TH1D*)fOutput->FindObject("hn_ratio_eta_L1L2")->Clone();
  hn_ratio_eta_L1L2->Write();
  
  TH1D *hn_ratio_eta_L2EF = (TH1D*)fOutput->FindObject("hn_ratio_eta_L2EF")->Clone();
  hn_ratio_eta_L2EF->Write();
  
  TH1D *hn_slow_ratio_pt_L1SEL = (TH1D*)fOutput->FindObject("hn_slow_ratio_pt_L1SEL")->Clone();
  hn_slow_ratio_pt_L1SEL->Divide(hn_slow_sel_L1_pt,hn_slow_sel_pt);                                                                         
  hn_slow_ratio_pt_L1SEL->Write();
  
  TH1D *hn_slow_ratio_pt_L1L2 = (TH1D*)fOutput->FindObject("hn_slow_ratio_pt_L1L2")->Clone();
  hn_slow_ratio_pt_L1L2->Divide(hn_slow_sel_L2_pt,hn_slow_sel_L1_pt);                                                                       
  hn_slow_ratio_pt_L1L2->Write();
  
  TH1D *hn_slow_ratio_pt_L2EF = (TH1D*)fOutput->FindObject("hn_slow_ratio_pt_L2EF")->Clone();
  hn_slow_ratio_pt_L2EF->Divide(hn_slow_sel_EF_pt,hn_slow_sel_L2_pt); 
  hn_slow_ratio_pt_L2EF->Write();
  
  TH1D *hn_sel_phi = (TH1D*)fOutput->FindObject("hn_sel_phi")->Clone();
  hn_sel_phi->Write();
  
  TH1D *hn_sel_L1_phi = (TH1D*)fOutput->FindObject("hn_sel_L1_phi")->Clone();
  hn_sel_L1_phi->Write();
  
  TH1D *hn_sel_L2_phi = (TH1D*)fOutput->FindObject("hn_sel_L2_phi")->Clone();
  hn_sel_L2_phi->Write();
  
  TH1D *hn_sel_EF_phi = (TH1D*)fOutput->FindObject("hn_sel_EF_phi")->Clone();
  hn_sel_EF_phi->Write();
  
  TH1D *hn_ratio_phi_L1SEL = (TH1D*)fOutput->FindObject("hn_ratio_phi_L1SEL")->Clone();
  hn_ratio_phi_L1SEL->Write();
  
  TH1D *hn_ratio_phi_L1L2 = (TH1D*)fOutput->FindObject("hn_ratio_phi_L1L2")->Clone();
  hn_ratio_phi_L1L2->Write();
  
  TH1D *hn_ratio_phi_L2EF = (TH1D*)fOutput->FindObject("hn_ratio_phi_L2EF")->Clone();
  hn_ratio_phi_L2EF->Write();
  
  TH1D *hn_slow_sel_phi = (TH1D*)fOutput->FindObject("hn_slow_sel_phi")->Clone();
  hn_slow_sel_phi->Write();
  
  TH1D *hn_slow_sel_L1_phi = (TH1D*)fOutput->FindObject("hn_slow_sel_L1_phi")->Clone();
  hn_slow_sel_L1_phi->Write();
  
  TH1D *hn_slow_sel_L2_phi = (TH1D*)fOutput->FindObject("hn_slow_sel_L2_phi")->Clone();
  hn_slow_sel_L2_phi->Write();
  
  TH1D *hn_slow_sel_EF_phi = (TH1D*)fOutput->FindObject("hn_slow_sel_EF_phi")->Clone();
  hn_slow_sel_EF_phi->Write();
  
  TH1D *hn_slow_ratio_phi_L1SEL = (TH1D*)fOutput->FindObject("hn_slow_ratio_phi_L1SEL")->Clone();
  hn_slow_ratio_phi_L1SEL->Write();
  
  TH1D *hn_slow_ratio_phi_L1L2 = (TH1D*)fOutput->FindObject("hn_slow_ratio_phi_L1L2")->Clone();
  hn_slow_ratio_phi_L1L2->Write();
  
  TH2D * hn_slow_ratio_phi_L2EF = (TH2D*)fOutput->FindObject("hn_slow_ratio_phi_L2EF")->Clone();
  hn_slow_ratio_phi_L2EF->Write();
  
  TH2D * hn_sel_eta_phi =  (TH2D*)fOutput->FindObject("hn_sel_eta_phi")->Clone();
  hn_sel_eta_phi->Write();
  
  TH2D * hn_sel_L1_eta_phi =  (TH2D*)fOutput->FindObject("hn_sel_L1_eta_phi")->Clone();
  hn_sel_L1_eta_phi->Write();
  
  TH2D * hn_sel_L2_eta_phi =  (TH2D*)fOutput->FindObject("hn_sel_L2_eta_phi")->Clone();
  hn_sel_L2_eta_phi->Write();
  
  TH2D * hn_sel_EF_eta_phi =  (TH2D*)fOutput->FindObject("hn_sel_EF_eta_phi")->Clone();
  hn_sel_EF_eta_phi->Write();
  
  TH2D * hn_ratio_eta_phi_L1SEL =  (TH2D*)fOutput->FindObject("hn_ratio_eta_phi_L1SEL")->Clone();
  hn_ratio_eta_phi_L1SEL->Write();
  
  TH2D * hn_ratio_eta_phi_L1L2 =  (TH2D*)fOutput->FindObject("hn_ratio_eta_phi_L1L2")->Clone();
  hn_ratio_eta_phi_L1L2->Write();
  
  TH2D * hn_ratio_eta_phi_L2EF =  (TH2D*)fOutput->FindObject("hn_ratio_eta_phi_L2EF")->Clone();
  hn_ratio_eta_phi_L2EF->Write();
  
  TH2D * hn_slow_sel_eta_phi =  (TH2D*)fOutput->FindObject("hn_slow_sel_eta_phi")->Clone();
  hn_slow_sel_eta_phi->Write();
  
  TH2D * hn_slow_sel_L1_eta_phi =  (TH2D*)fOutput->FindObject("hn_slow_sel_L1_eta_phi")->Clone();
  hn_slow_sel_L1_eta_phi->Write();
  
  TH2D * hn_slow_sel_L2_eta_phi =  (TH2D*)fOutput->FindObject("hn_slow_sel_L2_eta_phi")->Clone();
  hn_slow_sel_L2_eta_phi->Write();
  
  TH2D * hn_slow_sel_EF_eta_phi =  (TH2D*)fOutput->FindObject("hn_slow_sel_EF_eta_phi")->Clone();
  hn_slow_sel_EF_eta_phi->Write();
  
  TH2D * hn_slow_ratio_eta_phi_L1SEL =  (TH2D*)fOutput->FindObject("hn_slow_ratio_eta_phi_L1SEL")->Clone();
  hn_slow_ratio_eta_phi_L1SEL->Write();
  
  TH2D * hn_slow_ratio_eta_phi_L1L2 =  (TH2D*)fOutput->FindObject("hn_slow_ratio_eta_phi_L1L2")->Clone();
  hn_slow_ratio_eta_phi_L1L2->Write();
  
  TH2D * hn_slow_ratio_eta_phi_L2EF =  (TH2D*)fOutput->FindObject("hn_slow_ratio_eta_phi_L2EF")->Clone();
  hn_slow_ratio_eta_phi_L2EF->Write();
  
  TGraphAsymmErrors *hn_bayratio_pt_L1SEL =  new TGraphAsymmErrors();
  hn_bayratio_pt_L1SEL->BayesDivide(hn_sel_L1_pt,hn_sel_pt);            // Using automatic errors generation
  hn_bayratio_pt_L1SEL->SetMarkerStyle(8);
  hn_bayratio_pt_L1SEL->SetMarkerColor(9);
  hn_bayratio_pt_L1SEL->SetTitle("L1_MU10 Trigger efficiency vs #mu pT");
  hn_bayratio_pt_L1SEL->GetXaxis()->SetTitle("p_{T} [GeV]");
  hn_bayratio_pt_L1SEL->GetYaxis()->SetTitle("Eff.");
  hn_bayratio_pt_L1SEL->Write("hn_bayratio_pt_L1SEL");  
  
  TGraphAsymmErrors *hn_bayratio_pt_L1L2 =  new TGraphAsymmErrors();
  hn_bayratio_pt_L1L2->BayesDivide(hn_sel_L2_pt,hn_sel_L1_pt);            // Using automatic errors genebayration
  hn_bayratio_pt_L1L2->SetMarkerStyle(8);
  hn_bayratio_pt_L1L2->SetMarkerColor(9);
  hn_bayratio_pt_L1L2->SetTitle("L2_mu40_slow Trigger efficiency vs #mu pT");
  hn_bayratio_pt_L1L2->GetXaxis()->SetTitle("p_{T} [GeV]");
  hn_bayratio_pt_L1L2->GetYaxis()->SetTitle("Eff.");
  hn_bayratio_pt_L1L2->Write("hn_bayratio_pt_L1L2");  

  TGraphAsymmErrors *hn_bayratio_eta_L1SEL =  new TGraphAsymmErrors();
  hn_bayratio_eta_L1SEL->BayesDivide(hn_sel_L1_eta,hn_sel_eta);            // Using automatic errors generation
  hn_bayratio_eta_L1SEL->SetMarkerStyle(8);
  hn_bayratio_eta_L1SEL->SetMarkerColor(9);
  hn_bayratio_eta_L1SEL->SetTitle("L1_MU10 Trigger efficiency vs #mu eta");
  hn_bayratio_eta_L1SEL->GetXaxis()->SetTitle("eta");
  hn_bayratio_eta_L1SEL->GetYaxis()->SetTitle("Eff.");
  hn_bayratio_eta_L1SEL->Write("hn_bayratio_eta_L1SEL");  
  
  TGraphAsymmErrors *hn_bayratio_eta_L1L2 =  new TGraphAsymmErrors();
  hn_bayratio_eta_L1L2->BayesDivide(hn_sel_L2_eta,hn_sel_L1_eta);            // Using automatic errors genebayration
  hn_bayratio_eta_L1L2->SetMarkerStyle(8);
  hn_bayratio_eta_L1L2->SetMarkerColor(9);
  hn_bayratio_eta_L1L2->SetTitle("L2_mu40_slow Trigger efficiency vs #mu eta");
  hn_bayratio_eta_L1L2->GetXaxis()->SetTitle("eta");
  hn_bayratio_eta_L1L2->GetYaxis()->SetTitle("Eff.");
  hn_bayratio_eta_L1L2->Write("hn_bayratio_eta_L1L2");  
  
  TGraphAsymmErrors *hn_bayratio_phi_L1SEL =  new TGraphAsymmErrors();
  hn_bayratio_phi_L1SEL->BayesDivide(hn_sel_L1_phi,hn_sel_phi);            // Using automatic errors generation
  hn_bayratio_phi_L1SEL->SetMarkerStyle(8);
  hn_bayratio_phi_L1SEL->SetMarkerColor(9);
  hn_bayratio_phi_L1SEL->SetTitle("L1_MU10 Trigger efficiency vs #mu phi");
  hn_bayratio_phi_L1SEL->GetXaxis()->SetTitle("phi");
  hn_bayratio_phi_L1SEL->GetYaxis()->SetTitle("Eff.");
  hn_bayratio_phi_L1SEL->Write("hn_bayratio_phi_L1SEL");  
  
  TGraphAsymmErrors *hn_bayratio_phi_L1L2 =  new TGraphAsymmErrors();
  hn_bayratio_phi_L1L2->BayesDivide(hn_sel_L2_phi,hn_sel_L1_phi);            // Using automatic errors genebayration
  hn_bayratio_phi_L1L2->SetMarkerStyle(8);
  hn_bayratio_phi_L1L2->SetMarkerColor(9);
  hn_bayratio_phi_L1L2->SetTitle("L2_mu40_slow Trigger efficiency vs #mu pT");
  hn_bayratio_phi_L1L2->GetXaxis()->SetTitle("p_{T} [GeV]");
  hn_bayratio_phi_L1L2->GetYaxis()->SetTitle("Eff.");
  hn_bayratio_phi_L1L2->Write("hn_bayratio_phi_L1L2");  

  TGraphAsymmErrors *hn_slow_bayratio_pt_L1SEL =  new TGraphAsymmErrors();
  hn_slow_bayratio_pt_L1SEL->BayesDivide(hn_slow_sel_L1_pt,hn_slow_sel_pt);            // Using automatic errors generation
  hn_slow_bayratio_pt_L1SEL->SetMarkerStyle(8);
  hn_slow_bayratio_pt_L1SEL->SetMarkerColor(9);
  hn_slow_bayratio_pt_L1SEL->SetTitle("L1_MU10 Trigger efficiency vs #mu-girl muon pT");
  hn_slow_bayratio_pt_L1SEL->GetXaxis()->SetTitle("p_{T} [GeV]");
  hn_slow_bayratio_pt_L1SEL->GetYaxis()->SetTitle("Eff.");
  hn_slow_bayratio_pt_L1SEL->Write("hn_slow_bayratio_pt_L1SEL");  
  
  TGraphAsymmErrors *hn_slow_bayratio_pt_L1L2 =  new TGraphAsymmErrors();
  hn_slow_bayratio_pt_L1L2->BayesDivide(hn_slow_sel_L2_pt,hn_slow_sel_L1_pt);            // Using automatic errors genebayration
  hn_slow_bayratio_pt_L1L2->SetMarkerStyle(8);
  hn_slow_bayratio_pt_L1L2->SetMarkerColor(9);
  hn_slow_bayratio_pt_L1L2->SetTitle("L2_mu40_slow Trigger efficiency vs #mu-girl muon pT");
  hn_slow_bayratio_pt_L1L2->GetXaxis()->SetTitle("p_{T} [GeV]");
  hn_slow_bayratio_pt_L1L2->GetYaxis()->SetTitle("Eff.");
  hn_slow_bayratio_pt_L1L2->Write("hn_slow_bayratio_pt_L1L2");  
  
  TGraphAsymmErrors *hn_slow_bayratio_eta_L1SEL =  new TGraphAsymmErrors();
  hn_slow_bayratio_eta_L1SEL->BayesDivide(hn_slow_sel_L1_eta,hn_slow_sel_eta);            // Using automatic errors generation
  hn_slow_bayratio_eta_L1SEL->SetMarkerStyle(8);
  hn_slow_bayratio_eta_L1SEL->SetMarkerColor(9);
  hn_slow_bayratio_eta_L1SEL->SetTitle("L1_MU10 Trigger efficiency vs #mu-girl muon eta");
  hn_slow_bayratio_eta_L1SEL->GetXaxis()->SetTitle("eta");
  hn_slow_bayratio_eta_L1SEL->GetYaxis()->SetTitle("Eff.");
  hn_slow_bayratio_eta_L1SEL->Write("hn_slow_bayratio_eta_L1SEL");  
  
  TGraphAsymmErrors *hn_slow_bayratio_eta_L1L2 =  new TGraphAsymmErrors();
  hn_slow_bayratio_eta_L1L2->BayesDivide(hn_slow_sel_L2_eta,hn_slow_sel_L1_eta);            // Using automatic errors genebayration
  hn_slow_bayratio_eta_L1L2->SetMarkerStyle(8);
  hn_slow_bayratio_eta_L1L2->SetMarkerColor(9);
  hn_slow_bayratio_eta_L1L2->SetTitle("L2_mu40_slow Trigger efficiency vs #mu-girl muon eta");
  hn_slow_bayratio_eta_L1L2->GetXaxis()->SetTitle("eta");
  hn_slow_bayratio_eta_L1L2->GetYaxis()->SetTitle("Eff.");
  hn_slow_bayratio_eta_L1L2->Write("hn_slow_bayratio_eta_L1L2");  
  
  TGraphAsymmErrors *hn_slow_bayratio_phi_L1SEL =  new TGraphAsymmErrors();
  hn_slow_bayratio_phi_L1SEL->BayesDivide(hn_slow_sel_L1_phi,hn_slow_sel_phi);            // Using automatic errors generation
  hn_slow_bayratio_phi_L1SEL->SetMarkerStyle(8);
  hn_slow_bayratio_phi_L1SEL->SetMarkerColor(9);
  hn_slow_bayratio_phi_L1SEL->SetTitle("L1_MU10 Trigger efficiency vs #mu-girl muon phi");
  hn_slow_bayratio_phi_L1SEL->GetXaxis()->SetTitle("phi");
  hn_slow_bayratio_phi_L1SEL->GetYaxis()->SetTitle("Eff.");
  hn_slow_bayratio_phi_L1SEL->Write("hn_slow_bayratio_phi_L1SEL");  
  
  TGraphAsymmErrors *hn_slow_bayratio_phi_L1L2 =  new TGraphAsymmErrors();
  hn_slow_bayratio_phi_L1L2->BayesDivide(hn_slow_sel_L2_phi,hn_slow_sel_L1_phi);            // Using automatic errors genebayration
  hn_slow_bayratio_phi_L1L2->SetMarkerStyle(8);
  hn_slow_bayratio_phi_L1L2->SetMarkerColor(9);
  hn_slow_bayratio_phi_L1L2->SetTitle("L2_mu40_slow Trigger efficiency vs #mu-girl muon phi");
  hn_slow_bayratio_phi_L1L2->GetXaxis()->SetTitle("phi");
  hn_slow_bayratio_phi_L1L2->GetYaxis()->SetTitle("Eff.");
  hn_slow_bayratio_phi_L1L2->Write("hn_slow_bayratio_phi_L1L2");  
  
  
  fOut->Close();
  
  
  
  
}


double Analysis::CalcTranSphericity(double sumpx2,double sumpy2,double sumpxpy){
  double marray[4];
  for(int i=0;i<4;i++){
    marray[i]=0;
  }
  marray[0]=sumpx2;
  marray[1]=sumpxpy;
  marray[2]=sumpxpy;
  marray[3]=sumpy2;
  
  // use root matrix to find eigenvalues...
  TMatrixDSym matrix(2);  
  matrix.SetMatrixArray(marray);
  
  TMatrixDSymEigen eigen(matrix); 
  TVectorD E = eigen.GetEigenValues();   
  
  // from the babar sphericity code...
  double lambda1 = 0;
  double lambda2 = 0;
  
  if(E[0] < E[1]){
    lambda1 = E[0];
    lambda2 = E[1];
  }else{
    lambda1 = E[1];
    lambda2 = E[0];
  }
  
  double ST = 0;
  ST = 2*lambda1/( lambda1 + lambda2);
  return ST;
}



void Analysis::drawEtaPhi(Long64_t entry, int id)
{
  //This is a custom unction which takes mu, el, and jet eta and phi and plots them
  //return;
  
  //first we need to create some distinct Canvas names
  char tr[100];
  sprintf(tr, "top_%lld_%d", entry, id);
  
  //std::cout << "Pre_OPS_OR " << tr << std::endl;
  
  //Create a TGraph, add some Bogus Points (Tgraph doesn't like to be empty) and set the borders
  TGraph * tmp = new TGraph();
  tmp->SetPoint(1,-6,-4);
  tmp->SetPoint(2,6,4);
  tmp->GetXaxis()->SetLimits(-6,6);
  tmp->GetXaxis()->SetRangeUser(-6,6);
  tmp->GetXaxis()->SetTitle("eta");
  tmp->GetYaxis()->SetLimits(-3.2,3.2);
  tmp->GetYaxis()->SetRangeUser(-3.2,3.2);
  tmp->GetYaxis()->SetTitle("phi");
  
  //Create a new smallCanvas
  
  m_evs->cd(++counter);
  //TCanvas* bla = new TCanvas(tr,tr,600,600);
  //bla->SetTitle(tr);
  //bla->SetTitle("EtaPhi");
  //Draw the Graph
  tmp->Draw("AP*");
  
  TBox* b1 = new TBox(1.37, -3.15, 1.52, 3.15);
  TBox* b2 = new TBox(-1.37, -3.15, -1.52, 3.15);
  
  b1->Draw("SAME");
  b2->Draw("SAME");
  
  //We are using TMarkers with different shapes. Jets are triangles, muons are circles and electrons are suqares
  //Start with muons
  unsigned int size_mu;
  //Get the vectors eta and phi
  std::vector<float> mu_eta = m_sel->GetFinVectorByType< std::vector<float> >("MUO","_eta", &size_mu);
  std::vector<float> mu_phi = m_sel->GetFinVectorByType< std::vector<float> >("MUO","_phi");
  std::vector<float> mu_pT = m_sel->GetFinVectorByType< std::vector<float> >("MUO","_pt");
  //loops over the vector and create TMarkers at the eta and phi coordinates
  for (unsigned int a = 0; a<size_mu; a++)
    {
      TMarker * zaza = new TMarker(mu_eta.at(a), mu_phi.at(a), 24);
      zaza->SetMarkerSize(2); //One could set the size acording to pt.
      zaza->SetMarkerColor(4); //
    
      zaza->Draw(); //Plot the Marker
    
    
      char ctmp[100] = "";
      float pt = mu_pT.at(a);
      sprintf(ctmp, "%.1f", pt/1000.);
    
      TText * tmptext = new TText(mu_eta.at(a) +0.27, mu_phi.at(a) -0.1, std::string(ctmp).c_str() );
      tmptext->SetTextSize(0.03);
      tmptext->Draw();
    
    }
  
  //same for jet
  
  
  
  unsigned int size_jet;
  std::vector<float> jet_eta = m_sel->GetFinVectorByType< std::vector<float> >("JET","_eta",&size_jet);
  std::vector< float> jet_phi = m_sel->GetFinVectorByType<  std::vector<float> >("JET","_phi");
  std::vector< float> jet_pt = m_sel->GetFinVectorByType<  std::vector<float> >("JET","_pt");
  for (unsigned int a = 0; a<size_jet; a++)
    {
      TMarker * zaza = new TMarker(jet_eta.at(a), jet_phi.at(a), 23);
      zaza->SetMarkerSize(2);
      zaza->SetMarkerColor(2);
    
      zaza->Draw();
    
      char ctmp[100] = "";
      float pt = jet_pt.at(a);
      sprintf(ctmp, "%.1f", pt/1000.);
    
      TText * tmptext = new TText(jet_eta.at(a) - 0.2, jet_phi.at(a) +0.2, std::string(ctmp).c_str() );
      tmptext->SetTextSize(0.03);
      tmptext->Draw();
    
    
    }
  
  //and electrons
  
  unsigned int size_el;
  std::vector< float> el_eta = m_sel->GetFinVectorByType< std::vector<float> >("ELE","_eta", &size_el);
  std::vector< float> el_phi = m_sel->GetFinVectorByType<  std::vector<float> >("ELE","_phi");
  std::vector< float> el_pT = m_sel->GetFinVectorByType<  std::vector<float> >("ELE","_pt");
  for (unsigned int a = 0; a<size_el; a++)
    {
      TMarker * zaza = new TMarker(el_eta.at(a), el_phi.at(a), 25);
      zaza->SetMarkerSize(2);
      zaza->SetMarkerColor(3);
    
      zaza->Draw();
    
      char ctmp[100] = "";
      float pt = el_pT.at(a);
      sprintf(ctmp, "%.1f", pt/1000.);
    
      TText * tmptext = new TText(el_eta.at(a) +0.27, el_phi.at(a) -0.1, std::string(ctmp).c_str() );
      tmptext->SetTextSize(0.03);
      tmptext->Draw();
    
    }
  
  
  
}


//Physics functions

Bool_t Analysis::isBarcodePdgId( Int_t p_barcode, Int_t p_pdgId ){
  
  unsigned int mc_n = m_sel->GetScalarVal<Int_t>("mc_n");
  std::vector< int > mc_barcode = m_sel->GetRawVector< std::vector<int> >("mc_barcode");
  std::vector< int > mc_pdgId = m_sel->GetRawVector< std::vector<int> >("mc_pdgId");
  for (unsigned int j = 0 ; j < mc_n ; j++) {
    if (mc_barcode[j] == p_barcode){
      if (mc_pdgId[j] == p_pdgId){
        return true;
      }
    }
  }
  return false;
}


Float_t Analysis::findMCpT( Int_t p_barcode){
  unsigned int mc_n = m_sel->GetScalarVal<Int_t>("mc_n");
  std::vector< int > mc_barcode = m_sel->GetRawVector< std::vector<int> >("mc_barcode");
  std::vector< float > mc_pt = m_sel->GetRawVector< std::vector<float> >("mc_pt");
  for (unsigned int j = 0 ; j < mc_n ; j++) {
    if (mc_barcode[j] == p_barcode){
      return mc_pt[j];
    }
  }
  return -9999.;
}

Float_t Analysis::findMCeta( Int_t p_barcode){
  unsigned int mc_n = m_sel->GetScalarVal<Int_t>("mc_n");
  std::vector< int > mc_barcode = m_sel->GetRawVector< std::vector<int> >("mc_barcode");
  std::vector< float > mc_eta = m_sel->GetRawVector< std::vector<float> >("mc_eta");  
  for (unsigned int j = 0 ; j < mc_n ; j++) {
    if (mc_barcode[j] == p_barcode){
      return mc_eta[j];
    }
  }
  return -9999.;
}

Float_t Analysis::findMCphi( Int_t p_barcode){
  unsigned int mc_n = m_sel->GetScalarVal<Int_t>("mc_n");
  std::vector< int > mc_barcode = m_sel->GetRawVector< std::vector<int> >("mc_barcode");
  std::vector< float > mc_phi = m_sel->GetRawVector< std::vector<float> >("mc_phi");  
  for (unsigned int j = 0 ; j < mc_n ; j++) {
    if (mc_barcode[j] == p_barcode){
      return mc_phi[j];
    }
  }
  return -9999.;
}


Float_t Analysis::DeltaR(Float_t eta1, Float_t phi1, Float_t eta2, Float_t phi2) {
  
  Float_t dR = -999.0;
  Float_t dR2 = (eta1-eta2)*(eta1-eta2) + (moduloPiPi(phi1-phi2)*moduloPiPi(phi1-phi2));
  dR = TMath::Sqrt(dR2);  
  return dR;
  
}

Double_t Analysis::moduloPiPi(Double_t phi) {
  // Return input phi between -Pi and Pi  
  Double_t phiout = phi;
  
  if (phiout<(-1)*TMath::Pi())
    phiout = phiout+TMath::TwoPi();
  else if (phiout>TMath::Pi())
    phiout = phiout-TMath::TwoPi();
  
  return phiout;
  
}

