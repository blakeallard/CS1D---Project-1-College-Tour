#include "Helpers.h"
#include "AbsDatabase.h"
#include "crow/multipart.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

namespace Helpers
{
// If true then we uploaded successfully
bool getDatabaseFromRequest(crow::request req)
{
    crow::multipart::message msg(req);

    auto filePart = msg.get_part_by_name("file");

    if (req.body.empty())
    {
        throw std::runtime_error("Request body empty, probably empty file");
    }

    auto filename        = filePart.get_header_object("filename");
    std::string filedata = filePart.body;

    // Save file
    std::ofstream out("Databases/uploaded.db", std::ios::binary);
    out.write(filedata.c_str(), filedata.size());
    out.close();

    return true;
}

bool mergeDatabases(const std::string &mainDBName, const std::string &otherFile,
                    const std::string &mergeIntoTable,
                    const std::string &mergeFromTable, const bool isUnique,
                    const std::vector<std::string> &mergeIntoColumns,
                    const std::vector<std::string> &mergeFromColumns,
                    const std::string &uniqueIntoColumn,
                    const std::string &uniqueFromColumn)
{
    if (mergeFromColumns.size() != mergeIntoColumns.size())
        throw std::runtime_error("Code column sizes do not match");

    ABSDatabase mainDB(mainDBName);
    sqlite3 *db = mainDB.get();

    try
    {
        // Begin transaction
        {
            Statement begin(db, "BEGIN TRANSACTION;");
            begin.execute();
        }

        // Attach other database
        {
            Statement attach(db, "ATTACH DATABASE ? AS other;");
            attach.bind(1, otherFile);
            attach.execute();
        }

        {
            std::stringstream ss;

            if (isUnique)
                ss << "INSERT OR IGNORE INTO main." << mergeIntoTable << " ("
                   << uniqueIntoColumn << ")" << " SELECT " << uniqueFromColumn
                   << " FROM other." << mergeFromTable << ";";
            else
            {

                ss << "INSERT INTO main." << mergeIntoTable;

                if (!mergeIntoColumns.empty())
                {
                    ss << " (";
                    for (size_t i = 0; i < mergeIntoColumns.size(); ++i)
                    {
                        ss << mergeIntoColumns[i];

                        if (i < mergeIntoColumns.size() - 1)
                            ss << ", ";
                    }
                    ss << ")";
                }

                ss << " SELECT ";

                if (!mergeFromColumns.empty())
                {
                    for (size_t i = 0; i < mergeFromColumns.size(); ++i)
                    {
                        ss << mergeFromColumns[i];

                        if (i < mergeFromColumns.size() - 1)
                            ss << ", ";
                    }
                }
                else
                    ss << "*";

                ss << " FROM other." << mergeFromTable;

                if (!uniqueIntoColumn.empty())
                    ss << " WHERE NOT EXISTS ("
                          " SELECT 1 FROM main."
                       << mergeIntoTable << " WHERE main." << mergeIntoTable
                       << "." << uniqueIntoColumn << " = other."
                       << mergeFromTable << "." << uniqueFromColumn << ")";

                ss << ";";
            }

            std::string sql = ss.str();
            cout << sql << endl;

            Statement merge(db, sql);
            merge.execute();
        }

        // Commit
        {
            Statement commit(db, "COMMIT;");
            commit.execute();
        }

        // Detach
        {
            Statement detach(db, "DETACH DATABASE other;");
            detach.execute();
        }

        std::filesystem::path file = "Databases/uploaded.db";

        if (std::filesystem::remove(file))
        {
            std::cout << "Temp File deleted\n";
        }
        else
        {
            std::cout << "File not found or couldn't delete\n";
        }
    }
    catch (std::runtime_error e)
    {
        Statement rollback(db, "ROLLBACK;");
        rollback.execute();
        std::cerr << e.what() << endl;
        throw std::runtime_error("Could not merge databases");
    }
    return true;
}
} // namespace Helpers
