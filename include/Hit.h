#ifndef HIT_H
#define HIT_H

//#if !defined(__CINT__) || defined(__MAKECINT__)
#include <vector>
#include <iostream>
#include <Rtypes.h>
#include <TObject.h>
#include <TClass.h>
//#endif

using namespace std;

class Hit : public TNamed {

  public:
    Hit(const char* name, const char* title, vector<Float_t> timeBinCharges);
    Hit(int i=0){;}
    ~Hit();
    inline void SetHitSizeTime(Int_t hitSizeTime) {fHitSizeTime = hitSizeTime;}
    void SetChargeAndTimeBin();
    void SetAvgCharge();
    inline void SetClusterNum(Int_t cNum) {fClusterNum = cNum;}
    inline void SetEventNum(Int_t eNum) {fEventNum = eNum;}
    inline void SetCoordinates(vector<Int_t> coordinates) {fCoordinateVec = coordinates;}
    inline void SetHitNo(Int_t hitNo) {fHitNo = hitNo;}
    inline void SetRawTimeBinCharges(vector<Float_t> rawTimeBinCharges) {fRawTimeBinCharges = rawTimeBinCharges;}
    inline void SetSaturation(Int_t saturation) {fSaturationLevel = saturation;}
    inline void SetPedMean(Float_t pedMean) {fPedMean = pedMean;}
    inline void SetPedRMS(Float_t pedRMS) {fPedRMS = pedRMS;}
    inline vector<Int_t> GetCoordinates() {return fCoordinateVec;}
    inline Int_t GetXRowNo() {return fCoordinateVec[0];}
    inline Int_t GetYRowNo() {return fCoordinateVec[1];}
    inline Int_t GetHitNo() {return fHitNo;}
    inline Int_t GetClusterNum() {return fClusterNum;}
    inline Int_t GetEventNum() {return fEventNum;}
    inline Int_t GetHitSizeTime() {return fHitSizeTime;}
    inline Float_t GetIntegralCharge() {return fIntegralCharge;}
    inline Float_t GetAvgCharge() {return fAvgCharge;} // Avg over 3 time bins
    inline Float_t GetMaxCharge() {return fMaxCharge;}
    inline Float_t GetMaxTimeBin() {return fMaxTimeBin;}
    inline Bool_t GetSaturation() {return fSaturation;}
    inline Float_t GetTimeBinCharge(Int_t timeBin) {return fTimeBinCharges[timeBin];}
    inline Float_t GetPedMean() {return fPedMean;}
    inline Float_t GetPedRMS() {return fPedRMS;}
    inline vector<Float_t> GetTimeBinCharges() {return fTimeBinCharges;}
    inline vector<Float_t> GetRawTimeBinCharges() {return fRawTimeBinCharges;}
   
  private:
    Float_t fAvgCharge;
    Float_t fMaxCharge;
    Float_t fIntegralCharge;
    Float_t fPedRMS;
    Float_t fPedMean;
    Int_t fClusterNum;
    Int_t fEventNum;
    Float_t fMaxTimeBin;
    Int_t fHitSizeTime; // number of time bins above threshhold
    Bool_t fSaturation;
    Int_t fSaturationLevel;
    Int_t fHitNo;
    vector<Int_t> fCoordinateVec;
    vector<Float_t> fTimeBinCharges;
    vector<Float_t> fRawTimeBinCharges;

    //    ClassDef(Hit,1)
};

#endif

