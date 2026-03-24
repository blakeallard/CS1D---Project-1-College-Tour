#include <AbsDatabase.h>
#include <Queries.h>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using SQLiteValue = std::variant<int, double, std::string, std::nullptr_t>;
using Row         = std::unordered_map<std::string, SQLiteValue>;
using QueryResult = std::vector<Row>;

namespace QueryData
{

bool insertRow(const std::string &dbName, const std::string &table,
               const std::vector<std::string> &columns,
               const std::vector<SQLiteValue> &values)
{
    if (columns.size() != values.size())
        return false;

    // Build SQL
    std::stringstream ss;
    ss << "INSERT INTO " << table << " (";

    for (size_t i = 0; i < columns.size(); ++i)
    {
        ss << columns[i];
        if (i < columns.size() - 1)
            ss << ", ";
    }

    ss << ") VALUES (";

    for (size_t i = 0; i < values.size(); ++i)
    {
        ss << "?";
        if (i < values.size() - 1)
            ss << ", ";
    }

    ss << ");";

    std::string sql = ss.str();

    ABSDatabase db("Databases/" + dbName);
    Statement st(db.get(), sql);
    for (int i = 0; i < values.size(); ++i)
    {
        st.bind(i + 1, values[i]);
    }
    st.execute();
    return true;
}

bool deleteRow(const std::string &dbName, const std::string &table,
               const std::vector<std::string> &whereColumns,
               const std::vector<SQLiteValue> &whereValues)
{
    if (whereColumns.size() != whereValues.size())
        return false;

    std::stringstream ss;
    ss << "DELETE FROM " << table;

    if (!whereColumns.empty())
    {
        ss << " WHERE ";
        for (size_t i = 0; i < whereColumns.size(); ++i)
        {
            ss << whereColumns[i] << " = ?";
            if (i < whereColumns.size() - 1)
                ss << " AND ";
        }
    }

    ss << ";";
    std::string sql = ss.str();

    ABSDatabase db("Databases/" + dbName);
    Statement st(db.get(), sql);

    for (int i = 0; i < whereValues.size(); ++i)
    {
        st.bind(i + 1, whereValues[i]);
    }

    st.execute();

    return true;
}

bool updateRows(const std::string &dbName, const std::string &table,
                const std::vector<std::string> &whereColumns,
                const std::vector<SQLiteValue> &whereValues,
                const std::vector<std::string> &updateColumns,
                const std::vector<SQLiteValue> &updateValues,
                const std::string &mathValue)
{
    if (updateColumns.size() != updateValues.size() && mathValue == "")
        return false;

    if (whereColumns.size() != whereValues.size())
        return false;

    std::stringstream ss;
    ss << "UPDATE " << table << " SET ";

    if (mathValue != "")
        ss << updateColumns[0] << " = " << updateColumns[0] << " " << mathValue;
    else
    {

        for (size_t i = 0; i < updateColumns.size(); ++i)
        {
            ss << updateColumns[i] << " = ?";

            if (i < updateColumns.size() - 1)
                ss << ", ";
        }
    }

    if (!whereColumns.empty())
    {
        ss << " WHERE ";
        for (size_t i = 0; i < whereColumns.size(); ++i)
        {
            ss << whereColumns[i] << " = ?";
            if (i < whereColumns.size() - 1)
                ss << " AND ";
        }
    }

    ss << ";";
    std::string sql = ss.str();
    std::cout << sql << std::endl;

    ABSDatabase db("Databases/" + dbName);
    Statement st(db.get(), sql);
    int bindIndex = 1;

    // Bind SET values first
    int index = 1;
    if (mathValue == "")
    {
        for (int i = 0; i < updateValues.size(); ++i)
        {
            st.bind(index, updateValues[i]);
            index++;
        }
    }

    // Bind SET values first
    for (int i = 0; i < whereValues.size(); ++i)
    {
        st.bind(index, whereValues[i]);
        index++;
    }

    st.execute();
    return true;
}

void selectHelper(QueryResult &results, Statement &st)
{
    while (st.step())
    {
        Row row;
        int colCount = sqlite3_column_count(st.getStatement());

        for (int i = 0; i < colCount; ++i)
        {
            std::string name = sqlite3_column_name(st.getStatement(), i);
            int type         = sqlite3_column_type(st.getStatement(), i);

            switch (type)
            {
                case SQLITE_INTEGER:
                    row[name] = sqlite3_column_int(st.getStatement(), i);
                    break;

                case SQLITE_FLOAT:
                    row[name] = sqlite3_column_double(st.getStatement(), i);
                    break;

                case SQLITE_TEXT:
                    row[name] = std::string(reinterpret_cast<const char *>(
                        sqlite3_column_text(st.getStatement(), i)));
                    break;

                case SQLITE_NULL:
                    row[name] = nullptr;
                    break;
            }
        }

        results.push_back(row);
    }
}

QueryResult selectRows(const std::string &dbName, const std::string &table,
                       const std::vector<std::string> &columns,
                       const std::vector<std::string> &whereColumns,
                       const std::vector<SQLiteValue> &whereValues)
{
    QueryResult results;

    if (whereColumns.size() != whereValues.size())
        return results;

    std::stringstream ss;

    ss << "SELECT ";

    if (columns.empty())
        ss << "*";
    else
    {
        for (size_t i = 0; i < columns.size(); ++i)
        {
            ss << columns[i];
            if (i < columns.size() - 1)
                ss << ", ";
        }
    }

    ss << " FROM " << table;

    if (!whereColumns.empty())
    {
        ss << " WHERE ";
        for (size_t i = 0; i < whereColumns.size(); ++i)
        {
            ss << whereColumns[i] << " = ?";
            if (i < whereColumns.size() - 1)
                ss << " AND ";
        }
    }

    ss << ";";

    std::string sql = ss.str();

    ABSDatabase db("Databases/" + dbName);
    Statement st(db.get(), sql);

    // Bind WHERE values
    for (size_t i = 0; i < whereValues.size(); ++i)
        st.bind(i + 1, whereValues[i]);

    selectHelper(results, st);

    return results;
}

QueryResult selectRowsWithQuery(const std::string &dbName,
                                const std::string &query)
{

    QueryResult results;
    ABSDatabase db("Databases/" + dbName);
    Statement st(db.get(), query);
    selectHelper(results, st);

    return results;
}

bool customExecuteQuery(const std::string &dbName, const std::string &query)
{
    ABSDatabase db("Databases/" + dbName);
    Statement st(db.get(), query);
    st.execute();
    return true;
}

} // namespace QueryData
