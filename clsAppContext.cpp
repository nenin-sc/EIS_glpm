#include "clsAppContext.h"

#include <fmt/format.h>
#include <fmt/color.h>
extern Fl_Double_Window* winit_form1();
extern Fl_Double_Window* winit_formswc();
extern Fl_Double_Window* winit_viewfname();
clsAppContext::clsAppContext()
{
    //ctor

  tmpzstror4plot=nullptr;
}

clsAppContext::~clsAppContext()
{
    //dtor

}

void clsAppContext::init_windows(){
  RegisterWin("Main",win);
  RegisterWin("Form1",winit_form1());
  RegisterWin("FormSwc",winit_formswc());
  auto wfn=winit_viewfname();

  RegisterWin("FormFName",wfn);
  InitWidgets();

};
void clsAppContext::InitWidgets(){



 {
   auto ch=GetWg<Fl_Choice>("ch_minFrqK");
   ch->add("Hz");
   ch->add("kHz");
   ch->add("MHz");
   ch->value(0);
 }
 {
   auto ch=GetWg<Fl_Choice>("ch_maxFrqK");
   ch->add("Hz");
   ch->add("kHz");
   ch->add("MHz");
   ch->value(1);
 }

  {
   auto ch=GetWg<Fl_Choice>("ch.eisdf.minf");
   ch->add("Hz");
   ch->add("kHz");
   ch->add("MHz");
   ch->value(0);
 }
 {
   auto ch=GetWg<Fl_Choice>("ch.eisdf.maxf");
   ch->add("Hz");
   ch->add("kHz");
   ch->add("MHz");
   ch->value(1);
 }

 {
   auto ch=GetWg<Fl_Choice>("ch.mod.restore");
   ch->add("Original");
   ch->add("Slot 1");
   ch->add("Slot 2");
   ch->add("Slot 3");
   ch->value(0);
 }

 {
   rbtn_modslot=GetWg<Fl_Round_Button>("rb.mod.slot1");
   rbtn_modslot->value(1);
   modslot=1;
 }

 {
   auto ch=GetWg<Fl_Choice>("ch.ft.plot");
   ch->add("Temporary");
   ch->add("Slot 1");
   ch->add("Slot 2");
   ch->add("Slot 3");
   ch->value(0);
 }


 {
   rbtn_ftslot=GetWg<Fl_Round_Button>("rb.ftable.slot1");
   rbtn_ftslot->value(1);
   ftslot=1;
 }
{
   auto ch=GetWg<Fl_Choice>("ch.ft.save.ft");
   ch->add("EISDF file");
   ch->add("XLSX");
   ch->add("Text/CSV");
   ch->value(0);
 }

  {
   auto ch=GetWg<Fl_Choice>("ch.ft.save.src");
   ch->add("Temporary");
   ch->add("Slot 1");
   ch->add("Slot 2");
   ch->add("Slot 3");
   ch->value(0);
 }


{
   auto ch=GetWg<Fl_Choice>("ch.eisdf.plot");
   ch->add("Temporary");
   ch->add("Slot 1");
   ch->add("Slot 2");
   ch->add("Slot 3");
   ch->add("Original");
   eislotf=ch->find_index("Original");
   ch->value(0);
 }


 {
   auto ch=GetWg<Fl_Choice>("ch.eisdf.save.ft");
   ch->add("EISDF file");
   ch->add("XLSX");
   ch->add("Text(CSV)");
   ch->value(0);
 }

  {
   auto ch=GetWg<Fl_Choice>("ch.eisdf.save.src");
   ch->add("Temporary");
   ch->add("Slot 1");
   ch->add("Slot 2");
   ch->add("Slot 3");
   ch->add("Original");
   ch->value(0);
 }


 {
   rbtn_eislot=GetWg<Fl_Round_Button>("rb.eisdf.slot1");
   rbtn_eislot->value(1);
   eislot=1;
 }
{
   auto ch=GetWg<Fl_Choice>("ch.eisdf.src");
   ch->add("Temporary");
   ch->add("Slot 1");
   ch->add("Slot 2");
   ch->add("Slot 3");
   ch->add("Original");
   ch->value(0);

}
 {
   auto ch=GetWg<Fl_Choice>("ch.eisdf.src.ftbl");
   ch->add("Temporary");
   ch->add("Slot 1");
   ch->add("Slot 2");
   ch->add("Slot 3");
   ch->add("Original");
   int idx=ch->find_index("Original");
   ch->mode(idx,FL_MENU_INACTIVE );
   idx=ch->find_index("Temporary");
   ch->mode(idx,FL_MENU_INACTIVE );
   ch->value(1);
 }
 {
   auto ch=GetWg<Fl_Choice>("ch.eisdf.dataop");
   ch->add("Subtract");
   ch->add("Add");
   ch->add("UnParallel");
   ch->add("Parallel");
   ch->add("Append");
   ch->add("Scale");
   ch->add("Cancel");
   ch->value(0);
 }



  {
  auto ch=GetWg<Fl_Choice>("ch_loc_csv");

  ch->add("US",0,nullptr,(void*)0);
  ch->add("EU",0,nullptr,(void*)0);
  ch->value(0);
 }

{
  auto ch=GetWg<Fl_Choice>("ch_fsort_csv");

  ch->add("Decrease",0,nullptr,(void*)0);
  ch->add("Increase",0,nullptr,(void*)0);
  ch->value(0);
 }

 {
  auto ch=GetWg<Fl_Choice>("ch_loc_imptxt");

  ch->add("US",0,nullptr,(void*)0);
  ch->add("EU",0,nullptr,(void*)0);
  ch->value(0);
 }




   GetWg<Fl_Int_Input>("inpi_ppd")->value("10");
   GetWg<Fl_Float_Input>("inpf_minFrq")->value("0.001");
   GetWg<Fl_Float_Input>("inpf_maxFrq")->value("100");
   GetWg<Fl_Float_Input>("inpf_Area")->value("16.0");
   auto tree= GetWg<Fl_WgtTree>("tree_eis");
   tree->mod_ed=GetWg<Fl_Multiline_Input>("ted_ExtMod");
    tree->SetOwnerID("Main");
   tree->InitWidgets(win);

   for(const auto &el:ModLib.Lib){
      clsTreeProxyEl t1;
      std::string st0=el->GetID();
      size_t idxs=st0.find(":");
      t1.Prefix = st0.substr(0,idxs);
      t1.Name = st0.substr(idxs+1);
      t1.Suffix = "?";
      size_t ncft=el->GetNCft();
       for(size_t i=0;i<ncft;++i){
         clsTreeProxyRecord rec;
         rec.name=el->GetCftName(i);
         rec.val =el->defCft(i);
         rec.vmax =el->defCftMax(i);
         rec.vmin =el->defCftMin(i);
         rec.locked=false;
         t1.data.push_back(rec);
    }
    tree->RegElement(t1);
   }
   tree->BuildElMenu();

//++++
extern void cbmnu_cpy_from_data_minmax(Fl_Widget *p, void* userdata);

pockets[0]={GetWg<Fl_Float_Input>("inpf.eisdf.minf"),GetWg<Fl_Choice>("ch.eisdf.minf")};
pockets[1]={GetWg<Fl_Float_Input>("inpf.eisdf.maxf"),GetWg<Fl_Choice>("ch.eisdf.maxf")};

static Fl_Menu_Item mnu_cpy[] = {
  {"Copy from Data Min",	0,cbmnu_cpy_from_data_minmax, pockets},
  {"Copy from Data Max",	0,cbmnu_cpy_from_data_minmax, pockets+1},
  {0}};

  {
   auto inp=GetWg<Fl_InputWithMenu>("inpf_minFrq");
    inp->pulldown=mnu_cpy;
    inp->scale=GetWg<Fl_Choice>("ch_minFrqK");
  }
  {
   auto inp=GetWg<Fl_InputWithMenu>("inpf_maxFrq");
    inp->pulldown=mnu_cpy;
     inp->scale=GetWg<Fl_Choice>("ch_maxFrqK");
  }
{
 auto tw=GetWin("FormFName",ERR_PLACE);
 auto cntx=reinterpret_cast<clsGenContext*>(tw->user_data());
 txout_modfname=cntx->GetWg<Fl_Output>("txout_modfname",ERR_PLACE);
 txout_datfname=cntx->GetWg<Fl_Output>("txout_datfname",ERR_PLACE);
 cbShowFnames();
}


};
//---------------------

extern void cbmnu_cpy_from_data_minmax(Fl_Widget *p, void* userdata){
  auto inp=reinterpret_cast<Fl_InputWithMenu*>(p);
  auto src=reinterpret_cast<clsAppContext::strPocket*>(userdata);
  inp->value(src->v->value());
  inp->scale->value(src->sc->value());

};
extern void cbmnu_cpy_from_data_min(Fl_Widget *p, void* userdata);

 Fl_InputWithMenu::~Fl_InputWithMenu(){};

   int Fl_InputWithMenu::handle(int e){
    if(pulldown==nullptr)  return(Fl_Float_Input::handle(e));  // no proper menu
    switch (e) {
            case FL_PUSH:
                // RIGHT MOUSE PUSHED? Popup menu on right click
                if ( Fl::event_button() == FL_RIGHT_MOUSE ) {
                     const Fl_Menu_Item *m = pulldown->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
                    if ( m ) {
                             //m->do_callback(this, (void *)m); };
                              m->do_callback(this, m->user_data()); };
                    return(1);          // (tells caller we handled this event)
                }
                break;
            case FL_RELEASE:
                // RIGHT MOUSE RELEASED? Mask it from Fl_Input
                if ( Fl::event_button() == FL_RIGHT_MOUSE ) {
                    return(1);          // (tells caller we handled this event)
                }
                break;
        }
        return(Fl_Float_Input::handle(e));    // let Fl_Input handle all other events


   };

//---------------------
void clsAppContext::FillResults(std::vector<double> & fFrq,std::vector<std::complex<double> > & vZ){
 fFrq.clear();
 vZ.clear();
 for(const auto &zr:*tmpzstror4plot){
  fFrq.push_back(zr.f);
  vZ.push_back(zr.Z);
 }

} ;

int clsAppContext::CheckPostBox(const int msg){
 // if(msg==1)
    {
    //cbSetDefaultBest();
    return PBX_OK;
  }
  return PBX_NOT_MINE;
};
