void makeallplots() {
  TFile f("../simple/susy_hist.root");

  TH1 *h;
  TKey *key;
  TCanvas *c0=new TCanvas("c0","Canvas o");
  TIter nextkey(gDirectory->GetListOfKeys());
  while (key = (TKey*)nextkey()) {
    obj = key->ReadObj();
    if (obj->IsA()->InheritsFrom("TH1")) {
      h = (TH1*)obj; 
      cout << h->GetName() << endl;
      h->Draw();
      c0->Modified();
      c0->Update(); 
      TString mdir= "output/";
      TString mext = ".eps";
      TString nname =  h->GetName();
     c0->SaveAs( mdir + nname + mext );
    }
  }
}
