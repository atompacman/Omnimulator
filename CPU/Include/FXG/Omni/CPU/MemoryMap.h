#ifndef FXGUILD_OMNIMULATOR_CPU_MEMORYMAP
#define FXGUILD_OMNIMULATOR_CPU_MEMORYMAP

#include <set>
#include <vector>

namespace FXG { namespace Omni { namespace CPU {

class MemoryMap
{
    class Region
    {
    public:

        explicit Region(std::string const & i_Name, 
                        uint64_t            i_StartingAddress, 
                        uint64_t            i_PhysicalDataSize, 
                        uint64_t            i_TotalSize) :

            m_Name            { i_Name             }
          , m_TotalSize       { i_TotalSize        }
          , m_StartingAddress { i_StartingAddress  }
          , m_PhysicalData    ( i_PhysicalDataSize )
        {}

        uint8_t & operator [] (uint64_t i_Address) const
        {
            return m_PhysicalData[(i_Address - m_StartingAddress) % m_PhysicalData.size()];
        }

        bool operator < (Region const & i_Other) const
        {
            return m_StartingAddress < i_Other.m_StartingAddress;
        }

        std::string const & getName()             const { return m_Name;                }
        uint64_t            getPhysicalDataSize() const { return m_PhysicalData.size(); }
        uint64_t            getTotalSize()        const { return m_TotalSize;           }

    private:

        std::string const m_Name;
        uint64_t    const m_TotalSize;
        uint64_t    const m_StartingAddress;

        std::vector<uint8_t> mutable m_PhysicalData;
    };

public:

    explicit MemoryMap(std::string const & i_JSONFile);

    uint8_t   operator [] (uint64_t i_Address) const { return this->operator[](i_Address); }
    uint8_t & operator [] (uint64_t i_Address);

    std::string getName() const { return m_Name; }

    uint64_t    getSize() const { return m_Size; }

private:

    std::string      m_Name;
    uint64_t         m_Size;
    std::set<Region> m_Regions;
};

}}}

#endif // FXGUILD_OMNIMULATOR_CPU_MEMORYMAP