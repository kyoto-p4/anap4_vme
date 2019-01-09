#include <stdio.h>
#include <cfortran.h>
#include <kernlib.h>
#include <hbook.h>
#include "anap4.h"

int histdef(){
  int i,j;
  char hnam[100];

  /*
  for(i=0;i<N_QDC;i++){
    sprintf(hnam,"QDC %02d",i);
    HBOOK1(10+i,hnam,4096,0.,4096.,0);
  }
  */

  for(i=0;i<N_TDC;i++){
    sprintf(hnam,"TDC %02d",i);
    HBOOK1(20+i,hnam,4096,0.,16384.,0);
  }
  HBOOK1(5000,"TDC ch-mon 0",128,0.,128.,0);
  /*2018/06/08 homework */
  for(i=0;i<N_ADC;i++){
    for(j=0;j<N_ADC_MOD;j++){
      sprintf(hnam,"ADC mod:%1d ch:%02d",j,i);
      HBOOK1(100*(j+1)+i,hnam,2048,0.,4096.,0);
    }
  }
  for(j=0;j<N_ADC_MOD;j++){
    sprintf(hnam,"ch-counter mod:%d",j);
    HBOOK1(1000+j,hnam,32,0.,32.,0);
  }
  for(j=0;j<N_ADC_MOD;j++){
    sprintf(hnam,"ch-monitor mod:%d",j);
    HBOOK2(2000+j,hnam,32,0.,32.,128,0.,4096.,0);
  }

  return(1);
}
