plotRatioSignalBackground(TString signal="/work/LS/histos/may29/NL1NJ1/Ms500_Mn200_Mg10000/llp_hist.root",TString background="/work/LS/histos/may29/NL1NJ1/T1_McAtNlo/llp_hist.root", TString histoname="hHtAllJetsAllLeptons", Float_t ScaleSignal=0.14256 , Float_t ScaleBackground=0.194426 ){
  //plotRatioSignalBackground(TString signal="/work/LS/histos/may29/NL1NJ1/Ms500_Mn200_Mg10000/llp_hist.root",TString background="/work/LS/histos/may29/NL1NJ1/T1_McAtNlo/llp_hist.root", Float_t ScaleSignal=0.14256 , Float_t ScaleBackground=0.194426 , TString histoname="hMuLowBetaMuon1EtaAbs"){


  TFile sig(signal);
  TFile bkg(background);
  
  TH1F *htemp=(TH1F*)bkg.Get(histoname);
  TH1F *hb=(TH1F*)htemp->Clone();
  hb->SetName(histoname+"bkg");
  hb->Scale(ScaleSignal);

  TH1F *htemp2=(TH1F*)sig.Get(histoname);
  TH1F *hs=(TH1F*)htemp2->Clone();
  hs->SetName(histoname+"sig");
  hs->Scale(ScaleBackground);

  Int_t nbins=hs->GetNbinsX();
  
  Float_t FirstBinEdge= hs->GetBinLowEdge(1);
  Float_t LastBinEdge=hs->GetBinLowEdge(nbins+1);
  TH1F * hRatio= new TH1F("hRatio","ratio of sig / sqrt(sig + bkg) ",nbins,FirstBinEdge,LastBinEdge);
  TH1F * hRatioSB= new TH1F("hRatioSB","ratio of sig / sqrt(bkg) ",nbins,FirstBinEdge,LastBinEdge);

  Float_t sigIntegral;
  Float_t bkgIntegral;
  Float_t edge,ratio;

  Float_t sigIntegralSB;
  Float_t bkgIntegralSB;
  Float_t ratioSB;
  
  cout << "Entries sig " << hs->GetEntries() <<  "  Entries bkg "<< hb->GetEntries()<< endl;    
  
  for (Int_t i=1;i<nbins;i++){
    edge=hs->GetBinCenter(i);
    if ( histoname.Contains("Eta")){
      sigIntegral=hs->Integral(1,nbins-i);
      bkgIntegral=hb->Integral(1,nbins-i);
      if ( bkgIntegral+sigIntegral > 0 ){
	ratio = sigIntegral/(TMath::Sqrt(bkgIntegral+sigIntegral));
	hRatio->SetBinContent(nbins-i,ratio);
      }
      else 
	hRatio->SetBinContent(nbins-i,0.0);


      sigIntegralSB=hs->Integral(1,nbins-i);
      bkgIntegralSB=hb->Integral(1,nbins-i);
      if ( bkgIntegralSB > 0 ){
	ratioSB = sigIntegralSB/(TMath::Sqrt(bkgIntegralSB));
	hRatioSB->SetBinContent(nbins-i,ratioSB);
      }
      else 
	hRatioSB->SetBinContent(nbins-i,0.0);

    }
    else{
      sigIntegral=hs->Integral(i,nbins);
      bkgIntegral=hb->Integral(i,nbins);
      if ( bkgIntegral+sigIntegral > 0 ){
	ratio = sigIntegral/(TMath::Sqrt(bkgIntegral+sigIntegral));
	hRatio->SetBinContent(i,ratio);
      }
      else 
	hRatio->SetBinContent(i,0.0);

      sigIntegralSB=hs->Integral(i,nbins);
      bkgIntegralSB=hb->Integral(i,nbins);
      if ( bkgIntegralSB > 0 ){
	ratioSB = sigIntegralSB/(TMath::Sqrt(bkgIntegralSB));
	hRatioSB->SetBinContent(i,ratioSB);
      }
      else 
	hRatioSB->SetBinContent(i,0.0);

    }
    

    //    cout << i << " ) signal Integral " << sigIntegral  << "  Background integral "<< bkgIntegral << "   ratio  "<< ratio << "   Bin centre  " << edge<< endl; 

  } //   for (Int_t i=1;i<nbins;i++)
  //  cout << " done with optimization " << endl;
  cout << "max  " << hRatio->GetMaximum()  << "  for a cut of " << hRatio->GetBinCenter(hRatio->GetMaximumBin()) << endl;

  if (histoname.Contains("Eta"))
    cout << "ETASignal percentage " << hs->Integral(1,hRatio->GetMaximumBin())/hs->Integral() << "  Bkg percentage   " <<  hb->Integral(1,hRatio->GetMaximumBin())/hb->Integral()  << endl;
  else
    cout << "Signal percentage " << hs->Integral(hRatio->GetMaximumBin(),nbins)/hs->Integral() << "  Bkg percentage   " <<  hb->Integral(hRatio->GetMaximumBin(),nbins)/hb->Integral()  << endl;


  TCanvas C;
  hRatio->Draw();
  C.SaveAs(histoname+"1D"+"SignalOverSqrtSignalBackground.png");
  TCanvas C3;
  hRatioSB->Draw();
  C3.SaveAs(histoname+"1D"+"SignalOverSqrtBackground.png");
  TCanvas C1;
  hb->Draw();
  C1.SaveAs(histoname+"1D"+"Background.png");
  TCanvas C2;
  hs->Draw();
  C2.SaveAs(histoname+"1D"+"Signal.png");
}


