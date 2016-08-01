#ifndef FXGUILD_OMNIMULATOR_CPU_MEMORY
#define FXGUILD_OMNIMULATOR_CPU_MEMORY

#include <FXG/Omni/CPU/MemoryMap.h>
#include <FXG/Omni/CPU/Registers.h>

namespace FXG { namespace Omni { namespace CPU {

class Memory
{
public:

    explicit Memory(std::string const & i_MemoryMapJSONFile) :
        m_Registers {}
      , m_MemoryMap { i_MemoryMapJSONFile }
    {}

    Registers       & getRegisters()       { return m_Registers; }
    Registers const & getRegisters() const { return m_Registers; }
    MemoryMap       & getMemoryMap()       { return m_MemoryMap; }
    MemoryMap const & getMemoryMap() const { return m_MemoryMap; }

private:

    Registers m_Registers;
    MemoryMap m_MemoryMap;
};

}}}

#endif // FXGUILD_OMNIMULATOR_CPU_MEMORY