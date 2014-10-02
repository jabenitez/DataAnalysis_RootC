plotRatioSignalBackground2D(TString signal="/work/LS/histos/may29/NL1NJ1/Ms500_Mn200_Mg10000/llp_hist.root",TString background="/work/LS/histos/may29/NL1NJ1/T1_McAtNlo/llp_hist.root", TString histoname="hHtAllJetsAllLeptons_LowBetaMuon1Eta",Float_t ScaleSignal=0.14256 , Float_t ScaleBackground=0.194426 ){

  TFile sig(signal);
  TFile bkg(background);
  

  TH2D *htemp=(TH2D*)bkg.Get(histoname);
  TH2D *hb=(TH2D*)htemp->Clone();
  hb->SetName(histoname+"bkg");
  hb->Scale(ScaleSignal);
  TH2D *htemp2=(TH2D*)sig.Get(histoname);
  TH2D *hs=(TH2D*)htemp2->Clone();
  hs->SetName(histoname+"sig");
  hs->Scale(ScaleBackground);

  Int_t nXbins=hs->GetNbinsX();
  Int_t nYbins=hs->GetNbinsY();
  //  cout << " Nx " << nXbins <<   "  Ny " << nYbins << endl;
  Float_t FirstXBinEdge= hs->GetXaxis()->GetBinLowEdge(1);
  Float_t LastXBinEdge=hs->GetXaxis()->GetBinLowEdge(nXbins+1);
  Float_t FirstYBinEdge= hs->GetYaxis()->GetBinLowEdge(1);
  Float_t LastYBinEdge=hs->GetYaxis()->GetBinLowEdge(nXbins+1);
  TH2D *hRatio= new TH2D("hRatio","ratio of sig / sqrt(sig + bkg) ",nXbins,FirstXBinEdge,LastXBinEdge,nYbins,FirstYBinEdge,LastYBinEdge);
  TH2D *hRatioSB= new TH2D("hRatioSB","ratio of sig / sqrt(bkg) ",nXbins,FirstXBinEdge,LastXBinEdge,nYbins,FirstYBinEdge,LastYBinEdge);

  
  Float_t sigIntegral;
  Float_t bkgIntegral;
  Float_t edgeX,edgeY,ratio;

  Float_t sigIntegralSB;
  Float_t bkgIntegralSB;

  
  cout << "Entries sig " << hs->GetEntries() <<  "  Entries bkg "<< hb->GetEntries()<< endl;  
  for (Int_t i=1;i<nXbins;i++){
    for (Int_t j=1;j<nYbins;j++){
      sigIntegral=hs->Integral(i,nXbins,1,nYbins-j);
      bkgIntegral=hb->Integral(i,nXbins,1,nYbins-j);
      edgeX = hs->GetXaxis()->GetBinCenter(i);
      edgeY = hs->GetYaxis()->GetBinCenter(i);
      if ( bkgIntegral+sigIntegral > 0 ){
	ratio = sigIntegral/(TMath::Sqrt(bkgIntegral+sigIntegral));
	hRatio->SetBinContent(i,nYbins-j,ratio);
      }
      else 
	hRatio->SetBinContent(i,nYbins-j,0.0);


      sigIntegralSB=hs->Integral(i,nXbins,1,nYbins-j);
      bkgIntegralSB=hb->Integral(i,nXbins,1,nYbins-j);
      if ( bkgIntegral > 0 ){
	ratio = sigIntegral/(TMath::Sqrt(bkgIntegral));
	hRatioSB->SetBinContent(i,nYbins-j,ratio);
      }
      else 
	hRatioSB->SetBinContent(i,nYbins-j,0.0);

      
      //      cout << i << " )  "<< j << " )signal Integral " << sigIntegral  << "  Background integral "<< bkgIntegral << "   ratio  "<< ratio << "   Bin centre X " << edgeX<< "   Bin centre Y " << edgeY << endl; 
      
    }
  }

  Int_t xmax=0,ymax=0,zmax=0;
  hRatio->GetMaximumBin(xmax,ymax,zmax);
  cout << "max  " << hRatio->GetMaximum()  << endl;
  //<< " for a Ratio of " << hRatio->GetMaximumBin(xmax,ymax,zmax) << endl;
  //  cout << "xmax  " << xmax << "   ymax " << ymax <<  "zmax  " << zmax << endl;
  cout << "Signal percentage " << hs->Integral(xmax,nXbins,1,ymax)/hs->Integral() <<"  Bkg percentage   " <<  hb->Integral(xmax,nXbins,1,ymax)/hb->Integral()  << endl;
  cout << " HtCut " << hs->GetXaxis()->GetBinLowEdge(xmax) << "   EtaCut  " << hs->GetYaxis()->GetBinLowEdge(ymax) << endl;

  
  TCanvas C;
  hRatio->Draw("COLZ");
  C.SaveAs(histoname + "2DRatio.png");
  TCanvas C1;
  hRatioSB->Draw("COLZ");
  C1.SaveAs(histoname + "2DSBRatio.png");
  TCanvas C2;
  hs->Draw("COLZ");
  C2.SaveAs(histoname + "2DSig.png");
  TCanvas C3;
  hb->Draw("COLZ");
  C3.SaveAs(histoname + "2DBkg.png");

}
