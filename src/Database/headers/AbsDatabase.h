#ifndef ABS_DATABASE_H
#define ABS_DATABASE_H
#include "Queries.h"
#include <sqlite3.h>
#include <stdexcept>
#include <string>

using SQLiteValue = std::variant<int, double, std::string, std::nullptr_t>;
class ABSDatabase
{
    public:
        ABSDatabase()
        {
        }

        ABSDatabase(const std::string &file)
        {
            ConnectToDB(file);
        }

        ~ABSDatabase()
        {
            CloseDB();
        }

        void ConnectToDB(const std::string &file);
        void CloseDB();
        sqlite3 *get() const;
        void set(sqlite3 *aG)
        {
            generic_db = aG;
        }

    private:
        sqlite3 *generic_db{nullptr};
};

class Statement
{
    private:
        sqlite3_stmt *stmt{nullptr};
        sqlite3 *absDB;

    public:
        sqlite3_stmt *getStatement()
        {
            return stmt;
        }

        Statement(sqlite3 *db, const std::string &sql)
        {
            absDB = db;
            createStatement(db, sql);
        }

        void createStatement(sqlite3 *db, const std::string &sql)
        {
            int result =
                sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
            if (result != SQLITE_OK)
            {
                throw std::runtime_error(sqlite3_errmsg(db));
            }
        }

        ~Statement()
        {
            sqlite3_finalize(stmt);
        }

        void bind(int index, int value)
        {
            sqlite3_bind_int(stmt, index, value);
        }

        void bind(int index, double value)
        {
            sqlite3_bind_double(stmt, index, value);
        }

        void bind(int index, const std::string &value)
        {
            sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
        }

        void bind(int index)
        {
            sqlite3_bind_null(stmt, index);
        }

        void bind(int index, SQLiteValue value)
        {
            if (std::holds_alternative<int>(value))
            {
                bind(index, std::get<int>(value));
            }
            else if (std::holds_alternative<double>(value))
            {
                bind(index, std::get<double>(value));
            }
            else if (std::holds_alternative<std::string>(value))
            {
                bind(index, std::get<std::string>(value));
            }
            else if (std::holds_alternative<std::nullptr_t>(value))
            {
                sqlite3_bind_null(stmt, index);
            }
        }

        bool step()
        {
            return sqlite3_step(stmt) == SQLITE_ROW;
        }

        void execute()
        {
            if (sqlite3_step(stmt) != SQLITE_DONE)
            {
                throw std::runtime_error(sqlite3_errmsg(absDB));
            }
        }

        int getInt(int col)
        {
            return sqlite3_column_int(stmt, col);
        }

        std::string getText(int col)
        {
            return reinterpret_cast<const char *>(
                sqlite3_column_text(stmt, col));
        }
};

#endif
