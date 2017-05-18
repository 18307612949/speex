
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "waveread_write.h"
//WAVE��ȡ-Ĭ��Ϊ��ȡ��������ʽ

 //ȫ�ֱ���
 int num_channel_;
 int sample_rate_;
 int bits_per_sample_;
 int num_sample_; // sample points per channel
 float *data_in;    //ָ��������ݿռ�
 float *data_out;   //ָ��������ݿռ�

//ͷ�����ݿ�

typedef  struct WavHeader
{
    char riff[4]; // "riff"
    unsigned int size;  
    char wav[4];  // "WAVE"
    char fmt[4];  // "fmt "
    unsigned int fmt_size; // 
    unsigned short format; // 
    unsigned short channels; 
    unsigned int sample_rate; 
    unsigned int bytes_per_second; // 
    unsigned short block_size; 
    unsigned short bit;  // 
    char data[4]; // "data"
    unsigned int data_size; //
}WavHeader1;



//��ȡ���ݺ�ͷ����Ϣ
void WavReader(const char *filename)
  {
        FILE *fp = fopen(filename, "r");
        if (NULL == fp) {
            perror(filename);
            exit(1);
        }
        
        WavHeader1 header;

	  //����wavͷ��
        fread(&header, 1, sizeof(header), fp);
        if (header.fmt_size < 16) 
		{
            printf("WaveData: expect PCM format data to have fmt chunk of at least size 16.\n");
            exit(1);
        }
        else if (header.fmt_size > 16) {
            int offset = 44 - 8 + header.fmt_size - 16;
            fseek(fp, offset, SEEK_SET);
            fread(header.data, 8, sizeof(char), fp);
        }
        // check "riff" "WAVE" "fmt " "data"
        // only support one sub data chunk
        num_channel_ = header.channels;  //ͨ��
        sample_rate_ = header.sample_rate; //������
        bits_per_sample_ = header.bit;        //��������λ��
        int num_data = header.data_size / (bits_per_sample_ / 8); //���ݿ�ĵ���λ�����������û�����
        data_in = malloc(sizeof(float)*num_data);
		data_out = malloc(sizeof(float)*num_data);
        num_sample_ = num_data / num_channel_;   //�������ж��ٸ�������  
       
        for (int i = 0; i < num_data; i++) 
	   {
            switch (bits_per_sample_) 
		{       
		        //Ĭ�϶�ȡ���������� --��ͨ�������޸�
                case 8: 
				{
                    char sample;
                    fread(&sample, 1, sizeof(char), fp);
                    data_in[i] = (float)sample;
                    break;
                }
                case 16: 
				{
                    short sample;
                    fread(&sample, 1, sizeof(short), fp);
                    data_in[i] = (float)sample;
                    break;
                }
                case 32: 
				{
                    int sample;
                    fread(&sample, 1, sizeof(int), fp);
                    data_in[i] = (float)sample;
                    break;
                }
                default:
                    fprintf(stderr, "unsupported quantization bits");
                    exit(1);
            }
        }
        fclose(fp);
    }
	
	//�����ݴ����wav��ʽ
  void WavWriter(const char *filename) 
	{
        FILE *fp = fopen(filename, "w");
        // init char 'riff' 'WAVE' 'fmt ' 'data'
        WavHeader1 header;
        char wav_header[44] = 
		{
            0x52, 0x49, 0x46, 0x46, 0x00, 0x00, 0x00, 0x00,
            0x57, 0x41, 0x56, 0x45, 0x66, 0x6d, 0x74, 0x20,
            0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x64, 0x61, 0x74, 0x61,
            0x00, 0x00, 0x00, 0x00
        };
		//��Ĭ��ֵ
        memcpy(&header, wav_header, sizeof(header));
		//д�������
        header.channels = num_channel_;
        header.bit = bits_per_sample_;
        header.sample_rate = sample_rate_;
        header.data_size = num_sample_ * num_channel_ * (bits_per_sample_ / 8);
        header.size = sizeof(header) - 8 + header.data_size;
        header.bytes_per_second = sample_rate_ * num_channel_ * 
            (bits_per_sample_ / 8);
        header.block_size = num_channel_ * (bits_per_sample_ / 8);

        fwrite(&header, 1, sizeof(header), fp);
        //�ļ��ж��ٲ�����
        for (int i = 0; i < num_sample_; i++) 
		{
			//��ͨ��
           // for (int j = 0; j < num_channel_; j++) 
			{   
		        //������λ��	
                switch (bits_per_sample_) 
				{
					//Ĭ�϶�ȡ���������� --��ͨ�������޸�
                    case 8: 
					{
                        char sample = (char)data_out[i];
                        fwrite(&sample, 1, sizeof(sample), fp);
                        break;
                    }
                    case 16: 
					{
                        short sample = (short)data_out[i];
                        fwrite(&sample, 1, sizeof(sample), fp);
                        break;
                    }
                    case 32: 
					{
                        int sample = (int)data_out[i];
                        fwrite(&sample, 1, sizeof(sample), fp);
                        break;
                    }
                }
            }
        }
        fclose(fp);
    }	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	