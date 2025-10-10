#include <math.h>
 #include <FL/Fl_Native_File_Chooser.H>
 #include <FL/fl_ask.H>
 #include <FL/user/fl_small_stuff.h>
#include <config/libconfig.h++>
#include <fmt/format.h>
#include "clsAppContext.h"


bool sort_Zab(const clsAppContext::clsZrec & a,const clsAppContext::clsZrec & b) {return a.f>b.f; }
bool sort_Zba(const clsAppContext::clsZrec & a,const clsAppContext::clsZrec & b) {return a.f<b.f; }




  void clsAppContext::_cbSubstractModelOutput(const bool swcSub){
   int eidx0=GetWg<Fl_Choice>("ch.eisdf.src")->value();
   int eidx1=GetWg<Fl_Choice>("ch.eisdf.src.ftbl")->value();
   int swcTbl0=GetWg<Fl_Check_Button>("cb.eisdf.FtbSrc")->value();
   int swcTbl1=GetWg<Fl_Check_Button>("cb.eisdf.FtbAd")->value();
   const auto & tbl0=(swcTbl0)?ftstor[eidx0]:eistor[eidx0];
   const auto & tbl1=(swcTbl1)?ftstor[eidx1]:eistor[eidx1];
  if(tbl1.size()!=tbl0.size())
  {
     fl_alert("Error: Incompatible data sets.");
   return;
  }
  std::vector<clsZrec> tZ;
  for(size_t i=0;i<tbl1.size();++i){
     clsZrec t;
     t.f=tbl0[i].f;
     if(swcSub) {
       t.Z=tbl0[i].Z- tbl1[i].Z;
        } else t.Z=tbl0[i].Z + tbl1[i].Z;
    tZ.push_back(t);
  }
  eistor[0]=tZ;
  MetaData.push_back({"Refined",swcSub?"Substract  element":"Add element"});
  GetWg<clsMetaDataTable>("tbl_Meta_Ed")->update();
   };
 void clsAppContext::_cbUnParModelOutput(){
   int eidx0=GetWg<Fl_Choice>("ch.eisdf.src")->value();
   int eidx1=GetWg<Fl_Choice>("ch.eisdf.src.ftbl")->value();
   int swcTbl0=GetWg<Fl_Check_Button>("cb.eisdf.FtbSrc")->value();
   int swcTbl1=GetWg<Fl_Check_Button>("cb.eisdf.FtbAd")->value();
   const auto & tbl0=(swcTbl0)?ftstor[eidx0]:eistor[eidx0];
   const auto & tbl1=(swcTbl1)?ftstor[eidx1]:eistor[eidx1];
  if(tbl1.size()!=tbl0.size())
  {
     fl_alert("Error: Incompatible data sets.");
   return;
  }
  std::vector<clsZrec> tZ;
  for(size_t i=0;i<tbl1.size();++i){
   clsZrec t;
   t.f=tbl0[i].f;
   t.Z=tbl0[i].Z*tbl1[i].Z/(tbl1[i].Z - tbl0[i].Z);
    tZ.push_back(t);
  }
  eistor[0]=tZ;
  MetaData.push_back({"Refined","Remove parallel element"});
  GetWg<clsMetaDataTable>("tbl_Meta_Ed")->update();
 };
    void clsAppContext::_cbParModelOutput(){
  int eidx0=GetWg<Fl_Choice>("ch.eisdf.src")->value();
   int eidx1=GetWg<Fl_Choice>("ch.eisdf.src.ftbl")->value();
   int swcTbl0=GetWg<Fl_Check_Button>("cb.eisdf.FtbSrc")->value();
   int swcTbl1=GetWg<Fl_Check_Button>("cb.eisdf.FtbAd")->value();
   const auto & tbl0=(swcTbl0)?ftstor[eidx0]:eistor[eidx0];
   const auto & tbl1=(swcTbl1)?ftstor[eidx1]:eistor[eidx1];
  if(tbl1.size()!=tbl0.size())
  {
     fl_alert("Error: Incompatible data sets.");
   return;
  }
  std::vector<clsZrec> tZ;
  for(size_t i=0;i<tbl1.size();++i){
   clsZrec t;
   t.f=tbl0[i].f;
   t.Z=1./(1./tbl0[i].Z + 1./tbl1[i].Z);
    tZ.push_back(t);
  }
  eistor[0]=tZ;
  MetaData.push_back({"Refined","Intorduce parallel element"});
  GetWg<clsMetaDataTable>("tbl_Meta_Ed")->update();

    };



 //  -----
 void clsAppContext::_cbScaleModelOutput(){
  int eidx=GetWg<Fl_Choice>("ch.eisdf.src")->value();
  int swcTbl0=GetWg<Fl_Check_Button>("cb.eisdf.FtbSrc")->value();
  const auto & tbl0=(swcTbl0)?ftstor[eidx]:eistor[eidx];
  double sc=GetWg<Fl_Value_Input>("ch.eisdf.scale")->value();
   std::vector<clsZrec> tZ;
   for(size_t i=0;i<tbl0.size();++i){
   clsZrec t;
   t.f=tbl0[i].f;
   t.Z=sc*tbl0[i].Z;
   tZ.push_back(t);
  }
  eistor[0]=tZ;
  MetaData.push_back({"Refined","Scaled"});
  GetWg<clsMetaDataTable>("tbl_Meta_Ed")->update();

 };

//------------------
void clsAppContext::_cbAppendModelOutput(){
   int eidx0=GetWg<Fl_Choice>("ch.eisdf.src")->value();
   int eidx1=GetWg<Fl_Choice>("ch.eisdf.src.ftbl")->value();
   int swcTbl0=GetWg<Fl_Check_Button>("cb.eisdf.FtbSrc")->value();
   int swcTbl1=GetWg<Fl_Check_Button>("cb.eisdf.FtbAd")->value();
   const auto & tbl0=(swcTbl0)?ftstor[eidx0]:eistor[eidx0];
   const auto & tbl1=(swcTbl1)?ftstor[eidx1]:eistor[eidx1];
  std::vector<clsZrec> tZ;
  for(const auto & dt:tbl0){
   clsZrec t;
   t.f=dt.f;
    t.Z=dt.Z;
    tZ.push_back(t);
  }
  for(const auto & dt:tbl1){
    tZ.push_back(dt);
  }
  std::sort(tZ.begin(),tZ.end(),sort_Zab);
  eistor[0]=tZ;
  MetaData.push_back({"Refined","Append"});
  GetWg<clsMetaDataTable>("tbl_Meta_Ed")->update();
  double t0=tbl0.front().f;
  double t1=tbl0.back().f;
  if(t0>t1) std::swap(t0,t1);
  MetaData.push_back({"Fmin1",fmt::format("{}",t0)});
  MetaData.push_back({"Fmax1",fmt::format("{}",t1)});
  t0=1./t0;
  t1=1./t1;
  if(t0>t1) std::swap(t0,t1);
  MetaData.push_back({"Taumin1",fmt::format("{}",t0)});
  MetaData.push_back({"Taumax1",fmt::format("{}",t1)});
  t0=tbl1.front().f;
  t1=tbl1.back().f;
  if(t0>t1) std::swap(t0,t1);
  MetaData.push_back({"Fmin2",fmt::format("{}",t0)});
  MetaData.push_back({"Fmax2",fmt::format("{}",t1)});
  t0=1./t0;
  t1=1./t1;
  if(t0>t1) std::swap(t0,t1);
  MetaData.push_back({"Taumin2",fmt::format("{}",t0)});
  MetaData.push_back({"Taumax2",fmt::format("{}",t1)});
  GetWg<clsMetaDataTable>("tbl_Meta_Ed")->update();
};
//---------------------------------
void clsAppContext::cbGenerateFTable(){


 double minf= atof(GetWg<Fl_Float_Input>("inpf_minFrq")->value());
 switch(GetWg<Fl_Choice>("ch_minFrqK")->value()){
  case 1:minf*=1000.;break;
  case 2:minf*=1.e6;break;
  default:;
 };
 if(minf<0)
 {
   fl_alert("Error: Min. frequency value %f should be positive",minf);
   return;
 }


 double maxf= atof(GetWg<Fl_Float_Input>("inpf_maxFrq")->value());
 switch(GetWg<Fl_Choice>("ch_maxFrqK")->value()){
  case 1:maxf*=1000.;break;
  case 2:maxf*=1.e6;break;
  default:;
 };


 if(maxf<0.0)
 {
   fl_alert("Error: Max. frequency value %g should be positive",maxf);
   return;
 }

 if(minf>=maxf){
    fl_alert("Error: Min. frequency %g must be lower than Max. frequency %g, Hz",minf,maxf);
   return;
 }
 uint32_t szPPD= atoi(GetWg<Fl_Int_Input>("inpi_ppd")->value());
 if(szPPD<2||szPPD>1000)
 {
   fl_alert("Error: Points/Decade value %d should be in range 2...1000",szPPD);
   return;
 }

 double lminf0 = floor(log10(minf));
 double lmaxf0 = ceil(log10(maxf));
// double dcs =  lmaxf0-lminf0;
 double dfrq= 1./(szPPD);
 mgenF.clear();

 for(double _fi=lminf0;_fi<=lmaxf0;_fi+=dfrq){
   double fri=pow(10,_fi);
   if(fri<minf) continue;
   if(fri>maxf) break;
   mgenF.push_back(fri);
 }
 GetWg<Fl_Int_Input>("inpi_nPt")->value(fmt::format("{}",mgenF.size()).c_str());
 MetaData.push_back({"Table","Generated"});
 GetWg<clsMetaDataTable>("tbl_Meta_Ed")->update();
};

//------------------------------------------
void clsAppContext::cbGenerateModelOutput(const size_t mout){

 if(!mgenF.size()){
  fl_alert("Error: Empty frequency table!.");
  return;
 }


///- Compilation May need Error treatment!!!--------------
 if(CheckEISCalc()) return;
 std::vector<std::complex<double> > mgenZ(mgenF.size());


 std::vector<double> mgenFw=mgenF;
///============================================================
  //auto tree= GetWg<Fl_WgtTree>("tree_eis");
  auto trdt= ExtractTreeData();// tree->GetTreeData();
  for(auto & sf:mgenFw){
     sf*=2.0*M_PI;
  }


  if(EISCalc.InitData(trdt.x_init.size(),mgenFw)){
    fl_alert("Error: Insuffient frequency table or empty model!");
    return;

  };
  EISCalc.BindLimits(trdt.x_min, trdt.x_max);
  EISCalc.BindCfts(trdt.x_init);
  EISCalc.Calc(mgenZ);

 std::string st1= GetWg<Fl_Multiline_Input>("ted_ExtMod")->value();
  MetaData.push_back({"Model",st1});
  GetWg<clsMetaDataTable>("tbl_Meta_Ed")->update();
  auto & zstor=(mout==0)?ftstor[0]:eistor[0];
  zstor.clear();
  for(size_t i=0;i<mgenZ.size();++i)
    zstor.push_back({mgenF[i],mgenZ[i]});

}
///###################################################################


void clsAppContext::SaveGenEIS(std::vector<clsZrec> & ztable){
 //--------------------------------

 using namespace libconfig;
 Fl_Native_File_Chooser native;
  native.title("Select file to save");
  native.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
  native.filter("EIS data file\t*.eisdf\n");
  const char * buf=fl_check_access_nfc(native,".eisdf","EIS data file");
  if(buf==nullptr) return;
  Config cfg;
 /*  cfg.setOptions( Config::OptionSemicolonSeparators
                 | Config::OptionColonAssignmentForGroups
                 | Config::OptionOpenBraceOnSeparateLine);*/
  cfg.setFloatPrecision(12);
  cfg.setOption(Config::OptionAllowScientificNotation, true);
  Setting &root = cfg.getRoot();
  root.add("FID", Setting::TypeString)= FIDsls;
   double Area=atof(GetWg<Fl_Float_Input>("inpf_Area")->value());
  root.add("Area", Setting::TypeFloat)= Area;
  root.add("Descr", Setting::TypeString)= "F,Z.Re,-Z.Im";
  Setting &mlist =root.add("MetaData", Setting::TypeList);
  for(auto const & mtd:MetaData){
   Setting &rc1=mlist.add(Setting::TypeArray);
   rc1.add(Setting::TypeString)=mtd.first;
   rc1.add(Setting::TypeString)=mtd.second;
  }
  mlist.setLineCapacity(1);


 Setting &dlist =root.add("Data", Setting::TypeList);

 for(const auto &dt:ztable){
   Setting &rc1=dlist.add(Setting::TypeArray);
   rc1.add(Setting::TypeFloat) = dt.f;
   rc1.add(Setting::TypeFloat) = dt.Z.real()/Area;
   rc1.add(Setting::TypeFloat) = -dt.Z.imag()/Area;
  }
  dlist.setLineCapacity(1);
 std::string rts="OK";
 FILE * wf=fl_fopen(buf,"w");
   try
  {
   cfg.write(wf);
   fclose(wf);
  }
  catch(const FileIOException &fioex)
  {
    fclose(wf);
    rts="I/O error while writing file: %s";

  }
  if(rts!="OK"){
   fl_message(rts.c_str(),buf);
  }

  return;

};


