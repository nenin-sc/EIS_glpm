#ifndef CLSAPPCONTEX_H
#define CLSAPPCONTEX_H
#include <array>
#include <Fl/Fl_Int_Input.H>
#include <Fl/Fl_Float_Input.H>
#include <Fl/user/clsGenContext.h>
#include "../lib_shy/fitmods_rpn.h"
#include "../lib_shy/shunting-yard.h"
#include "clsMetaDataTable/clsMetaDataTable.h"
#include "../lib_tree/Fl_WgtTree.h"
#include "main.h"

class Fl_InputWithMenu:public Fl_Float_Input
{

 public:
   Fl_Choice *scale;
   Fl_Menu_Item *pulldown;
   Fl_InputWithMenu(int x, int y, int w, int h, const char *l=0):Fl_Float_Input(x, y, w, h, l){
       scale=nullptr;
       pulldown=nullptr;};
   virtual ~Fl_InputWithMenu();

   int handle(int e);
};

class clsAppContext:public clsGenContext
{
    public:
       struct strPocket{Fl_Float_Input * v; Fl_Choice * sc; } ;
        clsRPN_ModLib ModLib;
       struct clsZrec{
       double f;
       std::complex<double> Z;// area-specific!!!!
       } ;

       std::vector<std::pair<std::string, std::string>>  MetaData;
       std::vector<double> mgenF;

      void init_windows();
      clsAppContext();
      virtual ~clsAppContext();
      void InitWidgets();
      virtual int CheckPostBox(const int msg);
      bool CheckEISCalc(); // false-> OK

      void FillResults(std::vector<double> & fFrq,std::vector<std::complex<double> > & vZ) ;

      void SaveGenEIS(std::vector<clsZrec> & ztable);
      void ExportXLSX(std::vector<clsZrec> & ztable);
      void ExportTXT(std::vector<clsZrec> & ztable);

    //- cb ----------------------------
     void cbLoadMod();
     void cbSaveMod();

     void cbStoreMOD();
     void cbReStoreMOD(const int val);
     void cbModSlot(const int i, Fl_Round_Button * btn) {modslot=static_cast<size_t>(i);rbtn_modslot=btn;};
     void cbStoreFTABLE();
     void cbFTSlot(const int i, Fl_Round_Button * btn) {ftslot=static_cast<size_t>(i);rbtn_ftslot=btn;};
     void cbPlotFT(const int val);
     void cbSaveFT();

     void cbPrepareEISDFTable();
     void cbStoreEISDF();
     void cbStoreOrigEISDF();
     void cbPlotEISDF(const int val);
     void cbEISlot(const int i, Fl_Round_Button * btn) {eislot=static_cast<size_t>(i);rbtn_eislot=btn;};
     void cbSaveEISDF();
     void cbGenerateModelOutput(const size_t mout);
     void cbSvcTable(const char * lbl,const int mode);

     void cbClose();
     bool cbClearAll(); // true - Cancel


     void cb_mkNoise();
     void cbLoadFTable();
     void cbLoadData();
     void cbGenerateFTable();

     void cbDataOp(const int op);

     void cbSetDRTTools();

     void cbShowFnames();

    protected:

     strPocket pockets[2];
     const clsTreeFitData  ExtractTreeData();
     std::vector<clsTreeProxyEl> tempTreeData;
     void UpdateTree(const std::vector<double> xv);
     std::string  LoadModel(const char *fname);
     void InsElementToEdit(const std::string &s);
      clsRPCircCalc EISCalc;
      std::vector< std::shared_ptr<clsGenericFitMod_RPN> > Elems;


    void mknz_Uniform();
    void mknz_uni_Zmag();
    void mknz_uni_Zmagi();
    void mknz_uni_Zrimi();

    std::array<std::string,4> modstor,modslotlabels,ftslotlabels,eislotlabels;
    std::array<std::vector<clsZrec>,4> ftstor;
    std::array<std::vector<clsZrec>,5> eistor;
    std::vector<clsZrec> * tmpzstror4plot;
    Fl_Round_Button *rbtn_modslot,*rbtn_ftslot,*rbtn_eislot;
    size_t modslot,ftslot,eislot,eislotf;

    void _cbScaleModelOutput();
    void _cbSubstractModelOutput(const bool swcSub);
    void _cbAppendModelOutput();
    void _cbParModelOutput();
    void _cbUnParModelOutput();

    private:
      const std::string FIDsls="EIS Data v.9.5";
      Fl_Output *txout_modfname, * txout_datfname;
};

extern bool sort_Zab(const clsAppContext::clsZrec & a,const clsAppContext::clsZrec & b) ;
extern bool sort_Zba(const clsAppContext::clsZrec & a,const clsAppContext::clsZrec & b) ;

#endif // CLSAPPCONTEX_H
