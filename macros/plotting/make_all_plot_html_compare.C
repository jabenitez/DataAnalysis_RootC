// this macro generate an html file with all the plot contained in the two root
// file passed as argument. The histos from each root file are overlayed in blue and
// red. Both trees have to be identical for this to work.
// You have to create a directory where you wish to 
// save all the gif file created and then you have to pass this directory as
// argumenten to the macro. E.g. I have created a  directory named "figures"
// in root:
//.x make_all_plot_html_compare.C("/path1/tree1.root","/path2/tree2.root", "figures")
// You have to move your html file produced and the directory with figure in 
// the same place. 
//
// Mods:
//   Now follows the more complex directory structure in the root files
//   Will create teh directory if it sin't already there.

void make_all_plot_html_compare(TString file1, TString file2,TString path)
{
  ///
  /// Load up the macros to make sure we can interpret everything
  /// in there!
  ///

  //  gInterpreter->ExecuteMacro ("topd0root_analysis/compile.C");

  ///
  /// Setup
  ///

  TFile *f1 = new TFile(file1);
  TFile *f2 = new TFile(file2);
  TEnv *params = new TEnv ("make_all_plot_html");
  //  params->ReadFile ("topd0root_analysis/macros/make_all_plot_html.params", kEnvChange);
  params->ReadFile ("make_all_plot_html.params", kEnvChange);

  gStyle->SetPalette(1);

  ///
  /// Ok -- we are ready to recursively process this stuff!
  ///

  html_a_directory (f1, f2, path, params);
}

///
/// Process a directory recursively.
///
void html_a_directory (TDirectory *f1, TDirectory *f2, TString &path, TEnv *params)
{
  static TCanvas *c_h1 = 0;
  if (c_h1 == 0) {
    int x = params->GetValue("H1.XSize", 150);
    int y = params->GetValue("H1.YSize", 100);
    c_h1 = new TCanvas ("c_h1", "1d plots", x, y);
  }

  ///
  /// Generate the output directory
  ///

  gSystem->MakeDirectory (path);

  ///
  /// Get the html started
  ///

  ofstream html (path + "/index.html");
  html << "<html><head><title>" << f1->GetName() << "</title></head>" << endl;
  html << "<body>" << endl;
  html << "<h1>" << f1->GetName() << "</h1>" << endl;
  html << "<h1>" << f2->GetName() << "</h1>" << endl;
  cout << "Processing directory " << f1->GetName() << endl;
  cout << "Processing directory " << f2->GetName() << endl;

  ///
  /// Now loop over all the keys in the directory
  ///

  f1->cd();
  TList *objlist = f1->GetListOfKeys();
  TList *objlist2 = f2->GetListOfKeys();
  TIterator *itr = objlist->MakeIterator();
  TKey *key;

  TString *currentdir;

  while ((key = static_cast<TKey*>(itr->Next())) != 0) {
    TObject *obj = key->ReadObj();

    cout << "This object has name " << obj->GetName() << endl;

    if (obj->IsA()->InheritsFrom("TDirectory")) {
      TDirectory *d = static_cast<TDirectory*>(obj);

      //      cout << "pwd = " << d->pwd() << endl;
      //      currentdir = d->GetPath();
      //      cout << "path = " << currentdir << endl;

      html << "<br><a href=\"" << d->GetName() << "/\">" << d->GetName() << "</a>" << endl;
      html_a_directory (d, f2, path + "/" + d->GetName(), params);
    }

    else if (obj->IsA()->InheritsFrom("TObjString")) {
      TObjString *s = static_cast<TObjString*>(obj);
      html << "<p><h2>" << key->GetName() << "</h2>" << endl;
      html << "<blockquote><pre>" << static_cast<char*>(s->GetString())
	   << "</pre></blockquote></p>"
	   << endl;

    }

    //    else if (obj->IsA()->InheritsFrom("CutFlowTable")) {
    //      CutFlowTable *c = static_cast<CutFlowTable*> (obj);
    //      
    //      html << "<p><h2>" << key->GetName() << "</h2>" << endl;
    //
    //      CFTPrinterHTML txt (html);
    //      f->cd();
    //      c->PrintGlobal (txt, "All Events", "");
    //      html << "</p>" << endl;
    //    }

    else if (obj->IsA()->InheritsFrom("TCanvas")) {
      TCanvas *cnv = static_cast<TCanvas*>(obj);

      //      cnv->Draw();
      //      cnv->SaveAs(path + "/" + key->GetName() + ".gif");
      //      cnv->SaveAs(path + "/" + key->GetName() + ".eps");
      cnv->Close();

      html << "<p><a href=\"" << key->GetName() << ".eps\">";
      html << "<img src=\"" << key->GetName() << ".gif\"/>";
      html << "</a></p>" << endl;
    }

    else if (obj->IsA()->InheritsFrom("TH1")) {
      TH1 *h = static_cast<TH1*> (obj);

      cout << " this object is called = " << obj->GetName() << endl;
      TH1 *h2 = static_cast<TH1*> (f2->Get(obj->GetName()));

      c_h1->cd();
      h->SetLineColor(kRed);
      h->Draw();
      h2->SetLineColor(kBlue);
      h2->Draw("same");

      c_h1->SaveAs(path + "/" + key->GetName() + ".gif");
      c_h1->SaveAs(path + "/" + key->GetName() + ".eps");


      html << "<p><a href=\"" << key->GetName() << ".eps\">";
      html << "<img src=\"" << key->GetName() << ".gif\"/>";
      html << "</a></p>" << endl;




    }
    f1->cd();
  }

  ///
  /// Done!
  ///

  html << "</body></html>" << endl;
  html.close();
}








