//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("frmMain.cpp", main);
USEFORM("roomlistForm.cpp", rooms);
USEFORM("createroomForm.cpp", createroom);
USEFORM("pmsForm.cpp", pms);
USEFORM("exceptForm.cpp", except);                  
USEFORM("form_modtools.cpp", modtools);
USEFORM("form_distributefunds.cpp", distributefunds);
USEFORM("form_teamchallenge.cpp", createchallenge);
USEFORM("form_challengeinfo.cpp", challengeinfo);
USEFORM("form_challengeinfo_completed.cpp", challengeinfo_completed);
USEFORM("confirmForm.cpp", request);
USEFORM("form_getpass.cpp", getpass);
USEFORM("form_banned.cpp", banned);
USEFORM("form_gearratios.cpp", gearratios);
USEFORM("common_stuff.cpp", _common);
USEFORM("messageform.cpp", message);
USEFORM("inputForm.cpp", input);
USEFORM("teammessage_post.cpp", teammessage);
USEFORM("formHelp.cpp", Help);
USEFORM("form_trace.cpp", frm_trace);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "";
                 Application->CreateForm(__classid(Tmain), &main);
		Application->CreateForm(__classid(Trooms), &rooms);
		Application->CreateForm(__classid(Tcreateroom), &createroom);
		Application->CreateForm(__classid(Texcept), &except);
		Application->CreateForm(__classid(Tmodtools), &modtools);
		Application->CreateForm(__classid(Tdistributefunds), &distributefunds);
		Application->CreateForm(__classid(Tcreatechallenge), &createchallenge);
		Application->CreateForm(__classid(Tchallengeinfo), &challengeinfo);
		Application->CreateForm(__classid(Tchallengeinfo_completed), &challengeinfo_completed);
		Application->CreateForm(__classid(Trequest), &request);
		Application->CreateForm(__classid(Tgetpass), &getpass);
		Application->CreateForm(__classid(Tbanned), &banned);
		Application->CreateForm(__classid(Tgearratios), &gearratios);
		Application->CreateForm(__classid(T_common), &_common);
		Application->CreateForm(__classid(Tmessage), &message);
		Application->CreateForm(__classid(Tinput), &input);
		Application->CreateForm(__classid(Tteammessage), &teammessage);
		Application->CreateForm(__classid(THelp), &Help);
		Application->CreateForm(__classid(Tfrm_trace), &frm_trace);
		Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------








