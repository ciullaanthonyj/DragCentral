//---------------------------------------------------------------------------


#include <pch.h>
#pragma hdrstop

#include "chatserver.h"
#include "messageform.h"
#include "csecstrings.h"
#include "form_trace.h"

//---------------------------------------------------------------------------

__fastcall TChatThread::TChatThread(bool CreateSuspended)
        : TThread(CreateSuspended)
{
  RefreshRoom = false;
  ShowQuitEnter = false;
  ChatMessageMutex = CreateMutex(NULL,false,NULL);
  RoomMutex = CreateMutex(NULL,false,NULL);
  OtherQueryMutex = CreateMutex(NULL,false,NULL);
  Done = false;
  chatmess = new TStringList;
  chattonick = new TStringList;
  OtherQueries = new TStringList;
  SleepInterval = 700; // default
  ChatActive = false;
}

__fastcall TChatThread::~TChatThread() {
  CloseHandle(ChatMessageMutex);
  CloseHandle(RoomMutex);
  CloseHandle(OtherQueryMutex);
  delete chatmess;
  delete chattonick;
  delete OtherQueries;
}
void TChatThread::ChatMessageQue(AnsiString mess,AnsiString tonick) {
  WaitForSingleObject(ChatMessageMutex,INFINITE);
  chatmess->Add(mess);
  chattonick->Add(tonick);
  ReleaseMutex(ChatMessageMutex);
}


void _fastcall TChatThread::OtherQueries_process() {
  WaitForSingleObject(OtherQueryMutex,INFINITE);
  if(OtherQueries->Count!=0) {
     Owner->Query(OtherQueries->Strings[0],"");
     OtherQueries->Delete(0);
  }
  ReleaseMutex(OtherQueryMutex);
}

void TChatThread::OtherQueryAddQue(AnsiString query) {
  WaitForSingleObject(OtherQueryMutex,INFINITE);
	OtherQueries->Add(query);
  ReleaseMutex(OtherQueryMutex);
}
void __fastcall TChatThread::GetSysMessages() {
  AnsiString res = Owner->Query(encoder.decode("8DF6B283CEA3D67D03B741") /*"sysmess.php"*/,encoder.decode("8B3D3E08DC10C8") /*"userid="*/+Owner->userid);
  SysMessages = res;
  Synchronize(TThreadMethod(&CallOnSysMessages));
}

void __fastcall TChatThread::CallOnSysMessages() {
  if(OnSysMessages!=NULL)
    OnSysMessages(NULL);
}
void __fastcall TChatThread::ChatRequests() {
  if(LastMess==-1) return;

  AnsiString addmess = "";
  while(true) {
    WaitForSingleObject(ChatMessageMutex,INFINITE);
    if(chatmess->Count==0) { ReleaseMutex(ChatMessageMutex); break; }
    AnsiString cur_nick = nick.val();
    AnsiString cur_mess = chatmess->Strings[0];
    AnsiString cur_tonick = chattonick->Strings[0];
    chatmess->Delete(0);
    chattonick->Delete(0);
    ReleaseMutex(ChatMessageMutex);
    if(cur_tonick!="") {
      addmess += encoder.decode("D8F12E42A1E8") /*"&nk[]="*/+HTTPParam(cur_nick)+encoder.decode("D8F295B9C32B") /*"&ms[]="*/+HTTPParam(cur_mess)+encoder.decode("D8EBEECB670B2096") /*"&tonk[]="*/+HTTPParam(cur_tonick)+encoder.decode("D8EDAE90A37E") /*"&rm[]="*/;
//      Owner->Query(encoder.decode("9FBF6E9CF83D47DBDC2F4B") /*"addmess.php"*/,encoder.decode("9018889929") /*"nick="*/+HTTPParam(cur_nick)+encoder.decode("D8F283E4BE5A") /*"&mess="*/+HTTPParam(cur_mess)+encoder.decode("D8EBEECB656DC05F") /*"&tonick="*/+HTTPParam(cur_tonick),false);
    }
    else {
	  addmess += encoder.decode("D8F12E42A1E8") /*"&nk[]="*/+HTTPParam(cur_nick)+encoder.decode("D8F295B9C32B") /*"&ms[]="*/+HTTPParam(cur_mess)+encoder.decode("D8EBEECB670B2096CFECCECB2CAB") /*"&tonk[]=&rm[]="*/+room;
//      Owner->Query(encoder.decode("9FBF6E9CF83D47DBDC2F4B") /*"addmess.php"*/,encoder.decode("9018889929") /*"nick="*/+HTTPParam(cur_nick)+encoder.decode("D8F283E4BE5A") /*"&mess="*/+HTTPParam(cur_mess)+encoder.decode("D8EDACE56917") /*"&room="*/+room,false);
    }
  }


  if(room=="") return;

  WaitForSingleObject(RoomMutex,INFINITE);
  AnsiString res = Owner->Query(encoder.decode("92D3E21108CEEF7EEFB2A8B9") /*"lastmess.php"*/,encoder.decode("97B29E") /*"id="*/+IntToStr(LastMess)+encoder.decode("D8EDACE56917") /*"&room="*/+room+encoder.decode("D8F12CBBF2B1") /*"&nick="*/+HTTPParam(nick.val())+addmess);
//  res = AnsiReplaceStr(res,encoder.decode("DBB3E0") /*"%25"*/,encoder.decode("DB") /*"%"*/);
  ReleaseMutex(RoomMutex);

  AnsiString id;
  cStringTable tbl(res);
  while(tbl.GetLine()) {

	id = tbl++;
	msg.nick = tbl++;
	msg.text = tbl++;
	msg.tonick = tbl++;
	msg.priv = tbl++==S_1;
	msg.TypeFlags = StrToIntDef(tbl++,0);
	msg.admin = msg.TypeFlags & ADMIN_FLAG;
	msg.supermod = msg.TypeFlags & SUPERMOD_FLAG;
	msg.moder = msg.TypeFlags & MODER_FLAG;
	msg.member = msg.TypeFlags & MEMBER_FLAG;
	msg.male = msg.TypeFlags & MALE_FLAG;
	msg.female = msg.TypeFlags & FEMALE_FLAG;
	msg.sysmessage = false;
	msg.youwerekicked = false;
	msg.youwerechatbanned = false;
	msg.youweregamebanned = false;
	LastMess = StrToInt(id);
	if(msg.nick.val() == encoder.decode("DBD2100761AA8BE1") /*"%SYSTEM%"*/) {
	   msg.sysmessage = true;
       msg.nick = encoder.decode("ADDD79930267") /*"System"*/;
    }
    
    if(Signature(encoder.decode("DBCA0D70B2127734") /*"%KICKED%"*/,msg.text)) {
       RefreshRoom = true;
       msg.reason = msg.text.SubString(9,msg.text.Length());
       msg.text = encoder.decode("DE14E687AA627E794602A8E662") /*" was kicked. "*/+ msg.reason;
       msg.youwerekicked = this->nick.val()==msg.nick.val();
       msg.sysmessage = true;
	   Synchronize(TThreadMethod(&ShowRecievedMessage));
	} else
	if(Signature(encoder.decode("DBCA0D70B21277536B6B2BCA15") /*"%KICKEDBANED%"*/,msg.text)) {
	   RefreshRoom = true;
	   msg.reason = msg.text.SubString(14,msg.text.Length());
	   msg.text = encoder.decode("DE14E687AA627E794602A8E8E26A6E304BEA33840D3BFD02D9EDFBFC45") /*" was kicked and chat banned. "*/+ msg.reason;
	   msg.youwerechatbanned = this->nick.val()==msg.nick.val();
	   msg.sysmessage = true;
	   Synchronize(TThreadMethod(&ShowRecievedMessage));
	} else
	if(Signature(encoder.decode("DBCA0D70B21277564D841663769B6C5DA2") /*"%KICKEDGAMEBANED%"*/,msg.text)) {
	   RefreshRoom = true;
	   msg.reason = msg.text.SubString(18,msg.text.Length());
	   msg.text = encoder.decode("DE14E687AA627E794602A8E8E26A6E304F64B164C2AD630FFC4207BFD7") /*" was kicked and game banned. "*/+ msg.reason;
	   msg.youweregamebanned = this->nick.val()==msg.nick.val();
	   msg.sysmessage = true;
	   Synchronize(TThreadMethod(&ShowRecievedMessage));
	} else
	if(msg.text==encoder.decode("DBD05D1C24C1") /*"%QUIT%"*/ || msg.text==encoder.decode("DBC4CE0B9C332F") /*"%ENTER%"*/) {
	   RefreshRoom = true;
	   msg.sysmessage = true;
	   if(msg.text==encoder.decode("DBD05D1C24C1") /*"%QUIT%"*/) msg.text = encoder.decode("DE12333B6A23") /*" quit."*/;
	   if(msg.text==encoder.decode("DBC4CE0B9C332F") /*"%ENTER%"*/) msg.text = encoder.decode("DE06D08D183A977C1E") /*" entered."*/;
	   if(ShowQuitEnter) {
         Synchronize(TThreadMethod(&ShowRecievedMessage));
       }
    }
    else {
      Synchronize(TThreadMethod(&ShowRecievedMessage));
    }
  }

  if(room!="") {
    if(RefreshRoom) {
      if(Owner->moder)
        PeopleInRoom = Owner->Query(encoder.decode("8C80E14CF0DB5516E38BCC29891DD2") /*"room_people.php"*/,
          encoder.decode("8C80E14C92") /*"room="*/+room+encoder.decode("D8FE02EEBEE0") /*"&all=1"*/);
      else
        PeopleInRoom = Owner->Query(encoder.decode("8C80E14CF0DB5516E38BCC29891DD2") /*"room_people.php"*/,
          encoder.decode("8C80E14C92") /*"room="*/+room);
      Synchronize(TThreadMethod(&RoomPeopleUpdate));
      RefreshRoom = false;
    }
  }

}
void __fastcall TChatThread::ShowRecievedMessage() {
    Timer.Stop();
	if(ShowMessageTime)
      msg.text += encoder.decode("DE") /*" "*/+IntToStr(Timer.Milliseconds());
    if(OnRecieveMessage!=NULL) {
      Sleep(12);
      OnRecieveMessage((TObject*)&msg);
      // to see scrolling
    }
}
//---------------------------------------------------------------------------
void __fastcall TChatThread::myShowMessage() {
  message->ShowMessage(showmess);
}

void __fastcall TChatThread::RoomPeopleUpdate() {
  if(OnRoomPeopleUpdate!=NULL)
    OnRoomPeopleUpdate(NULL);
}

void __fastcall TChatThread::Execute()
{

  while(true) {

    try {
      if(ChatActive)
         ChatRequests();

      this->OtherQueries_process();
      static int challengestriger = 0;
      if(challengestriger==0) {
        this->GetSysMessages();
		challengestriger = 12;
      }
      challengestriger--;

    } catch (const Exception &E) {
       if(E.Message=="")
         showmess = encoder.decode("AB0CFE9971339DE0819FC1ECCA5BBF36614B1272755D2B6F79CE03CA05C813EFB824CE7B") /*"Unknown error in chat server thread."*/;
       else
         showmess = E.Message;
       if(ParamStr(1)==encoder.decode("D318311B73E3") /*"-debug"*/ || ParamStr(2)==encoder.decode("D318311B73E3") /*"-debug"*/)
         Synchronize(TThreadMethod(&myShowMessage));
    }
    if(Terminated) break;
//    Sleep(SleepInterval);
    for(int i=0;i<SleepInterval/3;i++) {
      Sleep(3);
      if(this->chatmess->Count!=0)
        break;
      if(this->OtherQueries->Count!=0)
        break;
      if(Terminated) break;
    }
    if(Terminated) break;
  }

  Done = true;
}
cChatServer::cChatServer(){
  room = ""; nick = "";
  chat = NULL;
  chat = new TChatThread(true);
  chat->Owner = this;
  logedin = false;
};

cChatServer::~cChatServer(){
  if(!chat->Suspended) {
    chat->Terminate();
    chat->WaitFor();
  }
  delete chat;
};

void cChatServer::EnterRoom(AnsiString roomid, AnsiString pass) {
    WaitForSingleObject(chat->RoomMutex,INFINITE);
    AnsiString res=Query(encoder.decode("8B3D3E08EA5AFE5749F2D93D72C33367B5") /*"user_goinroom.php"*/,encoder.decode("9018889929") /*"nick="*/+HTTPParam(nick.val())+encoder.decode("D8EDACE56917") /*"&room="*/+roomid+encoder.decode("D8EF653FCF18") /*"&pass="*/+HTTPParam(pass));
    if(Signature(encoder.decode("BB1F8CDD8F") /*"Error"*/,res)) {
      ReleaseMutex(chat->RoomMutex);
      throw(Exception(res));
    }
    int LastMess =
      StrToIntDef(res,chat->LastMess);
//    if(LastMess>chat->LastMess)
    chat->LastMess = LastMess;
    chat->room = roomid;
    chat->nick = nick;
    chat->RefreshRoom = true;
    room=roomid;
    if(chat->OnChangeRoom!=NULL)
      chat->OnChangeRoom(0);
    ReleaseMutex(chat->RoomMutex);
    chat->ChatActive = true;
}

void cChatServer::QuitChat() {
  Query(encoder.decode("8B3D3E08EA4CBD21B0E776F1A6ED3FCB46") /*"user_quitchat.php"*/,encoder.decode("9018889929") /*"nick="*/+nick.val());
  if(Stack.Stacking) return;
  this->room = "";
  chat->ChatActive = false;
}

void cChatServer::Logout() {
  if(userid!="")
    Query(encoder.decode("8B3D3E089BDAF433") /*"user.php"*/,encoder.decode("9FB8D1C40A86531C544FE7A311E3387FEC1F") /*"act=logout&userid="*/+userid);
  logedin = false;
}


void cChatServer::ChangePass(AnsiString nowpass, AnsiString newpass) {
  AnsiString res = Query(encoder.decode("8B3D3E089BDAF433") /*"user.php"*/,
    encoder.decode("9FB8D1C405AD87358A372F0B970349A5CF18") /*"act=changepass&id="*/+userid+encoder.decode("D8F0C950DB") /*"&old="*/+HTTPParam(nowpass)+
    encoder.decode("D8F12027E0") /*"&new="*/+HTTPParam(newpass));
  if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
    throw Exception(res);
}

void cChatServer::SendPass(AnsiString email) {
  AnsiString res = Query(encoder.decode("8B3D3E089BDAF433") /*"user.php"*/,
  encoder.decode("9FB8D1C415B5069331838EA1B638495B1F0CB6") /*"act=sendpass&email="*/+HTTPParam(email));

  AnsiString sig=encoder.decode("ADD1E12F30E6") /*"Succes"*/;
  if(res.SubString(0,sig.Length())==sig) {
     message->ShowMessage(encoder.decode("B2A8E87348823A3C356D86D9CF66A39ADF72AB9443D330269049105F48FA3AB92CC3B83A648CB43986D4692E") /*"Login and password were sent to your e-mail."*/);
  } else {
    throw Exception(res);
  }



}
bool cChatServer::Login(AnsiString nick,AnsiString pass,AnsiString version) {
  AnsiString res=Query(encoder.decode("8B3D3E08EA5183CD1E025836BD20") /*"user_login.php"*/,
  encoder.decode("9972BE07D898D76FB219184C6E94") /*"getinfo=1&ver="*/+version+encoder.decode("D8F12CBBF2B1") /*"&nick="*/+HTTPParam(nick)+encoder.decode("D8EF653FCF18") /*"&pass="*/+HTTPParam(pass)
  +encoder.decode("D8EBF3A91E6ABF") /*"&track="*/+version
  );
  if(Stack.Stacking) return false;
  if(Signature(encoder.decode("BF9ABFCC0BFCAEFAFD05") /*"Authorized"*/,res)) {
     // cut authorized
     res = res.SubString(res.Pos(encoder.decode("82") /*"|"*/)+1,res.Length());
     cStringTable tbl(res);
     tbl.GetLine();
     this->userid=StrToInt(tbl++);
     int flags = StrToInt(tbl++);
     this->nick = tbl++;
     this->moder = (flags & MODER_FLAG) || (flags & ADMIN_FLAG);
     this->member = (flags & MEMBER_FLAG);
     if(chat->Suspended)
       chat->Resume();
     logedin = true;
     return True;
  }
  else
     throw Exception(res);
}
void cChatServer::IgnoreUser(AnsiString nick) {
  AnsiString res = Query(encoder.decode("9DF17B801F8E4855") /*"chat.php"*/,
    encoder.decode("9FB8D1C40F6B64DB61721A61D69C92B1680E2FD9") /*"act=ignoreuser&nick="*/+HTTPParam(nick)+
    encoder.decode("D8EA92A7419065C2") /*"&userid="*/+userid);
  if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
    throw Exception(res);

}

void cChatServer::UnignoreUser(AnsiString nick) {
     chat->OtherQueryAddQue(
    encoder.decode("9DF17B801F8E4855D5B5531EF7FCEA2958566B70138E5E90C4E1DDBD264DD4") /*"chat.php?act=unignoreuser&nick="*/+HTTPParam(nick)+
    encoder.decode("D8EA92A7419065C2") /*"&userid="*/+userid);
}

void cChatServer::Kick(AnsiString userid, AnsiString reason) {
  AnsiString res = Query(encoder.decode("933C9DF245904B09C215F29F6B") /*"moder_act.php"*/,
    encoder.decode("9FB8D1C405AD872F425CCB6B571A747E3D22329349") /*"act=chatkick&moderid="*/+this->userid + encoder.decode("D8EA92A7419065C2") /*"&userid="*/+userid+
    encoder.decode("D8EDA6A75D2D448B") /*"&reason="*/+HTTPParam(reason));
  if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
    throw Exception(res);
}

void cChatServer::ChatBan(AnsiString userid, AnsiString reason, int Length) {
  AnsiString res = Query(encoder.decode("933C9DF245904B09C215F29F6B") /*"moder_act.php"*/,encoder.decode("9FB8D1C405AD872F4BBA64B167A1B77A06F253B0") /*"act=chatban&moderid="*/+this->userid+encoder.decode("D8EA92A7419065C2") /*"&userid="*/+userid+
    encoder.decode("D8EDA6A75D2D448B") /*"&reason="*/+HTTPParam(reason) + encoder.decode("D8F3E235D031165E") /*"&length="*/+IntToStr(Length));
  if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
    throw Exception(res);
}

void cChatServer::GameBan(AnsiString userid, AnsiString reason, int Length) {
  AnsiString res = Query(encoder.decode("933C9DF245904B09C215F29F6B") /*"moder_act.php"*/,encoder.decode("9FB8D1C401226333C26108A86E13429680553CC6") /*"act=gameban&moderid="*/+this->userid+encoder.decode("D8EA92A7419065C2") /*"&userid="*/+userid+
    encoder.decode("D8EDA6A75D2D448B") /*"&reason="*/+HTTPParam(reason) + encoder.decode("D8F3E235D031165E") /*"&length="*/+IntToStr(Length));
  if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
    throw Exception(res);
}

AnsiString cChatServer::CreateAccount(AnsiString nick,AnsiString pass, AnsiString email,
  bool male, bool female, AnsiString carid) {

  AnsiString res = Query(encoder.decode("8B3D3E08EA4FCF6C1624A49B7C40483D32") /*"user_register.php"*/,encoder.decode("9018889929") /*"nick="*/+HTTPParam(nick)+encoder.decode("D8EF653FCF18") /*"&pass="*/+HTTPParam(pass)+
    encoder.decode("D8FA86D64D7C62") /*"&email="*/+HTTPParam(email)+
    encoder.decode("D8F28775A337") /*"&male="*/+booltostr(male)+
    encoder.decode("D8F92F9581169004") /*"&female="*/+booltostr(female)+
    encoder.decode("D8FC4D30619851") /*"&carid="*/+carid);
              // 12345678
  if(!Signature(encoder.decode("ADD1E12F30E65ED0") /*"Success."*/,res))
    throw Exception(res);
  return res.SubString(9,res.Length());
}

AnsiString cChatServer::RoomList() {
  return Query(encoder.decode("8C80E14CC306F943FA8F0056") /*"roomlist.php"*/,encoder.decode("8B3D3E08DC10C8") /*"userid="*/+userid);
}
void cChatServer::CreateRoom(AnsiString room,AnsiString pass) {
  AnsiString res = Query(encoder.decode("8C80E14CF0C83674EE1B2F2FE4CD9E") /*"room_create.php"*/,encoder.decode("8C80E14C92") /*"room="*/ + HTTPParam(room) + encoder.decode("D8EF653FCF18") /*"&pass="*/ + HTTPParam(pass)+encoder.decode("D8EA92A7419065C2") /*"&userid="*/+userid);
  AnsiString sig=encoder.decode("ADD1E12F30E6") /*"Succes"*/;
  if(res.SubString(0,sig.Length())==sig) {
     message->ShowMessage(encoder.decode("B0E33207A5E885D4B2826BF0B91DF8501523A6DA13A6BF3BE639553F61BE8E848C38B32DE53874E4B109866E5D358714B1104400011A61704BC93D90AD96C7A34E37921BFB9767C6EFDAA94D1EDD14B0327529AE497DB4627D3B") /*"New room was succesfully created. It will be removed after some time, if it will be empty."*/);
  } else {
    throw Exception(res);
  }
}
AnsiString killbadchars(AnsiString in) {
  AnsiString s = in;
  int l=s.Length();
  for(int i=1;i<=l;i++)
    if((unsigned char)s[i]>=127)
      s[i] = '*';
  return s;
}
void cChatServer::SendMessage(AnsiString msg,AnsiString tonick) {
  msg = msg.SubString(1,128);
  if(msg.Trim() == "") return;
  chat->Timer.Start();
  msg = killbadchars(msg);
  tonick = killbadchars(tonick);
  chat->ChatMessageQue(msg,tonick);
}


void cChatServer::Unban(AnsiString nick) {
  AnsiString res = Query(encoder.decode("8B3D3E08EA482848205D0122833E") /*"user_unban.php"*/,encoder.decode("9018889929") /*"nick="*/+nick);
  if(res.SubString(1,6)!=encoder.decode("ADD1E12F30E6") /*"Succes"*/)
    throw Exception(res);
}

#pragma package(smart_init)


AnsiString cChatServer::ChallengeList()
{
  return Query(encoder.decode("8B3D3E08EA518598F1AE5DD11E") /*"user_list.php"*/,encoder.decode("9B20C6FADE85D0") /*"except="*/+userid);
}
