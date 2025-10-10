#include "../clsAppContext.h"
#include "clsForm1Context.h"
void clsForm1Context::cbClose(){
 Fl_Double_Window * parent_win=ParentWin();
 parent_win->activate();
 Win()->hide();
};


