//---------------------------------------------------------------------------


#include <pch.h>
#pragma hdrstop

#include "challenge.h"

//---------------------------------------------------------------------------
cChallenge Challenge;

#pragma package(smart_init)

void cChallenge::TeamRaceGetOpponent(AnsiString cid,AnsiString type) {
  AnsiString res = s->Query(
    encoder.decode("8AC80D0D3CD672A6AA69B2501617D5A8618D") /*"team_challenge.php"*/,
    encoder.decode("9FB8D1C40126E5FAE30A22D76DC378F70DF0CBCC139E") /*"act=getresults&userid="*/+s->userid+
    encoder.decode("D8FC4559B9") /*"&cid="*/+cid+
    encoder.decode("D8") /*"&"*/+type+encoder.decode("C343") /*"=1"*/);

  cStringTable tbl(res);
  if(tbl.GetLine()) {
     Opponent.ReactionTime = StrToFloat(tbl++);
     Opponent.RaceTime = StrToFloat(tbl++);
     Opponent.BracketTime = StrToFloat(tbl++);
     Opponent.FalseStart = StrToInt(tbl++);
  } else {
    throw Exception(res);
  }

}
void cChallenge::SaveTeamResults() {
  AnsiString AnswerInfo = "";
  if(Answering) {
     AnswerInfo = encoder.decode("D8FBFE4A9196") /*"&draw="*/+booltostr(Draw)+
       encoder.decode("D8E8C99851") /*"&win="*/+booltostr(YouWin);
  }

  AnsiString q =encoder.decode("8AC80D0D3CD672A6AA69B2501617D5A8618D5F") /*"team_challenge.php?"*/+
           encoder.decode("9FB8D1C415B193B47EB2A326F437F2B85A") /*"act=saverace&cid="*/+challengeid+
           encoder.decode("D8EA92A7419065C2") /*"&userid="*/+s->userid+
           encoder.decode("D8EDA6A74D4C4BF2FB8F555DCEB7") /*"&reactiontime="*/+FloatToStr(Car.ReactionTime)+
           encoder.decode("D8EDA2275812EFAA76C5") /*"&racetime="*/+FloatToStr(Car.RaceTime)+
           encoder.decode("D8FDBF9250EB7BA5F017D4D09F") /*"&brackettime="*/+FloatToStr(Car.BracketTime)+
           encoder.decode("D8F925C7DE34") /*"&foul="*/+FloatToStr(Car.FalseStart || Car.BreakOut())+
           encoder.decode("D8F606AF86E5") /*"&init="*/+booltostr(TeamRaceInitiators)+
           encoder.decode("D8FE00324BE37BEF") /*"&answer="*/+booltostr(TeamRaceAnswerers)+
           AnswerInfo;
  AnsiString res=s->Query(q,"");
  //s->chat->OtherQueryAddQue(q);
  StartedAndNotRecorded = false;
}

  bool cChallenge::Create_Start() {
    if(Practice) {
      StartedAndNotRecorded = true;
      return true;
    }
    if(withComputer) {
      cStringTable tbl(this->s->Query(encoder.decode("9DF17B982DE455CB3B06125EF0F3E4C8C06B7CB48B12") /*"challenge_withcomp.php"*/,encoder.decode("9FB8D1C415A4DCE8E19F60B875") /*"act=start&id="*/+s->userid+encoder.decode("D8E8C1968856B2") /*"&wager="*/+FloatToStr(Wager) ));
      if(tbl.GetLine()) {
        challengeid=tbl++;
        StartedAndNotRecorded = true;
        return true;
      }
    }

    if(TeamRace) {
      AnsiString res=
      s->Query(encoder.decode("8AC80D0D3CD672A6AA69B2501617D5A8618D") /*"team_challenge.php"*/,
        encoder.decode("9FB8D1C415A4DCE8E1CBC41141CAFE746BC9") /*"act=startrace&cid="*/+challengeid+
        encoder.decode("D8EA92A7419065C2") /*"&userid="*/+s->userid+
		encoder.decode("D8F606AF86E5") /*"&init="*/+booltostr(TeamRaceInitiators)+
        encoder.decode("D8FE00324BE37BEF") /*"&answer="*/+booltostr(TeamRaceAnswerers));
      if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
        return false;
      else {
        StartedAndNotRecorded = true;
        return true;
      }
    }

    if(withRacer && !Answering) {

        cStringTable tbl(this->s->Query(encoder.decode("9DF17B982DE455CB3B0606DD32C20FF1A074723E") /*"challenge_create.php"*/,
        encoder.decode("9FB8D1C415A4DCE8E19F7C1D3AC528E3FB") /*"act=start&userid="*/+s->userid+
        encoder.decode("D8EBEED05C558D3CC956") /*"&touserid="*/+withRacerID+
        encoder.decode("D8FDBF9250EB7BA5B9") /*"&bracket="*/+booltostr(Bracket)+
        encoder.decode("D8F925C028CB2A00") /*"&forfun="*/+booltostr(forFun)+
        encoder.decode("D8F925C023F459345871") /*"&formoney="*/+booltostr(forMoney)+
        encoder.decode("D8F925C02D8089C6") /*"&forcar="*/+booltostr(forCar)+
        encoder.decode("D8E8C1968856B2") /*"&wager="*/+FloatToStr(Wager)+
        encoder.decode("D8F76C8E682F3106B1") /*"&headsup="*/+booltostr(HeadsUp)+
        encoder.decode("D8EA92A7419A26775C3C15") /*"&usercarid="*/+usercarid+
		encoder.decode("D8FE00324BE37BA752788011F218654C26") /*"&answerusercarid="*/+answerusercarid+
		encoder.decode("D8F283E4BE066235CC") /*"&message="*/+HTTPParam(message)
        ));
        if(tbl.GetLine()) {
          challengeid=tbl++;
          StartedAndNotRecorded = true;
          return true;
        }
        return false;
    }

    if(withRacer && Answering) {
      this->s->Query(encoder.decode("9DF17B982DE455CB3B060400286D0645CDE00F97") /*"challenge_answer.php"*/,encoder.decode("9FB8D1C415A4DCE8E19F60B875") /*"act=start&id="*/+challengeid+encoder.decode("D8EA92A7419065") /*"&userid"*/+s->userid);
      StartedAndNotRecorded = true;
      return true;
    }

    return false;
  };
  void cChallenge::SetBracketTime(AnsiString val) {
    // here gotta be error control
    Car.BracketTime = StrToFloat(val);
  }

  void cChallenge::SaveWinComputer(AnsiString Win) {
    AnsiString win = Win;
    AnsiString q = "";
    q = q + encoder.decode("9DF17B982DE455CB3B06125EF0F3E4C8C06B7CB48B12CB957EFAEAAB70155067E13AE5") /*"challenge_withcomp.php?act=end&win="*/+win+encoder.decode("D8F60CB7") /*"&id="*/+s->userid+encoder.decode("D8FC4559B9") /*"&cid="*/+challengeid;
    AnsiString res=s->Query(q,"");
//    s->chat->OtherQueryAddQue(q);
    StartedAndNotRecorded = false;
  }
  bool cChallenge::GetInfo(AnsiString challengeid) {
     Challenge.Reset();
     this->challengeid = challengeid;
     cStringTable tbl(s->Query(encoder.decode("9DF17B982DE455CB3B060C1DAAB9C8B1F126") /*"challenge_info.php"*/,encoder.decode("9FB8D1C40126E5E1B8FD594275851A") /*"act=getinfo&id="*/+challengeid));
     if(tbl.GetLine()) {
       forFun = tbl++==S_1;
       Bracket = tbl++==S_1;
       Opponent.BracketTime = StrToFloat(tbl++);
       Opponent.RaceTime = StrToFloat(tbl++);
       Answered = (tbl++==S_1);
       forMoney = tbl++==S_1;
       HeadsUp = tbl++==S_1;
       Opponent.FalseStart = tbl++==S_1;
       Opponent.ReactionTime = StrToFloat(tbl++);
       Wager = StrToFloat(tbl++);
       Car.BracketTime = StrToFloat(tbl++);
       Car.RaceTime = StrToFloat(tbl++);
       Car.ReactionTime = StrToFloat(tbl++);
       Car.FalseStart = tbl++==S_1;
	   forCar = tbl++==S_1;
       return true;
     } else return false;

  }

  void cChallenge::DeclineChallenge(AnsiString challengeid) {
     AnsiString res = s->Query(encoder.decode("9DF17B982DE455CB3B060400286D0645CDE00F97") /*"challenge_answer.php"*/,
       encoder.decode("9FB8D1C402869559A24EBF5D0AAAE6") /*"act=decline&id="*/+challengeid+encoder.decode("D8EA92A7419065C2") /*"&userid="*/+s->userid);
     if(s->Stack.Stacking) return;
     if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
       throw Exception(res);
  }

  void cChallenge::HideIncoming(AnsiString challengeid) {
     AnsiString res = s->Query(encoder.decode("9DF17B982DE455CB3B060C1DAAB9C8B1F126") /*"challenge_info.php"*/,
       encoder.decode("9FB8D1C40E029B94D5D582720BB0BAC3A5FA5664") /*"act=hideincoming&id="*/+challengeid+encoder.decode("D8EA92A7419065C2") /*"&userid="*/+s->userid);
     if(s->Stack.Stacking) return;
     if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
       throw Exception(res);
  }

  void cChallenge::HideOutgoing(AnsiString challengeid) {
     AnsiString res = s->Query(encoder.decode("9DF17B982DE455CB3B060C1DAAB9C8B1F126") /*"challenge_info.php"*/,
       encoder.decode("9FB8D1C40E029B94D30F20975EDFE5767BF73605") /*"act=hideoutgoing&id="*/+challengeid+encoder.decode("D8EA92A7419065C2") /*"&userid="*/+s->userid);
     if(s->Stack.Stacking) return;
     if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
       throw Exception(res);
  }

  void cChallenge::GetRacerIDbyNick(AnsiString nick) {
     AnsiString res = s->Query(encoder.decode("9DF17B982DE455CB3B0606DD32C20FF1A074723E") /*"challenge_create.php"*/,
       encoder.decode("9FB8D1C40126E5FD1B141539EC22D10A1BAFD3") /*"act=getuserid&nick="*/+HTTPParam(nick));
     if(s->Stack.Stacking) return;
    cStringTable tbl(res);
    if(tbl.GetLine()) {
      this->withRacerID = tbl++;
    } else {
      throw Exception(res);
  }

  }

  void cChallenge::StartAnswerTo(AnsiString challengeid) {
     if (GetInfo(challengeid)) {
       if(Answered)
         throw Exception(encoder.decode("BDC679C808C9215DAD6C442D0B3EFF02DA69EA08B5AFE575EF9239") /*"Challenge already answered."*/);
       Answering = true;
       withRacer = true;
     }
  }
  void cChallenge::Create_End() {
    s->Query(
      encoder.decode("9DF17B982DE455CB3B0606DD32C20FF1A074723E") /*"challenge_create.php"*/,
      encoder.decode("9FB8D1C403EAB96D1DC7529D") /*"act=end&cid="*/+challengeid+
      encoder.decode("D8FDBF9250EB7BA5F017D4D09F") /*"&brackettime="*/+FloatToStr(Car.BracketTime)+
      encoder.decode("D8EDA2275812EFAA76C5") /*"&racetime="*/+FloatToStr(Car.RaceTime)+
      encoder.decode("D8F92B19016ADE9D7CE52BAA") /*"&falsestart="*/+booltostr(Car.FalseStart)+
      encoder.decode("D8EDA6A74D4C4BF2FB8F555DCEB7") /*"&reactiontime="*/+FloatToStr(Car.ReactionTime));
    if(s->Stack.Stacking) return;
    StartedAndNotRecorded = false;
  }

  void cChallenge::SaveAnswer(AnsiString answerwin) {
    AnsiString res = s->Query(encoder.decode("9DF17B982DE455CB3B060400286D0645CDE00F97") /*"challenge_answer.php"*/,
      encoder.decode("9FB8D1C403EAB96D178EBE") /*"act=end&id="*/+challengeid+
      encoder.decode("D8EA92A7419065C2") /*"&userid="*/+s->userid+
      encoder.decode("D8FDBF9250EB7BA5F017D4D09F") /*"&brackettime="*/+FloatToStr(Car.BracketTime)+
      encoder.decode("D8EDA2275812EFAA76C5") /*"&racetime="*/+FloatToStr(Car.RaceTime)+
      encoder.decode("D8F92B19016ADE9D7CE52BAA") /*"&falsestart="*/+booltostr(Car.FalseStart)+
      encoder.decode("D8E8C99851") /*"&win="*/+answerwin+
      encoder.decode("D8EDA6A74D4C4BF2FB8F555DCEB7") /*"&reactiontime="*/+FloatToStr(Car.ReactionTime));
    if(s->Stack.Stacking) return;
    StartedAndNotRecorded = false;
  }

  AnsiString cChallenge::TopResults() {
    return s->Query(encoder.decode("8AC2D8BF8A1E91A4F78C75") /*"toplist.php"*/,encoder.decode("8DE7387B6BA69FC431BC5002F4ABA4") /*"show=topresults"*/);
  }

  AnsiString cChallenge::RequestOutgoing() {
    return s->Query(encoder.decode("9DF17B982DE455CB3B0609F8B3D2D2A053AE") /*"challenge_list.php"*/,
    encoder.decode("8AD4848FCD7B52A8B3E06C8213F55B71") /*"type=out&userid="*/+s->userid
    );
  }

  AnsiString cChallenge::RequestIncoming() {
    return s->Query(encoder.decode("9DF17B982DE455CB3B0609F8B3D2D2A053AE") /*"challenge_list.php"*/,
    encoder.decode("8AD4848FCD7D073AA476ACDE1A1395") /*"type=in&userid="*/+s->userid
    );
  }

  AnsiString cChallenge::GetInfoString() {
    AnsiString res = "";

    res += encoder.decode("BDC679C808C9215DAD6C") /*"Challenge "*/;
    if(withComputer)
      res += encoder.decode("8AC5EF991E395F5882BA90E113") /*"the computer "*/;
    if(withRacer)
      res += encoder.decode("8AC5EF990F8DF0B20875") /*"the racer "*/;
    if(Bracket)
      res += encoder.decode("BC7F0299F1402C4E") /*"Bracket "*/;
    if(HeadsUp)
      res += encoder.decode("B62C3E98CBE6B889CE") /*"Heads Up "*/;
    if(forFun)
      res += encoder.decode("9819BBE2C54F34EC") /*"for fun "*/;
    if(forMoney)
      res += encoder.decode("9819BBE2CE8F8715E460") /*"for money "*/;
    return res;
  }

void cChallenge::Reset()
{
  Practice = false;
  withComputer = false;
  withRacer = false;
  forFun = false;
  forMoney = false;
  forCar = false;
  Bracket = false;
  HeadsUp = false;
  Answering = false;
  Car.FalseStart = false;
  Opponent.FalseStart = false;
  Wager = 0;
  Answered = false;
  TeamRace = false;
  TeamRaceInitiators = false;
  TeamRaceAnswerers = false;
  Draw = false;
  YouWin = false;
  YouLose = false;
}
