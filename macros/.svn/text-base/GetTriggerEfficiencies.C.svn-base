GetTriggerEfficiencies(TString filename="/work/LS/histos/jun6/abcd/NL2NJ3/T1_McAtNlo/llp_hist.root"){

  TFile *f = new TFile (filename);
  TH1F * hTrigger = (TH1F*)f->Get("h_trigger_muOR"); 
  TH1F * hTrigger_BC = (TH1F*)f->Get("h_trigger_muOR_BC"); 

   TCanvas *c = new TCanvas("c","HTrigger",0,0,800,800);
  hTrigger->Draw();
  //  c->SaveAs("Flow.png"); 

  TCanvas *c1 = new TCanvas("c1","HTrigger Before Cuts",0,0,800,800);
  hTrigger_BC->Draw();
  //  c->SaveAs("FlowIndividual.png"); 
  
  TString m_cutname[20];
  m_cutname[0]="no cut";
  m_cutname[1]="EF_mu22_medium";
  m_cutname[2]="EF_2mu10_loose";
  m_cutname[3]="EF_mu18_medium";
  m_cutname[4]="EF_mu40_slow";
  m_cutname[5]="None";
  m_cutname[6]="OrAllTriggers";
  m_cutname[7]="ORMuonTriggers";
  m_cutname[8]="EF_mu22_medium_OR_EF_mu18_medium";
  m_cutname[9]="EF_mu22_medium_OR_EF_2mu10_loose";
  m_cutname[10]="EF_mu18_medium_OR_EF_2mu10_loose";
  m_cutname[11]="ORSlowMuonTriggers";
  int myindexes[15]={1,2,3,4,5,6,7,8,9,10,11,12,13};
  for (int j=2;j<12;j++){
    float eff=hTrigger->GetBinContent(j)/hTrigger->GetBinContent(1)*100.0;
    float eff_BC=hTrigger_BC->GetBinContent(j)/hTrigger_BC->GetBinContent(1)*100.0;
    float eff_AC=hTrigger->GetBinContent(j)/hTrigger_BC->GetBinContent(1)*100.0;
    float nafterCuts=hTrigger->GetBinContent(j);
    printf ("%40s %15.8f     AfterACuts:   %15.8f    Rel: %15.8f   NEvents: %15.8f\n",m_cutname[j-1].Data(),eff_BC,eff_AC,eff,nafterCuts);
  }
}
