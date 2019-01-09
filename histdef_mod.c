#include <stdio.h>
#include <cfortran.h>
#include <kernlib.h>
#include <hbook.h>
#include "anap4.h"

int histdef(){
  int i;
  char hnam[100];

  /*
  for(i=0;i<N_QDC;i++){
    sprintf(hnam,"QDC %02d",i);
    HBOOK1(10+i,hnam,4096,0.,4096.,0);
  }
  */

  for(i=0;i<N_TDC;i++){
    sprintf(hnam,"TDC %02d",i);
    HBOOK1(20+i,hnam,4096,0.,4096.,0);
  }
  /*2018/06/08 homework */
  for(i=0;i<N_ADC;i++){
    for(j=0;j<N_ADC_MOD;j++){
      sprintf(hnam,"ADC mod:%1d ch:%02d",j,i);
      HBOOK1(100*(j+1)+i,hnam,2048,0.,4096.,0);
    }
  }
  for(i=0;i<N_ADC;i++){
    for(j=0;j<N_ADC_MOD;j++){
      sprintf(hnam,"ADC mod:%1d ch_re:%02d",j,i);
      HBOOK1(100*(j+1_N_ADC_MOD)+i,hnam,2048,0.,4096.,0);
    }
  }
  for(j=0;j<N_ADC_MOD;j++){
    sprintf(hnam,"ch-counter mod:%d",j);
    HBOOK1(10+j,hnam,32,0.,32.);
  }

  for(j=0;j<N_ADC_MOD;j++){
    sprintf(hnam,"ch_re-counter mod:%d",j);
    HBOOK1(10+j+N_ADC_MOD,hnam,32,0.,32.);
  }
  
  for(j=0;j<N_ADC_MOD;j++){
    sprintf(hnam,"xcount vs ycount mod:%d",j);
    HBOOK1(500,hnam,10,0.,10.);
  }

  return(1);
}
