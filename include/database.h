#ifndef DATABASE_H_
#define DATABASE_H_
#include <string>
#include <sqlite3.h>
#include <vector>
#include <unordered_map>


struct Souvenir
{
    std::string campus;
    std::string item;
    double price = 0;
};

struct User
{
    int id = 0;
    std::string username;
    double wallet_balance = 0;
};

struct Purchase
{
    int id = 0;
    int user_id = 0;
    std::string campus;
    std::string item;
    double price = 0;
    int quantity = 0;
    std::string purchase_date;
};


class Database
{
private:
    sqlite3* distances_db{nullptr};
    sqlite3* souvenirs_db{nullptr};
    sqlite3* new_campuses_db{nullptr};
    sqlite3* users_db{nullptr};

public:
    // DATABASE -
    bool ConnectToDB(const std::string& distancesFile,
                     const std::string& souvenirsFile,
                     const std::string& newCampusesFile,
                     const std::string& usersFile);
    void CloseDB();

    // HELPER -
    double QueryDistance(sqlite3* db, const std::string& campus1, const std::string& campus2);
    void QueryAllDistancesFromDB(sqlite3* db, const std::string& campus, std::unordered_map<std::string, double>& distances);
    bool CampusExists(const std::string& campus);

    // DISTANCES -
    double GetDistance(const std::string& campus1, const std::string& campus2);
    std::unordered_map<std::string, double> GetAllDistances(const std::string& campus);

    // SOUVENIRS -
    std::vector<std::string> GetAllCampuses();
    std::vector<Souvenir> GetSouvenirs(const std::string& campus);
    Souvenir GetSouvenir(const std::string& campus, const std::string& item);
    double GetSouvenirPrice(const std::string& campus, const std::string& item);
    bool SouvenirExists(const std::string& campus, const std::string& item);

    // USERS -
    int AuthenticateUser(const std::string& username, const std::string& password);
    User GetUser(int user_id);
    User GetUserByUsername(const std::string& username);
    bool UserExists(const std::string& username);
    int CreateUser(const std::string& username, const std::string& password, double initial_balance);

    // WALLET -
    double GetWalletBalance(int user_id);
    bool AddFunds(int user_id, double amount);
    bool DeductFunds(int user_id, double amount);

    // PURCHASES -
    bool AddPurchase(int user_id, const std::string& campus, const std::string& item, double price, int quantity);
    std::vector<Purchase> GetUserPurchases(int user_id);
    std::vector<Purchase> GetUserPurchasesByCampus(int user_id, const std::string& campus);
    double GetTotalSpent(int user_id);
    double GetSpendingByCampus(int user_id, const std::string& campus);
    int GetPurchaseCount(int user_id);

    // ADMIN -
    bool AddSouvenir(const std::string& campus, const std::string& item, double price);
    bool UpdateSouvenirPrice(const std::string& campus, const std::string& item, double new_price);
    bool DeleteSouvenir(const std::string& campus, const std::string& item);
};

#endif
