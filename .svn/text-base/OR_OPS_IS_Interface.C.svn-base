#include <iostream>
#include "OR_OPS_IS_Interface.h"

#define KEEP_BOTH 0
#define KEEP_FIRST 2
#define KEEP_SECOND 1
#define REJECT_FIRST 1
#define REJECT_SECOND 2
#define REJECT_BOTH 3

#define KEEP 0
#define REJECT 1

#include "MyObjectPreSelection.C"
#include "MyOverlapRemoval.C"


OverlapRemoval::OverlapRemoval(D3PDSelector* inst)
{
  m_selector = inst;
}


//--VS Jet
unsigned int OverlapRemoval::ElectronJet_OR(ObjAccess* /*ele*/, ObjAccess* /*jet*/, unsigned int /*iteration*/) //avoid warnings,
{
  return KEEP_BOTH;
}

unsigned int OverlapRemoval::MuonJet_OR(ObjAccess* /*muo*/, ObjAccess* /*jet*/, unsigned int /*iteration*/)
{
  return KEEP_BOTH;
}

unsigned int OverlapRemoval::PhotonJet_OR(ObjAccess* /*pho*/, ObjAccess* /*jet*/, unsigned int /*iteration*/)
{
  return KEEP_BOTH;
}

unsigned int OverlapRemoval::TauJet_OR(ObjAccess* /*tau*/, ObjAccess* /*jet*/, unsigned int /*iteration*/)
{
  return KEEP_BOTH;
}



//--VS Electron
unsigned int OverlapRemoval::MuonElectron_OR(ObjAccess* /*muo*/, ObjAccess* /*ele*/,  unsigned int /*iteration*/)
{
  return KEEP_BOTH;
}

unsigned int OverlapRemoval::PhotonElectron_OR(ObjAccess* /*pho*/, ObjAccess* /*ele*/,  unsigned int /*iteration*/)
{
  return KEEP_BOTH;
}

unsigned int OverlapRemoval::TauElectron_OR(ObjAccess* /*tau*/, ObjAccess* /*ele*/,  unsigned int /*iteration*/)
{
  return KEEP_BOTH;
}



//--VS Muon
unsigned int OverlapRemoval::PhotonMuon_OR(ObjAccess* /*pho*/, ObjAccess* /*muo*/,  unsigned int /*iteration*/)
{
  return KEEP_BOTH;
}
unsigned int OverlapRemoval::TauMuon_OR(ObjAccess* /*tau*/, ObjAccess* /*muo*/,  unsigned int /*iteration*/)
{
  return KEEP_BOTH;
}


//--VS Photon
unsigned int OverlapRemoval::TauPhoton_OR(ObjAccess* /*tau*/, ObjAccess* /*pho*/,  unsigned int /*iteration*/)
{
  return KEEP_BOTH;
}




void OverlapRemoval::do_Electron_Jet(unsigned int iteration)
{
  m_selector->AddDualSelector("ELE","JET",iteration);
}
void OverlapRemoval::do_Muon_Jet(unsigned int  iteration)
{
  m_selector->AddDualSelector("MUO","JET",iteration);
}
void OverlapRemoval::do_Photon_Jet(unsigned int  iteration)
{
  m_selector->AddDualSelector("PHO","JET",iteration);
}
void OverlapRemoval::do_Tau_Jet(unsigned int  iteration)
{
  m_selector->AddDualSelector("TAU","JET",iteration);
}


void OverlapRemoval::do_Muon_Electron(unsigned int  iteration)
{
  m_selector->AddDualSelector("MUO","ELE",iteration);
}
void OverlapRemoval::do_Photon_Electron(unsigned int  iteration)
{
  m_selector->AddDualSelector("PHO","ELE",iteration);
}
void OverlapRemoval::do_Tau_Electron(unsigned int  iteration)
{
  m_selector->AddDualSelector("TAU","ELE",iteration);
}


void OverlapRemoval::do_Photon_Muon(unsigned int  iteration)
{
  m_selector->AddDualSelector("PHO","MUO",iteration);
}
void OverlapRemoval::do_Tau_Muon(unsigned int  iteration)
{
  m_selector->AddDualSelector("TAU","MUO",iteration);
}

void OverlapRemoval::do_Tau_Photon(unsigned int  iteration)
{
  m_selector->AddDualSelector("TAU","PHO",iteration);
}



void OverlapRemoval::overlapRemovalOrder()
{
  //std::cout << "overlapRemovalOrder" << std::endl;
  do_Electron_Jet(1);
  do_Muon_Jet(1);
  do_Electron_Jet(2);
}




ObjectPreSelection::ObjectPreSelection(D3PDSelector* inst)
{
  m_selector = inst;
}


unsigned int ObjectPreSelection::Electron_OPS(ObjAccess* /*ele*/, unsigned int  /*iteration*/)
{
  //std::cout << "EleOPS " << electron_id << ", it " <<  iteration << std::endl;
  return KEEP;
}

unsigned int ObjectPreSelection::Muon_OPS(ObjAccess* /*muo*/, unsigned int  /*iteration*/)
{
  //std::cout << "MuoOPS " << muon_id << ", it " <<  iteration << std::endl;
  return KEEP;
}

unsigned int ObjectPreSelection::LowBetaMuon_OPS(ObjAccess* /*muo*/, unsigned int  /*iteration*/)
{
  //std::cout << "LowBetaMuoOPS " << lowbeta_id << ", it " <<  iteration << std::endl;
  return KEEP;
}

unsigned int ObjectPreSelection::MuGirlMuon_OPS(ObjAccess* /*muo*/, unsigned int  /*iteration*/)
{
  //std::cout << "LowBetaMuoOPS " << mugirl_id << ", it " <<  iteration << std::endl;
  return KEEP;
}

unsigned int ObjectPreSelection::Photon_OPS(ObjAccess* /*pho*/, unsigned int  /*iteration*/)
{
  //std::cout << "PhoOPS " << photon_id << ", it " <<  iteration << std::endl;
  return KEEP;
}

unsigned int ObjectPreSelection::Jet_OPS(ObjAccess* /*jet*/, unsigned int  /*iteration*/)
{
  //std::cout << "JetOPS " << jet_id << ", it " <<  iteration << std::endl;
  return KEEP;
}

unsigned int ObjectPreSelection::Tau_OPS(ObjAccess* /*jet*/, unsigned int  /*iteration*/)
{
  //std::cout << "TauOPS " << jet_id << ", it " <<  iteration << std::endl;
  return KEEP;
}



void ObjectPreSelection::do_Electron(unsigned int  iteration)
{
  m_selector->AddSingleSelector("ELE",iteration);
}

void ObjectPreSelection::do_Jet(unsigned int  iteration)
{
  m_selector->AddSingleSelector("JET",iteration);
}

void ObjectPreSelection::do_Muon(unsigned int  iteration)
{
  m_selector->AddSingleSelector("MUO",iteration);
}

void ObjectPreSelection::do_LowBetaMuon(unsigned int  iteration)
{
  m_selector->AddSingleSelector("MULOWBETA",iteration);
}

void ObjectPreSelection::do_MuGirlMuon(unsigned int  iteration)
{
  m_selector->AddSingleSelector("MUGIRL",iteration);
}

void ObjectPreSelection::do_Photon(unsigned int  iteration)
{
  m_selector->AddSingleSelector("PHO",iteration);
}

void ObjectPreSelection::do_Tau(unsigned int  iteration)
{
  m_selector->AddSingleSelector("TAU",iteration);
}



void ObjectPreSelection::objectPreSelectionOrder()
{
  do_Electron(1);
  do_Muon(1);
  do_LowBetaMuon(1);
  do_MuGirlMuon(1);
  do_Jet(1);
  do_Photon(1);
}











ObjAccess::ObjAccess(std::string ob_name, unsigned int index, D3PDSelector* infosource)
{
  m_accessor = infosource;
  m_ob_name = ob_name;
  m_index = index; 
}

template <class T>
T ObjAccess::GetValue(std::string key)
{
// std::cout << "GET OB " << m_ob_name << " key " << key << " at "  << m_index <<std::endl;;
//  Mystery: The following thing also works, although GetFinalValue is declared private... ?
//  T a = m_accessor->GetFinalValue<T>(key, m_ob_name, m_index);

  T b = m_accessor->GetFinValByType<T>(m_ob_name, key, m_index);
  
  return b;
}

std::string ObjAccess::getType()
{
  return m_ob_name;
}

unsigned int ObjAccess::getIndex()
{
  return m_index;
}


