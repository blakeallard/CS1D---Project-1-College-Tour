#include <AbsDatabase.h>
#include <Queries.h>
#include <DatabaseTypes.h>
#include <unordered_map>
#include <vector>

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

//SOUVENIRS
void souvenirs(const std::string &campus,
                  std::vector<SouvenirItem> &souvenirs)
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

} // namespace DataQuery
