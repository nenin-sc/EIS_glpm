#include "../clsAppContext.h"
#include "clsFormSwcContext.h"


void clsFormSwcContext::cbCancel(){
  status=true;
  Win()->hide();
  ParentWin()->activate();
  };
void clsFormSwcContext::cbOK(){
  status=false;
  Win()->hide();
  ParentWin()->activate();
  };
void clsFormSwcContext::cbOnOff(const bool state){
   GetWg<Fl_Check_Button>("cb_DFd")->value(state);
   GetWg<Fl_Check_Button>("cb_DFmd")->value(state);
   GetWg<Fl_Check_Button>("cb_DFrg")->value(state);
   GetWg<Fl_Check_Button>("cb_FTrg")->value(state);
   GetWg<Fl_Check_Button>("cb_Mod")->value(state);
  };
