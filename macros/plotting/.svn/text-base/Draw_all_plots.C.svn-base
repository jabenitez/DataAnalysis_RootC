//////////////////
//
// Draws all histograms in a root file in a postscript file.
// 
//
//////////////////


void Draw_all_plots(TString file, TString printEPSstring = "" )
{
  int MyRed = 102; int MyGreen = 106; int MyBlue = 101;
  int MyCol = MyRed;
  
  int CanX = 2; int CanY = 2;

  Bool_t _printEPS = ( printEPSstring == "printEPS" || printEPSstring == "PrintEPS" );

  TString FullRootName = file;
  std::string findroot(file.Data());
  Int_t pos = findroot.find(".root");
  if (pos > 1 && pos < file.Sizeof())
    file.Remove(pos);
  else
    FullRootName = file+".root";

  TFile f(FullRootName.Data());
  TString psname=file+".ps";
  
  cout << "\nHistogramming file:     " << FullRootName << endl;
  cout << "Postscript output file: " << psname.Data() << endl<<endl;
  
  gROOT->SetStyle("Plain");
  gStyle->SetTitleBorderSize(0);
  gStyle->SetPalette(1);
  
  TCanvas *Can    = new TCanvas();
  Can->Divide(CanX,CanY);

  TPostScript *ps = new TPostScript(psname.Data(),112);
  ps->NewPage();
  ps->Off();
  
  Draw_Recursive(f.GetListOfKeys(), file, ps, Can, _printEPS, CanX, CanY, MyCol);
  
  ps->On();
  ps->Close();
  
  f.Close();

  cout << "\nPostscript output file: " << psname.Data() << endl;
  
}


void Draw_Recursive(TList *keylist, TString PathName, TPostScript *ps, TCanvas *Can, 
		    Bool_t printEPS = false, Int_t CanX = 2, Int_t CanY = 2, Int_t MyCol = 102 ) {
  
  TIterator *it = keylist->MakeIterator();
  TKey *key;

  int CanWindow = 0;
  int CanNumber  = 0;

  while ((key = static_cast<TKey*>(it->Next())) != 0) {
    
     if( key->IsFolder() ){
       TObject *dirtype = (TObject*)key->ReadObj();
       if (! dirtype->InheritsFrom("TDirectory")) continue;
       cout<< endl << key->GetName() << endl;
       TDirectory *dir= (TDirectory*)key->ReadObj();
       TString NewPathName = PathName + "_"+ dir->GetName();
       dir->cd();
       
       Draw_Recursive(dir->GetListOfKeys(), NewPathName, ps, Can, printEPS, CanX, CanY, MyCol);
     }
     else {

       // Draw Histos inside folder

       //       Bool_t justWroteEPS=0;
 
       TH1F *histo = (TH1F*)key->ReadObj();
       TObject *histotype = (TObject*)key->ReadObj();
         
       if ( CanWindow == 0 ) {
	 // time for a new page:
	 ps->On(); ps->NewPage(); ps->Off();
	 //	 if ( CanNumber != 0 )
	 //  PrintPage(ps,Can,CanNumber,printEPS,PathName,CanX,CanY);
	 Can->Clear(); Can->Divide(CanX,CanY);
	 ++CanNumber;
       }

       Can->cd(++CanWindow);
       
       histo->SetFillColor(MyCol);
       if (histotype->InheritsFrom("TH2")) histo->Draw("col");
       else {
	   histo->Draw("HIST");
	   // histo->SetTitleSize(0.05);
	   // histo->GetXaxis()->SetTitleOffset(0.9);
	   //  histo->SetMarkerSize(0.8);
	   //  histo->SetMarkerStyle(8);
	   //  histo->Draw("B SAME");
       }

       printf(" - Did draw histo: %s in canvas window %d.\n", histo->GetName(), CanWindow);

       //       TString Zname("Zmass");
       //if ( Zname == histo->GetName()) {
       if (0) {
	 //	 TCanvas Can2;
	 //Can2.Print(PathName+"_"+Zname+".eps");
       }

       if ( CanWindow == (CanX*CanY)) {
	 // time to print the page:
	 PrintPage(ps,Can,CanNumber,printEPS,PathName,CanX,CanY);
       }

       CanWindow=CanWindow%(CanX*CanY);

     }
     
  }
  
  if ( CanNumber !=0 ) {
    PrintPage(ps,Can,CanNumber,printEPS,PathName,CanX,CanY);
  }

}

void PrintPage(TPostScript *ps, TCanvas *Can, Int_t CanNumber,
	       Bool_t printEPS, TString PathName, Int_t CanX, Int_t CanY)
{
  
  //  ps->On(); Can->Update(); ps->NewPage(); ps->Off();
  ps->On(); Can->Update(); ps->Off();
  if ( printEPS ){
    Can->Print(PathName+"_"+CanNumber+".eps");
    printf("Just wrote: ");
    cout<<PathName+"_"+CanNumber+".eps"<<endl<<endl;
  }
  //Can->Clear(); Can->Divide(CanX,CanY);
}
