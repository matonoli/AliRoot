#ifndef ALIESDFRIENDTRACK_H
#define ALIESDFRIENDTRACK_H

//-------------------------------------------------------------------------
//                     Class AliESDfriendTrack
//               This class contains ESD track additions
//       Origin: Iouri Belikov, CERN, Jouri.Belikov@cern.ch 
//-------------------------------------------------------------------------

#include <TObject.h>
#include <AliExternalTrackParam.h>

class AliTrackPointArray;
class AliKalmanTrack;
class TObjArrray;
//_____________________________________________________________________________
class AliESDfriendTrack : public TObject {
public:
  enum {
    kMaxITScluster=12,
    kMaxTPCcluster=160,
    kMaxTRDcluster=180
  };
  AliESDfriendTrack();
  AliESDfriendTrack(const AliESDfriendTrack &t);
  virtual ~AliESDfriendTrack();

  void Set1P(Float_t p) {f1P=p;}
  void SetTrackPointArray(AliTrackPointArray *points) {
    fPoints=points;
  }
  Float_t Get1P() const  {return f1P;}
  Int_t *GetITSindices() {return fITSindex;}
  Int_t *GetTPCindices() {return fTPCindex;}
  Int_t *GetTRDindices() {return fTRDindex;}
  const AliTrackPointArray *GetTrackPointArray() const {return fPoints;}

  void SetITStrack(AliKalmanTrack *t) {fITStrack=t;}
  void SetTRDtrack(AliKalmanTrack *t) {fTRDtrack=t;}
  AliKalmanTrack *GetTRDtrack() {return fTRDtrack;}
  AliKalmanTrack *GetITStrack() {return fITStrack;}
  void AddCalibObject(TObject * calibObject); 
  TObject * GetCalibObject(Int_t index);
  //
  // parameters backup
  void SetTPCOut(const AliExternalTrackParam &param);
  void SetITSOut(const AliExternalTrackParam &param);
  void SetTRDIn(const AliExternalTrackParam  &param);
  //
  const AliExternalTrackParam * GetTPCOut(){return  fTPCOut;} 
  const AliExternalTrackParam * GetITSOut() { return fITSOut;} 
  const AliExternalTrackParam * GetTRDIn() { return fTRDIn;} 

  void SetITSIndices(Int_t* indices, Int_t n);
  void SetTPCIndices(Int_t* indices, Int_t n);
  void SetTRDIndices(Int_t* indices, Int_t n);

  Int_t GetMaxITScluster() {return fnMaxITScluster;}
  Int_t GetMaxTPCcluster() {return fnMaxTPCcluster;}
  Int_t GetMaxTRDcluster() {return fnMaxTRDcluster;}

protected:
  Float_t f1P;                     // 1/P (1/(GeV/c))
  Int_t fnMaxITScluster; // Max number of ITS clusters
  Int_t fnMaxTPCcluster; // Max number of TPC clusters
  Int_t fnMaxTRDcluster; // Max number of TRD clusters
  Int_t* fITSindex; //[fnMaxITScluster] indices of the ITS clusters 
  Int_t* fTPCindex; //[fnMaxTPCcluster] indices of the TPC clusters
  Int_t* fTRDindex; //[fnMaxTRDcluster] indices of the TRD clusters

  AliTrackPointArray *fPoints;//Array of track space points in the global frame
  TObjArray      *fCalibContainer; //Array of objects for calibration    
  AliKalmanTrack *fITStrack; //! pointer to the ITS track (debug purposes) 
  AliKalmanTrack *fTRDtrack; //! pointer to the TRD track (debug purposes) 
  //
  //
  AliExternalTrackParam * fTPCOut; // tpc outer parameters
  AliExternalTrackParam * fITSOut; // its outer parameters
  AliExternalTrackParam * fTRDIn;  // trd inner parameters

private:
  AliESDfriendTrack &operator=(const AliESDfriendTrack & /* t */) {return *this;}

  ClassDef(AliESDfriendTrack,4) //ESD friend track
};

#endif


