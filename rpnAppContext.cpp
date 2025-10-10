#include <stdexcept>
#include <fmt/format.h>
#include <fmt/ranges.h>
#include <Fl/Fl_ask.h>
#include "clsAppContext.h"


size_t string_tokenizer(const std::string & str,const std::string & delims, std::vector<std::string> & strlist){
    size_t beg, pos = 0;
    strlist.clear();
    while ((beg = str.find_first_not_of(delims, pos)) != std::string::npos)
    {
        pos = str.find_first_of(delims, beg + 1);
        strlist.push_back(str.substr(beg, pos - beg));
    }

    return strlist.size();
}

void clsAppContext::InsElementToEdit(const std::string &s){
  auto ed=GetWg<Fl_Multiline_Input>("ted_ExtMod");
  ed->insert(s.c_str());
 };

//---------------------
 const clsTreeFitData clsAppContext::ExtractTreeData(){

  clsTreeFitData  trData;

  size_t counter=0;
  for(auto const &td:tempTreeData){
   for(auto const &tdr:td.data){
    trData.x_init.push_back(tdr.val);
    trData.x_min.push_back(tdr.vmin);
    trData.x_max.push_back(tdr.vmax);
    if(!tdr.locked){
      trData.mapv.push_back(counter);
    }
   counter++;
   };
  }
 trData.trdata=tempTreeData;
 return trData;
  };


//----  --------------------------------------------------------------------------------------------------------
bool clsAppContext::CheckEISCalc(){
  ///std::vector<clsTreeProxyEl> TreeData;
tempTreeData.clear();
  auto ed=GetWg<Fl_Multiline_Input>("ted_ExtMod");
  std::string modline(ed->value());
  if(!modline.size()) {

   fl_alert("Fatal: Empty EIS model definition.");
   return true;
  }
std::vector<std::string> varnames, lostvars;
   string_tokenizer(modline," +*()",varnames);

 if(!varnames.size()) {
      fl_alert("Fatal: Empty variables list in EIS model.");
    return true;
  }


auto trdata=GetWg<Fl_WgtTree>("tree_eis")->GetTreeData();
   if(!trdata.size()){
      fl_alert("Fatal: Empty EIS model variables values.");
    return true;
  }



for(auto const &vn:varnames){
  bool notfound=true;
  for(auto const &td0:trdata){

   std::string name=fmt::format("{}:{}{}",td0.Prefix,td0.Name,td0.Suffix);
     if(vn==name){
        tempTreeData.push_back(td0);
        notfound=false;
      }

  }
  if(notfound)
  lostvars.push_back(vn);
}
  if(!tempTreeData.size()) {

   fl_alert("Fatal: Empty EIS model Element list.");
   return true;
  }

 for(auto const &td:tempTreeData){
   for(auto const &tdr:td.data){

   if((tdr.vmin>=tdr.vmax)||
       (tdr.vmin>tdr.val)||
       (tdr.vmax<tdr.val)){
       auto s=fmt::format("Error in Circuit {}:{}{},\n record: Min({})<{}({})<Max({}) not correct!.",td.Prefix,td.Name,td.Suffix,tdr.vmin,tdr.name,tdr.val,tdr.vmax);
       fl_alert(s.c_str());
       tempTreeData.clear();
       return true;
         }

     }
 } //for(auto const &td:tempTreeData){

  if(lostvars.size()) {
   if(lostvars.size()>1) {
   fl_alert("Fatal:  Variables %s\n are missing in Model editor.",fmt::format("{}",fmt::join(lostvars,",")).c_str());
    } else
    fl_alert("Fatal:  Variable %s\n is missining in Model editor.",lostvars.front().c_str());
   return true;
  }

  std::map<std::string, clsGenericFitMod_RPN *> vars;


   Elems.clear();
   for(auto const &td0:tempTreeData){
     std::string id_EL=fmt::format("{}:{}",td0.Prefix,td0.Name);
     std::string name=fmt::format("{}:{}{}",td0.Prefix,td0.Name,td0.Suffix);
     auto EL=ModLib.Get(id_EL.c_str());
     Elems.push_back(std::shared_ptr<clsGenericFitMod_RPN>(EL->Clone()));
     Elems.back()->SetSuffix(td0.Suffix.c_str());
      vars[name]=Elems.back().get();
   }
 bool rs=false;
 try {
   EISCalc.CompileToRPN(modline.c_str(),&Elems,vars);
    }
  catch(std::domain_error &exer)  {
    fl_alert(exer.what( ));
    rs=true;
   }

return rs;
  };
