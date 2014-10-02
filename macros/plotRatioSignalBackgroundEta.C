plotRatioSignalBackgroundEta(TString signal="/work/LS/histos/may23/NL1NJ1/Ms500_Mn200_Mg10000/llp_hist.root",TString background="/work/LS/histos/may23/NL1NJ1/T1_McAtNlo/llp_hist.root",TString histoname="hMuLowBetaMuon1Eta"){

  TFile sig(signal);
  TFile bkg(background);
  
  TH1F *htemp=(TH1F*)bkg.Get(histoname);
  TH1F *hb=(TH1F*)htemp->Clone();
  hb->SetName(histoname+"bkg");
  TH1F *htemp2=(TH1F*)sig.Get(histoname);
  TH1F *hs=(TH1F*)htemp2->Clone();
  hs->SetName(histoname+"sig");
  //  TH1F hb=(TH1F)bkg.Clone(histoname);
  Int_t nbins=hs->GetNbinsX()/2;
  Float_t FirstBinEdge= hs->GetBinLowEdge(1);
  Float_t LastBinEdge=hs->GetBinLowEdge(2*nbins+1);
  TH1F * hsRebinned=new TH1F("hsRebinned","hsRebinned",nbins,0.0,LastBinEdge);
  TH1F * hbRebinned=new TH1F("hbRebinned","hbRebinned",nbins,0.0,LastBinEdge);
  TH1F * hRatio=new TH1F("hRatio","ratio of sig / \sqrt(sig + bkg) ",nbins,0.0,LastBinEdge);
  for (Int_t i=0;i<nbins;i++){
    //    cout << "i " << i << "  lastbinedge " << LastBinEdge<<"  nbis" << nbins << "  hs->GetBinContent(i)" << hs->GetBinContent(i)  <<  "   hb->GetBinContent(i) "<< hb->GetBinContent(i) << endl;
    hsRebinned->SetBinContent(nbins-i,hs->GetBinContent(i)+hs->GetBinContent(2*nbins-i));
    hbRebinned->SetBinContent(nbins-i,hb->GetBinContent(i)+hb->GetBinContent(2*nbins-i));
}
  
  
  for (Int_t i=1;i<nbins;i++){
    //      for (Int_t i=1;i<120;i++){
    Float_t sigIntegral=hsRebinned->Integral(1,nbins-i+1)/hsRebinned->Integral();
    Float_t bkgIntegral=hbRebinned->Integral(1,nbins-i+1)/hbRebinned->Integral();
    Float_t edge = hsRebinned->GetBinCenter(i);
    if ( bkgIntegral+sigIntegral > 0 ){
      Float_t ratio = sigIntegral/(TMath::Sqrt(bkgIntegral+sigIntegral));
      hRatio->SetBinContent(nbins-i+1,ratio);
    }
    else 
      hRatio->SetBinContent(nbins-i+1,0.0);
    //    cout << i << " ) signal Integral " << sigIntegral  << "  Background integral "<< bkgIntegral << "   ratio  "<< ratio << "   Bin centre  " << edge<< endl; 
    
  }
  //  cout << " done with optimization " << endl;
  cout << "max  " << hRatio->GetMaximum()  << "  for a cut of " << hRatio->GetBinCenter(hRatio->GetMaximumBin()) << "  at bin "<<  hRatio->GetMaximumBin()<< endl;
  cout << "Signal percentage " << hsRebinned->Integral(1,hRatio->GetMaximumBin())/hs->Integral() <<"  Bkg percentage   " <<  hbRebinned->Integral(1,hRatio->GetMaximumBin())/hb->Integral()  << endl;
  
  TCanvas C;
  hRatio->Draw();
  C.SaveAs(histoname+"1D"+"SignalOverSqrtSignalBackground.png");
  TCanvas C1;
  hbRebinned->Draw();
  C1.SaveAs(histoname+"1D"+"Background.png");
  TCanvas C2;
  hsRebinned->Draw();
  C2.SaveAs(histoname+"1D"+"Signal.png");
}
