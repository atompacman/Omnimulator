#include <fstream>

#include <Omni/Common/Common.h>
#include <Omni/Loader/NESROMFile.h>

namespace Omni { namespace Loader {

struct HeaderContent
{
    uint8_t m_NumPRGBanks;
    uint8_t m_NumCHRBanks;
    uint8_t m_NumRAMBanks;
    uint8_t m_CtrlByte1;
    uint8_t m_CtrlByte2;
};

HeaderContent readHeaderContent(std::basic_ifstream<uint8_t> & i_File);

NESROMFile::NESROMFile(std::string const & i_FilePath) :
    // Data banks
    m_PRGBanks ()
  , m_CHRBanks ()
  , m_Trainer  (nullptr)

    // Other info
  , m_NumPGRRAMBanks          (0)
  , m_HasBatteryBackedRAMBank (false)
  , m_MapperNumber            (0)
  , m_Mirroring               (Mirroring::HORIZONTAL)
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
    HeaderContent header = readHeaderContent(file);

    // Compute required file size
    uint64_t reqSize = HEADER_SIZE + header.m_NumPRGBanks * PRG_BANK_SIZE
                                   + header.m_NumCHRBanks * CHR_BANK_SIZE;
    bool hasTrainer = static_cast<bool>(header.m_CtrlByte1 & (1 << 2));
    if (hasTrainer)
    {
        reqSize += TRAINER_SIZE;
    }

    // Check if file is of correct size
    if (reqSize != size)
    {
        LOG(FATAL) << "Header implies there is " << reqSize 
                   << " bytes of data in the file but there is " << size;
    }

    // Read data banks
    if (hasTrainer)
    {
        m_Trainer = std::make_unique<Trainer>();
        file.read(m_Trainer->data(), TRAINER_SIZE);
    }
    m_PRGBanks.resize(header.m_NumPRGBanks);
    for (PRGBank & bank : m_PRGBanks)
    {
        file.read(bank.data(), PRG_BANK_SIZE);
    }
    m_CHRBanks.resize(header.m_NumCHRBanks);
    for (CHRBank & bank : m_CHRBanks)
    {
        file.read(bank.data(), CHR_BANK_SIZE);
    }

    // Save other info
    m_NumPGRRAMBanks          = max(header.m_NumRAMBanks, 1);
    m_HasBatteryBackedRAMBank = static_cast<bool>(header.m_CtrlByte1 & (1 << 1));
    m_MapperNumber            = (header.m_CtrlByte1 >> 4) + header.m_CtrlByte2;
    m_Mirroring               = header.m_CtrlByte1 & (1 << 3) 
                              ? Mirroring::FOUR_SCREEN 
                              : header.m_CtrlByte1 & (1 << 0) 
                              ? Mirroring::VERTICAL 
                              : Mirroring::HORIZONTAL;
}

HeaderContent readHeaderContent(std::basic_ifstream<uint8_t> & i_File)
{
    HeaderContent content;

    // Read file tag
    uint8_t tag[4];
    i_File.read(tag, 4);
    if (memcmp(tag, OFFICIAL_FILE_TAG, 4) != 0)
    {
        LOG(WARNING) << "File should start with the " << OFFICIAL_FILE_TAG << " characters";
    }

    // Read important content
    i_File >> content.m_NumPRGBanks 
           >> content.m_NumCHRBanks 
           >> content.m_CtrlByte1 
           >> content.m_CtrlByte2 
           >> content.m_NumRAMBanks;

    // Check that the first half of the control byte 2 are zeros
    if (content.m_CtrlByte2 & 0b00001111)
    {
        LOG(WARNING) << "First half of control byte 2 should be zeros";
        content.m_CtrlByte2 &= 0b11110000;
    }

    // Check that the rest of the header is zero
    uint8_t byte;
    for (uint8_t i = 0; i < 7; ++i)
    {
        i_File >> byte;
        if (byte != 0)
        {
            LOG(WARNING) << "The 7 last byte of the header should be zeros";
        }
    }

    return content;
}

}}
