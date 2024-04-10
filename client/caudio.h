//audio for games
// LoadSound(name,file)
// SetLoop(name,ofsstart,ofsend)
// PlaySound(name)

// this will work only in Borland C Builder (because of AnsiString class)

#ifndef CAUDIOH
#define CAUDIOH

class cSound;
class cMixer;

class cAudio {
public:
  void LoadSample(AnsiString name,AnsiString filename,float Volume=1);
  void PlaySample(AnsiString name);
  void SetSampleVolume(AnsiString name,float vol);
  void SetSamplePitch(AnsiString name, float newpitch);
  bool IsSamplePlaying(AnsiString name);
  void SetVolume(float vol);
  void SetPitchVolume(AnsiString name,float pmin,float pmax,float vmin,float vmax);//volume dec while pitch grows
  void StopSample(AnsiString name);
  void SetSampleLoop(AnsiString name);
  void FadeOutSample(AnsiString name,float timesec=2);
  void StopAll();
  cAudio();
  ~cAudio();
private:
  float vol;
  TStringList* smp;
  cSound* snd;
  cMixer* mix;
};

extern cAudio Audio;






#endif CAUDIOH
