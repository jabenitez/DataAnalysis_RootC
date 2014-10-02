#include <math.h>

#define KEEP_BOTH 0 
#define KEEP_FIRST 2
#define KEEP_SECOND 1
#define REJECT_FIRST 1
#define REJECT_SECOND 2
#define REJECT_BOTH 3


//helper function
float deltaR(float eta1, float phi1, float eta2, float phi2)
{
  float diffphi = phi1 - phi2;
  while (fabs(diffphi) > M_PI) diffphi += ( diffphi > 0 ) ? -2*M_PI : 2*M_PI;
  
  float tmp = (eta1 - eta2 )*(eta1 - eta2 ) + (diffphi*diffphi);
  float deltaR = sqrt(tmp); 
  return deltaR;
}


class MyOverlapRemoval : public OverlapRemoval
{
 public:
  //Constructor, Mandatory
  MyOverlapRemoval(D3PDSelector* inst) : OverlapRemoval(inst) {};  

 
  void overlapRemovalOrder()
  {
    //In this method, we may define the order in which the OR is done
    //we can have multiple iterations, idnicated by the parameter

    do_Electron_Jet(1); //First do Electron vs Jet
    do_Tau_Electron(1); // Tau OR //uncomment if you want to use taus
    do_Tau_Muon(1); // Tau OR //uncomment if you want to use taus
    do_Tau_Jet(1); // Tau OR //uncomment if you want to use taus
    
    do_Muon_Jet(1); // Then do muon vs jet
    do_Electron_Jet(2); // Again do Electorn vs Jet
    
    //Available:

    /*    
    do_Electron_Jet(unsigned int iteration);
    do_Muon_Jet(unsigned int  iteration);
    do_Photon_Jet(unsigned int  iteration);
    do_Tau_Jet(unsigned int  iteration);
    
    do_Muon_Electron(unsigned int  iteration);
    do_Photon_Electron(unsigned int  iteration);
    do_Tau_Electron(unsigned int  iteration);
    
    do_Photon_Muon(unsigned int  iteration);
    do_Tau_Muon(unsigned int  iteration);
    
    do_Tau_Photon(unsigned int  iteration);
    
    */
    
    
  }
 
 
 
 //the actualy overlap removal methods
 //availabel are:
 
 /*
     //--VS Jet
    unsigned int ElectronJet_OR(ObjAccess* ele, ObjAccess* jet, unsigned int iteration);  
    unsigned int MuonJet_OR(ObjAccess* muo, ObjAccess* jet, unsigned int iteration);
    unsigned int PhotonJet_OR(ObjAccess* pho, ObjAccess* jet, unsigned int iteration);
    unsigned int TauJet_OR(ObjAccess* tau, ObjAccess* jet, unsigned int iteration);
  
    //--VS Electron
    unsigned int MuonElectron_OR(ObjAccess* muo, ObjAccess* ele,  unsigned int iteration);
    unsigned int PhotonElectron_OR(ObjAccess* pho, ObjAccess* ele,  unsigned int iteration);
    unsigned int TauElectron_OR(ObjAccess* tau, ObjAccess* ele,  unsigned int iteration);
    
    //--VS Muon
    unsigned int PhotonMuon_OR(ObjAccess* pho, ObjAccess* muo,  unsigned int iteration);
    unsigned int TauMuon_OR(ObjAccess* tau, ObjAccess* muo,  unsigned int iteration);

    unsigned int TauPhoton_OR(ObjAccess* tau, ObjAccess* pho,  unsigned int iteration);


 */
 
  unsigned int ElectronJet_OR(ObjAccess* ele, ObjAccess* jet, unsigned int iteration)
  {
    //Example Electron vs Jet
  
    
    //First Getting eta and phi of electron and jet
    float ele_eta = ele->GetValue<float>("_eta");
    float ele_phi = ele->GetValue<float>("_phi");

    float jet_eta = jet->GetValue<float>("_eta");
    float jet_phi = jet->GetValue<float>("_phi");
    
    
    //calculating deltaR
    float dR = deltaR(ele_eta,ele_phi,jet_eta,jet_phi);

    //Iteration 1 handinlig
    if (iteration == 1)
    {
      if (dR < 0.2) 
      {
        // First and Second refer to the order in the method name. Here, first means Electron, second means Jet
        return KEEP_FIRST; // OR return REJECT_SECOND;
      } 
    }
    
    //Iteration 2 handling
    if (iteration == 2)
    {
      if (dR > 0.2 && dR < 0.4) 
      {
        return KEEP_SECOND;// OR return REJECT_FIRST;
      }
    }
    
    return KEEP_BOTH; //keep both if nothing happened
  }
 
 
 
  unsigned int MuonJet_OR(ObjAccess* muo, ObjAccess* jet, unsigned int /*iteration*/)
  {
    float muo_eta = muo->GetValue<float>("_eta");
    float muo_phi = muo->GetValue<float>("_phi");

    float jet_eta = jet->GetValue<float>("_eta");
    float jet_phi = jet->GetValue<float>("_phi");
    
    float dR = deltaR(muo_eta,muo_phi,jet_eta,jet_phi);
    
    
    //std::cout << " mu_jet dR " << dR << std::endl;

    if (dR < 0.4 ) return REJECT_FIRST;
    
    
    return KEEP_BOTH;
  }
   
 
 




  unsigned int TauElectron_OR(ObjAccess* tau, ObjAccess* ele, unsigned int /*iteration*/)
  {
    float tau_eta = tau->GetValue<float>("_eta");
    float tau_phi = tau->GetValue<float>("_phi");

    float ele_eta = ele->GetValue<float>("_eta");
    float ele_phi = ele->GetValue<float>("_phi");
    
    float dR = deltaR(tau_eta,tau_phi,ele_eta,ele_phi);
    
    
    //std::cout << " mu_jet dR " << dR << std::endl;

    if (dR < 0.4 ) return REJECT_FIRST;
    
    
    return KEEP_BOTH;
  }



  unsigned int TauMuon_OR(ObjAccess* tau, ObjAccess* muo, unsigned int /*iteration*/)
  {
    float muo_eta = muo->GetValue<float>("_eta");
    float muo_phi = muo->GetValue<float>("_phi");

    float tau_eta = tau->GetValue<float>("_eta");
    float tau_phi = tau->GetValue<float>("_phi");
    
    float dR = deltaR(muo_eta,muo_phi,tau_eta,tau_phi);
    
    
    //std::cout << " mu_jet dR " << dR << std::endl;

    if (dR < 0.4 ) return REJECT_SECOND;
    
    
    return KEEP_BOTH;
  }

 
 
 
  unsigned int TauJet_OR(ObjAccess* tau, ObjAccess* jet, unsigned int /*iteration*/)
  {
    float tau_eta = tau->GetValue<float>("_eta");
    float tau_phi = tau->GetValue<float>("_phi");

    float jet_eta = jet->GetValue<float>("_eta");
    float jet_phi = jet->GetValue<float>("_phi");
    
    float dR = deltaR(tau_eta,tau_phi,jet_eta,jet_phi);
    
    
    //std::cout << " mu_jet dR " << dR << std::endl;

    if (dR < 0.4 ) return REJECT_SECOND;
    
    
    return KEEP_BOTH;
  }



 
  
  
  

};
