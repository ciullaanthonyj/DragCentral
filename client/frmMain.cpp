//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop

#include "frmMain.h"
#include "roomlistForm.h"
#include "exceptForm.h"
#include "race_flash.h"
#include "race_game.h"
#include "race_gameandflash.h"
#include "account.h"
#include "challenge.h"
#include "myvcl.h"
#include "form_modtools.h"
#include "caudio.h"
#include "team.h"
#include "form_distributefunds.h"
#include "form_teamchallenge.h"
#include "form_challengeinfo.h"
#include "form_challengeinfo_completed.h"
#include "cars.h"
#include "confirmForm.h"
#include "parts.h"
#include "form_getpass.h"
#include "form_banned.h"
#include "form_gearratios.h"
#include "cchecker.h"
#include "inputForm.h"
#include "common_stuff.h"
#include "messageform.h"
#include "teammessage_post.h"
#include "cprofiler.h"
#include "csecstrings.h"
#include "formHelp.h"
#include "utils_testing.h"
#include "form_trace.h"
#include "myflash.h"
//---------------------------------------------------------------------------
#define MAXCAR 10
#pragma package(smart_init)
#pragma link "ShockwaveFlashObjects_OCX"
#pragma link "sensor"
#pragma link "sensortab"
#pragma link "cgrbutton"
#pragma link "cgrbutton"
#pragma link "sensor"
#pragma link "sensortab"
#pragma link "ShockwaveFlashObjects_OCX"
#pragma resource "*.dfm"

Tmain *main;
//---------------------------------------------------------------------------
__fastcall Tmain::Tmain(TComponent* Owner)
		: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tmain::loginTmrTimer(TObject *Sender)
{
  loginTmr->Enabled = false;

//  Server.ServerName = ini->ReadString("Connection","host","nazarkuliev.biz");
//  Server.Dir = ini->ReadString("Connection","dir","/cars/");

  Server.ServerName = encoder.decode("93216FE6DEDABF555FA156") /*"mrgame.info"*/;
  Server.Dir = encoder.decode("D15CD1EBA60350C8F5") /*"/game/sr/"*/;


  if(command("localhost")) {
	  Server.ServerName = "localhost";
	  Server.Dir = "/mrgame.info/sr/";
	  Trace("Connecting to localhost");
  }

  Server.UseSocket = ini->ReadBool("Connection","usesocket",0);
  try {
	Server.Connect();
  } catch (...) {
	ShowMessage("Error occured while connecting to game server.\r\nAre you connected to Internet?");
	return;
  }

  if(ini->ReadBool("Connection","autopass",false)) {
	ednick->Text = ini->ReadString("Connection","login","");
	edpass->Text = ini->ReadString("Connection","pass","");
	if(ednick->Text!="")
	  btnlogin->OnClick(0);
  }

}
//---------------------------------------------------------------------------

void __fastcall Tmain::sendClick(TObject *Sender)
{
  // for lame ping checking
  Server.chat->Timer.Start();
  AnsiString msg = mess->Lines->Text;
  if(msg.Trim()=="") return;
  static AnsiString previousmessage = "\r\n\r\n";
  if(previousmessage==msg.Trim()) {
	mess->Lines->Clear();
	throw Exception("Flood control");
  }
  previousmessage = msg.Trim();
  Server.SendMessage(msg,"");
  mess->Lines->Clear();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------

namespace Engine {
  void SelectCar(AnsiString carnum) {
    flsetvar(main->flselected,"_root.Cars",carnum+"||");
/*	switch(StrToIntDef(carnum,0)) {
		case 1: main->selected_image->Picture->Assign(_common->sel_01->Picture); break;
		case 2: main->selected_image->Picture->Assign(_common->sel_02->Picture); break;
		case 3: main->selected_image->Picture->Assign(_common->sel_03->Picture); break;
		case 4: main->selected_image->Picture->Assign(_common->sel_04->Picture); break;
		case 5: main->selected_image->Picture->Assign(_common->sel_05->Picture); break;
		default: assert(false);
	}*/
  }

}

void __fastcall Tmain::messKeyPress(TObject *Sender, char &Key)
{
  if(Key==13) {
	btnsend->OnClick(0);
    Key=0;
  }
  if(mess->Lines->Text.Length()>=128) {
    int save = mess->SelStart;
    mess->Lines->Text =
      mess->Lines->Text.SubString(1,128);
    mess->SelStart = save;
  }

}
//---------------------------------------------------------------------------


static bool SelectPage_ChangingPage = false;
//extern TStringList* dbstrings;
void __fastcall Tmain::appException(TObject *Sender,
	  Exception *E)
{
  SelectPage_ChangingPage = false;
  Screen->Cursor = crDefault;
  Server.Stack.Stacking = false;
  Server.Stack.QueriesSent = false;

//	   dbstrings->SaveToFile(
//		 ExtractFileDir(Application->ExeName)+"\\dbg.txt");
  if(AnsiContainsStr(E->Message," banned")) {
	banned->memo->Lines->Text = E->Message;
	banned->ShowModal();
  }
  else {
	except->WebSite = ini->ReadString("Web","MainSite","www.drag-central.com");
	except->memo->Lines->Text = E->Message;
	if(!except->Visible)
	  except->ShowModal();
  }
}
//---------------------------------------------------------------------------


void __fastcall Tmain::FormDestroy(TObject *Sender)
{
  delete PMS;
  delete ini;
  delete PrivatesWaiting;
  Server.chat->OnRecieveMessage = NULL;
  Server.chat->OnRoomPeopleUpdate = NULL;
  Server.chat->OnSysMessages = NULL;
}

void SetGrButtons(TComponent* c, int Value) {
  for(int i=0;i<c->ComponentCount;i++)
	if(dynamic_cast<TGrButton*>(c->Components[i])!=NULL) {
	  TGrButton* lb = (TGrButton*)(c->Components[i]);
	  lb->Font->Name = "Tahoma";
	  lb->Font->Size = 10;
	  lb->TabMargin = Value;
	  lb->ColorDown = 0xffffff;
//	  lb->ColorUp = 0xffffff;
	  lb->ColorUp = 0x2bf823;
	  lb->ExtImage = _common->_ButtonsBack;
//	  lb->ColorDown = clWhite;
	}
}
bool AnyParamStr(AnsiString value) {
  for(int i=0;i<20;i++)
   if(ParamStr(i)==value) return true;
  return false;
}

int StartupGameTimerInterval;
void CarViewLoad(TShockwaveFlash* fl)
  {

	fl->LoadMovie(0,wc(ExtractFilePath(Application->ExeName)+"\\flash\\garage.swf"));
	flsetvar(fl,"_root.SingleCar","1",false);
	flsetvar(fl,"_root.StageWidth",fl->Width,false);
	flsetvar(fl,"_root.backim._x",-fl->Left+(900-fl->Width)/2,false);
	flsetvar(fl,"_root.backim._y",-fl->Top+(370-fl->Height)/2,false);
	flsetvar(fl,"_root.CurrentCar",-1,false);
	flsetvar(fl,"_root.Cars","",false);
  }

void __fastcall Tmain::FormCreate(TObject *Sender)
{

  registercarlist->ColWidths[0] = 0;
  registercarlist->ColWidths[1] = registercarlist->Width;
  pages->Left = -4;
  pages->Top = 0;
  outgoingpanel->Align = alClient;
  IncomingPanel->Align = alClient;
  profiler.doprofile = AnyParamStr("-profile");
  Closing = false;
  teamchallenges_pages->ActivePage = team_incoming;
  DecimalSeparator = '.';
  StartupGameTimerInterval = GameTimer->Interval;
  Application->Title = "MyRacingGame";
  this->Caption = Application->Title;
  lbMainVersion->Caption = VERSION + (TestVersion?" TEST":"");
  lbChatVersion->Caption = VERSION + (TestVersion?" TEST":"");
  lbLoginVersion->Caption = VERSION  + (TestVersion?" TEST":"");
  this->DoubleBuffered = true;
  ini = new TIniFile(ChangeFileExt( Application->ExeName, ".ini" ));
  ednick->Text = ini->ReadString("User","LastNick","");
  pages->ActivePage = tablogin;

  pageschallenges->ActivePage = tabincoming;
  ActiveControl = ednick;
  Server.chat->ShowQuitEnter = false; //ini->ReadBool("Debug","ShowQuitEnter",false);
  Server.chat->ShowMessageTime = ini->ReadBool("Debug","ShowMessageTime",false);
  Server.chat->OnRecieveMessage = OnChatMessage;
  Server.chat->OnRoomPeopleUpdate = inroomupdateClick;
  Server.chat->OnSysMessages = OnServerSystemMessages;
  pages->Top = -24;
  Tests->Visible = (AnyParamStr("-test") && FileExists("\\dragcentraltesting."));
  this->ClientHeight = tablogin->Height;
  this->ClientWidth = tablogin->Width;
  PMS = new TList;
  PrivatesWaiting = new TStringList;
  //remove blinking
  RemoveImageBlink(this);
  flgarage->LoadMovie(0,wc(ExtractFilePath(Application->ExeName)+"\\flash\\garage.swf"));
  flsetvar(flgarage,"_root.StageWidth",flgarage->Width,false);
  flsetvar(flgarage,"_root.CorrectionY",70 ,false);
  flsetvar(flgarage,"_root.garageback._visible",1,false);
  {
	TShockwaveFlash* fl = flgarage;
	flsetvar(fl,"_root.backim._visible",0,false);
	flsetvar(fl,"_root.garageback._x",-fl->Left+(900-fl->Width)/2,false);
	flsetvar(fl,"_root.garageback._y",-fl->Top+(370-fl->Height)/2,false);
  }

  CarViewLoad(flchallengecar);
  CarViewLoad(flchallengeanswercar);
  CarViewLoad(flcarreg);
  flsetvar(flcarreg,"_root.ViewScale","110",false);
  flsetvar(flcarreg,"_root.backim._visible","0",false);
  CarViewLoad(fldealer);
  flsetvar(fldealer,"_root.ViewScale","120",false);
  CarViewLoad(flmodcar);
  flsetvar(flmodcar,"_root.ViewScale","120",false);
  CarViewLoad(flmaintcar);
  flsetvar(flmaintcar,"_root.ViewScale","120",false);

//  flsetvar(fl,encoder.decode("A16EE91D493829FFC14B") /*"_root.zoom"*/,zoom);

//  Cars.LoadFlash(flmodcar);
//  Cars.LoadFlash(flracercar);
  flchallengebadges->LoadMovie(0,wc(ExtractFilePath(Application->ExeName)+"\\flash\\badges.swf"));
  flsetvar(flchallengebadges,"_root.backim._visible",0,false);
  flsetvar(flchallengebadges,"member._visible",0,false);
  flsetvar(flchallengebadges,"admin._visible",0,false);
  flsetvar(flchallengebadges,"moder._visible",0,false);
  flsetvar(flchallengebadges,"100k._visible",0,false);
  flsetvar(flchallengebadges,"rt._visible",0,false);

  flaccountbadges->LoadMovie(0,wc(ExtractFilePath(Application->ExeName)+"\\flash\\badges.swf"));
  flsetvar(flaccountbadges,"member._visible",0,false);
  flsetvar(flaccountbadges,"admin._visible",0,false);
  flsetvar(flaccountbadges,"moder._visible",0,false);
  flsetvar(flaccountbadges,"100k._visible",0,false);
  flsetvar(flaccountbadges,"rt._visible",0,false);

  flsetvar(flaccountbadges,"_root.backim._x",-flaccountbadges->Left);
  flsetvar(flaccountbadges,"_root.backim._y",-flaccountbadges->Top);
  flchallenge->LoadMovie(0,wc(ExtractFilePath(Application->ExeName)+"\\flash\\garage.swf"));
  flsetvar(flchallenge,"_root.CorrectionY","102",false);
  flsetvar(flchallenge,"_root.backim._x",IntToStr(-flchallenge->Left));
  flsetvar(flchallenge,"_root.backim._y",-flchallenge->Top + (370-flchallenge->Height)/2);
  flsetvar(flchallenge,"_root.backim._visible",0,false);
  flsetvar(flchallenge,"_root.StageWidth",pages->Width - flchallenge->Left);
  flsetvar(flchallenge,"_root.thumbs_mc._y",120);
  flsetvar(flchallenge,"_root.SelectedCar",-1);

  {
    TShockwaveFlash* fl = flselected;
	fl->LoadMovie(0,wc(ExtractFilePath(Application->ExeName)+"\\flash\\garage.swf"));
	flsetvar(fl,"_root.SingleCar","1",false);
	flsetvar(fl,"_root.StageWidth",fl->Width,false);
	flsetvar(fl,"_root.backim._x",-fl->Left+(900-fl->Width)/2,false);
	flsetvar(fl,"_root.backim._y",-fl->Top+(370-fl->Height)/2,false);
	flsetvar(fl,"_root.CurrentCar",-1,false);
	flsetvar(fl,"_root.Cars","2||",false);
	flsetvar(fl,"_root.ViewScale","26",false);
	flsetvar(fl,"_root.CorrectionX","270",false);
	flsetvar(fl,"_root.CorrectionY","112",false);
  }

  RaceFlash.flroad.mv = flroad;
  RaceFlash.flupview = flupview;
  RaceFlash.flcarpanel = flroad;
  RaceFlash.flstaging = flstaging;
  RaceFlash.LoadFlashMovies();


  Account.srv = &Server;
  Challenge.s = &Server;
  Team.s = &Server;
  TeamChallenge.s = &Server;
  Cars.s = &Server;
  Parts.s = &Server;

  FixStringGrids(this);
  RemoveLabelCaptions(this);
  ReplaceEditsColor(this,0x909090,WebColor(ini->ReadString("Colors","MainForm","0xc7c2bf")));
  GridSelectColor = WebColor(ini->ReadString("Colors","GridSelect","0xff8802"));


  this->Color = WebColor(ini->ReadString("Colors","MainForm","0xc7c2bf"));
  pannoteam->Color = this->Color;
  Audio.SetVolume((float)ini->ReadInteger("Sound","Volume",100)*0.1/100);
  GameAndFlash.LoadAudio();
  AnsiString prefix = ExtractFilePath(Application->ExeName);
  if(ini->ReadBool("Sound","Music",true))
  Audio.LoadSample("music",prefix+"aud\\music.mp3",7);
  Audio.LoadSample("private",prefix+"aud\\private.mp3",7);
  Audio.SetSampleLoop("music");
  Audio.PlaySample("music");
  TNotifyEvent save = team_nb->OnPageChanged;
  team_nb->OnPageChanged = 0;
  team_nb->PageIndex = 0;
  team_nb->OnPageChanged = save;

}
//---------------------------------------------------------------------------

void __fastcall Tmain::roomlistClick(TObject *Sender)
{
  if(rooms->Visible) {
    rooms->SetFocus();
    return;
  }

  Screen->Cursor = crAppStart;
  rooms->room = Server.room;
  RoomNameLabel->Caption = roomname->Caption;
  rooms->lbCurrentRoom->Caption = roomname->Caption;
  rooms->Left = this->Left+150 - 20 - 8;
  rooms->Top = this->Top + 120 - 10 - 5;
  rooms->Server = &Server;
  rooms->roomlist = Server.RoomList();
  if(Signature("Sorry",rooms->roomlist)) {
     throw Exception(rooms->roomlist);
  }
  // change to tabchat no SelectPage parsing
  if(pages->ActivePage!=tabchat)
    pages->ActivePage=tabchat;
  Screen->Cursor = crDefault;
  rooms->ShowModal();
  Application->ProcessMessages();
  if(pages->ActivePage==tabchat)
    mess->SetFocus();
  if(rooms->Changed) {
	chat->Lines->Clear();
	RoomNameLabel->Caption = rooms->roomname;
	roomname->Caption = rooms->roomname;
  }

}
//---------------------------------------------------------------------------
bool roomlistupdated = false;
void __fastcall Tmain::inroomupdateClick(TObject *Sender)
{
	// THIS METHOD MUST BE CALLED LIKE Synchronize(method) FROM ServerThread

	int saveRow = inroom->Row;
	int saveTopRow = inroom->TopRow;
	int inRoomCount = 0;
	AnsiString saveNick = inroom->Cells[1][inroom->Row];
	{
	  cStringTable tbl(Server.chat->PeopleInRoom);
	  cGridFiller gridline(inroom);
	  while(tbl.GetLine()) {
		gridline[0] = "";
		gridline[1] = encoder.encode(tbl++); // nick
		gridline[2] = tbl++; // flags
		gridline[3] = tbl++; // id
		if(tbl.fieldscount>=4) {
		  AnsiString invis = tbl++;
		  gridline[4] = invis; // invisible (make them italic)
		  if(invis==S_1)
			inRoomCount--;
		}
		if(tbl.fieldscount>=5) {
		  gridline[5] = tbl++; // no privates
		}

		gridline++;
		inRoomCount++;
	  }
	}

    int findnick = -1;
    for(int i=0;i<inroom->RowCount;i++) {
      if (inroom->Cells[1][i]==saveNick) {
         findnick = i;
         break;
      }
    }
    if(findnick==-1) {
      if(saveRow>=inroom->RowCount)
        inroom->Row = inroom->RowCount-1;
      else
        inroom->Row = saveRow;
    }
    else
	  inroom->Row = findnick;

	if(saveTopRow>=inroom->RowCount) {
	  if(inroom->RowCount!=0)
		inroom->TopRow = inroom->RowCount-1;
	}
	else
	  inroom->TopRow = saveTopRow;

	int CanBeVisible = inroom->Height/inroom->DefaultRowHeight;
	if(inroom->VisibleRowCount<CanBeVisible) {
       inroom->TopRow = max(0,inroom->RowCount-CanBeVisible);
    }
	lbRacersInRoom->Caption = inRoomCount;

  for(int i=0;i<PrivatesWaiting->Count;i++)
	marknewmessage(PrivatesWaiting->Strings[i]);
  roomlistupdated = true; // flat for testing
}
//---------------------------------------------------------------------------


void __fastcall Tmain::FormClose(TObject *Sender, TCloseAction &Action)
{
  PSAVE;
  this->Visible = false;
  Closing = true;
  Application->ProcessMessages();
  if(pages->ActivePage==tabrace)
    SelectPage(tabmain);
  // to record you lose
  if(GameTimer->Enabled)
   GameTimer->OnTimer(0);
  Server.StartStack();
  while(Server.StackQueries()) {
    if(Server.room!="")
      Server.QuitChat();
    Server.Logout();
  }
//  Server.Disconnect();
}
//---------------------------------------------------------------------------

void Tmain::SelectPage(TTabSheet* newpage) {
  if(SelectPage_ChangingPage) return;
//  ModerMenu->AutoPopup = Server.moder;
//  chatkickban->Visible = Server.moder;
  SpeedUp->ShortCut = NULL;
  SpeedDown->ShortCut = NULL;

  garagetimer->Enabled = newpage == tabgarage;

  if(pages->ActivePage==tabrace && newpage!=tabrace) {
     // STOP AUDIO
     GameAndFlash.msgGameEnd();
  }


  if(newpage==tablogin) {
//     ednick->Text = "";
     edpass->Text = "";
     if(!Audio.IsSamplePlaying("music"))
     Audio.PlaySample("music");
  }


  if(newpage!=tablogin 
	 &&newpage!=tabcarselect && newpage!=tabtos && newpage!=tabregister) {
	toppanel->Parent = newpage;
//    bottompanel->Parent = newpage;
  }

  if(newpage==tabchat) {
	try {
	   pages->ActivePage = tabchat;
	   roomlistClick(0);
	} catch (Exception &E) {
	   SelectPage(tabmain);
	   throw Exception(E.Message);
	}
  }

  if(tabrace==newpage) {
	 SpeedUp->ShortCut = TextToShortCut("Up");
	 SpeedDown->ShortCut = TextToShortCut("Down");
     Challenge.message = message_edt->Text;
  }

  if(pages->ActivePage == tabchat && newpage!=tabchat) {
    Server.QuitChat();
    // kill privates
    for(int i=(PMS->Count-1);i>=0;i--) {
	  ((Tpms*)(PMS->Items[i]))->Release();
      PMS->Delete(i);
    }
  }


  if(newpage==tabregister) {
    edaccnick->Text = "";
    edaccpass->Text = "";
    edaccpass2->Text = "";
    edaccemail->Text = "";
    edaccemail2->Text = "";
  }


  if(newpage==tabrace) {
	pages->ActivePage = tabloading;
	Screen->Cursor = crAppStart;
	Application->ProcessMessages();
	// START RACE
	AnsiString usercarid = Cars.usercarid;
	AnsiString carid = Cars.selectedcarid;

	if(Challenge.TeamRace) {
	   usercarid = Challenge.usercarid;
	   carid = Challenge.carid;
	} else
	if(Challenge.Answering && Challenge.withRacer) {
	  usercarid = Challenge.answerusercarid;
	  carid = Challenge.answercarid;
	}

	Cars.GetUpgradeValues(usercarid);
	Game.CarPhys.mass = Cars.GetUpgradeValues_Weight;
	Game.CarPhys.horses = Cars.GetUpgradeValues_Horses+Cars.addhp;
	Game.CarPhys.reduceslip = Cars.reduceslip;
	Game.CarPhys.Nitro.Installed = Cars.Nitro.hp!=0;
	Game.CarPhys.Nitro.Left = Cars.Nitro.left;
	Game.CarPhys.Nitro.Horses = Cars.Nitro.hp;
	Game.CarPhys.Charger.turbocharger = Cars.Charger.turbocharger;
	Game.CarPhys.Charger.supercharger = Cars.Charger.supercharger;
	Game.CarPhys.MagicGear.Installed = Cars.MagicGear.Installed;
	Game.CarPhys.MagicGear.g1 = Cars.MagicGear.g1;
	Game.CarPhys.MagicGear.g2 = Cars.MagicGear.g2;
	Game.CarPhys.MagicGear.g3 = Cars.MagicGear.g3;
	Game.CarPhys.MagicGear.g4 = Cars.MagicGear.g4;
	Game.CarPhys.MagicGear.g5 = Cars.MagicGear.g5;
	Game.CarPhys.MagicGear.g6 = Cars.MagicGear.g6;
	Game.CarPhys.maxspeednum = Cars.maxspeednum;
	Game.CarPhys.enginedamage = Cars.EngineDamage;
	GameAndFlash.carid = carid; //SELECT CAR BUG

    GameAndFlash.msgGameStart(StartupGameTimerInterval);

    if(!Challenge.Create_Start()) {
      SelectPage(tabmain);
      throw Exception("Can not start challenge.");
    }
    if(Challenge.withRacer) {
      Game.OppLogic.OffLine = true;
	  if(Challenge.Answering) {
        Game.OppLogic.RaceTime = Challenge.Opponent.RaceTime;
        Game.OppLogic.BracketTime = Challenge.Opponent.BracketTime;
        Game.OppLogic.FalseStart = Challenge.Opponent.FalseStart;
        Game.OppLogic.ReactionTime = Challenge.Opponent.ReactionTime;
        if(!Game.OppLogic.FalseStart)
          Game.OppLogic.BreakOut = Challenge.Opponent.RaceTime<Challenge.Opponent.BracketTime;
        Game.OppLogic.Finished = true;
      }
      if(Challenge.HeadsUp)
        Challenge.Car.BracketTime = 0;
    }
    Game.CarLogic.BracketTime = Challenge.Car.BracketTime;
//    lbChallengeInfo->Caption = Challenge.GetInfoString();
    btngotomain->Visible = false;
    // to prepare flash
    GameAndFlash.OnGameTimer();
    GameTimer->Enabled = true;
    Screen->Cursor = crDefault;
  }

  if(newpage==tabracestart) {
     // save challenge info if it was started earlier
     pages->ActivePage = newpage;
     if(Challenge.StartedAndNotRecorded)
       GameTimer->OnTimer(0);
	 Challenge.Reset();
  }

  if(newpage==tabwagerselect) {
	 message_edt->Text = "";
     pages->ActivePage = newpage;
  }

  if(newpage==tabmain) {
  }
  if(newpage==tabchallenges) {
      incoming_buttonClick(0);
  }

  if(pages->ActivePage!=newpage) {
    SelectPage_ChangingPage = true;
    pages->ActivePage = newpage;
    SelectPage_ChangingPage = false;
  };

}

void __fastcall Tmain::btnloginClick(TObject *Sender)
{
	Screen->Cursor = crAppStart;
	if(ini->ReadBool("Debug","ShowHost",false)) {
       Caption = "http://"+Server.ServerName;
      if(ini->ReadBool("Connection","usesocket",0))
         Caption = Caption + " socket " + Server.php->RemotePort;
      else
         Caption = Caption + " no socket";
    }
    Server.StartStack();
    while(Server.StackQueries()) {
      Server.Login(ednick->Text,edpass->Text,VERSION);
      Cars.GetSelected();
      Cars.GetFreeLeft();
    }

    if(TestVersion && !Server.moder && !(AnyParamStr("-test") && FileExists("\\dragcentraltesting.")) )
      throw Exception("Test Version: ONLY MODERATORS ALLOWED");
	SelectPage(tabmain);
	Engine::SelectCar(Cars.selectedcarid);
	selectedcarhint->Hint = Cars.selectedcarname;
    if(ini->ReadBool("Debug","ShowUser",false)) {
      Caption = Caption + " " + Server.nick;
      Application->Title = Server.nick;
    }

  ini->WriteString("User","LastNick",ednick->Text);
  Audio.FadeOutSample("music",5);
  Screen->Cursor = crDefault;

  modtools->s = &Server;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnforgotClick(TObject *Sender)
{
  getpass->Left = this->Left + 230;
  getpass->Top = this->Top + 170;
  if(mrOk==getpass->ShowModal()) {
    try {
      Server.SendPass(getpass->edforgotemail->Text);
      SelectPage(tablogin);
    } catch (Exception &E) {
      message->ShowMessage(E.Message);
   }
  }
  //  SelectPage(tabforget);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btncancelClick(TObject *Sender)
{
  SelectPage(tablogin);
}
//---------------------------------------------------------------------------






void __fastcall Tmain::btnsendpassClick(TObject *Sender)
{
/*  try {
	Server.SendPass(edforgotemail->Text);
	SelectPage(tablogin);
  } catch (Exception &E) {
	message->ShowMessage(E.Message);
  }*/
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btncreateClick(TObject *Sender)
{
  if(edaccnick->Text.Trim()!=edaccnick->Text) {
     edaccnick->Text = edaccnick->Text.Trim();
     throw Exception("Bad nickname.");
  }
  if(edaccnick->Text.Trim().Length()==0) {
     throw Exception("Empty nicks not allowed.");
  }
  if(edaccnick->Text.Length()>14) {
     message->ShowMessage("Nick too long. Maximum - 14 symbols.");
     return;
  }
  if(AnsiContainsStr(edaccnick->Text,"|")) {
     message->ShowMessage("No [|] symbol allowed in nick.");
     return;
  }

  if(edaccpass->Text!=edaccpass2->Text) {
    message->ShowMessage("Password entries do not match.");
    return;
  }

  if(edaccemail->Text!=edaccemail2->Text) {
    message->ShowMessage("E-mail entries do not match.");
    return;
  }

  if(edaccnick->Text.Length()==0) {
    message->ShowMessage("Please enter nick.");
    return;
  }

  if(edaccpass->Text.Length()==0) {
    message->ShowMessage("Please enter pass.");
    return;
  }

  if(edaccemail->Text.Length()==0) {
    message->ShowMessage("Please enter e-mail.");
    return;
  }


  AnsiString res = Server.CreateAccount(edaccnick->Text,edaccpass->Text,edaccemail->Text,
    gender->ItemIndex==1, gender->ItemIndex==2,GridCell(registercarlist,0));
  message->ShowMessage(res);
  SelectPage(tablogin);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnregisterClick(TObject *Sender)
{
  SelectPage(tabcarselect);        
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnhaveaccountClick(TObject *Sender)
{
  SelectPage(tablogin);        
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnvehbackClick(TObject *Sender)
{
  SelectPage(tablogin);        
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnselectvehClick(TObject *Sender)
{
  SelectPage(tabtos);        
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnnotagreeClick(TObject *Sender)
{
  SelectPage(tabcarselect);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnagreeClick(TObject *Sender)
{
  SelectPage(tabregister);        
}
//---------------------------------------------------------------------------


void __fastcall Tmain::edpassKeyPress(TObject *Sender, char &Key)
{
  if(Key==13) {
    Key=0;
    btnlogin->OnClick(0);
  }
}
//---------------------------------------------------------------------------

void Tmain::marknewmessage(AnsiString nick) {
  for(int i=0;i<inroom->RowCount;i++)
  if(encoder.decode(inroom->Cells[1][i])==nick)
    inroom->Cells[0][i] = "msg";
  if(PrivatesWaiting->IndexOf(nick)==-1)
    PrivatesWaiting->Add(nick);
}

void Tmain::unmarknewmessage(AnsiString nick) {
  for(int i=0;i<inroom->RowCount;i++)
  if(encoder.decode(inroom->Cells[1][i])==nick)
    inroom->Cells[0][i]= "";
  int i = PrivatesWaiting->IndexOf(nick);
  if(i!=-1) PrivatesWaiting->Delete(i);
}

TColor Tmain::chatcolor(AnsiString nickortext, bool admin,bool moder, bool supermod, bool member, bool female) {
  TColor res;

  if(nickortext=="sys") {
	res = WebColor(ini->ReadString("Chat Colors","sys_text","0x00ff00"));
	return res;
  }

  AnsiString type;
  type = "member";
  if(!member)
	type = "nonmember";
  if(moder)
	type = "moder_"+type;
  if(female && moder)
	type = type+"_female";
  if(admin)
	type = "admin";

  if(supermod) {
	  type = "supermod";
	  if(!member) {
		  type += "_member";
	  } else {
		  type += "_nonmember";
	  }
	  if(female) {
		  type += "_female";
	  }
  }
  
  res = WebColor(ini->ReadString("Chat Colors",type+"_"+nickortext,"0xffffff"));
  return res;
}

void __fastcall Tmain::OnChatMessage(TObject *Sender)
{
  sRecieveMessage* msg = (sRecieveMessage*) Sender;
  AnsiString first = "";
  AnsiString second;
  if(msg->sysmessage) {
	if(msg->nick.val()=="System") {
      first = msg->nick.val() + ": ";
      second = msg->text;
    }
    else
	  second = msg->nick.val() + msg->text;
  }
  else {
	first = msg->nick.val() + ": ";
    second = msg->text;
  }

  TRichEdit* tochat = chat;
  TColor nickcolor = (TColor)-1;
  TColor msgcolor = (TColor)-1;
  TColor syscolor = (TColor)-1;
  nickcolor = chatcolor("nick",msg->admin,msg->moder,msg->supermod,msg->member,msg->female);
  msgcolor = chatcolor("text",msg->admin,msg->moder,msg->supermod,msg->member,msg->female);
  syscolor = chatcolor("sys",msg->admin,msg->moder,msg->supermod,msg->member,msg->female);

  if(msg->priv) {
    Tpms* pms;
    // sender myself - private with somebody
	if(msg->nick.val()==Server.nick)
	  pms = PMSForm(msg->tonick);
	// sender somebody - private
	else {
	  pms = PMSForm(msg->nick);       
//      if(ini->ReadInteger("Chat","PrivatePopup",1)) {      
		pms->Visible = true;
//		pms->SetFocus();
//      }
	  if(!pms->Focused())
		marknewmessage(msg->nick.val());

	  // send message to main window
//      if(ini->ReadInteger("Chat","PrivateMessageNotice",1)) {
		  int StartCount = tochat->Lines->Count;
		  try {
			addline(chat,msg->nick.val() + " - private message",syscolor,true,"",msgcolor,false);
			if(!AppActive)
			  Audio.PlaySample("private");
		  // catch rich edit insert line error
		  } catch (...) {
//        }
		int Change = chat->Lines->Count - StartCount;
		// don't scroll when cursor in chat window
//        if(!chat->Focused())
		  chat->Perform(EM_LINESCROLL,0,Change);
	  }

	}
	tochat = pms->chat;
  };

  int StartCount = tochat->Lines->Count;
  try {
	if(msg->sysmessage) {
	  nickcolor = syscolor;
	  msgcolor = syscolor;
	}
	addline(tochat,killbadchars(first),nickcolor,true,killbadchars(second),msgcolor,false);
  // catch rich edit insert line error
  } catch (...) {
  }
  int Change = tochat->Lines->Count - StartCount;
//  if(!chat->Focused())
	tochat->Perform(EM_LINESCROLL,0,Change);

  if(msg->youwerekicked) {
	message->ShowMessage("You have been kicked. "+msg->reason);
	btnmaintab->OnClick(0);
  }

  if(msg->youwerechatbanned) {
	message->ShowMessage("You have been kicked and CHAT banned. "+msg->reason);
	btnmaintab->OnClick(0);
  }

  if(msg->youweregamebanned) {
    message->ShowMessage("You have been kicked and GAME banned. "+msg->reason);
    Server.Logout();
    SelectPage(tablogin);
  }

}
//---------------------------------------------------------------------------
void __fastcall Tmain::PMSActivate(TObject* Sender) {
  unmarknewmessage(((Tpms*)Sender)->withnick);
}

Tpms* Tmain::PMSForm(AnsiString withnick) {
  Tpms* pms = NULL;
  // LOOK IF ALREADY OPENED
  for(int i=0;i<PMS->Count;i++) {
    if(((Tpms*)(PMS->Items[i]))->withnick == withnick) {
       pms = (Tpms*) (PMS->Items[i]);
       break;
    }
  }
//  TForm* f = Screen->ActiveForm;
  if(pms==NULL) {
	static int lastnewtop = 0;
	static int lastnewleft = 0;
	Application->CreateForm(__classid(Tpms), &pms);
	pms->Top = (Screen->Height - pms->Height) / 2 - 42;
	pms->Left = (Screen->Width - pms->Width) / 2;
	pms->PMSActivate = PMSActivate;
	if(lastnewtop!=0) {
      pms->Top = lastnewtop + 30;
	  pms->Left = lastnewleft + 30;
    }

    lastnewtop = pms->Top;
    lastnewleft = pms->Left;

    pms->lbwithnick->Caption = withnick;
    PMS->Add(pms);
    pms->withnick = withnick;
    pms->Server = &this->Server;
    pms->OnSmokem = smokemClick;
    pms->OnBlock = blockClick;
  };
//  pms->Visible = true;
  return pms;
}

void __fastcall Tmain::inroomDblClick(TObject *Sender)
{
  AnsiString withnick = encoder.decode(inroom->Cells[1][inroom->Row]);
  if(withnick.Length()==0) return;
  if(withnick==Server.nick.val()) {
     throw Exception("Want to talk to yourself?");
  }
  if(GridCell(inroom,5)==S_1 && !Server.moder) {
    message->ShowMessage("This racer doesn't accept private messages.");
    return;
  }

  TForm* f = PMSForm(withnick);
  Server.UnignoreUser(withnick);
  f->Visible = true;
  f->SetFocus();

  unmarknewmessage(withnick);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnchatClick(TObject *Sender)
{
	this->SelectPage(tabchat);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Image10Click(TObject *Sender)
{
  this->SelectPage(tabmain);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::appActivate(TObject *Sender)
{
  this->Visible = true;
  AppActive = true;

  static bool ft = true;
  if(!ft) return;
  ft=false;

  if(command("carviewtest")) {
	  int pause = StrToInt(param("carviewtest"));


	  tabselectracer->OnShow = 0;
	  pages->ActivePage = tabselectracer;
	  flchallenge->Visible = true;
	  flsetvar(flchallenge,"_root.Cars","1|1|2|2|3|3|4|4|5|5|6|6|7|7|8|8|9|9|10|10|",false);
	  Application->ProcessMessages();
	  Sleep(pause*10);



	  tabchallenge->OnShow = 0;
	  pages->ActivePage = tabchallenge;
	  for(int i=0;i<MAXCAR;i++) {
		  AnsiString car = IntToStr(i+1);
		  flsetvar(flchallengecar,"_root.Cars",car+"|"+car+"|",false);
		  flsetvar(flchallengeanswercar,"_root.Cars",car+"|"+car+"|",false);
		  Application->ProcessMessages();
		  Sleep(pause*2);
	  }

	  tabgarage->OnShow = 0;
	  pages->ActivePage = tabgarage;
	  flsetvar(flgarage,"_root.Cars","1|1|2|2|3|3|4|4|5|5|6|6|7|7|8|8|9|9|10|10|");

	  if(command("garageviewtest")) {
		return;
	  }
	  Application->ProcessMessages();
	  Sleep(pause*5);

	  tabdealer->OnShow = 0;
	  pages->ActivePage = tabdealer;
	  for(int i=0;i<MAXCAR;i++) {
		  AnsiString car = IntToStr(i+1);
		  flsetvar(fldealer,"_root.Cars",car+"|"+car+"|");
		  Application->ProcessMessages();
		  Sleep(pause*2);
	  }



	  tabmodifications->OnShow = 0;
	  pages->ActivePage = tabmodifications;
	  for(int i=0;i<MAXCAR;i++) {
		  AnsiString car = IntToStr(i+1);
		  flsetvar(flmodcar,"_root.Cars",car+"|"+car+"|");
		  Application->ProcessMessages();
		  Sleep(pause*2);
	  }
	  tabmaintenance->OnShow = 0;
	  pages->ActivePage = tabmaintenance;
	  for(int i=0;i<MAXCAR;i++) {
		  AnsiString car = IntToStr(i+1);
		  flsetvar(flmaintcar,"_root.Cars",car+"|"+car+"|");
		  Application->ProcessMessages();
		  Sleep(pause*2);
	  }

	  tabmain->OnShow = 0;
	  pages->ActivePage = tabmain;
	  for(int i=0;i<MAXCAR;i++) {
		  AnsiString car = IntToStr(i+1);
		  flsetvar(flselected,"_root.Cars",car+"|"+car+"|");
		  Application->ProcessMessages();
		  Sleep(pause*2);
	  }

	  tabcarselect->OnShow = 0;
	  pages->ActivePage = tabcarselect;
	  for(int i=0;i<MAXCAR;i++) {
		  AnsiString car = IntToStr(i+1);
		  flsetvar(flcarreg,"_root.Cars",car+"|"+car+"|");
		  Application->ProcessMessages();
		  Sleep(pause*2);
	  }


	  Close();
  }

  if(command("showtrace")) {
	  frm_trace->Visible = true;
  }

  if(command("modtest")) {
	modtools->ShowModal();

  }

  if(command("bantest")) {
	banned->memo->Lines->Text = "ouch";
	banned->ShowModal();
  }


  if(command("user") && command("pass")) {
	  autologin_tmr->Enabled = true;
  }


}
//---------------------------------------------------------------------------
void __fastcall Tmain::appDeactivate(TObject *Sender)
{
  AppActive = false;
}
//---------------------------------------------------------------------------


void press(TComponent* but,int pause=500) {
  if(but!=NULL) {
    if(dynamic_cast<TImage*>(but)!=NULL)
      ((TImage*)but)->OnClick(0);
    if(dynamic_cast<TButton*>(but)!=NULL)
      ((TButton*)but)->OnClick(0);
  }
  Application->ProcessMessages();
  Sleep(pause);
  Application->ProcessMessages();
}

void __fastcall Tmain::Newaccount1Click(TObject *Sender)
{
  AnsiString name=InputBox("Login name","Login will be same as pass","builder");
  this->SelectPage(tablogin);
  press(0);
  press(btnregister);
  press(btnselectveh);
  press(btnagree);
  edaccnick->Text = name;
  edaccpass->Text = name;
  edaccpass2->Text = name;
  edaccemail->Text = name+"@mail.ru";
  edaccemail2->Text = name+"@mail.ru";
  press(btncreate);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Login1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
//  rooms->autoselect->Enabled = true;
//  press(btnchat);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnracetrackClick(TObject *Sender)
{
  SelectPage(tabracestart);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Racetrack1Click(TObject *Sender)
{
/*  GameTimer->Interval = 1;
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
  press(btnracetrack);
//  press(btncomputerchallenge);
  AnsiString comp=InputBox("Computer select","1(100) or 2(50)","2");
  if(comp=="2")
	press(btncomputertwo);
  if(comp==S_1)
	press(btncomputertwo);
  edbracket->Text = "18.1";
  press(btngorace);
  Game.Testing.CarAutoDrive = true;
  Game.Testing.NoStaging = true;*/

}
//---------------------------------------------------------------------------

void __fastcall Tmain::btncomputerchallengeClick(TObject *Sender)
{
  SelectPage(tabcomputerselect);
  Challenge.withComputer = true;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btncomputertwoClick(TObject *Sender)
{
  // lower computer
  Challenge.forMoney = true;
  // bigger bracket difference
  Game.OppLogic.BracketTime = 14.3;
  // must be lower time
  Game.OppLogic.AI.maxthrottle = 0.18 - (float)random(10)/1000;
  Challenge.Wager = 50.0;
  SelectPage(tabsetbracket);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btngoraceClick(TObject *Sender)
{
  double BT = StrToFloat(edbracket->Text);
  if(BT<2) throw Exception("Please enter realistic bracket time.");

  SelectPage(tabrace);
  // IF NOT FOR TEAM RACE
  Game.CarLogic.BracketTime = BT;
  Challenge.SetBracketTime(edbracket->Text);
  if(!Challenge.TeamRace) {
    Game.msgGameStart();
    RaceFlash.msgGameStart();
    btngotomain->Visible = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::GameTimerTimer(TObject *Sender)
{
  // delay to disable macro recorders
  if(Game.CarLogic.EndStagingMessage) {
    static int i;
    static bool started = false;
    if(!started) {
      started = true;
      i = random(40);
      Screen->Cursor = crAppStart;
    }
    i--;
    if(i>0) {
      return;
    }
    else {
      started = false;
      Screen->Cursor = crDefault;
    }
  }

  PSTART("gameandflash");
  GameAndFlash.OnGameTimer();
  PEND("gameandflash");
//  btnSkipStaging->Visible = Game.CarLogic.Staging;
  // END OF RACE
  // check if quited before
  bool QuitedBeforeEnd = false;
if(Challenge.StartedAndNotRecorded) {

  if(!Game.Finished&&pages->ActivePage!=tabrace) {
     GameAndFlash.msgGameEnd();
     Game.Finished = true;
     Game.YouLose = true;
     Game.YouWin = false;
     QuitedBeforeEnd = true;
     // this must be in game
     Audio.PlaySample("youlose");
  }

  // save challenge results
  if(Game.Finished) {

    btngotomain->Visible = true;
    Cars.Nitro.left = Game.CarPhys.Nitro.Left;
    Cars.EngineDamage = Game.CarPhys.enginedamage;
    Cars.SaveNitrousAndDamage();
    // PRACTICE CHALLENGE
    if(Challenge.Practice) {
      Challenge.StartedAndNotRecorded = false;
    } else
    //COMPUTER CHALLENGE
    if(!Game.OppLogic.OffLine) {
		 if(Game.YouWin)  Challenge.SaveWinComputer(S_1);
		 if(Game.YouLose) Challenge.SaveWinComputer(S_0);
		 if(Game.Draw)    Challenge.SaveWinComputer(S_2);
    } else
    //ONLINE CHALLENGE
    if(Game.OppLogic.OffLine) {
      Challenge.Car.RaceTime = Game.CarLogic.RaceTime;
      Challenge.Car.FalseStart = Game.CarLogic.FalseStart;
      Challenge.Car.ReactionTime = Game.CarLogic.ReactionTime;
      Challenge.Draw = Game.Draw;
      Challenge.YouWin = Game.YouWin;
      Challenge.YouLose = Game.YouLose;

         if(QuitedBeforeEnd) {
		   if(Challenge.Answering) {
             // both fouled
             if(Challenge.Opponent.FalseStart) {
                Game.YouLose = false;
                Game.Draw = true;
             }
           }
           Challenge.Car.FalseStart = true;
           Challenge.Car.RaceTime = -1;
         }

      // TEAM CHALLENGE
      if(Challenge.TeamRace) {
        Challenge.SaveTeamResults();
      }
      // ONLINE CHALLENGE
      if(!Challenge.TeamRace) {
        // CREATING CHALLENGE
		if(!Challenge.Answering) {
           // server must select new car
           Server.StartStack();
           while(Server.StackQueries()) {
             Challenge.Create_End();
             if(Challenge.forCar) {
			   Cars.GetSelected();
			   Engine::SelectCar(Cars.selectedcarid);
			   selectedcarhint->Hint = Cars.selectedcarname;
               Cars.GetFreeLeft();
             }
           }
        }

		// ANSWERING CHALLENGE
		if(Challenge.Answering) {
           Server.StartStack();
           while(Server.StackQueries()) {
			 if(Game.YouWin)  Challenge.SaveAnswer(S_1);
			 if(Game.YouLose) Challenge.SaveAnswer(S_0);
			 if(Game.Draw)    Challenge.SaveAnswer(S_2);
             if(Challenge.forCar) {
               Cars.GetSelected();
               Cars.GetFreeLeft();
             }
           }
        }
      }


    } // end of online challenge
  } // end game finished
}

  if(pages->ActivePage!=tabrace)
      GameTimer->Enabled = false;

}
//---------------------------------------------------------------------------

void __fastcall Tmain::btngotomainClick(TObject *Sender)
{
  SelectPage(tabmain);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Racetrackonline1Click(TObject *Sender)
{
/*  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
  press(btnracetrack);
  press(btnOnlineChallenge);
  racerssearch->Row=0;
  while(racerssearch->Cells[1][racerssearch->Row]!="another")
	racerssearch->Row++;
  press(btnselectracer);
  press(btnBracket);
  press(btnForFun);
  Game.Testing.CarAutoDrive = true;
  Game.Testing.NoStaging = true;*/
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnOnlineChallengeClick(TObject *Sender)
{
  SelectPage(tabselectracer);
  Challenge.withRacer = true;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnselectracerClick(TObject *Sender)
{
  Challenge.withRacerID = GridCell(racerssearch,0);
  AnsiString racercar = GridCell(racercars,3);
  if(Challenge.withRacerID!="" && racercar!="") {
    Challenge.usercarid = Cars.usercarid;
    Challenge.answerusercarid = racercar;
    SelectPage(tabchallengeselect);
  }
}
//---------------------------------------------------------------------------


void __fastcall Tmain::btnBracketClick(TObject *Sender)
{
  Challenge.Bracket = true;
  Challenge.SetBracketTime(edOnlineBracket->Text);
  SelectPage(tabwagerselect);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnForFunClick(TObject *Sender)
{
  Challenge.forFun = true;
  SelectPage(tabrace);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnchallengesClick(TObject *Sender)
{
  SelectPage(tabchallenges);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Challenges1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
  press(btnchallenges);

}
//---------------------------------------------------------------------------

void __fastcall Tmain::Challengesanother1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "another";
  edpass->Text = "another";
  press(btnlogin);
  press(btnchallenges);

}
//---------------------------------------------------------------------------

void __fastcall Tmain::anotheranswerbuilderchallenge1Click(TObject *Sender)
{
  GameTimer->Interval = 1;
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "another";
  edpass->Text = "another";
  press(btnlogin);
  press(btnchallenges);
  incoming_grid->Row = 0;
  while(!Signature("Waiting",incoming_grid->Cells[4][incoming_grid->Row]))
	incoming_grid->Row++;
  press(btnAnswerChallenge);
  if(!Challenge.HeadsUp) {
    edAnswerBracket->Text = 16;
    press(btnAnswerBracket);
  }
  GameTimer->Interval = 1;
  Game.Testing.CarAutoDrive = true;
  Game.Testing.NoStaging = true;
  Game.CarLogic.AI.dofalsestart = Sender==falsestart;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnAnswerChallengeClick(TObject *Sender)
{
  // GET INFORMATION
  AnsiString ChallengeID = GridCell(incoming_grid,0);
  AnsiString membercar = GridCell(incoming_grid,9);
  Challenge.StartAnswerTo(ChallengeID);
  if(Challenge.forCar && membercar==S_1 && !Server.member)
    throw Exception("Can't answer. It is pink slip for member car and you don't have membership.");
  Screen->Cursor = crAppStart;
  AnsiString m = Account.GetBalance();
  Screen->Cursor = crDefault;
  if(Challenge.forMoney && Challenge.Wager>StrToFloat(m)) {
     throw Exception("You don't have enough money ("+m+")");
  }
  Cars.usercarid = Challenge.answerusercarid;
  Cars.selectedcarid = Challenge.answercarid;

  if(Challenge.Bracket)
    SelectPage(tabanswer);

  if(Challenge.HeadsUp)
    SelectPage(tabrace);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnAnswerBracketClick(TObject *Sender)
{
  Challenge.SetBracketTime(edAnswerBracket->Text);
  SelectPage(tabrace);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnpracticeClick(TObject *Sender)
{
  Challenge.Practice = true;
  Challenge.HeadsUp = true;
  Challenge.withRacer = true;
  SelectPage(tabrace);
}
//---------------------------------------------------------------------------


void __fastcall Tmain::SpeedUpClick(TObject *Sender)
{
  if(pages->ActivePage==tabrace) {
    Game.incspeed();
	RaceFlash.SetSpeedNum(Game.CarLogic.speednum);
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::SpeedDownClick(TObject *Sender)
{
  if(pages->ActivePage==tabrace) {
    Game.decspeed();
    RaceFlash.SetSpeedNum(Game.CarLogic.speednum);
  }

}
//---------------------------------------------------------------------------

void __fastcall Tmain::ChatSelectRoom1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
//  rooms->testingAutoPass = true; // autopass
  press(btnchat);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::ChatCreateRoom1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
  rooms->Tag = 1;
  press(btnchat);
}
//---------------------------------------------------------------------------




void __fastcall Tmain::Chatchat1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
  rooms->autoselect->Enabled = true;
  press(btnchat);
  for(int i=0;i<4;i++) {
    mess->Text = "Test-Message one";
    press(btnsend);
    mess->Text = "Test-Message two";
    press(btnsend);
    mess->Text = "Test-Message three";
    press(btnsend);
  }

}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnaccountClick(TObject *Sender)
{
  SelectPage(tabaccount);        
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnchangepassClick(TObject *Sender)
{
  if(edcurpass->Text.Trim()=="") return;
  if(ednewpass->Text!=ednewpassconfirm->Text) {
     throw Exception("Password entries do not match.");
  }
  Server.ChangePass(edcurpass->Text,ednewpass->Text);
  message->ShowMessage("Password was changed.");
  ednewpass->Text = "";
  ednewpassconfirm->Text = "";
  edcurpass->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Accountcheck1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
  press(btnaccount);
}
//---------------------------------------------------------------------------


void __fastcall Tmain::btncomputeroneClick(TObject *Sender)
{
  Challenge.forMoney = true;
  // harder - lower bracket difference
  Game.OppLogic.BracketTime = 14.5;
  // harder - faster car
  Game.OppLogic.AI.maxthrottle = 0.19 - (float)random(10)/1000;
  Challenge.Wager = 100.0;
  SelectPage(tabsetbracket);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnWagerClick(TObject *Sender)
{
  Challenge.Wager = StrToInt(edWager->Text);
  if(Challenge.Wager<=0)
	throw Exception("Wager must be bigger than zero.");
  if(Challenge.Wager>StrToInt(lbAccount->Caption))
	throw Exception("You don't have enough money.");
  Challenge.forMoney = true;
  SelectPage(tabrace);

}
//---------------------------------------------------------------------------

void __fastcall Tmain::Create1Click(TObject *Sender)
{
/*  GameTimer->Interval = 1;
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
  press(btnracetrack);
  press(btnOnlineChallenge);
  racers->Row=0;
  while(racers->Cells[1][racers->Row]!="another")
	racers->Row++;
  press(btnselectracer);
  press(btnBracket);
  edWager->Text = 1;
  press(btnWager);
  Game.Testing.CarAutoDrive = true;
  Game.Testing.NoStaging = true;*/

}
//---------------------------------------------------------------------------


void __fastcall Tmain::btnHeadsUpClick(TObject *Sender)
{
  Challenge.HeadsUp = true;
  Challenge.SetBracketTime(S_0);
  SelectPage(tabwagerselect);

}
//---------------------------------------------------------------------------

void __fastcall Tmain::Createchallengebuilderanotherheadsupwager11Click(
      TObject *Sender)
{
/*  GameTimer->Interval = 1;
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
  press(btnracetrack);
  press(btnOnlineChallenge);
  racerssearch->Row=0;
  while(racerssearch->Cells[1][racerssearch->Row]!="another")
	racerssearch->Row++;
  press(btnselectracer);
  press(btnHeadsUp);
  edWager->Text = 3;
  press(btnWager);
  Game.Testing.CarAutoDrive = true;
  Game.Testing.NoStaging = true;*/

}
//---------------------------------------------------------------------------

void __fastcall Tmain::ChatPMS1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
  rooms->autoselect->Enabled = true;
  press(btnchat,3000);
  inroom->Row = 1;
  inroom->OnDblClick(0);

}
//---------------------------------------------------------------------------

void __fastcall Tmain::Chatcolors1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
  rooms->autoselect->Enabled = true;
  press(btnchat);
  for(int i=1;i<1000;i++) {
    Application->ProcessMessages();
    Sleep(1);
  }

  sRecieveMessage msg;
  msg.reset();
  msg.priv = false;
  msg.text = "message text";
  msg.nick = "freemember";
  msg.admin = false;
  msg.moder = false;
  msg.female = false;
  msg.member = false;
  OnChatMessage((TObject*)&msg);

  msg.nick = "payingmember";
  msg.admin = false;
  msg.moder = false;
  msg.female = false;
  msg.member = true;
  OnChatMessage((TObject*)&msg);

  msg.nick = "payingmember-female";
  msg.admin = false;
  msg.moder = false;
  msg.female = true;
  msg.member = true;
  OnChatMessage((TObject*)&msg);

  msg.nick = "nonmembermoder";
  msg.admin = false;
  msg.moder = true;
  msg.female = false;
  msg.member = false;
  OnChatMessage((TObject*)&msg);

  msg.nick = "nonmembermoder-female";
  msg.admin = false;
  msg.moder = true;
  msg.female = true;
  msg.member = false;
  OnChatMessage((TObject*)&msg);

  msg.nick = "membermoder";
  msg.admin = false;
  msg.moder = true;
  msg.female = false;
  msg.member = true;
  OnChatMessage((TObject*)&msg);

  msg.nick = "membermoderfemale";
  msg.admin = false;
  msg.moder = true;
  msg.female = true;
  msg.member = true;
  OnChatMessage((TObject*)&msg);

  msg.nick = "admin";
  msg.admin = true;
  msg.moder = true;
  msg.female = false;
  msg.member = true;
  OnChatMessage((TObject*)&msg);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::inroomDrawCell(TObject *Sender, int ACol, int ARow,
      TRect &Rect, TGridDrawState State)
{
  bool noprivates = inroom->Cells[5][ARow]==S_1;
  inroom->Canvas->Font = this->Font;
  if(State.Contains(gdSelected)) {
   TRect srcrect,drect;
     srcrect = TRect(0,0,chatover->Width,chatover->Height);
     drect.Left = Rect.Left-1;
     drect.Top = Rect.Bottom-chatover->Height;
     drect.right = drect.left + chatover->Width;
     drect.Bottom = drect.top + chatover->Height;

     inroom->Canvas->Brush->Color = noprivates?inroom->Color:GridSelectColor;
     inroom->Canvas->FillRect(Rect);
  }
  else {
     inroom->Canvas->Brush->Color = clBlack;
     inroom->Canvas->FillRect(Rect);
  }


  bool Empty = inroom->Cells[2][ARow]=="";
  // since graphics not ready
  int TypeFlags = StrToIntDef(inroom->Cells[2][ARow],0);
  bool supermod = TypeFlags & 32;
  bool admin = TypeFlags & 16;
  bool moder = TypeFlags & 8;
  bool member = TypeFlags & 4;
  bool male =  TypeFlags & 2;
  bool female = TypeFlags & 1;

  if(ACol==1) {
   TColor nickcolor = clWhite;

   nickcolor = chatcolor("nick",admin,moder,supermod,member,female);

   if(inroom->Cells[4][ARow]==S_1)
     inroom->Canvas->Font->Style =
       TFontStyles() << fsItalic << fsBold;
   else
     inroom->Canvas->Font->Style =
       TFontStyles() << fsBold;


   inroom->Canvas->Font->Color = nickcolor;
//   SetBkMode(inroom->Canvas->Handle,TRANSPARENT);
   inroom->Canvas->TextOut(Rect.Left+2,Rect.Top+2,encoder.decode(inroom->Cells[1][ARow]));

   TRect srcrect,drect;
   srcrect.Left = 0;
   srcrect.Top = 0;
   srcrect.Right = chatline->Width;
   srcrect.Bottom = chatline->Height;
   drect.Left = Rect.Left-1;
   drect.Top = Rect.Bottom-chatline->Height;
   drect.right = drect.left + chatline->Width;
   drect.Bottom = drect.top + chatline->Height;
   if(!Empty)
    inroom->Canvas->CopyRect(drect,
      chatline->Canvas,srcrect);

  }

  if(ACol==0) {
     TRect srcrect,drect;
     TImage* im = chatuser;
     if(member)
       im=chatmember;
	 if(moder)
	   im=chatmoder;
	 if(admin)
	   im=chatadmin;

	 if(moder && female)
       im = chatfmod;

	 srcrect.Left = 0;
     srcrect.Top = 0;
     srcrect.Right = im->Width;
     srcrect.Bottom = im->Height;
     drect.Left = Rect.left+1;
     drect.Top = Rect.top;
     drect.right = drect.left + im->Width;
     drect.Bottom = drect.top + im->Height;
     if(!Empty)
     inroom->Canvas->CopyRect(drect,
       im->Canvas,srcrect
     );


  }
}
//---------------------------------------------------------------------------


void __fastcall Tmain::registerbackClick(TObject *Sender)
{
  SelectPage(tabtos);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::ModeratorToolsClick(TObject *Sender)
{
  if(Server.moder) {
    modtools->nick->Text = "";
    modtools->ShowModal();
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Moderatorwindowshow1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
//  rooms->autoselect->Enabled = true;
//  press(btnchat);
  ModeratorToolsClick(0);

}
//---------------------------------------------------------------------------


void __fastcall Tmain::Loginanothernonmoder1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "another";
  edpass->Text = "another";
  press(btnlogin);
//  rooms->autoselect->Enabled = true;
//  press(btnchat);

}
//---------------------------------------------------------------------------

void __fastcall Tmain::chatkickClick(TObject *Sender)
{
  modtools->nick->Text = GridCell(inroom,1);
  modtools->cbaction->ItemIndex = 0;
  modtools->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall Tmain::chatkickbanClick(TObject *Sender)
{
  modtools->nick->Text = encoder.decode(inroom->Cells[1][inroom->Row]);
  modtools->cbaction->ItemIndex = 1;
  modtools->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall Tmain::chatkickgamebanClick(TObject *Sender)
{
  modtools->nick->Text = encoder.decode(inroom->Cells[1][inroom->Row]);
  modtools->cbaction->ItemIndex = 2;
  modtools->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Createfoulchallengefrombuildertoanother1Click(
      TObject *Sender)
{
/*  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
  press(btnracetrack);
  press(btnOnlineChallenge);
  racers->Row=0;
  while(racers->Cells[1][racers->Row]!="another")
	racers->Row++;
  press(btnselectracer);
  press(btnBracket);
  press(btnForFun);
  GameTimer->Interval = 1;
  Game.Testing.CarAutoDrive = true;
  Game.Testing.NoStaging = true;
  Game.CarLogic.AI.dofalsestart = true;*/

}
//---------------------------------------------------------------------------

void __fastcall Tmain::Chatenter1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = ini->ReadString("Connection","login","builder");
  edpass->Text = ini->ReadString("Connection","login","builder");
  press(btnlogin);
  rooms->autoselect->Enabled = true;
  press(btnchat);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Chatenterandflood1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = ini->ReadString("Connection","login","builder");
  edpass->Text = ini->ReadString("Connection","login","builder");
  press(btnlogin);
  rooms->autoselect->Enabled = true;
  press(btnchat);

  TestFloodTimer->Enabled = true;


}
//---------------------------------------------------------------------------

void __fastcall Tmain::TestFloodTimerTimer(TObject *Sender)
{
  static int i = 0;
  AnsiString s = "Test-Message" + IntToStr(i++);
  for(int j=0;j<40;j++) {
   s += char(random(255)+1);
  }
  mess->Text = s;
  press(btnsend);

  if(pages->ActivePage!=tabchat) TestFloodTimer->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::appMessage(tagMSG &Msg, bool &Handled)
{
  if(Msg.message == WM_RBUTTONDOWN) {
/*    if(Msg.hwnd == flselected->Handle)
      Handled = true;*/
    if(pages->ActivePage!=tabchat)
      Handled = true;
  }
}
//---------------------------------------------------------------------------


void __fastcall Tmain::OnServerSystemMessages(TObject *Sender)
{
  cStringTable tbl(Server.chat->SysMessages);

  if(tbl.GetLine()) {
    AnsiString count = tbl++; // incoming challenges
    if(count==S_0) {
      lbchallenges->Caption = "no";
      ChallengeBlink->Enabled = false;
      ChallengeBlink->OnTimer(NULL);
    } else {
      lbchallenges->Caption = count;
      ChallengeBlink->Enabled = true;
    }
    tbl++; //team join requests
    AnsiString kicktologinmessage = tbl++; //kick to login message
    tbl++; //active team challenges
    if(kicktologinmessage!="") {
      if(pages->ActivePage!=tablogin) {
        message->ShowMessage(kicktologinmessage);
        SelectPage(tablogin);
      }
    }
  }

}
//---------------------------------------------------------------------------


void __fastcall Tmain::ChallengeBlinkTimer(TObject *Sender)
{
  static bool visible = false;
  visible = !visible;
  if(Sender==NULL) {
      ChallengesHighlight->Visible = false;
      lbchallenges->Visible = true;
  } else {
    ChallengesHighlight->Visible = visible;
    lbchallenges->Visible = !visible;
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Startchallengeandfoul1Click(TObject *Sender)
{
/*  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
  press(btnracetrack);
  press(btnOnlineChallenge);
  racers->Row=0;
  while(racers->Cells[1][racers->Row]!="another")
	racers->Row++;
  press(btnselectracer);
  press(btnHeadsUp);
  press(btnForFun);
  Game.Testing.CarAutoDrive = true;
  Game.Testing.NoStaging = true;
  Game.CarLogic.AI.dofalsestart = true;

  FoulQuitRaceTimer->Enabled = true;*/

}
//---------------------------------------------------------------------------

void __fastcall Tmain::FoulQuitRaceTimerTimer(TObject *Sender)
{
  // for testing only
  ((TTimer*)Sender)->Enabled=false;
  press(btnracetrack);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Checkingprotection1Click(TObject *Sender)
{
  AnsiString res = Server.Query("whothis.php","it'sme");
  message->ShowMessage(res);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Race1Click(TObject *Sender)
{
  AnsiString rt=InputBox("Reaction time","Reaction time:","0.5");
  AnsiString mt=InputBox("Max throttle","Max throttle:","0.7");
  Game.CarLogic.AI.RandomReactionTime = StrToFloat(rt);
  Game.CarLogic.AI.maxthrottle = StrToFloat(mt);
  // 1 millisecond
  GameTimer->Interval = 1;
  Game.Testing.CarAutoDrive = true;
  Game.Testing.NoStaging = true;
  Game.CarLogic.AI.dofalsestart = Sender==falsestart;
//  Game.CarLogic.AI.RandomReactionTime =   0.001*random(1000);

}
//---------------------------------------------------------------------------

void __fastcall Tmain::Racefalsestart1Click(TObject *Sender)
{
  GameTimer->Interval = 1;
  Game.Testing.CarAutoDrive = true;
  Game.Testing.NoStaging = true;
  Game.CarLogic.AI.maxthrottle = 1;
  Game.CarLogic.AI.dofalsestart = true;

}
//---------------------------------------------------------------------------

AnsiString FixSlashes(AnsiString s) {
  return AnsiReplaceStr(s,"\\'","'");
}
void Tmain::FillChallenges()
{
    {
	  int saverow = incoming_grid->Row;
	  cStringTable tbl(Challenge.RequestIncoming());
	  cGridFiller gridliner(incoming_grid,true);
	  while(tbl.GetLine()) {
		gridliner[0] = tbl++;//challengeid
		gridliner[6] = tbl++;//userid
		gridliner[1] = tbl++;//fromusernick
		if(tbl++==S_1) //forfun
		  gridliner[3] = "For Fun";
		if(tbl++==S_1) // bracket
		  gridliner[2] = "Bracket";
		bool Answered = (tbl++==S_1); // answered
		if(Answered) {
		  if(tbl++==S_1) // answered win
			gridliner[4] = "You win";
		  else
			gridliner[4] = "You lose";
		} else {
		  tbl++; //answerwin
		  gridliner[4] = "Waiting";
		}
		bool ForMoney= (tbl++==S_1);// for money
		AnsiString wager=tbl++; // wager
		if(ForMoney)
		  gridliner[3] = wager;
		bool HeadsUp = tbl++==S_1; //headsup
		if(HeadsUp)
		  gridliner[2] = "Heads Up";
		if(tbl++==S_1) // DRAW
		  gridliner[4] = "Draw";
		int HoursLeft = StrToInt(tbl++)/(60*60);
		if(Signature("Waiting",gridliner[4].val()))
		  gridliner[4] = "Waiting " + IntToStr(HoursLeft) + " hour(s) left";
		gridliner[7] = tbl++; //usercarid
		gridliner[8] = tbl++; //answerusercarid
		if(tbl++==S_1) //forcar
		  gridliner[3] = "Pink Slip";
		gridliner[9] = tbl++; //member car
		AnsiString message = tbl++;
		gridliner[5] = FixSlashes(message);
		Trace(message);
		gridliner.nextrow();
	  }
	}

	incoming_grid->OnClick(0);
	{
	  cStringTable tbl(Challenge.RequestOutgoing());
      cGridFiller gridliner(outgoing,true);
      while(tbl.GetLine()) {
        gridliner[0]=tbl++;//challengid
        tbl++; //touserid
        gridliner[1] = tbl++; // tousernick
        if(tbl++==S_1) //forfun
          gridliner[3] = "For Fun";
        if(tbl++==S_1) // bracket
          gridliner[2] = "Bracket";
        bool Answered = (tbl++==S_1); // answered
        if(Answered) {
          if(tbl++==S_1) // answered win
			gridliner[4] = "You lose";
          else
			gridliner[4] = "You win";
		} else {
		  tbl++; //answerwin
		  gridliner[4] = "Waiting";
		}

		bool ForMoney=(tbl++==S_1);// for money
		AnsiString wager=tbl++; // wager
		if(ForMoney)
		  gridliner[3] = wager;
		bool HeadsUp = tbl++==S_1; //headsup
		if(HeadsUp)
		  gridliner[2] = "Heads Up";
		if(tbl++==S_1) // DRAW
		  gridliner[4] = "Draw";
		int HoursLeft = StrToInt(tbl++)/(60*60);
		if(Signature("Waiting",gridliner[4].val()))
		  gridliner[4] = "Waiting " + IntToStr(HoursLeft) + " hour(s) left";
		if(tbl++==S_1) // for car
		  gridliner[3] = "Pink Slip";
		AnsiString message = tbl++;
		gridliner[5] = FixSlashes(message);
		Trace(message);
		gridliner.nextrow();
	  }
	}

	outgoing->OnClick(0);

  incoming_grid->FixedRows = 0;
  for(int i=0;i<outgoing->ColCount;i++)
	outgoing->ColWidths[i] = incoming_grid->ColWidths[i];
  outgoing->Left = incoming_grid->Left;
  outgoing->Top = incoming_grid->Top;
  outgoing->Width = incoming_grid->Width;

}
void __fastcall Tmain::Racerssearch1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
  press(btnracetrack);
//  press(btnOnlineChallenge);

}
//---------------------------------------------------------------------------

void __fastcall Tmain::edracersearchChange(TObject *Sender)
{
  racerssearch->RowCount = 0;
  for(int j=0;j<racerssearch->ColCount;j++)
      racerssearch->Cells[j][0]="";
  int entries = 0;
  for(int i=0;i<racers->RowCount;i++) {
    if(racers->Cells[1][i].LowerCase().Pos(edracersearch->Text.LowerCase())!=0 || edracersearch->Text=="") {
       for(int j=0;j<racers->ColCount;j++)
         racerssearch->Cells[j][racerssearch->RowCount-1]=
           racers->Cells[j][i];
       racerssearch->RowCount++;
       entries ++;
    }
  }
  racerssearch->RowCount = entries;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::edracersearchKeyPress(TObject *Sender, char &Key)
{
//  if(Key==13) {
//    if(racerssearch->RowCount==1)
//      racerssearch->OnDblClick(0);
//  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::incomingresultsClick(TObject *Sender)
{
  AnsiString ChallengeID = incoming_grid->Cells[0][incoming_grid->Row];
  Challenge.GetInfo(ChallengeID);
  if(Challenge.Answered) {
    AnsiString s;
    s.sprintf(
	  "Opponent: %s\n%s",
	  incoming_grid->Cells[1][incoming_grid->Row],
	  Challenge.Opponent.ResultStr(Challenge.Bracket)
	);
	opponentresult_lbl->Caption = s;

	s.sprintf("Your result:\n%s",Challenge.Car.ResultStr(Challenge.Bracket));
	yourresult_lbl->Caption = s;
  }


}
//---------------------------------------------------------------------------

void __fastcall Tmain::incoming_gridClick(TObject *Sender)
{
  opponentresult_lbl->Caption = "";
  yourresult_lbl->Caption = "";

  bool completed = !Signature("Waiting",incoming_grid->Cells[4][incoming_grid->Row]);
  // AUTO PRESS INCOMING RESULTS (SET BUTTON INVISIBLE)
  AnsiString ChallengeID = GridCell(incoming_grid,0);
  bool Empty = ChallengeID.Length()==0;
  if(completed && !incomingresults->Visible)
    incomingresults->OnClick(0);
  resultsbutton->Visible = completed && !Empty;
  btnDeclineChallenge->Enabled = !completed;
  btnAnswerChallenge->Enabled = !completed;
  hideresultsbutton->Visible = completed && !Empty;
  challengebutton->Visible =  !Empty && !completed;
}

//---------------------------------------------------------------------------

void __fastcall Tmain::outgoingresultsClick(TObject *Sender)
{
  AnsiString ChallengeID = outgoing->Cells[0][outgoing->Row];
  Challenge.GetInfo(ChallengeID);
  if(Challenge.Answered) {
    AnsiString s;

	s.sprintf("You:\n%s",
	Challenge.Opponent.ResultStr(Challenge.Bracket));
	outgoingresult_lbl->Caption = s;

	s.sprintf(
	  "Opponent: %s\n%s",
	  outgoing->Cells[1][outgoing->Row],
	  Challenge.Car.ResultStr(Challenge.Bracket)
	  );
	outgoingresult2_lbl->Caption = s;

  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::outgoingClick(TObject *Sender)
{
  outgoingresult_lbl->Caption = "";
  outgoingresult2_lbl->Caption = "";
  bool Completed = !Signature("Waiting",outgoing->Cells[4][outgoing->Row]);
  AnsiString ChallengeID = outgoing->Cells[0][outgoing->Row];
  bool Empty = ChallengeID.Length()==0;
  if(Completed && !outgoingresults->Visible)
	outgoingresults->OnClick(0);

  outgoingresults_button->Enabled = Completed && !Empty;
  hideoutgoing_button->Enabled = Completed && !Empty;
}
//---------------------------------------------------------------------------






void __fastcall Tmain::inroomMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(Button==mbRight) {
     int col;
     int row;
     inroom->MouseToCell(X,Y,col,row);
     if(row<inroom->RowCount && row>=0)
       inroom->Row = row;
     if(Server.moder)
       ModerMenu->Popup(Mouse->CursorPos.x,Mouse->CursorPos.y);
  }
      
}
//---------------------------------------------------------------------------

void __fastcall Tmain::edaccnickChange(TObject *Sender)
{
  edaccnick->Text = killbadchars(edaccnick->Text);         
}
//---------------------------------------------------------------------------


void __fastcall Tmain::chatMouseWheel(TObject *Sender, TShiftState Shift,
      int WheelDelta, TPoint &MousePos, bool &Handled)
{
  if(Shift.Contains(ssLeft)) Handled = true;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnteamClick(TObject *Sender)
{
  SelectPage(tabteam);
}
//---------------------------------------------------------------------------
void Tmain::FillTeams(AnsiString part) {
   cStringTable tbl(Team.TeamList(part));
   if(Server.Stack.Stacking) return;
   cGridFiller gridliner(teamlist,true);
   while(tbl.GetLine()) {
      gridliner[0] = tbl++;//teamid
      gridliner[1] = tbl++;//teamname
      gridliner.nextrow();
   }
}

void __fastcall Tmain::btncreateteamClick(TObject *Sender)
{
  AnsiString name=input->InputBox("Team name","Enter new team name:","");
  if(name.Trim().Length()==0)
    return;
  if(name!=killbadchars(name))
    throw Exception("Use simple symbols.");
  Team.CreateTeam(name);

  if(Team.teamid!="") {
    tabteam->OnShow(0);
  }

}
//---------------------------------------------------------------------------

void __fastcall Tmain::Tabteam1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
  press(btnteam);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::edteamsearchChange(TObject *Sender)
{
  TStringGrid* g;
  AnsiString text;
  if(Sender==race_edteamsearch) {
    g = race_teamsearch;
    text = race_edteamsearch->Text.LowerCase();
    text = "";
  } else if(Sender==edteamsearch)  {
    g = teamsearch;
    text = edteamsearch->Text.LowerCase();
    text = "";
  }
  g->RowCount = 0;
  for(int j=0;j<g->ColCount;j++)
      g->Cells[j][0]="";
  int entries = 0;
  for(int i=0;i<teamlist->RowCount;i++) {
    if(text=="" || teamlist->Cells[1][i].LowerCase().Pos(text)!=0) {
       for(int j=0;j<teamlist->ColCount;j++)
         g->Cells[j][g->RowCount-1]=
           teamlist->Cells[j][i];
       g->RowCount++;
       entries ++;
    }
  }
  g->RowCount = entries;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::tabteamShow(TObject *Sender)
{
  Screen->Cursor = crAppStart;
  Application->ProcessMessages();
  pannoteam->Left = 0;
  pannoteam->Top = 0;

//  teampages->Top = 72;
//  teampages->Left = 6;
  Server.StartStack();
  while(Server.StackQueries()) {
    Team.GetUserTeam(Server.userid);
	if(Server.Stack.Stacking || !Team.inteam) {
//	  teampages->Visible = false;
      teammanage_panel->Visible = false;
	  edteamsearch->Text = "";
	  FillTeams("");
	  edteamsearchChange(edteamsearch);
	  FillUserRequests();
	}
}

  if(Team.inteam) {
	teammanage_panel->Parent = tabteam;
	teammanage_panel->BringToFront();
    toppanel->BringToFront();
	teammanage_panel->Visible = true;
//	pages->ActivePage = tabteammanage;
//	teampages->Visible = true;
	lbTeamName->Caption = "Your team: "+Team.teamname;
	team_nbPageChanged(0);
/*	if(teampages->ActivePage->OnShow!=0)
	  teampages->ActivePage->OnShow(0);*/
	btnteamchallenge->Enabled = Team.owner;
	lbteamleaderhelp->Visible = !Team.owner;
  } else {
     pannoteam->Visible = true;
  }

  pannoteam->Visible = !Team.inteam;
  btndistributefunds->Enabled = Team.owner;

  cGridFiller clear(teammembers);
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void Tmain::FillUserRequests() {
  {
     cStringTable tbl(Team.UserRequests());
     if(Server.Stack.Stacking) return;
     cGridFiller gridliner(userjoinrequests,true);
     while(tbl.GetLine()) {
        gridliner[0] = tbl++;//request id
        gridliner[1] = tbl++;//teamname
        gridliner[2] = "pending";
        if(tbl++==S_1)
          gridliner[2] = "accepted";
        if(tbl++==S_1)
          gridliner[2] = "declined";
        gridliner.nextrow();
     }
  }
  userjoinrequests->OnClick(0);
}

void Tmain::FillTeamRequests() {
  {
	 AnsiString requests = Team.TeamRequests();
//	 TRACE(requests);
	 cStringTable tbl(requests);
	 if(Server.Stack.Stacking) return;
     cGridFiller gridliner(teamjoinrequests,true);
     while(tbl.GetLine()) {
        gridliner[0] = tbl++;//request id
        gridliner[1] = tbl++;//nick
        gridliner.nextrow();
     }
  }
  teamjoinrequests->OnClick(0);
}

void __fastcall Tmain::btnquitteamClick(TObject *Sender)
{
  if(request->confirm("Quit this team?")) {
    Team.QuitTeam();
    tabteam->OnShow(0);
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnjointeamClick(TObject *Sender)
{
  if(request->confirm("Join this team?\r\n All other requests will be removed.\r\n You will join with your current selected car.")) {
    AnsiString id = userjoinrequests->Cells[0][userjoinrequests->Row];
    if(id.Length()!=0) {
      Team.JoinAccepted(id);
      tabteam->OnShow(0);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::teamsearchDblClick(TObject *Sender)
{
   Screen->Cursor = crAppStart;
   TStringGrid* s; // source
   TStringGrid* d; // destination
   TLabel* teamstats; // teamstatistics
   s = teamsearch;
   d = teammembers;
   teamstats = lbjoin_teamstats;
   if(Sender==race_teamsearch) {
     s = race_teamsearch;
     d = race_teammembers;
     teamstats = lbrace_teamstats;
   }

   AnsiString teamid = s->Cells[0][s->Row];
   if(teamid.Length()==0) {
     return;
   }
   Server.StartStack();
   while(Server.StackQueries()) {
      Team.GetStats(teamid);
      teamstats->Caption =
        teamstats->Caption.sprintf("%s win(s) %s lose(s)",Team.wins,Team.loses);
     cStringTable tbl(Team.TeamMembers(teamid));
     cGridFiller gridliner(d,true);
     if(!Server.Stack.Stacking)
     while(tbl.GetLine()) {
        gridliner[0] = tbl++;//userid
        gridliner[1] = tbl++;//usernick
        // owner
        if(d->ColCount>=5) {
          gridliner[4] = tbl++=='1' ? "leader" : "";
          gridliner[3] = tbl++; // carid
          gridliner[2] = tbl++; // car name
        }
        gridliner.nextrow();
     }
   }

   Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------


void __fastcall Tmain::userjoinrequestsClick(TObject *Sender)
{
  if(!Team.inteam) {
    bool Accepted = userjoinrequests->Cells[2][userjoinrequests->Row]=="accepted";
    bool notempty = GridCell(userjoinrequests,0).Length()!=0;
    btnjointeam->Enabled = Accepted && notempty;
  }

}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnacceptjoinClick(TObject *Sender)
{
  AnsiString id = teamjoinrequests->Cells[0][teamjoinrequests->Row];
  if(id.Length()!=0)
  if(request->confirm("Accept join request?")) {
      Team.AcceptJoin(id);
      FillTeamRequests();
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btndeclinejoinClick(TObject *Sender)
{
  AnsiString id = teamjoinrequests->Cells[0][teamjoinrequests->Row];
  if(id.Length()!=0)
  if(request->confirm("Decline join request?")) {
      Team.DeclineJoin(id);
      FillTeamRequests();
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnapplytojoinClick(TObject *Sender)
{
  if(request->confirm("Apply to join this team?")) {
    AnsiString teamid = teamsearch->Cells[0][teamsearch->Row];
    if(teamid.Length()!=0) {
      Server.StartStack();
      while(Server.StackQueries()) {
        Team.RequestJoin(teamid);
        FillUserRequests();
      }
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall Tmain::teamteamShow(TObject *Sender)
{
  // team members

Screen->Cursor = crAppStart;
Application->ProcessMessages();

Server.StartStack();
while(Server.StackQueries()) {
  Team.GetStats(Team.teamid);
  lbteamstats->Caption =
    lbteamstats->Caption.sprintf("%s win(s) %s lose(s)",Team.wins,Team.loses);

  cStringTable tbl(Team.TeamMembers(Team.teamid));
  if(!Server.Stack.Stacking)
  {
   cGridFiller gridliner(teamteammembers,true);
   while(tbl.GetLine()) {
      gridliner[0] = tbl++;//userid
      gridliner[1] = tbl++;//usernick
      // owner
      gridliner[4] = tbl++=='1' ? "leader" : "";
      gridliner[3] = tbl++; // carid
      gridliner[2] = tbl++; // usercarname
      gridliner.nextrow();
   }
  }

   if(Sender!=0) {
     FillTeamRequests();
     btnremovemember->Enabled = Team.owner;
   }
}

  teamteammembers->OnClick(0);
Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------


void __fastcall Tmain::teamraceShow(TObject *Sender)
{
Screen->Cursor = crAppStart;
Application->ProcessMessages();
  this->FillTeams(race_edteamsearch->Text);
  this->edteamsearchChange(race_edteamsearch);
  cGridFiller clear(race_teammembers);
Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnremovememberClick(TObject *Sender)
{
  AnsiString userid = teamteammembers->Cells[0][teamteammembers->Row];
  if(request->confirm("Remove this member?")) {
    if(userid.Length()==0) return;
    Team.RemoveMember(userid);
    teamteamShow(0);
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::teamteammembersClick(TObject *Sender)
{
  if(!Team.owner) return;
  AnsiString tableid = teamteammembers->Cells[0][teamteammembers->Row];
  btnremovemember->Enabled = (Team.s->userid!=tableid);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::teamfundsShow(TObject *Sender)
{
  Screen->Cursor = crAppStart;
  Application->ProcessMessages();
Server.StartStack();
while(Server.StackQueries()) {

  if(Sender==btndistributefunds) {
    AnsiString userid=
      distributefunds->members->Cells[0][distributefunds->members->Row];
	if(userid.Length()==0) break;
	assert(StrToIntDef(encoder.decode(Team.BalanceEnc),0)>StrToIntDef(distributefunds->edAmount->Text,0));
	Team.Distribute(userid,distributefunds->edAmount->Text);
  }

  if(Sender==btndeposit) {
    Team.Deposit(IntToStr(btndeposit->Tag));
  }

  lbteammoney->Caption = Team.GetBalance();
  cStringTable tbl(Team.GetTransactions());
  if(!Server.Stack.Stacking) {
     cGridFiller gridliner(teammoney,true);
     while(tbl.GetLine()) {
        AnsiString nick = tbl++;//user nick
        int value = StrToInt(tbl++);//value
        AnsiString team = tbl++;//team name
        AnsiString teamid = tbl++;//team id


		if(nick!="")
			gridliner[1] = value>0 ? "Deposit by "+nick : "Disbursment to "+nick;

		if(teamid!=S_0) {
			if(team=="")
			  team = "-deleted team-";
			gridliner[1] = value>0 ? "Win ["+team+"]" : "Lose ["+team+"]";
		}


		gridliner[2] = IntToStr(value);

		if(tbl.fieldscount>=4) {
		  AnsiString comment = tbl++;
		  if(gridliner[1].val() =="")
			gridliner[1] = comment;
		}
		gridliner.nextrow();
	 }
  }
}
  Screen->Cursor = crDefault;

}
//---------------------------------------------------------------------------

void __fastcall Tmain::btndepositClick(TObject *Sender)
{
  Screen->Cursor = crAppStart;
  Application->ProcessMessages();
  AnsiString your=Account.GetBalance();
  Screen->Cursor = crDefault;
  AnsiString value=input->InputBox("Deposit funds","You account - "+your+". Enter value:","");

  if(value.Trim()=="") return;
  int intvalue = StrToInt(value);
  if(intvalue>StrToInt(your))
	throw Exception("You don't have enough money.");
  if(value<=0)
    throw Exception("Enter positive value.");

  btndeposit->Tag = StrToInt(value);
  teamfundsShow(btndeposit);
//  teamfunds->OnShow(btndeposit);

}
//---------------------------------------------------------------------------

void __fastcall Tmain::btndistributefundsClick(TObject *Sender)
{
  Screen->Cursor = crAppStart;
  Application->ProcessMessages();
  {
   cStringTable tbl(Team.TeamMembers(Team.teamid));
   cGridFiller gridliner(distributefunds->members,true);
   while(tbl.GetLine()) {
      gridliner[0] = tbl++;//userid
      gridliner[1] = tbl++;//usernick
      // owner
      gridliner[2] = tbl++=='1' ? "leader" : "";
      gridliner.nextrow();
   }
  }
  Screen->Cursor = crDefault;
  distributefunds->Color = this->Color;
  distributefunds->TeamAccount = StrToIntDef(encoder.decode(Team.BalanceEnc),0);
  if(distributefunds->ShowModal()==mrOk) {
	teamfundsShow(btndistributefunds);
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnteamchallengeClick(TObject *Sender)
{
  AnsiString o_teamid = race_teamsearch->Cells[0][race_teamsearch->Row];
  if(o_teamid.Length()==0) return;
  Server.StartStack();

  while(Server.StackQueries()) {
    createchallenge->yourteam = Team.TeamMembers(Team.teamid);
    createchallenge->otherteam = Team.TeamMembers(o_teamid);
    createchallenge->teamaccount = Team.GetBalance();
  }

  createchallenge->Color = this->Color;
  if(createchallenge->ShowModal()==mrOk) {
    TeamChallenge.Bracket = createchallenge->bracket->Checked;
    TeamChallenge.HeadsUp = createchallenge->headsup->Checked;
    TeamChallenge.ForFun = createchallenge->forfun->Checked;
    TeamChallenge.ForMoney = createchallenge->formoney->Checked;
    TeamChallenge.Wager = StrToIntDef(createchallenge->edWager->Text,0);
    TeamChallenge.teamid = Team.teamid;
    TeamChallenge.o_teamid = o_teamid;
    TeamChallenge.challengers = createchallenge->challengers;
    Screen->Cursor = crAppStart;
    Application->ProcessMessages();
    TeamChallenge.Create();
    Screen->Cursor = crDefault;
    message->ShowMessage("Challenge was added to outgoing team challenges.\n Other team leader should accept it or decline.");
  }
}
//---------------------------------------------------------------------------


void __fastcall Tmain::teamchallengeShow(TObject *Sender)
{
  Screen->Cursor = crAppStart;
  Application->ProcessMessages();
  teamchallenges_pages->ActivePage->OnShow(teamchallenges_pages->ActivePage);
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------


void __fastcall Tmain::team_incomingShow(TObject *Sender)
{
  teamchallenges->Parent = (TWinControl*)Sender;
  teamchallenges->Parent->DoubleBuffered = true;
  Screen->Cursor = crAppStart;
  Application->ProcessMessages();
  AnsiString type="";
  if(Sender==team_incoming)
	type="incoming";
  if(Sender==team_outgoing)
    type="outgoing";
  if(Sender==team_active)
    type="active";
  if(Sender==team_completed)
    type="completed";
  cStringTable tbl(TeamChallenge.GetList(Team.teamid,type));
  cGridFiller g(teamchallenges,true);
  while(tbl.GetLine()) {
    g[0] = tbl++; //id
    g[1] = tbl++; //teamid
    g[2] = tbl++; //teamname
    g[3] = tbl++; //headsup/bracket
    g[4] = tbl++; //for fun/wager
    if(type=="completed")
      g[5] = tbl++; //draw/win/lose
    g.nextrow();
  }
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------



void __fastcall Tmain::teamchallengesDblClick(TObject *Sender)
{
  AnsiString cid = GridCell(teamchallenges,0);
  if(cid.Length()!=0) {
    bool initiator = false;
    bool answerer = false;
    bool initOpponentRaced = false;
    bool ansOpponentRaced = false;
    AnsiString clineid = "";
    {
       FixStringGrid(challengeinfo->versus);
       challengeinfo->versus->ColWidths[2] = 210;
       challengeinfo->versus->ColWidths[5] = 210;
       challengeinfo->versus->ColWidths[3] = 60;
       challengeinfo->versus->ColWidths[6] = 60;

       challengeinfo_completed->versus->ColWidths[2] = 210;
       challengeinfo_completed->versus->ColWidths[5] = 210;
       challengeinfo_completed->versus->ColWidths[3] = 60;
       challengeinfo_completed->versus->ColWidths[6] = 60;

       cStringTable tbl(
         teamchallenges_pages->ActivePage==team_completed ?
         TeamChallenge.GetChallengers(cid,"completed") :
         TeamChallenge.GetChallengers(cid,"") );
       cGridFiller g(
         teamchallenges_pages->ActivePage==team_completed ?
           challengeinfo_completed->versus :
           challengeinfo->versus
       );
       while(tbl.GetLine()) {
          g[0] = tbl++; // challenge line id
          g[1] = tbl++; // userid
          g[2] = tbl++; // usernick
          g[3] = tbl++; // ended
          g[4] = tbl++; // o_userid
          g[5] = tbl++; // o_usernick
          g[6] = tbl++; // o_ended
          if(g[4].val()==Server.userid && g[6].like("wait")) {
            answerer = true;
            clineid = g[0].val();
            ansOpponentRaced = g[3].like("raced");
          }
          if(g[1].val()==Server.userid && g[3].like("wait")) {
            initiator = true;
            clineid = g[0].val();
            initOpponentRaced = g[6].like("raced");
          }
          if(teamchallenges_pages->ActivePage==team_completed) {
            g[7] = tbl++; // results
            g[8] = tbl++; // o_results
          }


          g.nextrow();
       }
    }
    challengeinfo->Color = this->Color;
    challengeinfo->btnaccept->Visible = false;
    challengeinfo->btndecline->Visible = false;
    challengeinfo->btnrace->Visible= false;
    challengeinfo->Reset();
	if(teamchallenges_pages->ActivePage==team_incoming) {
        challengeinfo->btnaccept->Visible = true;
        challengeinfo->btndecline->Visible = true;
        challengeinfo->btnaccept->Enabled = Team.owner;
        challengeinfo->btndecline->Enabled = Team.owner;
        challengeinfo->ShowModal();
    }
    if(teamchallenges_pages->ActivePage==team_active) {
        challengeinfo->btnrace->Visible = true;
        challengeinfo->btnrace->Enabled = answerer || initiator;
        challengeinfo->ShowModal();
    }

    if(teamchallenges_pages->ActivePage==team_completed) {
       challengeinfo_completed->Color = this->Color;
       challengeinfo_completed->Bracket = GridCellLike(teamchallenges,3,"bracket");
       challengeinfo_completed->ShowModal();
    }


      // ACCEPT - DECLINE
      if(challengeinfo->accept) {
        TeamChallenge.Answer(cid,Team.teamid,"accept");
        teamchallenges_pages->ActivePage->OnShow(teamchallenges_pages->ActivePage);
      }
      if(challengeinfo->decline) {
        TeamChallenge.Answer(cid,Team.teamid,"decline");
        teamchallenges_pages->ActivePage->OnShow(teamchallenges_pages->ActivePage);
      }
      // RACING ACTIVE
      if(challengeinfo->race) {
		Challenge.Reset();
        Challenge.withRacer = true;
        Challenge.TeamRace = true;
        if(initiator) {
          Challenge.TeamRaceInitiators = true;
		  Challenge.Answering = initOpponentRaced;
		  if(Challenge.Answering)
            Challenge.TeamRaceGetOpponent(clineid,"answer");
        }
        else
        if(answerer) {
          Challenge.TeamRaceAnswerers = true;
		  Challenge.Answering = ansOpponentRaced;
		  if(Challenge.Answering)
            Challenge.TeamRaceGetOpponent(clineid,"init");
        }
        Challenge.carid = Team.carid;
        Challenge.usercarid = Team.usercarid;
        Challenge.challengeid = clineid;
        Challenge.Bracket = GridCellLike(teamchallenges,3,"bracket");
        if(Challenge.Bracket)
          SelectPage(tabsetbracket);
        else
          SelectPage(tabrace);
      }

  }
}
//---------------------------------------------------------------------------


void __fastcall Tmain::Login2Click(TObject *Sender)
{
  SelectPage(tablogin);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Tabteammember011Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "member01";
  edpass->Text = "member01";
  press(btnlogin);
  press(btnteam);
}
//---------------------------------------------------------------------------


void __fastcall Tmain::edbracketKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if(Key==13)
    btngorace->OnClick(0);        
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btndealerClick(TObject *Sender)
{
  SelectPage(tabdealer);
}
//---------------------------------------------------------------------------


void __fastcall Tmain::tabdealerShow(TObject *Sender)
{
  ClearGrid(CarList);
  Screen->Cursor = crAppStart;
  Application->ProcessMessages();
  lbDealerAccount->Caption = Account.GetBalance();
{
  cStringTable tbl(Cars.GetList());
  cGridFiller g(CarList,true);

  while(tbl.GetLine()) {
     g[0] = tbl++; // car id
     g[1] = tbl++; // car name
     g[2] = tbl++; // car price
     g[3] = tbl++; // member car
     g[4] = tbl++; // weight lbs
     g[5] = tbl++; // horses
     g.nextrow();
  }
}

  CarList->OnClick(0);
Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::CarListClick(TObject *Sender)
{
  AnsiString carid;

  carid = GridCell(CarList,0);
  if(carid.IsEmpty()) return;

  flsetvar(fldealer,"_root.Cars", carid + "|"+GridCell(CarList,1)+"|",false);

  btnbuycar->Enabled = StrToInt(GridCell(CarList,2))<=StrToInt(lbDealerAccount->Caption);
  AnsiString s;
  s.printf("Weight: %slb HP: %s",GridCell(CarList,4),GridCell(CarList,5));
  flsetvar(fldealer,"_root.CarProps",s,false);

}
//---------------------------------------------------------------------------

void __fastcall Tmain::Dealer1Click(TObject *Sender)
{
  SelectPage(tablogin);
  Application->ProcessMessages();
  ednick->Text = "builder";
  edpass->Text = "builder";
  press(btnlogin);
  press(btndealer);
//  rooms->autoselect->Enabled = true;
//  press(btnchat);

}
//---------------------------------------------------------------------------

void __fastcall Tmain::btngarageClick(TObject *Sender)
{
  SelectPage(tabgarage);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnbuycarClick(TObject *Sender)
{
  if(GridCellLike(CarList,3,"member") && !Server.member)
    throw Exception("You don't have membership.");
  if(request->confirm("Buy this car?")) {
    Server.StartStack();
    while(Server.StackQueries()) {
      Cars.BuyCar(Server.userid,GridCell(CarList,0));
      Cars.GetFreeLeft();
      lbDealerAccount->Caption = Account.GetBalance();
    }
    message->ShowMessage("You bought the car. It was added to your garage.");
    SelectPage(tabgarage);
  }

}
//---------------------------------------------------------------------------

void __fastcall Tmain::tabgarageShow(TObject *Sender)
{

if(Sender==tabgarage)
  ClearGrid(garageGrid);

Screen->Cursor = crAppStart;
// to repaint
Application->ProcessMessages();
Server.StartStack();
while(Server.StackQueries()) {

    if(Sender==btnselectcar) {
      Cars.SelectCar(Server.userid,GridCell(garageGrid,3));
	  Cars.selectedcarid = GridCell(garageGrid,0);
	  Cars.selectedweight = StrToInt(GridCell(garageGrid,5))*POUNDKG;
	  Cars.selectedhorses = StrToInt(GridCell(garageGrid,6));
      Cars.usercarid = GridCell(garageGrid,3);
	  Cars.GetSelected();
	  Engine::SelectCar(Cars.selectedcarid);
      selectedcarhint->Hint = Cars.selectedcarname;
    }

    if(Sender==btnsellcar) {
      AnsiString usercarid=GridCell(garageGrid,3);
      Cars.SellCar(Server.userid,usercarid);
      Cars.GetFreeLeft();
	}



	lbgarageaccount->Caption = Account.GetBalance();

	cStringTable tbl(Cars.GetUserCars(Server.userid,S_0));
	if(!Server.Stack.Stacking) {

	  {
		cGridFiller g(garageGrid,true);

		while(tbl.GetLine()) {
		 g[0] = tbl++; // car id
		 g[1] = tbl++; // car name
		 g[2] = tbl++; // sell price
		 g[3] = tbl++; // user car id
		 g[4] = tbl++; // selected/garaged
		 g[5] = tbl++; // weight
		 g[6] = tbl++; // horses
		 g[7] = tbl++; // team car
		 if(tbl++==S_1) {
		   g[4] = "locked";
		 }

		 DecimalSeparator = '.';
		 g[8] = FloatToStrF(StrToFloat(tbl++),ffFixed,10,2); // average race time
		 g[9] = FloatToStrF(StrToFloat(tbl++),ffFixed,10,2); // last bracket time

		 g.nextrow();
		}
	  }

	   AnsiString cars;
	   AnsiString carcomments;
	   int Selected = 0;
	   for(int i=0;i<garageGrid->RowCount;i++) {
		cars = cars + garageGrid->Cells[0][i]+"|"+garageGrid->Cells[1][i]+"|";
		AnsiString carcomment;
		carcomment = "";
		if(StrToFloat(garageGrid->Cells[8][i])!=0) {
		  carcomment = carcomment + "ET:"+garageGrid->Cells[8][i];
		}
		if(StrToFloat(garageGrid->Cells[9][i])!=0) {
		  carcomment = carcomment+" BT:"+garageGrid->Cells[9][i];
		}
		if(!carcomment.Length()) {
			carcomment = "never used";
		}
		carcomments = carcomments + carcomment+"|";
		if(garageGrid->Cells[4][i].LowerCase()=="selected") {
		   Selected = i;
		}

	   }



	   flsetvar(flgarage,"_root.Cars",cars,false);
	   flsetvar(flgarage,"_root.CarsComments",carcomments,false);
	   flsetvar(flgarage,"_root.OldCars","",false);
	   TRACE(carcomments);
	   flsetvar(flgarage,"_root.SelectedCar",IntToStr(Selected),false);

	}

}

  garageGrid->OnClick(0);

Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::garageGridClick(TObject *Sender)
{
  AnsiString carid;

  carid = GridCell(garageGrid,0);
  if(carid.IsEmpty()) return;

  bool locked = GridCellLike(garageGrid,4,"lock");
  btnselectcar->Enabled =
	!GridCellLike(garageGrid,4,"select") && !locked;
  btnsellcar->Enabled = !locked;
  btnmodify->Enabled = !locked;
  btnmaintenance->Enabled= !locked;

  flsetvar(flmodcar,"_root.Cars",carid+"|"+GridCell(garageGrid,1)+"|",false);
  flsetvar(flmaintcar,"_root.Cars",carid+"|"+GridCell(garageGrid,1)+"|",false);

  lbgaragecarname->Caption = GridCell(garageGrid,1);
  
  AnsiString s;
  s.printf("Weight: %slb HP: %s",GridCell(garageGrid,5),GridCell(garageGrid,6));
  lbgaragecarparams->Caption = s;

}
//---------------------------------------------------------------------------


void __fastcall Tmain::btnsellcarClick(TObject *Sender)
{
  AnsiString usercarid=GridCell(garageGrid,3);
  if(usercarid.IsEmpty()) return;
  if(garageGrid->RowCount<=1)
    throw Exception("This is your last car.");
  if(GridCellLike(garageGrid,4,"select"))
    throw Exception("You can't sell selected car.");

  bool teamcar = GridCellLike(garageGrid,7,"team");
  AnsiString message = "Sell this car for "+GridCell(garageGrid,2)+"?"
     "\r\nAll incoming challenges for this car will be declined.";
  if(teamcar) message +=
     "\r\nIt is your team car if you will sell it, you will be removed from team,\r\nand all your active challenges will be recorded as foul.";
  if(request->confirm(message)) {
    tabgarage->OnShow(btnsellcar);
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::FormShow(TObject *Sender)
{
  static bool firsttime=true;
  if(firsttime) {
    SetGrButtons(this,6);
    SetGrButtons(message,6);
    SetGrButtons(request,6);
    SetGrButtons(input,6);
    SetGrButtons(distributefunds,6);
    SetGrButtons(createchallenge,6);
    SetGrButtons(gearratios,6);
    SetGrButtons(modtools,6);
    SetGrButtons(teammessage,6);
    SetGrButtons(challengeinfo,6);
    this->Top = (Screen->Height - this->Height) / 2 - 42;
    this->Left = (Screen->Width - this->Width) / 2;
    firsttime = false;
  }

  request->Color = this->Color;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::tabcarselectShow(TObject *Sender)
{
{
  AnsiString s = Cars.RegisterList();
  TRACE(s);
  cStringTable tbl(s);
  cGridFiller g(registercarlist,true);

  while(tbl.GetLine()) {
	 g[0] = tbl++; // car id
     g[1] = tbl++; // car name
     g[3] = tbl++; // weight
     g[4] = tbl++; // horses
     g.nextrow();
  }
}

  registercarlist->OnClick(0);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::registercarlistClick(TObject *Sender)
{
  AnsiString carid;

  carid = GridCell(registercarlist,0);
  if(carid.IsEmpty()) return;

  flsetvar(flcarreg,"_root.Cars", carid+"|"+GridCell(registercarlist,1)+"|",false);
  
//  car_register->Picture->Assign(Engine::RegPic(carid));
//  lbregistercarname->Caption = GridCell(registercarlist,1);
  AnsiString s;
  s.printf("Weight: %slb HP: %s",GridCell(registercarlist,3),GridCell(registercarlist,4));
//  lbregistercarparams->Caption = s;
  flsetvar(flcarreg,"_root.CarProps",s,false);


}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnselectcarClick(TObject *Sender)
{
  if(GridCellLike(garageGrid,4,"lock"))
    throw Exception("This car is locked.");
  if(request->confirm("This car will take part in challenges.")) {
    tabgarage->OnShow(btnselectcar);
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Sensor1Click(TObject *Sender)
{
  Application->ProcessMessages();
  SelectPage(tabshop);        
}
//---------------------------------------------------------------------------

void __fastcall Tmain::tabshopShow(TObject *Sender)
{
AnsiString partid = GridCell(shopparts,0);
if(Sender!=btnbuypart)
  ClearGrid(shopparts);
Screen->Cursor = crAppStart;
Application->ProcessMessages();
Server.StartStack();
while(Server.StackQueries()) {
  if(Sender==btnbuypart) {
    AnsiString usercarid = Cars.usercarid;
    Parts.BuyPart(usercarid,partid);
    // MAGIC GEAR
    if(partid==11) {
      Cars.MagicGear.g1 = 3.5;
      Cars.MagicGear.g2 = 2.2;
      Cars.MagicGear.g3 = 1.5;
      Cars.MagicGear.g4 = 1.1;
      Cars.MagicGear.g5 = 0.94;
      Cars.MagicGear.g6 = 0.763;
      Cars.SaveMagicGear(usercarid);
    }
  }
  cStringTable tbl(Parts.GetList(Cars.usercarid));
  if(!Server.Stack.Stacking) {
    cGridFiller g(shopparts,true);
    while(tbl.GetLine()) {
      g[0] = tbl++; //partid
      g[1] = tbl++; //groupname
      g[2] = tbl++; //part name
      g[3] = tbl++; //price
      g[4] = tbl++; // hp
      g[5] = tbl++; // slip
      g[6] = tbl++; // bought
      g.nextrow();
    }
  }
  lbshopaccount->Caption = Account.GetBalance();
}

  shopparts->OnClick(0);
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::shoppartsClick(TObject *Sender)
{
  AnsiString s;
  s = "Price: "+GridCell(shopparts,3);
  AnsiString hp = GridCell(shopparts,4);
  if(hp!=S_0)
    s += "\r\nAdds "+hp + " horse(s)";
  AnsiString slip = GridCell(shopparts,5);
  if(slip!=S_0)
    s += "\r\nReduces tire slip "+slip+"%";
  lbpartchars->Caption = s;
  lbpartname->Caption = GridCell(shopparts,2);
  btnbuypart->Enabled = !GridCellLike(shopparts,6,"b");
}
//---------------------------------------------------------------------------


void __fastcall Tmain::btnmodifyClick(TObject *Sender)
{
   if(GridCell(garageGrid,3) != "") // user car id
     SelectPage(tabmodifications);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::tabmodificationsShow(TObject *Sender)
{
//refresh
if(Sender==tabmodifications)
  ClearGrid(carparts);
Application->ProcessMessages();
Server.StartStack();
while(Server.StackQueries()) {

    // onshow is called from install/uninstall
    if(Sender==btninstall) {
    
      AnsiString partid = GridCell(carparts,0);
      if(GridCellLike(carparts,3,"not"))
        Parts.InstallPart(partid,1);
      else
        Parts.InstallPart(partid,0);
    }

    bool haveparts = false;
	cStringTable tbl(Parts.GetCarParts(GridCell(garageGrid,3)));
    if(!Server.Stack.Stacking) {
      cGridFiller g(carparts,true);
      while(tbl.GetLine()) {
        g[0] = tbl++; // usercarpartid
        g[1] = tbl++; // group name
        g[2] = tbl++; // part name
        g[3] = tbl++; // installed
        g[4] = tbl++; // HP
        g[5] = tbl++; // slip
        g.nextrow();
        haveparts = true;
      }

      if(!haveparts) {
        lbmodchars->Caption =
        "You don't have any parts for this car.\r\n"
        "Make this car selected and go to 'Speed Shop' to buy parts for it.\r\n";
      }
      else
        lbmodchars->Caption = "";
    }

  Cars.GetUpgradeValues(GridCell(garageGrid,3));
}

  AnsiString s;
  s.printf("Added %d horse(s).\r\nSlippery reduced by %d %. \r\n",
     Cars.addhp,Cars.reduceslip);
  lbupgrade->Caption = s;
  carparts->OnClick(0);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnbuypartClick(TObject *Sender)
{
  if(StrToInt(lbshopaccount->Caption)<=StrToInt(GridCell(shopparts,3)))
  throw Exception("You don't have enough money.");

  if(!request->confirm("Buy this part?\r\nDon't forget to install it.")) return;

  tabshop->OnShow(btnbuypart);
}
//---------------------------------------------------------------------------


void __fastcall Tmain::btninstallClick(TObject *Sender)
{
  if(GridCell(carparts,0)=="") return;
  tabmodifications->OnShow(Sender);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::carpartsClick(TObject *Sender)
{
  AnsiString s;
  if(GridCell(carparts,0)!="") {
    AnsiString hp = GridCell(carparts,4);
    if(hp!=S_0)
      s += "\r\nAdds "+hp + " horse(s)";
    AnsiString slip = GridCell(carparts,5);
    if(slip!=S_0)
      s += "\r\nReduces tire slip "+slip+"%";
    lbmodchars->Caption = s;
    btninstall->Enabled = true;
  } else {
    btninstall->Enabled = false;
  }

}
//---------------------------------------------------------------------------



void __fastcall Tmain::btnmaintenanceClick(TObject *Sender)
{
   if(GridCell(garageGrid,3) != "") // user car id
     SelectPage(tabmaintenance);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::tabmaintenanceShow(TObject *Sender)
{

Screen->Cursor = crAppStart;
Application->ProcessMessages();

 AnsiString usercarid = GridCell(garageGrid,3);
 Server.StartStack();
 while(Server.StackQueries()) {

   if(Sender==btnrefill) {
     AnsiString usercarid = GridCell(garageGrid,3);
     Cars.RefillNitrous(usercarid);
   }
   if(Sender==repair) {
     Cars.RepairEngine(usercarid);
   }
   if(Sender==reset) {
     Cars.ResetCar(usercarid);
   }

   Cars.GetUpgradeValues(usercarid);
   lbnitrousaccount->Caption = Account.GetBalance();
 }

   lbenginedamage->Caption = "Engine damage: "+FloatToStrF(Cars.EngineDamage,ffFixed,5,1) + "%" +
   "  Repair cost: "+IntToStr(Cars.EngineDamageRepairCost);

   repair->Enabled = Cars.EngineDamageRepairCost<StrToInt(lbnitrousaccount->Caption); 

   btngearratios->Enabled = Cars.MagicGear.Installed;
   if(!Cars.MagicGear.Installed)
     lbmagicgearmessage->Caption = "You don't have Magic Gear Box installed on this car.";
   else
     lbmagicgearmessage->Caption = "";

   lbnitrousleft->Caption = FloatToStr(Cars.Nitro.left)+"%";
   lbnitrouscost->Caption = IntToStr(Cars.Nitro.refillcost);

   btnrefill->Enabled = true;
   if(Cars.Nitro.hp==0) {
     btnrefill->Enabled = false;
     lbnitromessage->Caption = "You don't have nitrous installed on this car.";
   } else
   if(Cars.Nitro.left==100) {
     btnrefill->Enabled = false;
     lbnitromessage->Caption = "Nitrous full.";
   } else
   if(StrToInt(lbnitrousaccount->Caption)<Cars.Nitro.refillcost) {
     btnrefill->Enabled = false;
     lbnitromessage->Caption = "You don't have enough money to refill nitrous.";
   } else
     lbnitromessage->Caption ="";

  Screen->Cursor = crDefault;

}
//---------------------------------------------------------------------------


void __fastcall Tmain::btnrefillClick(TObject *Sender)
{
  if(StrToInt(lbnitrousaccount->Caption)<
     StrToInt(lbnitrouscost->Caption))
  throw Exception("You don't have enough money.");
  tabmaintenance->OnShow(Sender);
}
//---------------------------------------------------------------------------



void __fastcall Tmain::btnDeclineChallengeClick(TObject *Sender)
{
  AnsiString ChallengeID = GridCell(incoming_grid,0);
  if(ChallengeID.Length()==0) return;
  if(request->confirm("Decline this challenge?")) {
    //badly double filling tables
    tabchallenges->OnShow(btnDeclineChallenge);
    SelectPage(tabchallenges);
  }

}
//---------------------------------------------------------------------------

void __fastcall Tmain::race_edteamsearchKeyPress(TObject *Sender,
      char &Key)
{
    if(Key==13)
      btnteamsearch->OnClick(0);

}
//---------------------------------------------------------------------------




void __fastcall Tmain::btnteamsearchClick(TObject *Sender)
{
  teamraceShow(0);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnjointeamsearchClick(TObject *Sender)
{
  FillTeams(edteamsearch->Text);
  edteamsearchChange(edteamsearch);
  // clear grid
  cGridFiller clear(teammembers,true);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::edteamsearchKeyPress(TObject *Sender, char &Key)
{
  if(Key==13)
    btnjointeamsearch->OnClick(0);        
}
//---------------------------------------------------------------------------

void __fastcall Tmain::teamjoinrequestsClick(TObject *Sender)
{
  AnsiString id = teamjoinrequests->Cells[0][teamjoinrequests->Row];
  if(id.Length()!=0) {
    btnacceptjoin->Enabled = Team.owner;
    btndeclinejoin->Enabled= Team.owner;
  } else {
    btnacceptjoin->Enabled = false;
    btndeclinejoin->Enabled= false;
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::smokemClick(TObject *Sender)
{
  if(request->confirm("Send challenge to this racer?")) {
    SmokemTimer->Enabled = true;
    smokemnick = ((Tpms*)(Sender))->withnick;
  };
}
//---------------------------------------------------------------------------

void __fastcall Tmain::blockClick(TObject *Sender)
{
  Server.IgnoreUser(((Tpms*)(Sender))->withnick);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::tabchallengesShow(TObject *Sender)
{
  AnsiString ChallengeID = GridCell(incoming_grid,0);
  AnsiString OutgoingID = GridCell(outgoing,0);
  if(Sender==tabchallenges) {
	ClearGrid(incoming_grid);
	ClearGrid(outgoing);
  }
  Screen->Cursor = crAppStart;
  Application->ProcessMessages();
  Server.StartStack();
  while(Server.StackQueries()) {

    if(Sender==btnDeclineChallenge)
      Challenge.DeclineChallenge(ChallengeID);

	if(Sender==hideresultsbutton)
	  Challenge.HideIncoming(ChallengeID);

	if(Sender==hideoutgoing_button) {
      Challenge.HideOutgoing(OutgoingID);
    }

    FillChallenges();
  }
  Screen->Cursor = crDefault;

}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnHideIncomingClick(TObject *Sender)
{
  if(request->confirm("Don't show this challenge in list?"))
	tabchallengesShow(Sender);
}
//---------------------------------------------------------------------------






void __fastcall Tmain::tabaccountShow(TObject *Sender)
{
  edcurpass->Text = "";
  ednewpass->Text = "";
  ednewpassconfirm->Text = "";
//  this->lbModifyAccountBallance->Caption = "";
//  lbNick->Caption = "";

  cbnosound->Checked = ini->ReadInteger("Sound","Volume",0)==0;
  cbnoprivates->Enabled = Server.moder || Server.member;
  lbnoprivates->Enabled = cbnoprivates->Enabled;
  cbnoprivates->ShowHint = !cbnoprivates->Enabled;
  lbnoprivates->ShowHint = cbnoprivates->ShowHint;
  lbnoprivates->Enabled = cbnoprivates->Enabled;
  lbnoprivates->Hint = cbnoprivates->Hint;
  Screen->Cursor = crAppStart;
  Application->ProcessMessages();
  lbNick->Caption = Server.nick.val();
  Server.StartStack();
  while(Server.StackQueries()) {
    if(Sender==cbnoprivates)
      Account.SetNoPrivates(cbnoprivates->Checked);
    lbModifyAccountBallance->Caption = Account.GetBalance();
    Account.GetStatistics(Server.userid);
    Account.GetMembershipState();
	Account.GetSettings();
	Account.GetRank(Server.userid);
	Account.GetBadges(Server.userid);
  }

	lbrank->Caption = Account.rank;
	cbnoprivates->OnClick = NULL;
	cbnoprivates->Checked = Account.noprivates;
	cbnoprivates->OnClick = cbnoprivatesClick;
	lbstats->Caption = lbstats->Caption.sprintf(
	   "%s win(s), %s lose(s)", Account.wins, Account.loses);
  lbmember->Caption = Account.expires;
  Account.SetFlashBadges(flaccountbadges);
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::racerssearchClick(TObject *Sender)
{
  AnsiString RacerID = GridCell(racerssearch,0);
  if(RacerID!="") {
    Screen->Cursor = crAppStart;
    Server.StartStack();
    while(Server.StackQueries()) {
	  Account.GetStatistics(RacerID);
	  Account.GetRank(RacerID);
	  Account.GetBadges(RacerID);
	  Team.GetUserTeam(RacerID);
	  AnsiString RacerName = GridCell(racerssearch,1);
	  lbracername->Caption = RacerName;
	  AnsiString teaminfo = "Team: "+Team.teamname;
	  if(Team.teamname=="") teaminfo = " ";
	  lbracerstats->Caption = lbracerstats->Caption.sprintf(
		 "Rank: %s, %s win(s), %s lose(s)\n%s", Account.rank,
		 Account.wins, Account.loses, teaminfo);
	  AnsiString usercars = Cars.GetUserCars(RacerID,S_0);
	  cStringTable tbl(usercars);
	  cGridFiller g(racercars,true);
	  if(!Server.Stack.Stacking)
		while(tbl.GetLine()) {
		   g[0] = tbl++; // car id
		   g[1] = tbl++; // car name
		   g[2] = tbl++; // sell price
		   g[3] = tbl++; // user car id
		   tbl++; //4
		   tbl++; //5
		   tbl++; //6
		   tbl++; //7
		   g[4] = tbl++==S_1?"locked":"";

//		   Trace(tbl++);
//		   Trace(tbl++);

		   DecimalSeparator = '.';
		   g[8] = FloatToStrF(StrToFloat(tbl++),ffFixed,10,2); // average race time
		   g[9] = FloatToStrF(StrToFloat(tbl++),ffFixed,10,2); // last bracket time

		   g.nextrow();
	  }
	}
	racercars->OnClick(0);
	Screen->Cursor = crDefault;


  }

  Account.SetFlashBadges(flchallengebadges);

  AnsiString cars;
  AnsiString carcomments;
  TStringGrid* grid = racercars;
  for(int i=0;i<grid->RowCount;i++) {
	cars += grid->Cells[0][i]+"|"+grid->Cells[1][i]+"|";

		AnsiString carcomment;
		carcomment = "";
		if(StrToFloatDef(grid->Cells[8][i],0)!=0) {
		  carcomment = carcomment + "ET:"+grid->Cells[8][i];
		}
		if(StrToFloatDef(grid->Cells[9][i],0)!=0) {
		  carcomment = carcomment+" BT:"+grid->Cells[9][i];
		}
		if(!carcomment.Length()) {
			carcomment = "never used";
		}
		carcomments = carcomments + carcomment+"|";
  }

  flsetvar(flchallenge,"_root.CurrentCar",0);
  flsetvar(flchallenge,"_root.Cars",cars,false);
  flsetvar(flchallenge,"_root.CarsComments",carcomments,false);
  flsetvar(flchallenge,"_root.OldCars","",false);


  flchallenge->Visible = true;
//  flracercar->Visible = true;
//  lbracercar->Visible = true;
//  racercars->Visible = true;
  btnselectracer->Visible = true;
  lbracerstats->Visible = true;

}
//---------------------------------------------------------------------------

void __fastcall Tmain::racercarsClick(TObject *Sender)
{
  AnsiString carid;

  carid = GridCell(racercars,0);
  if(carid.IsEmpty()) return;

  lbracercar->Caption = GridCell(racercars,1);
//  Cars.SelectCar(flracercar,carid);
//  flsetvar(flracercar,"carname.text",GridCell(racercars,1));

  bool locked = GridCellLike(racercars,4,"lock");
  btnselectracer->Enabled = !locked;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::tabselectracerShow(TObject *Sender)
{
  Screen->Cursor = crAppStart;
//  flracercar->Visible = false;
  lbracercar->Visible = false;
  racercars->Visible = false;
  flchallenge->Visible = false;
  btnselectracer->Visible = false;
  lbracerstats->Visible = false;
  Application->ProcessMessages();
  // refresh racers list
  cStringTable tbl(Server.ChallengeList());
  racers->RowCount = 1;
  while(tbl.GetLine()) {
    racers->Cells[0][racers->RowCount-1] = tbl++;
    racers->Cells[1][racers->RowCount-1] = tbl++;
    racers->RowCount++;
  }
  racers->RowCount--;
  edracersearch->SetFocus();
  edracersearch->OnChange(0);
  Screen->Cursor = crDefault;
  selectracertimer->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::tabchallengeShow(TObject *Sender)
{
  Screen->Cursor = crAppStart;
  lbchallengeracerstats->Visible = false;
//  lbchallengecar->Visible = false;
//  lbchallengeanswercar->Visible = false;
  flchallengecar->Visible = false;
  flchallengeanswercar->Visible = false;

  Application->ProcessMessages();


  AnsiString userid = GridCell(incoming_grid,6);
  AnsiString usercarid = GridCell(incoming_grid,7);
  AnsiString answerusercarid = GridCell(incoming_grid,8);
  if(userid=="") return;
  Server.StartStack();
  while(Server.StackQueries()) {
	Account.GetStatistics(userid);
	lbchallengeracerstats->Caption =
	  lbchallengeracerstats->Caption.sprintf(
		"Racer: %s\n"
		"%s win(s) %s lose(s)",
		GridCell(incoming_grid,1),Account.wins,Account.loses);
	Trace("Opponent carid:"+usercarid);
	Cars.GetUserCarInfo(usercarid);
	flsetvar(flchallengecar,"_root.Cars",Cars.carid+"|"+Cars.carname+"|",false);
	Trace("Challenge car:"+Cars.carname);

	Trace("Answerusercarid:"+answerusercarid);
	Cars.GetUserCarInfo(answerusercarid);
	Trace("User car:"+Cars.carname);
	flsetvar(flchallengeanswercar,"_root.Cars",Cars.carid+"|"+Cars.carname+"|",false);
	Challenge.answercarid = Cars.carid;
	Challenge.answerusercarid = answerusercarid;
  }
  Screen->Cursor = crDefault;
  lbchallengeracerstats->Visible = true;
//  lbchallengecar->Visible = true;
  flchallengecar->Visible = true;
  flchallengeanswercar->Visible = true;
//  lbchallengeanswercar->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::btnchallengeClick(TObject *Sender)
{
  SelectPage(tabchallenge);
}
//---------------------------------------------------------------------------


void __fastcall Tmain::btnPinkSlipClick(TObject *Sender)
{
  Challenge.forCar = true;
  SelectPage(tabrace);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::race_teamsearchClick(TObject *Sender)
{
  ClearGrid(race_teammembers);        
}
//---------------------------------------------------------------------------

void __fastcall Tmain::teamsearchClick(TObject *Sender)
{
  ClearGrid(teammembers);
  lbjoin_teamstats->Caption = "";        
}
//---------------------------------------------------------------------------

void __fastcall Tmain::topsDblClick(TObject *Sender)
{
/*  if(tops->Tag==1) // top racers
  {
	smokemnick = GridCell(tops,1);
	if(smokemnick!=Server.nick.val()) {
	  if(request->confirm("Send challenge to this racer?")) {
		SmokemTimer->Enabled = true;
	  };
	} else {
	  throw Exception("Want to race yourself?");
	}
  }*/
}
//---------------------------------------------------------------------------

void __fastcall Tmain::SmokemTimerTimer(TObject *Sender)
{
	SmokemTimer->Enabled = false;
	SelectPage(tabselectracer);
	edracersearch->Text = smokemnick;
	char c;
	edracersearchKeyPress(0,c);
	racerssearchClick(0);
	Challenge.Reset();
	Challenge.withRacer = true;


}
//---------------------------------------------------------------------------

void __fastcall Tmain::tabwagerselectShow(TObject *Sender)
{
  Server.StartStack();
  while(Server.StackQueries()) {
    lbAccount->Caption = Account.GetBalance();
    Cars.GetFreeLeft();
  }
  
  btnPinkSlip->Enabled =
	Cars.freeleft!=S_1 && racercars->RowCount>1;

}
//---------------------------------------------------------------------------

void __fastcall Tmain::tabmainShow(TObject *Sender)
{
  tabaccountShow(Sender);
/*      toppanel->DoubleBuffered = true;
	  Screen->Cursor = crAppStart;
	  Application->ProcessMessages();
	  if(Sender==topteams) {
		tops->Tag = 2; //top teams
		cStringTable tbl(Team.TopTeams());
		if(Server.Stack.Stacking) return;
		ClearGrid(tops);
		cGridFiller gridliner(tops,true);
		int i = 1;
		while(tbl.GetLine()) {
		  gridliner[0] = IntToStr(i++); // place
		  gridliner[1] = tbl++; // team
		  gridliner[2] = tbl++ + " win(s) " + tbl++ + " lose(s)"; // wins
		  gridliner.nextrow();
		}
	  } else
	  if (Sender==fastestcars) {
		tops->Tag = 3; //top cars
		cStringTable tbl(Cars.TopCars());
		if(Server.Stack.Stacking) return;
		ClearGrid(tops);
		cGridFiller gridliner(tops,true);
		int i = 1;
		while(tbl.GetLine()) {
		  gridliner[0] = IntToStr(i++); // place
		  gridliner[1] = tbl++; // first column
		  gridliner[2] = tbl++; // second column
		  gridliner.nextrow();
		}
	  } else {
		tops->Tag = 1; // top racers
		cStringTable tbl(Challenge.TopResults());
		if(Server.Stack.Stacking) return;
		ClearGrid(tops);
		//IMPORTANT: on double click runs race with racer from list
		cGridFiller gridliner(tops,true);
		int i = 1;
		while(tbl.GetLine()) {
		  gridliner[0] = IntToStr(i++); // place
		  gridliner[1] = tbl++; // nick
		  gridliner[2] = tbl++; // result
		  gridliner.nextrow();
		}
	  }
	  Screen->Cursor = crDefault;*/

}
//---------------------------------------------------------------------------

void __fastcall Tmain::btngearratiosClick(TObject *Sender)
{
  gearratios->Color = this->Color;
  gearratios->ed1->Text = FloatToStrF(Cars.MagicGear.g1,ffFixed,10,3);
  gearratios->ed2->Text = FloatToStrF(Cars.MagicGear.g2,ffFixed,10,3);
  gearratios->ed3->Text = FloatToStrF(Cars.MagicGear.g3,ffFixed,10,3);
  gearratios->ed4->Text = FloatToStrF(Cars.MagicGear.g4,ffFixed,10,3);
  gearratios->ed5->Text = FloatToStrF(Cars.MagicGear.g5,ffFixed,10,3);
  gearratios->ed6->Text = FloatToStrF(Cars.MagicGear.g6,ffFixed,10,3);
  gearratios->maxspeednum = Cars.maxspeednum;
  if(mrOk==gearratios->ShowModal()) {
     Screen->Cursor = crAppStart;
     Application->ProcessMessages();
     Cars.MagicGear.g1 = StrToFloat(gearratios->ed1->Text);
     Cars.MagicGear.g2 = StrToFloat(gearratios->ed2->Text);
     Cars.MagicGear.g3 = StrToFloat(gearratios->ed3->Text);
     Cars.MagicGear.g4 = StrToFloat(gearratios->ed4->Text);
     Cars.MagicGear.g5 = StrToFloat(gearratios->ed5->Text);
     Cars.MagicGear.g6 = StrToFloat(gearratios->ed6->Text);
     AnsiString usercarid = GridCell(garageGrid,3);
     Cars.SaveMagicGear(usercarid);
     Screen->Cursor = crDefault;
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::garageGridDrawCell(TObject *Sender, int ACol, int ARow,
      TRect &Rect, TGridDrawState State)
{
  TStringGrid* g = (TStringGrid*)Sender;
  g->Canvas->Font = g->Font;
  if(State.Contains(gdSelected))
	 g->Canvas->Brush->Color = GridSelectColor;
  else
	g->Canvas->Brush->Color = clBlack;
  g->Canvas->FillRect(Rect);
//   SetBkMode(inroom->Canvas->Handle,TRANSPARENT);
  g->Canvas->TextOut(Rect.Left+2,Rect.Top+2,g->Cells[ACol][ARow]);
//  if(State.Contains(gdFocused))
//    g->Canvas->DrawFocusRect(Rect);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::durusTimer(TObject *Sender)
{
  durus->Enabled = false;
  if(FileExists("\\dragcentraltesting.") && (ParamStr(1)=="-test" || ParamStr(2)=="-test")) return;

  SetCurrentDir(ExtractFileDir(Application->ExeName));
  // CONTROL SUMS ARE WRITEN TO INI DURING INSTALLER CREATION
  bool wrong = false;
  wrong =
//	ini->ReadInteger("System",S_1,-1)!=
//    checker.CheckFile(".\\dragcentral.exe")
//    ||
//    ini->ReadInteger("System","2",-1)!=
//    checker.CheckFile(".\\flash\\car-panel.swf")
//    ||
    ini->ReadInteger("System","3",-1)!=
    checker.CheckFile(".\\flash\\race.swf")
    ||
    ini->ReadInteger("System","4",-1)!=
    checker.CheckFile(".\\flash\\staging.swf")
    ||
    ini->ReadInteger("System","5",-1)!=
    checker.CheckFile(".\\flash\\track.swf")
	||
	ini->ReadInteger("System","7",-1)!=
	checker.CheckFile(".\\flash\\garage.swf")
	;

  if(wrong) exit(EXIT_SUCCESS);

//checker.exe .\files\dragcentral.exe .\files\dragcentral.ini system 1
//checker.exe .\files\flash\car-panel.swf .\files\dragcentral.ini system 2
//checker.exe .\files\flash\race.swf .\files\dragcentral.ini system 3
//checker.exe .\files\flash\staging.swf .\files\dragcentral.ini system 4
//checker.exe .\files\flash\track.swf .\files\dragcentral.ini system 5
//checker.exe .\files\flash\carview.swf .\files\dragcentral.ini system 6

}
//---------------------------------------------------------------------------

void __fastcall Tmain::membershipClick(TObject *Sender)
{
  AnsiString WebSite =
  ini->ReadString("Web","Membership","http://www.myracinggame.com/membership.php");
  ShellExecute(NULL,"open",WebSite.c_str(),NULL,NULL,SW_SHOW);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::topteamsClick(TObject *Sender)
{
//  tabmainShow(topteams);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::topracersClick(TObject *Sender)
{
//  tabmainShow(topracers);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::fastestcarsClick(TObject *Sender)
{
//  tabmainShow(fastestcars);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::tabloginShow(TObject *Sender)
{
  if(ednick->Text!="")
    this->ActiveControl = edpass;        
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Quit1Click(TObject *Sender)
{
   Close();        
}
//---------------------------------------------------------------------------


void __fastcall Tmain::Gotochat1Click(TObject *Sender)
{
  if(Server.logedin)
    btnchatClick(0);
}
//---------------------------------------------------------------------------


void __fastcall Tmain::ShowMessage1Click(TObject *Sender)
{
  message->ShowMessage("Hello!");
}
//---------------------------------------------------------------------------

void __fastcall Tmain::challenginfo1Click(TObject *Sender)
{
  challengeinfo->btnrace->Visible= false;
  challengeinfo->ShowModal();
}
//---------------------------------------------------------------------------


void __fastcall Tmain::lbnoprivatesClick(TObject *Sender)
{
  if(lbnoprivates->Enabled) {
    cbnoprivates->Checked = !cbnoprivates->Checked;
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::cbnoprivatesClick(TObject *Sender)
{
   tabaccountShow(cbnoprivates);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::cbnosoundClick(TObject *Sender)
{
  if(cbnosound->Checked) {
     ini->WriteInteger("Sound","Volume",0);
     Audio.SetVolume(0);
  } else {
     ini->WriteInteger("Sound","Volume",100);
     Audio.SetVolume(1./10);
  }

}
//---------------------------------------------------------------------------

void __fastcall Tmain::lbnosoundClick(TObject *Sender)
{
  cbnosound->Checked = !cbnosound->Checked;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::tabteammessShow(TObject *Sender)
{
  Screen->Cursor = crAppStart;
  Application->ProcessMessages();
  Server.StartStack();
  while(Server.StackQueries()) {
     if(Sender==teammess_post)
       Team.TeamMessage_Post(teammessage->message->Text);
     if(Sender==teammess_edit)
       Team.TeamMessage_Edit(GridCell(sgteammess,0),teammessage->message->Text);
     if(Sender==teammess_del)
       Team.TeamMessage_Delete(GridCell(sgteammess,0));
     cStringTable tbl(Team.TeamMessage_GetAll());
     if(Server.Stack.Stacking) continue;
     cGridFiller gridliner(sgteammess,true);
     while(tbl.GetLine()) {
        gridliner[0] = tbl++;//id
        gridliner[1] = tbl++;//userid
        gridliner[2] = tbl++;//nick
        int sec = StrToInt(tbl++);//time
        AnsiString shours = IntToStr(sec/3600);
		AnsiString smin = IntToStr((sec%3600)/60);
		if(smin.Length()==1) smin = S_0+smin;
		gridliner[3] = shours+"h:"+smin+"m ago";
		gridliner[4] = tbl++;//message
		gridliner.nextrow();
	 }
  }
  Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::sgteammessClick(TObject *Sender)
{
  bool otheruser = GridCell(sgteammess,1)!=Server.userid;
  teammess_edit->Enabled = !otheruser;
  teammess_del->Enabled = !otheruser || Team.owner;        
}
//---------------------------------------------------------------------------

void __fastcall Tmain::teammess_postClick(TObject *Sender)
{
  teammessage->message->Text = "";
  teammessage->Caption = "Post team message";
  teammessage->Cap->Caption = "Enter new team message text:";
  if(mrOk==teammessage->ShowModal()) {
     if(teammessage->message->Text!="")
       tabteammessShow(teammess_post);
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::teammess_editClick(TObject *Sender)
{
  teammessage->message->Text = GridCell(sgteammess,4);
  teammessage->Caption = "Edit team message";
  teammessage->Cap->Caption = "Team message text:";
  if(mrOk==teammessage->ShowModal()) {
     tabteammessShow(teammess_edit);
  }

}
//---------------------------------------------------------------------------

void __fastcall Tmain::teammess_delClick(TObject *Sender)
{
  if(request->confirm("Delete this team message?")) {
     tabteammessShow(teammess_del);
  }
}
//---------------------------------------------------------------------------




void __fastcall Tmain::repairClick(TObject *Sender)
{
  if(request->confirm("Repair engine?"))
    tabmaintenanceShow(repair);
}
//---------------------------------------------------------------------------

void __fastcall Tmain::resetClick(TObject *Sender)
{
  if(request->confirm("Reseting your car will remove any damage,\n BUT all your parts will be removed too.\nContinue?"))
    tabmaintenanceShow(reset);
}
//---------------------------------------------------------------------------




void __fastcall Tmain::Help1Click(TObject *Sender)
{
  if(this->pages->ActivePage==this->tabrace) {
    Help->ShowModal();

  }	
}
//---------------------------------------------------------------------------


void __fastcall Tmain::autologin_tmrTimer(TObject *Sender)
{
  ((TTimer*)Sender)->Enabled = false;
  Trace("autologin");

  ednick->Text = param("user");
  edpass->Text = param("pass");
  btnloginClick(0);



  if(command("onlinechallenge")) {
	btnOnlineChallengeClick(0);
	racerssearchClick(0);
	btnselectracerClick(0);
	btnHeadsUpClick(0);
	message_edt->Text = "Hey you asshole";
	btnForFunClick(0);
  }

  if(command("viewchallenges")) {
	  btnchallengesClick(0);
  }

  if(command("viewresults")) {
	incomingresultsClick(0);
  }

  if(command("challenges")) {
	btnchallengesClick(0);
  }

  if(command("challenge")) {
	btnchallengeClick(0);
  }

  if(command("outgoing")) {
	pageschallenges->ActivePage = taboutgoing;
  }

  if(command("chat")) {
	 this->SelectPage(tabchat);
  }

  if(command("race")) {
	  SelectPage(tabracestart);
  }

  if(command("teams")) {
	 this->SelectPage(tabteam);
  }

  if(command("garage")) {
      SelectPage(tabgarage);
  }



}
//---------------------------------------------------------------------------





void __fastcall Tmain::tabracestartShow(TObject *Sender)
{
//  Image65->Visible = false;
 // dummytabstop->SetFocus();
}
//---------------------------------------------------------------------------




void __fastcall Tmain::Sensor12Click(TObject *Sender)
{
  outgoingpanel->Visible = true;
  IncomingPanel->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::incoming_buttonClick(TObject *Sender)
{
  IncomingPanel->Visible = true;
  outgoingpanel->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Sensor14Click(TObject *Sender)
{
  team_nb->PageIndex = 0;	
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Sensor15Click(TObject *Sender)
{
  team_nb->PageIndex = 1;	
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Sensor16Click(TObject *Sender)
{
  team_nb->PageIndex = 2;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Sensor17Click(TObject *Sender)
{
  team_nb->PageIndex = 3;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::Sensor18Click(TObject *Sender)
{
  team_nb->PageIndex = 4;
}
//---------------------------------------------------------------------------

void __fastcall Tmain::team_nbPageChanged(TObject *Sender)
{
//  ShowMessage(IntToStr(team_nb->PageIndex));
  switch(team_nb->PageIndex) {
	  case 0: tabteammessShow(Sender); break;
	  case 1: teamteamShow(Sender); break;
	  case 2: teamraceShow(Sender); break;
	  case 3: teamchallengeShow(Sender); break;
	  case 4: teamfundsShow(Sender); break;
  }
}
//---------------------------------------------------------------------------




void __fastcall Tmain::garagetimerTimer(TObject *Sender)
{
  int flashCurrent = StrToIntDef(flgetvar(flgarage,"_root.CurrentCar"),0);
  if(garageGrid->Row != flashCurrent) {
	  garageGrid->Row = flashCurrent;
  }
}
//---------------------------------------------------------------------------

void __fastcall Tmain::selectracertimerTimer(TObject *Sender)
{
  if(pages->ActivePage!=tabselectracer) {
      selectracertimer->Enabled = false;
  }
  int flashCurrent = StrToIntDef(flgetvar(flchallenge,"_root.CurrentCar"),0);
  flsetvar(flchallenge,"_root.SelectedCar",flashCurrent);
  if(racercars->Row != flashCurrent) {
	  racercars->Row = flashCurrent;
  }

}
//---------------------------------------------------------------------------



void __fastcall Tmain::registercarlistDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
  TStringGrid* g = registercarlist;
//  g->Canvas->Font = g->Font;
/*  if(State.Contains(gdSelected))
	 g->Canvas->Brush->Color = GridSelectColor;
  else
	g->Canvas->Brush->Color = clBlack;*/
	
  g->Canvas->FillRect(Rect);
  g->Canvas->TextOut(Rect.Left+2,Rect.Top+2,g->Cells[ACol][ARow]);
}
//---------------------------------------------------------------------------

