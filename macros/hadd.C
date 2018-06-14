void hadd(int nEvents) {
  TH2F* hadd = new TH2F(*SamplesST);
    for(int i=0; i<nEvents; i++){
      prun(1); hadd->Add(SamplesST);
    }
  hadd->Draw("colz");

}
