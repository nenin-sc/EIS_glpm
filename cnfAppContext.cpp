 #include <FL/Fl_Native_File_Chooser.H>
 #include <FL/fl_ask.H>
#include <config/libconfig.h++>
#include <fmt/format.h>
#include "clsAppContext.h"

using namespace libconfig;

void clsAppContext::cbLoadData(){
   if(cbClearAll()) return;
  //----  ------------------------------------------------------------------------------------------------------
  Fl_Native_File_Chooser native;
  native.title("Select EIS data file to load");
  native.type(Fl_Native_File_Chooser::BROWSE_FILE);
  native.filter("EIS data file\t*.eisdf\n");
   switch ( native.show() ) {
    case -1:  fl_message("EIS data file open error\n, %s.", native.errmsg());  return;	// ERROR
    case  1:  fl_message("EIS data file load canceled\n ."); return;		// CANCEL
    default: 								// PICKED FILE
      if ( !native.filename() ) {
         fl_message("No EIS data file selected.");
          return;
      };
   };
  Config cfg;
  std::string rts="OK";
   FILE * wf=nullptr;
   try
  {
   wf= fl_fopen(native.filename(),"r");
   cfg.read(wf);
  }
  catch(const FileIOException &fioex)
  {

    rts= fmt::format("I/O error while reading file: {}", native.filename());


  }
    catch(const ParseException &pex)
  {

    rts=fmt::format("Parse error:\n File:{}\nLine:{}\nCode:{}", native.filename(), pex.getLine(), pex.getError());
  }
    fclose(wf);
  if(rts!="OK"){
   fl_message("EIS data file read error:\n%s .", rts.c_str());
   return;
  }

  std::string fid;
  try
  {
    std::string fid0=cfg.lookup("FID");
    fid=fid0;

  }
  catch(const SettingNotFoundException &nfex)
  {
    fl_alert("Error: No FID found.");
    return;
  }
   const Setting& root = cfg.getRoot();


  if(fid!=FIDsls){
    rts=fmt::format("FID {}  is invalid.", fid);
    fl_message("EIS data file parse error:\n%s .", rts.c_str());
    return;
    };
  std::vector<std::pair<std::string, std::string>>  metaData;
   const Setting &mlist =root["MetaData"];
  size_t sz=mlist.getLength();
   for(size_t i=0;i<sz;++i){
       const Setting &rc1=mlist[i];
       std::string ts=rc1[0];
       std::string ts1=rc1[1];
       metaData.push_back({ts,ts1});
       }
  std::vector<double> tEis;
  std::vector<std::complex<double> >tZ;
  double Area=root.lookup("Area");
  GetWg<Fl_Float_Input>("inpf_Area")->value(fmt::format("{:g}",Area).c_str());

 const Setting &dlist =root["Data"];
 size_t sz1=dlist.getLength();
 auto & ztbl=eistor[eislotf];
 ztbl.clear();
 for(size_t i=0;i<sz1;++i){
    const Setting &rc1=dlist[i];
    double fr=rc1[0];
    double re=rc1[1];
    double im=rc1[2];
    ztbl.push_back({fr,{re*Area,-im*Area}});
  }
 if(ztbl.size()<2){
    fl_message("EIS data size %llu too small.", tEis.size());

 }
 MetaData=metaData;
 GetWg<clsMetaDataTable>("tbl_Meta_Ed")->update();
 auto mima=std::minmax_element(ztbl.begin(),ztbl.end(),sort_Zba);
 double mif=mima.first->f;
 double maf=mima.second->f;

 GetWg<Fl_Float_Input>("inpf.eisdf.minf")->value(fmt::format("{}",mif).c_str());
 GetWg<Fl_Choice>("ch.eisdf.minf")->value(0);
 GetWg<Fl_Float_Input>("inpf.eisdf.maxf")->value(fmt::format("{}",maf/1000.).c_str());
 GetWg<Fl_Choice>("ch.eisdf.maxf")->value(1);
 int pdec=ztbl.size()/log10(maf/mif);
 GetWg<Fl_Output>("txout.eisdf.ppd")->value(fmt::format("{}/{}",ztbl.size(),pdec).c_str());
 txout_datfname->value(native.filename());
  return;
};
//--------------------------------------------------
std::string clsAppContext::LoadModel(const char *fname){
  Config cfg;
  std::string rts="";
   FILE * wf=nullptr;
   try
  {

   wf= fl_fopen(fname,"r");
   cfg.read(wf);
   fclose(wf);
  }
  catch(const FileIOException &fioex)
  {
    fclose(wf);

    return std::string("I/O error while reading file ")+fname;
  }
  catch(const ParseException &pex)
  {
     fclose(wf);
     fmt::print("Parse error {} at {}:{} -  \n",pex.getError(), pex.getFile(),pex.getLine());

    return std::string ("Parse error in ") + std::string (pex.getFile());
  }
  const Setting& root = cfg.getRoot();
  std::string fid=root.lookup("FID");



  if(fid!="EIS Config v.9.8"&&fid!="EIS Config v.10.0"){

    return std::string ("FID ") + fid +  " is invalid.";
    }

   auto tree=GetWg<Fl_WgtTree>("tree_eis");
   tree->ClearAll();

   auto modimp=GetWg<Fl_Multiline_Input>("ted_ExtMod");


   std::string  _mod=root.lookup("Model");
   modimp->value(_mod.c_str());
   modstor[0]=_mod;

   const Setting &mlist =root["Elements"];
   size_t sz=mlist.getLength();
   bool swc_err1=false;

    for(size_t i=0;i<sz;++i){
      clsTreeProxyEl el;

       const Setting &rc1=mlist[i];
       size_t el_sz=rc1.getLength();
       std::string name_el=rc1[0];
       el.Suffix= (std::string) rc1[1];
       size_t j0=2;

   clsGenericFitMod_RPN * EL;
     try{
        EL=ModLib.Get(name_el.c_str());
     }
      catch (const std::out_of_range& oor) {

       swc_err1=true;
       rts=std::string("Element Type") + name_el + std::string("is invalid.");
      };
      if(swc_err1) break;
      size_t idxs=name_el.find(":");
      if(idxs==std::string::npos||idxs<2){
       rts=std::string("Element ") + name_el + std::string(" name is invalid.");
       break;
      }
      el.Prefix=name_el.substr(0,idxs);
      el.Name=name_el.substr(idxs+1);
       auto cntc=EL->GetNCft();
       for(size_t j=j0,n=0;j<el_sz;++j,++n){
         if(n==cntc){
           fl_alert("Warning: excessive element in %s.", name_el.c_str());
           break;
         }
         const Setting &vals=rc1[j];
         clsTreeProxyRecord rec;
         rec.name=EL->GetCftName(n);
         rec.val=vals[0];
         rec.vmin=vals[1];
         rec.vmax=vals[2];
         if(vals.getLength()>3) {
          rec.locked=vals[3];
          } else rec.locked=false;
        el.data.push_back(rec);
       }
     tree->AddEl(el);
    }; //for(size_t i=0;i<sz;++i)


  if(swc_err1)
    tree->ClearAll();
  tree->select(tree->root());
  tree->redraw();
  return rts;
  };




