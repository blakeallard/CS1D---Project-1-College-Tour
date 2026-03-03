#include <AbsDatabase.h>
#include <DatabaseTypes.h>
#include <Queries.h>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using SQLiteValue = std::variant<int, double, std::string, std::nullptr_t>;
namespace QueryData
{
// DISTANCES
double distance(const std::string &campus1, const std::string &campus2)
{
    ABSDatabase db(DISTANCES);
    Statement st(db.get(),
                 "SELECT distance FROM distances WHERE starting_college "
                 "= ? AND ending_college = ?");

    st.bind(1, campus1);
    st.bind(2, campus2);
    double distance = -1.0;

    if (st.step())
    {
        distance = st.getInt(0);
    }

    return distance;
}

void allDistances(const std::string &campus,
                  std::unordered_map<std::string, double> &distances)
{
    ABSDatabase db(DISTANCES);
    Statement st(db.get(), "SELECT ending_college, distance "
                           "FROM distances WHERE starting_college = ?");
    st.bind(1, campus);
    while (st.step())
    {
        std::string dest = st.getText(0);
        double dist      = st.getInt(1);
        distances[dest]  = dist;
    }
}

void allCampuses(std::vector<std::string> &campuses)
{
    ABSDatabase db(SOUVENIRS);
    Statement st(db.get(),
                 "SELECT DISTINCT college FROM souvenirs ORDER BY college");
    while (st.step())
    {
        const std::string collegeName = st.getText(0);
        campuses.push_back(collegeName);
    }
}

// SOUVENIRS
void souvenirs(const std::string &campus, std::vector<SouvenirItem> &souvenirs)
{
    ABSDatabase db(SOUVENIRS);
    Statement st(
        db.get(),
        "SELECT college, item, price FROM souvenirs WHERE college = ?");
    st.bind(1, campus);

    SouvenirItem s;
    while (st.step())
    {
        s.campus = st.getText(0);
        s.item   = st.getText(1);
        s.price  = st.getInt(2);
        souvenirs.push_back(s);
    }
}

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
                const std::vector<SQLiteValue> &updateValues)
{
    if (updateColumns.size() != updateValues.size())
        return false;

    if (whereColumns.size() != whereValues.size())
        return false;

    std::stringstream ss;
    ss << "UPDATE " << table << " SET ";

    for (size_t i = 0; i < updateColumns.size(); ++i)
    {
        ss << updateColumns[i] << " = ?";
        if (i < updateColumns.size() - 1)
            ss << ", ";
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

    ABSDatabase db("Databases/" + dbName);
    Statement st(db.get(), sql);
    int bindIndex = 1;

    // Bind SET values first
    int index = 1;
    for (int i = 0; i < updateValues.size(); ++i)
    {
        st.bind(index, updateValues[i]);
        index++;
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

} // namespace QueryData
