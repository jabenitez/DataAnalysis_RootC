#ifndef D3PDSelector_h
#define D3PDSelector_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TLorentzVector.h>

#include "OR_OPS_IS_Interface.h"



//Sorting helper classes and struct
template <class Q> class MySorter
{
 private:
   std::vector< Q > m_sort_array;
   bool m_descending;
 public:
 MySorter( std::vector< Q > the_array, bool desc = true)
 {
   m_sort_array = the_array;
   m_descending = desc;
 }
 bool thesorterfunc(unsigned int a, unsigned int b)
 {
   if (m_descending)
    return m_sort_array.at(a) > m_sort_array.at(b);
   else
    return m_sort_array.at(a) < m_sort_array.at(b);
 }
};


template <class Z> struct the_comp : std::binary_function<unsigned int, unsigned int, bool>
{
    the_comp( MySorter<Z> * p) : the_sorter(p) {}
    bool operator() (unsigned int o1, unsigned int o2) 
    {
        return the_sorter->thesorterfunc(o1, o2);
    }
    MySorter<Z> * the_sorter;
};
//






class OverlapRemoval;
class ObjectPreSelection;
class ObjAccess;
class Analysis;

class D3PDSelector : public TSelector {
public :
    TTree          *fChain;   //!pointer to the analyzed TTree or TChain
    
    
    D3PDSelector(TTree * /*tree*/ =0) {m_verbose = 0; }
    virtual ~D3PDSelector() { }
    virtual Int_t   Version() const { return 2; }
    virtual void    Begin(TTree *tree);
    virtual void    SlaveBegin(TTree *tree);
    virtual void    Init(TTree *tree);
    virtual Bool_t  Notify();
    virtual Bool_t  Process(Long64_t entry);
    virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
    virtual void    SetOption(const char *option) { fOption = option; }
    virtual void    SetObject(TObject *obj) { fObject = obj; }
    virtual void    SetInputList(TList *input) { fInput = input; }

    virtual void    SlaveTerminate();
    virtual void    Terminate();
    //   virtual float   GetValue(std::string key, unsigned int array_index = 0, int *arr_size = 0);

    virtual TList   *GetOutputList() const { return fOutput; }
    virtual void    AddTObject(TObject * obj);

    virtual void    AddDualSelector(std::string first, std::string second, unsigned int iteration);
    virtual void    AddSingleSelector(std::string which, unsigned int iteration);


    virtual void    SetElectronContainerPrefix(std::string prefix="el");
    virtual void    SetMuonContainerPrefix(std::string prefix="mu");
    virtual void    SetLowBetaMuonContainerPrefix(std::string prefix="mu");
    virtual void    SetMuGirlMuonContainerPrefix(std::string prefix="mu");
    virtual void    SetJetContainerPrefix(std::string prefix="jet");
    virtual void    SetPhotonContainerPrefix(std::string prefix="ph");
    virtual void    SetTauContainerPrefix(std::string prefix="tau");
    //User-Methods to get data:
    
    //Getting Scalar Values
    template <class T>
    T GetScalarVal(std::string contname);    


    //Getting Values by container name
    template <class T>
    T GetRawVal(std::string contname, unsigned int index); //before OPS/OR
    
    template <class T>
    T GetRawVector(std::string contname, unsigned int* size = 0); //after OPS/OR


    //Getting Values by ObjectType (Object Type being JET, MUO, ELE, PHO, TAU)
    template <class T>
    T GetValByType(std::string ObjectType, std::string contname, unsigned int index); //before OPS/OR 

    template <class T>
    T GetFinValByType(std::string ObjectType, std::string contname, unsigned int index); //after OPS/OR
    
    
    //Getting Vectors by ObjectType
    template <class T>
    T GetVectorByType(std::string ObjectType, std::string contname, unsigned int* size=0); //before OPS/OR

    template <class T>
    T GetFinVectorByType(std::string ObjectType, std::string contname, unsigned int* size=0); //after OPS/OR
    
    
    //Getting FourVectors by ObjectType
    TLorentzVector GetFourVecByType(std::string ObjectType, unsigned int index);
    TLorentzVector GetFinFourVecByType(std::string ObjectType, unsigned int index, std::string addition="");
    TLorentzVector GetFinFourVecByTypeMass(std::string ObjectType, unsigned int index, std::string addition="");
    
    
    //Getting Sizes
    unsigned int GetSizeByType(std::string ObjectType);
    unsigned int GetFinSizeByType(std::string ObjectType);
   
   // end User-Methods
   
    template <class T>
    void SortFinBy(std::string ObjectType, std::string contname, bool desc = true);
   
    template <class T>
    void SortFinByVector(std::string ObjectType, std::vector<T> sortybvec, bool desc = true);
   

  private:
  
  
    virtual void    loadBranch(std::string key);
    virtual void    loadData(std::string key);

    virtual void    GetUsedEntries(Long64_t entry);

    virtual unsigned int  performOPS();
    virtual unsigned int    performOR();

    virtual unsigned int    CallOR(ObjAccess* element1, ObjAccess* element2, unsigned int iteration );
    virtual unsigned int    CallOPS(ObjAccess* element, unsigned int iteration );


    template <class Z>
    Z* GetVectorValue(std::string key, unsigned int* size = 0);    


    template <class T>
    T GetFinalValue(std::string key, std::string ob_name, unsigned int index);
    //Final means, that this will return Values After objectPreselection and OverlapRemovel
    template <class T>
    T GetFinalVectorValue(std::string key, std::string ob_name, unsigned int* size = 0);

    void printList(std::string key); //helper function
    std::string getCorrespondingPrefix(std::string containerName);

  
   //internal variables which hold branches, pointers to variables, types
    std::map<std::string, void**> m_data;
    std::map<std::string, TBranch*> m_branches;
    std::map<std::string, std::string> m_types;
    std::map<std::string, bool> m_loaded;

    //current entry
    Long64_t m_curentry;    
    std::string m_curDataSet;


    //variable grouping map
    std::map< std::string, std::vector<unsigned int> > m_vector_indices; 
    //vector_indices is a vector which holds the information of which elements inside a vector, e.g. mu_px are kept. some are rejected due to object preselection and some due to overlap removal. it will also be used for sorting according to a variable. e.g. user wants to sort all mu_ variables according to pt.
    //for each variable group, such a vector will be made

    std::map< std::string, std::string > m_object_prefix; //contians object_prefixes, like mu_, el_
    std::map< std::string, std::string > m_object_counter; //contians the container name which holds the number of objects, for eample mu_n
    
    int m_verbose;
    
    OverlapRemoval* m_OR_class;
    ObjectPreSelection* m_OPS_class;
    Analysis* m_Analysis_class;

    //Single (Object Preselction) and Dual  (Overlap Removal) Selecors
    std::vector< std::pair< std::string, unsigned int > > m_OPS_order;
    std::vector< std::pair< std::pair< std::string, std::string > , unsigned int > > m_OR_order;
    
    //



   ClassDef(D3PDSelector,0);
};

//#endif

//#ifdef D3PDSelector_cxx


Bool_t D3PDSelector::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef D3PDSelector_cxx
