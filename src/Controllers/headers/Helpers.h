#ifndef HELPERS_H
#define HELPERS_H
#include "crow/http_request.h"
#include <string>
namespace Helpers
{
bool getDatabaseFromRequest(crow::request req);
bool mergeDatabases(const std::string &dbName, const std::string &otherFile,
                    const std::string &table, const std::string &uniqueColumn);
} // namespace Helpers
#endif
