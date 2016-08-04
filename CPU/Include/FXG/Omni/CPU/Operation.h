#ifndef FXGUILD_OMNIMULATOR_CPU_OPERATION
#define FXGUILD_OMNIMULATOR_CPU_OPERATION

#include <vector>

#include <FXG/Omni/CPU/Memory.h>

namespace FXG { namespace Omni { namespace CPU {

class Operation
{
public:

    explicit Operation(std::string i_Abbreviation, 
                       std::string i_FullName, 
                       uint8_t     i_Size, 
                       uint16_t    i_NumCycles) :

        m_Abbreviation { i_Abbreviation }
      , m_FullName     { i_FullName     }
      , m_Size         { i_Size         }
      , m_NumCycles    { i_NumCycles    }
    {}

    virtual ~Operation() {};

    virtual void execute(Memory & io_Memory, std::vector<uint8_t> const i_Args) const = 0;

private:

    std::string m_Abbreviation;
    std::string m_FullName;
    uint16_t    m_Size;
    uint16_t    m_NumCycles;
};

class StatusFlagOperation : public Operation
{
public:

    enum class Action
    {
        SET,
        CLEAR
    };

    explicit StatusFlagOperation(std::string i_OpCodeName, 
        Action      i_Action,
        StatusFlag  i_StatusFlag) :

        // TODO
        Operation    (i_OpCodeName, "TODO WHEN PARSABLE ENUM ARE THERE", 1, 2)
        , m_Action     (i_Action)
        , m_StatusFlag (i_StatusFlag)
    {}

    virtual ~StatusFlagOperation() {};
    /*
    virtual void execute(Memory & io_Memory) const override
    {
    Registers & registers = io_Memory.getRegisters();
    if (m_Action == Action::SET)
    {
    registers.setStatusFlag(m_StatusFlag);
    }
    else
    {
    registers.clearStatusFlag(m_StatusFlag);
    }
    }
    */
protected:

    Action     const m_Action;
    StatusFlag const m_StatusFlag;
};

class LoadOperation : public Operation
{
public:

    explicit LoadOperation(std::string            i_OpCodeName, 
        GeneralPurposeRegister i_RegisterToLoad) :

        Operation        (i_OpCodeName, "TODO WHEN PARSABLE ENUM ARE THERE", 1, 2)
        , m_RegisterToLoad (i_RegisterToLoad)
    {}

    virtual ~LoadOperation() {};

    /*
    virtual void execute(Memory & io_Memory, std::vector<uint8_t> const i_Args) const override
    {
    execute(io_Memory.getRegisters());
    }

    virtual void execute(Registers & io_Registers) const = 0;
    */

protected:

    GeneralPurposeRegister const m_RegisterToLoad;
};

}}}

#endif // FXGUILD_OMNIMULATOR_CPU_OPERATION