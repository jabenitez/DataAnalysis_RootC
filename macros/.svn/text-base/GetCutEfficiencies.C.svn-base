GetCutEfficiencies(TString filename="/work/LS/histos/Aug1_nLBM/htcutzmass/NL2NJ3/T1_McAtNlo/llp_hist.root"){

  TFile *f = new TFile (filename);
  TH1F * hFlow = (TH1F*)f->Get("hCutFlow"); 
  TH1F * hFlowIndividual = (TH1F*)f->Get("hCutFlowIndividual"); 

   TCanvas *c = new TCanvas("c","HFlow",0,0,800,800);
  hFlow->Draw();
  //  c->SaveAs("Flow.png"); 

  TCanvas *c1 = new TCanvas("c1","HFlowIndividual",0,0,800,800);
  hFlowIndividual->Draw();
  //  c->SaveAs("FlowIndividual.png"); 
  
  TString m_cutname[20];
  m_cutname[0]="nocut";
  m_cutname[1]="nocut";
  m_cutname[2]="nocut";
  m_cutname[3]="NJets";
  m_cutname[4]="JetspT";
  m_cutname[5]="NLeptons";
  m_cutname[6]="NLowBetaMu";
  m_cutname[7]="Trigger";
  m_cutname[8]="AJALHtCut";
  m_cutname[9]="LowBMu1Eta";
  m_cutname[10]="ZMassCut";
  m_cutname[11]="ReqNeutralino";
  m_cutname[12]="HtAllJets";
  m_cutname[13]="HtAllLeptons";
  m_cutname[14]="Nocut";
  int myindexes[14]={1,2,3,4,5,6,7,8,9,10,11,12,13,14};
  for (int j=2;j<14;j++){
    int i=myindexes[j];
    float eff=hFlow->GetBinContent(i)/hFlow->GetBinContent(1)*100.0;
    float effindividual=hFlowIndividual->GetBinContent(i)/hFlowIndividual->GetBinContent(1)*100.0;
    
    printf ("%15s: %15.8f     Rel: %15.8f\n",m_cutname[i-1].Data(),effindividual,eff);
    //    cout << "Cut name  "  << m_cutname[i] << " Content i   " << hFlow->GetBinContent(i) << endl;
    //    cout << "Cut name  "  << m_cutname[i] << "  eff: " << eff <<  "     eff Individual    " << effindividual << endl;
    // "  cero contnet " << hFlow->GetBinContent(2) << "  icontnent " << hFlow->GetBinContent(i)<<endl;
  }


  /*  for (int i=1;i<10;i++){
    cout << "Cut name  i: "<< i  << "   "<< m_cutname[i-1] << " Content i   " << hFlow->GetBinContent(i)  << "   Content Indi  "<<  hFlowIndividual->GetBinContent(i)<< endl;

    }*/
}
