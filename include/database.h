#ifndef DATABASE_H_
#define DATABASE_H_
#include <string>
#include <sqlite3.h>
#include <vector>


struct Souvenir 
{
    std::string campus;
    std::string item;
    double price;
};


class Database 
{
private:
    sqlite3* distances_db{nullptr};
    sqlite3* souvenirs_db{nullptr};

public:
    bool ConnectToDB(const std::string& distancesFile, const std::string& souvenirsFile);
    double GetDistance(const std::string& campus1, const std::string& campus2);
    std::vector<std::string> GetAllCampuses();
    std::vector<Souvenir> GetSouvenirs(const std::string& campus);
    void CloseDB();

};

#endif
