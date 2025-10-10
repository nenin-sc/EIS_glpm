#include "clsForm1Context.h"
#include "../clsAppContext.h"
clsForm1Context::clsForm1Context()
{
    //ctor
    parent_win=nullptr;
}

clsForm1Context::~clsForm1Context()
{
    //dtor
}

void clsForm1Context::ParentWin(  Fl_Double_Window *w){
   clsGenContext::ParentWin(w);
   clsAppContext * pcntx=(clsAppContext *) w->user_data();
   pcntx->FillResults(fFrq,vZ);
   FillPlots();
}
void clsForm1Context::InitWidgets(){

 InitPlot();
  {
  auto ch=GetWg<Fl_Choice>("ch_mod_plt");

  ch->add("Nyquist",0,nullptr,(void*)0);
  ch->add("Bode(Z.Re,Z.Im,f)",0,nullptr,(void*)0);
  ch->add("Bode(|Z|,fi,f)",0,nullptr,(void*)0);
  ch->value(0);
 }
 };
