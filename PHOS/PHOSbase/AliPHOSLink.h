#ifndef ALIPHOSLINK_H
#define ALIPHOSLINK_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//_________________________________________________________________________
//  Algorithm class used only by AliPHOSTrackSegmentMaker 
//  Links recpoints
// into tracksegments                
//*-- Author: Dmitri Peressounko (SUBATECH)

// --- ROOT system ---

#include "TObject.h"

// --- Standard library ---

// --- AliRoot header files ---

class AliPHOSLink : public  TObject{
  
public:
  
  AliPHOSLink() ;  // ctor            
  AliPHOSLink( Float_t x, Float_t z, Int_t emc, Int_t cpv, Int_t track) ;  // ctor            
  virtual ~AliPHOSLink(){
    // dtor
  }
  Int_t   Compare(const TObject * obj) const;
  Int_t   GetEmc(void) const { return fEmcN; }
  Int_t   GetCpv(void) const { return fCpvN ; }
  void    GetXZ(Float_t &dx, Float_t &dz) const { dx=fx ; dz = fz ; } 
  Int_t   GetTrack(void) const { return fTrack ; }
  Bool_t  IsSortable() const{ 
    // tells if this is a sortable object 
    return kTRUE ; 
  }
  
private:
  
  Int_t fEmcN ;  // Emc index
  Int_t fCpvN ;  // Cpv index 
  Int_t fTrack;  // Charged tracked within a minimum distance of the EMC
  Float_t fx ;   // Distance between EMC and CPV RecPoints in a track segment
  Float_t fz ;   // Distance between EMC and CPV RecPoints in a track segment
  
  ClassDef(AliPHOSLink,2)  // Auxilliary algorithm class used by AliPHOSTrackSegmentMaker

};

#endif // AliPHOSLINK_H
