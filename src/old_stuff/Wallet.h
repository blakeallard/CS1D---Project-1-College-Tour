#ifndef WALLET_H_
#define WALLET_H_
#include <string>
#include <unordered_map>

class Wallet
{
private:
    double balance{};
    double totalSpent{};
    std::unordered_map<std::string, double> spendingByCampus;

public:
    Wallet();
    void AddFunds(double amount);
    bool DeductFunds(const std::string &campus, double amount);
    double GetBalance();
    double GetTotalSpent();
    double GetSpendingByCampus(const std::string& campus);
};

#endif
