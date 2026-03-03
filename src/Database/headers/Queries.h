#ifndef QUERIES_H
#define QUERIES_H
#include <DatabaseTypes.h>
#include <unordered_map>
#include <variant>
#include <vector>

static const std::string DISTANCES     = "Databases/distances.db";
static const std::string SOUVENIRS     = "Databases/souvenirs.db";
static const std::string USERS         = "Databases/users.db";
static const std::string NEW_DISTANCES = "Databases/new_campuses.db";

namespace QueryData
{
double distance(const std::string &campus1, const std::string &campus2);
void allDistances(const std::string &campus,
                  std::unordered_map<std::string, double> &distances);
void allCampuses(std::vector<std::string> &campuses);
void souvenirs(const std::string &campus, std::vector<SouvenirItem> &souvenirs);

using SQLiteValue = std::variant<int, double, std::string, std::nullptr_t>;
bool insertRow(const std::string &dbName, const std::string &table,
               const std::vector<std::string> &columns,
               const std::vector<SQLiteValue> &values);

}; // namespace QueryData

#endif
