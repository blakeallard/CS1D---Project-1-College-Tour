/**
 * @file Helpers.h
 * @brief Utility functions for data import and database operations
 * 
 * This file provides helper functions for administrative operations
 * including file uploads and database merging for campus/souvenir imports.
 * 
 * @author CS1D Group Project
 * @date March 2026
 */

#ifndef HELPERS_H
#define HELPERS_H
#include "crow/http_request.h"
#include <string>

/**
 * @namespace Helpers
 * @brief Utility functions for administrative operations
 * 
 * Contains helper functions for processing file uploads and
 * merging database content during administrative imports.
 */
namespace Helpers
{

/**
 * @brief Extracts and saves database file from HTTP request
 * 
 * Parses multipart form data from the request to extract an
 * uploaded database file. Saves the file to Databases/uploaded.db
 * for subsequent merging operations.
 * 
 * @param req HTTP request containing multipart form data
 * @return true if extraction successful, false otherwise
 * 
 * @note Expected form field name varies based on implementation
 * @see mergeDatabases
 */
bool getDatabaseFromRequest(crow::request req);

/**
 * @brief Merges data from one database into another
 * 
 * Copies rows from the source database/table into the destination
 * database/table. Supports unique constraint checking to prevent
 * duplicate entries.
 * 
 * @param mainDBName Path to destination database
 * @param otherFile Path to source database (typically uploaded.db)
 * @param mergeIntoTable Table name in destination database
 * @param mergeFromTable Table name in source database
 * @param isUnique If true, only merge rows with unique key values
 * @param mergeIntoColumns Columns to insert into (empty for all)
 * @param mergeFromColumns Columns to read from (empty for all)
 * @param uniqueIntoColumn Column in destination for uniqueness check
 * @param uniqueFromColumn Column in source for uniqueness check
 * @return true if merge successful, false otherwise
 * 
 * @note Used for importing new campuses and souvenirs via admin interface
 * 
 * @code
 * // Merge distances from uploaded file
 * Helpers::mergeDatabases("Databases/distances.db",
 *                         "Databases/uploaded.db",
 *                         "distances", "distances");
 * 
 * // Merge unique colleges
 * Helpers::mergeDatabases("Databases/distances.db",
 *                         "Databases/uploaded.db",
 *                         "colleges", "distances",
 *                         true, {}, {},
 *                         "college", "starting_college");
 * @endcode
 */
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
