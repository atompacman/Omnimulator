#ifndef FXGUILD_OMNIMULATOR_CPU_ADDRESSER
#define FXGUILD_OMNIMULATOR_CPU_ADDRESSER

#include <map>

#include <FXG/Common/ParsableEnum.h>
#include <FXG/Omni/Common/Basic.h>
#include <FXG/Omni/CPU/Memory.h>

namespace FXG { namespace Omni { namespace CPU {

DECLARE_ENUM_WITH_STRING_CONVERSION_MAPS(AddressingMode, 

//  IMPLIED          // not in the list because it is operation-specific
    IMMEDIATE
//, ACCUMULATOR      // TODO or not TODO?

  , ZERO_PAGE
  , ZERO_PAGE_X
  , ZERO_PAGE_Y

  , ABSOLUTE_        // ABSOLUTE is #defined in wingdi.h
  , ABSOLUTE_X
  , ABSOLUTE_Y

//, INDIRECT         // specific to jump operation
  , INDEXED_INDIRECT
  , INDIRECT_INDEXED

//, RELATIVE         // specific to branch operation
)

class Addresser
{
public:
    
    DECLARE_ENUM_WITH_STRING_CONVERSION_MAPS(ArgSize, 

        ONE_BYTE, 
        TWO_BYTE
    )

    class Result
    {
    public:

        explicit Result(uint8_t i_Value, uint8_t i_NumCycles) :
            m_Value     { i_Value     }
          , m_NumCycles { i_NumCycles }
        {}

        uint8_t getValue    () const { return m_Value;     }
        uint8_t getNumCycles() const { return m_NumCycles; }

    private:

        uint8_t const m_Value;
        uint8_t const m_NumCycles;
    };

    explicit Addresser(ArgSize i_ArgSize) :
        m_ArgSize { i_ArgSize }
    {}

    virtual ~Addresser() {}

    // Address classes which m_ArgSize == 1 may only use the LSB and others must use both
    virtual Result address(Memory const & i_Memory, uint8_t i_LSB, uint8_t i_MSB) const = 0;

private:

    ArgSize  const m_ArgSize;
};

class ConstantNumCyclesAddresser : public Addresser
{
public:

    explicit ConstantNumCyclesAddresser(ArgSize i_ArgSize, uint16_t i_NumCycles) :
        Addresser           { i_ArgSize   }
      , m_CyclesNumModifier { i_NumCycles }
    {}

    virtual Result address(Memory const & i_Memory, uint8_t i_LSB, uint8_t i_MSB) const override
    {
        uint8_t value = addressImpl(i_Memory, i_LSB, i_MSB);
        return Result { value, m_CyclesNumModifier };
    }

    virtual uint8_t addressImpl(Memory const & i_Memory, uint8_t i_LSB, uint8_t i_MSB) const = 0;

private:

    uint16_t const m_CyclesNumModifier;
};

class ImmediateAddresser : public ConstantNumCyclesAddresser
{
public:

    explicit ImmediateAddresser() :
        ConstantNumCyclesAddresser { ArgSize::ONE_BYTE, 2 }
    {}
    
    virtual uint8_t addressImpl(Memory const & /* i_Memory */, 
                                uint8_t           i_LSB, 
                                uint8_t        /* i_MSB    */) const override 
    {
        return i_LSB;
    }
};

class ZeroPageAddresser : public ConstantNumCyclesAddresser
{
public:

    explicit ZeroPageAddresser() :
        ConstantNumCyclesAddresser { ArgSize::ONE_BYTE, 3 }
    {}

    virtual uint8_t addressImpl(Memory const & i_Memory, 
                                uint8_t        i_LSB, 
                                uint8_t     /* i_MSB */) const override
    {
        return i_Memory.getMemoryMap()[i_LSB];
    }
};

class IndexedZeroPageAddresser : public ConstantNumCyclesAddresser
{
public:

    explicit IndexedZeroPageAddresser(GeneralPurposeRegister i_GPR) :
        ConstantNumCyclesAddresser { ArgSize::ONE_BYTE, 4 }
      , m_GPR                      { i_GPR                }
    {}

    virtual uint8_t addressImpl(Memory const & i_Memory, 
                                uint8_t        i_LSB, 
                                uint8_t     /* i_MSB */) const override
    {
        uint8_t regVal = i_Memory.getRegisters().getGeneralPurposeRegisterValue(m_GPR);
        return i_Memory.getMemoryMap()[(i_LSB + regVal) & 0xFF];
    }

private:

    GeneralPurposeRegister const m_GPR;
};

class AbsoluteAddresser : public ConstantNumCyclesAddresser
{
public:

    explicit AbsoluteAddresser() :
        ConstantNumCyclesAddresser { ArgSize::TWO_BYTE, 4 }
    {}

    virtual uint8_t addressImpl(Memory const & i_Memory, 
                                uint8_t        i_LSB, 
                                uint8_t        i_MSB) const override
    {
        return i_Memory.getMemoryMap()[Common::buildLittleEndianAddress(i_LSB, i_MSB)];
    }
};

class IndexedAbsoluteAddresser : public Addresser
{
public:

    explicit IndexedAbsoluteAddresser(GeneralPurposeRegister i_GPR) :
        Addresser { ArgSize::TWO_BYTE }
      , m_GPR     { i_GPR             }
    {}

    virtual Result address(Memory const & i_Memory, uint8_t i_LSB, uint8_t i_MSB) const override
    {
        uint16_t firstArg     = Common::buildLittleEndianAddress(i_LSB, i_MSB);
        uint8_t  secondArg    = i_Memory.getRegisters().getGeneralPurposeRegisterValue(m_GPR);
        uint16_t finalAddress = firstArg + secondArg;
        uint8_t  numCycles    = 4;
        // TODO understand "one-cycle penalty if the base of the direct page doesn't lie on a 
        // 256-byte boundary"
        if ((firstArg >> 2) != (finalAddress >> 2))
        {
            ++numCycles;
        }
        return Result { i_Memory.getMemoryMap()[finalAddress], numCycles };
    }

private:

    GeneralPurposeRegister const m_GPR;
};

class IndexedIndirectAddresser : public ConstantNumCyclesAddresser
{
public:

    explicit IndexedIndirectAddresser() :
        ConstantNumCyclesAddresser { ArgSize::ONE_BYTE, 6 }
    {}

    virtual uint8_t addressImpl(Memory const & i_Memory, 
                                uint8_t        i_LSB, 
                                uint8_t      /* i_MSB */) const override
    {
        auto const & reg = i_Memory.getRegisters();
        auto const & mm  = i_Memory.getMemoryMap();
        uint8_t  xVal    = reg.getGeneralPurposeRegisterValue(GeneralPurposeRegister::INDEX_X);
        uint8_t  address = xVal + i_LSB;
        uint16_t target  = Common::buildLittleEndianAddress(mm[address], mm[address + 1]);
        return i_Memory.getMemoryMap()[target];
    }
};

class IndirectIndexedAddresser : public Addresser
{
public:

    explicit IndirectIndexedAddresser() :
        Addresser { ArgSize::TWO_BYTE }
    {}

    virtual Result address(Memory const & i_Memory, uint8_t i_LSB, uint8_t i_MSB) const override
    {
        uint8_t firstArg     = i_Memory.getMemoryMap()[i_LSB];
        uint8_t secondArg    = i_Memory.getRegisters()
                                   .getGeneralPurposeRegisterValue(GeneralPurposeRegister::INDEX_Y);
        uint8_t finalAddress = firstArg + secondArg;
        uint8_t numCycles    = 5;
        // TODO understand "one-cycle penalty if the base of the direct page doesn't lie on a 
        // 256-byte boundary"
        if ((firstArg >> 2) != (finalAddress >> 2))
        {
            ++numCycles;
        }
        return Result { i_Memory.getMemoryMap()[finalAddress], numCycles };
    }
};

std::map<AddressingMode, sptrc<Addresser>> const ADDRESSING_MODE_2_ADDRESSER = { 
    { AddressingMode::IMMEDIATE       , std::make_shared<ImmediateAddresser      >()                                }
  , { AddressingMode::ZERO_PAGE       , std::make_shared<ZeroPageAddresser       >()                                }
  , { AddressingMode::ZERO_PAGE_X     , std::make_shared<IndexedZeroPageAddresser>(GeneralPurposeRegister::INDEX_X) }
  , { AddressingMode::ZERO_PAGE_Y     , std::make_shared<IndexedZeroPageAddresser>(GeneralPurposeRegister::INDEX_Y) }
  , { AddressingMode::ABSOLUTE_       , std::make_shared<AbsoluteAddresser       >()                                }
  , { AddressingMode::ABSOLUTE_X      , std::make_shared<IndexedAbsoluteAddresser>(GeneralPurposeRegister::INDEX_X) }
  , { AddressingMode::ABSOLUTE_Y      , std::make_shared<IndexedAbsoluteAddresser>(GeneralPurposeRegister::INDEX_Y) }
  , { AddressingMode::INDEXED_INDIRECT, std::make_shared<IndexedIndirectAddresser>() }
  , { AddressingMode::INDIRECT_INDEXED, std::make_shared<IndirectIndexedAddresser>() }
};

}}}

#endif // FXGUILD_OMNIMULATOR_CPU_ADDRESSER
