#ifndef ABS_DATABASE_H
#define ABS_DATABASE_H
#include <sqlite3.h>
#include <stdexcept>
#include <string>

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

    private:
        sqlite3 *generic_db{nullptr};
};

class Statement
{
    private:
        sqlite3_stmt *stmt{nullptr};

    public:
        Statement(sqlite3 *db, const std::string &sql)
        {
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

        void bind(int index, const std::string &value)
        {
            sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
        }

        bool step()
        {
            return sqlite3_step(stmt) == SQLITE_ROW;
        }

        void execute()
        {
            sqlite3_step(stmt); // for INSERT/UPDATE/DELETE
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
