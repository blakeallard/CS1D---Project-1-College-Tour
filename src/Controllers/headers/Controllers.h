#ifndef CONTROLLERS_H
#define CONTROLLERS_H
#include <crow.h>
#include "../../../include/database.h"
#include "../../../include/Wallet.h"
#include "../../../include/StudentPurchases.h"

struct UniversityController
{
        static crow::json::wvalue read(std::string id);
        static crow::json::wvalue patch(std::string id);
        static crow::json::wvalue create(std::string id);
        static bool remove(std::string id);
};

struct UserController
{
        static Wallet wallet;
        static StudentPurchases purchases;

        static crow::json::wvalue read(std::string id);
        static crow::json::wvalue patch(std::string id);
        static crow::json::wvalue create(std::string id);
        static bool remove(std::string id);
};

struct AdminController
{
        static crow::json::wvalue read(std::string id);
        static crow::json::wvalue patch(std::string id);
        static crow::json::wvalue create(std::string id);
        static bool remove(std::string id);
};

struct TourController
{
        static crow::json::wvalue read(std::string id);
        static crow::json::wvalue patch(std::string id);
        static crow::json::wvalue create(std::string id);
        static bool remove(std::string id);
};

#endif
