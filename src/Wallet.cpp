#include "../include/Wallet.h"

Wallet::Wallet() : balance(0.0), totalSpent(0.0) {}

void Wallet::AddFunds(double amount)
{
    balance += amount;
}

bool Wallet::DeductFunds(const std::string& campus, double amount)
{
    if (amount > balance)
    {
        return false;
    }

    balance -= amount;
    totalSpent += amount;
    spendingByCampus[campus] += amount;

    return true;
}

double Wallet::GetBalance()
{
    return balance;
}

double Wallet::GetTotalSpent()
{
    return totalSpent;
}

double Wallet::GetSpendingByCampus(const std::string& campus)
{
    return spendingByCampus[campus];
}
