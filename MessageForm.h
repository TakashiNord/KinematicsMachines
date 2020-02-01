//---------------------------------------------------------------------------

#ifndef MessageFormH
#define MessageFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>

//---------------------------------------------------------------------------
class TFormMessage : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo1;
        TToolBar *ToolBar1;
        TToolButton *ToolButton1;
        TImageList *ImageList1;
        void __fastcall ToolButton1Click(TObject *Sender);

private:	// User declarations
public:		// User declarations
        __fastcall TFormMessage(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMessage *FormMessage;
//---------------------------------------------------------------------------
#endif
