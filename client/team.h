//---------------------------------------------------------------------------

#ifndef teamH
#define teamH
#include "chatserver.h"
#include "myinternet.h"
class cTeamChallenge {
public:
  cChatServer* s;
  AnsiString teamid;
  AnsiString o_teamid;
  bool Bracket;
  bool HeadsUp;
  bool ForFun;
  bool ForMoney;
  int Wager;
  // u - user, c - car
  // challengers u1c1u2c2u3c3
  AnsiString challengers;
  void Create() {
	 AnsiString res = s->Query(encoder.decode("8AC80D0D3CD672A6AA69B2501617D5A8618D") /*"team_challenge.php"*/,
	   encoder.decode("9FB8D1C405B74960A39763A61288EC908669EA") /*"act=create&bracket="*/+booltostr(Bracket)+
	   encoder.decode("D8EBE43FC8B30FE1") /*"&teamid="*/+teamid+
	   encoder.decode("D8F0FA876203B75970CB") /*"&o_teamid="*/+o_teamid+
	   encoder.decode("D8F76C8E682F3106B1") /*"&headsup="*/+booltostr(HeadsUp)+
	   encoder.decode("D8F925C028CB2A00") /*"&forfun="*/+booltostr(ForFun)+
	   encoder.decode("D8F925C023F459345871") /*"&formoney="*/+booltostr(ForMoney)+
	   encoder.decode("D8E8C1968856B2") /*"&wager="*/+IntToStr(Wager)+
	   encoder.decode("D8FC44FCC8D9D0C226189A1927") /*"&challengers="*/+challengers);
	 if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
	   throw Exception(res);
  }
  // type = incoming/outgoing/active/completed
  AnsiString GetList(AnsiString teamid,AnsiString type) {
	 AnsiString res = s->Query(encoder.decode("8AC80D0D3CD672A6AA69B2501617D5A8618D") /*"team_challenge.php"*/,
	   encoder.decode("9FB8D1C40126E5E4D1B1331AEE8CB00140C74F") /*"act=getlist&teamid="*/+teamid+
	   encoder.decode("D8") /*"&"*/+type+encoder.decode("C343") /*"=1"*/);
	 return res;
  }
  AnsiString GetChallengers(AnsiString cid,AnsiString type) {
	 AnsiString res = s->Query(encoder.decode("8AC80D0D3CD672A6AA69B2501617D5A8618D") /*"team_challenge.php"*/,
	   encoder.decode("9FB8D1C40126E5EB757F22A996A52F0995CE73EA056FBB") /*"act=getchallengers&cid="*/+cid+encoder.decode("D8EBF89B1EA0") /*"&type="*/+type);
	 return res;
  }
  //answer encoder.decode("9FB8C6772FD6") /*"accept"*/,encoder.decode("9ADD755ADAAED9") /*"decline"*/
  void Answer(AnsiString cid, AnsiString teamid,AnsiString answer) {
	 AnsiString res = s->Query(encoder.decode("8AC80D0D3CD672A6AA69B2501617D5A8618D") /*"team_challenge.php"*/,
       encoder.decode("9FB8D1C40769F94B72FC179B1F2720B57508") /*"act=answer&teamid="*/+teamid+
       encoder.decode("D8FC4559B9") /*"&cid="*/+cid+
	   encoder.decode("D8") /*"&"*/+answer+encoder.decode("C343") /*"=1"*/);
	 if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
       throw Exception(res);
  }
};
extern cTeamChallenge TeamChallenge;

class cTeam {
public:
  cChatServer* s;
  AnsiString teamid;
  AnsiString teamname;
  AnsiString wins;
  AnsiString loses;
  AnsiString usercarid;
  AnsiString carid;
  bool inteam;
  bool owner;
  AnsiString BalanceEnc;

  cTeam() {
	teamid = "";
	BalanceEnc = "0"; 
  }

  AnsiString TeamMessage_GetAll() {
	AnsiString res = s->Query(encoder.decode("8AC80D0D3CD83E06463C1EC8691869A123") /*"team_messages.php"*/,
	  encoder.decode("9FB8D1C40126E5AEA285D0FFFDFDF5") /*"act=get&teamid="*/+teamid);
	if(s->Stack.Stacking) return "";
    return res;
  }
  void TeamMessage_Post(AnsiString message) {
    message = killbadchars(message);
    message = HTTPParam(message);
    AnsiString res = s->Query(encoder.decode("8AC80D0D3CD83E06463C1EC8691869A123") /*"team_messages.php"*/,
      encoder.decode("9FB8D1C4161F03CE0688A118746E138A") /*"act=post&userid="*/+s->userid+encoder.decode("D8EBE43FC8B30FE1") /*"&teamid="*/+teamid+
      encoder.decode("D8F283E4BE066235CC") /*"&message="*/+message+"");
  }
  void TeamMessage_Edit(AnsiString messid,AnsiString message) {
    message = killbadchars(message);
    message = HTTPParam(message);
	AnsiString res = s->Query(encoder.decode("8AC80D0D3CD83E06463C1EC8691869A123") /*"team_messages.php"*/,
      encoder.decode("9FB8D1C403E0FE7E76E814A1FA062DD8") /*"act=edit&userid="*/+s->userid+encoder.decode("D8EBE43FC8B30FE1") /*"&teamid="*/+teamid+
      encoder.decode("D8F283E4BE066235CC") /*"&message="*/+message+
      encoder.decode("D8F60CB7") /*"&id="*/+messid);
  }
  void TeamMessage_Delete(AnsiString messid) {
     AnsiString res = s->Query(encoder.decode("8AC80D0D3CD83E06463C1EC8691869A123") /*"team_messages.php"*/,
     encoder.decode("9FB8D1C402869A7EBC89F872654DAB") /*"act=del&userid="*/+s->userid+encoder.decode("D8EBE43FC8B30FE1") /*"&teamid="*/+teamid+encoder.decode("D8F60CB7") /*"&id="*/+messid);
  }
  AnsiString TopTeams() {
    return s->Query(encoder.decode("8AC2D8BF8A1E91A4F78C75") /*"toplist.php"*/,encoder.decode("8DE7387B6BA69FC437F9392C95") /*"show=topteams"*/);
  }

  void GetStats(AnsiString teamid) {
    AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
      encoder.decode("9FB8D1C40126E5FB53244F62592F7E89C7C2FA86") /*"act=getstats&teamid="*/+teamid);
    if(s->Stack.Stacking) return;
    cStringTable tbl(res);
    if(tbl.GetLine()) {
      wins = tbl++;
	  loses = tbl++;
    } else throw(Exception(res));
  }

  void CreateTeam(AnsiString name) {
	AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
      encoder.decode("9FB8D1C405B74960A39763B13491763B1CE1") /*"act=create&userid="*/+s->userid+encoder.decode("D8F124BE5887") /*"&name="*/+HTTPParam(name));

    cStringTable tbl(res);

    if(!tbl.GetLine())
      throw Exception(res);
    else {
      teamid = tbl++;
    }
  }

  AnsiString TeamMembers(AnsiString teamid) {
	 AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
       encoder.decode("9FB8D1C4128BF485DDF7E20B710AD576F06A2F2FB0E062") /*"act=teammembers&teamid="*/+teamid);
     if (s->Stack.Stacking) return "";
     return res;
  }
  AnsiString TeamList(AnsiString part) {
    AnsiString except = encoder.decode("D34D") /*"-1"*/;
    if(inteam)
      except = teamid;
    AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
      encoder.decode("9FB8D1C40126E5E4D1B1331AFFECD0D5D08324") /*"act=getlist&except="*/+except+
      encoder.decode("D8EF653E6FFD") /*"&part="*/+HTTPParam(part));

    return res;
  }

  void GetUserTeam(AnsiString userid) {
	AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
      encoder.decode("9FB8D1C40126E5FD1B1415243F53EEBB66C47BA7544C18") /*"act=getuserteam&userid="*/+userid);
    if(s->Stack.Stacking) return;
	cStringTable tbl(res);
    if(tbl.GetLine()) {
      teamid = tbl++;
      teamname = tbl++;
      owner = tbl++==S_1;
	  inteam = true;
      usercarid = tbl++;
      carid = tbl++;

    } else {
      teamid = "";
      teamname = "";
      inteam = false;
      owner = false;
    }
  }

  void QuitTeam() {
	AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
      encoder.decode("9FB8D1C417648D74897DE6188F651BBC3FEC1563") /*"act=quitteam&userid="*/+s->userid+encoder.decode("D8EBE43FC8B30FE1") /*"&teamid="*/+teamid);

    if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
      throw Exception(res);
    inteam = false;
    owner = false;
    teamid = "";
  }
  void RequestJoin(AnsiString teamid) {
    AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
      encoder.decode("9FB8D1C4144A6D7715621FBD40C5A33D973BEC81A37EDD") /*"act=requestjoin&userid="*/+s->userid+encoder.decode("D8EBE43FC8B30FE1") /*"&teamid="*/+teamid);
    if(s->Stack.Stacking) return;
    if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
      throw Exception(res);
  }

  AnsiString UserRequests() {
    AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
      encoder.decode("9FB8D1C413FD230284B0EC4EC825DB2B0DEA85E5F189AE4E") /*"act=userrequests&userid="*/+s->userid);
	return res;
  }

  AnsiString TeamRequests() {
    AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
	  encoder.decode("9FB8D1C4128BF485C2C58CA20A4CBEC7C289C5F32725497D") /*"act=teamrequests&teamid="*/+teamid);
    return res;
  }

  void AcceptJoin(AnsiString id) {
    AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
      encoder.decode("9FB8D1C4076404DC47289AAB7F27F44EC428") /*"act=acceptjoin&id="*/+id+encoder.decode("D8EBE43FC8B30FE1") /*"&teamid="*/+teamid);
    if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
      throw Exception(res);
  }

  void DeclineJoin(AnsiString id) {
    AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
	  encoder.decode("9FB8D1C402869559A24EBF11DCADCBB9BBCA62") /*"act=declinejoin&id="*/+id+encoder.decode("D8EBE43FC8B30FE1") /*"&teamid="*/+teamid);
	if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
	  throw Exception(res);
  }
  void JoinAccepted(AnsiString id) {
	AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
	  encoder.decode("9FB8D1C40C45D3FAB677521A5E21A79F6E786351") /*"act=joinaccepted&id="*/+id+encoder.decode("D8EA92A7419065C2") /*"&userid="*/+s->userid);
	if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
	  throw Exception(res);
  }
  void RemoveMember(AnsiString userid) {
	AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
	  encoder.decode("9FB8D1C4144A71EA4F4C70BDC57FC9D2658A673CC489BC20") /*"act=removemember&userid="*/+userid+encoder.decode("D8EBE43FC8B30FE1") /*"&teamid="*/+teamid);
	if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
	  throw Exception(res);
  }

  AnsiString GetBalance() {
	AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
	  encoder.decode("9FB8D1C40126E5EA1DD2A0C7C0C6A6E6A28F41470F77") /*"act=getbalance&teamid="*/+teamid);
	if(s->Stack.Stacking) return "";
	cStringTable tbl(res);
	if(!tbl.GetLine())
	  throw Exception(res);
	AnsiString balance = tbl++;
    BalanceEnc = encoder.encode(balance);
	return balance;
  }

  void Deposit(AnsiString value) {
	AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
	  encoder.decode("9FB8D1C4028686A396D797D6A6B328FDF0DA7E") /*"act=deposit&userid="*/+s->userid+encoder.decode("D8EBE43FC8B30FE1") /*"&teamid="*/+teamid+encoder.decode("D8E921DE472E4B") /*"&value="*/+value);
	if(s->Stack.Stacking) return;
	if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
	  throw Exception(res);
  }


  AnsiString GetTransactions() {
	AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
	  encoder.decode("9FB8D1C40126E5FCBA6F2C5C76348671599FE0BBA2E037D8E7941F") /*"act=gettransactions&teamid="*/+teamid);
	return res;
  }

  void Distribute(AnsiString userid,AnsiString value) {
    value = value;
    AnsiString res = s->Query(encoder.decode("8AC80D0D4D10E821") /*"team.php"*/,
      encoder.decode("9FB8D1C4028A0B48F0A4E7A5D155B49A9D4D1739F30B") /*"act=distribute&userid="*/+userid+encoder.decode("D8EBE43FC8B30FE1") /*"&teamid="*/+teamid+encoder.decode("D8E921DE472E4B") /*"&value="*/+value);
    if(s->Stack.Stacking) return;
    if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
      throw Exception(res);
  }



};

extern cTeam Team;


//---------------------------------------------------------------------------
#endif
