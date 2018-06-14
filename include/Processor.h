#ifndef PROCESSOR_H
#define PROCESSOR_H

//#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TString.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TFile.h>
#include <TSystem.h>
#include <TIterator.h>
#include <TF1.h>
#include <TMath.h>
#include "Event.h"
#include "TObject.h"
#include <vector>
#include <map>
#include <cmath>
#include <pmonitor.h>
#include <fstream>
#include <utility>
//#endif

using namespace std;

class Hit;
class Configuration;
class Processor : public TNamed {

  public:
    Processor(const char* name, const char* title, Configuration *config, TString runType);
    ~Processor();
    void InitializeEvent(Event *e);
    void CalibrateChannels();
    void FreeMemory();
    void MakeCuts();
    void LoadMapping();
    void UnpackData();
    void SetOfflinePedestal();
    void SetOnlinePedestal(pair<Int_t,Int_t> pedRange);
    //void SetOfflineRawPedestal();
    //void ProcessOfflineRawPedestal();
    void ProcessOfflinePedestal();
    void QuickSort(int min, int max);
    void Reset();
    float NonlinCorrection(float adc);
    int Partition(int min, int max);
    void WritePedestalHistos();
    inline vector<Hit*> GetHitVec() {return fHits_v;}
    inline map<Int_t,vector<Float_t> > GetRawData() {return fRawData_m;}
    inline Int_t GetNTimeBins() {return fNTimeBins;}
    inline Int_t GetNStrips() {return fNStrips;}
    inline vector<Int_t> GetXMapping() {return fXMapping_v;}
    inline vector<Int_t> GetYMapping() {return fYMapping_v;}
    inline vector<Int_t> GetStripMapping() {return fStripMapping_v;}
    inline int GetPacketNb(int i) {return fDeviceMapping_v[i];}
    inline Int_t GetRunNum() {return fRunNum;}
    inline Event* GetEvent() {return fEvent;}
    inline Int_t GetDevNum() {return fDevNum;}

  private:
    vector<Hit*> fHits_v;
    vector<Float_t> fCalibration_v;
    TF1 *fNonlinFunc;
    Int_t fEventNum;
    Int_t fNChannels;
    Int_t fNStrips;
    Int_t fNRowsX;
    Int_t fNRowsY;
    Int_t fDevNum;
    Int_t fBaseline;
    Int_t fNPackets;
    pair<Int_t,Int_t> fPedRange;
    Int_t fNDevices;
    Int_t fNTimeBins;
    Int_t fSRSMaxADC;
    Int_t fStruck1MaxADC;
    Int_t fStruck23MaxADC;
    Int_t fDRSMaxADC;
    Int_t fBeamTestMaxADC;
    Int_t fSaturationSRS;
    Int_t fSaturationStruck1;
    Int_t fSaturationStruck23;
    Int_t fSaturationDRS;
    Int_t fNSigma;
    Int_t fBinaryCut;
    Int_t fRunNum;
    Int_t fMinHitSize;
    TString fCutType;
    TString fDAQSystem;
    TString fDRSTrn;
    TString fInvertSignal;
    map<Int_t,vector<Float_t> > fRawData_m;
    map<Int_t,vector<Float_t> > fData_m;
    vector<Packet*> fPacket_v;
    TH1F *fPedMean;
    TH1F *fPedRMS;
    TFile *fFilePedMean;
    TFile *fFilePedRMS;
    vector<TH1F*> fPedestals_v;
    // TH2F *fPed2d;
    //  TH2F *fStats2d;
    Configuration *fConfig; 
    Event *fEvent;
    vector<int> fXMapping_v; // data stored in terms of x,y coordinates so these maps needed to access coordinates anywhere in the code
    vector<int> fYMapping_v;
    vector<int> fStripMapping_v;
    vector<int> fDeviceMapping_v;
    ClassDef(Processor,1);
};

#endif

