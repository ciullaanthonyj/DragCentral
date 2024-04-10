//---------------------------------------------------------------------------

#ifndef cencoderH
#define cencoderH
//---------------------------------------------------------------------------

class cEncoder {
public:
  unsigned short Key;
  cEncoder() {
   Key = 0xfea1;
  }
  AnsiString encode(AnsiString value);
  AnsiString decode(AnsiString value);
};

extern cEncoder encoder;
class SecString {
private:
  AnsiString encoded;
  AnsiString set(AnsiString s) {
	encoded = encoder.encode(s);
  }
  AnsiString get() {
	return encoder.decode(encoded);
  }
public:
  SecString() {
	set("");
  }
  SecString(const char* c) {
	  AnsiString s = c;
	  set(s);
  }
  operator=(AnsiString s) {
	  set(s);
  }
  operator=(char* c) {
	  AnsiString s = c;
	  set(s);
  }
  operator=(const char* c) {
	  AnsiString s = c;
	  set(s);
  }

  bool operator==(AnsiString s) {
	  return (s == get());
  }
  operator AnsiString() {
      return get();
  }
  AnsiString val() {
	return get();
  }
};

// class for not storing Strings In Memory
class EncString {
public:
  AnsiString intstorage;
  // copy constructor
  EncString() {};
  EncString(char* c) {
    AnsiString temp = c;
    intstorage = encoder.encode(temp);
  }
  void enc(AnsiString source) {
    intstorage = encoder.encode(source);
  }
  AnsiString dec() {
    return encoder.decode(intstorage);
  }
  EncString& operator=(char* c) {
    AnsiString temp = c;
    intstorage = encoder.encode(temp);
    return *this;
  }

  bool operator==(EncString& other) {
    return other.intstorage==intstorage;
  }
  AnsiString operator+(AnsiString other) {
     return dec()+other;
  }
};

#endif
