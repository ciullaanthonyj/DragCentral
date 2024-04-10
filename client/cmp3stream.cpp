#include <pch.h>
#pragma hdrstop
#include "cmp3stream.h"

#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define max(a, b)  (((a) > (b)) ? (a) : (b))



#define STREAM_OK 0
#define STREAM_END 1
#define STREAM_ERROR 2

#define STOP 0
#define PLAY 1


#define MP3_FILE_BEGIN 0
#define MP3_FILE_CURRENT 1
#define MP3_FILE_END 2


#define         MPG_MD_STEREO           0
#define         MPG_MD_JOINT_STEREO     1
#define         MPG_MD_DUAL_CHANNEL     2
#define         MPG_MD_MONO             3

void initFileMapping ( FILEMAPPING *fm );
BOOL openFileMapping ( char *filename , FILEMAPPING *fm);
void closeFileMapping ( FILEMAPPING *fm);

BOOL openMp3File(HWND hwnd, FILEMAPPING *fm);
BOOL initMp3stream ( FILEMAPPING *fm ,MP3STREAM *mp3s);

void closeMp3File ( FILEMAPPING *fm );

void play(HWND hwndApp);
BOOL mp3Seek(MP3STREAM *mp3s, int offset, int whence);
void findTag(HWND hwnd,MP3STREAM *mp3s);
BOOL decodeFrameHeader ( char *header, FRAMEHEADER *fh );
BOOL findNextHeader (MP3STREAM *mp3s, FRAMEHEADER *fh);
int getnFrame(MP3STREAM *mp3s, FRAMEHEADER *mp3f, int num);



// ************* END FUNCTION DECLARATION ************



// *********  GLOBAL VARIABLE **************************

//FILEMAPPING fm;
//MP3STREAM mp3stream;
HINSTANCE    hmpglibdll = NULL;
//struct mpstr mp;
INITMP3      InitMP3;
EXITMP3      ExitMP3;
DECODEMP3    decodeMP3;

const char *sGenres [] =
	{
		"Blues",
		"Classic Rock",
		"Country",
		"Dance",
		"Disco",
		"Funk",
		"Grunge",
		"Hip-Hop",
		"Jazz",
		"Metal",
		"New Age",
		"Oldies",
		"Other",
		"Pop",
		"R&B",
		"Rap",
		"Reggae",
		"Rock",
		"Techno",
		"Industrial",
		"Alternative",
		"Ska",
		"Death Metal",
		"Pranks",
		"Soundtrack",
		"Euro-Techno",
		"Ambient",
		"Trip Hop",
		"Vocal",
		"Jazz+Funk",
		"Fusion",
		"Trance",
		"Classical",
		"Instrumental",
		"Acid",
		"House",
		"Game",
		"Sound Clip",
		"Gospel",
		"Noise",
		"Alternative Rock",
		"Bass",
		"Soul",
		"Punk",
		"Space",
		"Meditative",
		"Instrumental Pop",
		"Instrumental Rock",
		"Ethnic",
		"Gothic",
		"Darkwave",
		"Techno-Industrial",
		"Electronic",
		"Pop-Folk",
		"Eurodance",
		"Dream",
		"Southern Rock",
		"Comedy",
		"Cult",
		"Gangsta Rap",
		"Top 40",
		"Christian Rap",
		"Pop/Punk",
		"Jungle",
		"Native American",
		"Cabaret",
		"New Wave",
		"Phychedelic",
		"Rave",
		"Showtunes",
		"Trailer",
		"Lo-Fi",
		"Tribal",
		"Acid Punk",
		"Acid Jazz",
		"Polka",
		"Retro",
		"Musical",
		"Rock & Roll",
		"Hard Rock",
		"Folk",
		"Folk/Rock",
		"National Folk",
		"Swing",
		"Fast-Fusion",
		"Bebob",
		"Latin",
		"Revival",
		"Celtic",
		"Blue Grass",
		"Avantegarde",
		"Gothic Rock",
		"Progressive Rock",
		"Psychedelic Rock",
		"Symphonic Rock",
		"Slow Rock",
		"Big Band",
		"Chorus",
		"Easy Listening",
		"Acoustic",
		"Humour",
		"Speech",
		"Chanson",
		"Opera",
		"Chamber Music",
		"Sonata",
		"Symphony",
		"Booty Bass",
		"Primus",
		"Porn Groove",
		"Satire",
		"Slow Jam",
		"Club",
		"Tango",
		"Samba",
		"Folklore",
		"Ballad",
		"power Ballad",
		"Rhythmic Soul",
		"Freestyle",
		"Duet",
		"Punk Rock",
		"Drum Solo",
		"A Capella",
		"Euro-House",
		"Dance Hall",
		"Goa",
		"Drum & Bass",
		"Club-House",
		"Hardcore",
		"Terror",
		"indie",
		"Brit Pop",
		"Negerpunk",
		"Polsk Punk",
		"Beat",
		"Christian Gangsta Rap",
		"Heavy Metal",
		"Black Metal",
		"Crossover",
		"Comteporary Christian",
		"Christian Rock",
		"Merengue",
		"Salsa",
		"Trash Metal",
		"Anime",
		"JPop",
		"Synth Pop"
	};



const int tabsel_123[2][3][16] = {
   { {0,32,64,96,128,160,192,224,256,288,320,352,384,416,448,},
     {0,32,48,56, 64, 80, 96,112,128,160,192,224,256,320,384,},
     {0,32,40,48, 56, 64, 80, 96,112,128,160,192,224,256,320,} },

   { {0,32,48,56,64,80,96,112,128,144,160,176,192,224,256,},
     {0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,},
     {0,8,16,24,32,40,48,56,64,80,96,112,128,144,160,} }
};

const long freqs[9] = { 44100, 48000, 32000,
                  22050, 24000, 16000 ,
                  11025 , 12000 , 8000 };



typedef char mystr[40];

const mystr sMPEGaudioVersion[2][2]=
	{
    	{"MPEG Version 1 (ISO/IEC 11172-3)","MPEG Version 2 (ISO/IEC 13818-3)"},
    	{ "reserver","MPEG Version 2.5"}

    };

const mystr sMPEGlayer[3]=
	{	"Layer I",
    	"Layer II",
        "Layer III"
    };

const mystr sChannelMode[4]=
	{	"Stereo",
    	"Joint stereo  (Stereo)",
        "Dual channel  (Stereo)",
        "Single channel (Mono)"
    };

const mystr sEmphasis[4]=
	{   "none",
    	"50/15 ms",
    	"reserved",
        "CCIT J.17"
    };


int statwidths[] = {100, -1}; // statusbar array



//***********  END GLOBAL VARIABLE *******************






#define ID_PRVI 1000



void alert(char * text)
{
 	MessageBox(0,text,"Alert",0);
}

void alert(int number)
{
	char text[20];
    sprintf(text,"%i",number);
    MessageBox(0,text,"Alert",0);
}

void alertl(DWORD number)
{
	char text[20];
    sprintf(text,"%Li",number);
    MessageBox(0,text,"Alert",0);

}

void alertf(float number)
{
	char text[20];
    sprintf(text,"%f",number);
    MessageBox(0,text,"Alert",0);
}

void alertu(int number)
{
	char text[20];
    sprintf(text,"%u",number);
    MessageBox(0,text,"Alert",0);
}

void log(char *message, int num)
{

    FILE *file=fopen("log.txt","a+");
    fprintf(file,"%s : %u\n",message,num);
    fclose(file);


}



void initFileMapping ( FILEMAPPING *fm )
{
// initialize FILEMAPPING structure
//
//	INPUT:		FILEMAPPING *fm		- pointer to FILEMAPPING structure
//
//	OUTPUT:		function fills FILEMAPPING *fm structure
//
	fm->hFile=NULL;
    fm->hMapFile=NULL;
    fm->lpMapAddress=NULL;
    fm->dwSize=0xFFFFFFFF;
    *fm->szFileName=0;
}


BOOL openFileMapping ( char *filename , FILEMAPPING *fm)
{
// open file and maps a view of a file into the address space of the calling process.
//
//	INPUT:		char *filename 		- pointer to null-terminated string containing file name
//				FILEMAPPING *fm		- pointer to FILEMAPPING structure
//
//	OUTPUT:		function fills FILEMAPPING *fm structure
//
//	RETURN:		TRUE	- if all OK
//				FALSE	- if opening fails ( ALL CLEAN )
//
//	REMARK:		if function fails, FILEMAPPING *fm is clean ( mapping and file are closed )

	// open file
    fm->hFile=CreateFile(filename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
    if (fm->hFile== INVALID_HANDLE_VALUE) {
        return FALSE;

    }
    // creates a named or unnamed file-mapping object for the specified file.
    fm->hMapFile= CreateFileMapping(fm->hFile,NULL,PAGE_READONLY,0,0,NULL);
    if (fm->hMapFile == NULL ) {
        CloseHandle(fm->hFile);
        return FALSE;
    }
    // maps a view of a file into the address space of the calling process
    fm->lpMapAddress= (char *) MapViewOfFile(fm->hMapFile,FILE_MAP_READ,0,0,0);
    if (fm->lpMapAddress == NULL) {
    	CloseHandle(fm->hMapFile);
        CloseHandle(fm->hFile);
        return FALSE;

    }

    // get file size
    fm->dwSize=GetFileSize(fm->hFile,NULL);
    // if error get file size
    if (fm->dwSize == 0xFFFFFFFF) {
        UnmapViewOfFile(fm->lpMapAddress);
        CloseHandle(fm->hMapFile);
        CloseHandle(fm->hFile);
    	return FALSE;
    }
    strcpy(fm->szFileName, filename);

    return TRUE;
}

void closeFileMapping ( FILEMAPPING *fm)
{
// close open file
//
//	INPUT:		FILEMAPPING *fm		- pointer to FILEMAPPING structure
//
//	OUTPUT:		function fills FILEMAPPING *fm structure

    if (fm->dwSize != 0xFFFFFFFF ) {
		UnmapViewOfFile(fm->lpMapAddress);
        fm->lpMapAddress=NULL;
    	CloseHandle(fm->hMapFile);
        fm->hMapFile=NULL;
    	CloseHandle(fm->hFile);
        fm->hFile=NULL;
    	fm->dwSize=0xFFFFFFFF;
        *fm->szFileName=0;
    }
}


BOOL openMp3File(HWND hwnd, FILEMAPPING *fm)
{
// show open dialog, choose file , create file mapping
//	INPUT:		HWND hwnd 			- handle of owner window
//				FILEMAPPING *fm		- pointer to FILEMAPPING structure
//
//	OUTPUT:		function fills FILEMAPPING *fm structure
//
//	RETURN:		TRUE	- if all OK
//				FALSE	- if opening fails ( ALL CLEAN )
//
//	REMARK:		if function fails, FILEMAPPING *fm is clean ( mapping and file are closed )

	OPENFILENAME ofn;
    char szFileName[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = "Mp3 Files (*.mp3)\0*.mp3\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "mp3";
    if(GetOpenFileName(&ofn))
    {

    	closeFileMapping(fm); // close previous opened file
        if ( !openFileMapping ( szFileName, fm) ) { // fail to open file
        	char text[MAX_PATH + 20 ];
            sprintf(text,"Can't open file: %s",szFileName);
            MessageBox( hwnd, text, "Error", MB_ICONSTOP );
        	return FALSE;
        }
    }
    else
    	return FALSE;

    return TRUE;
}

void closeMp3File ( FILEMAPPING *fm )
{
	closeFileMapping(fm); // close previous opened file
}

BOOL initMp3stream( FILEMAPPING *fm ,MP3STREAM *mp3s)
{
// initialize MP3STREAM *mp3s
//
// INPUT:	FILEMAPPING *fm		- initialized with call to openFileMapping( char *filename , FILEMAPPING *fm);
//          MP3STREAM *mp3s		-
//
//	OUTPUT: MP3STREAM *mp3s		- fill main data needed to play
//
//	RETURN: TRUE	- all OK
//			FALSE	- can't find first header ( error in mp3 file )
//


 	mp3s->lpStreamStart=fm->lpMapAddress; 	// copy starting address of the mapped view
                                            // because we can't modyfy original address
    mp3s->uStreamLength=fm->dwSize;		// length of whole stream

    mp3s->spos.pos=mp3s->lpStreamStart;	// set pos to stream start;
    mp3s->spos.size=mp3s->uStreamLength;	// current length of rest of stream
	mp3s->spos.frame=0;						// current frame

    mp3s->uFrameNum=0; // count number of frames

    // find first header

    FRAMEHEADER fh;

   if ( !findNextHeader(mp3s,&fh)) {
        return FALSE;
   }

    // set first frame pointer
    mp3s->lpAudioStart=fh.spos.pos;

    // number of channels
    mp3s->nChannel=fh.stereo;
    // sampling rate frequency
    mp3s->dwSamplingRate=freqs[fh.sampling_frequency];
    // layer version
    mp3s->iLayer=fh.lay;
    mp3s->iMPEGversion=fh.lsf;

     //int frameSize; // size of frame ( number of samples containde in frame )
    if (mp3s->iMPEGversion == 1 ) // MPEG 2 & MPEG 2.5
    	mp3s->frameSize=576;
    else	// MPEG 1
    	mp3s->frameSize=1152;

    mp3s->uFrameNum++;

    // MPEG version
    strcpy(mp3s->MPEGversion, sMPEGaudioVersion[fh.mpeg25][fh.lsf]);
    // MPEG layer
    strcpy(mp3s->MPEGlayer, sMPEGlayer[fh.lay-1]);
    // channel mode
    strcpy(mp3s->channelMode, sChannelMode[fh.mode]);
    // emphasis
    strcpy(mp3s->emphasis, sEmphasis[fh.emphasis]);

    mp3s->minBitRate=0xFF;
    mp3s->maxBitRate=0;

   while ( getnFrame(mp3s,&fh,1) == STREAM_OK) {
    	mp3s->uFrameNum++;
        mp3s->minBitRate=min(mp3s->minBitRate,fh.bitrate_index);
        mp3s->maxBitRate=max(mp3s->maxBitRate,fh.bitrate_index);

    }

     mp3s->uAudioLength = mp3s->spos.pos - mp3s->lpAudioStart;

    // is variable birate ???
    if (mp3s->minBitRate != mp3s->maxBitRate)
    	 mp3s->vbr=TRUE;
    else
         mp3s->vbr=FALSE;

    mp3s->minBitRate=tabsel_123[fh.lsf][fh.lay-1][mp3s->minBitRate];
    mp3s->maxBitRate=tabsel_123[fh.lsf][fh.lay-1][mp3s->maxBitRate];



    // set position to first frame
    mp3s->spos.pos=mp3s->lpAudioStart;
    mp3s->spos.size = mp3s->uAudioLength;
    mp3s->spos.frame=0;

	return TRUE;
}

BOOL mp3Seek(MP3STREAM *mp3s, int offset, int whence)
{

    FRAMEHEADER fh;
    switch ( whence ) {
    	case MP3_FILE_BEGIN:
        	if (offset >= 0 ) {
            	  mp3s->spos.pos=mp3s->lpAudioStart;
                  mp3s->spos.size = mp3s->uAudioLength;
    		  mp3s->spos.frame=0;
    		  getnFrame(mp3s,&fh,offset);
                  return TRUE;
                }
        break;

        case MP3_FILE_CURRENT:
            mp3s->spos.pos=mp3s->lpAudioStart;
            mp3s->spos.size = mp3s->uAudioLength;

            int newpos;
            newpos=mp3s->spos.frame + offset;
            if (newpos < 0 )
            	newpos=0;
            mp3s->spos.frame=0;
            getnFrame(mp3s,&fh,newpos);

        return TRUE;


        case FILE_END:
        	if (offset > 0 ) {
        		mp3s->spos.pos=mp3s->lpAudioStart;
    			mp3s->spos.size = mp3s->uAudioLength;
    			mp3s->spos.frame=0;
        		int newpos;
            	newpos=mp3s->uFrameNum - offset;
            	getnFrame(mp3s,&fh,newpos);
            	return TRUE;
            }
        break;

        default:
        	return FALSE;
    }

    return FALSE;

}

BOOL decodeFrameHeader ( char *header, FRAMEHEADER *fh )
{
// decode first 32 bits in frame ( frame header )
//
// INPUT:	char *header	- pointer to header start
//
//	OUTPUT:	FRAMEHEADER *fh	- fill all
//
//	RETURN:	TRUE	- if valid header
//			FALSE	- if invalid header


	unsigned long newhead;
    newhead= (unsigned long) ( ( ( header[0] & 0xFF ) << 24 ) |
				( ( header[1] & 0xFF ) << 16 ) |
                                ( ( header[2] & 0xFF ) << 8 ) |
                                             ( header[3] & 0xFF )
                							  );

     // sync bits
    if ( ( ( newhead >> 21 ) & 0x07FF ) != 0x07FF )  // check sync bits
     	return FALSE;



    // MPEG audio version
    if( newhead & (1<<20) ) {
      fh->lsf = (newhead & (1<<19)) ? 0x0 : 0x1;
      fh->mpeg25 = 0;
    }
    else {
    	if ( newhead & (1<<19))
        	return FALSE;
      fh->lsf = 1;
      fh->mpeg25 = 1;
    }

    // MPEG Layer version
    fh->lay = 4-((newhead>>17)&3);
    if (fh->lay == 4 )
    	return FALSE;

     // crc error protection
    fh->error_protection = ((newhead>>16)&0x1)^0x1;

     // bitrate
    fh->bitrate_index = ((newhead>>12)&0xF);
    if (! fh->bitrate_index || fh->bitrate_index == 0xF)
    	return FALSE;

    // sampling frequency rate
    if( ((newhead>>10)&0x3) == 0x3) {
      return FALSE;
    }

    if(fh->mpeg25) {
      fh->sampling_frequency = 6 + ((newhead>>10)&0x3);
    }
    else
      fh->sampling_frequency = ((newhead>>10)&0x3) + (fh->lsf*3);


   // padding
    fh->padding   = ((newhead>>9)&0x1);
    // private bit
    fh->extension = ((newhead>>8)&0x1);
    // channel mode
    fh->mode      = ((newhead>>6)&0x3);
    //Mode extension (Only if Joint stereo)
    fh->mode_ext  = ((newhead>>4)&0x3);
    // Copyright
    fh->copyright = ((newhead>>3)&0x1);
    // Original
    fh->original  = ((newhead>>2)&0x1);
    // Emphasis
    fh->emphasis  = newhead & 0x3;


    // stereo
    fh->stereo    = (fh->mode == MPG_MD_MONO) ? 1 : 2;

    // calculate frame size

      switch(fh->lay)
    {
      case 1:
#if 0
        fh->framesize  = (long) tabsel_123[fh->lsf][0][fh->bitrate_index] * 12000;
        fh->framesize /= freqs[fhf->sampling_frequency];
        fh->framesize  = ((fh->framesize + fh->padding)<<2);
#else
		 return FALSE;

#endif
        break;
      case 2:

        fh->framesize = (long) tabsel_123[fh->lsf][1][fh->bitrate_index] * 144000;
        fh->framesize /= freqs[fh->sampling_frequency];
        fh->framesize += fh->padding;

      break;

      case 3:
          fh->framesize  = (long) tabsel_123[fh->lsf][2][fh->bitrate_index] * 144000;
          fh->framesize /= freqs[fh->sampling_frequency]<<(fh->lsf);
          fh->framesize = fh->framesize + fh->padding ;

      break;

      default:
        return FALSE;
    }

    return TRUE;
}

BOOL findNextHeader (MP3STREAM *mp3s, FRAMEHEADER *fh)
{
// search stream for next valid frame header
//
// INPUT:	MP3STREAM *mp3s
//
// OUTPUT:	update STREAMPOS *spos
//			FRAMEHEADER *fh
//
//	RETURN:	TRUE	- if found valid frame header
//			FASLE	- can't find valid frame header

    char *newhead;
    FRAMEHEADER fhl;
	while ( (newhead=(char *) memchr(mp3s->spos.pos,255,mp3s->spos.size)) != NULL) { // find 0xFF byte
    	if (decodeFrameHeader(newhead,fh)) {	// if valid first frame header
        	// go to next header
            if ( mp3s->spos.size - fh->framesize < 4 ) {	// end of stream
                mp3s->spos.size -= ( newhead - mp3s->spos.pos );
                mp3s->spos.pos=newhead;
                fh->spos.pos=newhead;
            	return TRUE;
            }

            if (decodeFrameHeader(newhead + fh->framesize,&fhl)) {	// if valid second frame header
            	if ( fhl.lsf == fh->lsf && fhl.mpeg25 == fh->mpeg25 && fhl.lay == fh->lay && fhl.sampling_frequency == fh->sampling_frequency ) {
                	mp3s->spos.size -= ( newhead - mp3s->spos.pos );
                	mp3s->spos.pos=newhead;
                    fh->spos.pos=newhead;
            		return TRUE;
                }
            }
        }
        mp3s->spos.pos++;
        mp3s->spos.size--;
        if ( mp3s->spos.size < 4 ) // end of stream
        	return FALSE;
    }
    return FALSE;
}



int getnFrame(MP3STREAM *mp3s, FRAMEHEADER *mp3f, int num)
{
// get n frames from stream
//
// INPUT:	MP3STREAM *mp3s			- need  ( mp3s->spos)
//
//	OUTPUT:	MP3STREAM *mp3s			- update MP3STREAM *mp3s->spos
//          FRAMEHEADER *mp3f		- set mp3f->framesize to length of  num frames
//
//	RETURN: STREAM_OK	- if found num frames
//			STREAM_END	- if found  less than num frames , but minimum one frame found
//			STREAM_ERROR	- can't find any frame
//
//	REMARK:	function updates mp3s->spos for sequential call to this function

    int frameLength=0;
    FRAMEHEADER fh;
    mp3f->spos=mp3s->spos;
    if(mp3s->spos.pos>=(mp3s->lpStreamStart+mp3s->uStreamLength))
      return STREAM_ERROR;

	for (int i=0; i < num; i++) { // count frames
    	if ( decodeFrameHeader (mp3s->spos.pos,&fh )) {
            if (fh.framesize > mp3s->spos.size) { // stream don't contain full frame i.e. end of stream
                mp3f->spos.size=frameLength;
            	return STREAM_END;
            }


            frameLength += fh.framesize;

            // jump to end of frame ( to start of next frame )
           	// mp3f->frameStart += mp3f->framesize;

            mp3s->spos.pos += fh.framesize;
            mp3s->spos.size -= fh.framesize;
            mp3s->spos.frame++;

            if ( ! mp3s->spos.size ) { // end of stream
            	mp3f->spos.size=frameLength;
            	return STREAM_END;
            }

		}
        else {
        	if (frameLength == 0 ) // can't find first frame
            	return STREAM_ERROR;
            else {
            	mp3f->spos.size=frameLength;
            	return STREAM_END;
            }
        }

    }
    mp3f->spos.size=frameLength;
    return STREAM_OK;
}


// class for init and deinit mpglib.dll
class cmpglib {
public:
  cmpglib();
  ~cmpglib();
};
cmpglib mpglib;

cmpglib::cmpglib() {
            hmpglibdll = LoadLibrary("mpglib.dll");

			if (!hmpglibdll) {
    			alert("Can't load mpglib.dll");
    		}

            InitMP3   = (INITMP3)   GetProcAddress(hmpglibdll, TEXT_INITMP3);
			ExitMP3   = (EXITMP3)   GetProcAddress(hmpglibdll, TEXT_EXITMP3);
			decodeMP3 = (DECODEMP3) GetProcAddress(hmpglibdll, TEXT_DECODEMP3);

            if(!InitMP3 || !ExitMP3 || !decodeMP3)
			{

				alert("Unable to get MPGLIB interfaces");
			}
}
cmpglib::~cmpglib() {
     FreeLibrary(hmpglibdll);
}


cMP3Stream::cMP3Stream()
 {
     // NOT OPTIMIZED DLL INITIALIZATION
     initFileMapping (&fm );
     newSeek = -1;

  };

cMP3Stream::~cMP3Stream()
 {
  };


void cMP3Stream::OpenMP3(char* filename)
   {
       mybufofs = 0;
       mybufdataleft = 0;
       closeMp3File(&fm);
       if(!openFileMapping ( filename, &fm))
         throw "Error opening file.";
       initMp3stream (&fm ,&mp3stream) ;
       InitMP3(&mp);
       mp3stream.iInputBufferLength = 1;
       Seek(0);
  }
void cMP3Stream::Seek(int offset)

  {
     newSeek=offset;
     return;
  };

void cMP3Stream::Reset() {
   ExitMP3(&mp);
   InitMP3(&mp);
   this->mybufofs = 0;
   this->mybufdataleft = 0;
}

int cMP3Stream::GetBytes(void* extbuf,int extbuflen)
   {
      if(newSeek!=-1) {
        mp3Seek(&mp3stream,newSeek, MP3_FILE_BEGIN);
        newSeek=-1;
        InitMP3(&mp);
      }

    int res = 0;
nexttry:
    // CASE DATA DECODED
    if(mybufdataleft>0) {

       // in buffer data for new request
       if(mybufdataleft>=extbuflen) {
         CopyMemory(extbuf,&mybuf[mybufofs],extbuflen);
         mybufdataleft -=extbuflen;
         mybufofs += extbuflen;
         res += extbuflen;
         return res;
       }

       // in buffer part of data (needed more)
       if(extbuflen>mybufdataleft) {
         CopyMemory(extbuf,&mybuf[mybufofs],mybufdataleft);
         mybufofs += mybufdataleft;
         extbuf = (void*)(int(extbuf)+mybufdataleft);
         extbuflen -= mybufdataleft;
         res += mybufdataleft;
         mybufdataleft = 0;
       }
    }

    int ret;
    int size = 0;
    // IF THERE IS IN DECODER SOME DATA
    ret = decodeMP3(&mp,NULL,0,mybuf,sizeof(mybuf),&size);
    if(ret==MP3_ERR) InitMP3(&mp);    // prevent program crash

    if(ret==MP3_OK) {
      mybufofs = 0;
      mybufdataleft = size;
      goto nexttry;
    }

    // NEW DATA REQUEST

    FRAMEHEADER fh;


    int stream_ret=getnFrame(&mp3stream,&fh,mp3stream.iInputBufferLength);
    if ( stream_ret == STREAM_ERROR) {
//       ExitMP3(&mp);
       return res;
    }

    if (stream_ret == STREAM_END) {
       return res;
    }

    ret = decodeMP3(&mp,fh.spos.pos,fh.spos.size,mybuf,sizeof(mybuf),&size);
    if(ret==MP3_ERR)
      InitMP3(&mp);    // prevent program crash

    if(ret==MP3_OK) {
      mybufofs = 0;
      mybufdataleft = size;
      goto nexttry;
    }

    goto nexttry;
  };

