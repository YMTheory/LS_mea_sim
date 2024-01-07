#ifndef NeutrinoPMTSD_h
#define NeutrinoPMTSD_h 1

#include "NeutrinoDetectorHit.hh"
#include "G4VSensitiveDetector.hh"
#include "NeutrinoAnalysis.hh"

class G4Step;
class G4HCofThisEvent;

class NeutrinoPMTSD : public G4VSensitiveDetector
{
    public:
        NeutrinoPMTSD(const G4String& name, 
                const G4String& hitsCollectionName);
        ~NeutrinoPMTSD();

        void Initialize     (G4HCofThisEvent* HCE);
        G4bool ProcessHits  (G4Step* aStep, G4TouchableHistory* ROhist);
        void EndOfEvent     (G4HCofThisEvent* hitCollection);


    private:
        NeutrinoDetectorHitsCollection* fHitsCollection;

        NeutrinoAnalysis* analysis;

    public:
        double get_ce(const G4ThreeVector local_pos);


};
#endif
