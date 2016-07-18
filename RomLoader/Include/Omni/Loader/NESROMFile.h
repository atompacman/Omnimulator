#ifndef OMNIMULATOR_ROMLOADER_NESROMFILE
#define OMNIMULATOR_ROMLOADER_NESROMFILE

/*================================================================================================\\
| Loads NES ROM files from disk
|--------------------------------------------------------------------------------------------------|
| File path is specified in constructor. Interprets and validate the header and extract data banks.
\=================================================================================================*/

#include <array>
#include <vector>

#include <Omni/Common/Common.h>

namespace Omni { namespace Loader {

// TODO : move this enum in the PPM project when it will exist
enum class Mirroring
{
    HORIZONTAL, 
    VERTICAL, 
    FOUR_SCREEN
};

uint8_t const OFFICIAL_FILE_TAG[] = "\x4e\x45\x53\x1a";

uint32_t const HEADER_SIZE   = 1 << 4;
uint32_t const PRG_BANK_SIZE = 1 << 14;
uint32_t const CHR_BANK_SIZE = 1 << 13;
uint32_t const TRAINER_SIZE  = 1 <<  9;

typedef std::array<uint8_t, PRG_BANK_SIZE> PRGBank;
typedef std::array<uint8_t, CHR_BANK_SIZE> CHRBank;
typedef std::array<uint8_t, TRAINER_SIZE > Trainer;

class NESROMFile
{
public:

    explicit NESROMFile(std::string const & i_FilePath);

    // Data banks
    std::vector<PRGBank> const & getPRGBanks() const { return m_PRGBanks;      }
    std::vector<CHRBank> const & getCHRBanks() const { return m_CHRBanks;      }
    Trainer              const * getTrainer () const { return m_Trainer.get(); }

    // Other info
    uint8_t   getNumPGRRAMBanks      () const { return m_NumPGRRAMBanks;          }
    bool      hasBatteryBackedRAMBank() const { return m_HasBatteryBackedRAMBank; }
    uint8_t   getMapperNumber        () const { return m_MapperNumber;            }
    Mirroring getMirroringType       () const { return m_Mirroring;               }

private:

    // Data banks
    std::vector<PRGBank> m_PRGBanks;
    std::vector<CHRBank> m_CHRBanks;
    uptr<Trainer>        m_Trainer;

    // Other info
    uint8_t   m_NumPGRRAMBanks;
    bool      m_HasBatteryBackedRAMBank;
    uint8_t   m_MapperNumber;
    Mirroring m_Mirroring;
};

}}

#endif // OMNIMULATOR_ROMLOADER_NESROMFILE 