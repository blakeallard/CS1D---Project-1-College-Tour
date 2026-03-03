#include "../include/database.h"
#include <iostream>
#include <sqlite3.h>

// SOUVENIRS -
std::vector<Souvenir> Database::GetSouvenirs(const std::string &campus)
{
    std::vector<Souvenir> souvenirs;

    if (souvenirs_db == nullptr)
    {
        std::cerr << "Souvenirs database not connected" << std::endl;
        return souvenirs;
    }

    std::string query =
        "SELECT college, item, price FROM souvenirs WHERE college = ?";
    sqlite3_stmt *stmt{nullptr};

    int result =
        sqlite3_prepare_v2(souvenirs_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return souvenirs;
    }

    sqlite3_bind_text(stmt, 1, campus.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Souvenir s;
        s.campus = (const char *)sqlite3_column_text(stmt, 0);
        s.item   = (const char *)sqlite3_column_text(stmt, 1);
        s.price  = sqlite3_column_double(stmt, 2);

        souvenirs.push_back(s);
    }

    sqlite3_finalize(stmt);

    return souvenirs;
}

Souvenir Database::GetSouvenir(const std::string &campus,
                               const std::string &item)
{
    Souvenir s;
    s.campus = "";
    s.item   = "";
    s.price  = -1.0;

    if (souvenirs_db == nullptr)
    {
        std::cerr << "Souvenirs database not connected" << std::endl;
        return s;
    }

    std::string query = "SELECT college, item, price FROM souvenirs WHERE "
                        "college = ? AND item = ?";
    sqlite3_stmt *stmt;

    int result =
        sqlite3_prepare_v2(souvenirs_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return s;
    }

    sqlite3_bind_text(stmt, 1, campus.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, item.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        s.campus = (const char *)sqlite3_column_text(stmt, 0);
        s.item   = (const char *)sqlite3_column_text(stmt, 1);
        s.price  = sqlite3_column_double(stmt, 2);
    }

    sqlite3_finalize(stmt);
    return s;
}

double Database::GetSouvenirPrice(const std::string &campus,
                                  const std::string &item)
{
    if (souvenirs_db == nullptr)
    {
        std::cerr << "Sourvenirs database not connected" << std::endl;
        return -1.0;
    }

    std::string query =
        "SELECT price FROM souvenirs WHERE college = ? AND item = ?";
    sqlite3_stmt *stmt;

    int result =
        sqlite3_prepare_v2(souvenirs_db, query.c_str(), -1, &stmt, nullptr);
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

// USERS
int Database::AuthenticateUser(const std::string &username,
                               const std::string &password)
{
    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return -1;
    }

    std::string query =
        "SELECT id FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt *stmt;

    int result =
        sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
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
    u.id             = -1;
    u.username       = "";
    u.wallet_balance = 0.0;

    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return u;
    }

    std::string query =
        "SELECT id, username, wallet_balance FROM users WHERE id = ?";
    sqlite3_stmt *stmt;

    int result =
        sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return u;
    }

    sqlite3_bind_int(stmt, 1, user_id);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        u.id             = sqlite3_column_int(stmt, 0);
        u.username       = (const char *)sqlite3_column_text(stmt, 1);
        u.wallet_balance = sqlite3_column_double(stmt, 2);
    }

    sqlite3_finalize(stmt);
    return u;
}

User Database::GetUserByUsername(const std::string &username)
{
    User u;
    u.id             = -1;
    u.username       = "";
    u.wallet_balance = 0.0;

    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return u;
    }

    std::string query =
        "SELECT id, username, wallet_balance FROM users WHERE username = ?";
    sqlite3_stmt *stmt;

    int result =
        sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare query" << std::endl;
        return u;
    }

    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        u.id             = sqlite3_column_int(stmt, 0);
        u.username       = (const char *)sqlite3_column_text(stmt, 1);
        u.wallet_balance = sqlite3_column_double(stmt, 2);
    }

    sqlite3_finalize(stmt);
    return u;
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
    sqlite3_stmt *stmt;

    int result =
        sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
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

    std::string query =
        "UPDATE users SET wallet_balance = wallet_balance + ? WHERE id = ?";
    sqlite3_stmt *stmt;

    int result =
        sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
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

    std::string query =
        "UPDATE users SET wallet_balance = wallet_balance - ? WHERE id = ?";
    sqlite3_stmt *stmt;

    int result =
        sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
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

std::vector<Purchase> Database::GetUserPurchases(int user_id)
{
    std::vector<Purchase> purchases;

    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return purchases;
    }

    std::string query =
        "SELECT id, user_id, campus, item, price, quantity, purchase_date FROM "
        "user_purchases WHERE user_id = ? ORDER BY purchase_date DESC";
    sqlite3_stmt *stmt;

    int result =
        sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
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
        p.campus        = (const char *)sqlite3_column_text(stmt, 2);
        p.item          = (const char *)sqlite3_column_text(stmt, 3);
        p.price         = sqlite3_column_double(stmt, 4);
        p.quantity      = sqlite3_column_int(stmt, 5);
        p.purchase_date = (const char *)sqlite3_column_text(stmt, 6);
        purchases.push_back(p);
    }

    sqlite3_finalize(stmt);
    return purchases;
}

std::vector<Purchase>
Database::GetUserPurchasesByCampus(int user_id, const std::string &campus)
{
    std::vector<Purchase> purchases;

    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return purchases;
    }

    std::string query = "SELECT id, user_id, campus, item, price, quantity, "
                        "purchase_date FROM user_purchases WHERE user_id = ? "
                        "AND campus = ? ORDER BY purchase_date DESC";
    sqlite3_stmt *stmt;

    int result =
        sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
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
        p.campus        = (const char *)sqlite3_column_text(stmt, 2);
        p.item          = (const char *)sqlite3_column_text(stmt, 3);
        p.price         = sqlite3_column_double(stmt, 4);
        p.quantity      = sqlite3_column_int(stmt, 5);
        p.purchase_date = (const char *)sqlite3_column_text(stmt, 6);
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

    std::string query =
        "SELECT SUM(price * quantity) FROM user_purchases WHERE user_id = ?";
    sqlite3_stmt *stmt;

    int result =
        sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
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

double Database::GetSpendingByCampus(int user_id, const std::string &campus)
{
    if (users_db == nullptr)
    {
        std::cerr << "Users database not connected" << std::endl;
        return 0.0;
    }

    std::string query = "SELECT SUM(price * quantity) FROM user_purchases "
                        "WHERE user_id = ? AND campus = ?";
    sqlite3_stmt *stmt;

    int result =
        sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
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
    sqlite3_stmt *stmt;

    int result =
        sqlite3_prepare_v2(users_db, query.c_str(), -1, &stmt, nullptr);
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
