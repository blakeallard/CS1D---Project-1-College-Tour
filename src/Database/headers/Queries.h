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

// For the following functions any of these types can be used in a table
using SQLiteValue = std::variant<int, double, std::string, std::nullptr_t>;
using Row         = std::unordered_map<std::string, SQLiteValue>;
using QueryResult = std::vector<Row>;

// Exapmle
// QueryData::insertRow("Test.db", "testTable",
//                      {"Columns needed to be added to", "int key"},
//                      {"values to be added to columns", 67});
bool insertRow(const std::string &dbName, const std::string &table,
               const std::vector<std::string> &columns,
               const std::vector<SQLiteValue> &values);

// Example QueryData::deleteRow("Test.db", "testTable",
//                      {"For these columns", "ex"},
//                      {"if they contain these values delete the row",4});
bool deleteRow(const std::string &dbName, const std::string &table,
               const std::vector<std::string> &whereColumns,
               const std::vector<SQLiteValue> &whereValues);

// Exapmle
// QueryData::updateRows("Test.db", "testTable",
//                      {"For these columns"}, {"that have these values"},
//                      {"update these columns", "id"}, {"to these values
//                      here",4});
bool updateRows(const std::string &dbName, const std::string &table,
                const std::vector<std::string> &whereColumns,
                const std::vector<SQLiteValue> &whereValues,
                const std::vector<std::string> &updateColumns,
                const std::vector<SQLiteValue> &updateValues);

// Example
// QueryData::selectRows
// selectRows("Test.db", "testTable",
//           {"Get data of these", "columns"},
//           {"Where we choose from", "These columns"},
//           {"That may have these specific value", 69});
QueryResult selectRows(const std::string &dbName, const std::string &table,
                       const std::vector<std::string> &columns,
                       const std::vector<std::string> &whereColumns = {},
                       const std::vector<SQLiteValue> &whereValues  = {});
// Exapmle for getting data out of the query result
/*
    for (const auto &row : results)
    {
        // Get the string variant from a row at the column name if we extracted
        // name
        std::string name = std::get<std::string>(row.at("name"));
    }
*/

}; // namespace QueryData

#endif
