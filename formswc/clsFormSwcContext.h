#ifndef CLSFORMSWCCONTEXT_H
#define CLSFORMSWCCONTEXT_H
#include <FL/user/Plot3.13/Fl_PlotXY3.h>
#include <Fl/user/clsGenContext.h>
#include "formswc.h"

class clsFormSwcContext:public clsGenContext
{
    public:
        clsFormSwcContext();
        virtual ~clsFormSwcContext();
     // --- cb --------------------------
     void cbOK();
     void cbCancel();
     void cbOnOff(const bool swc);
     bool isCanceled() const { return status;}
    protected:
     bool status; // false-  OK;
    private:
};

#endif // CLSFORMSWCCONTEXT_H
