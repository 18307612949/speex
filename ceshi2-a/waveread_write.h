#ifndef WAVEREAD_WRITE_H
#define WAVEREAD_WRITE_H


 extern float *data_in;    //指向读入数据空间
 extern float *data_out;    //指向读出数据空间
 extern int num_sample_; // sample points per channel
 
 void WavReader(const char *filename);
 void WavWriter(const char *filename);









#endif














