{
  
#include <iostream>
#include <iomanip>
#include <vector>
#include <stdio.h>
#include <string.h>
  
  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  // open input file containing all information: root files, scale factors, cross-sections, etc
  ifstream inFile;
  char inputFilename[] = "eMu_Plotter_MCandDATA.in";
  inFile.open(inputFilename, ios::in);
  // if can't open input file, exit the code
  if (!inFile) {
    cerr << "Can't open input file " << inputFilename << endl;
    exit(1);
  }
  
  // initialize variables
  vector<string> inRootFiles;
  inRootFiles.clear();
  vector<string> inProcess;
  inProcess.clear();
  vector<string> theHistNameStrings;
  theHistNameStrings.clear();
  string inputString;
  string inputType;
  
  // grab all relevant information from the input file
  while (inFile >> inputType >> inputString) {
    if(inputType == "rootfile") {
      inRootFiles.push_back(inputString); // create a vector of root files containing histograms
    } else if(inputType == "process") {
      inProcess.push_back(inputString); // name of the cut ... will be used to rename histograms
    } else if(inputType == "outputRootFile") {
      string outputRootFile = inputString; // name of output root file containing normalized histograms
    } else if(inputType == "luminosity") {
      float lumi = atof(inputString.c_str()); // luminosity
      string lumi_string = inputString;
    } else if(inputType == "doData"){
      string do_data = inputString;
    } else {
      cerr << "Incorrect input type " << inputType << endl; // exit code if unwanted input is specified
      exit(1);
    }
  }
  
  ofstream outFile;
  outFile.open("outputFile.log", ios::out);
  
  // initialization of variables
  HistList = new TList();
  int nHistList=0;
  int num=0;
  // loop over root files
  for(int j=0;j<inRootFiles.size();j++) {
    TFile *example1 = new TFile(inRootFiles.at(j).c_str()); // open root file
    TDirectory *current_sourcedir = gDirectory;
    TIter nextkey( current_sourcedir->GetListOfKeys() );
    TKey *key;
    int nhistos=0;
    // loop over keys(ROOT objects) within the root file
    while((key = (TKey*)nextkey())) {
      TObject *obj = key->ReadObj();
      // only consider 1D histograms within the root file
      if ( obj->IsA()->InheritsFrom( "TH1" ) ) {
        string histname = obj->GetName();       
        size_t found = histname.find(inProcess.at(j).c_str());
        string theProcess = inProcess.at(j).c_str();
        
        if(found!=string::npos){
	  
	  TH1 *hobj = (TH1*)obj;
	  // grab all 1D histograms within the root file and create a TList (i.e. vector) containing them to be used later
	  if( (hobj->GetYaxis()->GetNbins() == 1) ) {
	    // rename histogram
	    string new_theProcess = "_" + theProcess;
	    histname.erase(found-1, new_theProcess.size());
	    string histoName = histname;
	    TH1F* h1 = new TH1F(histoName.c_str(),histoName.c_str(),hobj->GetXaxis()->GetNbins(),
				hobj->GetXaxis()->GetXmin(),hobj->GetXaxis()->GetXmax());
	    for(int i=0; i<=(h1->GetXaxis()->GetNbins() + 1); i++) {
	      double scf = 1.0;
	      if(inProcess.at(j) == "DYtautau"){h1->SetBinContent(i,(hobj->GetBinContent(i)*scf));}
	      else{h1->SetBinContent(i,(hobj->GetBinContent(i)));} 
	    }
	    if(inProcess.at(j) == "Data"){
	      if(histname == "h_trans_eff_mass_" || histname == "h_eff_mass_high_" ){
		for(int i=40; i<= 240; i++) {h1->SetBinContent(i,0);} 
	      }
	    }
	    if(histname == "h_trans_eff_mass")  {
	      outFile<<" "<<std::endl;
	      outFile<<"====== Process: "<<theProcess<<std::endl;
	      outFile<<" "<<std::endl;
	      outFile<<"Number of Events M(eff) > 250 GeV : "<<h1->Integral(50, 240)<<std::endl;
	      outFile<<"Number of Events M(eff) > 300 GeV : "<<h1->Integral(60, 240)<<std::endl;
	      outFile<<"Number of Events M(eff) > 350 GeV : "<<h1->Integral(70, 240)<<std::endl;
	      outFile<<"Number of Events M(eff) > 400 GeV : "<<h1->Integral(80, 240)<<std::endl;
	      outFile<<"Number of Events M(eff) > 450 GeV : "<<h1->Integral(90, 240)<<std::endl;
	      outFile<<"Number of Events M(eff) > 600 GeV : "<<h1->Integral(120, 240)<<std::endl;
	    }
	    if(histname == "h_eff_mass_high" || histname == "h_trans_eff_mass")  {h1->Rebin(4);} // example for rebining.....
	    if(histname == "h_met_high" || histname == "h_muon_phi" || histname == "h_electron_phi")  {h1->Rebin(2);}
	    if(histname == "h_pzeta_vis")  {h1->Rebin(5);}
	    if(histname.find("Jet") != string::npos || histname.find("dR") != string::npos || histname.find("Lep") != string::npos || histname.find("pT") != string::npos  || histname.find("Eta") != string::npos || histname.find("Mass") != string::npos || histname.find("MET") != string::npos|| histname.find("Ht") != string::npos )  {
	      h1->Rebin(5);
	    }
	    
	    if(histoName != "h_n_event_cands" && histoName != "h_cut_flow_raw" && histoName != "h_cut_flow" && histoName != "h_n_events" && histoName != "h_n_events_weighted"){ 
	      HistList->Add(h1);
	      nHistList++;
	      nhistos++;
	      theHistNameStrings.push_back(histname);
              num++;
	    }
	  }
	}
      }
    }
    if(j==0) {int nHistos=num;}
  }
  
  if(( nHistList % inRootFiles.size() == 0 )) {
    for(int i=0; i<nHistos; i++) {
      TCanvas *c = new TCanvas(theHistNameStrings.at(i).c_str(), theHistNameStrings.at(i).c_str(), 600, 600);
      c->Range(-6.658292,-0.1855746,66.50754,1.231785);
      c->SetFillColor(0);
      c->SetBorderMode(0);
      c->SetBorderSize(3);
      c->SetTickx(1);
      c->SetTicky(1);
      c->SetLeftMargin(0.1593407);
      c->SetRightMargin(0.0206044);
      c->SetBottomMargin(0.1309298);
      c->SetFrameFillStyle(0);
      c->SetFrameBorderMode(0);
      c->SetFrameFillStyle(0);
      c->SetFrameBorderMode(0);
      c->cd();
      
      TH1F* h; TH1F* hh; TH1F* hhh; TH1F* sig;
      THStack *hs = new THStack(theHistNameStrings.at(i).c_str(),theHistNameStrings.at(i).c_str());
      TLegend *legend = new TLegend(0.6,0.80,0.95,0.95,NULL,"brNDC");
      TLegend *legend_allBG = new TLegend(0.6,0.65,0.95,0.80,NULL,"brNDC");  
      //      TLegend *legend_allBG = new TLegend(0.10,0.72,0.91,0.85,NULL,"brNDC");  
      bool data = false; 
      
      if(do_data=="True"){data=true;}
      
      if(data){
	string histname = HistList->At(i)->GetName();
	h = (TH1F*)HistList->At(i);
	h->SetName(histname.c_str());
	hhh = new TH1F("hhh","hhh",h->GetXaxis()->GetNbins(),h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
	for(int ijkk=0; ijkk<=(hhh->GetXaxis()->GetNbins() + 1); ijkk++) {
	  hhh->SetBinContent(ijkk,h->GetBinContent(ijkk)); 
	}
    	
	h->SetMarkerStyle(20);
	h->Sumw2();
	h->Draw("ep");      
	h->SetLineColor(1);
        legend->AddEntry(h,inProcess.at(0).c_str(), "lep");
      }
      int whichHisto=0;
      int k=0;
      int sty=0;
      if(data){k=1;}
      TH1F* allBG = new TH1F("allBG","allBG",h->GetXaxis()->GetNbins(),h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
      for(int bg=0; bg<=(hhh->GetXaxis()->GetNbins() + 1); bg++) {
	allBG->SetBinContent(bg,0.0);
      }
      allBG->SetFillStyle(3004);
      allBG->SetFillColor(kRed+2);
      for(int j=k;j<inRootFiles.size();j++) {
	whichHisto=j*nHistos; 
	int a = 0;
	bool foundHisto=false;
	if(theHistNameStrings.at(i) == theHistNameStrings.at(i+whichHisto)) {foundHisto=true; a=i;}
	else if(theHistNameStrings.at(i) != theHistNameStrings.at(i+whichHisto)){
	  for(a = 0; a < nHistos; a++){
	    if(theHistNameStrings.at(i) == theHistNameStrings.at(a+whichHisto)){
	      foundHisto=true;
	      break;
	    }
	  }
	}
        int is_sig = 0;
	string hist2name = HistList->At(a+whichHisto)->GetName();
	if(foundHisto){
	  hh = (TH1F*)HistList->At(a+whichHisto);
	  hh->SetName(hist2name.c_str());
          string processlegend = "";
          if(inProcess.at(j) == "Data"){processlegend = inProcess.at(j);}
	  if(inProcess.at(j) == "ttbar"){
	    allBG->Add(hh);
	    hh->SetLineStyle(1);
	    hh->SetLineWidth(3);
	    hh->SetLineColor(kGreen-6);
	    hh->SetFillColor(kGreen-6);
            processlegend = "t#bart";
	  } else if (inProcess.at(j) == "Ztautau"){
	    hh->SetLineStyle(1);
	    hh->SetLineWidth(3);
	    hh->SetLineColor(kRed+2);
	    hh->SetFillColor(kRed+2);
            processlegend = "Z#rightarrow#tau#tau";
          } else if (inProcess.at(j) == "DYtautau"){
            hh->SetLineStyle(1);
            hh->SetLineWidth(3);
            hh->SetLineColor(kCyan-6);
            hh->SetFillColor(kCyan-6);
            processlegend = "DY#rightarrow#tau#tau";
            allBG->Add(hh);
	  } else if (inProcess.at(j) == "zmumu"){
	    hh->SetLineStyle(1);
	    hh->SetLineWidth(3);
	    hh->SetLineColor(kMagenta-10);
	    hh->SetFillColor(kMagenta-10);
	    processlegend = "Z#rightarrow#mu#mu+jets";
	    allBG->Add(hh);
	  } else if(inProcess.at(j) == "DibosonZZ") {
	    hh->SetLineStyle(1);
	    hh->SetLineWidth(3);
	    hh->SetLineColor(kRed-6);
	    hh->SetFillColor(kRed-6);
            processlegend = "ZZ";
            allBG->Add(hh);
	  } else if(inProcess.at(j) == "DibosonWZ") {
	    hh->SetLineStyle(1);
	    hh->SetLineWidth(3);
	    hh->SetLineColor(kBlue+2);
	    hh->SetFillColor(kBlue+2);
            processlegend = "WZ";
            allBG->Add(hh);
          } else if(inProcess.at(j) == "Diboson") {
            hh->SetLineStyle(1);
            hh->SetLineWidth(3);
            hh->SetLineColor(kYellow-6);
            hh->SetFillColor(kYellow-6);
            processlegend = "Diboson";
            allBG->Add(hh);
          } else if(inProcess.at(j) == "Wjets") {
            hh->SetLineStyle(1);
            hh->SetLineWidth(3);
            hh->SetLineColor(kMagenta+3);
            hh->SetFillColor(kMagenta+3);
            processlegend = "W+jets";
            allBG->Add(hh);
	  } else if(inProcess.at(j) == "Wgamma") {
            hh->SetLineStyle(1);
            hh->SetLineWidth(3);
            hh->SetLineColor(kOrange+7);
            hh->SetFillColor(kOrange+7);
            processlegend = "W+#gamma";
            allBG->Add(hh);
	  } else if(inProcess.at(j) == "QCD") {
	    hh->SetLineStyle(1);
	    hh->SetLineWidth(3);
	    hh->SetLineColor(kRed-3);
	    hh->SetFillColor(kRed-3);
	    processlegend = "QCD";
            allBG->Add(hh); 
          } else if(inProcess.at(j) == "Ms500_Mn200_Mg10000") {
	    hh->SetLineStyle(1);
	    hh->SetLineWidth(3);
	    hh->SetLineColor(kMagenta+0);
            processlegend = "L Susy Ms500 Mn200 Mg10000";
            is_sig = 1;
            sig = hh;
	  } else if(inProcess.at(j) == "Ms1000_Mn600_Mg10000") {
            hh->SetLineStyle(1);
            hh->SetLineWidth(3);
            hh->SetLineColor(kBlue+1);
            processlegend = "L Susy Ms1000 Mn600 Mg10000";
            is_sig = 1;
            sig = hh;
	  } else if(inProcess.at(j) == "Ms1000_Mn600_Mg2000") {
	    hh->SetLineStyle(1);
	    hh->SetLineWidth(3);
	    hh->SetLineColor(kRed+1);
            processlegend = "L Susy Ms1000 Mn600 Mg2000";
            sig= hh;
            is_sig = 1;
	  } else if(inProcess.at(j) == "Zprime_tautau_SSM875") {
            hh->SetLineStyle(1);
            hh->SetLineWidth(3);
            hh->SetLineColor(kGreen+3);
            processlegend = "Z'(875)#rightarrow#tau#tau";
            is_sig = 1;
            sig = hh;
	  } else if(inProcess.at(j) == "Zprime_tautau_SSM1000") {
            hh->SetLineStyle(1);
            hh->SetLineWidth(3);
            hh->SetLineColor(kOrange+10);
            sig= hh;
            is_sig = 1;
            processlegend = "Z'(1000)#rightarrow#tau#tau";
          } else if(inProcess.at(j) == "Zprime_tautau_SSM1250") {
            hh->SetLineStyle(1);
            hh->SetLineWidth(3);
            hh->SetLineColor(kGreen+4);
            processlegend = "Z'(1250)#rightarrow#tau#tau";
            sig= hh;
            is_sig = 1;
	  }else {
	    hh->SetLineStyle(7);
	    hh->SetLineWidth(3);
	  }
	  
	  if(is_sig != 1) {
	    hs->Add(hh);
	    legend->AddEntry(hh,processlegend.c_str(), "f");
	  }else{
	    legend_allBG->AddEntry(hh,processlegend.c_str(), "f");
	  }
	}
        
	if(data && (j-1)==0){
	  hhh->SetMarkerStyle(20);
	  hhh->SetMarkerSize(1.5);
	  hhh->SetLineColor(1);
	  hhh->Sumw2();
	  hs->Draw("");
	} else if (data && (j-1) > 0 && is_sig == 0){
	  hs->Draw("fsame");hhh->Draw("e1psame"); 
        }
	else if (!data && j==0 && is_sig == 0){hs->Draw("");}
	else if (is_sig == 0){hs->Draw("fsame");}
        if(is_sig == 1){
	  sig->Draw("same"); 
	  allBG->Draw("e2same");
	}
	sty++;
      }
      legend->SetTextFont(42);
      legend->SetLineColor(1);
      legend->SetLineStyle(1);
      legend->SetLineWidth(1);
      legend->SetFillColor(0);
      legend->SetFillStyle(1001);
      legend->SetBorderSize(0);
      legend->SetFillColor(kWhite);
      legend->AddEntry(allBG, "All BGs (stat uncertainties)", "f");
      legend->Draw();
      legend_allBG->SetTextFont(42);
      legend_allBG->SetLineColor(1);
      legend_allBG->SetLineStyle(1);
      legend_allBG->SetLineWidth(1);
      legend_allBG->SetFillColor(0);
      legend_allBG->SetFillStyle(1001);
      legend_allBG->SetBorderSize(0);
      legend_allBG->SetFillColor(kWhite);
      legend_allBG->Draw();
      TFile *hfile = (TFile*)gROOT->FindObject(outputRootFile.c_str());
      if (hfile) {hfile->Close();}
      hfile = new TFile(outputRootFile.c_str(),"UPDATE");
      c->Write();
      TString savename="myplots/"+histname+".png";
      cout << "save name is : " << savename << endl;
      c->SaveAs(savename);
      hfile->Close();
      c->Close();

      if(histname == "h_el_pt"){
        TCanvas *c1 = new TCanvas("c1","example",600,600);
	TPad *pad1 = new TPad("pad1","pad1",0,0.35,1,1);
	pad1->SetBottomMargin(0);
	pad1->Draw();
	pad1->cd();
	
	THStack* hs_clone = hs->DrawClone();
	hs_clone->SetMinimum(-1);
	TH1F *h3= hhh->DrawCopy("same");
	allBG->Draw("e2same");
	
	c1->cd();
	TPad *pad2 = new TPad("pad2","pad2",0,0,1,0.28);
	pad2->SetTopMargin(0);
	pad2->Draw();
	pad2->cd();
	hhh->Sumw2();
	hhh->SetStats(0);
	hhh->Divide(allBG);
	hhh->SetMarkerStyle(20);
	hhh->Draw("ep");
	pad1->cd();
	legend->Draw();
	c1->cd();
	
	c1->SaveAs("ratio.root");
	
	c->cd();
	
      }
      
    }
  }
}

