
#include "NeutrinoDetectorSD.hh"
#include "NormalTrackInfo.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

NeutrinoDetectorSD::NeutrinoDetectorSD( const G4String& name, 
                  const G4String& hitsCollectionName)
    : G4VSensitiveDetector(name),
    fHitsCollection(NULL)
{
    collectionName.insert(hitsCollectionName);

    analysis = NeutrinoAnalysis::Instance();
}

NeutrinoDetectorSD::~NeutrinoDetectorSD()
{;}


void NeutrinoDetectorSD::Initialize(G4HCofThisEvent* hce)
{
    // Create hits collection
    fHitsCollection
        = new NeutrinoDetectorHitsCollection( SensitiveDetectorName, collectionName[0]);
    
    // Add this collection in hce
    G4int hcID 
        = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    hce->AddHitsCollection( hcID, fHitsCollection );


    // Create hits
    //for( G4int i=0; i<fNofPmts+1; i++ )  {
    //    fHitsCollection->insert(new NeutrinoDetectorHit());
    //}
}

G4bool NeutrinoDetectorSD::ProcessHits( G4Step* aStep, G4TouchableHistory*)
{
    G4double edep = aStep->GetTotalEnergyDeposit();
    G4double stepLength = aStep->GetStepLength();
    if(edep == 0. && stepLength == 0. ) return false;

    auto touchable = (aStep->GetPreStepPoint()->GetTouchable());

    // Get pmt id
    auto pmtNumber = touchable->GetReplicaNumber(1);

    //auto hit = (*fHitsCollection)[pmtNumber];
    //if ( ! hit ) {
    //    G4ExceptionDescription msg;
    //    msg << "Cannot access hit " << pmtNumber; 
    //    G4Exception("B4cCalorimeterSD::ProcessHits()",
    //            "MyCode0004", FatalException, msg);
    //}         

    G4Track* track      = aStep->GetTrack();
    G4int trackId       = track->GetTrackID();
    G4double energy     = track->GetKineticEnergy();
    G4double time       = aStep->GetPreStepPoint()->GetGlobalTime();

    G4int m_num_scintillation   = 0;
    G4int m_num_rayleigh        = 0;
    G4int m_num_reflection      = 0;

    G4VUserTrackInformation* trkinfo = track->GetUserInformation();
    if(trkinfo) {
        NormalTrackInfo* normaltrk = dynamic_cast<NormalTrackInfo*>(trkinfo);
        if (normaltrk) {
            m_num_scintillation = normaltrk->getScintillationNumber();
            m_num_rayleigh      = normaltrk->getRayleighNumber();
            m_num_reflection    = normaltrk->getReflectionNumber();
        }
    }
    
    // set and insert hits:
    NeutrinoDetectorHit* hit = new NeutrinoDetectorHit();
    hit->SetTrackID(trackId);
    hit->SetEnergy(energy);
    hit->SetTime(time);
    hit->SetRefNum(m_num_reflection);
    hit->SetRayNum(m_num_rayleigh);
    hit->SetScintNum(m_num_scintillation);
    fHitsCollection->insert(hit);


    // write to the ROOT file:
    analysis->analysePEenergy(energy);
    analysis->analysePEtime(time);



    //G4Track* track = aStep->GetTrack();
    //analysis-> analyseDetPolX(track->GetPolarization().getX());
    //analysis-> analyseDetPolY(track->GetPolarization().getY());
    //analysis-> analyseDetPolZ(track->GetPolarization().getZ());

    //G4ThreeVector pos = aStep->GetPreStepPoint()->GetPosition();    
    //G4double posx = pos.x();
    //G4double posy = pos.y();
    //G4double posz = pos.z();
    //analysis->analyseDetPosX(posx);
    //analysis->analyseDetPosY(posy);
    //analysis->analyseDetPosZ(posz);



    return true;
}



void NeutrinoDetectorSD::EndOfEvent(G4HCofThisEvent*)
{
    G4int nofHits = fHitsCollection->entries();
    analysis->analyseTotalPENumber(nofHits);
  if ( verboseLevel>1 ) { 
     G4int nofHits = fHitsCollection->entries();
     G4cout << G4endl
            << "-------->Hits Collection: in this event they are " << nofHits 
            << " hits in the tracker chambers: " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
}
