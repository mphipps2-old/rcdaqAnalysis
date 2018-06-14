#include "Cluster.h"

ClassImp(Cluster)

Cluster::Cluster(const char* name, const char* title,Configuration *config) {
  fClusterSizeX = 0; fClusterCharge = 0;
  fDAQSystem = config->GetDAQSystem();
  fChargeMethod = config->GetChargeMethod();
  fConfig = config;
  fReducedChi2 = 0;
}

//////////////////////////////////////////////////////////////////////////

Cluster::~Cluster() {;}

//////////////////////////////////////////////////////

void Cluster::FindCentroid() {
  Float_t charge;
  vector<Hit*> temp = fHitVec;
  Int_t nHits =  fHitVec.size() ;
  Int_t xRowNo;
  Float_t xCentroid = 0; 
  for (int i = 0; i < nHits; i++) {
    if (fChargeMethod.EqualTo("PeakBin",TString::kIgnoreCase)) charge = (temp[i])->GetMaxCharge();
    else if (fChargeMethod.EqualTo("IntegralCharge",TString::kIgnoreCase)) charge = (temp[i])->GetIntegralCharge();
    else charge = (temp[i])->GetAvgCharge();
    xRowNo = (temp[i])->GetXRowNo();
    fClusterCharge += charge;
    xCentroid += charge * xRowNo; 
  }
  fCentroid = xCentroid /= fClusterCharge; 
}

//////////////////////////////////////////////////////

void Cluster::SetClusterStats() {
  Int_t nHits = fHitVec.size();
  fClusterSizeX = 0;
  fMaxClusterHit = 0;
  fClusterSizeTime = 0;
  vector<Int_t> xVec;
  vector<Int_t>::iterator xIt;
  vector<Hit> xHits;
  for (int i = 0; i < nHits; i++) {
    Int_t x = (fHitVec[i])->GetXRowNo();
    xIt = find(xVec.begin(),xVec.end(),x);
    if (xIt == xVec.end()) {
      ++fClusterSizeX; 
      xVec.push_back(x);
    }
    Float_t charge = (fHitVec[i])->GetMaxCharge();
    if (charge > fMaxClusterHit) fMaxClusterHit = charge; 
    if ((fHitVec[i])->GetHitSizeTime() > fClusterSizeTime) fClusterSizeTime = (fHitVec[i])->GetHitSizeTime();
  }
}

///////////////////////////////////////////////////

void Cluster::GaussianCorrection() {
  Int_t nHits = (int) fHitVec.size();
  Float_t hitX[nHits] ; Float_t hitCharge[nHits];
  for (int i = 0; i < nHits; ++i) {
    hitX[i] = (fHitVec[i])->GetXRowNo();
    if (fChargeMethod.EqualTo("PeakBin",TString::kIgnoreCase)) hitCharge[i] = (fHitVec[i])->GetMaxCharge();
    else if (fChargeMethod.EqualTo("IntegralCharge",TString::kIgnoreCase)) hitCharge[i] = (fHitVec[i])->GetIntegralCharge();
    else hitCharge[i] = (fHitVec[i])->GetAvgCharge();
  }
  TGraph * g1 = new TGraph(nHits, hitX, hitCharge);
  TF1 * f1 = new TF1("f1", "gaus", hitX[0], hitX[nHits -1]);
  Float_t width = TMath::Abs(hitX[0] - hitX[nHits -1]);
  f1->SetParLimits(1, hitX[0], hitX[nHits -1]);
  f1->SetParLimits(2, 0.0, (Double_t) width);
  TFitResultPtr r = g1->Fit("f1","SQ");
  // TCanvas *c1 = new TCanvas("c1","c1");
  //  g1->Draw("APL");

  // Calculate Chi^2 by formula
  Float_t chi2 = 0;
  Float_t constant = r->Value(0);
  Float_t mean = r->Value(1);
  Float_t sigma = r->Value(2);
  for(int i = 0; i<nHits; i++){
    Float_t strip = hitX[i];
    Float_t charge = hitCharge[i];
    Float_t functionValue = constant*TMath::Exp(-0.5*TMath::Power(((strip-mean)/sigma),2));
    chi2 += TMath::Power((charge-functionValue),2)/charge;
  }
  Float_t stripDifference = TMath::Abs(r->Value(1) - fCentroid);
  fReducedChi2 = chi2 / r->Ndf();
  if (stripDifference < fConfig->GetGausFitStripCut()){
    fCentroid = r->Value(1);
  }
  else fReducedChi2 = 0;
  delete g1;
  delete f1;
}
