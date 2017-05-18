#ifdef HAVE_CONFIG_H
//#include "config.h"
#endif

#include "speex_preprocess.h"
#include <stdio.h>

#define FRAME_SIZE 1152
#define FRAME_SAMPLERATE 44000
#define DENOISE_DB (-90)

int main(int argn, char* argv[]) {
char* szInFilename = NULL;
char* szOutFilename = NULL;
FILE* pInFileHandle = NULL;
FILE* pOutFileHandle = NULL;

short in[FRAME_SAMPLERATE];

int i;
SpeexPreprocessState *st;
int count=0;

float f;

printf("starting....\r\n");

if(argn != 3){
printf("please input 2 parameters\r\n");
return -1;
}

//memset((void*)empty, 0, sizeof(empty));

szInFilename = argv[1];
szOutFilename = argv[2];

pInFileHandle = fopen(szInFilename, "rb");
if(!pInFileHandle){
printf("open file %s error\r\n", szInFilename);
return -2;
}

pOutFileHandle = fopen(szOutFilename, "wb");
if(!pOutFileHandle){
printf("open file %s error\r\n", szOutFilename);
fclose(pInFileHandle);
return -3;
}

st = speex_preprocess_state_init(FRAME_SIZE, FRAME_SAMPLERATE);

int denoise = 1;
int noiseSuppress = DENOISE_DB;
speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DENOISE, &denoise); //Ωµ‘Î
speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noiseSuppress); //…Ë÷√‘Î…˘µƒdB
i=0;
speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC, &i);
i=8000;
speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_AGC_LEVEL, &i);
i=0;
speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB, &i);
f=.0;
speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_DECAY, &f);
f=.0;
speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DEREVERB_LEVEL, &f);

int vad = 1;
int vadProbStart = 80;
int vadProbContinue = 65;
speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_VAD, &vad); //æ≤“ÙºÏ≤‚
speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_PROB_START , &vadProbStart); //Set probability required for the VAD to go from silence to voice
speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_PROB_CONTINUE, &vadProbContinue); //Set probability required for the VAD to stay in the voice state (integer percent)

while (1)
{
int vad;
int iLen = fread(in, sizeof(short), FRAME_SIZE, pInFileHandle);
 if(iLen <= 0)
 {
   break;
 }

 if (feof(pInFileHandle))
  
 break;

vad = speex_preprocess_run(st, in);

 if(vad != 0)
 {
  printf("speech.\r\n");
  fwrite(in, sizeof(short), FRAME_SIZE, pOutFileHandle);
 }
 else
 {
  printf("slience############################\r\n");
  fwrite(in, sizeof(short), FRAME_SIZE, pOutFileHandle);
 }
 count++;
}
speex_preprocess_state_destroy(st);

fclose(pInFileHandle);
fclose(pOutFileHandle);

return 0;
}