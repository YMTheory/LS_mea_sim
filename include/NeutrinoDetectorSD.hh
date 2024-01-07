
#ifndef NeutrinoDetectorSD_h
#define NeutrinoDetectorSD_h 1

#include "G4VSensitiveDetector.hh"
#include "NeutrinoDetectorHit.hh"
#include "NeutrinoAnalysis.hh"

class G4Step;
class G4HCofThisEvent;

class NeutrinoDetectorSD : public G4VSensitiveDetector
{
    public:
        NeutrinoDetectorSD(const G4String& name,
                const G4String& hitsCollectionName);
        virtual ~NeutrinoDetectorSD();

        // methods from base class
        virtual void   Initialize  (G4HCofThisEvent* hitCollection);
        virtual G4bool ProcessHits (G4Step* step, G4TouchableHistory* history);
        virtual void   EndOfEvent  (G4HCofThisEvent* hitCollection);

    private:
        NeutrinoDetectorHitsCollection* fHitsCollection;
        NeutrinoAnalysis* analysis;
};

#endif
