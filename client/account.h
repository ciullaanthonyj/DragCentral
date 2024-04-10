//---------------------------------------------------------------------------

#ifndef accountH
#define accountH
//---------------------------------------------------------------------------
#include "chatserver.h";
#include "myflash.h"
#define BADGE_100K 128
#define BADGE_MEMBER 4
#define BADGE_MODER 8
#define BADGE_ADMIN 16
#define BADGE_SUPERMOD 32
#define BADGE_RT 64
#define BADGE_100K 128

class cAccount {
public:
  cChatServer* srv;
  AnsiString wins;
  AnsiString loses;
  AnsiString GetBalance();
  AnsiString expires;
  AnsiString rank;
  int Badges;     
  bool noprivates;
  void GetStatistics(AnsiString userid);
  void GetMembershipState();
  void GetSettings();
  void GetRank(AnsiString userid);
  void SetNoPrivates(bool value);
  void GetBadges(AnsiString userid);
  void SetFlashBadges(TShockwaveFlash* fl);
};

extern cAccount Account;

#endif
