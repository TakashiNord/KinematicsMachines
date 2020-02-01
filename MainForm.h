//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <ImgList.hpp>
#include <Buttons.hpp>
#include <Mask.hpp>
#include <jpeg.hpp>
#include <Graphics.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
        TTabControl *TabControlMachine;
        TMainMenu *MainMenuProg;
        TMenuItem *N1;
        TMenuItem *N2;
        TImage *ImageMachine;
        TStatusBar *StatusBarMes;
        TImageList *ImageListMachine;
        TTabControl *TabControlMethods;
        TLabel *La1;
        TLabel *La2;
        TLabel *La3;
        TEdit *E2;
        TEdit *E3;
        TBitBtn *BitBtnRun;
        TLabel *La4;
        TLabel *La5;
        TLabel *La6;
        TEdit *E4;
        TEdit *E5;
        TEdit *E6;
        TEdit *Estr;
        TCheckBox *CheckBoxStr;
        TLabel *Label2;
        TEdit *E1;
        TImageList *ImageListButton;
        TEdit *E11;
        TEdit *E22;
        TEdit *E33;
        TLabel *Label1;
        TBitBtn *BitBtnStr;
        TCheckBox *CheckBoxMesForm;
        TBitBtn *BitBtnFile;
        TOpenDialog *OpenDialogFile;
        TStaticText *StaticText1;
        TBitBtn *BitBtnSQRT;
        TButton *Button1;
        TButton *Button2;
        TButton *Button3;
        TButton *Button4;
        TButton *Button5;
        TButton *Button6;
        TMenuItem *N3;
        TMenuItem *N4;
        TMenuItem *N5;
        TMenuItem *N6;
        TEdit *Eincr;
        TLabel *Label3;
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall N1Click(TObject *Sender);
        void __fastcall N2Click(TObject *Sender);
        void __fastcall TabControlMachineChange(TObject *Sender);
        void __fastcall TabControlMethodsChange(TObject *Sender);
        void __fastcall BitBtnRunClick(TObject *Sender);
        void __fastcall CheckBoxStrClick(TObject *Sender);
        void __fastcall EstrChange(TObject *Sender);
        void __fastcall BitBtnStrClick(TObject *Sender);
        void __fastcall CheckBoxMesFormClick(TObject *Sender);
        void __fastcall BitBtnFileClick(TObject *Sender);
        void __fastcall BitBtnSQRTClick(TObject *Sender);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall N5Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormMain(TComponent* Owner);
        AnsiString caption;
        TStrings *ListMachine ;
        TStrings *ListDecriptions ;
        int __fastcall  DecideMachine(int Machine, int Methods);
        void __fastcall OutputMessage(AnsiString s);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
