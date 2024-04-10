//---------------------------------------------------------------------------

#ifndef carsH
#define carsH
//---------------------------------------------------------------------------
#include "chatserver.h"
#include "myflash.h"
#include "form_trace.h"
// this is abstraction from Server and Flash
// probably flash must be in separate unit but it will be more complex structure
#define POUNDKG 0.4536
class cCars {
public:
  cChatServer* s;                     
  AnsiString selectedcarid;
  AnsiString selectedcarname; // type
  AnsiString usercarid; // SELECTED
  AnsiString freeleft; // FREE CARS LEFT
  int addhp;
  int reduceslip;
  int maxspeednum;
  AnsiString carid; // BY  GetUserCarInfo
  AnsiString carname; // BY GetUserCarInfo
  float EngineDamage;
  int EngineDamageRepairCost;
  struct sNitro {
    int hp;
    double left;
    int refillcost;
  } Nitro;
  struct sCharger {
    bool turbocharger;
    bool supercharger;
  } Charger;
  struct sMagicGear {
    bool Installed;
    float g1;
    float g2;
    float g3;
	float g4;
    float g5;
    float g6;
  } MagicGear;

  int selectedweight;
  int selectedhorses;
  cCars() { selectedcarid = S_0; };

  AnsiString TopCars() {
    return s->Query(encoder.decode("8AC2D8BF8A1E91A4F78C75") /*"toplist.php"*/,encoder.decode("8DE7387B6BA69FC4204F6B40") /*"show=topcars"*/);
  }

  AnsiString GetList() {
    return
      s->Query(encoder.decode("9DF80C17EF8B893A") /*"cars.php"*/,encoder.decode("9972BE02FAFE4C0FBA") /*"getlist=1"*/);
  }

  void SaveNitrousAndDamage() {
       s->chat->OtherQueryAddQue(
      encoder.decode("9DF80C17EF8B893AA6") /*"cars.php?"*/+
      encoder.decode("D8FE0D1D292E5BDEE53EAEDCEC9C1FF33396929FBF069C2E5498F8640F136A83") /*"&act=setnitrousanddamage&userid="*/+s->userid+
      encoder.decode("D8EA92A7419A26775C3C15") /*"&usercarid="*/+usercarid+
      encoder.decode("D8F12CAC5DD6E34DBA142EFE24") /*"&nitrousleft="*/+FloatToStrF(Nitro.left,ffFixed,5,2)+
      encoder.decode("D8FBEDEDE8343754") /*"&damage="*/+FloatToStrF(EngineDamage,ffFixed,5,2));
  }
  void SaveMagicGear(AnsiString usercarid) {

    AnsiString gears =
    encoder.decode("C8AF") /*"6:"*/+FloatToStrF(MagicGear.g1,ffFixed,10,3)+encoder.decode("C4") /*":"*/
     +FloatToStrF(MagicGear.g2,ffFixed,10,3)+encoder.decode("C4") /*":"*/
     +FloatToStrF(MagicGear.g3,ffFixed,10,3)+encoder.decode("C4") /*":"*/
     +FloatToStrF(MagicGear.g4,ffFixed,10,3)+encoder.decode("C4") /*":"*/
     +FloatToStrF(MagicGear.g5,ffFixed,10,3)+encoder.decode("C4") /*":"*/
     +FloatToStrF(MagicGear.g6,ffFixed,10,3)+encoder.decode("C4") /*":"*/
     ;
    AnsiString res = s->Query(encoder.decode("9DF80C17EF8B893A") /*"cars.php"*/,
                     encoder.decode("9FB8D1C415B51CA8BBAC4FC5BA2E6857A4010EEFDDC82514C48091") /*"act=setmagicgear&usercarid="*/+usercarid+
					 encoder.decode("D8F8CF579317E7") /*"&gears="*/+gears
                     );
  }

  void GetUserCarInfo(AnsiString usercarid) {
    AnsiString res = s->Query(encoder.decode("9DF80C17EF8B893A") /*"cars.php"*/,
                     encoder.decode("9FB8D1C40126E5EB7CF02427B09C18C66D6129AC7E667A99FE") /*"act=getcarinfo&usercarid="*/+usercarid
                     );
    if(s->Stack.Stacking) return;
    cStringTable tbl(res);
    if(tbl.GetLine()) {
       carid = tbl++;
       carname = tbl++;
    } else
      throw Exception(res);
  }

  void GetSelected() {
    AnsiString res = s->Query(encoder.decode("9DF80C17EF8B893A") /*"cars.php"*/,
      encoder.decode("8B3D3E08DC10C8") /*"userid="*/+s->userid+encoder.decode("D8FE0D1D293AA3C17606FE093172C728") /*"&act=getselected"*/);
    if(s->Stack.Stacking) return;
    cStringTable tbl(res);
    if(!tbl.GetLine())
      throw Exception(res);
    selectedcarid = tbl++;
    selectedweight = StrToInt(tbl++)*POUNDKG;
    selectedhorses = StrToInt(tbl++);
    usercarid = tbl++;
    selectedcarname = tbl++;
  }

  void GetFreeLeft() {
    AnsiString res = s->Query(encoder.decode("9DF80C17EF8B893A") /*"cars.php"*/,
      encoder.decode("8B3D3E08DC10C8") /*"userid="*/+s->userid+encoder.decode("D8FE0D1D293AA3C163059870B8465DFA") /*"&act=getfreeleft"*/);
    if(s->Stack.Stacking) return;
    cStringTable tbl(res);
    if(!tbl.GetLine())
      throw Exception(res);
    freeleft = tbl++;
  }



  void LoadFlash_obsolete(TShockwaveFlash* fl,AnsiString zoom=encoder.decode("CFC9F6") /*"100"*/) {
	int savewidth = fl->Width;
	int saveheight = fl->Height;
	fl->LoadMovie(0,wc(ExtractFilePath(Application->ExeName)+encoder.decode("A2E1B14FB36E23B6FDBFFE50CD8FF2E1A5D2A6CD") /*"\\flash\\carview.swf"*/));
	flsetvar(fl,encoder.decode("A16EE91D493829FFC14B") /*"_root.zoom"*/,zoom);
	flsetvar(fl,"backim._x",IntToStr(-fl->Left),false);
	flsetvar(fl,"backim._y",IntToStr(-fl->Top),false);
  }

  void SelectCar_obsolete(TShockwaveFlash* fl,AnsiString carid) {
	 fl->SetVariable(L"_root.carnum",wc(carid));
  }
  void RefillNitrous(AnsiString usercarid) {
	AnsiString res = s->Query(encoder.decode("9DF80C17EF8B893A") /*"cars.php"*/,
					 encoder.decode("8C8AB2581636AD15EB7C57DB541A5D0D2247DF8AA57CD2") /*"refillnitrous=1&userid="*/+s->userid+
					 encoder.decode("D8EA92A7419A26775C3C15") /*"&usercarid="*/+usercarid);
	if(s->Stack.Stacking) return;
	if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
	  throw Exception(res);

  }

  void RepairEngine(AnsiString usercarid) {
	AnsiString res = s->Query(encoder.decode("9DF80C17EF8B893A") /*"cars.php"*/,
					 encoder.decode("9FB8D1C4144A6C0316C45158AE9180DF6FA7A5255417F2FC") /*"act=repairengine&userid="*/+s->userid+
					 encoder.decode("D8EA92A7419A26775C3C15") /*"&usercarid="*/+usercarid);
	if(s->Stack.Stacking) return;
	if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
	  throw Exception(res);

  }

  void ResetCar(AnsiString usercarid) {
    AnsiString res = s->Query(encoder.decode("9DF80C17EF8B893A") /*"cars.php"*/,
                     encoder.decode("9FB8D1C4144A6F2698FB9C7B5ED0338119856091") /*"act=resetcar&userid="*/+s->userid+
                     encoder.decode("D8EA92A7419A26775C3C15") /*"&usercarid="*/+usercarid);
    if(s->Stack.Stacking) return;
    if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
      throw Exception(res);
  }

  void BuyCar(AnsiString userid,AnsiString carid) {
    AnsiString res = s->Query(encoder.decode("9DF80C17EF8B893A") /*"cars.php"*/,
                     encoder.decode("9C8CEBF7A35C911DD79BAB2630D8FC8A") /*"buycar=1&userid="*/+userid+
                     encoder.decode("D8FC4D30619851") /*"&carid="*/+carid);
    if(s->Stack.Stacking) return;
    if(!Signature(encoder.decode("ADD1E12F30E6") /*"Succes"*/,res))
      throw Exception(res);
  }
  AnsiString GetUserCars(AnsiString userid, AnsiString unlocked) {
    return
      s->Query(encoder.decode("9DF80C17EF8B893A") /*"cars.php"*/,
      encoder.decode("9FB8D1C40126E5FD1B141533A5FAF1D6D9FBFF8C9BFD213637") /*"act=getusercars&unlocked="*/+unlocked+encoder.decode("D8EA92A7419065C2") /*"&userid="*/+userid);
  }
  void SellCar(AnsiString userid,AnsiString usercarid) {
    AnsiString res = s->Query(encoder.decode("9DF80C17EF8B893A") /*"cars.php"*/,
      encoder.decode("9FB8D1C415B504D9BDD3449CAB3AA854D3FBDC") /*"act=sellcar&userid="*/+userid+
      encoder.decode("D8EA92A7419A26775C3C15") /*"&usercarid="*/+usercarid);
    if(s->Stack.Stacking) return;
    if(!Signature(encoder.decode("ADD1E12F30E65E") /*"Success"*/,res))
      throw Exception(res);
  }
  AnsiString RegisterList() {
    return
      s->Query(encoder.decode("9DF80C17EF8B893A") /*"cars.php"*/,encoder.decode("8C8AB3B8AC2EADBF498F0314803C") /*"registerlist=1"*/);
  }
  void SelectCar(AnsiString userid,AnsiString usercarid) {
    AnsiString res=
    s->Query(encoder.decode("9DF80C17EF8B893A") /*"cars.php"*/,encoder.decode("8DEA55D38D5D2A359FA40E20D51205DAC9C3C3") /*"selectcar=1&userid="*/+userid+
      encoder.decode("D8EA92A7419A26775C3C15") /*"&usercarid="*/+usercarid);
    if(s->Stack.Stacking) return;
    if(!Signature(encoder.decode("ADD1E12F30E65E") /*"Success"*/,res))
      throw Exception(res);
  }

  int GetUpgradeValues_Weight;
  int GetUpgradeValues_Horses;

  void GetUpgradeValues(AnsiString usercarid) {
     AnsiString res =
       s->Query(encoder.decode("9DF80C17EF8B893A") /*"cars.php"*/,
         encoder.decode("9FB8D1C40126E5FD18E885CA5CC33902CD0E67EF7EF8D2CA301873CB5BCF12") /*"act=getupgradevalues&usercarid="*/+usercarid+
		 encoder.decode("D8EA92A7419065C2") /*"&userid="*/+s->userid);
	if(s->Stack.Stacking) return;
	AnsiString carvalues = res;
	TRACE(carvalues);
    addhp = 0;
    reduceslip = 0;
    Nitro.hp = 0;
    cStringTable tbl(res);
    if(tbl.GetLine()) {
       addhp = StrToInt(tbl++);
       reduceslip = StrToInt(tbl++);
       Nitro.hp = StrToInt(tbl++);
       Nitro.left = StrToFloat(tbl++);
       Nitro.refillcost = StrToInt(tbl++);
	   Charger.turbocharger = tbl++==S_1;
	   Charger.supercharger = tbl++==S_1;
	   MagicGear.Installed = tbl++==S_1;
       cStringTable gears(
         AnsiReplaceStr(tbl++,encoder.decode("C4") /*":"*/,encoder.decode("82") /*"|"*/));
       if(gears.GetLine()) {
         MagicGear.g1 = StrToFloat(gears++);
         MagicGear.g2 = StrToFloat(gears++);
         MagicGear.g3 = StrToFloat(gears++);
         MagicGear.g4 = StrToFloat(gears++);
         MagicGear.g5 = StrToFloat(gears++);
         if(gears.fieldscount>5)
           MagicGear.g6 = StrToFloat(gears++);
         else
           MagicGear.g6 = 0.763;
       }
       maxspeednum = StrToInt(tbl++);
	   EngineDamage = StrToFloat(tbl++);
	   EngineDamageRepairCost = StrToInt(tbl++);

	   GetUpgradeValues_Weight = StrToInt(tbl++)*POUNDKG;
	   GetUpgradeValues_Horses = StrToInt(tbl++);

    } else
      throw Exception(res);
  }



};

extern cCars Cars;

#endif
