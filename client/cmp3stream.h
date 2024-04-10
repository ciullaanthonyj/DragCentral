
#ifndef cmp3streamH
#define cmp3streamH

#include "mpglibdll.h"

typedef struct {
	char *pos;
    UINT size;
    UINT frame;
} STREAMPOS;

typedef struct {
	HANDLE hFile;		// handle of opened file ( CreateFile )
    HANDLE hMapFile;	// handle of the file-mapping object
    char *lpMapAddress;	// starting address of the mapped view ( don't modify )
    DWORD dwSize;
    char szFileName[MAX_PATH];
} FILEMAPPING;


typedef struct {
    char *lpStreamStart;	// start of stream
    UINT uStreamLength;		// length of stream
    char *lpAudioStart;		// start of audio data
    UINT uAudioLength;		// length of audio data
    char *lpPos;			// current position
    UINT uLength;			// length of resto of stream ( uStreamLength - ( lpPos - lpStreamStart) )
    UINT uFrameNum;			// number of frames
    UINT uCurrFrame;		// current frame
    BOOL vbr;				// variable bitrate
    WORD nChannel;			// number of chennels ( 1 = MONO, 2 = STEREO )
    DWORD dwSamplingRate;	// sampling rate frequency  ( Hz )
    int iLayer;				// layer
    int iMPEGversion;		// mpeg version
    int iInputBufferLength;	// // input buffer ( number of frames  to load per output buffr )
    char MPEGversion[64];
    char MPEGlayer[40];
    int minBitRate;			// min bitrateindex
    int maxBitRate;			// max bitrate index
    char channelMode[40];
    char emphasis[40];
    UINT frameSize;			// number of samples in frame
    STREAMPOS spos;

} MP3STREAM;


typedef struct {
    int stereo;		// channel number 1= mono, 2=stereo
    int lsf;
    int mpeg25;
    int lay;
    int error_protection;
    int sampling_frequency;
    int bitrate_index;
    int padding;
    int extension;
    int mode;
    int mode_ext;
    int copyright;
    int original;
    int emphasis;
    int framesize;  /* computed framesize */
    STREAMPOS spos;
} FRAMEHEADER ;

class cMP3Stream {

FILEMAPPING fm;
struct mpstr mp;
public:
  MP3STREAM mp3stream;
  char mybuf[8192*4];
  int mybufofs;
  int mybufdataleft;
  int newSeek;


  cMP3Stream();
  ~cMP3Stream();
  void OpenMP3(char* filename);
  void Seek(int offset);
  void Reset();
  int SampleFreq() {  
     return mp3stream.dwSamplingRate;
  }
  int Channels() {
     return mp3stream.nChannel;
  }
  int GetBytes(void* extbuf,int extbuflen);
};

#endif
