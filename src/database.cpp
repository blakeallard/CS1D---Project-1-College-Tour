#include "../include/database.h"
#include <iostream>
#include <sqlite3.h>

bool Database::ConnectToDB(const std::string& distancesFile,
                           const std::string& souvenirsFile,
                           const std::string& newCampusesFile,
                           const std::string& usersFile)
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

    // Open newCampusesFile.db
    result = sqlite3_open(newCampusesFile.c_str(), &new_campuses_db);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to connect to 'newCampusesFile.db' database" << std::endl;
        sqlite3_close(souvenirs_db);
        sqlite3_close(distances_db);
        sqlite3_close(new_campuses_db);
        return false;
    }

    // Open users.db
    result = sqlite3_open(usersFile.c_str(), &users_db);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to connect to users.db" << std::endl;
        sqlite3_close(distances_db);
        sqlite3_close(souvenirs_db);
        sqlite3_close(new_campuses_db);
        sqlite3_close(users_db);
        return false;
    }

    std::cout << "All databases connected successfully" << std::endl;
    return true;
}

void Database::CloseDB()
{
    if (distances_db != nullptr)
    {
        sqlite3_close(distances_db);
        distances_db = nullptr;
    }

    if (souvenirs_db != nullptr)
    {
        sqlite3_close(souvenirs_db);
        souvenirs_db = nullptr;
    }

    if (new_campuses_db != nullptr)
    {
        sqlite3_close(new_campuses_db);
        new_campuses_db = nullptr;
    }

    if (users_db != nullptr)
    {
        sqlite3_close(users_db);
        users_db = nullptr;
    }

    std::cout << "All databases closed" << std::endl;
}


// HELPER -
double Database::QueryDistance(sqlite3* db, const std::string& campus1, const std::string& campus2)
{
    if (!db)
    {
        return -1.0;
    }

    std::string query = "SELECT distance FROM distances WHERE starting_college = ? AND ending_college = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "QueryDistance prepare failed: " << sqlite3_errmsg(db) << std::endl;
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

void Database::QueryAllDistancesFromDB(sqlite3* db, const std::string& campus, std::unordered_map<std::string, double>& distances)
{
    if (!db)
    {
        return;
    }

    std::string query =
        "SELECT ending_college, distance "
        "FROM distances WHERE starting_college = ?";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
    {
        return;
    }

    sqlite3_bind_text(stmt, 1, campus.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string dest =
            (const char*)sqlite3_column_text(stmt, 0);

        double dist =
            sqlite3_column_double(stmt, 1);

        distances[dest] = dist;
    }

    sqlite3_finalize(stmt);
}

bool Database::CampusExists(const std::string& campus)
{
    std::string query = "SELECT COUNT(*) FROM distances WHERE starting_college = ? OR ending_college = ?";

    sqlite3_stmt* stmt;

    // Check original distances DB
    int result = sqlite3_prepare_v2(distances_db, query.c_str(), -1, &stmt, nullptr);

    if (result != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, campus.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, campus.c_str(), -1, SQLITE_STATIC);

    bool exists = false;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        exists = (sqlite3_column_int(stmt, 0) > 0);
    }

    sqlite3_finalize(stmt);

    if (exists)
    {
        return true;
    }

    // Check new campuses DB
    result = sqlite3_prepare_v2(new_campuses_db, query.c_str(), -1, &stmt, nullptr);

    if (result != SQLITE_OK)
    {
        return false;
    }

    sqlite3_bind_text(stmt, 1, campus.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, campus.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        exists = (sqlite3_column_int(stmt, 0) > 0);
    }

    sqlite3_finalize(stmt);

    return exists;
}

// DISTANCES -
double Database::GetDistance(const std::string& campus1, const std::string& campus2)
{
    double d = QueryDistance(distances_db, campus1, campus2);

    if (d >= 0)
    {
        return d;
    }

    return QueryDistance(new_campuses_db, campus1, campus2);
}

std::unordered_map<std::string, double> Database::GetAllDistances(const std::string& campus)
{
    std::unordered_map<std::string, double> distances;

    QueryAllDistancesFromDB(distances_db, campus, distances);
    QueryAllDistancesFromDB(new_campuses_db, campus, distances);

    return distances;
/*
    if (distances_db == nullptr)
    {
        std::cerr << "Database not connected" << std::endl;
        return distances;
    }

    std::string query = "SELECT ending_college, distance FROM distances WHERE starting_college = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(distances_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query: " << sqlite3_errmsg(distances_db) << std::endl;
        return distances;
    }

    sqlite3_bind_text(stmt, 1, campus.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        std::string destination = (const char*)sqlite3_column_text(stmt, 0);
        double distance = sqlite3_column_double(stmt, 1);
        distances[destination] = distance;
    }

    sqlite3_finalize(stmt);
    return distances;
    */
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
        std::cerr << "GetAllCampuses: Failed to prepare query " << sqlite3_errmsg(souvenirs_db) << std::endl;
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

// SOUVENIRS -
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

Souvenir Database::GetSouvenir(const std::string& campus, const std::string& item)
{
    Souvenir s;
    s.campus = "";
    s.item = "";
    s.price = -1.0;

    if (souvenirs_db == nullptr)
    {
        std::cerr << "Souvenirs database not connected" << std::endl;
        return s;
    }

    std::string query = "SELECT college, item, price FROM souvenirs WHERE college = ? AND item = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(souvenirs_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return s;
    }

    sqlite3_bind_text(stmt, 1, campus.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, item.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        s.campus = (const char*)sqlite3_column_text(stmt, 0);
        s.item   = (const char*)sqlite3_column_text(stmt, 1);
        s.price  = sqlite3_column_double(stmt, 2);
    }

    sqlite3_finalize(stmt);
    return s;
}

double Database::GetSouvenirPrice(const std::string& campus, const std::string& item)
{
    if (souvenirs_db == nullptr)
    {
        std::cerr << "Sourvenirs database not connected" << std::endl;
        return -1.0;
    }

    std::string query = "SELECT price FROM souvenirs WHERE college = ? AND item = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(souvenirs_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return -1.0;
    }

    sqlite3_bind_text(stmt, 1, campus.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, item.c_str(), -1, SQLITE_STATIC);

    double price = -1.0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        price = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return price;
}

bool Database::SouvenirExists(const std::string& campus, const std::string& item)
{
    if (souvenirs_db == nullptr)
    {
        std::cerr << "Souvenirs database not connected" << std::endl;
        return false;
    }

    std::string query = "SELECT COUNT(*) FROM souvenirs WHERE college = ? AND item = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(souvenirs_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, campus.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, item.c_str(), -1, SQLITE_STATIC);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int count = sqlite3_column_int(stmt, 0);
        exists = (count > 0);
    }

    sqlite3_finalize(stmt);
    return exists;
}

// USERS
int Database::AuthenticateUser(const std::string& username, const std::string& password)
{
    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return -1;
    }

    std::string query = "SELECT id FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return -1;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    int user_id = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        user_id = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return user_id;
}

User Database::GetUser(int user_id)
{
    User u;
    u.id = -1;
    u.username = "";
    u.wallet_balance = 0.0;

    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return u;
    }

    std::string query = "SELECT id, username, wallet_balance FROM users WHERE id = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return u;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        u.id = sqlite3_column_int(stmt, 0);
        u.username = (const char*)sqlite3_column_text(stmt, 1);
        u.wallet_balance = sqlite3_column_double(stmt, 2);
    }

    sqlite3_finalize(stmt);
    return u;
}

User Database::GetUserByUsername(const std::string& username)
{
    User u;
    u.id = -1;
    u.username = "";
    u.wallet_balance = 0.0;

    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return u;
    }

    std::string query = "SELECT id, username, wallet_balance FROM users WHERE username = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return u;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        u.id = sqlite3_column_int(stmt, 0);
        u.username = (const char*)sqlite3_column_text(stmt, 1);
        u.wallet_balance = sqlite3_column_double(stmt, 2);
    }

    sqlite3_finalize(stmt);
    return u;
}

bool Database::UserExists(const std::string& username)
{
    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return false;
    }

    std::string query = "SELECT COUNT(*) FROM users WHERE username = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int count = sqlite3_column_int(stmt, 0);
        exists = (count > 0);
    }

    sqlite3_finalize(stmt);
    return exists;
}

int Database::CreateUser(const std::string& username, const std::string& password, double initial_balance)
{
    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return -1;
    }

    std::string query = "INSERT INTO users (username, password, wallet_balance) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return -1;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, initial_balance);

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (result == SQLITE_DONE)
    {
        return sqlite3_last_insert_rowid(users_db);
    }

    return -1;
}

// WALLET -
double Database::GetWalletBalance(int user_id)
{
    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return -1.0;
    }

    std::string query = "SELECT wallet_balance FROM users WHERE id = ?";
    sqlite3_stmt* stmt;


    int result = sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return -1.0;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    double balance = -1.0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        balance = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return balance;
}

bool Database::AddFunds(int user_id, double amount)
{
    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return false;
    }

    std::string query = "UPDATE users SET wallet_balance = wallet_balance + ? WHERE id = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return false;
    }

    sqlite3_bind_double(stmt, 1, amount);
    sqlite3_bind_int(stmt, 2, user_id);

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (result == SQLITE_DONE);
}

bool Database::DeductFunds(int user_id, double amount)
{
    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return false;
    }

    std::string query = "UPDATE users SET wallet_balance = wallet_balance - ? WHERE id = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return false;
    }

    sqlite3_bind_double(stmt, 1, amount);
    sqlite3_bind_int(stmt, 2, user_id);

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (result == SQLITE_DONE);
}

// PURCHASES -
bool Database::AddPurchase(int user_id, const std::string& campus, const std::string& item, double price, int quantity)
{
    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return false;
    }

    std::string query = "INSERT INTO user_purchases (user_id, campus, item, price, quantity) VALUES (? ,? ,? ,?, ?)";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, campus.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, item.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, price);
    sqlite3_bind_int(stmt, 5, quantity);

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (result == SQLITE_DONE);
}

std::vector<Purchase> Database::GetUserPurchases(int user_id)
{
    std::vector<Purchase> purchases;

    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return purchases;
    }

    std::string query = "SELECT id, user_id, campus, item, price, quantity, purchase_date FROM user_purchases WHERE user_id = ? ORDER BY purchase_date DESC";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return purchases;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Purchase p;
        p.id            = sqlite3_column_int(stmt, 0);
        p.user_id       = sqlite3_column_int(stmt, 1);
        p.campus        = (const char*)sqlite3_column_text(stmt, 2);
        p.item          = (const char*)sqlite3_column_text(stmt, 3);
        p.price         = sqlite3_column_double(stmt, 4);
        p.quantity      = sqlite3_column_int(stmt, 5);
        p.purchase_date = (const char*)sqlite3_column_text(stmt, 6);
        purchases.push_back(p);
    }

    sqlite3_finalize(stmt);
    return purchases;
}

std::vector<Purchase> Database::GetUserPurchasesByCampus(int user_id, const std::string& campus)
{
    std::vector<Purchase> purchases;

    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return purchases;
    }

    std::string query = "SELECT id, user_id, campus, item, price, quantity, purchase_date FROM user_purchases WHERE user_id = ? AND campus = ? ORDER BY purchase_date DESC";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return purchases;
    }

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, campus.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Purchase p;
        p.id            = sqlite3_column_int(stmt, 0);
        p.user_id       = sqlite3_column_int(stmt, 1);
        p.campus        = (const char*)sqlite3_column_text(stmt, 2);
        p.item          = (const char*)sqlite3_column_text(stmt, 3);
        p.price         = sqlite3_column_double(stmt, 4);
        p.quantity      = sqlite3_column_int(stmt, 5);
        p.purchase_date = (const char*)sqlite3_column_text(stmt, 6);
        purchases.push_back(p);
    }

    sqlite3_finalize(stmt);
    return purchases;
}

double Database::GetTotalSpent(int user_id)
{
    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return 0.0;
    }

    std::string query = "SELECT SUM(price * quantity) FROM user_purchases WHERE user_id = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return 0.0;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    double total = 0.0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        total = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return total;
}

double Database::GetSpendingByCampus(int user_id, const std::string& campus)
{
    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return 0.0;
    }

    std::string query = "SELECT SUM(price * quantity) FROM user_purchases WHERE user_id = ? AND campus = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return 0.0;
    }

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_text(stmt, 2, campus.c_str(), -1, SQLITE_STATIC);

    double total = 0.0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        total = sqlite3_column_double(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return total;
}

int Database::GetPurchaseCount(int user_id)
{
    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return 0;
    }

    std::string query = "SELECT COUNT(*) FROM user_purchases WHERE user_id = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return 0;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return count;
}

// ADMIN -
bool Database::AddSouvenir(const std::string& campus, const std::string& item, double price)
{
    if (souvenirs_db == nullptr)
    {
        std::cerr << "Souvenirs database not connected" << std::endl;
        return false;
    }

    if (!CampusExists(campus))
    {
        return false;
    }

    if (SouvenirExists(campus, item))
    {
        return false;
    }

    std::string query = "INSERT INTO souvenirs (college, item, price) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(souvenirs_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, campus.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, item.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, price);

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (result == SQLITE_DONE);
}

bool Database::UpdateSouvenirPrice(const std::string& campus, const std::string& item, double new_price)
{
    if (souvenirs_db == nullptr)
    {
        std::cerr << "Souvenirs database not connected" << std::endl;
        return false;
    }

    std::string query = "UPDATE souvenirs SET price = ? WHERE college = ? AND item = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(souvenirs_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return false;
    }

    sqlite3_bind_double(stmt, 1, new_price);
    sqlite3_bind_text(stmt, 2, campus.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, item.c_str(), -1, SQLITE_STATIC);

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (result == SQLITE_DONE);
}

bool Database::DeleteSouvenir(const std::string& campus, const std::string& item)
{
    if (souvenirs_db == nullptr)
    {
        std::cerr << "Souvenirs database not connected" << std::endl;
        return false;
    }

    std::string query = "DELETE FROM souvenirs WHERE college = ? AND item = ?";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(souvenirs_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return false;
    }

    sqlite3_bind_text(stmt, 1, campus.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, item.c_str(), -1, SQLITE_STATIC);

    result = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return (result == SQLITE_DONE);
}
