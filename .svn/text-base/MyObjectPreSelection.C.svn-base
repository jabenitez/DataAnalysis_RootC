#define KEEP 0
#define REJECT 1
#define PRE_SKIP_EVENT 2


class MyObjectPreSelection : public ObjectPreSelection
{

  public:

    MyObjectPreSelection(D3PDSelector* inst) : ObjectPreSelection(inst) {};
    //Mandatory
 
 
 
    void objectPreSelectionOrder()
    {
      //In this method which is called by the D3PD Selector, we define the order of how to execute 
      //the Object Preselection
      
      //We can do multiple iterations by giving an integer argument to the method.
      //This can later be reused to differentiate between the iterations
      //This is not used in the OPS example, but in the OR example
      
      do_Electron(1);
      do_Muon(1);
      do_LowBetaMuon(1);
      do_MuGirlMuon(1);
      do_Jet(1);
      //do_Photon(1);
      do_Tau(1);
      
    }

    unsigned int Electron_OPS(ObjAccess* ele, unsigned int  iteration)
    {
      (void)iteration; //avoid compiler warnings
      
      //The OPS methods return 0 if the obejct is to be kept, and 1 if it is to be rejected

      //Checking author (must be 1 or 3)
      //we can access any variable. However DO NOT CROSS-ACCESS different particle types, e.g. accessing muon variables in the Electron OPS method. This will mess up the internal ordering!!!
      
      //see
      // https://twiki.cern.ch/twiki/bin/view/AtlasProtected/SusyObjectDefintions
      
      //
      

      int author = ele->GetValue<int>("_author");
      if (author!=1 && author !=3) return REJECT;


      int medium = ele->GetValue<int>("_mediumPP");
      if (medium == 0) return REJECT;


      float pt = ele->GetValue<float>("_pt");
      if ( fabs(pt) <= 25000. ) return REJECT;
       

      float eta = ele->GetValue<float>("_eta");
      if ( fabs(eta) >= 2.47 ) return REJECT;
      
      
      int OQ = ele->GetValue<int>("_OQ");
      if ((OQ&1446) != 0) return REJECT;


      //      if (fabs(eta) > 1.37 &&  fabs(eta) < 1.52) return PRE_SKIP_EVENT; //electron crack veto

      //      float etcone20 = ele->GetValue<float>("_Etcone20");
      //      if (etcone20 >= 10000.) return REJECT;
             
      return KEEP;
    }

    unsigned int Muon_OPS(ObjAccess* muo, unsigned int  iteration)
    {
      (void)iteration; //avoid compiler warnings

      float eta = muo->GetValue<float>("_eta");
      if ( fabs(eta) >= 2.4 ) return REJECT;


      // Quality cuts
      int isComMuo = muo->GetValue<int>("_isCombinedMuon");
      int isSegmentTaggedMuo = muo->GetValue<int>("_isSegmentTaggedMuon");
      if (!(isComMuo||isSegmentTaggedMuo)) return REJECT;
      int isLooseMuo=muo->GetValue<int>("_loose");
      if (!isLooseMuo) return REJECT;

      //Track quality cuts:
      int MuonexpectBLayerHit = muo->GetValue<int>("_expectBLayerHit");
      int MuonnBLHits = muo->GetValue<int>("_nBLHits");
      if (MuonexpectBLayerHit && MuonnBLHits == 0) return REJECT;
      int MuonnPixHits = muo->GetValue<int>("_nPixHits");
      int MuonnPixelDeadSensors = muo->GetValue<int>("_nPixelDeadSensors");
      if (MuonnPixHits + MuonnPixelDeadSensors <= 1) return REJECT;
      int MuonnSCTHits = muo->GetValue<int>("_nSCTHits");
      int MuonnSCTDeadSensors=muo->GetValue<int>("_nSCTDeadSensors");
      if (MuonnSCTHits + MuonnSCTDeadSensors < 6) return REJECT;
      int MuonnPixHoles = muo->GetValue<int>("_nPixHoles");
      int MuonnSCTHoles = muo->GetValue<int>("_nSCTHoles");
      if (MuonnPixHoles + MuonnSCTHoles >= 3) return REJECT;
      int nTRTOutliers = muo->GetValue<int>("_nTRTOutliers");
      int MuonnTRTHits = muo->GetValue<int>("_nTRTHits");
      int nTRTTotal = nTRTOutliers + MuonnTRTHits;
      float Muonid_theta = muo->GetValue<float>("_id_theta");
      float trackEta = -log(tan(Muonid_theta/2));
      if (fabs(trackEta) < 1.9 && nTRTTotal <= 5) return REJECT;
      
      if (nTRTTotal > 5 && nTRTOutliers >= 0.9*nTRTTotal) return REJECT;

      //Isolation cut
      float ptcone20 = muo->GetValue<float>("_ptcone20");
      if ( ptcone20 >= 1800. ) return REJECT;

      //      float matchchi2 = muo->GetValue<float>("_matchchi2");
      //      if ( matchchi2 > 100. || matchchi2 < 0.) return REJECT;
      
      float pt = muo->GetValue<float>("_pt");
      if ( pt <= 25000. ) return REJECT;
       

       return KEEP;    
    }

  unsigned int MuGirlMuon_OPS(ObjAccess* muo, unsigned int  iteration)
  {
    (void)iteration; //avoid compiler warnings
    /*
    int isComMuo = muo->GetValue<int>("_isCombinedMuon");
    if (!isComMuo) return REJECT;
    
    float etcone20 = muo->GetValue<float>("_etcone20");
    if ( etcone20 >= 10000. ) return REJECT;
    
    float matchchi2 = muo->GetValue<float>("_matchchi2");
    if ( matchchi2 > 100. || matchchi2 < 0.) return REJECT;
*/    
		
		float eta = muo->GetValue<float>("_eta");
		if ( fabs(eta) >= 2.4 ) return REJECT;
		
		
		// Quality cuts
		int isComMuo = muo->GetValue<int>("_isCombinedMuon");
		int isSegmentTaggedMuo = muo->GetValue<int>("_isSegmentTaggedMuon");
		if (!(isComMuo||isSegmentTaggedMuo)) return REJECT;
		int isLooseMuo=muo->GetValue<int>("_loose");
		if (!isLooseMuo) return REJECT;
		
		//Track quality cuts:
		int MuonexpectBLayerHit = muo->GetValue<int>("_expectBLayerHit");
		int MuonnBLHits = muo->GetValue<int>("_nBLHits");
		if (MuonexpectBLayerHit && MuonnBLHits == 0) return REJECT;
		int MuonnPixHits = muo->GetValue<int>("_nPixHits");
		int MuonnPixelDeadSensors = muo->GetValue<int>("_nPixelDeadSensors");
		if (MuonnPixHits + MuonnPixelDeadSensors <= 1) return REJECT;
		int MuonnSCTHits = muo->GetValue<int>("_nSCTHits");
		int MuonnSCTDeadSensors=muo->GetValue<int>("_nSCTDeadSensors");
		if (MuonnSCTHits + MuonnSCTDeadSensors < 6) return REJECT;
		int MuonnPixHoles = muo->GetValue<int>("_nPixHoles");
		int MuonnSCTHoles = muo->GetValue<int>("_nSCTHoles");
		if (MuonnPixHoles + MuonnSCTHoles >= 3) return REJECT;
		int nTRTOutliers = muo->GetValue<int>("_nTRTOutliers");
		int MuonnTRTHits = muo->GetValue<int>("_nTRTHits");
		int nTRTTotal = nTRTOutliers + MuonnTRTHits;
		float Muonid_theta = muo->GetValue<float>("_id_theta");
		float trackEta = -log(tan(Muonid_theta/2));
		if (fabs(trackEta) < 1.9 && nTRTTotal <= 5) return REJECT;
		
		if (nTRTTotal > 5 && nTRTOutliers >= 0.9*nTRTTotal) return REJECT;
		
		//Isolation cut
		float ptcone20 = muo->GetValue<float>("_ptcone20");
		if ( ptcone20 >= 1800. ) return REJECT;
		
		//      float matchchi2 = muo->GetValue<float>("_matchchi2");
		//      if ( matchchi2 > 100. || matchchi2 < 0.) return REJECT;
		
		float pt = muo->GetValue<float>("_pt");
		if ( pt <= 25000. ) return REJECT;
    
    
    return KEEP;    
  }
  
  unsigned int LowBetaMuon_OPS(ObjAccess* muo, unsigned int  iteration)
  {
    (void)iteration; //avoid compiler warnings
      float eta = muo->GetValue<float>("_eta");
      if ( fabs(eta) >= 2.4 ) return REJECT;


      // Quality cuts
      int isComMuo = muo->GetValue<int>("_isCombinedMuon");
      int isSegmentTaggedMuo = muo->GetValue<int>("_isSegmentTaggedMuon");
      if (!(isComMuo||isSegmentTaggedMuo)) return REJECT;
      int isLooseMuo=muo->GetValue<int>("_loose");
      if (!isLooseMuo) return REJECT;

      //Track quality cuts:
      int MuonexpectBLayerHit = muo->GetValue<int>("_expectBLayerHit");
      int MuonnBLHits = muo->GetValue<int>("_nBLHits");
      if (MuonexpectBLayerHit && MuonnBLHits == 0) return REJECT;
      int MuonnPixHits = muo->GetValue<int>("_nPixHits");
      int MuonnPixelDeadSensors = muo->GetValue<int>("_nPixelDeadSensors");
      if (MuonnPixHits + MuonnPixelDeadSensors <= 1) return REJECT;
      int MuonnSCTHits = muo->GetValue<int>("_nSCTHits");
      int MuonnSCTDeadSensors=muo->GetValue<int>("_nSCTDeadSensors");
      if (MuonnSCTHits + MuonnSCTDeadSensors < 6) return REJECT;
      int MuonnPixHoles = muo->GetValue<int>("_nPixHoles");
      int MuonnSCTHoles = muo->GetValue<int>("_nSCTHoles");
      if (MuonnPixHoles + MuonnSCTHoles >= 3) return REJECT;
      int nTRTOutliers = muo->GetValue<int>("_nTRTOutliers");
      int MuonnTRTHits = muo->GetValue<int>("_nTRTHits");
      int nTRTTotal = nTRTOutliers + MuonnTRTHits;
      float Muonid_theta = muo->GetValue<float>("_id_theta");
      float trackEta = -log(tan(Muonid_theta/2));
      if (fabs(trackEta) < 1.9 && nTRTTotal <= 5) return REJECT;
      
      if (nTRTTotal > 5 && nTRTOutliers >= 0.9*nTRTTotal) return REJECT;

      //Isolation cut
      float ptcone20 = muo->GetValue<float>("_ptcone20");
      if ( ptcone20 >= 1800. ) return REJECT;

      //      float matchchi2 = muo->GetValue<float>("_matchchi2");
      //      if ( matchchi2 > 100. || matchchi2 < 0.) return REJECT;
      
      float pt = muo->GetValue<float>("_pt");
      float px = muo->GetValue<float>("_px");
      float py = muo->GetValue<float>("_py");
      float pz = muo->GetValue<float>("_pz");
      float beta = muo->GetValue<float>("_beta");
      
      if ( beta < 0. ) return REJECT;
      if ( beta > 1.0 ) return REJECT;
      
      if ( pt <= 25000. ) return REJECT;
      
      TLorentzVector tempmulowbeta_f;
      
      std::vector<TVector3> slepfake;
      TLorentzVector tempStau;
      TVector3 tempSlepton;    
      tempSlepton.SetXYZ(px/1000.0,py/1000.0,pz/1000.0);
      float staumass =sqrt(1-beta*beta)*tempSlepton.Mag()/beta;
      //      if ( staumass <= 80. ) return REJECT; // added cut to remove slepton contamination
      //			cout << "PASSED MASS CUT: " <<  staumass << endl;
      		 
    return KEEP;    
  }
  
  unsigned int Jet_OPS(ObjAccess* jet, unsigned int  iteration)
    {
      (void)iteration; //avoid compiler warnings

      float eta = jet->GetValue<float>("_eta");
      if ( fabs(eta) >= 2.8 ) return REJECT;

      float pt = jet->GetValue<float>("_pt");
      if ( fabs(pt) <= 20000. ) return REJECT;

      int isbad = jet->GetValue<int>("_isBadLooseMinus");
      if ( isbad ) return REJECT;

       return KEEP;    
    }


    unsigned int Tau_OPS(ObjAccess* tau, unsigned int  iteration)
    {
      (void)iteration; //avoid compiler warnings


      float eta = tau->GetValue<float>("_eta");
      if ( fabs(eta) >= 2.5 ) return REJECT;

      float pt = tau->GetValue<float>("_pt");
      if ( fabs(pt) <= 20000. ) return REJECT;

      //      int ntr = tau->GetValue<int>("_numTrack");
      //      if ( ntr!=1 && ntr!=3 ) return REJECT;

      //      float charge = tau->GetValue<float>("_charge");
      //      if ( fabs(charge)!=1.0 ) return REJECT;

      //      int cutsafemed = tau->GetValue<int>("_tauCutSafeMedium");
      //      if ( cutsafemed!=1 ) return REJECT;

      //      int muonvet = tau->GetValue<int>("_muonVeto");
      //      if ( muonvet!=0 ) return REJECT;
    
       return KEEP;    
    }

/*
    unsigned int Photon_OPS(ObjAccess* pho, unsigned int  iteration)
    {
        
      return KEEP;
    }
*/



};

