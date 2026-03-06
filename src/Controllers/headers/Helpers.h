#ifndef HELPERS_H
#define HELPERS_H
#include "crow/http_request.h"
#include <string>
namespace Helpers
{
bool getDatabaseFromRequest(crow::request req);

// This needs to be changed and refactored but I'm too lazy because we meet all
// requirements
bool mergeDatabases(const std::string &mainDBName, const std::string &otherFile,
                    const std::string &mergeIntoTable,
                    const std::string &mergeFromTable,
                    const bool isUnique                              = false,
                    const std::vector<std::string> &mergeIntoColumns = {},
                    const std::vector<std::string> &mergeFromColumns = {},
                    const std::string &uniqueIntoColumn              = "",
                    const std::string &uniqueFromColumn              = "");

} // namespace Helpers
#endif
