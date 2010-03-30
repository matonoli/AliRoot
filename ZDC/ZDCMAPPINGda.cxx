/*

This program reads the DAQ data files passed as argument using the monitoring library.

The program reports about its processing progress.

Messages on stdout are exported to DAQ log system.

DA to write mapping for ADC modules and VME scaler

Contact: Chiara.Oppedisano@to.infn.it
Link: 
Run Type: PHYSICS, CALIBRATION_BC, CALIBRATION_CENTRAL, 
	  CALIBRATION_MB, CALIBRATION_SEMICENTRAL, CALIBRATION_COSMIC
DA Type: MON
Number of events needed: 1 (SOD is read) 
Input Files:  none
Output Files: ZDCChMapping.dat
Trigger Types Used: different trigger types are used

*/

#define MAPDATA_FILE  "ZDCChMapping.dat"

#include <stdio.h>
#include <stdlib.h>
#include <Riostream.h>

// DATE
#include <event.h>
#include <monitor.h>
#include <daqDA.h>

//ROOT
#include <TFile.h>

//AliRoot
#include <AliRawReaderDate.h>
#include <AliRawEventHeaderBase.h>
#include <AliZDCRawStream.h>

int main(int argc, char **argv) {

  const Char_t* tableSOD[]  = {"ALL", "no", "SOD", "all", NULL, NULL};
  monitorDeclareTable(const_cast<char**>(tableSOD));
  
  int status = 0;
  int const kNModules = 10;
  int const kNChannels = 24;
  int const kNScChannels = 32;

  /* log start of process */
  printf("\n ZDC MAPPING program started\n");  

  /* check that we got some arguments = list of files */
  if (argc<2) {
    printf("Wrong number of arguments\n");
    return -1;
  }
  
  FILE *mapFile4Shuttle;

  /* read the data files */
  int n;
  for(n=1;n<argc;n++){
   
    status=monitorSetDataSource( argv[n] );
    if (status!=0) {
      printf("monitorSetDataSource() failed : %s\n",monitorDecodeError(status));
      return -1;
    }
    
    /* declare monitoring program */
    status=monitorDeclareMp( __FILE__ );
    if (status!=0) {
      printf("monitorDeclareMp() failed : %s\n",monitorDecodeError(status));
      return -1;
    }
    monitorSetNowait();
    monitorSetNoWaitNetworkTimeout(1000);

    struct eventHeaderStruct *event;
    eventTypeType eventT;

    Int_t iev = 0;
    Bool_t sodRead = kFALSE;
    while(!sodRead && iev<1000){
 
      /* check shutdown condition */
      if (daqDA_checkShutdown()) {break;}

      /* get next event */
      status=monitorGetEventDynamic((void **)&event);
      if(status==MON_ERR_EOF){
        printf ("End of File detected\n");
        break; /* end of monitoring file has been reached */
      }
      if(status!=0) {
        printf("monitorGetEventDynamic() failed : %s\n",monitorDecodeError(status));
        return -1;
      }

      /* retry if got no event */
      if(event==NULL) continue;
      
      // Initalize raw-data reading and decoding
      AliRawReader *reader = new AliRawReaderDate((void*)event);
      reader->Select("ZDC");
      // --- Reading event header
      //UInt_t evtype = reader->GetType();
      //printf("\t ZDCMAPPINGda -> run # %d\n",reader->GetRunNumber());
      //
      AliZDCRawStream *rawStreamZDC = new AliZDCRawStream(reader);
        
	
      /* use event - here, just write event id to result file */
      eventT=event->eventType;
      
      if(eventT==START_OF_DATA){
	  	
      
      Int_t iMod=-1;
      Int_t modGeo[kNModules], modType[kNModules],modNCh[kNModules];
      for(Int_t kl=0; kl<kNModules; kl++){
         modGeo[kl]=modType[kl]=modNCh[kl]=0;
      }
  
      Int_t ich=0;
      Int_t adcMod[2*kNChannels], adcCh[2*kNChannels], sigCode[2*kNChannels];
      Int_t det[2*kNChannels], sec[2*kNChannels];
      for(Int_t y=0; y<2*kNChannels; y++){
        adcMod[y]=adcCh[y]=sigCode[y]=det[y]=sec[y]=0;
      }
      
      Int_t iScCh=0;
      Int_t scMod[kNScChannels], scCh[kNScChannels], scSigCode[kNScChannels];
      Int_t scDet[kNScChannels], scSec[kNScChannels];
      for(Int_t y=0; y<kNScChannels; y++){
        scMod[y]=scCh[y]=scSigCode[y]=scDet[y]=scSec[y]=0;
      }
	
	rawStreamZDC->SetSODReading(kTRUE);
	
	// --------------------------------------------------------
	// --- Writing ascii data file for the Shuttle preprocessor
        mapFile4Shuttle = fopen(MAPDATA_FILE,"w");
	if(!rawStreamZDC->Next()) printf(" \t No raw data found!! \n");
        else{
	  while((rawStreamZDC->Next())){
            if(rawStreamZDC->IsHeaderMapping()){ // mapping header
	       iMod++;
	       modGeo[iMod]  = rawStreamZDC->GetADCModule();
	       modType[iMod] = rawStreamZDC->GetModType();
	       modNCh[iMod]  = rawStreamZDC->GetADCNChannels();
	    }
            if(rawStreamZDC->IsChMapping()){ 
	      if(modType[iMod]==1){ // ADC mapping ----------------------
	        adcMod[ich]  = rawStreamZDC->GetADCModFromMap(ich);
	        adcCh[ich]   = rawStreamZDC->GetADCChFromMap(ich);
	        sigCode[ich] = rawStreamZDC->GetADCSignFromMap(ich);
	        det[ich]     = rawStreamZDC->GetDetectorFromMap(ich);
	        sec[ich]     = rawStreamZDC->GetTowerFromMap(ich);
	        ich++;
	      }
	      else if(modType[iMod]==2){ //VME scaler mapping --------------------
	        scMod[iScCh]     = rawStreamZDC->GetScalerModFromMap(iScCh);
	        scCh[iScCh]      = rawStreamZDC->GetScalerChFromMap(iScCh);
	        scSigCode[iScCh] = rawStreamZDC->GetScalerSignFromMap(iScCh);
	        scDet[iScCh]     = rawStreamZDC->GetScDetectorFromMap(iScCh);
	        scSec[iScCh]     = rawStreamZDC->GetScTowerFromMap(iScCh);
	        iScCh++;
	      }
	    }
 	  }
	  // Writing data on output FXS file
	  for(Int_t is=0; is<kNModules; is++){
	     fprintf(mapFile4Shuttle,"\t%d\t%d\t%d\n",
	     modGeo[is],modType[is],modNCh[is]);
	     //printf("  Mapping DA -> Module mapping: geo %d type %d #ch %d\n",
	     //  modGeo[is],modType[is],modNCh[is]);
	  }
	  for(Int_t is=0; is<2*kNChannels; is++){
	     fprintf(mapFile4Shuttle,"\t%d\t%d\t%d\t%d\t%d\t%d\n",
	       is,adcMod[is],adcCh[is],sigCode[is],det[is],sec[is]);
    	     //printf("  Mapping DA -> %d ADC: mod %d ch %d, code %d det %d, sec %d\n",
	     //  is,adcMod[is],adcCh[is],sigCode[is],det[is],sec[is]);
	  }
	  for(Int_t is=0; is<kNScChannels; is++){
	     fprintf(mapFile4Shuttle,"\t%d\t%d\t%d\t%d\t%d\t%d\n",
	       is,scMod[is],scCh[is],scSigCode[is],scDet[is],scSec[is]);
 	     //if(scMod[is]!=0) printf("  Mapping DA -> %d Scaler: mod %d ch %d, code %d det %d, sec %d\n",
	     //  is,scMod[is],scCh[is],scSigCode[is],scDet[is],scSec[is]);
	  }
	  
	}
        fclose(mapFile4Shuttle);
      }// SOD event
      else{ 
        if(sodRead){
	  printf("\t SOR read -> exiting from DA\n");
	  break;
	}
	else continue;
      }
      
      iev++; 

      /* free resources */
      free(event);
    }    
      
  }
  
  /* store the result files on FES */
  status = daqDA_FES_storeFile(MAPDATA_FILE, "MAPPING");
  if(status){
    printf("Failed to export file : %d\n",status);
    return -1;
  }

  return status;
}
