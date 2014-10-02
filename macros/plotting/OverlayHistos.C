// 10/15/2003 
// Aran Garcia-Bellido (aran@fnal.gov)
// University of Washington
//
// 22/3/2006 Yann Coadou (yann@fnal.gov)
//           Updated to go into top_cafe. Called from compare_files.C
//
//
// This macro plots a bunch of histograms one on top of each other with a
// statistics box or a legend for each one of them. 
// Histograms can be plotted as histograms only or with error bars
// It takes as input a TObjArray of TH1F, the main title, the x and y axis
// labels. It will calculate the dimensions of the pad automatically based on
// the largest histograms. 
// The user specifies the colors in a palette. 

// Example of call from main program:
/*
{ 
  gROOT->ProcessLine(".L OverlayHistos.C"); 
  TFile f("~/root/tutorial/RecoCert.root");
  TH1F *h1 = (TH1F*) f->Get("MET/MET ();1"); 
  TH1F *h2 = (TH1F*) f->Get("MET/MEx ();1"); 
  TH1F *h3 = (TH1F*) f->Get("MET/MEy ();1");
  TCanvas *mycanvas = new TCanvas("mycanvas","Fancy plots");
  // Change the name of the histos here (this will be the legend):
  h1->SetName("Black line"); h2->SetName("Red line"); h3->SetName("Green line");
  // Switch to log scale if you need to:
  gPad->SetLogy();
  // Create array of histos: 
  TObjArray hists; hists.Clear(); 
  hists.Add(h1); hists.Add(h2);  hists.Add(h3);
  int palette[3]={1,2,4};
  int with_errors[3]={0,1,1};
  OverlayHistos("StatBox",hists,"","My variable [cm]","",palette); 
}
*/

void OverlayHistos(TString mode="StatBox", TObjArray histograms, 
				TString title="Main Title",
				TString xtitle="x title",
				TString ytitle="y title",
				Int_t palette[20],
		                int with_errors[20],
		                int goodNbins){
  bool ratio = mode=="Ratios";

  // Number of histos to plot:
  Int_t ntot = histograms.GetEntries();
  
  // First, check largest dimensions so that we can plot all histograms at once:
  Float_t xmin, xmax, ymin, ymax;
  xmin = 0.0;
  xmax = 0.0;
  ymin = 0.0;
  ymax = 0.0;
  for(Int_t i = 0; i<ntot; i++){
    TH1F * h = (TH1F*) histograms[i];
    int rebin = 1;
    if ( goodNbins > 0 )
      rebin = h->GetXaxis()->GetNbins()/goodNbins;
    rebin = rebin ? rebin : 1;
    h->Rebin(rebin); if (ratio) h->Scale(1.0/rebin);
    TAxis *axis = h->GetXaxis();
    if( axis->GetXmin() < xmin ) xmin = axis->GetXmin();
    if( axis->GetXmax() > xmax ) xmax = axis->GetXmax();
    if( h->GetMinimum() < ymin ) ymin = h->GetMinimum();
    if( h->GetMaximum() > ymax ) ymax = h->GetMaximum();
  }
  if (ratio) { //ymax=1.0+2.5*(ymax-1.0); ymin=2.0-ymax;
    ymax=1.20; ymin=0.80;
  }
  else ymax = TMath::Nint(ymax*1.05+1); // round up 5% over the max value

  // Get the stats for each plot. You don't need to store them anywhere. 
  // Just plot the individual histos so you can get the TPave and then next time
  // you Draw("sames") the histo its stats will be displayed with the changed values 
  if (mode == "StatBox") {
    for(Int_t i = 0; i<ntot; i++){
      TH1F * h = (TH1F*) histograms[i];
      if (title == "") title = h->GetTitle();
      h->Draw(); gPad->Update();
      TPaveStats* sta = (TPaveStats*) h->FindObject("stats");
      // Arrange positions:
      sta->SetX1NDC(0.79);  // common for all
      sta->SetX2NDC(0.99); // was 0.925
      Float_t height = (0.97-0.3)/6; // divide some reasonable total area by 4 // was 0.975
      Float_t separation = height*0.1; // separation between boxes
      Float_t y2 = 0.99-i*height-i*separation; // was 0.915
      Float_t y1 = y2-height;
      sta->SetY2NDC(y2);
      sta->SetY1NDC(y1);
      // Set attributes:
      //      sta->SetOptStat(1111);
      sta->SetOptStat(000000111);
      sta->SetName(h->GetName());
      sta->SetTextColor(palette[i]);
      sta->SetLineColor(palette[i]);
      sta->SetLineWidth(1);
    }
  }

  // Calculate the number of bins from the first histogram:
  TH1F * h0 = (TH1F*) histograms[0];
  Int_t nbinsx = h0->GetXaxis()->GetNbins();
  int rebin = 1;
  if (goodNbins > 0 )
    rebin = nbinsx/goodNbins;
  rebin = rebin ? rebin : 1;
  Int_t nbinsy = 100;
  if (title == "") title = h0->GetTitle();

  // Now make the plots:
  TH2F * frame = new TH2F("frame","",nbinsx,xmin,xmax,nbinsy,ymin,ymax);
  if ( xtitle = "" ) {// Use the main title as axis title if we don't have any
    frame->SetXTitle(title.Data());
  } else {
    frame->SetXTitle(xtitle.Data());
  }
  frame->SetYTitle(ytitle.Data()); 
  frame->SetStats(false);
  frame->Draw();
  TLegend *lgd = new TLegend(.70,.8,.95,.99); // AGB used to be (.7,.7,.89,.89)
  // This line makes the legend transparent (not grey, ewwww!): 
  lgd->SetBorderSize(0); lgd->SetTextSize(0.04); lgd->SetTextFont(62); lgd->SetFillColor(0);
  for(Int_t i = 0; i<ntot; i++){
    TH1F * h = (TH1F*) histograms[i];
    h->Rebin(rebin);
    TString bkgname = h->GetName();
    h->SetLineColor(palette[i]); h->SetLineWidth(3);
    TString SAME="sames";
    if (mode == "Legend"||mode=="Ratios") {
      h->SetStats(0);
      SAME="same";
    }
    if (with_errors[i])
      h->Draw(SAME+" e");
    else
      h->Draw(SAME+" hist");
    if (mode == "Legend"||mode=="Ratios") lgd->AddEntry(bkgname, bkgname, "l");
  }
  if (mode == "Legend"||mode=="Ratios") lgd->Draw();
  // Voila!
}


/// Do it now -- but for 2d. Here we just draw several of them.
///
void OverlayHistos2D(TCanvas *c1,
				      TObjArray histograms, 
		       TString title="Main Title",
		       TString xtitle="x title",
		       TString ytitle="y title",
		       Int_t palette[20])
{
  ///
  /// Calculate the number of rows
  ///

  int n = histograms.GetEntriesFast();
  int rows = 0;
  int columns = 100;
  while (columns > 3) {
    rows++;
    columns = n / rows;
    if (rows * columns < n) {
      columns++;
    }
  }

  c1->Divide (columns, rows);
  gStyle->SetPalette(1);
  for (int i = 0; i < n; i++) {
    TH2 *h = histograms[i];
    c1->cd(i+1);
    h->Draw("COLZ"); gPad->Update();
    TPaveStats* sta = (TPaveStats*) h->FindObject("stats");
    if (sta) {
      // Arrange positions:
      sta->SetX1NDC(0.7);  // common for all
      sta->SetX2NDC(0.90); // was 0.925
      Float_t height = (0.97-0.3)/6;  // divide some reasonable total area by 4 // was 0.975
      Float_t y2 = 0.90; // was 0.915
      Float_t y1 = y2-height;
      sta->SetY2NDC(y2);
      sta->SetY1NDC(y1);
      // Set attributes:
      //      sta->SetOptStat(1111);
      sta->SetOptStat(000000111);
      sta->SetName(h->GetName());
      sta->SetTextColor(palette[i]);
      sta->SetLineColor(palette[i]);
      sta->SetLineWidth(1);
    } else {
      cout<<"No stat in "<<h->GetName()<<endl;
    }
  }
}

