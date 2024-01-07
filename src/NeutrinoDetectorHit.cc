#include "NeutrinoDetectorHit.hh"

G4ThreadLocal G4Allocator<NeutrinoDetectorHit>* NeutrinoDetectorHitAllocator = 0;

NeutrinoDetectorHit::NeutrinoDetectorHit()
    : G4VHit(),
    fTrackID(0)
{}


NeutrinoDetectorHit::~NeutrinoDetectorHit()
{;}


NeutrinoDetectorHit::NeutrinoDetectorHit( const NeutrinoDetectorHit& right)
    : G4VHit()
{
    fTrackID = right.fTrackID;
}



const NeutrinoDetectorHit& NeutrinoDetectorHit::operator=(const NeutrinoDetectorHit& right)
{
    fTrackID = right.fTrackID;

    return *this;
}

G4int NeutrinoDetectorHit::operator==(const NeutrinoDetectorHit& right) const
{
    return (  this == &right ) ? 1 : 0;
}
void NeutrinoDetectorHit::Draw()
{

}

void NeutrinoDetectorHit::Print()
{
    G4cout << " trackID: " << fTrackID << G4endl;
}






