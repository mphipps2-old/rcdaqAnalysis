#ifndef CLUSTER_H
#define CLUSTER_H

//#if !defined(__CINT__) || defined(__MAKECINT__)

#include <vector>
#include <algorithm>
#include "Hit.h"
#include "Configuration.h"
#include <Rtypes.h>
#include <TGraph.h>
#include <TF1.h>
#include <TH2F.h>
#include <TFitResultPtr.h>
#include <TFitResult.h>
#include <TMath.h>
#include <TObject.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TFile.h>
//#endif

using namespace std;

class Cluster : public TObject {

  public:
    Cluster(const char* name, const char* title,Configuration *config);
    Cluster(int i=0) {}
    ~Cluster();
    inline void AddHit(Hit *hit) {
      ++fClusterSizeX;
      fHitVec.push_back(hit);
    }
    inline void ClearArrayOfHits() {fHitVec.clear();}
    void FindCentroid();
    inline vector<Hit*> GetArrayOfHits() {return fHitVec;}
    inline Hit* GetHit(Int_t i) {
      return (Hit*) fHitVec[i];
    }
    inline Float_t GetXCentroid() {return fCentroid;}
    inline int GetClusterSizeX() {return fClusterSizeX;}
    inline Int_t GetClusterSizeTime() {return fClusterSizeTime;}
    inline Float_t GetMaxClusterHit() {return fMaxClusterHit;}
    inline float GetClusterCharge() {return fClusterCharge;}
    inline float GetChi2() {return fReducedChi2;}
    inline Int_t GetEventNum() {return fEventNum;}
    inline Int_t GetClusterNum() {return fClusterNum;}
    void GaussianCorrection();
    inline void SetCentroid(Float_t centroid) {fCentroid = centroid;}
    void SetClusterStats();
    inline void SetEventNum(Int_t eNum) {fEventNum = eNum;}
    inline void SetClusterNum(Int_t cNum) {fClusterNum = cNum;}

  private:
    int fClusterSizeX;
    Int_t fClusterSizeTime; // number of time bins above threshold for max hit in this cluster 
    Int_t fEventNum;
    Int_t fClusterNum;
    TString fDAQSystem;
    TString fChargeMethod;
    Float_t fMaxClusterHit;
    Float_t fClusterCharge;
    vector<Hit*> fHitVec;
    Float_t fCentroid;
    Float_t fReducedChi2;
    Configuration *fConfig;
    ClassDef(Cluster,1)
    
};

#endif
