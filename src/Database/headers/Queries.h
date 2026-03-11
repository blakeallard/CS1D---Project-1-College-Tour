/**
 * @file Queries.h
 * @brief Database query utilities for SQLite operations
 * 
 * This file provides a namespace of functions for interacting with
 * SQLite databases used by the College Tour application. Supports
 * CRUD operations with type-safe parameter binding.
 * 
 * @author CS1D Group Project
 * @date March 2026
 */

#ifndef QUERIES_H
#define QUERIES_H
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

/** @brief Path to distances database */
static const std::string DISTANCES     = "Databases/distances.db";
/** @brief Path to souvenirs database */
static const std::string SOUVENIRS     = "Databases/souvenirs.db";
/** @brief Path to users database */
static const std::string USERS         = "Databases/users.db";
/** @brief Path to new campuses database (for admin imports) */
static const std::string NEW_DISTANCES = "Databases/new_campuses.db";

/**
 * @namespace QueryData
 * @brief Database query functions and type definitions
 * 
 * Provides type-safe database operations using std::variant for
 * flexible value types. All functions handle SQLite database
 * connections internally.
 */
namespace QueryData
{

/**
 * @brief Variant type for SQLite-compatible values
 * 
 * Supports int, double, string, and nullptr (NULL) values.
 * Used for parameter binding in queries.
 */
using SQLiteValue = std::variant<int, double, std::string, std::nullptr_t>;

/**
 * @brief Represents a single database row as column-value pairs
 */
using Row = std::unordered_map<std::string, SQLiteValue>;

/**
 * @brief Result set from a SELECT query
 */
using QueryResult = std::vector<Row>;

/**
 * @brief Inserts a new row into a database table
 * 
 * @param dbName Name/path of the database file
 * @param table Name of the table to insert into
 * @param columns Vector of column names to populate
 * @param values Vector of values corresponding to columns
 * @return true if insertion successful, false otherwise
 * 
 * @pre columns.size() == values.size()
 * 
 * @code
 * QueryData::insertRow("souvenirs.db", "souvenirs",
 *                      {"college", "item", "price"},
 *                      {"UCLA", "T-Shirt", 25.99});
 * @endcode
 * 
 * Time Complexity: O(1) for single row insertion
 */
bool insertRow(const std::string &dbName, const std::string &table,
               const std::vector<std::string> &columns,
               const std::vector<SQLiteValue> &values);

/**
 * @brief Deletes rows from a database table matching criteria
 * 
 * @param dbName Name/path of the database file
 * @param table Name of the table to delete from
 * @param whereColumns Columns to match for deletion
 * @param whereValues Values to match in those columns
 * @return true if deletion successful, false otherwise
 * 
 * @pre whereColumns.size() == whereValues.size()
 * 
 * @code
 * QueryData::deleteRow("souvenirs.db", "souvenirs",
 *                      {"college", "item"},
 *                      {"UCLA", "T-Shirt"});
 * @endcode
 * 
 * Time Complexity: O(n) where n is number of rows scanned
 */
bool deleteRow(const std::string &dbName, const std::string &table,
               const std::vector<std::string> &whereColumns,
               const std::vector<SQLiteValue> &whereValues);

/**
 * @brief Updates rows in a database table
 * 
 * Supports two modes:
 * 1. Direct value update: Set columns to new values
 * 2. Math operation: Apply arithmetic to existing values
 * 
 * @param dbName Name/path of the database file
 * @param table Name of the table to update
 * @param whereColumns Columns to match for selection
 * @param whereValues Values to match in those columns
 * @param updateColumns Columns to update
 * @param updateValues New values for those columns
 * @param mathValue Optional arithmetic operation (e.g., "+ 5.0")
 * @return true if update successful, false otherwise
 * 
 * @code
 * // Direct update
 * QueryData::updateRows("souvenirs.db", "souvenirs",
 *                       {"college", "item"}, {"UCLA", "T-Shirt"},
 *                       {"price"}, {29.99});
 * 
 * // Math operation (increase price by 5)
 * QueryData::updateRows("souvenirs.db", "souvenirs",
 *                       {"college"}, {"UCLA"},
 *                       {"price"}, {}, "+ 5.0");
 * @endcode
 * 
 * Time Complexity: O(n) where n is number of rows scanned
 */
bool updateRows(const std::string &dbName, const std::string &table,
                const std::vector<std::string> &whereColumns,
                const std::vector<SQLiteValue> &whereValues,
                const std::vector<std::string> &updateColumns,
                const std::vector<SQLiteValue> &updateValues,
                const std::string &mathValue = "");

/**
 * @brief Selects rows from a database table
 * 
 * Retrieves data from specified columns, optionally filtered by
 * WHERE conditions. Returns results as vector of row maps.
 * 
 * @param dbName Name/path of the database file
 * @param table Name of the table to query
 * @param columns Columns to retrieve (empty for all)
 * @param whereColumns Optional columns for WHERE clause
 * @param whereValues Optional values for WHERE clause
 * @return QueryResult (vector of Row maps)
 * 
 * @code
 * // Get all souvenirs for a campus
 * QueryResult results = QueryData::selectRows(
 *     "souvenirs.db", "souvenirs",
 *     {"item", "price"},
 *     {"college"},
 *     {"UCLA"});
 * 
 * // Extract values from results
 * for (const auto &row : results) {
 *     std::string item = std::get<std::string>(row.at("item"));
 *     double price = std::get<double>(row.at("price"));
 * }
 * @endcode
 * 
 * Time Complexity: O(n) where n is number of rows scanned
 */
QueryResult selectRows(const std::string &dbName, const std::string &table,
                       const std::vector<std::string> &columns,
                       const std::vector<std::string> &whereColumns = {},
                       const std::vector<SQLiteValue> &whereValues  = {});

/**
 * @brief Executes a custom SELECT query
 * 
 * For complex queries not supported by selectRows().
 * Use with caution - no parameter binding (SQL injection risk).
 * 
 * @param dbName Name/path of the database file
 * @param query Complete SQL SELECT query string
 * @return QueryResult (vector of Row maps)
 * 
 * @code
 * QueryResult results = QueryData::selectRowsWithQuery(
 *     "distances.db",
 *     "SELECT DISTINCT starting_college FROM distances ORDER BY starting_college");
 * @endcode
 * 
 * @warning Query string is executed directly - sanitize inputs
 */
QueryResult selectRowsWithQuery(const std::string &dbName,
                                const std::string &query);

/**
 * @brief Executes a custom non-SELECT query
 * 
 * For INSERT, UPDATE, DELETE, or DDL statements not supported
 * by the other functions. Use with caution.
 * 
 * @param dbName Name/path of the database file
 * @param query Complete SQL query string
 * @return true if execution successful, false otherwise
 * 
 * @warning Query string is executed directly - sanitize inputs
 */
bool customExecuteQuery(const std::string &dbName, const std::string &query);

}; // namespace QueryData

#endif
