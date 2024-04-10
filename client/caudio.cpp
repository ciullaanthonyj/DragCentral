//audio for games
// LoadSound(name,file)
// SetLoop(name,ofsstart,ofsend)
// PlaySound(name)


// cAudio, cMixer, cDynList, cSound, cRawData


#include <pch.h>
#pragma hdrstop

#include "cmp3stream.h"
#include "caudio.h"

struct sHdrRiff {
    char sigRiff[4]; // gotta be "RIFF"
    long lenRiffBlock;
    char sigWave[4]; // gotta be "WAVE"
    char sigFmt[4];  // gotta be "fmt "
    long lenFmtBlock;
};

struct sHdrData {
   char sigData[4];  // gotta be "data"
   long lenDataBlock;
};

typedef struct {
    WORD  wFormatTag;       // see below
    WORD  nChannels;        // see below
    DWORD nSamplesPerSec;   // sample rate, in samples per second
    DWORD nAvgBytesPerSec;  // see below
    WORD  nBlockAlign;      // see below
    WORD  wBitsPerSample;
} myWAVEFORMAT;

class cRawData {
  private:
    unsigned int filedataofs;
  public:
    unsigned int dataofs;
    int SampleFreq;
    int Channels;
    FILE *infile;
    char* raw;
    long lenRaw;
    void openWav(char* fileName);
    int readFile(void* Buf, int BufLen);
    void loadFromWav(char* fileName);
    void loadFromMP3(char* fileName);
    void saveToWav(char* fileName);
    void saveHeader(char* fileName);
    void cut(long posStart,long posEnd,long whole);
    int  getBytes(long Ofs,char* Buf, long BufLen);
    char BPS; // bytes per sample
    void assignProps(cRawData* src);
    void reserve(long newByteLen);
    void setDataOfs(long newOfs) {
      dataofs = newOfs;
      fseek(infile, filedataofs+newOfs,SEEK_SET);
    }

    char fmtChannels();
    int fmtSampleFreq();
    int writeBytes(long Ofs,char* Buf,long BufLen);
    cRawData();
    ~cRawData();
    sHdrRiff hdrRiff;
    char * sFmt;
    sHdrData hdrData;
  private:
};

char cRawData::fmtChannels() {
  myWAVEFORMAT* wfmt;
  if(sFmt==NULL)
    return 0;
  else {
    wfmt = (myWAVEFORMAT*) sFmt;
    return wfmt->nChannels;
  }
}

int cRawData::fmtSampleFreq() {
  myWAVEFORMAT* wfmt;
  if(sFmt==NULL)
    return 0;
  else {
    wfmt = (myWAVEFORMAT*) sFmt;
    return wfmt->nSamplesPerSec;
  }
}


int cRawData::readFile(void* Buf,int BufLen) {

  if(dataofs>=lenRaw) {
     return 0;
  }
  if(BufLen>lenRaw-dataofs) {
     BufLen = lenRaw - dataofs;
  }
  int res = fread(Buf,1,BufLen,infile);
  dataofs +=res;
  return res;

}

void cRawData::openWav(char* fileName) {

  infile=fopen(fileName,"rb");
  if (!infile) throw "::openWav";

    fread(&hdrRiff,sizeof(hdrRiff),1,infile);

    if(sFmt!=NULL) delete sFmt;
    sFmt = new char[hdrRiff.lenFmtBlock];
    fread(sFmt,hdrRiff.lenFmtBlock,1,infile);
    // looking for data block                          
    while(true) {
      fread(&hdrData,sizeof(hdrData),1,infile);
      // check if it is sndrec32 "data" block then quit parsing
      if(hdrData.sigData[0]=='d' && hdrData.sigData[1]=='a' && hdrData.sigData[2]=='t' && hdrData.sigData[3]=='a')
         break;
      if(0!=fseek(infile,hdrData.lenDataBlock,SEEK_CUR))
        throw "error reading wav file header";
    }

   if(raw!=NULL) delete raw;
   lenRaw = hdrData.lenDataBlock;

   fpos_t pos;
   fgetpos(infile, &pos);
   filedataofs = pos;
   dataofs = 0;
   SampleFreq = fmtSampleFreq();
   Channels = fmtChannels();

}

void cRawData::loadFromWav(char* fileName) {

  FILE *f;
  f=fopen(fileName,"rb");
  if (!f) throw "::loadFromWav";

    fread(&hdrRiff,sizeof(hdrRiff),1,f);

    if(sFmt!=NULL) delete sFmt;
    sFmt = new char[hdrRiff.lenFmtBlock];
    fread(sFmt,hdrRiff.lenFmtBlock,1,f);
    fread(&hdrData,sizeof(hdrData),1,f);

    if(raw!=NULL) delete raw;
    lenRaw = hdrData.lenDataBlock;
    raw = new char[lenRaw];
    fread(raw,1,lenRaw,f);

  fclose(f);

}

void cRawData::loadFromMP3(char* fileName) {
  cMP3Stream mp3;
  mp3.OpenMP3(fileName);
  lenRaw = mp3.mp3stream.uFrameNum * mp3.mp3stream.frameSize*4;
  raw = new char[lenRaw];
  mp3.GetBytes(raw,lenRaw);
}

void cRawData::cut(long posStart,long posEnd,long whole)
{
  char * rawNew;
  long lenRawNew;
  
  if (posStart>posEnd) throw "::cut posStart>posEnd";
  if (posEnd>whole) throw "::cut posEnd>whole";

  lenRawNew = lenRaw * (posEnd - posStart) / whole;
  lenRawNew = (lenRawNew / BPS) * BPS;
  
  long posStartAbs = lenRaw * posStart / whole;
  posStartAbs = (posStartAbs / BPS) * BPS;
  rawNew = new char[lenRawNew];
  for(long i=0;i<lenRawNew;i++)
    rawNew[i] = raw[posStartAbs + i];
  delete raw;
  raw = rawNew;
  lenRaw = lenRawNew;
  hdrData.lenDataBlock = lenRaw;
}

int cRawData::getBytes(long Ofs,char* Buf, long BufLen) {
   if (Ofs>=this->lenRaw)
     return 0;

   if ((Ofs+BufLen)>=this->lenRaw)
     BufLen = this->lenRaw-Ofs;

   Move((char*)this->raw+Ofs,Buf,BufLen);
   return BufLen;
}

cRawData::cRawData() {
  sFmt = NULL;
  raw  = NULL;
  BPS  = 4; // default stereo and 16 bit
  lenRaw = 0;
  infile = NULL;
}

int cRawData::writeBytes(long Ofs,char* Buf,long BufLen) {
  if(Ofs>=this->lenRaw)
    return 0;

   if ((Ofs+BufLen)>=this->lenRaw)
     BufLen = this->lenRaw-Ofs;

   Move(Buf,(char*)this->raw+Ofs,BufLen);
   return BufLen;

}
void cRawData::reserve(long newByteLen) {
  if (raw!=NULL) delete raw;
  raw = new char[newByteLen];
  lenRaw = newByteLen;
}

void cRawData::assignProps(cRawData* src) {
  if(sFmt!=NULL)
    delete sFmt;

  Move(&src->hdrRiff,&hdrRiff,sizeof(hdrRiff));
  Move(&src->hdrData,&hdrData,sizeof(hdrData));
  sFmt = new char[src->hdrRiff.lenFmtBlock];
  Move(src->sFmt,sFmt,hdrRiff.lenFmtBlock);

}
void cRawData::saveToWav(char* fileName)
{
  FILE *f;
  f = fopen(fileName,"wb");
  if (!f) throw "::saveToWav";
    fwrite(&hdrRiff,sizeof(hdrRiff),1,f);
    fwrite(sFmt,hdrRiff.lenFmtBlock,1,f);
    hdrData.lenDataBlock = lenRaw;
    fwrite(&hdrData,sizeof(hdrData),1,f);
    fwrite(raw,lenRaw,1,f);
  fclose(f);
}

cRawData::~cRawData() {   
  if(raw!=NULL) delete raw;
  if(sFmt!=NULL) delete sFmt;
  if(infile!=NULL) fclose(infile);
}

void cRawData::saveHeader(char* fileName)
     {
      FILE* f;
      f = fopen(fileName,"rb+");
      myWAVEFORMAT fmt;
      memcpy(hdrRiff.sigRiff,"RIFF",4);
      hdrRiff.lenRiffBlock = sizeof(fmt) + sizeof(hdrData)+lenRaw;
      memcpy(hdrRiff.sigWave,"WAVE",4);
      memcpy(hdrRiff.sigFmt,"fmt ",4);
      hdrRiff.lenFmtBlock = sizeof(fmt);
      fmt.wFormatTag = 1;//WAVE_FORMAT_PCM;
      fmt.nChannels = Channels;
      fmt.nSamplesPerSec = SampleFreq;
      fmt.nAvgBytesPerSec = SampleFreq * 2 * Channels;
      fmt.nBlockAlign = 2 * Channels;
      fmt.wBitsPerSample = 16;
      memcpy(hdrData.sigData,"data",4);
      hdrData.lenDataBlock = this->lenRaw;
      fwrite(&hdrRiff,sizeof(hdrRiff),1,f);
      fwrite(&fmt,sizeof(fmt),1,f);
      fwrite(&hdrData,sizeof(hdrData),1,f);
      fclose(f);
    }




// here there is FmtBlock where stereo and mono


// MAIN CLASS



typedef void NextDataProc(void*, long); // filling buffer here

class cSound {
  HWAVEOUT handleWaveOut;
  WAVEFORMATEX WaveFormat;
  bool Closing;
  void PlayHeader(WAVEHDR &WaveHeader);
  WAVEHDR WaveHeader1;
  WAVEHDR WaveHeader2;
public:
  bool Closed;
  int Device; // 0 - wave mapper, 1,2,3 - cards (from windows enum)
  NextDataProc* NextData;
  long SampleRate;
  int Channels;
  long BlockLen;
  void NextHeader(); // internal use only
  cSound();
  void Open(long newSampleRate = 44100,int Channels = 2);
  void Close();
};



cSound::cSound() {

  SampleRate = 44100;
  NextData = NULL;
  Closed = true;
  Device = 0;

}
void cSound::PlayHeader(WAVEHDR &WaveHeader) {

  MMRESULT  res = waveOutUnprepareHeader(
   handleWaveOut,
   &WaveHeader,
   sizeof(WaveHeader));
  if (res==WAVERR_STILLPLAYING)
    throw (":stillplaying:");
  if (res!=MMSYSERR_NOERROR)
    throw (":waveOutUnprepareHeader:error");

  res = waveOutPrepareHeader(
   handleWaveOut,
   &WaveHeader,
   sizeof(WaveHeader));

  if (res!=MMSYSERR_NOERROR)
    throw ":waveOutPreapareHeader:error";

  res = waveOutWrite(
    handleWaveOut,
    &WaveHeader,
    sizeof(WaveHeader));

  if (res!=MMSYSERR_NOERROR)
    throw ":waveOutWrite:error";

}

void cSound::NextHeader() {

  if(Closing) return;

    try {
      if (WaveHeader1.dwFlags & WHDR_DONE) {
         NextData(WaveHeader1.lpData,BlockLen);
         PlayHeader(WaveHeader1);
      }

      if (WaveHeader2.dwFlags & WHDR_DONE) {
         NextData(WaveHeader2.lpData,BlockLen);
         PlayHeader(WaveHeader2);
      }

    }
    catch (char* s) {
    }

}

void CALLBACK waveOutProc(HWAVEOUT, UINT uMsg, DWORD dwInstance, 
                 DWORD, DWORD) {

  
  cSound* Sound;
  Sound = (cSound*) (dwInstance);

  if(uMsg==WOM_DONE)
    Sound->NextHeader();
}

void cSound::Open(long newSampleRate, int newChannels) {

  if(NextData==NULL)
    throw "cSound::Open - no NextData proc";

  SampleRate = newSampleRate;
  Channels = newChannels;

  BlockLen = SampleRate*2*Channels / 15;

  WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
  WaveFormat.nChannels = Channels;
  WaveFormat.nSamplesPerSec = SampleRate;
  WaveFormat.nAvgBytesPerSec = SampleRate * 2 * Channels;
  WaveFormat.nBlockAlign = 2 * Channels;
  WaveFormat.wBitsPerSample = 16;
  WaveFormat.cbSize = 0; // ignored
  int outDevice;
  if(this->Device == 0)
    outDevice = WAVE_MAPPER;
  else {
    outDevice = this->Device - 1;
    if((outDevice+1)>waveOutGetNumDevs()) {
       Device = 0;
       outDevice = WAVE_MAPPER;
    }


  }

  MMRESULT res = waveOutOpen(
    &handleWaveOut,
    outDevice,
    &WaveFormat,
    DWORD(&waveOutProc),
    DWORD(this),
    CALLBACK_FUNCTION
   );

  if (res!=MMSYSERR_NOERROR)
    throw ":waveOutOpen:error";



  WaveHeader1.lpData = new char[BlockLen];
  WaveHeader1.dwBufferLength = BlockLen;
  WaveHeader1.dwBytesRecorded = 0;
  WaveHeader1.dwUser = 0;
  WaveHeader1.dwFlags = 0;
  WaveHeader1.dwLoops = 0;
  WaveHeader1.lpNext = NULL;
  WaveHeader1.reserved = 0;

  WaveHeader2.lpData = new char[BlockLen];
  WaveHeader2.dwBufferLength = BlockLen;
  WaveHeader2.dwBytesRecorded = 0;
  WaveHeader2.dwUser = 0;
  WaveHeader2.dwFlags = 0;
  WaveHeader2.dwLoops = 0;
  WaveHeader2.lpNext = NULL;
  WaveHeader2.reserved = 0;

  NextData(WaveHeader1.lpData,BlockLen);
  PlayHeader(WaveHeader1);
  NextData(WaveHeader2.lpData,BlockLen);
  PlayHeader(WaveHeader2);

  Closing = false;

  Closed = false;
}

void cSound::Close() {

  if (Closed) return;
  MMRESULT res;

  try {
    Closing = true;
    res = waveOutReset(handleWaveOut);
    if (res!=MMSYSERR_NOERROR) throw ":waveOutReset:error";

    res = waveOutClose(handleWaveOut);
    if (res!=MMSYSERR_NOERROR) throw ":waveOutClose:error";
  } catch (char* s) {
  }

  delete [] WaveHeader1.lpData;
  delete [] WaveHeader2.lpData;

  Closed = true;

}



typedef void* pointer;

class cDynList {
public:
  pointer* Items;
  void Add(pointer p);
  long Count;
  long ReservedCount;
  cDynList();
  ~cDynList();
private:
};

cDynList::cDynList() {
  Items = new pointer[32];  
  ReservedCount = 32;
  Count = 0;
}

cDynList::~cDynList() {
  delete Items;
}

void cDynList::Add(pointer p) {
  Items[Count] = p;
  Count++;

  if(Count>=ReservedCount) {
    pointer* newItems;
    newItems = new pointer[ReservedCount + 32];
    for(int i=0;i<ReservedCount;i++)
      newItems[i] = Items[i];
    delete Items;
    Items = newItems;
    ReservedCount += 32;
  }
}



// mix raw data must be stereo-16bit


struct sMixVar {
  pointer PCM; // start
  pointer CurPos; 
  pointer EndPos;
  bool Loop;
  bool FadeOut;
  float FadeOutStep; 
  float Volume;
  float Pitch;
  long PitchOfs;  // for internal use
  long Offset; // in MixBuffer bytes
  char KeyCode; // used for live mixing
  bool KeyReleased; //
  bool InMix; // used internally
};

typedef sMixVar* pMixVar;

class cMixer {
  cDynList MixVars;
  pointer MyBuf;
  long MyBufLen;
  void MixTo32(pointer Buf, long BufLen);
public:
  void MixTo(pointer Buf, long BufLen);
  void AddToMix(long Offset, pointer PCM, long PCMLen, float Volume=1, float Pitch=1, char KeyCode=0, bool Loop=false);
  bool ReleaseKey(char KeyCode);
  void StopAll();
  pMixVar KeyMixParam(char KeyCode);
  bool KeyInMix(char KeyCode);
  cMixer();
  ~cMixer();
};

cMixer::cMixer() {
  
  MyBuf = NULL;
  MyBufLen = 0;

}

typedef short* pShort;
typedef long* pLong;

void cMixer::MixTo(pointer Buf, long BufLen) {

  long NewMyBufLen = BufLen * 2;
  if (MyBufLen<NewMyBufLen) {
    if (MyBuf!=NULL)
      delete MyBuf;
    MyBuf = new char[NewMyBufLen];
    MyBufLen = NewMyBufLen;
  }

  // mixing to 32bit buffer without cliping for 16bit
  MixTo32(MyBuf, MyBufLen);

  // coping data to 16bit buffer with cliping
  pLong src = pLong(MyBuf);
  pShort dst = pShort(Buf);

  pLong endSrc = pLong( long(MyBuf) + MyBufLen);
  long v;

  while(true) {
    if (src>=endSrc) break;

    v = src[0];
    if (v>32768) v=32768;
    if (v<-32767) v=-32767;
    dst[0] = short(v);

    src++;
    dst++;

  }

}

void cMixer::MixTo32(pointer Buf,long BufLen) {

  // INITIALIZING WITH SILENCE
  pLong BufPos;
  pointer EndBufPos = pointer(long(Buf)+BufLen);
  BufPos = pLong(Buf);
  for(long i=0;i<(BufLen/long(sizeof(long)));i++)
    BufPos[i] = 0;

  pLong dest;
  pShort src;
  // ADDING WAVS TO MIXBUFFER
  for(long i=0;i<MixVars.Count;i++) {
    // USING MIXVARS (EVERY CHANNEL)
    pMixVar p;
    p = pMixVar(MixVars.Items[i]);
    if(p->FadeOut) {
      p->Volume -= p->FadeOutStep;
      if(p->Volume <= 0) {
        p->Volume = 0;
        p->InMix = false;
      }
    }

    if (p->InMix) {
      // offset in 16bit - BufLen in 32bit
      if(p->Offset>BufLen/2) {
         p->Offset -= BufLen/2; // decrease offset by buflen
         break; // next MixVar
      }
      long volInt = long(p->Volume * 0x1000);
      long pitchStep = long(p->Pitch * 0x10000);
      dest = pLong (long(Buf) + p->Offset * 2);
      p->Offset = 0; // erase offset at all
      src = pShort (p->CurPos );
      // MIX BY ADDING PCM TO BUFFER
      long v;
      long MyPitchOfs;
      MyPitchOfs = p->PitchOfs; // caching for speed
      while(true) {
        // LEFT CHANNEL
        v = src[0]; // base
        v += (src[2] - src[0]) * MyPitchOfs / 0x10000;
        v = v*volInt/0x1000;
        dest[0] += v;

        // RIGHT CHANNEL
        v = src[1];
        v += (src[3] - src[1]) * MyPitchOfs / 0x10000;
        v = v*volInt/0x1000;
        dest[1] += v;

        // INC POINTERS
        dest++;dest++;
        MyPitchOfs += pitchStep;
        if(MyPitchOfs>0x10000) {
          src = src + (MyPitchOfs / 0x10000) * 2;
          MyPitchOfs = MyPitchOfs & 0xffff;
        }

        if(dest>=EndBufPos) break;
        if(src>=p->EndPos) {
          if(p->Loop)
            src = (short*)p->PCM;
          else {
            p->InMix = false;
            break;
          }
        }
      }

      p->PitchOfs = MyPitchOfs;
      p->CurPos = src;


    }

  }
}

void cMixer::AddToMix(long offset, pointer PCM, long PCMLen, float Volume, float Pitch, char KeyCode, bool Loop) {
  int i = 0;
  pMixVar p=NULL;
  // SCAN FOR NOT USED MIXVARS
  while(i<MixVars.Count) {
    p = pMixVar(MixVars.Items[i]);
    if (!p->InMix) break;
    i++;
    p = NULL;
  }
  // IF NOT FOUND NOT USED
  if(p==NULL) {
    p = new sMixVar;
    MixVars.Add(p);
  }

  p->FadeOut = false;
  p->PCM = PCM;
  p->CurPos = PCM;                  // -4 for interpolation
  p->EndPos = pointer(long(PCM)+PCMLen-4);
  p->Offset = offset;
  p->Volume = Volume;
  p->InMix = true;
  p->Pitch = Pitch;
  p->PitchOfs = 0;
  p->KeyCode = KeyCode;
  p->Loop = Loop;
  p->KeyReleased = false;
}

bool cMixer::KeyInMix(char KeyCode) {
  pMixVar p;
  for(int i=0;i<MixVars.Count;i++) {
    p = pMixVar(MixVars.Items[i]);
    if (!p->KeyReleased && p->KeyCode==KeyCode) {
       return(true);
    }
  }
  return(false);
}

pMixVar cMixer::KeyMixParam(char KeyCode) {
  pMixVar p;
  for(int i=0;i<MixVars.Count;i++) {
    p = pMixVar(MixVars.Items[i]);
    if (!p->KeyReleased && p->KeyCode==KeyCode) {
       return p;
    }
  }
  return NULL;
}

bool cMixer::ReleaseKey(char KeyCode) {
  pMixVar p;
  bool res = false;
  for(int i=0;i<MixVars.Count;i++) {
    p = pMixVar(MixVars.Items[i]);
    if (!p->KeyReleased && p->KeyCode==KeyCode) {
      p->InMix = false;
      p->KeyReleased = true;
      res = true;
    }
  }
  return(res);
}

void cMixer::StopAll() {
  pMixVar p;
  for(int i=0;i<MixVars.Count;i++) {
    p = pMixVar(MixVars.Items[i]);
    {
      p->InMix = false;
      p->KeyReleased = true;
    }
  }
}

cMixer::~cMixer() {
  // FREE RESERVED MIXVARS
  for(int i=0;i<MixVars.Count;i++)
    delete MixVars.Items[i];
}

cAudio Audio;
cMixer* AudioMixer;

void AudioNextData(void* Buf, long BufLen) {
  AudioMixer->MixTo(Buf,BufLen);
};

cAudio::cAudio() {
  smp = new TStringList;
  mix = new cMixer;
  snd = new cSound;
  AudioMixer = mix;
  snd->NextData = AudioNextData;
  snd->Open(44100,2);
}
struct sAudioSample {
  float Volume;
  cRawData raw;
  bool Loop;
  bool PitchVolume;
  float pmin;
  float pmax;
  float vmin;
  float vmax;
  void Reset() {
     Loop = false;
     Volume = 1;
     PitchVolume = false;
  }
};

cAudio::~cAudio() {
 snd->Close();
 delete snd;
 delete mix;
 // clear and free mem for samples
 for(int i=smp->Count-1;i>=0;i--) {
   delete (sAudioSample*)smp->Objects[i];
   smp->Delete(i);
 }

 delete smp;
}

void cAudio::PlaySample(AnsiString name) {

  int i = smp->IndexOf(name);

  if(i==-1) return;
  sAudioSample* audsamp;
  audsamp = (sAudioSample*)smp->Objects[i];

  mix->AddToMix(0,audsamp->raw.raw,audsamp->raw.lenRaw,
     this->vol*audsamp->Volume,1,i,
     audsamp->Loop);
}

void cAudio::StopAll() {
   mix->StopAll();
}
void cAudio::SetSampleLoop(AnsiString name) {
  int i = smp->IndexOf(name);
  if(i!=-1)
    ((sAudioSample*)smp->Objects[i])->Loop = true;
}

void cAudio::StopSample(AnsiString name) {
  int i = smp->IndexOf(name);
  if(i!=-1)
    mix->ReleaseKey(i);
}

void cAudio::SetVolume(float vol) {
  this->vol = vol;
}
void cAudio::SetSampleVolume(AnsiString name, float newvolume) {
  int i = smp->IndexOf(name);
  if(i==-1) return;
  ((sAudioSample*)smp->Objects[i])->Volume = newvolume;
}
void cAudio::SetPitchVolume(AnsiString name,float pmin,float pmax,float vmin,float vmax) {//volume dec while pitch grows
  int i = smp->IndexOf(name);
  if(i==-1) return;
  sAudioSample* aud = (sAudioSample*)(smp->Objects[i]);
  aud->pmin = pmin;
  aud->pmax = pmax;
  aud->vmin = vmin;
  aud->vmax = vmax;
  aud->PitchVolume = true;
}

bool cAudio::IsSamplePlaying(AnsiString name) {
  int i = smp->IndexOf(name);
  return mix->KeyInMix(i);
}

float prop(float pmin,float pmax,float p,float vmin,float vmax) {
  return (float)vmin + (vmax-vmin)*(p-pmin)/(pmax-pmin);
}

void cAudio::SetSamplePitch(AnsiString name, float newpitch) {
  int i = smp->IndexOf(name);
  if(i==-1) return;

  pMixVar p = mix->KeyMixParam(i);

  sAudioSample* aud = (sAudioSample*)(smp->Objects[i]);
  if(p!=NULL) {
    if(aud->PitchVolume) {
      p->Volume = this->vol*prop(aud->pmin,aud->pmax,newpitch,aud->vmin,aud->vmax);
    }
    p->Pitch = newpitch;
  }

}
void cAudio::FadeOutSample(AnsiString name,float timesec) {
  int i = smp->IndexOf(name);
  if(i==-1) return;

  pMixVar p = mix->KeyMixParam(i);
  if(p==NULL) return;
  p->KeyReleased = true;
  p->KeyCode = -1;

  sAudioSample* aud = (sAudioSample*)(smp->Objects[i]);
  if(p!=NULL) {
    p->FadeOut = true;

    float oneblock = (float)snd->BlockLen / (snd->SampleRate * 2 * snd->Channels);
    float pieces = timesec / oneblock;


    p->FadeOutStep = p->Volume / pieces;
  }

}


void cAudio::LoadSample(AnsiString name,AnsiString filename,float volume) {
  smp->Add(name);
  int i = smp->Count-1;

  smp->Objects[i] = (TObject*)new sAudioSample;

  ((sAudioSample*)smp->Objects[i])->Reset();
  if(ExtractFileExt(filename)==".wav")
    ((sAudioSample*)smp->Objects[i])->raw.loadFromWav(filename.c_str());
  if(ExtractFileExt(filename)==".mp3")
    ((sAudioSample*)smp->Objects[i])->raw.loadFromMP3(filename.c_str());

  SetSampleVolume(name,volume);
}



