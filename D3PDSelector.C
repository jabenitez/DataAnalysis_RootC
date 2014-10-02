#define D3PDSelector_cxx
//#define USERW 1

#include "D3PDSelector.h"
#include "OR_OPS_IS_Interface.h"
#include "Analysis.h"

#include <TROOT.h>
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <TLeaf.h>
#include <TLeafObject.h>
#include <math.h>
#include "TMap.h"
#include "TDSet.h"

#include "OR_OPS_IS_Interface.C"
#include "Analysis.C"


void D3PDSelector::Begin(TTree * /*tree*/)
{
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
   std::cout << "BEGIN" << std::endl;



   TString option = GetOption();
}


void D3PDSelector::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();
  
  std::cout << "SLAVEBEGIN" << std::endl;




  //create OPS and OR Tool
  m_OR_class = new MyOverlapRemoval(this);
  std::cout << "overlap removal" << std::endl;
  m_OPS_class = new MyObjectPreSelection(this);
  std::cout << "object preselection" << std::endl;
  m_Analysis_class = new Analysis(this);
  std::cout << "analysis" << std::endl;
  
  //Ask the OPS and OR Tools to define the order of OPS and OR
  m_OR_class->overlapRemovalOrder();
  m_OPS_class->objectPreSelectionOrder();


}


void D3PDSelector::Init(TTree *tree)
{

   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
  // Set branch addresses and branch pointers


  std::cout << "INIT" << std::endl;

// we do the following in Analysis.C
//  SetElectronContainerPrefix("el");
//  SetMuonContainerPrefix("mu");
//  SetJetContainerPrefix("jet");
//  SetPhotonContainerPrefix("ph");


  //Reset intenral data structure to Branch Addresses  
  m_data.clear();
  m_branches.clear();
  m_types.clear();

  if (!tree) return;
  fChain = tree;
  fChain->SetMakeClass(1);
  
  TObjArray* leaves =  tree->GetListOfLeaves();

  for (int c=0; c<leaves->GetSize(); c++)
  {
    TLeaf* leaf = (TLeaf*)leaves->UncheckedAt(c);
    TLeafObject* leafobj = (TLeafObject*)leaf;
  
  
    //TLeafObject* curbra = (TLeafObject*)tor->At(c);
    const char* curbraname = leafobj->GetName();
    const char* curtitle = leafobj->GetTypeName();
    //const char* classn = leafobj->ClassName();
  
    if (m_verbose) 
      std::cout << curbraname << "    " << /*curbraclassName <<"    " << */curtitle << std::endl;
  }

}


Bool_t D3PDSelector::Process(Long64_t entry)
{
  
  m_loaded.clear(); //Reset the information of what has been loaded already
  
  if (m_verbose)
    std::cout <<  "Event Nr. " << entry << std::endl;
  m_curentry = entry;

  //fChain->GetTree()->GetEntry(entry);
  // OR (faster)
  //GetUsedEntries(entry);
  //GetUsedEntry is not needed anymore. Variables are only loaded if requested
  //See method loadData



  std::map< std::string, std::string >::iterator itr;


  for( itr = m_object_counter.begin(); itr!=m_object_counter.end(); itr++)
  {
    std::string key = (*itr).first;
    std::string value = (*itr).second;

    //first we clear the array that contains the indices
    m_vector_indices[key].clear();  

    //we get the number of elements for that kind of object
    unsigned int num_el = GetScalarVal<Int_t>(value);


   //std::cout << "MAKING SPACE FOR " << key << " " << num_el << std::endl;
    

    //we create a vector with exactly that many elements. the vector contains 0,1,2... # num_el
    //later, object preselection and overlap removal will reduce that vector
    for (unsigned int zu = 0; zu < num_el; zu++)
    {
      //std::cout << " --- >MAKING SPACE FOR " << key << " " << zu << std::endl;
      m_vector_indices[key].push_back(zu);
    }
     //at this stage, m_vector_indices should look like that:
    // m_vector_indices["el_"][] = {0,1,2,3... el_n-1}
    // m_vector_indices["mu_"][] = {0,1,2,3... mu_n-1}
    // m_vector_indices["ph_"][] = {0,1,2,3... ph_n-1}
    // m_vector_indices["jet_"][] = {0,1,2,3... jet_n-1}

 
  }

  
  //Calling the Pre OPS/OR function
  unsigned int skipevent = m_Analysis_class->Pre_OPS_OR(entry);
  if (skipevent) return kTRUE;
  
  //let's go into object preselection
  skipevent = performOPS();
  if (skipevent) return kTRUE;

  skipevent = m_Analysis_class->Between_OPS_and_OR(entry);
  if (skipevent) return kTRUE;
  
  //let's go into overlap removal
  skipevent = performOR();
  if (skipevent) return kTRUE;
  //Calling Analysis
  m_Analysis_class->doAnalysis(entry);
  m_Analysis_class->finalizeEvent(entry);


  return kTRUE;
}

void D3PDSelector::SlaveTerminate()
{
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.

   std::cout << "SLAVTERM" << std::endl;
}

void D3PDSelector::Terminate()
{
  Analysis::postAnalysis(fOutput);
    
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.

}



void D3PDSelector::SetElectronContainerPrefix(std::string prefix)
{
  m_object_prefix["ELE"] = prefix;
  m_object_counter["ELE"] = prefix + "_n";
}

void D3PDSelector::SetMuonContainerPrefix(std::string prefix)
{
  m_object_prefix["MUO"] = prefix;
  m_object_counter["MUO"] = prefix + "_n";
}

void D3PDSelector::SetLowBetaMuonContainerPrefix(std::string prefix)
{
  m_object_prefix["MULOWBETA"] = prefix;
  m_object_counter["MULOWBETA"] = prefix + "_n";
}

void D3PDSelector::SetMuGirlMuonContainerPrefix(std::string prefix)
{
  m_object_prefix["MUGIRL"] = prefix;
  m_object_counter["MUGIRL"] = prefix + "_n";
}

void D3PDSelector::SetJetContainerPrefix(std::string prefix)
{
  m_object_prefix["JET"] = prefix;
  m_object_counter["JET"] = prefix + "_n";
}

void D3PDSelector::SetPhotonContainerPrefix(std::string prefix)
{
  m_object_prefix["PHO"] = prefix;
  m_object_counter["PHO"] = prefix + "_n";
}

void D3PDSelector::SetTauContainerPrefix(std::string prefix)
{
  m_object_prefix["TAU"] = prefix;
  m_object_counter["TAU"] = prefix + "_n";
}





/* //DEPRECATED
TLorentzVector D3PDSelector::GetFourVec(std::string contname, unsigned int index)
{
	std::string prefix = getCorrespondingPrefix(contname);
	TLorentzVector par;
	float E = GetVal<float>(prefix+"E", index);
	float eta = GetVal<float>(prefix+"eta", index);
	float phi = GetVal<float>(prefix+"phi", index);
	float pt = GetVal<float>(prefix+"pt", index);
	par.SetPtEtaPhiE(pt,eta,phi,E);
	
	return par;
}

TLorentzVector D3PDSelector::GetFinFourVec(std::string contname, unsigned int index)
{
	std::string prefix = getCorrespondingPrefix(contname);
	TLorentzVector par;
	float E = GetFinalValue<float>(prefix+"E", prefix, index);
	float eta = GetFinalValue<float>(prefix+"eta",prefix, index);
	float phi = GetFinalValue<float>(prefix+"phi",prefix, index);
	float pt = GetFinalValue<float>(prefix+"pt",prefix, index);
	par.SetPtEtaPhiE(pt,eta,phi,E);
	
	return par;
	
}
*/


//Getting Scalar Values
template <class T>
T D3PDSelector::GetScalarVal(std::string key)
{
    if (m_types.find(key) == m_types.end())
    {
      loadBranch(key);
    }
    loadData(key);

    T* value = (T*)m_data[key];
    T ret = *value;
    return ret;
}
//end scalar


//Getting Values by container name
template <class T>
T D3PDSelector::GetRawVal(std::string contname, unsigned int index) //before OPS/OR
{
  std::vector< T >* vv = GetVectorValue< std::vector<T> >(contname);
  T val = vv->at(index);
  return val;
}

    
template <class T>
T D3PDSelector::GetRawVector(std::string contname, unsigned int* size) //after OPS/OR
{
  T* vval = GetVectorValue<T>(contname, size);
  return *vval;
}
//end get values by container name


//Getting Values by ObjectType (Object Type being JET, MUO, ELE, PHO, TAU)
template <class T>
T D3PDSelector::GetValByType(std::string ObjectType, std::string contname, unsigned int index) //before OPS/OR 
{
  std::string prefix = getCorrespondingPrefix(ObjectType);
  std::string container = prefix + contname;
  
  std::vector< T >* vv = GetVectorValue< std::vector<T> >(container);
  T val = vv->at(index);
  return val;
}

template <class T>
T D3PDSelector::GetFinValByType(std::string ObjectType, std::string contname, unsigned int index) //after OPS/OR
{
  std::string prefix = getCorrespondingPrefix(ObjectType);
  std::string container = prefix + contname;

  //std::cout << " Trying to get " << container << " OB "<< ObjectType  << " index " <<index <<std::endl;;

  T ret = GetFinalValue<T>(container, ObjectType, index);
  return ret;  
}
// end values by object type



//Getting Vectors by ObjectType
template <class T>
T D3PDSelector::GetVectorByType(std::string ObjectType, std::string contname, unsigned int* size) //before OPS/OR
{
  std::string prefix = getCorrespondingPrefix(ObjectType);
  std::string container = prefix + contname;

  T* vval = GetVectorValue<T>(container, size);
  return *vval;
}

template <class T>
T D3PDSelector::GetFinVectorByType(std::string ObjectType, std::string contname, unsigned int* size) //after OPS/OR
{
  std::string prefix = getCorrespondingPrefix(ObjectType);
  std::string container = prefix + contname;
  return GetFinalVectorValue<T>(container,ObjectType,size);
}
//end Vectors by ObjectType


//Getting FourVectors
TLorentzVector D3PDSelector::GetFourVecByType(std::string ObjectType, unsigned int index)
{
	TLorentzVector par;
	float E = GetValByType<float>(ObjectType, "_E", index);
	float eta = GetValByType<float>(ObjectType, "_eta", index);
	float phi = GetValByType<float>(ObjectType, "_phi", index);
	float pt = GetValByType<float>(ObjectType, "_pt", index);
	par.SetPtEtaPhiE(pt,eta,phi,E);
	
	return par;
}

TLorentzVector D3PDSelector::GetFinFourVecByType(std::string ObjectType, unsigned int index, std::string addition)
{
	
	std::string s_ene = addition + "_E";
	std::string s_eta = addition + "_eta";
	std::string s_phi = addition + "_phi";
	std::string s_pt = addition + "_pt";
	
	
	
	TLorentzVector par;
	float E = GetFinValByType<float>(ObjectType, s_ene, index);
	float eta = GetFinValByType<float>(ObjectType, s_eta, index);
	float phi = GetFinValByType<float>(ObjectType, s_phi, index);
	float pt = GetFinValByType<float>(ObjectType, s_pt, index);
	par.SetPtEtaPhiE(pt,eta,phi,E);
	
	return par;
	
}

TLorentzVector D3PDSelector::GetFinFourVecByTypeMass(std::string ObjectType, unsigned int index, std::string addition)
{
	
	std::string s_mass = addition + "_m";
	std::string s_eta = addition + "_eta";
	std::string s_phi = addition + "_phi";
	std::string s_pt = addition + "_pt";
	
	
	
	TLorentzVector par;
	float mass = GetFinValByType<float>(ObjectType, s_mass, index);
	float eta = GetFinValByType<float>(ObjectType, s_eta, index);
	float phi = GetFinValByType<float>(ObjectType, s_phi, index);
	float pt = GetFinValByType<float>(ObjectType, s_pt, index);
	par.SetPtEtaPhiM(pt,eta,phi,mass);
	
	return par;
	
}
//end four vector

//Getting Sizes
unsigned int D3PDSelector::GetSizeByType(std::string ObjectType)
{
  return GetScalarVal<Int_t>(m_object_counter[ObjectType]);
}

unsigned int D3PDSelector::GetFinSizeByType(std::string ObjectType)
{
  return m_vector_indices[ObjectType].size();
}
//end sizes



template <class T>
void D3PDSelector::SortFinBy(std::string ObjectType, std::string contname, bool desc)
{
  std::string orig = getCorrespondingPrefix(ObjectType) + contname;  
  std::vector<T> sortby = this->GetRawVector< std::vector<T> >(orig);
  
  MySorter<T> * th_sort = new MySorter< T >(sortby, desc);

  std::sort(m_vector_indices[ObjectType].begin(), m_vector_indices[ObjectType].end(), the_comp< T >(th_sort)); 

  delete th_sort;

}

template <class T>
void D3PDSelector::SortFinByVector(std::string ObjectType, std::vector<T> sortby, bool desc)
{
  //std::string orig = getCorrespondingPrefix(ObjectType) + contname;  
  //std::vector<T> sortby = this->GetRawVector< std::vector<T> >(orig);

  MySorter<T> * th_sort = new MySorter< T >(sortby, desc);


//  std::cout << "VEC SIZE src " <<sortby.size() << std::endl;
//  std::cout << "VEC SIZE " << m_vector_indices[ObjectType].size() << std::endl;


 // for (unsigned int o = 0 ; o< m_vector_indices[ObjectType].size(); o++)
//  {
//    std::cout << m_vector_indices[ObjectType].at(o) << ", ";
//  }
//std::cout << std::endl;
  

  std::sort(m_vector_indices[ObjectType].begin(), m_vector_indices[ObjectType].end(), the_comp< T >(th_sort)); 

  delete th_sort;

}



/*template <class Y>
Y* D3PDSelector::GetPointerValue(std::string key)
{
    if (m_types.find(key) == m_types.end())
    {
      loadBranch(key);
    }
    loadData(key);

    Y* value = (Y*)*m_data[key];    
    return value;
}*/



//NOTE:  ---- private methods -------

template <class Z>
Z* D3PDSelector::GetVectorValue(std::string key, unsigned int* size)
{
    if (m_types.find(key) == m_types.end())
    {
      loadBranch(key);
    }
    loadData(key);

    Z* value = (Z*)*m_data[key];
    if (size)
      *size = value->size();
    return value;
}


template <class T>
T D3PDSelector::GetFinalValue(std::string key, std::string ob_name, unsigned int index)
{
  //printList(ob_name);
  
  
  if (index >= m_vector_indices[ob_name].size())
  {
    std::cout << "WARNING2 you are requesting an element (OB "<< ob_name << " var " << key << " index " << index <<") outside vector range! ("<< m_vector_indices[ob_name].size() <<") Your programm might crash now" << std::flush;
    
    
  }
  
  unsigned int real_index = m_vector_indices[ob_name].at(index); 

  unsigned int size;
  std::vector<T>* tmp =  GetVectorValue< std::vector<T> >(key, &size);

  if (real_index >= tmp->size())
  {
    std::cout << "WARNING you are requesting an element ("<< key << " " <<real_index <<") outside vector range! ("<< size <<") Your programm might crash now" << std::flush;
  }
  
  return tmp->at(real_index);
}


template <class T>
T D3PDSelector::GetFinalVectorValue(std::string key, std::string ob_name, unsigned int* size)
{
  T* full_vec = GetVectorValue< T >(key);
  T ret;
  
  for (unsigned int u=0; u < m_vector_indices[ob_name].size(); u++)
  {
    ret.push_back(full_vec->at(m_vector_indices[ob_name].at(u)));
  }
  if (size) *size = ret.size(); 
  return ret;

}



std::string D3PDSelector::getCorrespondingPrefix(std::string containerName)
{
  return m_object_prefix[containerName];
}



void D3PDSelector::GetUsedEntries(Long64_t entry)
{
  std::map<std::string, TBranch*>::iterator cbr = m_branches.begin();
  for (; cbr!=m_branches.end(); cbr++)
  {
     //std::cout <<  cbr->second  << " ";
     cbr->second->GetEntry(entry); 
  }
}

void D3PDSelector::loadData(std::string key)
{
  if (m_loaded.find(key) == m_loaded.end())
  {
    TBranch* toload = m_branches.find(key)->second;
    toload->GetEntry(m_curentry);
    m_loaded[key] = true;
  }
}


void D3PDSelector::loadBranch(std::string key )
{
  // std::cout << "Getting List Of Leaves" << std::endl;
   TObjArray* leaves = fChain->GetListOfLeaves();
  // std::cout << "Got List Of Leaves" << std::endl;
   
   for (int c=0; c<leaves->GetSize(); c++)
   {
      TLeaf* leaf = (TLeaf*)leaves->UncheckedAt(c);
      TLeafObject* leafobj = (TLeafObject*)leaf;

      const char* curbraname = leafobj->GetName();

     // std::cout << "Got Leave " <<  curbraname << std::endl;

      if (strcmp(curbraname, key.c_str()) == 0)
      {
        const char* curtitle = leafobj->GetTypeName();
        std::string classtype(leafobj->ClassName());
        m_types[key] = std::string(curtitle);

        void* address;
 
        //if (m_types[key].find("<") == std::string::npos) //vector type? //old check
        //TLeafElement means it's a pointer to an obect
        //Else its a more primitve type (int, float etc)
        if (classtype.find("TLeafElement") == std::string::npos)
        {
          void* tmpfl = (void* )(new double(0.)); //make sure enough space, so make doube
          address = tmpfl;
        }
        else
        {
          void** tmp = new void*; //works   1
          *tmp = 0;      
          address=tmp;
        }
      
        m_data[key] = (void**)address;
        TBranch* cur_branch = 0;
        m_branches[key] = cur_branch;
        fChain->SetBranchAddress(key.c_str(), m_data[key] , &m_branches[key] );
        //m_branches[key]->GetEntry(m_curentry); //Entry is loaded in loadData method

        return;
      }
   }
   std::cout << "WARNING: Requested variable " << key << " not found. The program might crash now." << std::endl;

}

void D3PDSelector::AddDualSelector(std::string first, std::string second, unsigned int iteration )
{
  //this function stores the overlap removal combination (e.g. ele vs jet) in m_OR_order for later processing
  std::pair< std::string, std::string > vs;
  vs.first = first;
  vs.second = second;
  
  std::pair< std::pair< std::string, std::string >, unsigned int > dual_entry;
  dual_entry.first = vs; 
  dual_entry.second = iteration ;
  
  m_OR_order.push_back(dual_entry);

}


void D3PDSelector::AddSingleSelector(std::string which, unsigned int iteration)
{
  std::pair< std::string, unsigned int > single_entry;
  single_entry.first = which; 
  single_entry.second = iteration ;
  
  m_OPS_order.push_back(single_entry);
  
}

unsigned int D3PDSelector::performOPS()
{
  for (unsigned int r = 0; r < m_OPS_order.size(); r++)
  {
    

    std::string ops_name = m_OPS_order.at(r).first; //the OPS object, eg "mu_"
    unsigned int iteration = m_OPS_order.at(r).second; //iteration, as given by user


    //printList(ops_name);
 
    std::vector< unsigned int >::iterator one = m_vector_indices[ops_name].begin();
    while ( one != m_vector_indices[ops_name].end() )
    {

      unsigned int choice = 0;
      unsigned int num_el_ops = one - m_vector_indices[ops_name].begin();// we must get the number of the element in the vector 
  
      if (m_verbose) std::cout << "doing " << ops_name << " "  <<  num_el_ops << " " <<   m_vector_indices[ops_name].size() << " ... ";
      
      
      ObjAccess * tmp = new ObjAccess(ops_name, num_el_ops, this);
      choice = CallOPS(tmp, iteration);  //we are using a helper function in order to avoid clutter
      delete tmp;

      //std::cout << "here" <<std::endl;
    
      if (choice == 2)  return 1;
    
      if (choice == 1)
      {
        one = m_vector_indices[ops_name].erase(one);
        if (m_verbose) std::cout << " drop." << std::endl;
      }
      else
      {
        ++one;        
        if (m_verbose) std::cout << " keep." << std::endl;

      }
    
    }
    //printList(ops_name);
  }

  return 0; //keep event
}

unsigned int D3PDSelector::performOR()
{
//    std::cout << "performOR" << std::endl;

//   printList(std::string("el_"));
//   printList(std::string("mu_"));
//   printList(std::string("ph_"));
//   printList(std::string("jet_"));

   for (unsigned int r = 0; r < m_OR_order.size(); r++)
   {
      std::string oro1 = m_OR_order.at(r).first.first; //the first OR object, eg "mu_"
      std::string oro2 = m_OR_order.at(r).first.second; //the second OR object, eg "jet_"
      unsigned int iteration = m_OR_order.at(r).second; //iteration, as given by user

      //unsigned int size_oro1 = m_vector_indices[oro1].size(); //get the current number of e.g mu_ elements //not used, we use iters
      //unsigned int size_oro2 = m_vector_indices[oro2].size();//get the current number of e.g jet_ elements //not used, we use iters
      
      std::vector< unsigned int >::iterator one = m_vector_indices[oro1].begin();
      std::vector< unsigned int >::iterator two;
      
      //this loop combines pair-wise the objects in question. the added complication is due to the fact that during the double-looping, entries are deleted. That's the way to avoid accessing void elements.


      while ( one!= m_vector_indices[oro1].end() )
      {
        //std::cout << "got a " << oro1 << std::endl;
        two = m_vector_indices[oro2].begin();
        bool okay = true; //goes to false if an a-element is deleted
        while ( okay && two != m_vector_indices[oro2].end() )
        {
          unsigned int choice = 0 ; //default choice: KEEP BOTH
          bool b_erased = false; //goes to true if an element from b is erased
          
          
          unsigned int num_el_oro1 = one - m_vector_indices[oro1].begin();// we must get the number of the element in the vector 1 
          unsigned int num_el_oro2 = two - m_vector_indices[oro2].begin();// we must get the number of the element in the vector 2

          ObjAccess * tmp1 = new ObjAccess(oro1, num_el_oro1, this);
          ObjAccess * tmp2 = new ObjAccess(oro2, num_el_oro2, this);
          
          choice = CallOR(tmp1, tmp2, iteration);  //we are using a helper function in order to avoid clutter
          
          delete tmp1;
          delete tmp2;
          


          if ( choice & 1)
          {
              //REJECT "a" element
              one = m_vector_indices[oro1].erase(one);
              okay = false;
          }
          if ( choice & 2)
          {
              //REJECT "b" element
              two = m_vector_indices[oro2].erase(two);
              b_erased = true;
          }
          if (!b_erased) ++two; //if no b element was erased, need to go to the next (if it was erased, the iterator is already advance by two = erase...)
        }

        if (okay) ++one; //if no a element was erased, need to go to the next (if it was erased, the iterator is already advance by one = erase...)
          
      } //end of while
   } //end of for loop
//   printList(std::string("el_"));
//   printList(std::string("jet_"));
  return 0; //keep event

}

unsigned int D3PDSelector::CallOR(ObjAccess* element1, ObjAccess* element2, unsigned int iteration)
{
   unsigned int choice = 0;
   
   if ( element2->getType() == "JET" )
   {
      if ( element1->getType() == "ELE" ) choice = m_OR_class->ElectronJet_OR(element1, element2, iteration);
      if ( element1->getType() == "MUO" ) choice = m_OR_class->MuonJet_OR(element1, element2, iteration);
      if ( element1->getType() == "PHO" ) choice = m_OR_class->PhotonJet_OR(element1, element2, iteration);
      if ( element1->getType() == "TAU" ) choice = m_OR_class->TauJet_OR(element1, element2, iteration);

   }

   if ( element2->getType() == "ELE" )
   {
      if ( element1->getType() == "MUO" ) choice = m_OR_class->MuonElectron_OR(element1, element2, iteration);
      if ( element1->getType() == "PHO" ) choice = m_OR_class->PhotonElectron_OR(element1, element2, iteration);
      if ( element1->getType() == "TAU" ) choice = m_OR_class->TauElectron_OR(element1, element2, iteration);
   }

   if ( element2->getType() == "MUO" )
   {
      if ( element1->getType() == "PHO" ) choice = m_OR_class->PhotonMuon_OR(element1, element2, iteration);
      if ( element1->getType() == "TAU" ) choice = m_OR_class->TauMuon_OR(element1, element2, iteration);
   }

   if ( element2->getType() == "PHO" )
   {
      if ( element1->getType() == "TAU" ) choice = m_OR_class->TauPhoton_OR(element1, element2, iteration);
   }

   return choice; 
}

unsigned int D3PDSelector::CallOPS(ObjAccess* element, unsigned int iteration )
{
  unsigned int choice = 0;
  
  if ( element->getType() == "JET" )
  {
    choice = m_OPS_class->Jet_OPS(element, iteration);
  }
  
  if ( element->getType() == "ELE" )
  {
    choice = m_OPS_class->Electron_OPS(element, iteration);
  }
  
  if ( element->getType() == "MUO" )
  {
    choice = m_OPS_class->Muon_OPS(element, iteration);
  }
  
  if ( element->getType() == "MUGIRL" )
  {
    choice = m_OPS_class->MuGirlMuon_OPS(element, iteration);
  }

  if ( element->getType() == "MULOWBETA" )
  {
    choice = m_OPS_class->LowBetaMuon_OPS(element, iteration);
  }

  if ( element->getType() == "PHO" )
  {
    choice = m_OPS_class->Photon_OPS(element, iteration);
  }

  if ( element->getType() == "TAU" )
  {
    choice = m_OPS_class->Tau_OPS(element, iteration);
  }
  
  
  return choice; 
}


void D3PDSelector::printList(std::string key)
{
    std::cout << key << " (" << m_vector_indices[key].size() << ") : " ;
  for (unsigned int i=0; i < m_vector_indices[key].size(); i++)
    std::cout << m_vector_indices[key].at(i) << ", ";
  
  std::cout << std::endl;
}


void D3PDSelector::AddTObject(TObject * obj)
{
  fOutput->Add(obj);
}
