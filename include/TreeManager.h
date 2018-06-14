#ifndef __TREEMANAGER__
#define __TREEMANAGER__

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>
#include <fstream>
#include <vector>


#include "TFile.h"
#include "TTree.h"
#include "TObject.h"
#include "TString.h"
#include "TList.h"

#include "Hit.h"
#include "Processor.h"
#include "Configuration.h"
#include "TROOT.h"
#endif

using namespace std ;

class TreeManager : public TObject {
	
  public:
    TreeManager(Configuration *config);
    TreeManager(int i=0){;} 
    ~TreeManager();
    void InitRootFile() ;
    void WriteRootFile() ;
    void FillObjectsInRoot(Processor *processor) ;
    void Reset();

  private:
    int fRunNum;
    int fNStrips;
    int fNDevices;
    TFile *fFile ;
    TTree * fStripTree;
    TString fOutputTrees;
    TString fRunName;
    TString fDirectoryName;
    int fNTimeBins;
    vector<Hit*> fHits_v;
    Processor *fProcessor;

    // Used for traditional TTree
    vector<Int_t> fXRowNo_v;
    vector<Int_t> fYRowNo_v;
    vector<Int_t> fStripNo_v;
    Int_t fStripEventNo;
    vector<Int_t> fTimeLength_v;
    vector<Int_t> fTimeSize_v;
    vector<Float_t> fPeakStripCharge_v;
    vector<Float_t> fAvgStripCharge_v; // peak charge averaged over 2 neighbors
    vector<Float_t> fIntStripCharge_v;
    vector<Float_t> fPedMean_v;
    vector<Float_t> fPedRMS_v;
    vector<Int_t> fMaxTimeBin_v;
    vector<Bool_t> fSaturation_v;
    vector<vector<Float_t> > fTimeBinCharge_vv;
    vector<vector<Float_t> > fRawTimeBinCharge_vv;
    ClassDef(TreeManager,1)
};

#endif

