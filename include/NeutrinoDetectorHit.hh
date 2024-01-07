#ifndef NeutrinoDetectorHit_h
#define NeutrinoDetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

class NeutrinoDetectorHit : public G4VHit
{
    public: 
        NeutrinoDetectorHit();
        NeutrinoDetectorHit(const NeutrinoDetectorHit&);
        virtual ~NeutrinoDetectorHit();

        // operators
        const NeutrinoDetectorHit& operator=(const NeutrinoDetectorHit&);
        G4int operator==(const NeutrinoDetectorHit&) const;

        inline void* operator new       (size_t);
        inline void  operator delete    (void*);

        // methods from base class
        virtual void Draw();
        virtual void Print();

        // Set methods:
        void SetTrackID(G4int trackId)      { fTrackID = trackId; }
        void SetEnergy(G4double energy)     { fEnergy = energy; }
        void SetTime(G4double time)         { fTime = time; }

        void SetRefNum(G4int val)           { fRefNum = val; }
        void SetRayNum(G4int val)           { fRayNum = val; }
        void SetScintNum(G4int val)         { fScintNum = val; }

        // Get methods:
        G4int GetTrackID() const;
        G4double GetEnergy() const;
        G4double GetTime() const;

    private:
        G4int       fTrackID;
        G4double    fEnergy;
        G4double    fTime;

        G4int       fRefNum;        // number of reflection
        G4int       fRayNum;        // number of Rayleigh scattering
        G4int       fScintNum;      // number of scintillation


};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<NeutrinoDetectorHit> NeutrinoDetectorHitsCollection;

extern G4ThreadLocal G4Allocator<NeutrinoDetectorHit>* NeutrinoDetectorHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* NeutrinoDetectorHit::operator new(size_t)
{
  if(!NeutrinoDetectorHitAllocator)
      NeutrinoDetectorHitAllocator = new G4Allocator<NeutrinoDetectorHit>;
  return (void *) NeutrinoDetectorHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void NeutrinoDetectorHit::operator delete(void *hit)
{
  NeutrinoDetectorHitAllocator->FreeSingle((NeutrinoDetectorHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4int NeutrinoDetectorHit::GetTrackID() const {
    return fTrackID;
}




#endif
