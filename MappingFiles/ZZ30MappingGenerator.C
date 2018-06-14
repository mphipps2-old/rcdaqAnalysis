// Load and run using CINT (ROOT)
// Produces mapping txt files that are read into DAQ program
// Note: -100 indicates an unused channel

#include <fstream>

using namespace std;

void Generator() {
  // Program inputs
  TString fileName = "ZZ30Mapping.txt";
  Int_t nDevices = 4;
  Int_t nChannels = 128;

  ofstream file;
  file.open(fileName, ofstream::out | ofstream::trunc);
  file << "###########################################" << endl;
  file << "#Channel,        Pad,        X,        Y  #" << endl;
  file << "###########################################" << endl;

  for (int device=0; device < nDevices; ++device) {
    for (int chNo=0; chNo < nChannels; ++chNo) {
      Int_t x = chNo+(device*128);
      if (x%2 == 1) x = ((x-1)/2) + 128;
      else x = ((x/2)+64) + 128;
      Int_t y = 0;
      Int_t pad = x;
      if (device != 2 ) {x = -100; y=-100; pad=-100;}
      file << (device*nChannels) + chNo<< ",               " << pad << ",          " << x << ",         " << y << endl;   
    }
  }
  file.close();
  cout << "Mapping file successfully created" << endl;
}
