//#include <fmt/format.h>
#include <locale>
#include <fmt/core.h>
#include <fmt/xchar.h>
#include <scn/scan.h>
#include <FL/filename.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/fl_ask.H>
#include <FL/user/fl_small_stuff.h>
#include "clsAppContext.h"

template <typename Char> struct numpunct_C : std::numpunct<Char> {
 protected:
  Char do_decimal_point() const override { return '.'; }

};

template <typename Char> struct numpunct_EU : std::numpunct<Char> {
 protected:
  Char do_decimal_point() const override { return ','; }

};
std::locale lc_LC_C(std::locale(), new numpunct_C<char>());

std::locale lc_LC_EU(std::locale(), new numpunct_EU<char>());


void clsAppContext::ExportTXT(std::vector<clsZrec> & ztable){

  char fn1[2048]={0};
  const char * csvf= "CSV file\t*.csv\n";
  const char * csve=".csv";
  const char * txte=".txt";
  const char * txtf= "Tab separated text table\t*.txt\n";
  const bool swctab=GetWg<Fl_Check_Button>("cb_tab_csv")->value();

  strncpy(fn1,fl_filename_name(txout_datfname->value()),2047);
  fl_filename_setext(fn1,2047, swctab?txte:csve);
  Fl_Native_File_Chooser native;
  native.title("Select file to save");
  native.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
  native.filter(swctab?txtf:csvf);

  native.preset_file(fn1);
   const char * buf=fl_check_access_nfc(native,swctab?txte:csve,swctab?txtf:csvf);

  if(buf==nullptr) return;
  FILE * wf=fl_fopen(buf,"w");
  if(!wf){
   fl_alert("Fatal write error: open file.\n .");
   return;
   }
 bool swcft=GetWg<Fl_Choice>("ch_loc_csv")->value();
  std::string colsep=swcft?";":",";
  if(swctab) colsep="\t";
  auto & lc_LC=swcft?lc_LC_EU:lc_LC_C;
  double zsign=GetWg<Fl_Check_Button>("cb_nzim_csv")->value()?-1.:1.;
  if(GetWg<Fl_Check_Button>("cb_col_csv")->value()){
   fmt::print(wf,"f(Hz){0:}Z.Re(Ohm*cm^2){0:}Z.Im(Ohm*cm^2)\n",colsep,zsign<0?"-":"");

  };
 if(GetWg<Fl_Choice>("ch_fsort_csv")->value()){
        std::sort(ztable.begin(),ztable.end(),sort_Zba);
   } else
       std::sort(ztable.begin(),ztable.end(),sort_Zab);
 int asr=GetWg<Fl_Check_Button>("cb_asr_csv")->value();
 double ar=(asr==1)?1.0:atof(GetWg<Fl_Float_Input>("inpf_Area")->value());
 for(const auto & dt:ztable){
    auto st=fmt::format(lc_LC,"{1:Lg}{0:}{2:Lg}{0:}{3:Lg}\n",colsep,dt.f,dt.Z.real()/ar,zsign*dt.Z.imag()/ar);
    fmt::print(wf,"{}",st);
  }
  fclose(wf);

};

void clsAppContext::cbLoadFTable(){
  Fl_Native_File_Chooser native;
  native.title("Select text file to load");
  native.type(Fl_Native_File_Chooser::BROWSE_FILE);
  native.filter("Text file\t*.txt\nCSV\t*.csv\nAny file\t*.*\n");
   switch ( native.show() ) {
    case -1:  fl_message("Text file open error\n, %s.", native.errmsg());  return;	// ERROR
    case  1:  fl_message("Text  file load canceled\n ."); return;		// CANCEL
    default: 								// PICKED FILE
      if ( !native.filename() ) {
         fl_message("No  file selected.");
          return;
      };
   };
  FILE * sfile=fl_fopen(native.filename(),"r");
   if (sfile == NULL){
            fl_alert("File \"%s\" open error.", native.filename());
             return ;
   };
   std::vector<std::string>  rawFile;

     char bufff [1001];
     while( fgets (bufff , 1000 , sfile) != nullptr ){
        rawFile.push_back(bufff);
     }
   fclose (sfile);
   if(rawFile.size()<2){
     fl_alert("File \"%s\" size too small (1 line or less).", native.filename());
            return;
   };
   mgenF.clear();
   int swcLC=GetWg<Fl_Choice>("ch_loc_imptxt")->value();
    auto & lc_LC=swcLC?lc_LC_EU:lc_LC_C;
   for(const auto &stv:rawFile){
    auto result = scn::scan<double>(lc_LC,stv,"{:Lf}");
     if (result){
       double t1rs=result->value();
       mgenF.push_back(t1rs);}
   }
   std::sort(mgenF.begin(),mgenF.end(),std::greater<double>());
   double maf=log10(mgenF.front());
   double mif=log10(mgenF.back());
   int ppd=mgenF.size()/(maf-mif);
   GetWg<Fl_Int_Input>("inpi_ppd")->value(fmt::format("{}",ppd).c_str());
   GetWg<Fl_Int_Input>("inpi_nPt")->value(fmt::format("{}",mgenF.size()).c_str());
};
