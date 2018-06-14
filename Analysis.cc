/*
********************************
To compile, use makefile found in this directory
To run, start ROOT and type:
".L libAnalysis.so"
real_init();
"poncsopen(filename);"
"prun();"
********************************
*/

#include "Analysis.h"
#include <pmonitor.h>
#include <iostream>
#include <fstream>
#include <time.h> 
#include "Processor.h"
#include "TreeManager.h"
#include "TROOT.h"

HistoManager *fHistos;
Configuration *fConfig;
Processor *fProcessor;
TreeManager *fTrees;
Int_t fCount = 0;
clock_t fT;

int pinit() {
  return 0;
}

int real_init() {
  fCount = 0;
  if (fCount == 0) {
    fT = clock();
    fConfig = new Configuration("RunParameters.txt");
    fProcessor = new Processor("processor","processor",fConfig, "Analysis");
    if ((fConfig->GetRunMode()).EqualTo("MONITORING",TString::kIgnoreCase)) {
      fHistos = new HistoManager("histos","histos",fConfig);
      fHistos->InitRawDataHisto();
      fHistos->InitMonitoringHistos();
    }
    if ((fConfig->GetRunMode()).EqualTo("ANALYSIS",TString::kIgnoreCase)) {
      fTrees = new TreeManager(fConfig); 
    }
  }
  else {
    if ((fConfig->GetRunMode()).EqualTo("MONITORING",TString::kIgnoreCase)) {
      fHistos->Reset();
    }
  }
  fProcessor->LoadMapping(); 
  ++fCount;

  return 0;
}

void Write() {
  fTrees->WriteRootFile();
  cout << " writing ttrees " << endl;
}

void ResetCount() {
  fCount = 0;
  fT = clock();
  //if ((fConfig->GetRunMode()).EqualTo("ANALYSIS",TString::kIgnoreCase)) {
  //fTrees = new TreeManager(fConfig); 
  //}
}

int process_event(Event *e) {
  /*
  ofstream myfile;
  myfile.open("evtType.log", ios::app);
  char s[100];
  sprintf(s, "event %d event type %d  event address %d ", fCount, e->getEvtType(),e); 
  myfile << s << endl;
  myfile.close();
  */
  if (e->getEvtType() == 9 || e == 0 || e->getEvtType() == 0 || e->getEvtType() == 601) return 0;
  ++fCount;
  if ( fCount < 3) return 0; 
  //  if (e == 0) return 0;
  //  e->identify();
  //if (e->getPacket(21101) == 0) {cout << " empty packet " << endl; return 0;}
  /*  
  if (fCount == 500 ) { 
     if ( (fConfig->GetRunMode()).EqualTo("ANALYSIS",TString::kIgnoreCase) && (fConfig->GetOutputTrees()).EqualTo("YES",TString::kIgnoreCase)) {
      WriteTrees();
      fT = clock() - fT;
      printf ("Run Time: %f seconds.\n",((float)fT)/CLOCKS_PER_SEC);
      fT = clock(); 
      fProcessor->FreeMemory();
      delete fConfig;
      delete fProcessor;
      delete fTrees;
      
    }
  }
  */
     /*
  if ((fConfig->GetRunMode()).EqualTo("PEDESTAL",TString::kIgnoreCase)) fProcessor->WritePedestalHistos();
     return 0;
  } 
     */

  //if (fCount > 500) {return 0; cout << " free memory " << endl; fProcessor->FreeMemory(); return 0;}
  //  cout <<"  event " << fCount << endl;
  if (fCount % 500 == 0) cout << " Events processed: " << fCount <<  endl;
  
  // cout << " getting event type " << endl;
  if (e->getEvtType() == 12 || fCount > 11000 ) {
    fT = clock() - fT;
    printf ("Run Time: %f seconds.\n",((float)fT)/CLOCKS_PER_SEC);
    fT = clock(); 
    if ((fConfig->GetRunMode()).EqualTo("ANALYSIS",TString::kIgnoreCase) && (fConfig->GetOutputTrees()).EqualTo("YES",TString::kIgnoreCase)) {
      WriteTrees();
    }
    if ((fConfig->GetRunMode()).EqualTo("MONITORING",TString::kIgnoreCase) && (fConfig->GetOutputHistos()).EqualTo("YES",TString::kIgnoreCase)) {
      WriteHistos();
    }
    if ((fConfig->GetRunMode()).EqualTo("PEDESTAL",TString::kIgnoreCase)) fProcessor->WritePedestalHistos();
    fProcessor->FreeMemory();
    delete fConfig;
    delete fProcessor;
    delete fTrees;
    return 0;
  }
  if (fCount < 2 || e->getEvtType() == 12) return 0;
  fProcessor->InitializeEvent(e);
  // Note: UnpackData() must be updated whenever new readout system added
  fProcessor->UnpackData(); 
  if ((fConfig->GetRunMode()).EqualTo("PEDESTAL",TString::kIgnoreCase)) {
    fProcessor->ProcessOfflinePedestal();
    return 0;
  }
  if ((fConfig->GetPedType()).EqualTo("OFFLINEPEDESTAL",TString::kIgnoreCase)/* && fCount == 3*/) {fProcessor->SetOfflinePedestal();}  
  if ((fConfig->GetPedType()).EqualTo("ONLINEPEDESTAL",TString::kIgnoreCase)) fProcessor->SetOnlinePedestal(fConfig->GetOnlinePedRange());

  // Hits built in this step with pedestal corrections in space and time and hit limits set. 
  fProcessor->MakeCuts(); 
  if ((fConfig->GetRunMode()).EqualTo("MONITORING",TString::kIgnoreCase)) {
    fHistos->FillRawDataHisto(fProcessor);
    fHistos->FillMonitoringHistos(fProcessor);

  }
  if ((fConfig->GetRunMode()).EqualTo("ANALYSIS",TString::kIgnoreCase)) {
    fTrees->FillObjectsInRoot(fProcessor);
  }

  fProcessor->Reset();

  return 0;
}

int WriteHistos() {
  fHistos->WriteHistos(); 
  return 0;
}

int WriteTrees() {

  fTrees->WriteRootFile();

  return 0;
}

int FreeMemory() {
  fProcessor->Reset();
  return 0;
}
