#ifndef ALITRDCLUSTERINFO_H
#define ALITRDCLUSTERINFO_H

/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */


#ifndef Root_TObject
#include "TObject.h"
#endif

class AliTRDcluster;
class AliTRDclusterInfo : public TObject
{
public:
  AliTRDclusterInfo();

  Float_t   GetAnisochronity() const {return fD;}
  inline void GetCluster(Int_t &det, Float_t &x, Float_t &y, Float_t &z, Float_t &q, Int_t &t) const;
  void      GetMC(Int_t &pdg, Int_t &label) const{
      pdg  = fPdg;
      label= fLbl; }
  void      GetGlobalPosition(Float_t &yt, Float_t &zt, Float_t &dydx, Float_t &dzdx, Float_t *cov) const {
      dydx = fdydx;
      dzdx = fdzdx;
      yt   = fYt;
      zt   = fZt;
      memcpy(cov, fCov, 3*sizeof(Float_t));}
  Float_t   GetResolution() const {return fdy;}
  Float_t   GetDriftLength() const {return fXd;}

  void      Print(Option_t *opt="") const;

  void      SetAnisochronity(Float_t d) {fD = d;}
  void      SetCluster(const AliTRDcluster *c=0x0);
  void      SetMC(Int_t pdg, Int_t label){
      fPdg  = pdg;
      fLbl  = label;}
  void      SetGlobalPosition(Float_t yt, Float_t zt, Float_t dydx, Float_t dzdx, Float_t *cov=0x0) {
      fdydx = dydx;
      fdzdx = dzdx;
      fYt   = yt;
      fZt   = zt;
      if(cov) memcpy(fCov, cov, 3*sizeof(Float_t));}
  void      SetResolution(Float_t dy) {fdy = dy;}
  void      SetDriftLength(Float_t d) {fXd = d;}

private:
  UShort_t fDet;   // detector
  Short_t  fPdg;   // particle code
  Short_t fLbl;    // track label (MC)
  Short_t fLocalTime; // calibrate drift time
  Float_t  fQ;     // cluster charge (REC)
  Float_t  fX;     // x coordinate (REC)
  Float_t  fY;     // y coordinate (REC)
  Float_t  fZ;     // z coordinate (REC)
  Float_t  fdydx;  // slope in phi (MC)
  Float_t  fdzdx;  // slope in theta (MC)
  Float_t  fXd;    // drift length
  Float_t  fYt;    // y coordinate (MC)
  Float_t  fZt;    // z coordinate (MC)
  Float_t  fCov[3];// covariance matrix in the yz plane (global)
  Float_t  fdy;    // difference in y after tilt correction
  Float_t  fD;     // distance to the anode wire

  ClassDef(AliTRDclusterInfo, 1) // extracted cluster2MC information
};


//_________________________________________________
inline void AliTRDclusterInfo::GetCluster(Int_t &det, Float_t &x, Float_t &y, Float_t &z, Float_t &q, Int_t &t) const
{
  det = fDet;
  x   = fX;
  y   = fY;
  z   = fZ;
  q   = fQ;
  t   = fLocalTime;
}

#endif

