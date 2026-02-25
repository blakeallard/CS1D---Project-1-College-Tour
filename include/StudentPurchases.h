#ifndef STUDENT_PURCHASES_H
#define STUDENT_PURCHASES_H
#include <string>
#include <vector>
#include <unordered_map>

struct PurchasedItem
{
    std::string campus{};
    std::string item{};
    double price{};
    int quantity{};
};

class StudentPurchases
{
private:
    std::vector<PurchasedItem> allItems;
    std::unordered_map<std::string, std::vector<PurchasedItem>> itemsByCampus;

public:
    void AddItem(const std::string& campus, const std::string & item, double price, int quantity);
    std::vector<PurchasedItem> GetItemsByCampus(const std::string& campus);
    std::vector<PurchasedItem> GetAllItems();
};

#endif
