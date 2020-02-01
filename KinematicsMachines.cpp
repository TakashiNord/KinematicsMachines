//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("KinematicsMachines.res");
USEFORM("MainForm.cpp", FormMain);
USEFORM("about.cpp", FormAbout);
USEFORM("MessageForm.cpp", FormMessage);
USEOBJ("KinematicsMachines_utils.obj");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->CreateForm(__classid(TFormMain), &FormMain);
                 Application->CreateForm(__classid(TFormMessage), &FormMessage);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
