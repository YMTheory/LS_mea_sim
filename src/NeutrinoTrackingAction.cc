#include "NeutrinoTrackingAction.hh"
#include "NormalTrackInfo.hh"
#include "NeutrinoRunAction.hh"
#include "NeutrinoAnalysis.hh"
#include "G4OpticalPhoton.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4EventManager.hh"
#include "G4TrackingManager.hh"

NeutrinoTrackingAction::NeutrinoTrackingAction()
{
}

NeutrinoTrackingAction::~NeutrinoTrackingAction()
{
}

void NeutrinoTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
    if (0 and aTrack->GetParentID() != 0 ) {
    G4double initx = aTrack->GetPosition().getX();
    G4double inity = aTrack->GetPosition().getY();
    G4double initz = aTrack->GetPosition().getZ();
    G4String creator = aTrack->GetCreatorProcess()->GetProcessName();
    G4cout << "Track initial information: "
           << aTrack->GetDefinition()->GetParticleName() << " " 
           << aTrack->GetParentID() << " "
           << creator << " "
           << aTrack->GetTrackID() << " " << initx 
                                   << " " << inity
                                   << " " << initz
                                   << G4endl;
    }

    if (0 and ( aTrack->GetDefinition() == G4OpticalPhoton::Definition()) ) { 
        G4double initx = aTrack->GetPosition().getX();
        G4double inity = aTrack->GetPosition().getY();
        G4double initz = aTrack->GetPosition().getZ();
        G4double energy = aTrack->GetKineticEnergy();
        G4String creator = aTrack->GetCreatorProcess()->GetProcessName();
        G4cout << "Track initial information: "
               << aTrack->GetDefinition()->GetParticleName() << " " 
               << aTrack->GetParentID() << " "
               << creator << " "
               << energy << " "
               << aTrack->GetTrackID() << " " << initx 
                                       << " " << inity
                                       << " " << initz
                                       << G4endl;
        
    }
    //G4cout << "Load from NormalTrackInfo : " << info->isReemission() << " " << info->getScintillationNumber() 
    //       << " or " << info->isFromCerenkov() 
    //       << G4endl;
    
    
    if (aTrack->GetParentID() == 0 and aTrack->GetUserInformation()==0) {
        NormalTrackInfo* anInfo = new NormalTrackInfo(aTrack);
        G4Track* theTrack = (G4Track*)aTrack;
        theTrack->SetUserInformation(anInfo);

        if (aTrack->GetDefinition() == G4OpticalPhoton::Definition() ) {
            anInfo->setOPenergy( aTrack->GetKineticEnergy() );
        }
    }

    if (aTrack->GetDefinition() == G4OpticalPhoton::Definition()) { 
        NormalTrackInfo* anInfo = (NormalTrackInfo*)aTrack->GetUserInformation();
        if (anInfo->getOPenergy() == 0) {
            anInfo -> setOPenergy( aTrack->GetKineticEnergy() );
        }
        
    }


    // judge if a OP and count scint number -> seems has logic error...
    //if (aTrack->GetDefinition() == G4OpticalPhoton::Definition()) {
    //    NormalTrackInfo* info = (NormalTrackInfo*)(aTrack->GetUserInformation());
    //    if (info->isReemission()) {
    //        info->addScintillationNumber();
    //    }
    //    if (aTrack->GetParentID() == 1) { // Set OP if parent ID = 1
    //        info->setOriginalOP();
    //    }
    //}

    NeutrinoAnalysis::Instance()->PreUserTrackingAction(aTrack);
}

void NeutrinoTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
    //G4TrackStatus trackStatus = aTrack->GetTrackStatus();
    //G4cout << "PostUserTrackingAction -> " << trackStatus << G4endl;
    //NeutrinoAnalysis::Instance()->PostUserTrackingAction(aTrack);

    G4TrackingManager* tm = G4EventManager::GetEventManager()->GetTrackingManager();

    G4TrackVector* secondaries = tm->GimmeSecondaries();
    if(secondaries)
    {
        NormalTrackInfo* info = (NormalTrackInfo*)(aTrack->GetUserInformation());

        if (!info) {
             return;
        }

        size_t nSeco = secondaries->size();
        if(nSeco>0)
        {
            // loop over all secondaries :
            for(size_t i=0;i<nSeco;i++)
            { 
                //NormalTrackInfo* infoNew = new NormalTrackInfo();
                NormalTrackInfo* infoNew = new NormalTrackInfo(info);

                // cerenkov photons tag
                if((*secondaries)[i]->GetCreatorProcess() 
                   and (*secondaries)[i]->GetDefinition() == G4OpticalPhoton::Definition()
                   and (*secondaries)[i]->GetCreatorProcess()->GetProcessName() == "Cerenkov" ) {
                    infoNew->setFromCerenkov();
                }

                // reemission tag
                // + parent track is an OP
                // + secondary is also an OP
                // + the creator process is Scintillation
                if (//aTrack->GetDefinition()==G4OpticalPhoton::Definition() and
                    (*secondaries)[i]->GetDefinition() == G4OpticalPhoton::Definition()
                    and( (*secondaries)[i]->GetCreatorProcess()->GetProcessName() == "Scintillation" or (*secondaries)[i]->GetCreatorProcess()->GetProcessName() == "OpAbsReemit" )) {
                    infoNew->setReemission();
                    infoNew->addScintillationNumber();
                }

                // original photons tag
                if (aTrack->GetDefinition() != G4OpticalPhoton::Definition()
                    and (*secondaries)[i]->GetDefinition() == G4OpticalPhoton::Definition()) {
                    infoNew->setOriginalOP();
                }

                // Rayleigh scattering tag, no more action
                (*secondaries)[i] -> SetUserInformation(infoNew);
            }
        }
    } 

    // outputs:
    //if (aTrack->GetDefinition() == G4OpticalPhoton::Definition()) {
    //    NormalTrackInfo* info = (NormalTrackInfo*)(aTrack->GetUserInformation()) ;
    //    G4cout << "PostTrackId = " << aTrack->GetTrackID() << " and parent ID = " << aTrack->GetParentID() 
    //           << " creator process is " << aTrack->GetCreatorProcess()->GetProcessName()
    //           << " is Original Photon ---> " << info->isOriginalOP() 
    //           << " is Cerenkov Photon ---> " << info->isFromCerenkov()
    //           << " is Scintillation Photon ---> " << info->isReemission()
    //           << G4endl;
    //}

    
}
