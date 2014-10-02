CalculateScaleFactorFromFile(TString file="/work/LS/histos/may21/NL1NJ1/Ms1000_Mn600_Mg10000/llp_hist_Ms1000_Mn600_Mg10000.root",Double_t xsec=1.0, Double_t lumi=1.28){

  TFile f(file);
  TH1F h=(TH1F)f.Get("hCutFlow");
  Double_t nevents=h.GetBinContent(1);
  Double_t scalefactor=1.0/nevents*xsec*lumi;
  cout <<"Scale factor " << scalefactor << endl;
  //  cout << "nevents  " << nevents << endl;
}
