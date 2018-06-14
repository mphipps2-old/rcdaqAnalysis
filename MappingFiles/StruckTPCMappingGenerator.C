// Load and run using CINT (ROOT)
// Produces mapping txt files that are read into DAQ program
// Note: -100 indicates an unused channel

#include <fstream>

using namespace std;

void Generator() {
  // Program inputs
  TString fileName = "DRS32FullMapping.txt";
  Int_t nDevices = 4;
  Int_t nChannels = 32;

  ofstream file;
  file.open(fileName, ofstream::out | ofstream::trunc);
  file << "###########################################" << endl;
  file << "#Channel,        Pad,        X,        Y  #" << endl;
  file << "###########################################" << endl;

  for (int device=0; device < nDevices; ++device) {
    for (int chNo=0; chNo < nChannels; ++chNo) {
      Int_t x = chNo+(device*8);
      Int_t y = 0;
      Int_t pad = x;
      file << (device*nChannels) + chNo<< ",               " << pad << ",          " << x << ",         " << y << endl;   
    }
  }
  file.close();
  cout << "Mapping file successfully created" << endl;
}
