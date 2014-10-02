{
  
#include <iostream>
#include <iomanip>
#include <vector>
  gROOT->Reset();
  //  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  // open input file containing all information: root files, scale factors, cross-sections, etc
  ifstream inFile;
  char inputFilename[] = "eMuPlotter.in";
  inFile.open(inputFilename, ios::in);
  // if can't open input file, exit the code
  if (!inFile) {
    cerr << "Can't open input file " << inputFilename << endl;
    exit(1);
  }
  
  // initialize variables
  float effSkim;
  float branching;
  float branchingError;
  vector<string> inRootFiles;
  inRootFiles.clear();
  vector<string> inDirectories;
  inDirectories.clear();
  vector<string> inProcess;
  inProcess.clear();
  vector<float> inScaleFactor;
  inScaleFactor.clear();
  vector<float> inScaleFactorError;
  inScaleFactorError.clear();
  vector<float> x_section;
  x_section.clear();
  vector<float> theSkimmingEfficiency;
  theSkimmingEfficiency.clear();
  vector<float> theEventsAnalyzed;
  theEventsAnalyzed.clear();
  vector<float> theEventsPassing;
  theEventsPassing.clear();
  vector<float> theCumulativeEfficiency;
  theCumulativeEfficiency.clear();
  vector<int> theRelativeEff_numerator;
  theRelativeEff_numerator.clear();
  vector<int> theRelativeEff_denominator;
  theRelativeEff_denominator.clear(); 
  vector<string> theHistNameStrings;
  theHistNameStrings.clear();
  string inputString;
  string inputType;
  
  // grab all relevant information from the input file
  while (inFile >> inputType >> inputString) {
    if(inputType == "rootfile") {
      inRootFiles.push_back(inputString); // create a vector of root files containing histograms
    } else if(inputType == "directory") {
      inDirectories.push_back(inputString); // directory path within root file that contains the histograms
    } else if(inputType == "process") {
      inProcess.push_back(inputString); // name of the cut ... will be used to rename histograms
    } else if(inputType == "outputRootFileForNormalizedHistos") {
      string outputRootFileForNormalizedHistos = inputString; // name of output root file containing normalized histograms
    } else if(inputType == "outputRootFileForProbabilityHistos") {
      string outputRootFileForProbabilityHistos = inputString; // name of output root file containing probability histograms
    } else if(inputType == "outputLogFile") {
      string outputLogFile = inputString; // name of output log file (e.g. contains efficiency table)
    } else if(inputType == "scaleFactor") {
      inScaleFactor.push_back(atof(inputString.c_str())); // scale factors used to recalculate efficiencies for a each cut
    } else if(inputType == "scaleFactorError") {
      inScaleFactorError.push_back(atof(inputString.c_str())); // scale factor error used to recalculate efficiency for each cut
    } else if(inputType == "luminosity") {
      float lumi = atof(inputString.c_str()); // luminosity
      string lumi_string = inputString;
    } else if(inputType == "effectiveXsection") {
      x_section.push_back(atof(inputString.c_str())); // cross-section
    } else if(inputType == "branchingRatio") {
      branching = atof(inputString.c_str()); // branching ratio (should only be non-zero for signal)
    } else if(inputType == "branchingRatioError") {
      branchingError = atof(inputString.c_str()); // branching ratio error
    } else if(inputType == "doData"){
      string do_data = inputString;
    } else {
      cerr << "Incorrect input type " << inputType << endl; // exit code if unwanted input is specified
      exit(1);
    }
  }
  
  // create an output log file that will contain the cut flow efficiency table
  ofstream outFile;
  char outputFilename[] = outputLogFile.c_str();
  outFile.open(outputFilename, ios::out);
  // if output log file cannot be opened, exit the code
  if (!outFile) {
    cerr << "Can't open output file " << outputFilename << endl;
    exit(1);
  } else {
    outFile << "" << endl;
    outFile << "======= Openning Text File =======" << endl;
    outFile << "" << endl;
    outFile << "" << endl;
  }
  
  // initialization of variables
  vector<double> maxHistContents;
  maxHistContents.clear();
  TList* HistList = new TList();
  int nHistList=0;
  TList* HistList2 = new TList();
  int nHistList2=0;
  
  // loop over root files
  for(int j=0;j<inRootFiles.size();j++) {
    
    TFile *example1 = new TFile(inRootFiles.at(j).c_str());
    // printout information to log file
    outFile << "Name of Root File #" << (j+1) << " : " << inRootFiles.at(j) << endl;
    //outFile << "Name of Directory #" << (j+1) << " : " << inDirectories.at(j) << endl;
    outFile << "Name of Process #" << (j+1) << "   : " << inProcess.at(j) << endl;
    outFile << "" << endl;
    
    float sk_eff=0;
    float cumulativeEff_denominator = 0;
    float cumulativeEff_numerator = 0;
    
    TH1D* n_events;  
    if(inProcess.at(j) == "Data"){n_events = hCutFlow;}
    else{n_events = hCutFlow;}
    if(n_events->GetBinContent(1) == 0) {
      cerr << "'Events' histogram contains zero entries in bin 1: 0 events were analyzed ..." << endl;
      exit(1);
    } else {
      //calculate skimming efficiency
      theRelativeEff_denominator.push_back(n_events->GetBinContent(2));
      cumulativeEff_denominator = (float)n_events->GetBinContent(1); 
      theEventsAnalyzed.push_back((float)n_events->GetBinContent(1)); // denominator for calculation of skimming eff.
      theEventsPassing.push_back((float)n_events->GetBinContent(2)); // numerator for calculation of skimming eff.
      outFile<<"theEventsAnalyzed "<<(float)n_events->GetBinContent(1)<<" theEventsPassing "<<(float)n_events->GetBinContent(2)<<endl; 
      sk_eff = ((float)n_events->GetBinContent(2)) / ((float)n_events->GetBinContent(1));
      theSkimmingEfficiency.push_back(sk_eff); // skimming eff.
    }
    
    if(sk_eff != 0) outFile<<"SKIMMING EFFICINECY : "<<sk_eff<<"========="<<endl;
    
    //example1.cd(inDirectories.at(j).c_str()); // cd to appropriate directory
    TDirectory *current_sourcedir = gDirectory;
    TIter nextkey( current_sourcedir->GetListOfKeys() );
    TKey *key;
    int num=0;
    // loop over keys(ROOT objects) within the root file
    while((key = (TKey*)nextkey())) {
      TObject *obj = key->ReadObj();
      // only consider 1D histograms within the root file
      if ( obj->IsA()->InheritsFrom( "TH1" ) ) {
	string histname = obj->GetName();
        string histoName;
	TH1 *hobj = (TH1*)obj;
	
	// grab all 1D histograms within the root file and create a TList (i.e. vector) containing them to be used later
	if( (hobj->GetYaxis()->GetNbins() == 1)) {
	  // rename hisrogram
          if(histname == "hMET"){cumulativeEff_numerator = (float)hobj->Integral();}
	  histoName = histname + "_" + inProcess.at(j);
	  outFile<< histoName.c_str() <<endl;
          string probability = "pro";
	  string histoNameName = histoName + "_" + probability;
	  h1 = new TH1F(histoName.c_str(),histoName.c_str(),hobj->GetXaxis()->GetNbins(),
			hobj->GetXaxis()->GetXmin(),hobj->GetXaxis()->GetXmax());
	  for(int i=0; i<=(h1->GetXaxis()->GetNbins() + 1); i++) {h1->SetBinContent(i,hobj->GetBinContent(i));}
	  h2 = new TH1F(histoNameName.c_str(),histoNameName.c_str(),hobj->GetXaxis()->GetNbins(),
			hobj->GetXaxis()->GetXmin(),hobj->GetXaxis()->GetXmax());
	  for(int i=0; i<=(h2->GetXaxis()->GetNbins() + 1); i++) {h2->SetBinContent(i,hobj->GetBinContent(i));}
	  HistList->Add(h1);
	  nHistList++;
	  HistList2->Add(h2);
	  nHistList2++;
	  theHistNameStrings.push_back(histname); // store default names - used later in the code
	  double max = 0;
	  for(int i=0; i<=(h1->GetXaxis()->GetNbins() + 1); i++) { if(h1->GetBinContent(i) > max) {max = h1->GetBinContent(i);} }
	  if(j==0) {maxHistContents.push_back(max);}
	  else {if(max > maxHistContents.at(num)) {maxHistContents.at(num) = max;}} // determine bin with largest entries
	  num++;
	}
      }
    }
    
    float cumulativeEff = cumulativeEff_numerator/cumulativeEff_denominator;
    theCumulativeEfficiency.push_back(cumulativeEff);
    
    outFile<<"Number of Historgams : "<<num<<endl;
    outFile<<"The Cumulative Efficiency: "<<cumulativeEff<<endl<<endl<<"========================= Moving on to next sample  ======================="<<endl;
    
    // make sure that each root file contains the same histograms
    if(j==0) {int nHistos=num;}
    else {if(num != nHistos) {cerr << "ERROR: Input Root Files DO NOT have the same histograms!!" << endl; exit(1);} }
  }
  
  std::cout<<"NUM "<<nHistList<<std::endl; 
  if(( nHistList % inRootFiles.size() == 0 )) {
    int k=0; 
    bool data = false;
    if(do_data=="True"){data=true;}
    for(int i=0; i<nHistList; i++) {
      string histname = HistList->At(i)->GetName(); 
      if(i > 0 && i % nHistos == 0){k++; std::cout<<"K "<<k<<std::endl; outFile<<"HistoName "<<histname<<" K "<<k<<endl;} 
      string histoEffyName = "hEffy_" + inProcess.at(0);
      TH1F *h = (TH1F*)HistList->At(i);
      h->Sumw2();
      // Calculate relative efficiency
      float numerator = (float)h->Integral(0,(h->GetXaxis()->GetNbins()+1));
      float denominator = theEventsPassing.at(k);
      float theRelativeEfficiency = numerator / denominator;
      float efferror = sqrt(theRelativeEfficiency*(1.-theRelativeEfficiency)/denominator); // binomial uncertainty
      /* binomial uncertainties do not work when the efficiency gets close to 1 or 0 (e.g. efficiency cannot 
	 be 99 +- 2 because the efficiency cannot be e.g. 101) ... in these cases, use bayesian */
      
      if(((theRelativeEfficiency + efferror) >= 1.) || ((theRelativeEfficiency - efferror) <= 0.)){
	TH1F* theNumHisto = new TH1F("theNumHisto","theNumHisto",1,0,1);
	theNumHisto->SetBinContent(1,numerator);
	theNumHisto->Sumw2();
	TH1F* theDenHisto = new TH1F("theDenHisto","",1,0,1);
	theDenHisto->SetBinContent(1,denominator);
	theDenHisto->Sumw2();
	TGraphAsymmErrors* bayesEff = new TGraphAsymmErrors();
	bayesEff->BayesDivide(theNumHisto,theDenHisto,"b");
	if(bayesEff->GetErrorYhigh(0) > bayesEff->GetErrorYlow(0)) {efferror = bayesEff->GetErrorYhigh(0);}
	else {efferror = bayesEff->GetErrorYlow(0);}
	delete theNumHisto;
	delete theDenHisto;
	delete bayesEff;
      }
      if(theRelativeEfficiency == 1.0) {efferror = 0;}
      // recalculate efficiencies and errors incorporating scale factors
      efferror = sqrt(pow(efferror/theRelativeEfficiency,2.0) + pow((inScaleFactorError.at(k)/inScaleFactor.at(k)),2.0));
      theRelativeEfficiency = theRelativeEfficiency * inScaleFactor.at(k);
      efferror = efferror * theRelativeEfficiency;
      
      if(h->Integral(0,(h->GetXaxis()->GetNbins()+1)) > 0) {
	if(data){ 
	  if (k>=1){
	    outFile<<"Integral "<<h->Integral()<<" theEventsAnalyzed "<<theEventsAnalyzed.at(k)<<" x-sec "<<x_section.at(k)<<endl;
	    float sf = (1.0 / theEventsAnalyzed.at(k)) * x_section.at(k) * lumi;
	    outFile<<"SF " <<sf<<endl;
	    h->Scale(sf);
	    outFile<<"Integral "<<h->Integral()<<endl;
	  }
	} else {h->Scale((1.0 / theEventsAnalyzed.at(k)) * x_section.at(k) * lumi);}
      }
      
      TH1F* hEffy = new TH1F(histoEffyName.c_str(),histoEffyName.c_str(),
                             h->GetXaxis()->GetNbins(),h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
      for(int ibin=0; ibin<=(hEffy->GetXaxis()->GetNbins() + 1); ibin++) {
        hEffy->SetBinContent(ibin,inScaleFactor.at(k));
        hEffy->SetBinError(ibin,efferror);
      }
      hEffy->Sumw2();
      
      if(data){
	if(k>=1){h->Sumw2(); h->Multiply(h,hEffy,1,1,"");}
      } else {h->Sumw2(); h->Multiply(h,hEffy,1,1,"");}
      
      h->SetName(histname.c_str());
      string YtitleString = "N / " + lumi_string + " pb^{-1}";
      h->GetYaxis()->SetTitle(YtitleString.c_str());
      h->GetYaxis()->SetTitleSize(0.06);
      h->GetYaxis()->SetTitleFont(62);
      h->GetYaxis()->CenterTitle();
      h->GetYaxis()->SetLabelSize(0.05);
      h->GetYaxis()->SetLabelFont(62);
      h->GetXaxis()->SetTitle(theHistNameStrings.at(i).c_str());
      h->GetXaxis()->SetTitleSize(0.06);
      h->GetXaxis()->SetTitleFont(62);
      h->GetXaxis()->CenterTitle();
      h->GetXaxis()->SetLabelSize(0.05);
      h->GetXaxis()->SetLabelFont(62);
      TFile *hfile = (TFile*)gROOT->FindObject(outputRootFileForNormalizedHistos.c_str());
      if (hfile) {hfile->Close();}
      hfile = new TFile(outputRootFileForNormalizedHistos.c_str(),"UPDATE");
      h->Write();
      hfile->Close();
      
      delete hEffy;
      delete h;
    }
  }
  
  std::cout << "I'm here ..." << std::endl;
  if(( nHistList2 % inRootFiles.size() == 0 )) {
    for(int i=0; i<nHistList2; i++) {
      string histname = HistList2->At(i)->GetName();
      TH1F *hp = (TH1F*)HistList2->At(i);
      
      hp->Sumw2();
      if(hp->Integral(0,(hp->GetXaxis()->GetNbins()+1)) > 0) {hp->Scale(1.0 / hp->Integral(0,(hp->GetXaxis()->GetNbins()+1)));}
      hp->SetName(histname.c_str());
      hp->GetYaxis()->SetTitle("a.u.");
      hp->GetYaxis()->SetTitleSize(0.06);
      hp->GetYaxis()->SetTitleFont(62);
      hp->GetYaxis()->CenterTitle();
      hp->GetYaxis()->SetLabelSize(0.05);
      hp->GetYaxis()->SetLabelFont(62);
      hp->GetXaxis()->SetTitle(theHistNameStrings.at(i).c_str());
      hp->GetXaxis()->SetTitleSize(0.06);
      hp->GetXaxis()->SetTitleFont(62);
      hp->GetXaxis()->CenterTitle();
      hp->GetXaxis()->SetLabelSize(0.05);
      hp->GetXaxis()->SetLabelFont(62);
      TFile *hfile = (TFile*)gROOT->FindObject(outputRootFileForProbabilityHistos.c_str());
      if (hfile) {hfile->Close();}
      hfile = new TFile(outputRootFileForProbabilityHistos.c_str(),"UPDATE");
      hp->Write();
      hfile->Close();
      delete hp;
      
    }
  }
  
}
