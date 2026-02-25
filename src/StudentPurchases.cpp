#include "../include/StudentPurchases.h"

void StudentPurchases::AddItem(const std::string& campus, const std::string& item, double price, int quantity)
{
    PurchasedItem purchase;
    purchase.campus   = campus;
    purchase.item     = item;
    purchase.price    = price;
    purchase.quantity = quantity;

    allItems.push_back(purchase);
    itemsByCampus[campus].push_back(purchase);
}

std::vector<PurchasedItem> StudentPurchases::GetItemsByCampus(const std::string& campus)
{
    return itemsByCampus[campus];
}

std::vector<PurchasedItem> StudentPurchases::GetAllItems()
{
    return allItems;
}
