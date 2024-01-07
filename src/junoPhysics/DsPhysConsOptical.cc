#include "DsPhysConsOptical.h"

#include "G4Cerenkov.hh"
#include "G4Cerenkov_modified.hh"
#ifdef WITH_G4OPTICKS
#include "LocalG4Cerenkov1042.hh"
#endif

#include "G4Scintillation.hh"
#include "DsG4Scintillation.h"
#include "DsG4ScintSimple.h"
#include "DsG4OpAbsReemit.h"


#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4ProcessManager.hh"
#include "G4FastSimulationManagerProcess.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

DsPhysConsOptical::DsPhysConsOptical(const G4String& name): G4VPhysicsConstructor(name)
{
    m_cerenMaxPhotonPerStep     = 300;
    m_cerenPhotonStack          = true;

    m_doReemission              = true;
    m_doScintAndCeren           = true;
    m_doReemissionOnly          = false;

    m_useCerenkov               = true;
    m_useCerenkovType           = "modified";
    m_cerenkovYieldFactor       = 1.;
    m_applyWaterQe              = true;

    m_useScintillation          = true;
    m_useRayleigh               = true;
    m_useAbsorption             = true;
    m_useScintSimple            = false;
    m_useAbsReemit              = false;

    m_useFastMu300nsTrick       = false;
    m_ScintillationYieldFactor  = 1.0;
   
    m_enableQuenching           = true;

    m_doFastSim                 = false; // just the fast simulation

    flagDecayTimeFast           = true;
    flagDecayTimeSlow           = true;

    m_doTrackSecondariesFirst   = true;

    m_cerenPhotonScaleWeight    = 1;
    m_scintPhotonScaleWeight    = 1;

    m_birksConstant1            = 15.8e-3*g/cm2/MeV;
    m_birksConstant2            = 1.5e-6*(g/cm2/MeV)*(g/cm2/MeV);
    m_birksConstant2            = 0.;

    theMessenger = new OpticalPhysicsMessenger(this);
}

DsPhysConsOptical::~DsPhysConsOptical()
{
}

void DsPhysConsOptical::ConstructParticle()
{
}

void DsPhysConsOptical::setUseScintSimple(G4bool flag) {
    G4cout << "Set ScintSimple process in DsPhysConsOptical as " << flag << G4endl;
    m_useScintSimple = flag;
}

void DsPhysConsOptical::setUseAbsReemit(G4bool flag) {
    G4cout << "Set AbsReemit process in DsPhysConsOptical as " << flag << G4endl;
    m_useAbsReemit = flag;
}



void DsPhysConsOptical::ConstructProcess()
{
    G4VProcess* cerenkov_ = 0;

    G4cout<<"check: m_useCerenKov == "<< m_useCerenkov <<std::endl;
    G4cout<<"check: m_useScintillation == "<< m_useScintillation  <<std::endl;
    G4cout<<"check:  m_useScintSimple == "<<  m_useScintSimple <<std::endl;
    if (m_useCerenkov) {
        if( m_opticksMode == 0 )
        {
            if (m_useCerenkovType == "modified") {
                G4Cerenkov_modified* cerenkov = new G4Cerenkov_modified() ;
                cerenkov->SetMaxNumPhotonsPerStep(m_cerenMaxPhotonPerStep);
                cerenkov->SetStackPhotons(m_cerenPhotonStack);
                cerenkov->SetTrackSecondariesFirst(m_doTrackSecondariesFirst);
                cerenkov->SetCerenkovYieldFactor(m_cerenkovYieldFactor);
                cerenkov_ = cerenkov ;
            } else if (m_useCerenkovType == "original") {
                G4Cerenkov* cerenkov = new G4Cerenkov() ;
                cerenkov->SetMaxNumPhotonsPerStep(m_cerenMaxPhotonPerStep);
                cerenkov->SetStackPhotons(m_cerenPhotonStack);
                cerenkov->SetTrackSecondariesFirst(m_doTrackSecondariesFirst);
                cerenkov_ = cerenkov ;
            } else {
                G4cerr << __FILE__ << ":" << __LINE__
                       << " Unknown m_useCerenkovType: '"
                       << m_useCerenkovType << "'"
                       << G4endl;
                assert(0);
            }

        } else {
#ifdef WITH_G4OPTICKS
            LocalG4Cerenkov1042* cerenkov = new LocalG4Cerenkov1042(m_opticksMode) ;
            cerenkov->SetMaxNumPhotonsPerStep(m_cerenMaxPhotonPerStep);
            cerenkov->SetTrackSecondariesFirst(m_doTrackSecondariesFirst);
            cerenkov_ = cerenkov ;
#else
            G4cout 
               << __FILE__ << ":" << __LINE__ 
               << " DsPhysConsOptical::ConstructProcess "
               << " FATAL "
               << " non-zero opticksMode requires compilation -DWITH_G4OPTICKS " 
               << " m_useCerenkov " << m_useCerenkov 
               << " m_opticksMode " << m_opticksMode 
               << G4endl
               ;
            assert(0) ;   
#endif
        }  
    }

    G4VProcess* scint_ = 0;

    if (m_useScintillation && 1) { // DsG4 (with re-emission)
        DsG4Scintillation* scint = new DsG4Scintillation(m_opticksMode);

        scint->SetDoQuenching(m_enableQuenching);
        scint->SetBirksConstant1(m_birksConstant1);
        scint->SetBirksConstant2(m_birksConstant2);
        scint->SetDoReemission(m_doReemission);
        scint->SetDoReemissionOnly(m_doReemissionOnly);
        scint->SetDoBothProcess(m_doScintAndCeren);
        scint->SetApplyPreQE(m_scintPhotonScaleWeight>1.);
        scint->SetPreQE(1./m_scintPhotonScaleWeight);
        scint->SetScintillationYieldFactor(m_ScintillationYieldFactor); //1.);
        scint->SetUseFastMu300nsTrick(m_useFastMu300nsTrick);
        scint->SetTrackSecondariesFirst(true);
        scint->SetFlagDecayTimeFast(flagDecayTimeFast);
        scint->SetFlagDecayTimeSlow(flagDecayTimeSlow);
        scint->SetVerboseLevel(0);


        scint_ = scint;

    }else if (m_useScintillation && 0) { // default G4
        G4Scintillation* scint = 0;  
        scint = new G4Scintillation();
        scint->SetScintillationYieldFactor(m_ScintillationYieldFactor);
        scint->SetTrackSecondariesFirst(m_doTrackSecondariesFirst);
        scint_ = scint;
    } 

 if (1 && m_useScintSimple){
        G4cout<<"Scintillation physics process : ScintSample is used"<<std::endl;
        DsG4ScintSimple * scint= new DsG4ScintSimple();
        scint->SetDoQuenching(m_enableQuenching);
        scint->SetBirksConstant1(m_birksConstant1);
        scint->SetBirksConstant2(m_birksConstant2);
        scint->SetDoReemission(m_doReemission);
        scint->SetDoReemissionOnly(m_doReemissionOnly);
        scint->SetDoBothProcess(m_doScintAndCeren);
        scint->SetApplyPreQE(m_scintPhotonScaleWeight>1.);
        scint->SetPreQE(1./m_scintPhotonScaleWeight);
        scint->SetScintillationYieldFactor(m_ScintillationYieldFactor);
        scint->SetUseFastMu300nsTrick(m_useFastMu300nsTrick);
        scint->SetTrackSecondariesFirst(true);
        scint->SetFlagDecayTimeFast(flagDecayTimeFast);
        scint->SetFlagDecayTimeSlow(flagDecayTimeSlow);
        scint->SetVerboseLevel(0);

        scint_ = scint;   


      }

     DsG4OpAbsReemit* absreemit_PPO =0;
     DsG4OpAbsReemit* absreemit_bisMSB =0;
      if (m_useAbsReemit){
          G4cout << "Absorption-reemission process: AbsReemit is used" << G4endl;
                absreemit_PPO= new DsG4OpAbsReemit("PPO");
                absreemit_bisMSB= new DsG4OpAbsReemit("bisMSB");
                 absreemit_PPO->SetVerboseLevel(0);
                 absreemit_bisMSB->SetVerboseLevel(0);
              }


    

    G4OpAbsorption* absorb = 0;
    if (m_useAbsorption) {
        absorb = new G4OpAbsorption();
    }

    G4OpRayleigh* rayleigh = 0;
    if (m_useRayleigh) {
        rayleigh = new G4OpRayleigh();
	    //rayleigh->SetVerboseLevel(2);
    }

    G4OpBoundaryProcess* boundproc = new G4OpBoundaryProcess();
    boundproc->SetInvokeSD(false);


    G4FastSimulationManagerProcess* fast_sim_man = 0;
    if (m_doFastSim) {
        fast_sim_man = new G4FastSimulationManagerProcess("fast_sim_man");
    }

    auto theParticleIterator=G4ParticleTable::GetParticleTable()->GetIterator();
    theParticleIterator->reset();
    while( (*theParticleIterator)() ) {

        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
    
        // Caution: as of G4.9, Cerenkov becomes a Discrete Process.
        // This code assumes a version of G4Cerenkov from before this version.

        if(cerenkov_ && cerenkov_->IsApplicable(*particle)) {
            pmanager->AddProcess(cerenkov_);
            pmanager->SetProcessOrdering(cerenkov_, idxPostStep);
        }

        if(scint_ && scint_->IsApplicable(*particle)) {
            //if (m_useScintSimple) G4cout << "Associate Scintillation with Particle " << (particle->GetParticleName()) << std::endl;

            pmanager->AddProcess(scint_);
            pmanager->SetProcessOrderingToLast(scint_, idxAtRest);
            pmanager->SetProcessOrderingToLast(scint_, idxPostStep);
        }

       /* if (particle == G4OpticalPhoton::Definition()) {
            if (absorb)
                pmanager->AddDiscreteProcess(absorb);
            if (rayleigh)
                pmanager->AddDiscreteProcess(rayleigh);
            pmanager->AddDiscreteProcess(boundproc);
            //pmanager->AddDiscreteProcess(pee);
            if (m_doFastSim) {
                std::cout << "############## Using new PMT optical model!!! " << std::endl;
                pmanager->AddDiscreteProcess(fast_sim_man);
            } else {
                std::cout << "############## Using old PMT optical model!!! " << std::endl;
            }
        }*/
          if(particle==G4OpticalPhoton:: Definition()){
              if (absreemit_PPO)
                  pmanager->AddDiscreteProcess(absreemit_PPO);
              if (absreemit_bisMSB)
                  pmanager->AddDiscreteProcess(absreemit_bisMSB);
              if (absorb)
                  pmanager->AddDiscreteProcess(absorb);
              if (rayleigh)
                  pmanager->AddDiscreteProcess(rayleigh);
              pmanager->AddDiscreteProcess(boundproc);
             //pmanager->AddDiscretePrcess(pee);
              if (m_doFastSim)
                  pmanager->AddDiscreteProcess(fast_sim_man);
           }
  }
}
