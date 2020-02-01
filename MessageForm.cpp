//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MessageForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#pragma resource "*.dfm"
TFormMessage *FormMessage;
//---------------------------------------------------------------------------
__fastcall TFormMessage::TFormMessage(TComponent* Owner)
        : TForm(Owner)
{
}

void __fastcall TFormMessage::ToolButton1Click(TObject *Sender)
{
    Memo1->Lines->Clear();        
}
//---------------------------------------------------------------------------

