// Load and run using CINT (ROOT)
// Produces mapping txt files that are read into DAQ program
// Note: -100 indicates an unused channel
// Note: "#" used in txt file as comments and "," used as delimiters

#include <fstream>

using namespace std;

void Generator() {
  // Program inputs
  TString fileName = "ChevronMappingInverted.txt";
  Int_t nRowsX = 24;
  Int_t nRowsY = 5;
  Int_t channel = 0;

  ofstream file;
  file.open(fileName, ofstream::out | ofstream::trunc);
  file << "###########################################" << endl;
  file << "#Channel,        Pad,        X,        Y###" << endl;
  file << "###########################################" << endl;

 Int_t xMapTemplate[10] = {1,1,1,1,2,1,2,2,2,2};
 Int_t yMapTemplate[10] = {3,2,5,4,2,1,4,3,1,5};

 Int_t round = -1;
 //APV Ch. Map:   APV0: 0-127,   APV1: 128-255,   APV2: 256-383,   APV3: 384-511, 
 Int_t xMap[512];
 Int_t yMap[512];
 Int_t padMap[512];

 //APV0:
 for(int i=127; i>=0; i--) {
   if (i>=120) { // first 8 channels unused
     xMap[i] = -100;
     yMap[i] = -100;
     padMap[i] = -100;
      file << i << ",         " << -100 << ",          " << -100 << ",           " << -100 << endl;   
   }
   else  {
     if ( (i+2) % 10 == 0) round++;
     xMap[i] = xMapTemplate[(i+2) % 10] + 2*(round);
     yMap[i] = yMapTemplate[(i+2) % 10];
     padMap[i] = ((yMap[i]-1)*nRowsX)+ (xMap[i]-1);
     file << i << ",         " << 128-padMap[i] << ",          " << xMap[i]-1 << ",           " << yMap[i]-1 << endl;   
   }
 }
 //APV1:

 for (int i=255; i>=128; i--) {
   if (i>=248) {
     xMap[i] = -100;
     yMap[i] = -100;
     padMap[i] = -100;
     file << i << ",         " << -100 << ",          " << -100 << ",           " << -100 << endl;   
   }
   else {
     xMap[i] = xMap[i-128]+24;
     yMap[i] = yMap[i-128];
     padMap[i] =  ((yMap[i]-1+10)*nRowsX)+ (xMap[i]-1-24);
     file << i << ",         " << padMap[i] << ",          " << xMap[i]-1-24 << ",           " << yMap[i]-1+10 << endl;   
   }
 }
 //APV2:

 for (int i=256; i<384; i++) {
   if (i<264) {
     xMap[i] = -100;
     yMap[i] = -100;
     padMap[i] = -100;
     file << i << ",         " << -100 << ",          " << -100 << ",           " << -100 << endl;   
   }
   else {
     xMap[i] = 49-xMap[i-256];
     yMap[i] = 11-yMap[i-256];
     padMap[i] =  ((yMap[i]-1+10)*nRowsX)+ (xMap[i]-1-24);
     file << i << ",         " << padMap[i] << ",          " << xMap[i]-1-24 << ",           " << yMap[i]-1+10 << endl;  
   }
 }
 //APV3:
 
 for (int i=384; i<512; i++) {
   if (i<392) {
     xMap[i] = -100;
     yMap[i] = -100;
     padMap[i] = -100;
     file << i << ",         " << -100 << ",          " << -100 << ",           " << -100 << endl;   
   }
   else {
     xMap[i] = 25-xMap[i-384];
     yMap[i] = 11-yMap[i-384];
     padMap[i] =  ((yMap[i]-1)*nRowsX)+ (xMap[i]-1);
     file << i << ",         " << padMap[i] << ",          " << xMap[i]-1 << ",           " << yMap[i]-1 << endl;  
   }
 }
 file.close();
 cout << "Mapping file successfully created" << endl;
}
