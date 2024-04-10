//---------------------------------------------------------------------------

#ifndef utils_testingH
#define utils_testingH
//---------------------------------------------------------------------------

bool command(AnsiString param) {
	for(int i=1;i<=ParamCount();i++) {
		if(ParamStr(i).LowerCase()==param.LowerCase())
		  return true;
	}
	return false;
}

AnsiString param(AnsiString param) {
	for(int i=1;i<=ParamCount();i++) {
		if(ParamStr(i).LowerCase()==param.LowerCase())
		  return ParamStr(i+1);
	}
	return "";
}

#endif
