/**
 * \class DsWrapPhysConsOptical
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


#ifndef DSWRAPPHYSCONSOPTICAL_H
#define DSWRAPPHYSCONSOPTICAL_H

#include "G4VPhysicsConstructor.hh"

class G4VPhysicsConstructor;

class DsWrapPhysConsOptical : public G4VPhysicsConstructor
{

public:

    DsWrapPhysConsOptical(const G4String& name = "optical");
                     
    virtual ~DsWrapPhysConsOptical();

    // Interface methods
    void ConstructParticle();
    void ConstructProcess();
    void SetOpticalProcess(G4VPhysicsConstructor * b){ m_optical_process = b ;}

private:
    G4VPhysicsConstructor * m_optical_process ; 

};

#endif  // DSPHYSCONSOPTICAL_H
