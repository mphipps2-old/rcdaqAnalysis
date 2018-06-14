#include "TreeManager.h"
ClassImp(TreeManager);

TreeManager::TreeManager(Configuration *config) {
  fOutputTrees = config->GetOutputTrees();
  fRunName = config->GetRunName();
  fDirectoryName = config->GetTreeDirectory();
  fNStrips = config->GetNChannels();
  fNDevices = config->GetNDevices();
  fNTimeBins = config->GetNTimeBins();
  InitRootFile();
}
//====================================================================================================================
TreeManager::~TreeManager() {
    delete fStripTree;
}

//===================================================================================================================

void TreeManager::Reset() {
  fStripTree->Reset();
  fHits_v.clear();
}

//====================================================================================================================

// Called in TreeManager()
void TreeManager::InitRootFile() { 
  fStripTree = new TTree("Strip","Strip") ;
  fStripTree->Branch("XRowNo", "std::vector<Int_t>", &fXRowNo_v);
  fStripTree->Branch("YRowNo", "std::vector<Int_t>", &fYRowNo_v);
  fStripTree->Branch("StripNo","std::vector<Int_t>",&fStripNo_v);
  fStripTree->Branch("EventNo", &fStripEventNo);
  fStripTree->Branch("TimeSize","std::vector<int>", &fTimeSize_v);
  fStripTree->Branch("MaxCharge","std::vector<float>", &fPeakStripCharge_v);
  fStripTree->Branch("AvgCharge","std::vector<float>", &fAvgStripCharge_v);
  fStripTree->Branch("IntCharge","std::vector<float>", &fIntStripCharge_v);
  fStripTree->Branch("MaxTimeBin", "std::vector<Int_t>",&fMaxTimeBin_v);
  fStripTree->Branch("Saturation","std::vector<Bool_t>", &fSaturation_v);
  fStripTree->Branch("PedMean","std::vector<float>", &fPedMean_v);
  fStripTree->Branch("PedRMS","std::vector<float>", &fPedRMS_v);
  fTimeSize_v.resize(fNStrips*fNDevices);
  fPeakStripCharge_v.resize(fNStrips*fNDevices);
  fAvgStripCharge_v.resize(fNStrips*fNDevices);
  fIntStripCharge_v.resize(fNStrips*fNDevices);
  fMaxTimeBin_v.resize(fNStrips*fNDevices);
  fSaturation_v.resize(fNStrips*fNDevices);
  fPedMean_v.resize(fNStrips*fNDevices);
  fPedRMS_v.resize(fNStrips*fNDevices);
  fXRowNo_v.resize(fNStrips*fNDevices);
  fYRowNo_v.resize(fNStrips*fNDevices);
  fStripNo_v.resize(fNStrips*fNDevices);
  fRawTimeBinCharge_vv.resize(fNStrips*fNDevices);
  fTimeBinCharge_vv.resize(fNStrips*fNDevices);
  for( int c = 0; c < fNDevices*fNStrips; c++ ){
    //    cout << " time bins " << fNTimeBins << endl;
    fRawTimeBinCharge_vv[c].resize(fNTimeBins);
    fTimeBinCharge_vv[c].resize(fNTimeBins);
    //    std::cout << "       channel " << c << std::endl;
    fStripTree->Branch( Form( "RawC%d", c), "std::vector<float>", &fRawTimeBinCharge_vv[c] );
    fStripTree->Branch( Form( "C%d", c), "std::vector<float>", &fTimeBinCharge_vv[c] );
    //  fStripTree->Branch("Time_v", "vector<Float_t>", &fTime_v);
  }
  //  cout << "done w/ init root file " << endl;
}

//====================================================================================================================
// Called in FillRootFile()
void TreeManager::FillObjectsInRoot(Processor *processor) {
  fProcessor = processor;
  fHits_v = fProcessor->GetHitVec();
  for(Int_t stripNb = 0; stripNb < fNStrips*fNDevices; stripNb++) {      
    fXRowNo_v[stripNb] = fHits_v[stripNb]->GetXRowNo();
    fYRowNo_v[stripNb] = fHits_v[stripNb]->GetYRowNo();
    fStripNo_v[stripNb] = fHits_v[stripNb]->GetHitNo();
     fTimeSize_v[stripNb] = fHits_v[stripNb]->GetHitSizeTime();
      fPeakStripCharge_v[stripNb] = fHits_v[stripNb]->GetMaxCharge();
      fAvgStripCharge_v[stripNb] = fHits_v[stripNb]->GetAvgCharge();
      fIntStripCharge_v[stripNb] = fHits_v[stripNb]->GetIntegralCharge();
      fMaxTimeBin_v[stripNb] = fHits_v[stripNb]->GetMaxTimeBin();
      fSaturation_v[stripNb] = fHits_v[stripNb]->GetSaturation();
      fPedMean_v[stripNb] = fHits_v[stripNb]->GetPedMean();
      fPedRMS_v[stripNb] = fHits_v[stripNb]->GetPedRMS();
      fTimeBinCharge_vv[stripNb] = fHits_v[stripNb]->GetTimeBinCharges();
      fRawTimeBinCharge_vv[stripNb] = fHits_v[stripNb]->GetRawTimeBinCharges();
  }
  fStripTree->Fill();
}


//====================================================================================================================
void TreeManager::WriteRootFile() {
  char fileName[256];
  sprintf(fileName,"%s/Tree%sRun%d.root",fDirectoryName.Data(),fRunName.Data(), fProcessor->GetRunNum());
  if (fOutputTrees.EqualTo("YES",TString::kIgnoreCase))  fFile = TFile::Open(fileName,"RECREATE");
  fFile->cd();
  fStripTree->Write();
  gDirectory->pwd(); 
  fFile->Close();
  gROOT->cd(0);
}
