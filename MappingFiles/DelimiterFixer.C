#include <TString.h>
#include <TObject.h>
#include <fstream>
#include <TIterator.h>
#include <TSystem.h>
#include <iostream>
#include <TObjString.h>
#include <vector>
#include <string>
#include <iterator>
#include <sstream>
#include <stdlib.h>

using namespace std;

vector<string> split(string const &input);

int main() {
    TString fileName = "ChevronPrototypeMapping1.txt";
TString fileName2 = "ChevronPrototypeMapping1b.txt";
 ifstream file(fileName.Data());
  if(!file.is_open()) cerr << "Processor::CalibrateChannels(): calibration file not open" << endl;
  TString line;
  // To correct SRS nonlinearity/response differences. Each channel normalized


  ofstream file2;
  file2.open(fileName2.Data(), ofstream::out | ofstream::trunc);
  file2 << "###########################################" << endl;
  file2 << "#Channel,        Pad,        X,        Y###" << endl;
  file2 << "###########################################" << endl;

  
    while (line.ReadLine(file)) {
  
    // strip leading spaces and skip comments
      line.Remove(TString::kBoth, ' ');
      if(line.BeginsWith("#")) continue;
      // Array of tokens separated by ","
      string str = line.Data();
      vector<string> vec;
      vec  = split(str);
      int sCount = 0;
      
      for (int i = 0; i < (int) vec.size(); ++i) {
	string str = vec[i];
        int val = atoi(str.c_str());
      file2 << val ;
      if (sCount < 3) file2 << ",      " ;
        sCount++;
      }
    file2 << endl;
    }
file.close();
file2.close();
cout << " mapping file created " << endl;
  return 0;
}

vector<string> split(string const &input) {
  istringstream buffer(input);
  vector<string> ret((istream_iterator<string>(buffer)),istream_iterator<string>());
  return ret;
}
