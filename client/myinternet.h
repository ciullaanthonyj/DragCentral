//---------------------------------------------------------------------------

#ifndef myinternetH
#define myinternetH
//---------------------------------------------------------------------------
#include <sockets.hpp>
#include "csecstrings.h"
#pragma hdrstop
AnsiString booltostr(bool val) {
 if(val) return S_1;
 else return S_0;
}
bool Signature(AnsiString sig,AnsiString s);
AnsiString HTTPParam(AnsiString msg); // replaces 'spaces' with %20 etc.
class cServer {
private:
  bool Connected;
  struct hostent *hp;
  struct sockaddr_in server;
  SOCKET conn;
  HANDLE Mutex; // query mutex for different threads
public:
  TTcpClient* apache;
  TTcpClient* php;
  struct cStackQueries {
    int ThreadID;
    bool  Stacking;
    bool  QueriesSent;
    TStringList* queries;
    TStringList* answers;
    int ReturnIndex;
  } Stack;
  bool UseSocket;  // set it true before connect (don't change on fly)
                   // phpsocket.php - must be on server (returning port that it start to listen)
  AnsiString ServerName;
  AnsiString Dir;
  cServer();
  ~cServer();
  void cServer::StartStack();
  bool StackQueries();
  void Connect();
  AnsiString Query(AnsiString Script, AnsiString Params, bool WaitResponse = true);
  void Disconnect();
};

class cStringTable {
private:
  AnsiString data;
  int p;
  int curfield;
  bool firstline;
public:
  int fieldscount;
  cStringTable(AnsiString newdata);
  // THIS MUST BE USED ONLY IN while(!LastLine()) { }
  bool GetLine();
  AnsiString Next();
  AnsiString operator++(int);
};
AnsiString Decrypt(AnsiString in, unsigned short Key);
AnsiString Encrypt(AnsiString in, unsigned short Key);


#endif
