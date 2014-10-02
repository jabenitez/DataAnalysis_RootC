#ifndef Analysis_h
#define Analysis_h

#include <iostream>
#include <TH1F.h>
#include <TH2F.h>


class Analysis
{
  
 public:
  //Constructors
  Analysis(D3PDSelector * inst); //Constructor with D3PD Selector Object
  
  //Destructor
  virtual ~Analysis() {}; //default destructor
  
  //Analysis Methods
  virtual unsigned int Pre_OPS_OR(Long64_t entry);
  virtual unsigned int Between_OPS_and_OR(Long64_t entry);
  virtual void doAnalysis(Long64_t entry);
  virtual void finalizeEvent(Long64_t entry);
  
  //helper method
  virtual void drawEtaPhi(Long64_t entry, int id);
  
  
  
  //static postAnalysis method (Drawing/Saving)
  static void postAnalysis (TSelectorList* fOutput);
  
  //ST calc
  double CalcTranSphericity(double sumpx2,double sumpy2,double sumpxpy);
  Bool_t isBarcodePdgId( int p_barcode, int p_pdgId );
  Bool_t hasTruthBarcode( std::vector<int> myvector, int element );
  int hasTruthDuplicates( std::vector<int> myvector, int element );
  Float_t DeltaR(Float_t eta1, Float_t phi1, Float_t eta2, Float_t phi2);
  Double_t moduloPiPi(Double_t phi);
  Float_t findMCpT( Int_t p_barcode);
  Float_t findMCmass( Int_t p_barcode);
  Float_t findMCeta( Int_t p_barcode);
  Float_t findMCphi( Int_t p_barcode);
  
  //Example TH1F
  TH1D* meff;
  TH1D *hNJets;
  TH1D *hNOkJets;
  
  TH1D * hCutFlow;
  TH1D * hCutFlowIndividual;
	
  TH1D * hJet1pT;
  TH1D * hJet2pT;
  TH1D * hJet3pT;
  TH1D * hJet4pT;
  TH1D * hJet1Eta;
  TH1D * hJet2Eta;
  TH1D * hJet3Eta;
  TH1D * hJet4Eta;
  TH1D * hJet1Phi;
  TH1D * hJet2Phi;
  TH1D * hJet3Phi;
  TH1D * hJet4Phi;
  TH1D * hJet1Mass;
  TH1D * hJet2Mass;
  TH1D * hJet3Mass;
  TH1D * hJet4Mass;  
  TH1D * hJet1FlavorWeight;
  TH1D * hJet2FlavorWeight;
  TH1D * hJet3FlavorWeight;
  TH1D * hJet4FlavorWeight;
  TH1D * hJet1dRTruthB;
  TH1D * hJet2dRTruthB;
  TH1D * hJet3dRTruthB;
  TH1D * hJet4dRTruthB;
  TH1D * hdRJet1Jet2;
  TH1D * hdRJet1Jet3;
  TH1D * hdRJet1Jet4;
  TH1D * hdRJet2Jet3;
  TH1D * hdRJet2Jet4;
  TH1D * hdRJet3Jet4;

  TH1D *hHtJet1Jet2;
  TH1D *hHtAllJets;
  TH1D *hHtAllObjects;
  TH1D *hHJet1Jet2;
  TH1D *hHAllJets;
  TH1D *hHAllObjects;
  TH1D *hHtLepton1Lepton2;
  TH1D *hHtAllLeptons;
  TH1D *hHtAllJetsAllLeptons;

  // Electrons
  TH1D *hRecoElectrons;
  TH1D *hRecoOkElectrons;
  TH1D *hRecoEleInvMass;
  
  TH1D *hElectron1pT;
  TH1D *hElectron2pT;
  TH1D *hElectron3pT;
  
  TH1D *hElectron1Eta;
  TH1D *hElectron2Eta;
  TH1D *hElectron3Eta;
  
  TH1D *hElectron1Phi;
  TH1D *hElectron2Phi;
  TH1D *hElectron3Phi;
  
  TH1D *hElectron1Mass;
  TH1D *hElectron2Mass;
  TH1D *hElectron3Mass;
	
  TH1D *hdRElectron1Electron2;
  TH1D *hdRElectron1Electron3;
  TH1D *hdRElectron2Electron3;
  TH1D *hdRElectron1Muon1;
  TH1D *hdRElectron1Muon2;
  TH1D *hdRElectron1Tau1;
  TH1D *hdRElectron1Tau2;
  TH1D *hdRElectron2Muon1;
  TH1D *hdRElectron2Muon2;
  TH1D *hdRElectron2Tau1;
  TH1D *hdRElectron2Tau2;
  TH1D *hElectronTruthType;
  TH1D *hElectronBSTruthType;
  
  //Muons
  TH1D *hRecoMuons;
  TH1D *hRecoOkMuons;	
  
  TH1D *hMuon1pT;
  TH1D *hMuon2pT;
  TH1D *hMuon3pT;
  
  TH1D *hMuon1Eta;
  TH1D *hMuon2Eta;
  TH1D *hMuon3Eta;
  
  TH1D *hMuon1Phi;
  TH1D *hMuon2Phi;
  TH1D *hMuon3Phi;  
	
  TH1D *hMuon1Mass;
  TH1D *hMuon2Mass;
  TH1D *hMuon3Mass;
	
  TH1D *hMuon1Beta;
  TH1D *hMuon2Beta;
  TH1D *hMuon3Beta;
  
  TH1D *hdRMuon1Muon2;
  TH1D *hdRMuon1Muon3;
  TH1D *hdRMuon2Muon3;
  TH1D *hdRMuon1Tau1;
  TH1D *hdRMuon1Tau2;
  TH1D *hdRMuon2Tau1;
  TH1D *hdRMuon2Tau2;
  TH1D *hMuonTruthType;
  TH1D *hMuonBSTruthType;
  
  //Slow Muons Muons
  TH1D *hRecoMuLowBetaMuons;
  TH1D *hRecoOkMuLowBetaMuons;	
  
  TH1D *hMuLowBetaMuon1pT;
  TH1D *hMuLowBetaMuon2pT;
  TH1D *hMuLowBetaMuon3pT;
  
  TH1D *hMuLowBetaMuon1EtaAbs;
  TH1D *hMuLowBetaMuon1Eta;
  TH1D *hMuLowBetaMuon2Eta;
  TH1D *hMuLowBetaMuon3Eta;
  
  TH1D *hMuLowBetaMuon1Phi;
  TH1D *hMuLowBetaMuon2Phi;
  TH1D *hMuLowBetaMuon3Phi;
  
  TH1D *hMuLowBetaMuon1Mass;
  TH1D *hMuLowBetaMuon2Mass;
  TH1D *hMuLowBetaMuon3Mass;
	
  TH1D *hMuLowBetaMuon1MassOrig;
  TH1D *hMuLowBetaMuon2MassOrig;
  TH1D *hMuLowBetaMuon3MassOrig;
	
  TH1D *hMuLowBetaMuon1Beta;
  TH1D *hMuLowBetaMuon2Beta;
  TH1D *hMuLowBetaMuon3Beta;

  TH1D *hMuLowBetaMuon1TruthBeta;
  TH1D *hMuLowBetaMuon1TileBeta;
  TH1D *hMuLowBetaMuon1DeltaBeta;
  TH1D *hMuLowBetaMuon1DeltaTileBeta;

  TH1D *hdRMuLowBetaMuon1Muon1;
  TH1D *hdRMuLowBetaMuon1Muon2;
  TH1D *hdRMuLowBetaMuon1Muon3;
  TH1D *hdRMuLowBetaMuon1Electron1;
  TH1D *hdRMuLowBetaMuon1Electron2;
  TH1D *hdRMuLowBetaMuon1Electron3;
  TH1D *hdRMuLowBetaMuon1Jet1;
  TH1D *hdRMuLowBetaMuon1Jet2;
  TH1D *hdRMuLowBetaMuon1Jet3;
  TH1D *hdRMuLowBetaMuon1MuLowBetaMuon2;
  TH1D *hdRMuLowBetaMuon1MuLowBetaMuon3;
  TH1D *hdRMuLowBetaMuon2MuLowBetaMuon3;
  TH1D *hdRMuLowBetaMuon2Muon1;
  TH1D *hdRMuLowBetaMuon2Muon2;
  TH1D *hdRMuLowBetaMuon2Muon3;
  TH1D *hdRMuLowBetaMuon2Electron1;
  TH1D *hdRMuLowBetaMuon2Electron2;
  TH1D *hdRMuLowBetaMuon2Electron3;
  TH1D *hdRMuLowBetaMuon2Jet1;
  TH1D *hdRMuLowBetaMuon2Jet2;
  TH1D *hdRMuLowBetaMuon2Jet3;
  TH1D *hdRMuLowBetaMuon3Muon1;
  TH1D *hdRMuLowBetaMuon3Muon2;
  TH1D *hdRMuLowBetaMuon3Muon3;
  TH1D *hdRMuLowBetaMuon3Electron1;
  TH1D *hdRMuLowBetaMuon3Electron2;
  TH1D *hdRMuLowBetaMuon3Electron3;
  TH1D *hdRMuLowBetaMuon3Jet1;
  TH1D *hdRMuLowBetaMuon3Jet2;
  TH1D *hdRMuLowBetaMuon3Jet3;
	
  TH1D *hMuLowBetaMuonTruthType;
  TH1D *hMuLowBetaMuonBSTruthType;
	
	
  TH1D *hRecoMuGirlMuons;
  TH1D *hRecoOkMuGirlMuons;	
	
  TH1D *hMuGirlMuon1pT;
  TH1D *hMuGirlMuon2pT;
  TH1D *hMuGirlMuon3pT;
  
  TH1D *hMuGirlMuon1Eta;
  TH1D *hMuGirlMuon2Eta;
  TH1D *hMuGirlMuon3Eta;
  
  TH1D *hMuGirlMuon1Phi;
  TH1D *hMuGirlMuon2Phi;
  TH1D *hMuGirlMuon3Phi;
  
  TH1D *hMuGirlMuon1Mass;
  TH1D *hMuGirlMuon2Mass;
  TH1D *hMuGirlMuon3Mass;
	
  TH1D *hMuGirlMuon1MassBeta;
  TH1D *hMuGirlMuon2MassBeta;
  TH1D *hMuGirlMuon3MassBeta;
	
  TH1D *hMuGirlMuon1Beta;
  TH1D *hMuGirlMuon2Beta;
  TH1D *hMuGirlMuon3Beta;
	
  TH1D *hMuGirlMuonTruthType;
  TH1D *hMuGirlMuonBSTruthType;
  
  //Taus
  TH1D *hRecoTaus;
  TH1D *hRecoOkTaus;
  
  TH1D *hTau1pT;
  TH1D *hTau2pT;
  TH1D *hTau3pT;
  
  TH1D *hTau1Eta;
  TH1D *hTau2Eta;
  TH1D *hTau3Eta;
  
  TH1D *hTau1Phi;
  TH1D *hTau2Phi;
  TH1D *hTau3Phi;
  
  TH1D *hTau1Mass;
  TH1D *hTau2Mass;
  TH1D *hTau3Mass;
	
  TH1D *hdRTau1Tau2;
  TH1D *hdRTau1Tau3;
  TH1D *hdRTau2Tau3;
	
  TH1D *hRecoOkLeptons;
	
  // MET
  TH1D *hMET;
  TH1D *hMETPhi;
  
  TH1D *hNHiggs;
  // Reco Masses of Higgs and Z from b-partons
  TH1D *lshMassHiggs;
  TH1D *lshMassZ;
  
  // Parton b's PT spectrum from Higgs1 and Higgs2
  TH1D *lshPTb1;
  TH1D *lshPTb2;
  TH1D *lshPTb3;
  TH1D *lshPTb4;
	
  // PT of matched jets
  TH1D *lshJet1Match;
  TH1D *lshJet2Match;
	
  // Matched and unmatched reco Higgs masses from jets
  TH1D *lshJetsMass34;
  TH1D *lshJetsNewMass34;
  TH1D *lshRecoJJInvMass;
  TH1D *lshRecoBBInvMass;
  TH1D *lshRecoEleInvMass;
	
  TH1D *hdRMinDilep;
  TH1D *hdRMinDilepInvMass;
  TH2D *h2dInvMassMindRDilep;
  TH1D *hdRDilep;
  TH1D *hDilepInvMass;
  TH2D *h2dInvMassdRDilep;
	
  TH1D *hNeutralinoMass;
  TH1D *hNeutralino2Mass;
  TH1D *hSquarkMass;
  TH1D *hGluinoMass;
	
  TH1D *hNeutralinoTransverseMass;
  TH1D *hNeutralino2TransverseMass;
  TH1D *hSquarkTransverseMass;
  TH1D *hGluinoTransverseMass;
	
  TH1D * hdRMinLeptons;
  TH1D * hMassCloseZLeptons;
  TH1D * hMassCloseZMuons;
  TH1D * hMassMindRLeptons;
  TH2D * h2dMassMindRLeptons;
  TH1D * hdRLeptons;
  TH1D * hMassdRLeptons;
  TH2D * h2dMassdRLeptons;
  //bTagInfo
	
  TH1D * lshJetFlavor1;
  TH1D * lshJetFlavor2;
  TH1D * lshJetFlavor3;
  TH1D * lshJetFlavor4;
  TH1D * lshJetFlavor5;
  TH1D * lshJetFlavor6;
  TH1D * lshJetFlavorMatch1;
  TH1D * lshJetFlavorMatch2;
	
  TH1D * lshJetWeight1;
  TH1D * lshJetWeight2;
  TH1D * lshJetWeight3;
  TH1D * lshJetWeight4;
  TH1D * lshJetWeight5;
  TH1D * lshJetWeight6;
  TH1D * lshJetWeightMatch1;
  TH1D * lshJetWeightMatch2;
	
	
  // Parton Level quantities
	
  TH1D *lshMinus;
  TH1D *lshEMinus;
  TH1D *lshMuMinus;
  TH1D *lshTauMinus;
	
  TH1D *lshPlus;
  TH1D *lshEPlus;
  TH1D *lshMuPlus;
  TH1D *lshTauPlus;
	
  TH1D *lshLepton;
	
	
	
  // sleptons masses
  TH1D *lsh4;
  TH1D *lsh44;
  TH1D *lsh5;
  TH1D *lsh55;
  TH1D *lsh6;
  TH1D *lsh66;
	
  TH1D *lshMassChi0e;
  TH1D *lshMassChi0m;
  TH1D *lshMassChi0t;
  TH1D *lshChi02Mass;
  TH1D *lshChi01Mass;
  TH1D *lshChi03Mass;
	
  //TRIGGER STUDIES

  TH1D *h_EF_mu22_medium;
  TH1D *h_EF_mu18_medium;
  TH1D *h_EF_2mu10_loose;
  TH1D *h_EF_mu40_slow;
  TH1D *h_EF_mu40_slow_medium;
  TH1D *h_EF_mu40_slow_outOfTime_medium;
  TH1D *h_EF_mu40_slow_empty;
  TH1D *h_EF_mu40_slow_outOfTime;
  TH1D *h_trigger_muOR;
  TH1D *h_trigger_muOR_BC;
  
  TH1D *hn_sel_pt;
  TH1D *hn_sel_L1_pt;
  TH1D *hn_sel_L2_pt;
  TH1D *hn_sel_EF_pt;
  TH1D *hn_ratio_pt_L1SEL;
  TH1D *hn_ratio_pt_L1L2;
  TH1D *hn_ratio_pt_L2EF;
  TH1D *hn_slow_sel_pt;
  TH1D *hn_slow_sel_L1_pt;
  TH1D *hn_slow_sel_L2_pt;
  TH1D *hn_slow_sel_EF_pt;
  TH1D *hn_slow_ratio_pt_L1SEL;
  TH1D *hn_slow_ratio_pt_L1L2;
  TH1D *hn_slow_ratio_pt_L2EF;
	
  TH1D *hn_sel_eta;
  TH1D *hn_sel_L1_eta;
  TH1D *hn_sel_L2_eta;
  TH1D *hn_sel_EF_eta;
  TH1D *hn_ratio_eta_L1SEL;
  TH1D *hn_ratio_eta_L1L2;
  TH1D *hn_ratio_eta_L2EF;
  TH1D *hn_slow_sel_eta;
  TH1D *hn_slow_sel_L1_eta;
  TH1D *hn_slow_sel_L2_eta;
  TH1D *hn_slow_sel_EF_eta;
  TH1D *hn_slow_ratio_eta_L1SEL;
  TH1D *hn_slow_ratio_eta_L1L2;
  TH1D *hn_slow_ratio_eta_L2EF;
	
  TH1D *hn_sel_phi;
  TH1D *hn_sel_L1_phi;
  TH1D *hn_sel_L2_phi;
  TH1D *hn_sel_EF_phi;
  TH1D *hn_ratio_phi_L1SEL;
  TH1D *hn_ratio_phi_L1L2;
  TH1D *hn_ratio_phi_L2EF;
  TH1D *hn_slow_sel_phi;
  TH1D *hn_slow_sel_L1_phi;
  TH1D *hn_slow_sel_L2_phi;
  TH1D *hn_slow_sel_EF_phi;
  TH1D *hn_slow_ratio_phi_L1SEL;
  TH1D *hn_slow_ratio_phi_L1L2;
  TH1D *hn_slow_ratio_phi_L2EF;
  
  TH2D *hn_sel_eta_phi;
  TH2D *hn_sel_L1_eta_phi;
  TH2D *hn_sel_L2_eta_phi;
  TH2D *hn_sel_EF_eta_phi;
  TH2D *hn_ratio_eta_phi_L1SEL;
  TH2D *hn_ratio_eta_phi_L1L2;
  TH2D *hn_ratio_eta_phi_L2EF;
  TH2D *hn_slow_sel_eta_phi;
  TH2D *hn_slow_sel_L1_eta_phi;
  TH2D *hn_slow_sel_L2_eta_phi;
  TH2D *hn_slow_sel_EF_eta_phi;
  TH2D *hn_slow_ratio_eta_phi_L1SEL;
  TH2D *hn_slow_ratio_eta_phi_L1L2;
  TH2D *hn_slow_ratio_eta_phi_L2EF;  

  TH2D *hHtJet1Jet2_HtLepton1Lepton2;
  TH2D *hHtJet1Jet2_Met;
  TH2D *hHtJet1Jet2_LowBetaMuonBeta;
  TH2D *hHtJet1Jet2_NJets;
  TH2D *hHtJet1Jet2_NLeptons;
  TH2D *hHtJet1Jet2_LowBetaMuon1Eta;
  TH2D *hHtJet1Jet2_LowBetaMuon1pT;
  TH2D *hHtJet1Jet2_LowBetaMuon1MassBeta;
  TH2D *hHtJet1Jet2_GluinoMass;
  TH2D *hHtLepton1Lepton2_Met;
  TH2D *hHtLepton1Lepton2_LowBetaMuonBeta;
  TH2D *hHtLepton1Lepton2_NJets;
  TH2D *hHtLepton1Lepton2_NLeptons;
  TH2D *hHtLepton1Lepton2_LowBetaMuon1Eta;
  TH2D *hHtLepton1Lepton2_LowBetaMuon1pT;
  TH2D *hHtLepton1Lepton2_LowBetaMuon1MassBeta;
  TH2D *hHtLepton1Lepton2_GluinoMass;
  TH2D *hHtAllJets_HtAllLeptons;
  TH2D *hHtAllJets_Met;
  TH2D *hHtAllJets_LowBetaMuonBeta;
  TH2D *hHtAllJets_NJets;
  TH2D *hHtAllJets_NLeptons;
  TH2D *hHtAllJets_LowBetaMuon1Eta;
  TH2D *hHtAllJets_LowBetaMuon1pT;
  TH2D *hHtAllJets_LowBetaMuon1MassBeta;
  TH2D *hHtAllJets_GluinoMass;
  TH2D *hHtAllLeptons_Met;
  TH2D *hHtAllLeptons_LowBetaMuonBeta;
  TH2D *hHtAllLeptons_NJets;
  TH2D *hHtAllLeptons_NLeptons;
  TH2D *hHtAllLeptons_LowBetaMuon1Eta;
  TH2D *hHtAllLeptons_LowBetaMuon1pT;
  TH2D *hHtAllLeptons_LowBetaMuon1MassBeta;
  TH2D *hHtAllLeptons_GluinoMass;  
  TH2D *hHtAllJetsAllLeptons_Met;
  TH2D *hHtAllJetsAllLeptons_LowBetaMuonBeta;
  TH2D *hHtAllJetsAllLeptons_NJets;
  TH2D *hHtAllJetsAllLeptons_NLeptons;
  TH2D *hHtAllJetsAllLeptons_LowBetaMuon1Eta;
  TH2D *hHtAllJetsAllLeptons_LowBetaMuon1pT;
  TH2D *hHtAllJetsAllLeptons_LowBetaMuon1MassBeta;
  TH2D *hHtAllJetsAllLeptons_GluinoMass;
  TH2D * hJet1Pt_Met;
  TH2D * hJet1Pt_LowBetaMuonBeta;
  TH2D * hJet1Pt_NJets;
  TH2D * hJet1Pt_NLeptons;
  TH2D * hJet1Pt_LowBetaMuon1Eta;
  TH2D * hJet1Pt_LowBetaMuon1pT;
  TH2D * hJet1Pt_LowBetaMuon1MassBeta;
  TH2D * hJet1Pt_GluinoMass;
  TH2D * hMet_LowBetaMuonBeta;
  TH2D * hMet_NJets;
  TH2D * hMet_NLeptons;
  TH2D * hMet_LowBetaMuon1Eta;
  TH2D * hMet_LowBetaMuon1pT;
  TH2D * hMet_LowBetaMuon1MassBeta;
  TH2D * hMet_GluinoMass;
  TH2D * hLowBetaMuonBeta_NJets;
  TH2D * hLowBetaMuonBeta_NLeptons;
  TH2D * hLowBetaMuonBeta_LowBetaMuon1Eta;
  TH2D * hLowBetaMuonBeta_LowBetaMuon1pT;
  TH2D * hLowBetaMuonBeta_LowBetaMuon1MassBeta;
  TH2D * hLowBetaMuonBeta_GluinoMass;
  TH2D * hNJets_NLeptons;
  TH2D * hNJets_LowBetaMuon1Eta;
  TH2D * hNJets_LowBetaMuon1pT;
  TH2D * hNJets_LowBetaMuon1MassBeta;
  TH2D * hNJets_GluinoMass;
  TH2D * hNLeptons_LowBetaMuon1Eta;
  TH2D * hNLeptons_LowBetaMuon1pT;
  TH2D * hNLeptons_LowBetaMuon1MassBeta;
  TH2D * hNLeptons_GluinoMass;
  TH2D * hLowBetaMuon1Eta_LowBetaMuon1pT;
  TH2D * hLowBetaMuon1Eta_LowBetaMuon1MassBeta;
  TH2D * hLowBetaMuon1Eta_GluinoMass;
  TH2D * hLowBetaMuon1pT_LowBetaMuon1MassBeta;
  TH2D * hLowBetaMuon1pT_GluinoMass;
  TH2D * hLowBetaMuon1MassBeta_GluinoMass;
  TH2D *hMassCloseZMuons_HtAllJetsAllLeptons;
  TH2D *hMassCloseZMuons_HtAllJets;
  TH2D *hMassCloseZMuons_HtAllLeptons;
  TH2D *hMassCloseZMuons_Met;
  TH2D *hMassCloseZMuons_LowBetaMuonBeta;
  TH2D *hMassCloseZMuons_NJets;
  TH2D *hMassCloseZMuons_NLeptons;
  TH2D *hMassCloseZMuons_LowBetaMuon1Eta;
  TH2D *hMassCloseZMuons_LowBetaMuon1pT;
  TH2D *hMassCloseZMuons_LowBetaMuon1MassBeta;
  TH2D *hMassCloseZMuons_GluinoMass;

  TH2D *hRequireNeutralino_HtAllJetsAllLeptons;
  TH2D *hRequireNeutralino_HtAllJets;
  TH2D *hRequireNeutralino_HtAllLeptons;
  TH2D *hRequireNeutralino_Met;
  TH2D *hRequireNeutralino_LowBetaMuonBeta;
  TH2D *hRequireNeutralino_NJets;
  TH2D *hRequireNeutralino_NLeptons;
  TH2D *hRequireNeutralino_LowBetaMuon1Eta;
  TH2D *hRequireNeutralino_LowBetaMuon1pT;
  TH2D *hRequireNeutralino_LowBetaMuon1MassBeta;
  TH2D *hRequireNeutralino_GluinoMass;
  TH2D *hRequireNeutralino_MassCloseZMuons;

  TH1D *BarCodeHistoMuon;
	
 protected:
  D3PDSelector * m_sel;
  float m_cutflow[15];
  string m_cutname[15];
  //see https://twiki.cern.ch/twiki/bin/view/AtlasProtected/SUSYOneLeptonCuts2010
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
  
  float m_meff; //value used in cuts do stoe meeff, so we don't need to calculate again in Analysis
  TLorentzVector m_lep;
  float m_lep_charge;
  
  TCanvas * m_evs;
  int counter;
  Int_t debug;
  Int_t ControlTTbar;
  Int_t ControlZmumu;
  Int_t RemoveZWindow;
  Int_t ReqTrigger;
  int bbcounter;
  Int_t eventsProcessed;
  Int_t eventsPassed;
  Int_t nOneHiggsPerEvent;
  Int_t nTwoHiggsPerEvent;  
  map<string,int > triggerPassed;
  map<string,int > triggerPassed_1Higgs;
  map<string,int > triggerPassed_2Higgs;
  float JETPT1_CUT;
  float JETPT2_CUT;
  float JETPT3_CUT;
  float JETPT4_CUT;
  float NJETMIN;
  float NELEMIN;
  float NMUOMIN;
  float NMUGMIN;
  float NTAUMIN;
  float NLEPMIN;
  float NSLOWMUOMIN;
  float ETMISS_CUT;
  float HTALLJETSALLLEPTONS_CUT;
  float HTALLJETS_CUT;
  float HTALLLEPTONS_CUT;
  float LOWBETAMUON1ETA_CUT;
  float ZMASSMIN_CUT;
  float ZMASSMAX_CUT;
  Int_t REALDATA;
  Int_t REQUIRE_NEUTRALINO;
};

#endif

