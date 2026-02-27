#include <AbsDatabase.h>
#include <stdexcept>

void ABSDatabase::ConnectToDB(const std::string &file)
{
    // Open generic database
    int result = sqlite3_open(file.c_str(), &generic_db);
    if (result != SQLITE_OK)
    {
        CloseDB();
        throw std::runtime_error(sqlite3_errmsg(generic_db));
    }
}

void ABSDatabase::CloseDB()
{
    if (generic_db != nullptr)
    {
        sqlite3_close(generic_db);
        generic_db = nullptr;
    }
}

sqlite3 *ABSDatabase::get() const
{
    return generic_db;
}
