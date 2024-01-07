#ifndef NormalTrackInfo_hh
#define NormalTrackInfo_hh


#include "globals.hh"
#include "G4VUserTrackInformation.hh"

#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

#include <vector>
#include <map>

// Ref: http://geant4.slac.stanford.edu/Tips/

class NormalTrackInfo: public G4VUserTrackInformation {
    public:
        NormalTrackInfo();
        NormalTrackInfo(const G4Track* aTrack);
        NormalTrackInfo(const NormalTrackInfo* aTrackInfo);
        virtual ~NormalTrackInfo(); 


        inline void *operator new(size_t);
        inline void operator delete(void *aTrackInfo);
        inline int operator ==(const NormalTrackInfo& right) const {
            return (this==&right);
        }
    public:
        inline G4int GetOriginalTrackID() const {return originalTrackID;}

        inline void setFromCerenkov() {from_cerenkov = true;}
        inline G4bool isFromCerenkov(){return from_cerenkov;}

        inline void setReemission() {is_reemission = true;}
        inline G4bool isReemission(){return is_reemission;}
        inline void addScintillationNumber() { m_num_scintillation += 1; }
        inline G4int getScintillationNumber() {return m_num_scintillation;}

        inline void setRayleigh() {is_rayleigh = true;}
        inline G4bool isRayleigh() {return is_rayleigh;}
        inline void addRayleighNumber() { m_num_rayleigh += 1;}
        inline G4int getRayleighNumber() { return m_num_rayleigh; }

        inline void setOriginalOP() { m_op_is_original_op = true; }
        inline G4bool isOriginalOP() { return m_op_is_original_op; }

        inline void setParentName(const G4String& parent_name) { m_parent_name = parent_name; }
        inline const G4String& getParentName() const { return m_parent_name; }

        inline void addEdep(G4double edep) { m_edep += edep; }
        inline const G4double getEdep()    { return m_edep; }

        inline void addQedep(G4double qedep) { m_Qedep += qedep; }
        inline const G4double getQedep()     { return m_Qedep; }

        inline void setReflection() { is_reflection = true;}
        inline G4bool isReflection() { return is_reflection; }
        inline void addReflection() { m_num_reflection += 1; }
        inline G4int getReflectionNumber() { return m_num_reflection; }

        inline void setOPenergy(G4double val) { m_op_energy = val; }
        inline const G4double getOPenergy()   { return m_op_energy; }

        inline void setRefPosX(G4double val)  { m_ref_posx.push_back(val); }
        inline const std::vector<float> getRefPosX() { return m_ref_posx; }

        inline void setRefPosY(G4double val)  { m_ref_posy.push_back(val); }
        inline const std::vector<float> getRefPosY() { return m_ref_posy; }

        inline void setRefPosZ(G4double val)  { m_ref_posz.push_back(val); }
        inline const std::vector<float> getRefPosZ() { return m_ref_posz; }


    private:
        G4int originalTrackID;

        G4bool from_cerenkov;
        G4bool is_reemission;

        G4bool is_rayleigh;

        G4bool is_reflection;

        G4int m_num_scintillation;
        G4int m_num_rayleigh;
        G4int m_num_reflection;

        std::vector<float> m_ref_posx;
        std::vector<float> m_ref_posy;
        std::vector<float> m_ref_posz;

        // the original OP info
        // Here, original means the parent of OP is not OP.
        G4bool m_op_is_original_op;

        // info of Parent Track
        G4String m_parent_name;

        // energy deposit
        G4double m_edep;
        G4double m_Qedep;

        G4double m_op_energy;
};

extern G4ThreadLocal G4Allocator<NormalTrackInfo>* aNormalTrackInformationAllocator;


inline void* NormalTrackInfo::operator new(size_t)
{ 
    void* aTrackInfo;
    if (!aNormalTrackInformationAllocator) {
        aNormalTrackInformationAllocator = new G4Allocator<NormalTrackInfo>;
    }
    aTrackInfo = (void*)aNormalTrackInformationAllocator->MallocSingle();
    return aTrackInfo;
}

inline void NormalTrackInfo::operator delete(void *aTrackInfo)
{ aNormalTrackInformationAllocator->FreeSingle((NormalTrackInfo*)aTrackInfo);}

#endif
