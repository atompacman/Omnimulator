#ifndef OMNIMULATOR_COMMON_LOGGING
#define OMNIMULATOR_COMMON_LOGGING

// ReSharper disable once CppUnusedIncludeDirective
#include <easylogging++.h>

namespace Omni { namespace Common {

void initLogging(std::string const & i_LoggerConfigFilePath);

}}

#endif // OMNIMULATOR_COMMON_LOGGING