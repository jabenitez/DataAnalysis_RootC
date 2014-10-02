///
/// compare_histos.C
///
/// 22/3/2006 Yann Coadou (yann@fnal.gov)
///           Updated from a macro by Aran Garcia-Bellido to go into top_cafe.
///           Called from compare_files.sh
///
///  Given a list of directories -- which have the same structure and plot -- traverse them
/// making comparison plots (put all the plots on the same canvas) and write them out.
///
///  This is very similar to what is done in compare_many_files_mod.C, but it will work
/// even if the strings to compare are in the same file. 
///
///  Much code was copied from the compare_many_files_mod -- thanks.
///
///
///  Sample input can be found in the compare_files.list file.

int Nhistos = 0;
TString histo_names[100];
TH1F *first_file_histo = NULL;
bool ratios = false;

void error(TString msg) { cout<<"ERROR:\n  "<<msg<<endl; abort(); }

class file_info : public TObject {
public:
  inline file_info (void)
    : _d(0), _offset(0), _startOffset(0), _dir_init (false), _factor(0), _color(1), _normalization(-1),_scalefactor(1), _channel(-1), _with_errors(0), _ratio(false)
  {}
  inline file_info (const file_info &f)
    : _d(f._d), _offset(0), _startOffset(0), _dir_init (false), _title(f._title),
      _factor(f._factor),
      _color(f._color),
      _normalization(f._normalization),
      _scalefactor(f._scalefactor),
      _channel(f._channel),
      _with_errors(f._with_errors),
      _ratio(f._ratio)
  {
  }
  inline char *GetName (void) {
    return _d->GetName();
  }
  TDirectory *_d;
  int _offset;
  int _startOffset;
  TString _title;
  bool _dir_init;
  TList _dirList;
  double _factor;
  int _color;
  double _normalization, _scalefactor; bool _ratio;
  int _channel;
  int _with_errors;

  TString TranslateDirName (TString name);
  TString GetNormalizedName (void);
  int GetNumberDirectories (void);
  void InitDirList (void);
  inline TDirectory *GetDirectory (int index) {
    InitDirList ();
    return (TDirectory*) _dirList.At(index);
  }

};


TObjArray *LoadFiles (TEnv *params);

TEnv *LoadParameters(TString configfile="");

void PlotFiles (TObjArray *list, TDirectory *output, TEnv *params);
void PlotHistoList (TObjArray &histograms, TObjArray *fileinfo, TEnv *params);

void compare_histos(TString configfile="")
{
  gROOT->ProcessLine(".L OverlayHistos.C");

  gROOT->SetStyle("Plain"); gROOT->ForceStyle();

  TEnv *params = LoadParameters(configfile);

  TObjArray *info = LoadFiles (params);
  if (info->GetEntriesFast() == 0) {
    cout << "No files found in input parameter files compare_files.list!" << endl;
    return;
  }

  TString outputFileName = params->GetValue("Output.File", "bogus.root");
  TFile *output = new TFile (outputFileName, "RECREATE");

  Nhistos = params->GetValue("Histos.Number", 0);
  for ( int hi=1; hi<=Nhistos; hi++ ) histo_names[hi] = params->GetValue(Form("Histos.%d.Name",hi),"");
  
  if (Nhistos>0) { 
    printf("\nWill only compare the following %d histos:\n",Nhistos); 
    for (int hi=1;hi<=Nhistos;hi++) printf("  %s\n",histo_names[hi].Data()); printf("\n");
  }

  PlotFiles (info, output, params);
  output->Write();
  output->Close();
}

/// Load the input file -- compare_files.defaults and then the input file compare_files.list, or the file specified as argument
TEnv *LoadParameters (TString configfile)
{
  TEnv *result = new TEnv ("compare_files");
  result->ReadFile ("compare_files.defaults", kEnvChange);
  if (configfile == "")
    result->ReadFile ("compare_files.list", kEnvChange);
  else
    result->ReadFile (configfile, kEnvChange);

  return result;
}

/// LoadFiles -- Given the parameters, attempt to open all specified files
TObjArray *LoadFiles (TEnv *params)
{
  TObjArray *result = new TObjArray ();

  int num_files = params->GetValue ("Files.Number", 0);

  for (int i = 1; i <= num_files; i++) {
    ostringstream baseName;
    baseName << "Files." << i << ".";
    TString bName (baseName.str().c_str());

    TString fname (params->GetValue(bName+"Name", "bogus_file"));
    TString base (params->GetValue(bName+"BaseDir", "."));

    TFile *f = new TFile (fname, "READ");
    f->cd(base);

    file_info *info = new file_info ();
    info->_d = gDirectory;
    info->_title = params->GetValue(bName+"Title", "BogusStuff");

    ///
    /// Lets say there is a dir sitting in the middle of the first sequence (#5)
    /// and not in the second sequence. On the first sequence you would then
    /// specify that you wanted to start offset at 5, and the offset would be 1.
    ///

    info->_offset = params->GetValue(bName+"DirOffset", 0);
    info->_startOffset = params->GetValue(bName+"StartOffset", 0);

    info->_factor = params->GetValue(bName+"Factor", 1.0);

    info->_color = params->GetValue(bName+"Color", 1);
    info->_channel = params->GetValue(bName+"Channel", -1);
    info->_with_errors = params->GetValue(bName+"PlotErrors", 0);
    info->_scalefactor = params->GetValue(bName+"ScaleFactor", -1.0);
    if ( info->_scalefactor == -1 ){
      info->_normalization = params->GetValue(bName+"Normalization", -1.0);
      if (info->_normalization == -1) {
	info->_normalization = params->GetValue("Global.Normalization", -1.0);
      }
    }
    info->_ratio = params->GetValue("Global.Ratios", 0);
    if ( info->_ratio ) ratios=true;
    result->Add(info);
  }

  return result;
}

/// PlotFiles -- do the work of traversing the plots! Recursively, even!
void PlotFiles (TObjArray *list, TDirectory *output_dir, TEnv *params)
{
  ///
  /// Use the first file in the list to run our deep-searching stradegy.
  ///
  file_info *master_file = (file_info*) list->At(0);
  int ndir = master_file->GetNumberDirectories();
  cout << "Processing file " << master_file->GetName();
  if (ndir > 0) {
    cout << " (" << ndir << " subdirs)";
  }
  cout << endl;

  ///
  /// First, process directories
  ///

  for (int didx = 0; didx < ndir; didx++) {
    ///
    /// Go down a level by making a copy...
    ///

    TObjArray newSubDirs;
    newSubDirs.SetOwner (true);
    TString f_name;
    file_info *first = 0;
    for (int i = 0; i < list->GetEntriesFast(); i++) {
      file_info *old = (file_info*) (*list)[i];
      file_info *newinfo = new file_info (*old);
	
      TDirectory *new_d = old->GetDirectory(didx);
      if (new_d == 0) {
	cout << "File " << i << " doesn't have the " << didx << " directory! This is bad!" << endl;
      }
      newinfo->_d = new_d;

      bool good = true;
      if (first == 0) {
	first = newinfo;
	f_name = first->GetNormalizedName();
      } else {
	TString s_name (newinfo->GetNormalizedName());
	if (f_name != s_name) {
	  cout << "File " << i << " doesn't have matching dir." << endl;
	  cout << "  Expected " << first->GetNormalizedName() << " but found "
	       << newinfo->GetNormalizedName() << endl;
	  cout << "  Skipping..." << endl;
	}
      }
      if (good) {
	newSubDirs.Add(newinfo);
      }
    }
    if (newSubDirs.GetEntriesFast() > 1) {
      TDirectory *subdir = output_dir->mkdir(first->GetName());
      PlotFiles (&newSubDirs, subdir, params);
    }
  }

  ///
  /// Now -- do the objects in the directory!
  ///

  master_file->_d->cd();
  TIter nextkey (master_file->_d->GetListOfKeys());
  TKey *key;
  while (key = (TKey*)nextkey()) {
    TObject *obj = key->ReadObj();
    

    if (obj->IsA()->InheritsFrom("TH1")) {
      /// Ok -- need to make a plot!
      TString name=obj->GetName();
      bool skip=false;
      if (Nhistos>0) {
	skip=true;
	for (int i=1;i<=Nhistos;++i) if (histo_names[i]==name) skip=false;
      }
      //cout <<"Name is: "<<name<<endl;
      if (skip) continue;
      //cout <<"Name is: "<<name<<endl;
      TObjArray sameHistos;
      bool good = true;
      int palette[100];
      int channel[100];
      int with_errors[100];
      int index = 0;
      for (int i = 0; i < list->GetEntriesFast(); i++) {
	file_info *info = (file_info*) (*list)[i];
	// cout << "Directory: " << info->_d->GetName() << endl;
	TH1 *h = (TH1*) info->_d->Get(obj->GetName());
	if (h) {
	  if (info->_scalefactor > 0) {
	    h->Scale(info->_scalefactor);
	  }
	  else if (info->_normalization > 0) {
	    if (h->Integral() != 0) {
	      h->Scale(info->_normalization/h->Integral());
	    }
	  } else {
	    h->Add (h, info->_factor-1.0);
	  }
	  
	  if ( info->_ratio == 1 ) {
	    if (i==0) { first_file_histo = new TH1F(*((TH1F*)h)); }
	    if (first_file_histo==NULL) error("First file histo == NULL ???");
	    if (h==NULL) error("histo == NULL ???");
	    h->Divide(first_file_histo);
	  }
	  sameHistos.Add(h);
	  // cout << " Added histo " << h->GetName() << endl;
	  palette[index++] = info->_color;
	  channel[i] = info->_channel;
	  with_errors[i] = info->_with_errors;
	} else {
	  good = false;
	  cout << "Missing plot " << obj->GetName() << endl; 
	}
      }
      PlotHistoList (sameHistos, list, output_dir, params, palette, channel, with_errors);
    }
  }
}

void PlotHistoList (TObjArray &histograms, TObjArray *fileinfo, TDirectory *output_dir, TEnv *params, int *palette, int *channel, int *with_errors)
{
  ///
  /// We'll just re-use a canvas to save with processing time
  ///

  TCanvas *c1 = 0;
  if (c1 == 0) {
    c1 = new TCanvas ("c1");
  }
  c1->Clear();

  ///
  /// Get the model histogram we'll be using.
  ///

  TH1 *first_guy = (TH1*) histograms[0];
  c1->SetName (first_guy->GetName());
  c1->SetTitle (first_guy->GetTitle());
  TString htitle (first_guy->GetTitle());
  TString xtitle (first_guy->GetXaxis()->GetTitle());
  TString hname (first_guy->GetName());

  ///
  /// Use the titles given to us so they show in the
  /// histogram legend
  ///

  for (int i = 0; i < histograms.GetEntriesFast(); i++) {
    TH1 *h = (TH1*) histograms[i];
    file_info *f = (*fileinfo)[i];
    h->SetName (f->_title);
    // h->SetStats(kFALSE);
  }

  ///
  /// Plot them all
  ///

  TString type = params->GetValue("Histo.Type", "Overlay");
  int goodNbins = params->GetValue("Histo.GoodNbins", -1);
  if (type == "OverlayLines" || type == "Legend") {
    if (h->InheritsFrom(TH2::Class())) {
      OverlayHistos2D (c1, histograms, htitle, "", "", palette);
    } else {
      OverlayHistos(ratios?"Ratios":"Legend",histograms, htitle, "", "", palette, with_errors, goodNbins);
    }
  }
  else if (type == "OverlayLinesWithStatBoxes" || type == "StatBox" || type == "Overlay") {
    if (h->InheritsFrom(TH2::Class())) {
      OverlayHistos2D (c1, histograms, htitle, "", "", palette);
    } else {
      OverlayHistos(ratios?"Ratios":"StatBox",histograms, htitle, "", "", palette, with_errors, goodNbins);
    }
  }
  else {
    cout << "Unknown histogram type: " << type.Data() << endl;
  }

  ///
  /// Now, write it out to a file! Also remove some temp stuff,
  /// otherwise root will complain a lot!
  ///

  TDirectory *old = gDirectory;
  output_dir->cd();
  c1->Write (hname);
  old->cd();

  if (gDirectory->FindObject("frame")) {
    delete gDirectory->FindObject("frame");
  }
}

///
/// InitDirList()
///
///  Get our dirlist setup
///
void file_info::InitDirList (void)
{
  if (_dir_init) {
    return;
  }
  _dir_init = true;

  _d->cd();
  TIter nextKey (_d->GetListOfKeys());
  TKey *key;
  int index = 0;
  while (key = (TKey*)nextKey()) {
    TObject *obj = key->ReadObj();
    if (obj->IsA()->InheritsFrom("TDirectory")) {
      ///
      /// If we are beyond _startOffset, then add the offset to see where we should be...
      ///
      bool useful = true;
      if (index >= _startOffset) {
	useful = index >= (_startOffset + _offset);
      }
      if (useful) {
	_dirList.Add(obj);
      }
      index++;
    }
  }
}

///
/// GetNumberDirectories
///
///  How many directories do we have floating around in here?
///
int file_info::GetNumberDirectories (void)
{
  InitDirList();
  return _dirList.GetSize();
}

///
///  Strip the leading index off...
///
TString file_info::GetNormalizedName (void)
{
  TString dirname (_d->GetName());
  char basename[2000];
  int index;
  sscanf(dirname.Data(), "%d%s", &index, basename);
  return TString (basename);
}

///
/// Sometimes the names have to move indicies for some odd
/// reason. Deal with it here.
///
TString file_info::TranslateDirName (TString name)
{
  if (_offset == 0) {
    return name;
  }

  ///
  /// Get the info, see if it is relavent!
  ///

  char basename[2000];
  int index;
  sscanf(name.Data(), "%d%s", &index, basename);

  if (index < _startOffset) {
    return name;
  }

  char finalname[2000];
  sprintf (finalname, "%02d%s", index + _offset, basename);

  cout << "Looking at " << finalname << endl;

  return finalname;
}
