#ifndef FXGUILD_OMNIMULATOR_CPU_REGISTERS
#define FXGUILD_OMNIMULATOR_CPU_REGISTERS

#include <array>

namespace FXG { namespace Omni { namespace CPU {

enum class GeneralPurposeRegister
{
    ACCUMULATOR = 0,
    INDEX_X     = 1,
    INDEX_Y     = 2,
    MAX         = 3
};

enum class StatusFlag
{
    CARRY        = 1 << 0,
    ZERO         = 1 << 1,
    INTERRUPT    = 1 << 2,
    DECIMAL_MODE = 1 << 3,
    BREAK_CMD    = 1 << 4,
    UNUSED       = 1 << 5,
    OVERFLOW_    = 1 << 6, // OVERFLOW is already defined in math.h
    NEGATIVE     = 1 << 7
};

class Registers
{
public:

    explicit Registers() :
        m_ProgramCounter { 0x0000 }
      , m_StackPointer   { 0xFF   }
      , m_Status         { 0x00   }

      , m_GeneralPurposeRegisters { }
    {}

    // Getters
    uint16_t getProgramCounterValue () const { return m_ProgramCounter; }
    uint8_t  getStackPointerValue   () const { return m_StackPointer;   }
    bool     getStatusFlag                  (StatusFlag i_Flag)            const;
    uint8_t  getGeneralPurposeRegisterValue (GeneralPurposeRegister i_GPR) const;

    // Setters
    void setProgramCounterValue         (uint16_t i_Value) { m_ProgramCounter = i_Value; }
    void setStackPointerValue           (uint8_t  i_Value) { m_StackPointer   = i_Value; }
    void setStatusFlag                  (StatusFlag i_Flag);
    void clearStatusFlag                (StatusFlag i_Flag);
    void setGeneralPurposeRegisterValue (GeneralPurposeRegister i_GPR, uint8_t i_Value);

private:

    uint16_t m_ProgramCounter;
    uint8_t  m_StackPointer;
    uint8_t  m_Status;

    std::array<uint8_t,static_cast<uint8_t>(GeneralPurposeRegister::MAX)> m_GeneralPurposeRegisters;

};

inline bool Registers::getStatusFlag(StatusFlag i_Flag) const
{
    return (m_Status & static_cast<uint8_t>(i_Flag)) != 0;
}

inline uint8_t Registers::getGeneralPurposeRegisterValue(GeneralPurposeRegister i_GPR) const
{
    return m_GeneralPurposeRegisters[static_cast<uint8_t>(i_GPR)];
}

inline void Registers::setStatusFlag(StatusFlag i_Flag)
{
    m_Status |= static_cast<uint8_t>(i_Flag);
}

inline void Registers::clearStatusFlag(StatusFlag i_Flag)
{
    m_Status &= ~static_cast<uint8_t>(i_Flag);
}

inline void Registers::setGeneralPurposeRegisterValue(GeneralPurposeRegister i_GPR, uint8_t i_Value)
{
    m_GeneralPurposeRegisters[static_cast<uint8_t>(i_GPR)] = i_Value;
}

}}}

#endif // FXGUILD_OMNIMULATOR_CPU_REGISTERS