//author: J.A.
#include <TChain.h>
#include <TROOT.h>
#include <TStopwatch.h>

void run() {

  //  TChain *c = new TChain("llp");
  TChain *c = new TChain("smp");
  gROOT->SetStyle("Plain");

  string line;
  ifstream myfile ("input.txt");

  if (myfile.is_open())
    {
      while ( myfile.good() )
	{
	  getline (myfile,line);
	  TString myline(line);
      	  cout << myline.Data() << endl;
	  c->Add(myline);
	}
      myfile.close();
    }

  else cout << "Unable to open file"; 

  cout << "Udone" << endl;; 
  c->Process("D3PDSelector.C+");
}
