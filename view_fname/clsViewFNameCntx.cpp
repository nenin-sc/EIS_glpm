#include <Fl/Fl_Menu_Item.h>
#include "clsViewFNameCntx.h"

clsViewFNameCntx::clsViewFNameCntx()
{
    //ctor

}

clsViewFNameCntx::~clsViewFNameCntx()
{
    //dtor
}

void clsViewFNameCntx::cbClose(){


};

//--------------------------------------------
static void cbmnu_cpy_fname(Fl_Widget *p, void* userdata){
  auto inp=reinterpret_cast<Fl_OutputWithMenu*>(p);
  const auto l=inp->size();
  inp->insert_position(l);
  inp->redraw();

};

static void cbmnu_cpy_fdir(Fl_Widget *p, void* userdata){
  auto inp=reinterpret_cast<Fl_OutputWithMenu*>(p);
  inp->insert_position(0);
  inp->redraw();

};

static void cbmnu_cpy_ffull(Fl_Widget *p, void* userdata){
  auto inp=reinterpret_cast<Fl_OutputWithMenu*>(p);
  const auto l=inp->size();
  inp->insert_position(l/2);
  inp->redraw();

};
//---------------------------------------
static Fl_Menu_Item mnu_fname[] = {
  {"File Name",	0,cbmnu_cpy_fname},
  {"Dir Name",	0,cbmnu_cpy_fdir},
  {"Middle",	0,cbmnu_cpy_ffull},
  {0}};

 Fl_OutputWithMenu::~Fl_OutputWithMenu(){};

   int Fl_OutputWithMenu::handle(int e){
    if(pulldown==nullptr)  return(Fl_OutputWithMenu::handle(e));  // no proper menu
    switch (e) {
            case FL_PUSH:
                // RIGHT MOUSE PUSHED? Popup menu on right click
                if ( Fl::event_button() == FL_RIGHT_MOUSE ) {
                     const Fl_Menu_Item *m = pulldown->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
                    if ( m ) {
                              m->do_callback(this, m->user_data());
                               };
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
        return(Fl_Output::handle(e));    // let Fl_Input handle all other events

   };

void clsViewFNameCntx::InitWidgets(){
  {
   auto inp=GetWg<Fl_OutputWithMenu>("txout_datfname");
    inp->pulldown=mnu_fname;

  }
 {
   auto inp=GetWg<Fl_OutputWithMenu>("txout_modfname");
    inp->pulldown=mnu_fname;

  }
};
