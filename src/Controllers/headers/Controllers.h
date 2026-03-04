#ifndef CONTROLLERS_H
#define CONTROLLERS_H
#include <crow.h>

struct UniversityController
{
        static crow::response read(std::string id);
        static crow::response patch(const crow::request &req, std::string id);
        static crow::response create(const crow::request &req, std::string id);
        static crow::response remove(const crow::request &req, std::string id);
};

struct UserController
{
        static crow::response read(std::string id);
        static crow::response patch(const crow::request &req, std::string id);
        static crow::response create(const crow::request &req, std::string id);
        static crow::response remove(const crow::request &req, std::string id);
};

struct AdminController
{
        static crow::response read(std::string id);
        static crow::response patch(const crow::request &req, std::string id);
        static crow::response create(const crow::request &req, std::string id);
        static crow::response remove(const crow::request &req, std::string id);
};

struct TourController
{
        static crow::response read(std::string id);
        static crow::response patch(const crow::request &req, std::string id);
        static crow::response create(const crow::request &req, std::string id);
        static crow::response remove(const crow::request &req, std::string id);
};

struct SouvenirController
{
        static crow::response read(std::string id);
        static crow::response patch(const crow::request &req, std::string id);
        static crow::response create(const crow::request &req, std::string id);
        static crow::response remove(const crow::request &req, std::string id);
};

#endif
