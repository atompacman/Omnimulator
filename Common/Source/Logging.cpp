#include <FXG/Omni/Common/Logging.h>

// Macro to init easylogging++ library
INITIALIZE_EASYLOGGINGPP

namespace FXG { namespace Omni { namespace Common {

void initLogging(std::string const & i_LoggerConfigFilePath)
{
    el::Configurations conf;
    if (!conf.parseFromFile(i_LoggerConfigFilePath))
    {
        LOG(WARNING) << "Easylogging++ configuration file " << i_LoggerConfigFilePath
                     << " was not found. Using default parameters";
    }
    el::Loggers::reconfigureAllLoggers(conf);
}

}}}