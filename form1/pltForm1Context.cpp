#include <gsl/gsl_complex_math.h>
#include "clsForm1Context.h"

using namespace FL_PlotXY3_13;

void clsForm1Context::InitPlot(){
    auto  pxy_test = GetWg<Fl_PlotXY3>("pltxy3_mod");
    auto & crdOrigL=pxy_test->GetOriginBL();
    auto & crdOrigR=pxy_test->GetOriginBR();
    crdOrigR.On();
    crdOrigL.On();
    pxy_LineStyle axsY2={FL_BLUE,FL_SOLID,2};
    pxy_LineStyle axsXY={FL_BLACK,FL_SOLID,2};
    pxy_LineStyle tgst={FL_GREEN,FL_DOT,0};

    pxy_Axis & xAx =crdOrigL.GetXAxis();
    xAx.SetScaleType(xAx.SCL_PLAIN).SetTiksN(11).TickGridOff().SetAxStyle(axsXY).SetAutoTicks(true).SetAutoScale(true);
    pxy_Axis & yAxL =crdOrigL.GetYAxis();
    yAxL.SetScaleType(yAxL.SCL_PLAIN).SetTiksN(7).TickGridOff().SetAxStyle(axsXY).SetAutoTicks(true).SetAutoScale(true);
    pxy_Axis & yAxR =crdOrigR.GetYAxis();
    auto & gl=yAxL.NewGridLine("glZero");
    gl.SetLineStyle(tgst).On();
    yAxR.SetScaleType(yAxR.SCL_PLAIN).SetTiksN(7).TickGridOff().SetAxStyle(axsY2).SetAutoTicks(true).SetAutoScale(true);

    pxy_PlotStyle sty1, sty2, sty3;
    sty1.SetSym("XCross_3x3").SetSymColor(FL_BLACK).SetJoined(false);
    sty2.SetSym("Cross_3x3").SetSymColor(FL_BLUE).SetJoined(false);
    sty3.SetSym("Box_3x3").SetSymColor(FL_RED).SetJoined(false);
    regPlt={
    crdOrigL.NewPlot("pltNyq",&sty1),
    crdOrigL.NewPlot("pltBode_Re",&sty1),
    crdOrigL.NewPlot("pltBode_|Z|",&sty1),
    crdOrigR.NewPlot("pltBode_Im",&sty2),
    crdOrigR.NewPlot("pltBode_fi",&sty2)};
    cbSwitchPlot(0);
};
//----  --------------------------------------------------------------------------------------------------------

void clsForm1Context::cbSwitchPlot(const int swc){
  auto  pxy_test = GetWg<Fl_PlotXY3>("pltxy3_mod");
  auto & crdOrigL=pxy_test->GetOriginBL();
  auto & crdOrigR=pxy_test->GetOriginBR();
  pxy_Axis & yAxL =crdOrigL.GetYAxis();
  pxy_Axis & yAxR =crdOrigR.GetYAxis();
  auto xlab=GetWg<Fl_Output>("outx_Xlab");

  auto yLlab=GetWg<Fl_Rotated_Label>("rtl_YLax");

  auto yRlab=GetWg<Fl_Rotated_Label>("rtl_YRax");
  for(auto plt:regPlt)
      plt->GetStyle().SetOnOff(false);
 switch(swc){
  case 1:
        crdOrigR.On();
        yAxR.On();
        crdOrigL.GetPlot("pltBode_Re")->GetStyle().SetOnOff(true);
        crdOrigR.GetPlot("pltBode_Im")->GetStyle().SetOnOff(true);
        yAxL.LGridOff();
        xlab->value("log(f/Hz)");
        yLlab->text("Z.Re,Ω");
        yRlab->text("Z.Im,Ω");
        yRlab->show();
        break;
  case 2:
        crdOrigR.On();
        yAxR.On();
        crdOrigL.GetPlot("pltBode_|Z|")->GetStyle().SetOnOff(true);
        crdOrigR.GetPlot("pltBode_fi")->GetStyle().SetOnOff(true);
        yAxL.LGridOff();
        xlab->value("log(f/Hz)");
        yLlab->text("|Z|,Ω");
        yRlab->text("ϕ,rad");
        yRlab->show();
        break;

  default:
       crdOrigR.Off();
        yAxR.Off();
        crdOrigL.GetPlot("pltNyq")->GetStyle().SetOnOff(true);
        yAxL.LGridOn();
        xlab->value("Z.Re,Ω");
        yLlab->text("-Z.Im,Ω");
        yRlab->hide();
  }
  xlab->redraw();
  yLlab->redraw();
  yRlab->redraw();
  pxy_test->redraw();
}
//----  --------------------------------------------------------------------------------------------------------
void clsForm1Context::FillPlots(){
  auto  pxy_test = GetWg<Fl_PlotXY3>("pltxy3_mod");
  auto & crdOrigL=pxy_test->GetOriginBL();
  auto & crdOrigR=pxy_test->GetOriginBR();
  pxy_test->ClearPlots();
  auto pltNyq=crdOrigL.GetPlot("pltNyq");
  auto pltBode_Re=crdOrigL.GetPlot("pltBode_Re");
  auto pltBode_Z=crdOrigL.GetPlot("pltBode_|Z|");
  auto pltBode_Im=crdOrigR.GetPlot("pltBode_Im");
  auto pltBode_fi=crdOrigR.GetPlot("pltBode_fi");


  for(size_t idx=0;idx<fFrq.size();++idx){

    pltNyq->AddXY(vZ[idx].real(),-vZ[idx].imag());
    double lgf=log10(fFrq[idx]);
    pltBode_Re->AddXY(lgf,vZ[idx].real());
    pltBode_Im->AddXY(lgf,vZ[idx].imag());

    pltBode_Z->AddXY(lgf,abs(vZ[idx]));
    pltBode_fi->AddXY(lgf,arg(vZ[idx]));
  }
 cbSwitchPlot(0);

}
