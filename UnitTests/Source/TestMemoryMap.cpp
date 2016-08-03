#include <gtest/gtest.h>

#include <FXG/Omni/CPU/MemoryMap.h>

TEST(MEMORY_MAP, LOL)
{
    FXG::Omni::CPU::MemoryMap mm { "..\\Config\\NES_CPU_MemoryMap.json" };
}