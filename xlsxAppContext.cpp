
#include <complex>
#include <Xlsx/Chart.h>
#include <Xlsx/Chartsheet.h>
#include <Xlsx/Workbook.h>
#include <XLSXColors/XLSXColorLib.h>
#include <FL/Fl_Native_File_Chooser.H>
#include <scratch/clsScratchTable.h>
#include <FL/fl_ask.H>
#include <FL/filename.H>
#include <Fl/user/fl_small_stuff.h>
#include "clsAppContext.h"
using namespace SimpleXlsx;
using namespace ScratchTable;

static bool cmp_Zr(const clsAppContext::clsZrec & a,const clsAppContext::clsZrec & b) {return a.Z.real()<b.Z.real();};



void clsAppContext::ExportXLSX(std::vector<clsZrec> & ztable){

CWorkbook book;
  XLSXColorLib cl_lib;
  make_excell_like_named_colors(cl_lib);
 CWorksheet * sh_info;
 size_t ZeroCol=8;
 double Area=atof(GetWg<Fl_Float_Input>("inpf_Area")->value());
{
  CWorksheet &sheet = book.AddSheet("info");
  sh_info = &sheet;
  clsScratchTable Tbl;
  clsCell cl;
   size_t col=0,row=0;
  Tbl.Add(cl.Set(row,col++,"Area"));
  Tbl.Add(cl.Set(row,col++,Area));
  Tbl.Add(cl.Set(row++,col,"cm\u00B2"));
  row++;
  col=0;
  for(auto const &mtd:MetaData){
   Tbl.Add(cl.Set(row,col,mtd.first));
   Tbl.Add(cl.Set(row++,col+1,mtd.second));
  }

    row=0;
    col=ZeroCol;
    Tbl.Add(cl.Set(row,col++,"log₁₀(ƒ/Hz)"));
    Tbl.Add(cl.Set(row,col++,"Zero"));
    Tbl.Add(cl.Set(row++,col,"Z.Re"));


    auto mm=minmax_element(ztable.begin(),ztable.end(),sort_Zba);
   // auto &mmv=*mm;
    double dt=(log10(mm.second->f)-log10(mm.first->f))/(16.);
    auto mmZr=minmax_element(ztable.begin(),ztable.end(),cmp_Zr);
    double dtZr=(mmZr.second->Z.real()-mmZr.first->Z.real())/(16.);
    col=ZeroCol;
    Tbl.Add(cl.Set(row,  col++,log10(mm.first->f)-dt));
    Tbl.Add(cl.Set(row,  col++,0.0));
    Tbl.Add(cl.Set(row++,col,  mmZr.first->Z.real()-dtZr));
    col=ZeroCol;
    Tbl.Add(cl.Set(row,  col++,log10(mm.second->f)+dt));
    Tbl.Add(cl.Set(row,  col++,0.0));
    Tbl.Add(cl.Set(row,  col,  mmZr.second->Z.real()+dtZr));
    Tbl.InsertTo(sheet);
   };
 CWorksheet * sh_orig;
 {
    CWorksheet &sheet = book.AddSheet("data");
    sh_orig=&sheet;
    clsScratchTable Tbl;
    clsCell cl;
   size_t col=0,row=0;
   Tbl.Add(cl.Set(row,col++,"log₁₀(ƒ/Hz)"));
   Tbl.Add(cl.Set(row,col++,"Z.Re, \u03A9\u00B7cm\u00B2"));
   Tbl.Add(cl.Set(row,col++,"-Z.Im, \u03A9\u00B7cm\u00B2"));
   Tbl.Add(cl.Set(row,col++,"|Z|, \u03A9\u00B7cm\u00B2"));
   Tbl.Add(cl.Set(row,col++,"φ, rad"));
   Tbl.Add(cl.Set(row,col,"ƒ,Hz"));

   for(const auto & dt:ztable){
    Tbl.Add(cl.Set(++row,(col=0)++,log10(dt.f)));
    Tbl.Add(cl.Set(  row,col++,dt.Z.real()));
    Tbl.Add(cl.Set(  row,col++,-dt.Z.imag()));
    Tbl.Add(cl.Set(  row,col++,abs(dt.Z)));
    Tbl.Add(cl.Set(  row,col++,arg(dt.Z)));
    Tbl.Add(cl.Set(  row,col, dt.f));
   }
 Tbl.InsertTo(sheet);
 }
  {

     CChartsheet &scatter_chart = book.AddChartSheet("Nyquist", CHART_SCATTER);
     CChart::Series ser;

     ser.title = "Generated data";
     ser.catSheet =  sh_orig;
     ser.catAxisFrom = CellCoord(2,1);
     ser.catAxisTo = CellCoord(ztable.size()+1, 1);

     ser.valSheet =  sh_orig;
     ser.valAxisFrom = CellCoord(2,2);
     ser.valAxisTo = CellCoord(ztable.size()+1, 2);

     ser.JoinType = CChart::Series::joinNone;
     ser.Marker.Type = CChart::Series::symCircle;
     ser.Marker.Size=6;    // if true add diamond marks in each node of the sequence set
     ser.Marker.FillColor=cl_lib.GetColor("Gold");
     ser.Marker.LineColor=cl_lib.GetColor("Plum");
     ser.Marker.LineWidth=0.5;

     scatter_chart.Chart().AddSeries(ser);
{

     ser.title = "Zero";
     ser.catSheet =  sh_info;
     ser.catAxisFrom = CellCoord(2,ZeroCol+2);
     ser.catAxisTo = CellCoord(3, ZeroCol+2);

     ser.valSheet =  sh_info;
     ser.valAxisFrom = CellCoord(2,ZeroCol+1);
     ser.valAxisTo = CellCoord(3, ZeroCol+1);

     ser.JoinType = CChart::Series::joinLine;
     ser.DashType = CChart::Series::dashSolid;
     ser.LineColor=cl_lib.GetColor("Violet");
     ser.LineWidth=1.5;
     ser.Marker.Type = CChart::Series::symNone;


     scatter_chart.Chart().AddSeries(ser);
}


     scatter_chart.Chart().SetLegendPos(CChart::POS_TOP_ASIDE);
     scatter_chart.Chart().SetXAxisGrid(CChart::GRID_MAJOR_N_MINOR);
     scatter_chart.Chart().SetYAxisGrid(CChart::GRID_MAJOR_N_MINOR);
     scatter_chart.Chart().SetYAxisCross(CChart::CROSS_MIN);
     scatter_chart.Chart().SetXAxisCross(CChart::CROSS_MIN);

     scatter_chart.Chart().SetXAxisName("Z.Re, \u03A9\u00B7cm\u00B2");
     scatter_chart.Chart().SetYAxisName("-Z.Im, \u03A9\u00B7cm\u00B2");
     scatter_chart.Chart().SetDiagrammName("Nyquist plot");
  }

{

     CChartsheet &scatter_chart = book.AddChartSheet("Z(f)", CHART_SCATTER);
     scatter_chart.Chart().SetAddType( CHART_SCATTER );
     scatter_chart.Chart().SetDiagrammName("Generated bode plot");
     scatter_chart.Chart().SetXAxisName("log₁₀(ƒ/Hz)");
     scatter_chart.Chart().SetYAxisName("Z.Re, \u03A9\u00B7cm\u00B2");
     scatter_chart.Chart().SetY2AxisName("-Z.Im, \u03A9\u00B7cm\u00B2");
     scatter_chart.Chart().SetXAxisGrid(CChart::GRID_MAJOR_N_MINOR);
     scatter_chart.Chart().SetXAxisCross(CChart::CROSS_MIN);
     scatter_chart.Chart().SetYAxisCross(CChart::CROSS_MIN);
     scatter_chart.Chart().SetY2AxisCross(CChart::CROSS_MAX);

{
    CChart::Series ser;
     ser.title ="Z.Re, \u03A9\u00B7cm\u00B2";
     ser.catSheet =  sh_orig;
     ser.catAxisFrom = CellCoord(2,0);
     ser.catAxisTo = CellCoord(ztable.size()+1, 0);

     ser.valSheet =  sh_orig;
     ser.valAxisFrom = CellCoord(2,1);
     ser.valAxisTo = CellCoord(ztable.size()+1, 1);

     ser.JoinType = CChart::Series::joinNone;
     ser.Marker.Type = CChart::Series::symCircle;
     ser.Marker.Size=3;    // if true add diamond marks in each node of the sequence set
     ser.Marker.FillColor=cl_lib.GetColor("Gold");
     ser.Marker.LineColor=cl_lib.GetColor("Plum");
     ser.Marker.LineWidth=0.5;

     scatter_chart.Chart().AddSeries(ser);

}

{
    CChart::Series ser;
     ser.title ="Z.Im, \u03A9\u00B7cm\u00B2";
     ser.catSheet =  sh_orig;
     ser.catAxisFrom = CellCoord(2,0);
     ser.catAxisTo = CellCoord(ztable.size()+1, 0);

     ser.valSheet =  sh_orig;
     ser.valAxisFrom = CellCoord(2,2);
     ser.valAxisTo = CellCoord(ztable.size()+1, 2);

     ser.JoinType = CChart::Series::joinNone;
     ser.Marker.Type = CChart::Series::symCircle;
     ser.Marker.Size=3;    // if true add diamond marks in each node of the sequence set
     ser.Marker.FillColor=cl_lib.GetColor("Bright Green");
     ser.Marker.LineColor=cl_lib.GetColor("Dark Teal");
     ser.Marker.LineWidth=0.5;

     scatter_chart.Chart().AddSeries(ser,false);
}

}



 Fl_Native_File_Chooser native;
  native.title("Select a file");
  native.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
  native.filter("XLSX report\t*.xlsx\n");
  char fn1[2048]={0};
  strncpy(fn1,fl_filename_name(txout_datfname->value()),2047);
  fl_filename_setext(fn1,2047,".xlsx");

  native.preset_file(fn1);
  // Show native chooser
  const char * buf=fl_check_access_nfc(native,".xlsx","Export Excel file");
     if(buf!=nullptr){
          fl_cursor(FL_CURSOR_WAIT);
          FILE * dfl=fl_fopen(buf,"w");
           if(!book.Save( dfl,false) ) {
             fl_alert("XLSX file %s error.",buf);
           };
          fclose(dfl);
          fl_cursor(FL_CURSOR_DEFAULT );
   };
  };

