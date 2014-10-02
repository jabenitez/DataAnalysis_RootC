drawtgraphs(TString mfile="susy_hist.root"){
  //Open root file                                                                                
  TFile *f1 = new TFile(mfile);
  TCanvas *cv = new TCanvas;
  TIter next(f1->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)next())) {
    //    file://key->GetClassName() returns the name of the object class
    TObject *obj = key->ReadObj(); 
    if (obj->InheritsFrom("TGraph")){
      TGraph *tg = static_cast<TGraph*>(obj);
      tg->Draw("AP");
      TString tname=key->GetName();
      cv->SaveAs( tname + "file.eps");
    } 
  } 
  
}
