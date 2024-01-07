#include "NeutrinoSteppingAction.hh"
#include "NeutrinoRunAction.hh"
#include "NeutrinoAnalysis.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4VProcess.hh"
#include "G4Material.hh"
#include "G4VPhysicalVolume.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ProcessManager.hh"
#include "G4OpticalPhoton.hh"
#include "NormalTrackInfo.hh"

NeutrinoSteppingAction::NeutrinoSteppingAction()
{
}

NeutrinoSteppingAction::~NeutrinoSteppingAction()
{
}

void NeutrinoSteppingAction::UserSteppingAction(const G4Step* aStep)
{
    G4Track* aTrack = aStep->GetTrack();

    NormalTrackInfo* info = (NormalTrackInfo*)(aTrack->GetUserInformation());
    if(!info) { return; }

    // Set rayleigh:
    if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "OpRayleigh") {
        info -> setRayleigh();
        info -> addRayleighNumber();
    }

    // Set Al reflection
    G4StepPoint* prePoint   = aStep->GetPreStepPoint();
    G4StepPoint* postPoint  = aStep->GetPostStepPoint();
    if( aTrack->GetDefinition() == G4OpticalPhoton::Definition() and postPoint->GetStepStatus() == fGeomBoundary ) {
        G4String preMatName  = prePoint->GetMaterial()->GetName();
        G4String postMatName = postPoint->GetMaterial()->GetName();
        
        if (preMatName == "Aluminium" and postMatName == "LS") {
            // a preliminary criteria for reflection on Al
            info -> setReflection();
            info -> addReflection();

            G4double ref_x = postPoint->GetPosition().getX();
            G4double ref_y = postPoint->GetPosition().getY();
            G4double ref_z = postPoint->GetPosition().getZ();

            info -> setRefPosX(ref_x);
            info -> setRefPosY(ref_y);
            info -> setRefPosZ(ref_z);
        }
    }


    //G4OpBoundaryProcess* bdproc = new G4OpBoundaryProcess();
    //G4OpBoundaryProcessStatus reflectionStatus = bdproc->GetStatus();
    //G4cout << "Track ID " << aTrack->GetTrackID() << " has boundary status " << reflectionStatus << G4endl;
    //if (reflectionStatus == LambertianReflection) {
    //    G4cout << "A photon is lambertian reflected..." << G4endl;
    //}




    if (0 and aTrack->GetDefinition() == G4OpticalPhoton::Definition()) {
    //G4StepPoint* prePoint   = aStep->GetPreStepPoint();
    //G4StepPoint* postPoint  = aStep->GetPostStepPoint();


      const G4VProcess* postPointProc = postPoint->GetProcessDefinedStep();
      G4String procName;
      if (!postPointProc) procName = "None";
      else
        procName = postPointProc->GetProcessName();

      G4String preMatName  = prePoint->GetMaterial()->GetName();
      G4String postMatName ;
      G4Material* postMat = postPoint->GetMaterial();
      if (!postMat)
          postMatName = "None";
      else
          postMatName = postPoint->GetMaterial()->GetName();

     if (preMatName == "Aluminium" and postMatName == "LS") {
        G4cout << "Step process is " << procName << G4endl;
     }

    //if (postMatName == "Aluminium") {
    //  G4OpBoundaryProcess* boundaryProcess = dynamic_cast<G4OpBoundaryProcess*>(aStep->GetPostStepPoint()->GetProcessDefinedStep());
    //    G4cout << boundaryProcess->GetStatus()  << G4endl;
    //}

     /// G4String preVolName = prePoint->GetPhysicalVolume()->GetName();
     /// G4String postVolName ;
     /// G4VPhysicalVolume* postVol = postPoint->GetPhysicalVolume();
     /// if(!postVol) postVolName = "None";
     /// else
     ///     postVolName = postVol->GetName();
     /// 
     /// postVolName = postPoint->GetPhysicalVolume()->GetName();

      //G4cout << "This is " << aTrack->GetCurrentStepNumber() << " step of track " << aTrack->GetTrackID() 
      //       << "[ " << prePoint->GetPosition().getX() << ", " << prePoint->GetPosition().getY() << ", " << prePoint->GetPosition().getZ() << " ] in " << preMatName
      //       << "[ " << postPoint->GetPosition().getX() << ", " << postPoint->GetPosition().getY() << ", " << postPoint->GetPosition().getZ() << " ] in " << postMatName
      //       <<  " " << procName
      //       << preMatName << "(" << preVolName << ")" << " through " 
      //       << procName << " in " << postMatName << "(" << postVolName << ")  "
      //       << "\n"
      //       << G4endl;

      //G4cout << "This is " << aTrack->GetCurrentStepNumber() << " step of track " << aTrack->GetTrackID() 
      //       << " from volume " << preVolName << " --> through " << procName << " <-- to volume "
      //       << postVolName
      //       << G4endl;


    }

    //NeutrinoAnalysis::Instance()->UserSteppingAction(aStep);
}
