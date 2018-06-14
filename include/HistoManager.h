#ifndef HISTOMANAGER_H
#define HISTOMANAGER_H

//#if !defined(__CINT__) || defined(__MAKECINT__)
#include "Processor.h"
#include "Configuration.h"
#include "Hit.h"
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TList.h>
#include <TIterator.h>
#include <TLatex.h>
#include <TObject.h>
#include <vector>
#include <map>
#include <cstdio>
#include <Rtypes.h>
#include <TF1.h>
#include <TMath.h>
#include <TROOT.h>
#include <pmonitor.h>
//#endif

using namespace std;

class HistoManager: public TNamed {

  public:
    HistoManager(const char* name, const char* title,Configuration *config);
    ~HistoManager();
    void InitMonitoringHistos();
    void InitRawDataHisto();
    void FillMonitoringHistos(Processor *processor);
    void FillRawDataHisto(Processor *processor);
    void Reset();
    void WriteHistos();

  private:
    Configuration *fConfig;
    TH2F * fRawSamplesXY;
    TH2F * fRawSamplesST;
    TH2F * fSamplesXT;
    TH2F * fSamplesYT;
    TH2F * fSamplesST;
    TH2F * fSamplesXY;
    TCanvas * fC1;
    vector<TH1F*> fRawSamplesVec;
    vector<TH1F*> fSamplesVec;
    TH1F * fStripOccupancyX;
    TH1F * fStripOccupancyY;
    TH1F * fStripOccupancyS;
    TH1F * fTimeDist;
    TH2F * fHitMap;
    TH2F * fChargeMap;
    TH2F * fChargeMapAvg;
    TH2F * fChargeMapInt;
    // TH1F * fChargeDiff;
    TString fDAQSystem;    
    Int_t fNTimeBins;
    Int_t fDevNum;
    Int_t fTimeBinSize;
    Int_t fNRowsX;
    Int_t fNRowsY;
    Int_t fNDevices;
    Int_t fNChannels;
    Int_t fSatCount;
    Int_t fPitchX;
    Processor *fProcessor;

    ClassDef(HistoManager,1);
};

#endif

