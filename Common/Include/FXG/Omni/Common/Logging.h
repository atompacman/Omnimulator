#ifndef FXGUILD_OMNIMULATOR_COMMON_LOGGING
#define FXGUILD_OMNIMULATOR_COMMON_LOGGING

// ReSharper disable once CppUnusedIncludeDirective
#include <easylogging++.h>

namespace FXG { namespace Omni { namespace Common {

void initLogging(std::string const & i_LoggerConfigFilePath);

}}}

#endif // FXGUILD_OMNIMULATOR_COMMON_LOGGING