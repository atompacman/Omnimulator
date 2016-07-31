#ifndef FXGUILD_OMNIMULATOR_COMMON_JSONUTILS
#define FXGUILD_OMNIMULATOR_COMMON_JSONUTILS

/*================================================================================================\\
| Utility functions for JSON file parsing
|--------------------------------------------------------------------------------------------------|
| assertValueIsA##type     : check if a value is of a certain type
| extract##type            : extract field from a JSON value (triggers an error when non-existing)
| extract##type##IfPresent : extract field from a JSON value if it exists, else use default value
\=================================================================================================*/

#include <algorithm>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>
#include <rapidjson/filereadstream.h>

#include <easylogging++.h>

#undef GetObject

namespace FXG { namespace Omni { namespace Common {

#define DECLARE_JSON_HELPER_FUNC_ASSERT_JSON_VALUE_IS_A(type, ...)                                 \
    inline void assertValueIsA##type(rapidjson::Value const & i_Value,                             \
                                     char const *             i_FieldName = nullptr)               \
    {                                                                                              \
        if (i_Value.Is##type())                                                                    \
        {                                                                                          \
            return;                                                                                \
        }                                                                                          \
        std::stringstream ss;                                                                      \
        char const * typeStr = #type;                                                              \
        ss << "Expected a " << static_cast<char>(tolower(typeStr[0])) << (typeStr + 1);            \
        if (i_FieldName)                                                                           \
        {                                                                                          \
            ss << " at field \"" << i_FieldName << "\"";                                           \
        }                                                                                          \
        LOG(FATAL) << ss.str();                                                                    \
    }

#define DECLARE_JSON_HELPER_FUNC_EXTRACT(type, ...)                                                \
    inline __VA_ARGS__ extract##type(rapidjson::Value const & i_Parent, char const * i_FieldName)  \
    {                                                                                              \
        LOG_IF(!i_Parent.HasMember(i_FieldName), FATAL) << "Missing field \""<< i_FieldName <<"\"";\
        auto & val = i_Parent[i_FieldName];                                                        \
        assertValueIsA##type(val, i_FieldName);                                                    \
        return val.Get##type();                                                                    \
    }

#define DECLARE_JSON_HELPER_FUNC_EXTRACT_IF_PRESENT(type, ...)                                     \
    inline __VA_ARGS__ extract##type##IfPresent(rapidjson::Value const & i_Parent,                 \
                                                char const *             i_FieldName,              \
                                                __VA_ARGS__ const &      i_DefaultValue)           \
    {                                                                                              \
        return i_Parent.HasMember(i_FieldName) ? extract##type(i_Parent, i_FieldName)              \
                                               : i_DefaultValue;                                   \
    }

#define DECLARE_JSON_HELPER_FUNC(FUNC_MACRO)                                                       \
    FUNC_MACRO(Array,  rapidjson::GenericArray<true, rapidjson::Value>)                            \
    FUNC_MACRO(Bool,   bool)                                                                       \
    FUNC_MACRO(Float,  float)                                                                      \
    FUNC_MACRO(Double, double)                                                                     \
    FUNC_MACRO(Int,    int)                                                                        \
    FUNC_MACRO(Int64,  uint64_t)                                                                   \
    FUNC_MACRO(Object, rapidjson::GenericObject<true, rapidjson::Value>)                           \
    FUNC_MACRO(String, std::string)                                                                \
    FUNC_MACRO(Uint,   unsigned int)                                                               \
    FUNC_MACRO(Uint64, uint64_t)

DECLARE_JSON_HELPER_FUNC(DECLARE_JSON_HELPER_FUNC_ASSERT_JSON_VALUE_IS_A)
DECLARE_JSON_HELPER_FUNC(DECLARE_JSON_HELPER_FUNC_EXTRACT)
DECLARE_JSON_HELPER_FUNC(DECLARE_JSON_HELPER_FUNC_EXTRACT_IF_PRESENT)

inline void parseJSONFile(rapidjson::Document & o_Doc, 
                          std::string const &   i_JSONFile, 
                          uint64_t              i_BufSize = 0x10000)
{
    // Open JSON file
    FILE * handle;
    fopen_s(&handle, i_JSONFile.c_str(), "r");
    LOG_IF(!handle, FATAL) << "File not found";

    // Create file stream reader
    char * buf = new char[i_BufSize];
    rapidjson::FileReadStream frs(handle, buf, sizeof buf);

    // Parse JSON document
    o_Doc.ParseStream(frs);
    LOG_IF(o_Doc.HasParseError(), FATAL) << GetParseError_En(o_Doc.GetParseError());
    
    // Close file
    delete [] buf;
    fclose(handle);
}

}}}

#endif // FXGUILD_OMNIMULATOR_COMMON_JSONUTILS