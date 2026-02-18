#include "database.h"
#include <iostream>
#include <sqlite3.h>

bool Database::ConnectToDB(const std::string& distancesFile, const std::string& souvenirsFile)
{
    // Open "distances.db" Database
    int result = sqlite3_open(distancesFile.c_str(), &distances_db);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to connect to 'distances.db' database" << std::endl;
        sqlite3_close(distances_db);
        return false;
    }

    // Open "sourvenirs.db" Database 
    result = sqlite3_open(souvenirsFile.c_str(), &souvenirs_db);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to connect to 'souvenirs.db' database" << std::endl;
        sqlite3_close(souvenirs_db);
        sqlite3_close(distances_db);
        return false;
    }

    std::cout << "'distances.db' & 'souvenirs.db' have been connected successfully" << std::endl;
    return true;
}

double Database::GetDistance(const std::string& campus1, const std::string& campus2)
{
    if (distances_db == nullptr)
    {
        std::cerr << "Database not connected" << std::endl;
        return -1.0;
    }

    std::string query = "SELECT distance FROM distances WHERE starting_college = ? AND ending_college = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(distances_db, query.c_str(), -1, &stmt, nullptr);

    if (result != SQLITE_OK)
    {
        std::cout << "Failed to prepare query: " << sqlite3_errmsg(distances_db) << std::endl;
        return -1.0;
    }

    sqlite3_bind_text(stmt, 1, campus1.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, campus2.c_str(), -1, SQLITE_STATIC);

    double distance = -1.0;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        distance = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return distance;
}

void Database::CloseDB()
{
    if (distances_db != nullptr || souvenirs_db != nullptr)
    {
        sqlite3_close(distances_db);
        sqlite3_close(souvenirs_db);
        distances_db = nullptr;
        souvenirs_db = nullptr;
        std::cout << "Databases closed" << std::endl;
    }

}

std::vector<std::string> Database::GetAllCampuses()
{
    std::vector<std::string> campuses;

    if (souvenirs_db == nullptr)
    {
        std::cerr << "Souvenirs database not connected" << std::endl;
        return campuses;
    }

    std::string query = "SELECT DISTINCT college FROM souvenirs ORDER BY college";

    // Create pointer to database object
    sqlite3_stmt* stmt;

    // Compile database query
    int result = sqlite3_prepare_v2(souvenirs_db, query.c_str(), -1, &stmt, nullptr);

    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return campuses;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char* college_name = (const char*)sqlite3_column_text(stmt, 0);
        campuses.push_back(std::string(college_name));
    }

    sqlite3_finalize(stmt);

    return campuses;
}


std::vector<Souvenir> Database::GetSouvenirs(const std::string& campus)
{
    std::vector<Souvenir> souvenirs;

    if (souvenirs_db == nullptr)
    {
        std::cerr << "Souvenirs database not connected" << std::endl;
        return souvenirs;
    }

    std::string query = "SELECT college, item, price FROM souvenirs WHERE college = ?";
    sqlite3_stmt* stmt{nullptr};


    int result = sqlite3_prepare_v2(souvenirs_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return souvenirs;
    }


    sqlite3_bind_text(stmt, 1, campus.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Souvenir s;
        s.campus = (const char*)sqlite3_column_text(stmt, 0);
        s.item = (const char*)sqlite3_column_text(stmt, 1);
        s.price = sqlite3_column_double(stmt, 2);

        souvenirs.push_back(s);
    }

    sqlite3_finalize(stmt);

    return souvenirs;
}
