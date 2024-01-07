
#include "NormalTrackInfo.hh"
#include "G4ios.hh"

G4ThreadLocal G4Allocator<NormalTrackInfo>* aNormalTrackInformationAllocator = 0;



NormalTrackInfo::NormalTrackInfo()
{
    from_cerenkov =false;
    is_reemission =false;
    is_rayleigh   =false;
    is_reflection =false;
    m_op_is_original_op = false;

    m_edep = 0;
    m_Qedep = 0;

    m_num_scintillation = 0;
    m_num_rayleigh      = 0;
    m_num_reflection    = 0;

    m_op_energy = 0.;

    m_ref_posx.clear();
    m_ref_posy.clear();
    m_ref_posz.clear();
}

NormalTrackInfo::NormalTrackInfo(const G4Track* aTrack)
{
    from_cerenkov =false;
    is_reemission =false;
    is_rayleigh   =false;
    is_reflection =false;

    m_op_is_original_op = false;

    m_edep = 0;
    m_Qedep = 0;

    m_num_scintillation = 0;
    m_num_rayleigh      = 0;
    m_num_reflection    = 0;

    m_op_energy = 0.;

    m_ref_posx.clear();
    m_ref_posy.clear();
    m_ref_posz.clear();
}

NormalTrackInfo::NormalTrackInfo(const NormalTrackInfo* aTrackInfo)
{
    from_cerenkov = aTrackInfo->from_cerenkov;
    is_reemission = aTrackInfo->is_reemission;
    is_rayleigh   = aTrackInfo->is_rayleigh;
    is_reflection = aTrackInfo->is_reflection;

    m_num_scintillation = aTrackInfo->m_num_scintillation;
    m_num_reflection    = aTrackInfo->m_num_reflection;
    m_num_rayleigh      = aTrackInfo->m_num_rayleigh;

    m_ref_posx          = aTrackInfo->m_ref_posx;
    m_ref_posy          = aTrackInfo->m_ref_posy;
    m_ref_posz          = aTrackInfo->m_ref_posz;

    //G4cout << "Info in NormalTrackInfo registered -> " 
    //       << "Cerenkov: "      << from_cerenkov << " " 
    //       << "Scintillation: " << is_reemission << " "  << m_num_scintillation << " "
    //       << "Rayleigh: "      << is_rayleigh   << " " << m_num_rayleigh << " "
    //       << "Reflection: "    << is_reflection << " " << m_num_reflection
    //       << "" << G4endl;

    //if (is_reemission) {
    //    m_num_scintillation += 1;
    //}

    //if (is_rayleigh ) {
    //    m_num_rayleigh += 1;
    //}

    //if (is_reflection) {
    //    m_num_reflection += 1;
    //}
    
    m_op_is_original_op = false;

    m_edep = 0;
    m_Qedep = 0;
    m_op_energy = aTrackInfo->m_op_energy;
}

NormalTrackInfo::~NormalTrackInfo(){;}
