#ifndef CLSVIEWFNAMECNTX_H
#define CLSVIEWFNAMECNTX_H
#include <Fl/user/clsGenContext.h>
#include <Fl/Fl_Output.h>
#include "ViewFNameForm.h"


class Fl_OutputWithMenu:public Fl_Output
{

 public:
   //Fl_Choice *scale;
   Fl_Menu_Item *pulldown;
  // int mode;
   Fl_OutputWithMenu(int x, int y, int w, int h, const char *l=0):Fl_Output(x, y, w, h, l){

      };
   virtual ~Fl_OutputWithMenu();

   int handle(int e);
  // void text(const char* t) {txt=t;};
  // const char * text(const char* t) {txt.c_str();};
  // void mode(const int swc);
};

class clsViewFNameCntx:public clsGenContext
{
    public:
     clsViewFNameCntx();
     virtual ~clsViewFNameCntx();
     void InitWidgets();

//--- cb -------------------------
       void  cbClose();

    protected:

    private:
};

#endif //CLSVIEWFNAMECNTX_H
