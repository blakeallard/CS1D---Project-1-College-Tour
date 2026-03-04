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

// For the following functions any of these types can be used in a table
// Meaning you can put a {string, int, or double} in value vectors
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
// TO USE THE MATH INSTEAD OF REGULAR UPDATE:
// Only 1 column can be updated with no update values, it will simply adjust the
// value already set in the db
// QueryData::updateRows("Test.db", "testTable",
//                      {"For these columns"}, {"that have these values"},
//                      {"update only 1 column}, {}, "+ 5.3");
bool updateRows(const std::string &dbName, const std::string &table,
                const std::vector<std::string> &whereColumns,
                const std::vector<SQLiteValue> &whereValues,
                const std::vector<std::string> &updateColumns,
                const std::vector<SQLiteValue> &updateValues,
                const std::string &mathValue = "");

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
 *  we get a row in the result vector, we get a value from a column of that row
 *  with row.at("column"), we then convert it to the value we need using get
    for (const auto &row : results)
    {
        // Get a string from a row at the column name
        std::string name = std::get<std::string>(row.at("name"));
    }
*/

// Simply a custom select query, gets output the same way
QueryResult selectRowsWithQuery(const std::string &dbName,
                                const std::string &query);

// A custom query for things like insert, update and delete
bool customExecuteQuery(const std::string &dbName, const std::string &query);

}; // namespace QueryData

#endif
