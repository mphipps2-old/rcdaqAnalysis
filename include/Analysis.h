#ifndef ANALYSIS_H
#define ANALYSIS_H
#include "HistoManager.h"
#include "Configuration.h"

using namespace std;

int real_init();
int WriteHistos();
int WriteTrees();
int FreeMemory();
void ResetCount();
void Write();

#endif
