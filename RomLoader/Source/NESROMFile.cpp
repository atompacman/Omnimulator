#include <fstream>

#include <Omni/Loader/NESROMFile.h>

namespace Omni { namespace Loader {

struct HeaderContent
{
    uint8_t m_MagicNumber[4];
    uint8_t m_NumPRGBanks;
    uint8_t m_NumCHRBanks;

    union
    {
        uint8_t m_Byte;
        struct
        {
            uint8_t m_IsHorizMirroring        : 1;
            uint8_t m_HasBatteryBackedRam     : 1;
            uint8_t m_HasTrainer              : 1;
            uint8_t m_UsesFourScreenMirroring : 1;
            uint8_t m_MapperNumberLSB         : 4;
        } ;
    } m_CtrlByte1;

    union
    {
        uint8_t m_Byte;
        struct
        {
            uint8_t m_Unused          : 4;
            uint8_t m_MapperNumberMSB : 1;
        };
    } m_CtrlByte2;

    uint8_t m_NumRAMBanks;

    uint8_t m_Unused[7];
};

NESROMFile::NESROMFile(std::string const & i_FilePath) :
    // Data banks
    m_PRGBanks ()
  , m_CHRBanks ()
  , m_Trainer  (nullptr)

    // Other info
  , m_NumPGRRAMBanks          (0)
  , m_HasBatteryBackedRAMBank (false)
  , m_MapperNumber            (0)
  , m_Mirroring               (Common::Mirroring::HORIZONTAL)
{
    // Open file
    std::basic_ifstream<uint8_t> file(i_FilePath, std::ios::ate | std::ios::binary);
    
    // Check if file exists
    if (file.bad())
    {
        LOG(FATAL) << "File not found \"" << i_FilePath << "\"" << std::endl;
    }

    // Check minimum file size for header
    uint64_t size = file.tellg();
    if (size < HEADER_SIZE)
    {
        LOG(FATAL) << "Incomplete header: file is too small";
    }
    file.seekg(file.beg);

    LOG(INFO) << "Reading iNES file \"" << i_FilePath << "\"" << std::endl;

    // Read header content
    HeaderContent header;
    file.read(reinterpret_cast<uint8_t *>(&header), sizeof(header));

    // Check file magic number
    if (memcmp(header.m_MagicNumber, OFFICIAL_FILE_TAG, 4) != 0)
    {
        LOG(WARNING) << "File should start with the " << OFFICIAL_FILE_TAG << " characters";
    }

    // Check that the MSB of the control byte 2 are zeros
    if (header.m_CtrlByte2.m_Unused != 0)
    {
        LOG(WARNING) << "First half of control byte 2 should be zeros";
    }

    // Check that the rest of the header is zero
    static uint8_t const zeros[sizeof(header.m_Unused)];
    if (memcmp(header.m_Unused, zeros, sizeof(header.m_Unused)) != 0)
    {
        LOG(WARNING) << "The " << sizeof(header.m_Unused) << " last byte of the header should be 0";
    }
    
    // Compute required file size
    uint64_t reqSize = HEADER_SIZE + header.m_NumPRGBanks * Common::PRG_BANK_SIZE
                                   + header.m_NumCHRBanks * Common::CHR_BANK_SIZE;
    bool hasTrainer = header.m_CtrlByte1.m_HasTrainer;
    if (hasTrainer)
    {
        reqSize += Common::TRAINER_SIZE;
    }

    // Check if file is of correct size
    if (reqSize != size)
    {
        LOG(FATAL) << "Header implies file size of " << reqSize << " bytes but actually is "<< size;
    }

    // Read data banks
    if (hasTrainer)
    {
        m_Trainer = std::make_unique<Common::Trainer>();
        file.read(m_Trainer->data(), Common::TRAINER_SIZE);
    }
    m_PRGBanks.resize(header.m_NumPRGBanks);
    for (Common::PRGBank & bank : m_PRGBanks)
    {
        file.read(bank.data(), Common::PRG_BANK_SIZE);
    }
    m_CHRBanks.resize(header.m_NumCHRBanks);
    for (Common::CHRBank & bank : m_CHRBanks)
    {
        file.read(bank.data(), Common::CHR_BANK_SIZE);
    }

    // Save other info
    m_NumPGRRAMBanks          = max(header.m_NumRAMBanks, 1);
    m_HasBatteryBackedRAMBank = header.m_CtrlByte1.m_HasBatteryBackedRam;
    m_MapperNumber            = buildByteFromMSBandLSB(header.m_CtrlByte2.m_MapperNumberMSB, 
                                                       header.m_CtrlByte1.m_MapperNumberLSB);
    m_Mirroring               = header.m_CtrlByte1.m_UsesFourScreenMirroring 
                              ? Common::Mirroring::FOUR_SCREEN 
                              : header.m_CtrlByte1.m_IsHorizMirroring 
                              ? Common::Mirroring::VERTICAL 
                              : Common::Mirroring::HORIZONTAL;
}

}}
