#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UserTrackingAction.hh"

#include "NeutrinoPrimaryGeneratorAction.hh"
#include "NeutrinoDetectorConstruction.hh"

#include "NeutrinoRunAction.hh"
#include "NeutrinoEventAction.hh"
#include "NeutrinoTrackingAction.hh"
#include "NeutrinoSteppingAction.hh"

#include "NeutrinoPhysicsList.hh"
#include "G4VModularPhysicsList.hh"

#include "G4VisManager.hh"
#include "G4VisExecutive.hh"

#include "G4UIExecutive.hh"
#include "G4UImanager.hh"

//#include <pybind11/pybind11.h>

int main(int argc, char **argv){

    G4UIExecutive* ui = 0;
    if (argc == 1) {
        ui = new G4UIExecutive(argc, argv);
    }

    char* macName = NULL;
    int   rdmSeed = 1001;

    for(int i=1;i<argc;i++){
        if( strcmp(argv[i],"-seed") == 0 ){
            i++;
            rdmSeed = std::atoi(argv[i]);
        }
        else if( strstr(argv[i], ".mac") ){
            macName = argv[i];
        }
    }

    CLHEP::HepRandom::setTheSeed(rdmSeed);

    G4RunManager* runManager = new G4RunManager;
    /* Initialization classes (mandatory) */
    // detector
    G4VUserDetectorConstruction* detectorConstruction = new NeutrinoDetectorConstruction();
    runManager->SetUserInitialization(detectorConstruction);
    
    // physics list
    G4VModularPhysicsList* physicsList = new NeutrinoPhysicsList;
    runManager->SetUserInitialization(physicsList);
    
    // generator
    G4VUserPrimaryGeneratorAction* genAction = new NeutrinoPrimaryGeneratorAction(); 
    runManager->SetUserAction(genAction);

    /* User Action classes */
    // run action
    NeutrinoRunAction* runAction = new NeutrinoRunAction();
    runManager->SetUserAction(runAction);
    
    // event action
    NeutrinoEventAction* eventAction = new NeutrinoEventAction(); 
    runManager->SetUserAction(eventAction);
    
    // tracking action
    G4UserTrackingAction* trackingAction = new NeutrinoTrackingAction();
    runManager->SetUserAction(trackingAction);
    
    // stepping action
    G4UserSteppingAction* steppingAction = new NeutrinoSteppingAction();
    runManager->SetUserAction(steppingAction);

    // runManager->Initialize();

    /* Visualization */
    // initialize visualization
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();

    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    if ( ! ui ) { 
      // batch mode
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }
    else { 
      // interactive mode
      UImanager->ApplyCommand("/control/execute init_vis.mac");
      ui->SessionStart();
      delete ui;
    }
    //if (!macName) 
    //{
    //    macName = "vis.mac";
    //}

    //G4String command = "/control/execute ";
    //UImanager->ApplyCommand(command + macName);

    delete visManager;
    delete runManager;

    return 0;
}




