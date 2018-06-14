#include <iostream>
#include <unistd.h>

void EventDisplay() {
  //    prun(1);
    //    h2->Draw("lego2");
    TTimer timer(1000);
  while (1) {

    timer.SetCommand("gSystem->ProcessEvents()");
    prun(1);
    c1->Modified();

  }
}



