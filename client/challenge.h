//---------------------------------------------------------------------------

#ifndef challengeH
#define challengeH
//---------------------------------------------------------------------------
#include "chatserver.h"
#include "myinternet.h"
struct sRacerChars {
  double BracketTime;
  double RaceTime;
  double ReactionTime;
  bool  FalseStart;
  bool BreakOut() { return (ReactionTime+RaceTime)<BracketTime; };
  AnsiString RaceTimeStr() { return (RaceTime ==-1 || FalseStart) ? AnsiString("-") : (AnsiString)FloatToStrF(RaceTime,ffFixed,7,3); };
  AnsiString BracketTimeStr() { return BracketTime==0 ? AnsiString("-") : (AnsiString)FloatToStrF(BracketTime,ffFixed,7,3); };
  AnsiString ReactionTimeStr() { return FalseStart ? AnsiString("-") : (AnsiString)FloatToStrF(ReactionTime,ffFixed,7,3); };
  AnsiString ResultStr(bool Bracket) {
    AnsiString s;
    s.sprintf("Reaction: %s\nRace time: %s\n%s%s%s\n",
    ReactionTimeStr(),
      RaceTimeStr(),
      Bracket ? ("Bracket time: "+BracketTimeStr()+"\n").c_str() : "",
      FalseStart ? "Foul\n" : "",
      Bracket && BreakOut() ? "Breakout\n" : "");
    return s;

  }
};

class cChallenge {
public:
  cChatServer* s;
  bool StartedAndNotRecorded;
  bool withComputer;
  bool withRacer;
  bool forFun;
  bool forMoney;
  bool forCar; // nitto pink slip
  bool Bracket;
  bool HeadsUp;
  bool Answered;
  bool TeamRace;
  bool TeamRaceInitiators;
  bool TeamRaceAnswerers;
  bool Practice;
  float Wager;
  bool Draw;
  bool YouWin;
  bool YouLose;
  AnsiString message;
  AnsiString challengeid;
  sRacerChars Car;
  sRacerChars Opponent;
  AnsiString withRacerID;
  AnsiString usercarid;
  AnsiString answerusercarid;
  AnsiString carid;
  AnsiString answercarid;
  // ANSWERING
  bool Answering;
  cChallenge() { StartedAndNotRecorded = false; }
  void Reset();
  bool Create_Start();
  void Create_End();
  void SetBracketTime(AnsiString val);
  void SaveWinComputer(AnsiString Win);
  void StartAnswerTo(AnsiString challengeid);
  void SaveTeamResults();
  void GetRacerIDbyNick(AnsiString nick);
  bool GetInfo(AnsiString challengeid);
  // type - "answer", "init"
  void TeamRaceGetOpponent(AnsiString cid,AnsiString type);
  AnsiString TopResults();
  void SaveAnswer(AnsiString answerwin);
  void DeclineChallenge(AnsiString challengeid);
  void HideOutgoing(AnsiString challengeid);
  void HideIncoming(AnsiString challengeid);
  AnsiString RequestOutgoing();
  AnsiString RequestIncoming();
  AnsiString GetInfoString();

};

extern cChallenge Challenge;


#endif
