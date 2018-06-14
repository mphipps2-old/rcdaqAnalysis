#include "Configuration.h"

ClassImp(Configuration)

Configuration::Configuration(TString configFile) {
  Load((char*) configFile.Data());
}

//////////////////////////////////////////////////////

Configuration::~Configuration() {
;
}

//////////////////////////////////////////////////////

void Configuration::Load(char* configFile) {
  ifstream file(gSystem->ExpandPathName(configFile), ifstream::in);
  if(!file.is_open()) cerr << "Configuration::Load(): file not open" << endl;
  TString line;
  while (line.ReadLine(file)) {
  // strip leading spaces and skip comments
    line.Remove(TString::kBoth, ' ');
    if(line.BeginsWith("#")) continue;        	
    if(line.BeginsWith("RUNNAME")) {
      char runName[1000];
      sscanf(line.Data(), "RUNNAME %s", runName);
      fRunName = runName;
    }
    if(line.BeginsWith("RUNMODE")) {
      char runMode[1000];
      sscanf(line.Data(), "RUNMODE %s", runMode);
      fRunMode = runMode;
    }
    if(line.BeginsWith("NSIGMA")) {
      char nSigma[1000];
      sscanf(line.Data(), "NSIGMA %s", nSigma);
      fNSigma = atoi(nSigma);
    }
   if(line.BeginsWith("NTIMEBINS")) {
      char nTimeBins[1000]; 
      sscanf(line.Data(), "NTIMEBINS %s", nTimeBins);
      fNTimeBins = atoi(nTimeBins);
    }
    if(line.BeginsWith("NCHANNELS")) {
      char nCh[1000]; 
      sscanf(line.Data(), "NCHANNELS %s", nCh);
      fNChannels = atoi(nCh);
    }
    if(line.BeginsWith("NPACKETS")) {
      char nPackets[1000]; 
      sscanf(line.Data(), "NPACKETS %s", nPackets);
      fNPackets = atoi(nPackets);
    }
    if(line.BeginsWith("BASELINE")) {
      char baseline[1000]; 
      sscanf(line.Data(), "BASELINE %s", baseline);
      fBaseline = atoi(baseline);
    }
   if(line.BeginsWith("TIMEBINSIZE")) {
      char timeBinSize[1000]; 
      sscanf(line.Data(), "TIMEBINSIZE %s", timeBinSize);
      fTimeBinSize = atoi(timeBinSize);
    }
   if(line.BeginsWith("XPITCH")) {
      char xPitch[1000]; 
      sscanf(line.Data(), "XPITCH %s", xPitch);
      fPitchX = atoi(xPitch);
    }
   if(line.BeginsWith("DEVICENUMBER")) {
      char devNum[1000]; 
      sscanf(line.Data(), "DEVICENUMBER %s", devNum);
      fDevNum = atoi(devNum);
    }
   if(line.BeginsWith("MINHITSIZE")) {
      char minHitSize[1000]; 
      sscanf(line.Data(), "MINHITSIZE %s", minHitSize);
      fMinHitSize = atoi(minHitSize);
    }
    if(line.BeginsWith("CHANNELCALIBRATION")) {
      char channelCalibration[1000];
      sscanf(line.Data(), "CHANNEL CALIBRATION %s", channelCalibration);
      fChCalibration = channelCalibration;
    }
    if(line.BeginsWith("DRSTRN")) {
      char drsTrn[1000];
      sscanf(line.Data(), "DRSTRN %s", drsTrn);
      fDRSTrn = drsTrn;
    }
    if(line.BeginsWith("DAQ")) {
      char daq[1000]; 
      sscanf(line.Data(), "DAQ %s", daq);
      fDAQ = daq;
    }
   if(line.BeginsWith("NREADOUTDEVICES")) {
      char nDevices[1000]; 
      sscanf(line.Data(), "NREADOUTDEVICES %s", nDevices);
      fNDevices = atoi(nDevices);
    }
   if(line.BeginsWith("NROWSX")) {
      char nRowsX[1000]; 
      sscanf(line.Data(), "NROWSX %s", nRowsX);
      fNRowsX = atoi(nRowsX);
    }
   if(line.BeginsWith("NROWSY")) {
      char nRowsY[1000]; 
      sscanf(line.Data(), "NROWSY %s", nRowsY);
      fNRowsY = atoi(nRowsY);
    }
    if(line.BeginsWith("INVERTSIGNAL")) {
      char invert[1000]; 
      sscanf(line.Data(), "INVERTSIGNAL %s", invert);
      fInvertSignal = invert;
    }
    if(line.BeginsWith("CUTTYPE")) {
      char cutType[1000]; 
      sscanf(line.Data(), "CUTTYPE %s", cutType);
      fCutType = cutType;
    }
    if(line.BeginsWith("BINARYCUT")) {  
      char bCut[1000];  
      sscanf(line.Data(), "BINARYCUT %s", bCut);  
      fBinaryCut = atoi(bCut); 
    }
    if(line.BeginsWith("CHARGEMETHOD")) {
      char chargeMethod[1000]; 
      sscanf(line.Data(), "CHARGEMETHOD %s", chargeMethod);
      fChargeMethod = chargeMethod;
    }
    if(line.BeginsWith("MAXCLUSTERSIZE")) {
      char maxClustSize[1000];
      sscanf(line.Data(), "MAXCLUSTERSIZE %s", maxClustSize);
      fMaxClustSize = atoi(maxClustSize);
    }
    if(line.BeginsWith("GAUSSIANCORRECTION")) {  
      char gaussianCorrection[1000];  
      sscanf(line.Data(), "GAUSSIANCORRECTION %s", gaussianCorrection);  
      fGaussianCorrection = gaussianCorrection; 
    }
   if(line.BeginsWith("GAUSSTRIPCUT")) {
      char gausStripCut[1000]; 
      sscanf(line.Data(), "GAUSSTRIPCUT %s", gausStripCut);
      fGausStripCut = atoi(gausStripCut);
    }
   if(line.BeginsWith("GAUSCHI2CUT")) {
      char gausChi2Cut[1000]; 
      sscanf(line.Data(), "GAUSCHI2CUT %s", gausChi2Cut);
      fGausChi2Cut = atoi(gausChi2Cut);
    }
   if(line.BeginsWith("GAUSCLUSTERSIZECUT")) {
      char gausClusterSizeCut[1000]; 
      sscanf(line.Data(), "GAUSCLUSTERSIZECUT %s", gausClusterSizeCut);
      fGausClustSizeCut = atoi(gausClusterSizeCut);
    }
    if(line.BeginsWith("RAWPEDMEANFILE")) {  
      char rawPedMeanFile[1000];  
      sscanf(line.Data(), "RAWPEDMEANFILE %s", rawPedMeanFile);  
      fRawPedMeanFile = rawPedMeanFile; 
    }
    if(line.BeginsWith("RAWPEDRMSFILE")) {  
      char rawPedRMSFile[1000];  
      sscanf(line.Data(), "RAWPEDRMSFILE %s", rawPedRMSFile);  
      fRawPedRMSFile = rawPedRMSFile; 
    }
    if(line.BeginsWith("PEDMEANFILE")) {  
      char pedMeanFile[1000];  
      sscanf(line.Data(), "PEDMEANFILE %s", pedMeanFile);  
      fPedMeanFile = pedMeanFile; 
    }
    if(line.BeginsWith("PEDRMSFILE")) {  
      char pedRMSFile[1000];  
      sscanf(line.Data(), "PEDRMSFILE %s", pedRMSFile);  
      fPedRMSFile = pedRMSFile; 
    }
    if(line.BeginsWith("CALIBFILE")) {
      char calibrationFile[1000];
      sscanf(line.Data(), "CALIBFILE %s", calibrationFile);
      fCalibrationFile = calibrationFile;
    }
    if(line.BeginsWith("MAPPINGFILE")) {
      char mappingFile[1000];
      sscanf(line.Data(), "MAPPINGFILE %s", mappingFile);
      fMappingFile = mappingFile;
    }
    if(line.BeginsWith("DEVICEMAPPINGFILE")) {
      char deviceMappingFile[1000];
      sscanf(line.Data(), "DEVICEMAPPINGFILE %s", deviceMappingFile);
      fDeviceMappingFile = deviceMappingFile;
    }
    if(line.BeginsWith("TTREES")) {
      char tTrees[1000];
      sscanf(line.Data(), "TTREES %s", tTrees);
      fTTrees = tTrees;
    }
    if(line.BeginsWith("THISTOS")) {
      char tHistos[1000];
      sscanf(line.Data(), " THISTOS %s", tHistos);
      fTHistos = tHistos;
    }
    if(line.BeginsWith("OUTPUTTREES")) {
      char outputTrees[1000];
      sscanf(line.Data(), " OUTPUTTREES %s", outputTrees);
      fOutputTrees = outputTrees;
    }
    if(line.BeginsWith("OUTPUTHISTOS")) {
      char outputHistos[1000];
      sscanf(line.Data(), " OUTPUTHISTOS %s", outputHistos);
      fOutputHistos = outputHistos;
    }
    if(line.BeginsWith("PEDTYPE")) {
      char pedType[1000];
      sscanf(line.Data(), "PEDTYPE %s", pedType);
      fPedType = pedType;
    }
    if(line.BeginsWith("TREEDIRECTORY")) {
      char treeDirectory[1000];
      sscanf(line.Data(), "TREEDIRECTORY %s", treeDirectory);
      fTreeDirectory = treeDirectory;
    }
    if(line.BeginsWith("FIRSTONLINEPEDTIMEBIN")) {
      char firstBin[1000]; 
      sscanf(line.Data(), "FIRSTONLINEPEDTIMEBIN %s", firstBin);
      fFirstOnlinePedTB = atoi(firstBin);
    }
    if(line.BeginsWith("LASTONLINEPEDTIMEBIN")) {
      char lastBin[1000];
      sscanf(line.Data(), "LASTONLINEPEDTIMEBIN %s", lastBin);
      fLastOnlinePedTB = atoi(lastBin);
    }
  } 
}

