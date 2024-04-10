//---------------------------------------------------------------------------

#include <pch.h>
#pragma hdrstop
#include "myinternet.h"

//---------------------------------------------------------------------------

#define  C1 42815
#define  C2 32732

AnsiString Encrypt(AnsiString in, unsigned short Key)
{
  AnsiString out;
  out=in;
  int l = in.Length();
  for(int i=1;i<=l;i++) {
	out[i] = in[i] ^ (Key >> 8);
	Key = (Key+(unsigned char)(out[i])) * C1 + C2;
  }
  return out;
}

AnsiString Agent1 = "r";
AnsiString Agent2 = "p";
class cWSALink {
  WSADATA wsaData;
public:
  cWSALink() {
    int wsaret=WSAStartup(0x101,&wsaData);
    if(wsaret) throw Exception("WSAStartup error.");
  }
  ~cWSALink() {
    WSACleanup();
  }
};

void cServer::Connect() {
  if (Connected) return;

if(UseSocket) {
  apache->RemotePort = 80;
  apache->RemoteHost = ServerName;
  apache->Active = true;
  AnsiString Header = "GET "+Dir+"phpsocket.php HTTP/1.0\r\n"
              "User-Agent: rp";
  Header += "\r\n";
  Header +=   "Host: "+ServerName+"\r\n"
              "\r\n"
               ;
  apache->SendBuf(Header.c_str(),Header.Length(),0);
  AnsiString out = "";
  char buf[1024];
  while(true) {
    int res = apache->ReceiveBuf(buf,sizeof(buf),0);
    if(res==0 || res==-1) break;
    buf[res] = 0;
    out = out + buf;
    if(out.Pos("PR")!=0) break;
  }

  out = out.SubString(out.Pos("\r\n\r\nPR")+6,out.Length());
  AnsiString OpenedPort;
  OpenedPort = out;
  apache->Active = false;
  php->Active = false;
  php->RemoteHost = ServerName;
  php->RemotePort = StrToInt(OpenedPort);
  php->Active = true;
}

if(!UseSocket)
{
  cWSALink Link;

  char* servername = ServerName.c_str();
//  int wsaret=WSAStartup(0x101,&wsaData);
//  if(wsaret) throw Exception("WSAStartup error.");
  //  printf("Connecting to server");

  hp=gethostbyname(servername);             

  if(hp==NULL) {
      closesocket(conn);
    throw Exception("gethostbyname error.");
  }

  ZeroMemory(&server,sizeof(server));
  server.sin_addr.s_addr=*((unsigned long*)hp->h_addr);
  server.sin_family=AF_INET;
  server.sin_port=htons(80);
}

  Mutex = CreateMutex(NULL,false,NULL);
  Connected = true;

}

bool Signature(AnsiString sig,AnsiString s) {
  return s.SubString(1,sig.Length())==sig;
};

AnsiString LastSocketError() {

int e = WSAGetLastError();
AnsiString res = "";

#define er(e,mess) case e: res = mess; break;
  switch(e) {

er(WSANOTINITIALISED,"A successful WSAStartup must occur before using this function.");
er(WSAENETDOWN,"The network subsystem has failed.");
er(WSAEFAULT,"The buf argument is not totally contained in a valid part of the user address space.");
er(WSAENOTCONN,"The socket is not connected.");
er(WSAEINTR,"The (blocking) call was canceled through WSACancelBlockingCall.");
er(WSAEINPROGRESS,"A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.");
er(WSAENETRESET,"The connection has been broken due to the remote host resetting.");
er(WSAENOTSOCK,"The descriptor is not a socket.");
er(WSAEOPNOTSUPP,"MSG_OOB was specified, but the socket is not stream style such as type SOCK_STREAM, out-of-band data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations.");
er(WSAESHUTDOWN,"The socket has been shut down; it is not possible to recv on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH.");
er(WSAEWOULDBLOCK,"The socket is marked as nonblocking and the receive operation would block.");
er(WSAEMSGSIZE,"The message was too large to fit into the specified buffer and was truncated.");
er(WSAEINVAL,"The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative.");
er(WSAECONNABORTED,"The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.");
er(WSAETIMEDOUT,"The connection has been dropped because of a network failure or because the peer system failed to respond.");
er(WSAECONNRESET,"The virtual circuit was reset by the remote side executing a 'hard' or 'abortive' close. The application should close the socket as it is no longer usable. On a UDP datagram socket this error would indicate that a previous send operation resulted in an ICMP 'Port Unreachable' message.");
er(WSAEADDRINUSE,"The specified address is already in use.");
er(WSAEALREADY,"A nonblocking connect call is in progress on the specified socket.Note  In order to preserve backward compatibility, this error is reported as WSAEINVAL to Windows Sockets 1.1 applications that link to either WINSOCK.DLL or WSOCK32.DLL.");
er(WSAEADDRNOTAVAIL,"The specified address is not available from the local machine.");
er(WSAEAFNOSUPPORT,"Addresses in the specified family cannot be used with this socket.");
er(WSAECONNREFUSED,"The attempt to connect was forcefully rejected.");
er(WSAEISCONN,"The socket is already connected (connection-oriented sockets only).");
er(WSAENETUNREACH,"The network cannot be reached from this host at this time.");
er(WSAENOBUFS,"No buffer space is available. The socket cannot be connected.");
er(WSAEACCES,"Attempt to connect datagram socket to broadcast address failed because setsockopt SO_BROADCAST is not enabled.");


  }

  return res;
}
inline char toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}
AnsiString HexStr(AnsiString in) {
  AnsiString out = "";

  int p=1;
  int l=in.Length();
  while(true) {
    if(p>l) break;

    unsigned char c = in[p];
    out += toHex(c>>4);
    out += toHex(c%16);
    p++;
  }
  return out;
}

AnsiString Decrypt(AnsiString in, unsigned short Key)
{
  AnsiString out;
  out=in;
  int l = in.Length();
  for(int i=1;i<=l;i++) {
    out[i] = in[i] ^ (Key >> 8);
    Key = (Key+(unsigned char)(in[i])) * C1 + C2;
  }
  return out;
}



void cServer::StartStack() {
  Stack.Stacking = false;
  Stack.QueriesSent = false;
  Stack.ReturnIndex = 0;
}
bool cServer::StackQueries() {
  // starting stack
  if(!Stack.Stacking && !Stack.QueriesSent) {
    Stack.ThreadID = GetCurrentThreadId();
    Stack.Stacking = true;
    Stack.QueriesSent = false;
    Stack.queries->Clear();
    return true;
  }
  // ending stack
  if(Stack.Stacking && !Stack.QueriesSent) {
    AnsiString params = "";
    for(int i=0;i<Stack.queries->Count;i++) {
       params += Stack.queries->Strings[i];
    }
    Stack.Stacking = false;
    AnsiString res = Query("stack.php",params);

    Stack.answers->Clear();
    while(true) {
      int p=res.Pos("\r\n\r\n");
      if(p==0) {
        while(Stack.answers->Count<Stack.queries->Count)
          Stack.answers->Add(res);
        break;
      }

      AnsiString sub = res.SubString(1,p-1);
      Stack.answers->Add(sub);
      res = res.SubString(p+4,res.Length());
    }
    Stack.QueriesSent = true;
    return true;
  }

  if(!Stack.Stacking && Stack.QueriesSent) {
    Stack.QueriesSent = false;
    return false;
  }
}

AnsiString k="This code was originally written using my poor brains.";
AnsiString cServer::Query(AnsiString Script, AnsiString Params, bool WaitResponse) {
  if(!Connected) return "";

  AnsiString k2="Unknown error in server chat thread, please contact nearest police station.";

  int p = Script.Pos("?");
  if(p!=0) {
     Params = Script.SubString(p+1,Script.Length())+Params;
     Script = Script.SubString(1,p-1);
  }

  if(Script!="stack.php") {
    int tagval = 0;
    int l = k.Length();
    int l2 = k2.Length();
    for(int i=1;i<=Params.Length();i++) {
      unsigned char c = Params[i];
      c = k[c%l+1];
      c = k2[c%l2+1];
      tagval += c+i%2+(i+4)%7+i%5+c%2;
    }
    Params = Params+"&tag="+tagval;

    Params = Encrypt(Params,0xced1);
    Params = HexStr(Params);
  }

  if(GetCurrentThreadId()==Stack.ThreadID) {
     if(Stack.Stacking && !Stack.QueriesSent) {
       Stack.queries->Add("&q[]="+Script+"&p[]="+Params);
       return "";
     }
     if(!Stack.Stacking && Stack.QueriesSent) {
       return Stack.answers->Strings[Stack.ReturnIndex++];
     }
  }

  WaitForSingleObject(Mutex,INFINITE);
AnsiString s = "";

if(UseSocket) {
  AnsiString send = Script+"?"+Params+"\n";
tryagain:
  int sent = php->SendBuf(send.c_str(),send.Length(),0);
  if(sent==-1) {
    php->Active = false;
    php->Active = true;
    goto tryagain;
  }
  AnsiString out = "";
  while(true) {
    char Buf[1024];
    int res = php->ReceiveBuf(Buf,sizeof(Buf),0);
    if(res==0)
      break;
    if(res==-1)
      //reconnect
      goto tryagain;
    Buf[res]=0;
    out += Buf;
    if(out.Pos("\r\nEND\r\n")!=0)
      break;
  }
  out = out.SubString(1,out.Pos("\r\nEND\r\n")-1);
  s = out;
}

if(!UseSocket)
{
cWSALink Link;
  conn=socket(AF_INET,SOCK_STREAM,0);
  if(conn==INVALID_SOCKET) throw Exception("Invalid socket.");

  if(SOCKET_ERROR==connect(conn,(struct sockaddr*)&server,sizeof(server)))
  {
    ReleaseMutex(Mutex);
    throw Exception(LastSocketError().c_str());
  }
  AnsiString mess;
  AnsiString get = Dir+Script+"?"+Params;
  if(Params=="") get = Dir+Script;
  AnsiString Header = "GET %s HTTP/1.0\r\n"
              "User-Agent: ";
  Header += Agent1;
  Header += Agent2;
  Header += "\r\n";
  Header +=
//              "Content-Length: %d\r\n"
              "Host: %s\r\n"
//              "Content-Type: text/html\r\n"
              "\r\n"
//              "%s";
               ;
  mess.printf(Header.c_str()
             ,
             get.c_str(),
             /*0,*/ServerName.c_str());

  if(SOCKET_ERROR==send(conn,mess.c_str(),mess.Length(),0)) {
    closesocket(conn);
    ReleaseMutex(Mutex);
    throw Exception("::Query:Socket send Error");
  }
  if(WaitResponse) {
    int y;
    char buff[1024*10];
    int res = 0;
    while(true) {
      res = recv(conn,buff,sizeof(buff)-1,0);
      if(res==SOCKET_ERROR) {
        closesocket(conn);
        ReleaseMutex(Mutex);
        throw Exception(("::Query:sockethread:"+LastSocketError()).c_str());
      }
      if(res==0) {
        break;
      }
      buff[res] = 0;
      s = s + buff;
    }
    if(s.Length()!=0) {
    // double return I guess this is header end
      int p=s.Pos("\r\n\r\n");
      p+=4;
      s = s.SubString(p,s.Length());
    }
  }
  // IN WINDOWS XP ALWAYS RETURN -1
  int r=closesocket(conn);

} // WSA Cleanup Called here

  ReleaseMutex(Mutex);
  return s;
}

void cServer::Disconnect() {
  if(!Connected) return;
//  WSACleanup();
  CloseHandle(Mutex);
  Connected = false;
}



AnsiString HTTPDecode(AnsiString in) {
  AnsiString out = "";
  int p=1;
  int l=in.Length();
  while(true) {
    if(p>l) break;

    char c = in[p];
    if(c=='+') {
      out += ' ';
      p++;
    }
    else
    if(c=='%') {
      p++;
      if(isxdigit(in[p]) && isxdigit(in[p+1])) {
         AnsiString v = "0x";
         c = StrToInt(v+in[p]+in[p+1]);
         out += c;
         p +=2;
      }
    } else {
      out += c;
      p++;
    }
  }

  return out;

}


cServer::cServer() {
    Connected = false;
    Stack.Stacking = false;
    Stack.QueriesSent = false;
    UseSocket = false;
    Stack.queries = new TStringList;
    Stack.answers = new TStringList;
    apache = new TTcpClient(0);
	php = new TTcpClient(0);
  };

cServer::~cServer() {
  delete apache;
  delete php;
  delete Stack.queries;
  delete Stack.answers;
};

AnsiString HTTPParam(AnsiString in) {
  AnsiString out = "";

//  msg = AnsiReplaceStr(msg,"|","!");
//  msg = AnsiReplaceStr(msg," ","%20");
//  msg = AnsiReplaceStr(msg,"+","%2b");
//  msg = AnsiReplaceStr(msg,"\n","%20");
//  msg = AnsiReplaceStr(msg,"\r","");

  int p=1;
  int l=in.Length();
  while(true) {
    if(p>l) break;

    char c = in[p];
    if(isalnum(in[p]))
         out += c;
    else
    if(isspace(c))
	 out += '+';

    else {
	 out += '%';
         out += toHex(c>>4);
         out += toHex(c%16);
    }
    p++;
  }
  return out;
}

  cStringTable::cStringTable(AnsiString newdata) {
    data = newdata;
    if(data=="") return;
    p = 1;
    AnsiString fcount = Next();
    fieldscount = StrToIntDef(fcount,0);
    curfield = 1;
    firstline = true;
  };
  // THIS MUST BE USED ONLY IN while(GetLine()) { }
  bool cStringTable::GetLine() {
    if(data=="") return false;
    if(fieldscount==0) return false;
    if(!firstline) {
      while(curfield<=fieldscount)
         Next();
    }
    firstline = false;
    curfield = 1;
    return (p<data.Length());
  }
  AnsiString cStringTable::Next() {
    if(data=="") return "";
    AnsiString res;
    res = "";
    int l = data.Length();
    while(data[p]!='|') {
      res += data[p++];
      if(p>l) break;
    }
    p++;
    curfield++;
    return HTTPDecode(res);
  }
  AnsiString cStringTable::operator++(int) {
     return Next();
  }

#pragma package(smart_init)
