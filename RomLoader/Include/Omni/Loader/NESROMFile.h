#ifndef OMNIMULATOR_ROMLOADER_NESROMFILE
#define OMNIMULATOR_ROMLOADER_NESROMFILE

/*================================================================================================\\
| Loads NES ROM files from disk
|--------------------------------------------------------------------------------------------------|
| File path is specified in constructor. Interprets and validate the header and extract data banks.
\=================================================================================================*/

#include <vector>

#include <Omni/Common/Basic.h>
#include <Omni/Common/DataTypes.h>
#include <Omni/Common/Graphic.h>

namespace Omni { namespace Loader {

uint8_t const OFFICIAL_FILE_TAG[] = "\x4e\x45\x53\x1a";

uint32_t const HEADER_SIZE   = 1 << 4;

class NESROMFile
{
public:

    explicit NESROMFile(std::string const & i_FilePath);

    // Data banks
    std::vector<Common::PRGBank> const & getPRGBanks() const { return m_PRGBanks;      }
    std::vector<Common::CHRBank> const & getCHRBanks() const { return m_CHRBanks;      }
    Common::Trainer              const * getTrainer () const { return m_Trainer.get(); }

    // Other info
    uint8_t           getNumPGRRAMBanks      () const { return m_NumPGRRAMBanks;          }
    bool              hasBatteryBackedRAMBank() const { return m_HasBatteryBackedRAMBank; }
    uint8_t           getMapperNumber        () const { return m_MapperNumber;            }
    Common::Mirroring getMirroringType       () const { return m_Mirroring;               }

private:

    // Data banks
    std::vector<Common::PRGBank> m_PRGBanks;
    std::vector<Common::CHRBank> m_CHRBanks;
    uptr       <Common::Trainer> m_Trainer;

    // Other info
    uint8_t           m_NumPGRRAMBanks;
    bool              m_HasBatteryBackedRAMBank;
    uint8_t           m_MapperNumber;
    Common::Mirroring m_Mirroring;
};

}}

#endif // OMNIMULATOR_ROMLOADER_NESROMFILE 