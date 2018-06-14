#include "Processor.h"
#include "Configuration.h"
#include "Hit.h"

ClassImp(Processor);

Processor::Processor(const char* name, const char* title, Configuration *config, TString runType) {

  fConfig = config;
  fSRSMaxADC = 4096; // 12 bits
  fStruck1MaxADC = 8192; // 13 bits 
  fStruck23MaxADC = 2048; // 11 bits
  fBeamTestMaxADC = 4096; // bipolar HBD electronics (2048 pos; 2048 neg)
  fDRSMaxADC = 4096;
  fSaturationStruck1 = 8000; 
  fSaturationStruck23 = 1700;
  fSaturationSRS = 4050; // Need to test
  fSaturationDRS = 4050; // Need to test
  fNChannels = fConfig->GetNChannels();
  fNStrips = fNChannels; 
  fBaseline = fConfig->GetBaseline();
  fNRowsX = fConfig->GetNRowsX();
  fNRowsY = fConfig->GetNRowsY();
  fNDevices = fConfig->GetNDevices();
  fNTimeBins = fConfig->GetNTimeBins();
  fPedRange = fConfig->GetOnlinePedRange();
  fNSigma = fConfig->GetNSigma();
  fCutType = fConfig->GetCutType();
  fBinaryCut = fConfig->GetBinaryCut();
  fDAQSystem = fConfig->GetDAQSystem();
  fDRSTrn = fConfig->GetDRSTrn();
  fInvertSignal = fConfig->GetInvertSignal(); 
  fDevNum = fConfig->GetDevNum();  
  fNPackets = fConfig->GetNPackets();
  fMinHitSize = fConfig->GetMinHitSize();
  char pedName[100];
  sprintf(pedName, "pedMean");
  fPedMean = new TH1F(pedName,pedName,fNChannels*fNDevices,0,fNChannels*fNDevices);
  fPedMean->SetXTitle("Strip Number");
  fPedMean->SetYTitle("Mean ADC");
  sprintf(pedName, "pedRMS");
  fPedRMS = new TH1F(pedName,pedName,fNChannels*fNDevices,0,fNChannels*fNDevices);
  fPedRMS->SetXTitle("Strip Number");
  fPedRMS->SetYTitle("ADC RMS");
 
  if ((fConfig->GetRunMode()).EqualTo("PEDESTAL",TString::kIgnoreCase)) {
    for (int i=0; i < fNStrips*fNDevices; ++i) {
      char name[50];
      sprintf(name, "pedChannel%d",i);
      TH1F *ped = new TH1F(name,name,10000,0,9999);
      fPedestals_v.push_back(ped);
    }
  }
  if ((fConfig->GetChannelCalibration()).EqualTo("YES",TString::kIgnoreCase)) {CalibrateChannels(); }
}

//////////////////////////////////////////////////////

Processor::~Processor() {

   delete fPedMean;

    delete fPedRMS;   
    //    delete fNonlinFunc;   
    // delete fRootEvent;
  // delete fRawPedMean;
  // delete fRawPedRMS;
  // int unusedCh = 32; 
  /*
  for (int chNo=0; chNo < fNChannels - unusedCh; ++chNo) {
    delete fPedestals_v[chNo];
  }
  */
}

//////////////////////////////////////////////////////

void Processor::FreeMemory() {
  cout << "freeing memory" << endl; 
  for (int i =0; i<fHits_v.size(); ++i) {
    delete fHits_v[i];
  }

  //  if ((fConfig->GetChannelCalibration()).EqualTo("YES",TString::kIgnoreCase) && (fDAQSystem.EqualTo("SRS",TString::kIgnoreCase))) delete fNonlinFunc;
 fHits_v.clear();
  fPedMean->Reset();
  fPedRMS->Reset();
  // delete fRootEvent;
  //  delete fPedMean;
  //  delete fPedRMS;
}

//////////////////////////////////////////////////////

void Processor::Reset() {
  for (int i =0; i<fHits_v.size(); ++i) {
    delete fHits_v[i];
  }
  fHits_v.clear();
  for (int p = 0; p < fNPackets; ++p) {
    delete fPacket_v[p];  
  }
  fPacket_v.clear();
}

//////////////////////////////////////////////////////

void Processor::InitializeEvent(Event *e) {
  fEvent = e;  
  fRunNum = e->getRunNumber();
  fData_m.clear();
  fRawData_m.clear();
  fHits_v.clear();
  fEventNum = e->getEvtSequence();
}

//////////////////////////////////////////////////////

void Processor::CalibrateChannels() {
  ifstream file(gSystem->ExpandPathName(fConfig->GetCalibrationFile()));
  if(!file.is_open()) cerr << "Processor::CalibrateChannels(): calibration file not open" << endl;
  TString line;
  // To correct SRS nonlinearity/response differences. Each channel normalized
  if (fDAQSystem.EqualTo("Struck",TString::kIgnoreCase)) {
    while (line.ReadLine(file)) {
    // strip leading spaces and skip comments
      line.Remove(TString::kBoth, ' ');
      if(line.BeginsWith("#")) continue;
      // Array of tokens separated by ","
      TObjArray *tokens = line.Tokenize(",");
      TIter myIter(tokens);
   
      while(TObjString *st = (TObjString*) myIter.Next()) {
        
        fCalibration_v.push_back((((TObjString*) myIter.Next())->GetString().Remove(TString::kBoth,' ')).Atof());
      }
    }
  }
  // To correct SRS nonlinearity: 1 pol5 function for all channels (channel-to-channel distortions ignored)
  else if (fDAQSystem.EqualTo("SRS",TString::kIgnoreCase)) {
    fNonlinFunc = new TF1("nonlinFunc","[0]+[1]*x", 0,2500);
    int count = 0;
    while (line.ReadLine(file)) {
    // strip leading spaces and skip comments
      line.Remove(TString::kBoth, ' ');
      if(line.BeginsWith("#")) continue;

      char par[200];
      sscanf(line.Data(), "Par%d %s",&count,par);
      fNonlinFunc->SetParameter(count,atof(par));
      //      cout << "param " << fNonlinFunc->GetParameter(count) << endl; 
      ++count;
      //int check; cin >> check;
    }
    delete fNonlinFunc;
    //    fNonlinFunc->Draw();
   
  }
}

/////////////////////////////////////////////////////
void Processor::LoadMapping() {

  // Channel Mapping
  ifstream file(gSystem->ExpandPathName(fConfig->GetMappingFile()));
  if(!file.is_open()) cerr << "Processor::LoadMapping(): mapping file not open" << endl;
  TString line;
  while (line.ReadLine(file)) {
  // strip leading spaces and skip comments
    line.Remove(TString::kBoth, ' ');
    if(line.BeginsWith("#")) continue;
    // Array of tokens separated by ","
    TObjArray *tokens = line.Tokenize(",");
    TIter myIter(tokens);
    while(TObjString *st = (TObjString*) myIter.Next()) {
      fStripMapping_v.push_back( (((TObjString*) myIter.Next())->GetString().Remove(TString::kBoth,' ')).Atoi());        
      fXMapping_v.push_back( (((TObjString*) myIter.Next())->GetString().Remove(TString::kBoth,' ')).Atoi());
      fYMapping_v.push_back( (((TObjString*) myIter.Next())->GetString().Remove(TString::kBoth,' ')).Atoi());
    }
  }

  // Device Mapping (ie packets corresponding to digitizer devices
  ifstream deviceFile(gSystem->ExpandPathName(fConfig->GetDeviceMappingFile()));
  if(!deviceFile.is_open()) cerr << "Processor::LoadMapping(): device mapping file not open" << endl;
  while (line.ReadLine(deviceFile)) {
  // strip leading spaces and skip comments
    line.Remove(TString::kBoth, ' ');
    if(line.BeginsWith("#")) continue;
    // Array of tokens separated by ","
    TObjArray *tokens = line.Tokenize(",");
    TIter myIter(tokens);
    while(TObjString *st = (TObjString*) myIter.Next()) {
      fDeviceMapping_v.push_back( (((TObjString*) myIter.Next())->GetString().Remove(TString::kBoth,' ')).Atoi());        
    }
  }
}

/////////////////////////////////////////////////////
// Note: this function must be updated whenever new readout system added
void Processor::UnpackData() {
  // Device numbering in RCDAQ
  for (int device=0; device < fNDevices; ++device) {
    int packetNb = fDeviceMapping_v[device];
    fPacket_v.push_back(fEvent->getPacket(packetNb));
  }

  // Fill fRawData with fully mapped data from all time bins (no cuts)
  for (int device=0; device < fNDevices; ++device) {
    for (int chNo=0; chNo < fNChannels; ++chNo) {
      vector<Float_t> timeBinVec; 
      Int_t padCoordinate; 
      for (int timeBin=0; timeBin < fNTimeBins; ++timeBin) {
        // for the struck, "device" == FADC
        // for the SRS, "device" == APV
        if (fDAQSystem.EqualTo("Struck",TString::kIgnoreCase)) {
          timeBinVec.push_back((fPacket_v[device]->iValue(chNo, timeBin)));
        }
	else if (fDAQSystem.EqualTo("SRS",TString::kIgnoreCase)) {
          timeBinVec.push_back(fPacket_v[device]->iValue(chNo, timeBin, device*2) );  
        }
        else if (fDAQSystem.EqualTo("DRS4",TString::kIgnoreCase)) {
	  timeBinVec.push_back(fPacket_v[device]->rValue(timeBin,chNo));
	}
        else if (fDAQSystem.EqualTo("DRS32",TString::kIgnoreCase) && device == 0) {

	  
          if (chNo == fNChannels -1 && fDRSTrn.EqualTo("TR0",TString::kIgnoreCase)) {timeBinVec.push_back(fPacket_v[device]->iValue(timeBin,"TR0"));  } 
          else if (chNo == fNChannels -1 && fDRSTrn.EqualTo("TR1",TString::kIgnoreCase))  timeBinVec.push_back(fPacket_v[device]->iValue(timeBin,"TR1"));
          else if (chNo == fNChannels - 2 && fDRSTrn.EqualTo("BOTH",TString::kIgnoreCase))   timeBinVec.push_back(fPacket_v[device]->iValue(timeBin,"TR0"));
          else if (chNo == fNChannels - 1 && fDRSTrn.EqualTo("BOTH",TString::kIgnoreCase))   timeBinVec.push_back(fPacket_v[device]->iValue(timeBin,"TR1"));
          else  timeBinVec.push_back(fPacket_v[device]->iValue(timeBin,chNo));
        }
        else if (fDAQSystem.EqualTo("BEAMTEST",TString::kIgnoreCase)) {
          if (fDevNum == 0) timeBinVec.push_back(fPacket_v[device]->iValue(chNo,timeBin));       
        }
        else timeBinVec.push_back(0);
      }
      padCoordinate = fStripMapping_v[chNo+(device*fNChannels)];
      if (padCoordinate == -100) continue; // Unused channels set to -100 in mapping file
      Int_t xCoord = fXMapping_v[chNo+(device*fNChannels)];
      Int_t yCoord = fYMapping_v[chNo+(device*fNChannels)];       
 
        fRawData_m[padCoordinate] = timeBinVec;  // each entry in fRawData is the data from one event. This includes all time bins across all channels
    }
  }
}


//////////////////////////////////////////////////////

void Processor::MakeCuts() {
  //  cout << " making cuts!!! " << endl;
  for (int device = 0; device < fNDevices; ++device) {
    //cout << "device " << device << endl;
  /*
  // Common mode correction calculation (correction for each strip in device (APV or FADC) at particular time bin (ie 1 correction value for every time bin))
  // Found common mode very small and not periodic enough to easily correct. May consider using again after more detailed study
  Float_t timeBinCorrection[fNTimeBins];
  for (int timeBin=0; timeBin < fNTimeBins; ++timeBin) {
    timeBinCorrection[timeBin] = 0;
    for (int stripNo=(device*fNCh); stripNo < (fNCh+device*fNCh); ++stripNo) {
      vector<Int_t> coordinateVec; coordinateVec.push_back(fXMap[stripNo]); coordinateVec.push_back(fXMap[stripNo]);
      timeBinCorrection[timeBin] += (fRawData_m[stripNo][coordinateVec][timeBin] - fPedMean->GetBinContent(stripNo+1));
       //       if (fRawData_m[stripNo][timeBin] > 500) cout << " raw data  " << fRawData_m[stripNo][timeBin] << "   ped mean " << fPedMean->GetBinContent(stripNo+1)<< " strip no " << stripNo <<  endl;
    }  
    timeBinCorrection[timeBin] /= fNCh;
    //if ( timeBin == 95 || timeBin == 105) cout << " device " << device << " tb " << timeBin << " tbc " << timeBinCorrection[timeBin] << endl; 
  }
    */

  // Pedestal correction (correction in time for each strip -- ie each strip has its own correction value)
    for (int chNo=(device*fNChannels); chNo < fNChannels+(device*fNChannels); ++chNo) {
      vector<Float_t> timeBinVec;    
      int cNo = chNo;   

      Int_t padCoordinate = fStripMapping_v[chNo];

      if (padCoordinate == -100) continue; // unused channels
      for (int timeBin=0; timeBin < fNTimeBins; ++timeBin) {
	   float neighborAvg = 0;
	   /*	 /// this routine is used for internal pulser SRS data to correct for overshoot  
        if (device == 1 && timeBin == 0) cNo -=128;
        else if (device == 2 && timeBin == 0) cNo -= 256;
        else if (device == 3 && timeBin == 0) cNo -= 384;
	if (cNo % 8 == 4) {
          if (cNo == 0) neighborAvg = ((fSRSMaxADC - fPacket_v[device]->iValue(cNo+1, timeBin, device*2))+(fSRSMaxADC - fPacket_v[device]->iValue(cNo+2, timeBin, device*2))) /2;
          else if (cNo ==1 ) neighborAvg = ((fSRSMaxADC - fPacket_v[device]->iValue(cNo-1, timeBin, device*2))+(fSRSMaxADC - fPacket_v[device]->iValue(cNo+1, timeBin, device*2))+(fSRSMaxADC - fPacket_v[device]->iValue(cNo+2, timeBin, device*2))) / 2;
	  else if (cNo == 126) neighborAvg = ((fSRSMaxADC - fPacket_v[device]->iValue(cNo-2, timeBin, device*2))+(fSRSMaxADC - fPacket_v[device]->iValue(cNo-1, timeBin, device*2))+(fSRSMaxADC - fPacket_v[device]->iValue(cNo+1, timeBin, device*2))) / 3;
	  else if (cNo == 127) neighborAvg = ((fSRSMaxADC - fPacket_v[device]->iValue(cNo-1, timeBin, device*2))+(fSRSMaxADC - fPacket_v[device]->iValue(cNo-2, timeBin, device*2))) / 2;
	  else neighborAvg = ((fSRSMaxADC - fPacket_v[device]->iValue(cNo-2, timeBin, device*2))+(fSRSMaxADC - fPacket_v[device]->iValue(cNo-1, timeBin, device*2))+(fSRSMaxADC - fPacket_v[device]->iValue(cNo+1, timeBin, device*2))+(fSRSMaxADC - fPacket_v[device]->iValue(cNo+2, timeBin, device*2))) / 4;
        }
        
        else neighborAvg = fSRSMaxADC - fPacket_v[device]->iValue(cNo, timeBin, device*2);
	   */	   
         Float_t calibrationConst;
         if (fCalibration_v.size() == 0) calibrationConst = 1;
         else calibrationConst = fCalibration_v[chNo + (device*fNChannels)];
 	 if (fDAQSystem.EqualTo("SRS",TString::kIgnoreCase)) {
	   float adc = fRawData_m[padCoordinate][timeBin] - fPedMean->GetBinContent(chNo+1);
	   /// USE for internal pulser data to correct overshoot
	   // float adc = fRawData_m[padCoordinate][timeBin] - neighborAvg;
           if (adc > 790) timeBinVec.push_back(NonlinCorrection(adc)); // strip specific pedestal subtraction
           else timeBinVec.push_back(adc*calibrationConst);
	  
	 }
	 else {
           float adc = fRawData_m[padCoordinate][timeBin] - fPedMean->GetBinContent(chNo+1);

	   //	   if (timeBin ==  5 ) cout << "channel " << chNo <<  " sraw data tr data " << fRawData_m[padCoordinate][timeBin]    << " mean " << fPedMean->GetBinContent(chNo+1) << " calibConst " << calibrationConst << endl; 
	   if (fInvertSignal.EqualTo("YES",TString::kIgnoreCase)) {
             adc *= -1;
             if (fDAQSystem.EqualTo("DRS4",TString::kIgnoreCase) && (padCoordinate == 8 || padCoordinate == 9)) adc *= -1;
	   }
           timeBinVec.push_back(adc*calibrationConst); // strip specific pedestal subtraction

          
	   //           if (timeBin <  5 && chNo == 13 ) cout << "channel " << chNo <<  " sraw data tr data " << fRawData_m[padCoordinate][timeBin] << " mean " << fPedMean->GetBinContent(chNo+1) << endl;
	 }
        // Common mode correction
        // if (fDAQSystem.EqualTo("SRS",TString::kIgnoreCase)) temp[timeBin] -= timeBinCorrection[timeBin]; // Only done for SRS since too few channels in struck. 
      }

      fData_m[padCoordinate] = timeBinVec;
    } 
  }
  // set hit limits (zero suppression)
  
  for (int stripNo=0; stripNo < fNStrips * fNDevices; ++stripNo) {
    vector<Float_t> timeBinCharges; 
    vector<Float_t> rawTimeBinCharges; 
    Float_t charge; 
    Bool_t hit = 0;
    Int_t hitSizeTimeMax = 0;
    Float_t cut;
    if (fCutType.EqualTo("Analog",TString::kIgnoreCase)) cut = fNSigma * fPedRMS->GetBinContent(stripNo+1);
    else cut = fBinaryCut;
    Int_t padCoordinate = fStripMapping_v[stripNo];
    if (padCoordinate == -100) continue; // Unused channels
    Int_t xCoord = fXMapping_v[stripNo];
    Int_t yCoord = fYMapping_v[stripNo];
    float pedMean = fPedMean->GetBinContent(stripNo+1);
    for (int timeBin=0; timeBin < fNTimeBins; ++timeBin) {
      Int_t hitSizeTime = 0;
      float totalAdc = fData_m[padCoordinate][timeBin];
      int nBins = 0;
      //       cout << " strip " << stripNo << " timebin " << timeBin <<  " data " << fData_m[padCoordinate][timeBin] << " pedmean " << pedMean << endl;
      if (fData_m[padCoordinate][timeBin] > 0 && timeBin+1 != fNTimeBins) { // 0 since pedmean already subtracted
	 // cout << " strip " << stripNo << " timebin " << timeBin <<  " cut " << cut * TMath::Sqrt(nBins+1) << " data " << fData_m[padCoordinate][timeBin] << " pedmean " << pedMean << endl;
        for (int N=1; timeBin+N < fNTimeBins; ++N) { // flexible sliding window -- 
          float next = fData_m[padCoordinate][timeBin+N]; 
	  //    	  if (N >5)         cout << " strip " << stripNo << " N " << N << " timebin " << timeBin+N << " totalADC " << totalAdc << " cut " << cut * TMath::Sqrt(nBins+1) << " next " << next << " pedmean " << pedMean << endl;
          if (next <= 0) {  break;}
	 
          else {totalAdc += next; nBins++; /*timeBinCharges.push_back(fData_m[padCoordinate][timeBin]);       rawTimeBinCharges.push_back(fRawData_m[padCoordinate][timeBin]); */} 
        } 
        if (stripNo == 0 || stripNo == 1 ) { // hack put in b/c pmt and sipm widths are different
          if ( totalAdc < (TMath::Sqrt(nBins+1) * cut)) {
            for (int N=0; N <= nBins; ++N) {
              timeBinCharges.push_back(fData_m[padCoordinate][timeBin+N]);
              rawTimeBinCharges.push_back(fRawData_m[padCoordinate][timeBin+N]);
              timeBinCharges[timeBin+N] = 0;
	    }
   	  }
          else {
            for (int N=0; N <= nBins; ++N) {
              timeBinCharges.push_back(fData_m[padCoordinate][timeBin+N]);
              rawTimeBinCharges.push_back(fRawData_m[padCoordinate][timeBin+N]);
              ++hitSizeTime;
	    }
          }
	}
	else {
          if ( totalAdc < (TMath::Sqrt(nBins+1) * cut) || nBins < fMinHitSize) {
            for (int N=0; N <= nBins; ++N) {
              timeBinCharges.push_back(fData_m[padCoordinate][timeBin+N]);
              rawTimeBinCharges.push_back(fRawData_m[padCoordinate][timeBin+N]);
              timeBinCharges[timeBin+N] = 0;
	    }
   	  }
          else {
            for (int N=0; N <= nBins; ++N) {
              timeBinCharges.push_back(fData_m[padCoordinate][timeBin+N]);
              rawTimeBinCharges.push_back(fRawData_m[padCoordinate][timeBin+N]);
              ++hitSizeTime;
	    }
          }
	}
        timeBin+=nBins;
      }
      else {
        timeBinCharges.push_back(fData_m[padCoordinate][timeBin]);
        rawTimeBinCharges.push_back(fRawData_m[padCoordinate][timeBin]);
        timeBinCharges[timeBin] = 0;
      }
      //      timeBin += (nBins - 1);
      if (hitSizeTime > hitSizeTimeMax) hitSizeTimeMax = hitSizeTime;
    }
    //    if (strip == 1) {
      Hit *strip = new Hit("strip","strip",timeBinCharges);
      strip->SetRawTimeBinCharges(rawTimeBinCharges);
      if (fDAQSystem.EqualTo("Struck",TString::kIgnoreCase)&& stripNo < (fNStrips*fNDevices)/3) strip->SetSaturation(fSaturationStruck1);
      else if (fDAQSystem.EqualTo("Struck",TString::kIgnoreCase)&& stripNo >= (fNStrips*fNDevices)/3) strip->SetSaturation(fSaturationStruck23);
      else if (fDAQSystem.EqualTo("SRS",TString::kIgnoreCase)) strip->SetSaturation(fSaturationSRS);
      strip->SetChargeAndTimeBin();
      strip->SetAvgCharge();
      //      cout << "strip " << stripNo << " avg charge " << strip->GetAvgCharge() << " cut " << cut << " pedrms " << fPedRMS->GetBinContent(stripNo+1) << " pedmean " << fPedMean->GetBinContent(stripNo+1) << endl;
      vector<Int_t> CoordVec;
      CoordVec.push_back(xCoord); CoordVec.push_back(yCoord);
      strip->SetCoordinates(CoordVec);
      strip->SetHitNo(padCoordinate);
      strip->SetPedMean(fPedMean->GetBinContent(stripNo+1));
      strip->SetPedRMS(fPedRMS->GetBinContent(stripNo+1));
      strip->SetHitSizeTime(hitSizeTimeMax);
      //   if (strip->GetAvgCharge() < cut) delete strip;
      // else {
        fHits_v.push_back(strip);
	// }
      // } 
  }
}

//////////////////////////////////////////////////////

void Processor::ProcessOfflinePedestal() {
  //   fPedMean->Reset();
  // fPedRMS->Reset();
 
  for (int stripNo=0; stripNo < fNStrips*fNDevices; ++stripNo) {
    Int_t padCoordinate = fStripMapping_v[stripNo];
    if (padCoordinate == -100) continue; // unused channels
    Int_t xCoord = fXMapping_v[stripNo];
    Int_t yCoord = fYMapping_v[stripNo];
    //    fPedestals_v[stripNo]->Reset(); 
    for (int timeBin=0; timeBin < fNTimeBins; ++timeBin) {
      (fPedestals_v[stripNo])->Fill(fRawData_m[padCoordinate][timeBin]);
      //        if (stripNo == 300 ) cout << "time bin " << timeBin << " data " << fRawData_m[padCoordinate][timeBin] << endl; 
    }
    // if (stripNo == 300) cout <<" strip " << stripNo <<  " mean " << (fPedestals_v[stripNo])->GetMean() << " nentries " << (fPedestals_v[stripNo])->GetEntries() << endl;
    /*   if (stripNo > 255 && stripNo < 384 ) { 
 fPedMean->Fill(padCoordinate,(fPedestals_v[stripNo])->GetMean());
    fPedRMS->Fill(padCoordinate,(fPedestals_v[stripNo])->GetRMS());
    }*/
    fPedMean->Fill(stripNo,(fPedestals_v[stripNo])->GetMean());
    fPedRMS->Fill(stripNo,(fPedestals_v[stripNo])->GetRMS());
  }
}

//////////////////////////////////////////////////////

void Processor::SetOfflinePedestal() {

    fPedMean->Reset();
    fPedRMS->Reset();
    //TFile *rmsFile = new TFile(rmsName, "RECREATE");  
    TFile *meanFile = new TFile(fConfig->GetPedMeanFile(), "READ");  
    TFile *rmsFile = new TFile(fConfig->GetPedRMSFile(), "READ");  
    fPedMean = (TH1F*) meanFile->Get("pedMean");
    fPedRMS = (TH1F*) rmsFile->Get("pedRMS");
    meanFile->Close();
    delete meanFile;
    rmsFile->Close();
    delete rmsFile;

}

//////////////////////////////////////////////////////

void Processor::SetOnlinePedestal(pair<Int_t,Int_t> pedRange) {

  fPedMean->Reset();
  fPedRMS->Reset();
  int stripCor = 0;
  vector<TH1F*> tbPeds;
  /*
  for (int timeBin=0; timeBin < 27 ; ++timeBin) {
    for (int stripNo=0; stripNo < fNStrips*fNDevices; ++stripNo) {
      Int_t padCoordinate = fStripMapping_v[stripNo]; 
      if (padCoordinate == -100) { if (stripNo < 10)  ++stripCor; continue; } // unused readout channel
      Int_t xCoord = fXMapping_v[stripNo];
      Int_t yCoord = fYMapping_v[stripNo];
      if (stripNo % 8 != 4) {
        
      }
    }
  }
  */
  for (int stripNo=0; stripNo < fNStrips*fNDevices; ++stripNo) {
    Int_t padCoordinate = fStripMapping_v[stripNo]; 
    if (padCoordinate == -100) { if (stripNo < 10)  ++stripCor; continue; } // unused readout channel
    Int_t xCoord = fXMapping_v[stripNo];
    Int_t yCoord = fYMapping_v[stripNo];
    TH1F *pedestals = new TH1F("Ped","Ped",2048,-0.5,2047.5);
    for (int timeBin=pedRange.first; timeBin <= pedRange.second; ++timeBin) {
      //cout << " time bin " << timeBin << endl;
      //cout << " pad coord " << padCoordinate << endl;
      //cout << " data " << fRawData_m[padCoordinate][timeBin] << endl;
      pedestals->Fill(fRawData_m[padCoordinate][timeBin]);
      //     if (stripNo == 200) cout <<" time bin " << timeBin << " ped " << fRawData_m[padCoordinate][timeBin] << endl;
    }

    fPedMean->Fill(stripNo,pedestals->GetMean());
    fPedRMS->Fill(stripNo,pedestals->GetRMS());
    //    if (stripNo == 200) cout << " strip " << stripNo << " ped mean " << pedestals->GetMean() << " ped rms " << pedestals->GetRMS() << endl;
    delete pedestals;
  }
}

//////////////////////////////////////////////////////

void Processor::WritePedestalHistos() {
  char meanName[50];
  sprintf(meanName,"../Analysis/Peds/PedMeanZDCBeam.root",fRunNum);
  TFile *meanFile = new TFile(meanName, "RECREATE");
  fPedMean->Write();
  meanFile->Close();
  char rmsName[50];
  sprintf(rmsName,"../Analysis/Peds/PedRMSZDCBeam.root",fRunNum);
  TFile *rmsFile = new TFile(rmsName, "RECREATE");  
  fPedRMS->Write();
  rmsFile->Close();
  delete meanFile;
  delete rmsFile;
  for (int i=0; i < fNStrips*fNDevices; ++i) {
    delete fPedestals_v[i];
  }
  delete meanFile;
  delete rmsFile;
}

/////////////////////////////////////////////////////

void Processor::QuickSort(int min, int max) {
  if (min < max) {
    int j = Partition(min,max);
    QuickSort(0,j-1);
    QuickSort(j+1,max);
  }
}

/////////////////////////////////////////////////////

int Processor::Partition(int min, int max) {
  int pivot = fHits_v[max]->GetHitNo();
  while (min < max) {
    while (fHits_v[min]->GetHitNo() < pivot) ++min;
    while (fHits_v[max]->GetHitNo() > pivot) --max;
    if (fHits_v[min]->GetHitNo() == fHits_v[max]->GetHitNo()) ++min;
    else if (min < max) {
      Hit *temp = fHits_v[min];
      fHits_v[min] = fHits_v[max];
      fHits_v[max] = temp;
    }
  }
  return max;
}

//################################################################
float Processor::NonlinCorrection(float adc) {
  // This correction is for pedestal subtracted ADC
  float correctedADC = 0;
  correctedADC = fNonlinFunc->Eval(adc);
  //  cout << "before " << adc  << " after " << correctedADC << endl;
  return correctedADC;
}
