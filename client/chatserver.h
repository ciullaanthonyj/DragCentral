//---------------------------------------------------------------------------

#ifndef chatserverH
#define chatserverH
//---------------------------------------------------------------------------
#include "myinternet.h"
#include "cencoder.h"
#include "myprofiler.h"
#include "cSecureString.h"



#define SUPERMOD_FLAG 32
#define ADMIN_FLAG 16
#define MODER_FLAG 8
#define MEMBER_FLAG 4
#define MALE_FLAG 2
#define FEMALE_FLAG 1

// THIS MUST ENCAPSULATE ALL TRANSACTIONS WITH PHP SERVER SCRIPTS

// ChatThread is sending constantly requests on messages update
// calls master method of update chat lines and people in room
// people in room when recieved "%QUIT%" or "%ENTER%" messages
class TChatThread;

// CHAT INTERFACE
class cChatServer:public cServer {
public:
  bool logedin;
  SecString nick;
  AnsiString room;
  bool moder;
  bool member;
  TChatThread* chat;
  AnsiString userid;
  cChatServer();
  ~cChatServer();
  AnsiString CreateAccount(AnsiString nick,AnsiString pass, AnsiString email, bool male, bool female,
	AnsiString carid);
  bool Login(AnsiString nick, AnsiString pass, AnsiString version);
  void IgnoreUser(AnsiString nick);
  void UnignoreUser(AnsiString nick);
  void SendPass(AnsiString email);
  void Logout();

  // this must be chat methods
  // they are here because wanted to put it in different thread then chat new message requesting
  void QuitChat();
  void EnterRoom(AnsiString roomid, AnsiString pass="");
  void SendMessage(AnsiString msg, AnsiString tonick); // if no tonick then to room
  void CreateRoom(AnsiString room,AnsiString pass);
  void ChangePass(AnsiString nowpass, AnsiString newpass);
  AnsiString RoomList();

  // MODERATING
  void Kick(AnsiString userid, AnsiString reason);
  void ChatBan(AnsiString userid, AnsiString reason, int Length);
  void GameBan(AnsiString userid, AnsiString reason, int Length);
  void Unban(AnsiString nick);
        AnsiString ChallengeList();
};

struct sRecieveMessage {
		SecString nick;
        AnsiString text;
        bool priv;
        AnsiString tonick;
        bool sysmessage; // quit enter kick ban
        int TypeFlags;
        bool admin;
        bool moder;
        bool member;
        bool male;
		bool female;
		bool supermod;
		bool youwerekicked;
		bool youwerechatbanned;
		bool youweregamebanned;
		AnsiString reason;
		void reset() {
		  youwerekicked = false;
		  youwerechatbanned = false;
          youweregamebanned = false;
          sysmessage = false;
        }
};

class TChatThread : public TThread
{
private:
        AnsiString showmess;
        void __fastcall myShowMessage();
        HANDLE ChatMessageMutex;
protected:
        void __fastcall Execute();
        void __fastcall ChatRequests();
        void __fastcall GetSysMessages();
        void __fastcall OtherQueries_process();
        TStringList* chatmess;
        TStringList* chattonick; // similar to chatmess
        void __fastcall ShowRecievedMessage();
        void __fastcall CallOnSysMessages();
        TStringList* OtherQueries;
        HANDLE OtherQueryMutex;
        sRecieveMessage msg;
public:
        HANDLE RoomMutex;
        __fastcall TChatThread(bool CreateSuspended);
        __fastcall ~TChatThread();
        cChatServer* Owner;
        int LastMess;
        cSecureString nick;
        cTimer Timer;
        bool ChatActive;
        int SleepInterval; // interval of requerying msec
        void ChatMessageQue(AnsiString mess,AnsiString tonick);
        void OtherQueryAddQue(AnsiString query);
        // configuration
        bool ShowMessageTime;
        bool ShowQuitEnter;

        TNotifyEvent OnRecieveMessage; // Sender is (RecieveMessage*)
        AnsiString SysMessages;
        TNotifyEvent OnSysMessages;
        AnsiString PeopleInRoom;
        TNotifyEvent OnRoomPeopleUpdate;

        AnsiString room;
        TNotifyEvent OnChangeRoom;


        void __fastcall RoomPeopleUpdate();
        bool RefreshRoom;
        bool Done;
};
AnsiString killbadchars(AnsiString in);

#endif
