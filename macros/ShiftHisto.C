void ShiftHisto(TH1F* h) {

  for (int i = 0; i < 1024; ++i) {
    float ADC = h->GetBinContent(i+1);
    h->SetBinContent(i+1-150,ADC);
  }
  h->GetXaxis()->SetRangeUser(0,300);
  h->GetYaxis()->SetTitle("ADC");
  h->GetXaxis()->SetTitle("Time (1 ns)");
  h->SetLineColor(2);
  h->Draw();

  TLatex *   tex = new TLatex(0.1824713,0.8707627,"ATLAS");
  tex->SetNDC();
  tex->SetTextFont(73);
  tex->SetTextSize(28);
  tex->SetLineWidth(2);
  tex->Draw();
  tex = new TLatex(0.3318966,0.8707627,"Internal");
  tex->SetNDC();
  tex->SetTextFont(43);
  tex->SetTextSize(28);
  tex->SetLineWidth(2);
  tex->Draw();
  tex = new TLatex(0.5704023,0.8686441,"#font[12]{E}_{Pb}=150A GeV");
  tex->SetNDC();
  tex->SetTextFont(43);
  tex->SetTextSize(28);
  tex->SetLineWidth(2);
  tex->Draw();  
}
