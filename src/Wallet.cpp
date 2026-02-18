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
