//---------------------------------------------------------------------------


#include <pch.h>
#pragma hdrstop
#include "account.h"
#include "csecstrings.h"


//---------------------------------------------------------------------------
cAccount Account;

#pragma package(smart_init)


void cAccount::SetNoPrivates(bool value) {
  AnsiString val=value?S_1:S_0;
  AnsiString res = srv->Query(encoder.decode("8B3D3E089BDAF433") /*"user.php"*/,
	encoder.decode("9FB8D1C415B51CABD32255EB848D2B94ACEE8EE4175B019937") /*"act=setnoprivates&userid="*/+srv->userid+encoder.decode("D8E921DE0F") /*"&val="*/+val);
}
void cAccount::GetSettings() {
  AnsiString res = srv->Query(encoder.decode("8B3D3E089BDAF433") /*"user.php"*/,
    encoder.decode("9FB8D1C40126E5FB42EE2C5334ABAE23E1B87496E90D97") /*"act=getsettings&userid="*/+srv->userid);
  if(srv->Stack.Stacking) return;
  cStringTable tbl(res);
  if(tbl.GetLine()) {
     noprivates = tbl++==encoder.decode("CF") /*"1"*/;
  }

}
void cAccount::GetMembershipState() {
  AnsiString res = srv->Query(encoder.decode("8B3D3E089BDAF433") /*"user.php"*/,
	encoder.decode("9FB8D1C40126E5E53C06AAFE5E6DA91B92031F6C209A9CAEC7E464F3AF02") /*"act=getmembershipstate&userid="*/+srv->userid);
  if(srv->Stack.Stacking) return;
  cStringTable tbl(res);
  if(tbl.GetLine()) {
	 expires = tbl++;
  }
}

void cAccount::SetFlashBadges(TShockwaveFlash* fl) {
  flsetvar(fl,"member._visible",int((Badges & BADGE_MEMBER)!=0),false);
  flsetvar(fl,"admin._visible",int((Badges & BADGE_ADMIN)!=0),false);
  flsetvar(fl,"moder._visible",int((Badges & BADGE_MODER)!=0),false);
  flsetvar(fl,"100k._visible",int((Badges & BADGE_100K)!=0),false);
  flsetvar(fl,"rt._visible",int((Badges & BADGE_RT)!=0),false);
}
void cAccount::GetBadges(AnsiString userid) {
  AnsiString res = srv->Query(encoder.decode("8B3D3E089BDAF433") /*"user.php"*/,
	"act=getbadges&userid="+userid);
  if(srv->Stack.Stacking) return;
  cStringTable tbl(res);
  if(tbl.GetLine()) {
	 Badges = StrToInt(tbl++);
  }
}

void cAccount::GetRank(AnsiString userid) {
  AnsiString res = srv->Query(encoder.decode("8B3D3E089BDAF433") /*"user.php"*/,
	encoder.decode("9FB8D1C40126E5FAE7954539AD0F087B345D90") /*"act=getrank&userid="*/+userid);
  if(srv->Stack.Stacking) return;
  cStringTable tbl(res);
  if(tbl.GetLine()) {
	 rank = tbl++;
  }

}

AnsiString cAccount::GetBalance()
{
  AnsiString res = srv->Query(encoder.decode("8B3D3E089BDAF433") /*"user.php"*/,
    encoder.decode("9FB8D1C40126E5EA1DD2A0C7C0C6A6E7149E0BF0714D") /*"act=getbalance&userid="*/+srv->userid);
  if(srv->Stack.Stacking) return "";
  cStringTable tbl(res);
  if(tbl.GetLine()) {
     return tbl++;
  }
  return S_0;
}

void cAccount::GetStatistics(AnsiString userid) {
  AnsiString res = srv->Query(encoder.decode("8B3D3E089BDAF433") /*"user.php"*/,
    encoder.decode("9FB8D1C40126E5FB53244F62592E0930DC584248") /*"act=getstats&userid="*/+userid);
  if(srv->Stack.Stacking) return;
  cStringTable tbl(res);
  wins = S_0;
  loses = S_0;
  if(tbl.GetLine()) {
     wins = tbl++;
     loses = tbl++;
  }
  return;
}

