#ifdef HAVE_CONFIG_H
//#include "config.h"
#endif

#include "speex_preprocess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "waveread_write.h"

#define FRAME_SIZE 1152
#define FRAME_SAMPLERATE 44000
#define DENOISE_DB (-90)

int main(int argn, char* argv[]) 
{
  char* szInFilename = NULL;
  char* szOutFilename = NULL;
  //һ֡�Ĵ洢�ռ�
  float in[FRAME_SAMPLERATE];

  int i,j,k,z;
  SpeexPreprocessState *st;
  float f;

  printf("starting....\r\n");

  if(argn != 3)
  {
  printf("please input 2 parameters\r\n");
  return -1;
  }
  //�洢�����ļ���
  szInFilename = argv[1];
  szOutFilename = argv[2];

//SPEEX��ʼ��
st = speex_preprocess_state_init(FRAME_SIZE, FRAME_SAMPLERATE);

int denoise = 1;
int noiseSuppress = DENOISE_DB;
speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_DENOISE, &denoise); //����
speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_NOISE_SUPPRESS, &noiseSuppress); //����������dB
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
speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_VAD, &vad); //�������
speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_PROB_START , &vadProbStart);      //Set probability required for the VAD to go from silence to voice
speex_preprocess_ctl(st, SPEEX_PREPROCESS_SET_PROB_CONTINUE, &vadProbContinue); //Set probability required for the VAD to stay in the voice state (integer percent)



//��ȡwav�ļ�---Ĭ�϶�������Ϊ��ͨ��
 WavReader(szInFilename);
 
for(j=0;j<(num_sample_/FRAME_SIZE);j++)
//��֡����-����β��
{ 
  //����һ֡
  for(k=0;k<(FRAME_SIZE);k++)
  {
	in[k]=data_in[j*FRAME_SIZE+k];
	printf("data_in=%f\r\n",data_in[j*FRAME_SIZE+k]);
  }
  //SPEEX_����
   vad = speex_preprocess_run(st, in);
  //��������
   if(vad != 0)
   {
    printf("speech.\r\n");
   }
   else
   {
    printf("slience############################\r\n");
   }
   //д����һ֡
  for(z=0;z<(FRAME_SIZE);z++)
  {
	data_out[j*FRAME_SIZE+z]=in[z];
	printf("data_out=%f\r\n",data_out[j*FRAME_SIZE+z]);
  }
   
}
//д��wav�ļ�
 WavWriter(szOutFilename);

//����speex�ռ�
speex_preprocess_state_destroy(st);
//����malloc����ռ�
free(data_in);
free(data_out);

return 0;
}