#include "AbsDatabase.h"
#include "crow/multipart.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
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
                    const std::string &table, const std::string &uniqueColumn)
{
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
            // Build SQL dynamically
            std::string sql =
                "INSERT INTO main." + table + " SELECT * FROM other." + table +
                " WHERE NOT EXISTS ("
                " SELECT 1 FROM main." +
                table + " WHERE main." + table + "." + uniqueColumn +
                " = other." + table + "." + uniqueColumn + ");";

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
