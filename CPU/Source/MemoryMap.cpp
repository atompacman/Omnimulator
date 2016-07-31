#include <FXG/Omni/Common/JSONUtils.h>
#include <FXG/Omni/CPU/MemoryMap.h>

namespace FXG { namespace Omni { namespace CPU {

char const * const MEMORY_MAP_NAME_FIELD   = "Name";
char const * const REGIONS_FIELD           = "Regions";
char const * const REGION_NAME_FIELD       = "Name";
char const * const REGION_PHYS_SIZE_FIELD  = "PhysicalSize";
char const * const REGION_TOTAL_SIZE_FIELD = "TotalSize";

MemoryMap::MemoryMap(std::string const & i_JSONFile) :
    m_Name    {   }
  , m_Size    { 0 }
  , m_Regions {   }
{
    // Parse JSON file
    rapidjson::Document doc;
    Common::parseJSONFile(doc, i_JSONFile);
    Common::assertValueIsAObject(doc);

    // Extract memory map name
    m_Name = Common::extractString(doc, MEMORY_MAP_NAME_FIELD);

    // Extract regions
    auto regions = Common::extractArray(doc, REGIONS_FIELD);
    
    for (rapidjson::SizeType i = 0; i < regions.Size(); ++i)
    {
        auto & region = regions[i];

        // Extract name
        std::string regionName = Common::extractString(region, REGION_NAME_FIELD);

        // Extract physical size
        uint64_t physicalSize = Common::extractUint64(region, REGION_PHYS_SIZE_FIELD);

        // Extract total size if present
        uint64_t totalSize = Common::extractUint64IfPresent(
            region, REGION_TOTAL_SIZE_FIELD, physicalSize);

        // Create memory region
        m_Regions.emplace(regionName, m_Size, physicalSize, totalSize);

        // Increment region map size
        m_Size += totalSize;
    }
}

uint8_t & MemoryMap::operator [] (uint64_t i_Address)
{
    LOG_IF(i_Address >= getSize(), FATAL) << "Memory address is out of bounds";
    Region const & node = *m_Regions.lower_bound(Region { "", i_Address, 0, 0 });
    return node[i_Address];
}

}}}
