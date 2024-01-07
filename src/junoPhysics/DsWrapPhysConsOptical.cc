#include "DsWrapPhysConsOptical.h"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

DsWrapPhysConsOptical::DsWrapPhysConsOptical(const G4String& name): G4VPhysicsConstructor(name)
                                                          
{
   m_optical_process = NULL;
}

DsWrapPhysConsOptical::~DsWrapPhysConsOptical()
{
}

void DsWrapPhysConsOptical::ConstructParticle()
{
}



void DsWrapPhysConsOptical::ConstructProcess()
{
    m_optical_process -> ConstructProcess();
}
