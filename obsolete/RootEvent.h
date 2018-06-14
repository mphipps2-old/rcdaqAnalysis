#ifndef ROOTEVENT_H
#define ROOTEVENT_H

//#if !defined(__CINT__) || defined(__MAKECINT__)
#include <Rtypes.h>
#include <TObject.h>

using namespace std ;
class RootEvent : public TObject {
	
  public:
    RootEvent(int i=0) {fClusterMult = 0;}
    ~RootEvent(){;}
    Int_t GetClusterMult() {return fClusterMult;}
    Int_t GetEventNum() {return fEventNum;}
    void SetClusterMult(Int_t clusterMult) {fClusterMult = clusterMult;}
    void SetEventNum(Int_t eventNum) {fEventNum = eventNum;}

  private:
    Int_t fClusterMult;
    Int_t fEventNum;
 
    ClassDef(RootEvent,1)
};

#endif
