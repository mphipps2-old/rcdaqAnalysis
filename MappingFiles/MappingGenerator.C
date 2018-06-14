// Load and run using CINT (ROOT)
// Produces mapping txt files that are read into DAQ program
// Note: -100 indicates an unused channel

#include <fstream>

using namespace std;

void Generator() {
  // Program inputs
  TString fileName = "SRSMapping.txt";
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
 
 //APV0:
 for(int i=0; i<128; i++) {
   if (i<8) { // first 8 channels unused
      file << i << ",         " << -100 << ",          " << -100 << ",           " << -100 << endl;   
   }
   else if (i >= 8) {
     if ( (i+2) % 10 == 0) round++;
     Int_t x = xMapTemplate[(i+2) % 10] + 2*(round) -1;
     Int_t y = yMapTemplate[(i+2) % 10] -1;
     Int_t pad = (y*nRowsX)+ x;
     file << i << ",         " << pad << ",          " << x << ",           " << y << endl;   
   }
 }
 //APV1:
 round = -1;
 for (int i=128; i<256; i++) {
   if (i<136) {
     file << i << ",         " << -100 << ",          " << -100 << ",           " << -100 << endl;   
   }
   else {
     if ( (i+2) % 10 == 0) round++;
     Int_t x = xMapTemplate[(i+2) % 10] + 2*(round) -1;
     Int_t y = yMapTemplate[(i+2) % 10] + 5 -1;
     Int_t pad = (y*nRowsX)+ x;
     file << i << ",         " << pad << ",          " << x << ",           " << y << endl;   
   }
 }
 //APV2:
 round = -1;
 for (int i=256; i<384; i++) {
   if (i<264) {
     file << i << ",         " << -100 << ",          " << -100 << ",           " << -100 << endl;   
   }
   else {
     if ( (i+2) % 10 == 0) round++;
     Int_t x = xMapTemplate[(i+2) % 10] + 2*(round) -1;
     Int_t y = yMapTemplate[(i+2) % 10] + 10 -1;
     Int_t pad = (y*nRowsX)+ x;
     file << i << ",         " << pad << ",          " << x << ",           " << y << endl;  
   }
 }
 //APV3:
 round = -1;
 for (int i=384; i<512; i++) {
   if (i<392) {
     file << i << ",         " << -100 << ",          " << -100 << ",           " << -100 << endl;   
   }
   else {
     if ( (i+2) % 10 == 0) round++;
     Int_t x = xMapTemplate[(i+2) % 10] + 2*(round) -1;
     Int_t y = yMapTemplate[(i+2) % 10] + 15 -1;
     Int_t pad = (y*nRowsX)+ x;
     file << i << ",         " << pad << ",          " << x << ",           " << y << endl;  
   }
 }
 file.close();
 cout << "Mapping file successfully created" << endl;
}
