
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "waveread_write.h"


int main(int argn, char* argv[]) 
{
	printf("starting....\r\n");

   if(argn != 3)
   {
    printf("please input 2 parameters\r\n");
    return -1;
   }
    WavReader(argv[1]);
  
    WavWriter(argv[2]);
   
    printf("end....\r\n");
	
  return 0;	
	
}