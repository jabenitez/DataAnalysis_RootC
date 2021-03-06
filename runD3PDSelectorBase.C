//author: Cyril Topfel, cyril.topfel@lhep.unibe.ch
//skype (for instant support): ctopfel (Please mention D3PD reader in the contact addition window)
//Feel free to contact me if you have questions
#include <TChain.h>
#include <TROOT.h>
#include <TStopwatch.h>

void runD3PDSelector() {

  TChain *c = new TChain("llp");
  gROOT->SetStyle("Plain");


    bool USEPROOF = false; //set to true if you want to use PROOF
  //  bool USEPROOF = true; //set to true if you want to use PROOF

//put your files here. You may also use * (wildcard) to add multiple files
//c->Add("/tmp/ctopfel/debug/user10.FlorianAhles.SUSYD3PD.data10_7TeV.00152221.physics_L1Calo.recon.ESD.f238.280410.1/user10.FlorianAhles.SUSYD3PD.data10_7TeV.00152221.physics_L1Calo.recon.ESD.f238.280410.1.D3PD._00003.root");
//  c->Add("/Users/benitez/work/d3pdreader/tuples_jul/lepto*5200*root");
//  c->Add("/Users/benitez/work/d3pdreader/tuples_jul/lepto*s2*00*root");
//  c->Add("/Users/benitez/work/LS/D3PDs/*Ms500_Mn600_Mg10000*root");
  c->Add("/Users/benitez/work/LS/D3PDs/*MYFILENAME*root");

  if (USEPROOF) 
  {
    p = TProof::Open("");
    c->SetProof();
  }
  else 
  {

  }

  c->Process("D3PDSelector.C+");

}
