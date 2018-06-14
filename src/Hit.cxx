#include "Hit.h"
//ClassImp(Hit)


Hit::Hit(const char* name, const char* title, vector<Float_t> timeBinCharges) {
  fTimeBinCharges = timeBinCharges;
}

//////////////////////////////////////////////////////

Hit::~Hit() {;}

/////////////////////////////////////////////////////
//Called in Processor::MakeCuts()
void Hit::SetChargeAndTimeBin() {
  fMaxCharge=0;
  fMaxTimeBin=0;
  fIntegralCharge = 0;
  for (int timeBin=0; timeBin < (int) fTimeBinCharges.size(); ++timeBin) {
    if (fTimeBinCharges[timeBin] > fMaxCharge) {
      fMaxCharge = fTimeBinCharges[timeBin];
      fMaxTimeBin = timeBin;
    }
    fIntegralCharge += fTimeBinCharges[timeBin];
  }
}

//////////////////////////////////////////////////////

//Called in Processor::MakeCuts()
void Hit::SetAvgCharge() {
  fAvgCharge = fMaxCharge + fTimeBinCharges[fMaxTimeBin-1] + fTimeBinCharges[fMaxTimeBin+1];
  fAvgCharge /= 3;
  fSaturation = 0;
  // saturation 
  if (fAvgCharge > fSaturationLevel) fSaturation = 1;
}
