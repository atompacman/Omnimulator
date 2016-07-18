#ifndef OMNIMULATOR_COMMON_COMMON
#define OMNIMULATOR_COMMON_COMMON

#include <memory>

// ReSharper disable once CppUnusedIncludeDirective
#include <Omni/Common/Logging.h>

template <typename T>
using sptr = std::shared_ptr<T>;

template <typename T>
using sptrc = std::shared_ptr<T const>;

template <typename T>
using uptr = std::unique_ptr<T>;

template <typename T>
using uptrc = std::unique_ptr<T const>;

#endif // OMNIMULATOR_COMMON_COMMON