#ifndef CLSFORM1CONTEXT_H
#define CLSFORM1CONTEXT_H
#include <vector>
#include <complex>
#include <Fl/user/Plot3.13/Fl_PlotXY3.h>
#include <Fl/user/clsGenContext.h>
#include "../rtb/Fl_rotated_label.h"
#include "form1.h"

class clsForm1Context:public clsGenContext
{
    public:
       using clsGenContext::ParentWin;
       virtual void ParentWin(  Fl_Double_Window *w);
        clsForm1Context();
        virtual ~clsForm1Context();
        void InitWidgets();
      // --- cb --------------------------
       void cbSwitchPlot(const int swc);
       void cbClose();
    protected:

      std::vector<double> fFrq;
      std::vector<std::complex<double>> vZ;
      void InitPlot();
      void FillPlots();
    private:
     std::vector<FL_PlotXY3_13::pxy_Plot*> regPlt;
};

#endif // CLSFORM1CONTEXT_H
