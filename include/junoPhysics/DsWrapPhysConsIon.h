/**
 * \class DsWrapPhysConsIon
 *
 * \brief Construct Optical Processes
 *
 * 
 *
 *
 *
 * bv@bnl.gov Tue Jan 29 15:25:22 2008
 *
 */


#ifndef DSWRAPPHYSCONSION_H
#define DSWRAPPHYSCONSION_H

#include "G4VPhysicsConstructor.hh"

class G4VPhysicsConstructor;

class DsWrapPhysConsIon : public G4VPhysicsConstructor
{

public:

    DsWrapPhysConsIon(const G4String& name);
                     
    virtual ~DsWrapPhysConsIon();

    // Interface methods
    void ConstructParticle();
    void ConstructProcess();

private:
    G4VPhysicsConstructor * m_ion_process ; 
    bool m_enable_ionPHP  ; // enable G4IonPhysicsPHP process
};

#endif 
