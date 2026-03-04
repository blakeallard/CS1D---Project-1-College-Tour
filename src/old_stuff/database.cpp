#include "../include/database.h"
#include <iostream>
#include <sqlite3.h>

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
