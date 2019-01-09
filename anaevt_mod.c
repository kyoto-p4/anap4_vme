#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cfortran.h>
#include <hbook.h>
#include <kernlib.h>

#include "kinema.h"
#include "anap4.h"

int anaevt(int evtlen,unsigned short *rawbuf,struct p4dat *dat){
  int i,ip,j;
  int segsize,ids,ips,ipsn,count;
  //  unsigned short qdc[N_QDC];
  unsigned short tdc[N_TDC];
  unsigned int adc[N_ADC_MOD][N_ADC];
  unsigned int adc_re[N_ADC_MOD][N_ADC];//real channel asign
  //  double qdcc[N_QDC];
  double tdcc[N_TDC];
  double adcc[N_ADC_MOD][N_ADC];
  double adcc_re[N_ADC_MOD][N_ADC];//real channel asign
  const double tpar[N_TDC][2]={{0.0, 0.01}, {0.0, 0.01},{0.0, 0.01},{0.0, 0.01}};
  int ix[N_ADC_MOD][3],iy[N_ADC_MOD][3];
  char tmpbuf[200];
  double adc_thre=100.;
  int nx[N_ADC_MOD],ny[N_ADC_MOD]; 
 
  /************* Clear Event Buffer *****************/
  /*
  for(i=0;i<N_QDC;i++) {
    qdc[i]=0;
    qdcc[i]=0.;
  }
  */
  for(i=0;i<N_TDC;i++){
    tdc[i]=0;
    tdcc[i]=0.;
  }
  for(i=0;i<N_ADC;i++){
    for(j=0;j<N_ADC_MOD;j++){
      adc[j][i]=0;
      adcc[j][i]=0.;
    }
  }
  count=0;

  /************* Clear Event Buffer *****************/

  /************* Decode Event Data Here *****************/
  ip=0;
  while(ip<evtlen){  
    int tmpdat,tmpch,tmpnwd;

    /** Segment Header ************/
    segsize=rawbuf[ip++];
    ipsn=ip+segsize-1;
    ids=rawbuf[ip++];
    
#if _DEBUG
    ips=0;
    printf(" New Seg %d\n",ids);
#endif
    
    while(ip<ipsn){ /*** Segment loop ***/
      switch(ids){
      case 1:  /*** MADC32 ****/     
	tmpdat=rawbuf[ip++];
	tmpdat+=(rawbuf[ip++]*0x10000);
	if((tmpdat & 0xc0000000)==0x40000000){ /* Header */
	  unsigned int tmpmod,tmpnwd;
	  tmpmod=((tmpdat>>16)&0x00ff);
	  tmpnwd=(tmpdat&0x3ff);
#if _DEBUG
	  printf("  MADC32:ID %02x:Count %2d\n",tmpmod,tmpnwd);
#endif
	  for(i=0;i<tmpnwd;i++){
	    tmpdat=rawbuf[ip++];
	    tmpdat+=(rawbuf[ip++]*0x10000);
	    switch(tmpdat & 0xc0000000){
	    case 0x0000000: /* Data */
	      if((tmpdat & 0xffe00000)==0x04000000) { /* Data event */
		tmpch=(tmpdat & 0x01f0000) >> 16;
		//tmpch += (tmpmod&0x1)*32;
		//adc[tmpch]=(tmpdat & 0xffff);
		adc[(tmpmod&0x1)][tmpch]=(tmpdat & 0xffff);
#if _DEBUG
		printf("  ADC:0x%08x:  Ch:%2d  Adc:%4d\n", 
		       tmpdat,tmpch,adc[tmpch]);
		/*
		  sprintf(tmpbuf,"ADC:   %2d: %08x  ch:%02d  adc:%4d\n",
		  i, tmpdat,tmpch, qdc[i]);
		  dumpmsg(tmpbuf);*/
#endif
	      }
	      else {
		printf("  MADC32: Non event data (0x%08x).\n",tmpdat);
	      }
	      break;
	    case 0xc0000000: /* End of Event data */
#if _DEBUG
	      printf("  ADC:0x%08x:  Timestamp:0x%08x\n",
		     tmpdat,(tmpdat & 0x3fffffff));
#endif
	      break;
	    default:
	      printf("  MADC32:Unknown data format (%08x)\n",tmpdat);
	    }
	  }
	}
	else{
	  printf("  MADC32:Wrong header word (%08x)\n",tmpdat);
	}
	break;
      default:
	ip++;ip++; /* Skip segment ids != 1 */
	break;
      }
    }
  }
/**** Decode Event Data Above***************/

/****adc channel reset HERE****/
  for(j=0;j<N_ADC_MOD;j++){
    for(i=16;i<24;i++){
      adc_re[j][i] = adc[j][39-i];
      adcc_re[j][i] = adcc[j][39-i];
    }
    for(i=24;i<32;i++){
      adc_re[j][i] = adc[j][55-i];
      adcc_re[j][i] = adcc[j][55-i];
    }
  }
/****adc channel reset ABOVE****/
 
  /**** Data Analysis Here ***************/

  /* Energy calibration */
  /*
  for(i=0;i<N_QDC;i++){
    qdcc[i]=1.*qdc[i];
  }
  */

  /* time calibration */
  for(i=0;i<N_TDC;i++) {
    tdcc[i]=(tdc[i]-tpar[i][0])*tpar[i][1];
  }
  

  /* ADC calibration */
  for(i=0;i<N_ADC;i++){
    for(j=0;j<N_ADC_MOD;j++){
      adcc[i]=(double)adc[i];
    }
  }
 
  /**** Data Analysis Above ***************/
  
  /*********** Booking here **********/

#if 0  
  /*
  for(i=0;i<N_QDC;i++){
    HF1(10+i,qdc[i],1.0);
  }
  */
  for(j=0;j<N_ADC_MOD;j++){
    for(i=0;i<N_ADC;i++){
      HF1(10+j,i,1.0);
    }
  }
  //real channel asign
 for(j=0;j<N_ADC_MOD;j++){
   if(16<=i && i<24){i=39-i;}
   else if(i<=24){i=55-i;}
   else i=i;
   for(i=0;i<N_ADC;i++){
      HF1(10+j+N_ADC_MOD,i,1.0);
    }
  }

  for(i=0;i<N_TDC;i++){
    HF1(20+i,tdc[i],1.0);
  }
#endif
  
  for(i=0;i<N_ADC;i++){
    for(j=0;j<N_ADC_MOD;j++){
      HF1(100*(j+1)+i,adc[j][i],1.0);
    }
  }
 
  for(i=0;i<N_ADC;i++){
    for(j=0;j<N_ADC_MOD;j++){
      HF1(100*(j+1+N_ADC_MOD)+i,adc_re[j][i],1.0);
    }
  }
 
  for(j=0;j<N_ADC_MOD;j++){
    nx[j] = 0;
    ny[j] = 0;
    for(i=0;i<N_STRIP;i++){
      if(adcc_re[j][i]>adc_thre){
	nx[j] = nx[j]+1;
      }
      else nx[j]= nx[j];
    }
    for(i=N_STRIP;i<N_ADC;i++){
      if(adcc_re[j][i]>adc_thre){      
	ny[j] = ny[j]+1;
      }
      else ny[j] = ny[j];
    }
    HF2(500,nx[j],ny[j],1.0);
  }

  if(n)
  /*********** Booking Above **********/
  return(ip);
}
