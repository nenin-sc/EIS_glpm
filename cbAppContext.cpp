#include <FL/Fl_Native_File_Chooser.H>
#include <FL/fl_ask.H>
#include <FL/filename.H>
#include <Fl/user/fl_small_stuff.h>
#include <fmt/format.h>
#include "clsAppContext.h"
#include "formswc/clsFormSwcContext.h"


void clsAppContext::cbClose(){
  if (Fl::event()==FL_SHORTCUT && Fl::event_key()==FL_Escape)
    return; //
 GetWin("FormFName",ERR_PLACE)->hide();
  Win()->hide();
};



//-----------------------------
void clsAppContext::cbStoreMOD(){
  modstor[modslot]=GetWg<Fl_Multiline_Input>("ted_ExtMod")->value();
  modslotlabels[modslot]=fmt::format("{}, {} ch.",modslot,modstor.at(modslot).size());
  rbtn_modslot->label(modslotlabels.at(modslot).c_str());
  rbtn_modslot->redraw_label();
  rbtn_modslot->parent()->redraw();
};
//-------------------------
void clsAppContext::cbReStoreMOD(const int val){
  size_t i=static_cast<size_t>(val);
  if(i>modstor.size()) i=0; // just in case;
  GetWg<Fl_Multiline_Input>("ted_ExtMod")->value(modstor.at(i).c_str());

};
//-----------------------------------------

void clsAppContext::cbSvcTable(const char * lbl,const int mode){
 auto ch=GetWg<Fl_Choice>(lbl);
 if(mode){
   int idx=ch->find_index("Original");
   ch->mode(idx,FL_MENU_INACTIVE );
   idx=ch->find_index("Temporary");
   ch->mode(idx,FL_MENU_INACTIVE);
   idx=ch->find_index("Slot 1");
   ch->value(idx);
 } else {
   int idx=ch->find_index("Original");
   ch->mode(idx,0 );
   idx=ch->find_index("Temporary");
   ch->mode(idx,0 );

 }

};
//-----------------------------------------

void clsAppContext::cbStoreFTABLE(){
  ftstor[ftslot]=ftstor[0];
  ftslotlabels[ftslot]=fmt::format("{}, {} pts.",ftslot,ftstor.at(ftslot).size());
  rbtn_ftslot->label(ftslotlabels.at(ftslot).c_str());
  rbtn_ftslot->redraw_label();
  rbtn_ftslot->parent()->redraw();
};
//--------------------------------------------------
void clsAppContext::cbPlotFT(const int val){
  size_t i=static_cast<size_t>(val);
  if(i>ftstor.size()) i=0; // just in case;
  tmpzstror4plot=&ftstor.at(i);
   if(tmpzstror4plot->size()==0)
 {
   fl_alert("Error:Data for plot is empty");
   return;
 }
 Win()->deactivate();
 Fl_Double_Window* tdw=GetWin("Form1");
 clsGenContext * new_cntxt=(clsGenContext *) tdw->user_data();
 new_cntxt->ParentWin(Win());
 new_cntxt->Win()->show();
};
//------------------------------------
void clsAppContext::cbSaveFT(){
   int ftype= GetWg<Fl_Choice>("ch.ft.save.ft")->value();
   size_t i= GetWg<Fl_Choice>("ch.ft.save.src")->value();
   if(i>ftstor.size()) i=0; // just in case;

   if(ftstor.at(i).size()==0)
    {
     fl_alert("Error:Data for export/save is empty");
     return;
    }
   switch(ftype){

  case 0:SaveGenEIS(ftstor[i]); return;
  case 1:ExportXLSX(ftstor[i]); return;
  case 2:ExportTXT(ftstor[i]); return;
  default:fl_alert("Error:Unexpected file type");
   }
 };
///---------------------------------------------------------
void clsAppContext::cbSaveEISDF(){
   int ftype= GetWg<Fl_Choice>("ch.eisdf.save.ft")->value();
   size_t  i= GetWg<Fl_Choice>("ch.eisdf.save.src")->value();
   if(i>eistor.size()) i=0; // just in case;

   if(eistor.at(i).size()==0)
    {
     fl_alert("Error:Data for export/save is empty");
     return;
    }
   switch(ftype){

  case 0:SaveGenEIS(eistor[i]); return;
  case 1:ExportXLSX(eistor[i]); return;
  case 2:ExportTXT(eistor[i]); return;
  default:fl_alert("Error:Unexpected file type");
   }
 };
//-----------------------------------
void clsAppContext::cbPrepareEISDFTable(){
  if(eistor.at(eislotf).size()<1){
   fl_message("Frequency table is empty.");
   return;

  }
  mgenF.clear();
  for(const auto & ztb:eistor[eislotf])
    mgenF.push_back(ztb.f);
};
//--------------------------------------------

void clsAppContext::cbStoreEISDF(){
  eistor[eislot]=eistor[0];
  eislotlabels[eislot]=fmt::format("{}, {} pts.",eislot,eistor.at(eislot).size());
  rbtn_eislot->label(eislotlabels.at(eislot).c_str());
  rbtn_eislot->redraw_label();
  rbtn_eislot->parent()->redraw();
};

//--------------------------------------------

void clsAppContext::cbStoreOrigEISDF(){
  if(ftstor.at(ftslot).size()<10){
   fl_message("Size of the data %llu in slot %llu is too small.",ftstor.at(ftslot).size(),ftslot);
   return;
  }
  if(eistor.back().size()>0
    &&fl_choice("Oriog is not empty, do you want ovewrite it?", "No", "Yes", 0)) {
  if(fl_choice("Clear existing meta data?", "No", "Yes", 0)){
    MetaData.clear();
  }
   MetaData.push_back({"Orig generated",GetWg<Fl_Multiline_Input>("ted_ExtMod")->value()});
   GetWg<clsMetaDataTable>("tbl_Meta_Ed")->update();
  };

  eistor.back()=ftstor.at(ftslot);

};



//------------------------------------
void clsAppContext::cbPlotEISDF(const int val){
  size_t i=static_cast<size_t>(val);
  if(i>eistor.size()) i=0; // just in case;
  tmpzstror4plot=&eistor.at(i);
   if(tmpzstror4plot->size()==0)
 {
   fl_alert("Error:Data for plot is empty");
   return;
 }
 Win()->deactivate();
 Fl_Double_Window* tdw=GetWin("Form1");
 clsGenContext * new_cntxt=(clsGenContext *) tdw->user_data();
 new_cntxt->ParentWin(Win());
 new_cntxt->Win()->show();
};
//-----------------------
void clsAppContext::cbSetDRTTools(){
  GetWg<Fl_Check_Button>("cb_tab_csv")->value(1);
  GetWg<Fl_Check_Button>("cb_col_csv")->value(0);
  GetWg<Fl_Check_Button>("cb_nzim_csv")->value(0);
  GetWg<Fl_Choice>("ch_loc_csv")->value(0);
  GetWg<Fl_Choice>("ch_fsort_csv")->value(0);

 };
//----  --------------------------------------------------------------------------------------------------------
void clsAppContext::cbLoadMod(){
 if(cbClearAll()) return;
  Fl_Native_File_Chooser native;
  std::string fname;
  native.title("Select IES Model File to load");
  native.type(Fl_Native_File_Chooser::BROWSE_FILE);
  native.filter("IES Model File\t*.eism\n");
  switch ( native.show() ) {
    case -1:  fl_message("IES Model File open error\n, %s.", native.errmsg());  break;	// ERROR
    case  1:  fl_message("IES Model File load canceled\n ."); break;		// CANCEL
    default: 								// PICKED FILE
      if ( native.filename() ) {

        auto rts=LoadModel(native.filename());
        if(rts!="") {
         fl_alert(rts.c_str());

        } else
            txout_modfname->value(native.filename());
           }

      break;
  }
};
//----  --------------------------------------------------------------------------------------------------------

void clsAppContext::cbSaveMod(){

  Fl_Native_File_Chooser native;
  native.title("Select IES Model File to save");
  native.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
  native.filter("IES Model File\t*.eism\n");
 // native.directory(GetDataPath().c_str());
  native.preset_file("draft_mod.eism");
  const char * buf=fl_check_access_nfc(native,".eism","IES Model file");
   if(buf==nullptr) return;
     auto tr=GetWg<Fl_WgtTree>("tree_eis");
       // tr->mod=GetWg<Fl_Multiline_Input>("ted_ExtMod")->value();
        auto rts=tr->SaveDraftMod(buf);
        if(rts!="")
         fl_alert(rts.c_str());
   };
//------------------------------------------
void clsAppContext::cbDataOp(const int op){
 switch(op){
   case 0:_cbSubstractModelOutput(true); break;
   case 1:_cbSubstractModelOutput(false); break;
   case 2:_cbUnParModelOutput(); break;
   case 3:_cbParModelOutput(); break;
   case 4:_cbAppendModelOutput(); break;
   case 5:_cbScaleModelOutput(); break;
 default:;
  }

  };

//---------------------------------------
void clsAppContext::cbShowFnames(){
auto tw=GetWin("FormFName",ERR_PLACE);
if(tw->visible())
    {tw->hide();
    return; }
 int rhWtw=tw->decorated_h();
 int ryWW=Win()->y()-Win()->decorated_h()+Win()->h();
 int ny=ryWW-rhWtw;
 if(ny<0) ny=Win()->y();
 tw->resize(Win()->x(),ny,tw->w(),tw->h());
tw->show();
 }

//---------------------------------------

bool clsAppContext::cbClearAll(){

 Win()->deactivate();
 auto tdw=GetWin("FormSwc");
 auto new_cntxt=reinterpret_cast<clsFormSwcContext *>(tdw->user_data());
 new_cntxt->ParentWin(Win());
 new_cntxt->Win()->set_modal();
 new_cntxt->Win()->show();
 while (new_cntxt->Win()->shown()) Fl::wait();
 if(new_cntxt->isCanceled()){
     fl_message("Operation canceled.");
     return true;}
 if(new_cntxt->GetWg<Fl_Check_Button>("cb_DFd")->value()){
   eistor.at(eislotf).clear();
   txout_datfname->value("");
   GetWg<Fl_Output>("txout.eisdf.ppd")->value("empty");
   }
 if(new_cntxt->GetWg<Fl_Check_Button>("cb_DFmd")->value()){
   MetaData.clear();
   GetWg<clsMetaDataTable>("tbl_Meta_Ed")->update();
   }
 if(new_cntxt->GetWg<Fl_Check_Button>("cb_DFrg")->value()){
   for(size_t i=0;i<eistor.size();++i){
    if(i==eislotf) continue;
    eistor.at(i).clear();
    if(i==0) continue;
    eislotlabels[i]=fmt::format("{}, empty.",i);
   }
   }
 if(new_cntxt->GetWg<Fl_Check_Button>("cb_FTrg")->value()){
   for(size_t i=0;i<ftstor.size();++i){
    ftstor.at(i).clear();
    if(i==0) continue;
    ftslotlabels[i]=fmt::format("{}, empty.",i);
   }
   }
 if(new_cntxt->GetWg<Fl_Check_Button>("cb_Mod")->value()){
   txout_modfname->value("");
   auto tree=GetWg<Fl_WgtTree>("tree_eis");
   tree->ClearAll();
   tree->redraw();

   GetWg<Fl_Multiline_Input>("ted_ExtMod")->value("empty");
   for(size_t i=0;i<modstor.size();++i){
     modstor.at(i).clear();
     if(i==0) continue;
     modslotlabels[i]=fmt::format("{}, empty.",i);
   }
  }
  Win()->redraw();
  return false;
};
