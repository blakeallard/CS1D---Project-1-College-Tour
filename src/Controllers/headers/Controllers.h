/**
 * @file Controllers.h
 * @brief REST API controllers for the College Tour application
 * 
 * This file defines controller structs that handle HTTP requests
 * for different API endpoints. Each controller follows CRUD pattern
 * with read, create, patch, and remove operations.
 * 
 * Controllers use the Crow HTTP framework for request/response handling.
 * 
 * @author CS1D Group Project
 * @date March 2026
 */

#ifndef CONTROLLERS_H
#define CONTROLLERS_H
#include <crow.h>

/**
 * @struct UniversityController
 * @brief Handles university/campus-related API endpoints
 * 
 * Endpoints:
 * - GET /api/University/all - List all campuses with distances from Saddleback
 * - GET /api/University/from-{campus} - List campuses reachable from specified campus
 * - GET /api/University/{campus} - Get souvenirs for a specific campus
 * - POST /api/University/import - Import new campus data
 */
struct UniversityController
{
    /**
     * @brief Handles GET requests for university data
     * @param id Endpoint identifier ("all", "from-{campus}", or campus name)
     * @return JSON response with campus or souvenir data
     */
    static crow::response read(std::string id);
    
    /**
     * @brief Handles PATCH requests for updating university data
     * @param req The HTTP request object
     * @param id Identifier for the resource to update
     * @return Response indicating success or failure
     */
    static crow::response patch(const crow::request &req, std::string id);
    
    /**
     * @brief Handles POST requests for creating/importing university data
     * @param req The HTTP request object containing data to create
     * @param id Endpoint identifier (e.g., "import")
     * @return Response indicating success or failure
     */
    static crow::response create(const crow::request &req, std::string id);
    
    /**
     * @brief Handles DELETE requests for removing university data
     * @param req The HTTP request object
     * @param id Identifier for the resource to delete
     * @return Response indicating success or failure
     */
    static crow::response remove(const crow::request &req, std::string id);
};

/**
 * @struct UserController
 * @brief Handles user account-related API endpoints
 * 
 * Manages user registration, authentication, and profile operations.
 */
struct UserController
{
    /**
     * @brief Handles GET requests for user data
     * @param id User identifier or action
     * @return JSON response with user data
     */
    static crow::response read(std::string id);
    
    /**
     * @brief Handles PATCH requests for updating user data
     * @param req The HTTP request object
     * @param id User identifier
     * @return Response indicating success or failure
     */
    static crow::response patch(const crow::request &req, std::string id);
    
    /**
     * @brief Handles POST requests for user creation/actions
     * @param req The HTTP request object
     * @param id Action identifier
     * @return Response indicating success or failure
     */
    static crow::response create(const crow::request &req, std::string id);
    
    /**
     * @brief Handles DELETE requests for user removal
     * @param req The HTTP request object
     * @param id User identifier
     * @return Response indicating success or failure
     */
    static crow::response remove(const crow::request &req, std::string id);
};

/**
 * @struct AdminController
 * @brief Handles administrator API endpoints
 * 
 * Provides password-protected administrative functions:
 * - Login/authentication
 * - Campus management
 * - Souvenir price modifications
 * - Data import operations
 */
struct AdminController
{
    /**
     * @brief Handles GET requests for admin operations
     * @param id Action identifier (e.g., "login")
     * @return JSON response with authentication result or admin data
     */
    static crow::response read(std::string id);
    
    /**
     * @brief Handles PATCH requests for admin updates
     * @param req The HTTP request object
     * @param id Resource identifier
     * @return Response indicating success or failure
     */
    static crow::response patch(const crow::request &req, std::string id);
    
    /**
     * @brief Handles POST requests for admin actions
     * @param req The HTTP request object
     * @param id Action identifier
     * @return Response indicating success or failure
     */
    static crow::response create(const crow::request &req, std::string id);
    
    /**
     * @brief Handles DELETE requests for admin deletions
     * @param req The HTTP request object
     * @param id Resource identifier
     * @return Response indicating success or failure
     */
    static crow::response remove(const crow::request &req, std::string id);
};

/**
 * @struct TourController
 * @brief Handles tour planning API endpoints
 * 
 * Endpoints:
 * - POST /api/Tour/calculate - Calculate optimal tour with selected campuses
 * - POST /api/Tour/calculate-n - Calculate tour visiting N nearest campuses
 * 
 * Uses TourPlanner class for route optimization algorithms.
 */
struct TourController
{
    /**
     * @brief Handles GET requests for tour data
     * @param id Tour type or identifier
     * @return JSON response with tour information
     */
    static crow::response read(std::string id);
    
    /**
     * @brief Handles PATCH requests for tour updates
     * @param req The HTTP request object
     * @param id Tour identifier
     * @return Response indicating success or failure
     */
    static crow::response patch(const crow::request &req, std::string id);
    
    /**
     * @brief Handles POST requests for tour calculations
     * 
     * Supported endpoints:
     * - "calculate": Calculate tour with user-selected campuses
     *   Body: { startCampus: string, selectedCampuses: string[] }
     * - "calculate-n": Calculate tour visiting N nearest campuses
     *   Body: { startCampus: string, numberOfCampuses: int }
     * 
     * @param req The HTTP request object containing tour parameters
     * @param id Endpoint identifier ("calculate" or "calculate-n")
     * @return JSON response with optimal route and total distance
     * 
     * @see TourPlanner::calculateOptimalTour
     */
    static crow::response create(const crow::request &req, std::string id);
    
    /**
     * @brief Handles DELETE requests for tour removal
     * @param req The HTTP request object
     * @param id Tour identifier
     * @return Response indicating success or failure
     */
    static crow::response remove(const crow::request &req, std::string id);
};

/**
 * @struct SouvenirController
 * @brief Handles souvenir-related API endpoints
 * 
 * Endpoints:
 * - GET /api/Souvenir/{campus} - Get souvenirs for a campus
 * - POST /api/Souvenir/add - Add new souvenir (admin)
 * - PATCH /api/Souvenir/update - Update souvenir price (admin)
 * - DELETE /api/Souvenir/delete - Delete souvenir (admin)
 */
struct SouvenirController
{
    /**
     * @brief Handles GET requests for souvenir data
     * @param id Campus name to get souvenirs for
     * @return JSON array of souvenirs with names and prices
     */
    static crow::response read(std::string id);
    
    /**
     * @brief Handles PATCH requests for souvenir updates
     * @param req The HTTP request object with update data
     * @param id Souvenir or action identifier
     * @return Response indicating success or failure
     */
    static crow::response patch(const crow::request &req, std::string id);
    
    /**
     * @brief Handles POST requests for adding souvenirs
     * @param req The HTTP request object with souvenir data
     * @param id Action identifier
     * @return Response indicating success or failure
     */
    static crow::response create(const crow::request &req, std::string id);
    
    /**
     * @brief Handles DELETE requests for souvenir removal
     * @param req The HTTP request object
     * @param id Souvenir identifier
     * @return Response indicating success or failure
     */
    static crow::response remove(const crow::request &req, std::string id);
};

/**
 * @struct PurchaseController
 * @brief Handles souvenir purchase API endpoints
 * 
 * Endpoints:
 * - POST /api/Purchase/save - Save purchase records to database
 * - GET /api/Purchase/history - Get purchase history for a user
 * 
 * Tracks souvenirs purchased at each campus during tours.
 */
struct PurchaseController
{
    /**
     * @brief Handles GET requests for purchase history
     * @param id User identifier or action
     * @return JSON response with purchase records
     */
    static crow::response read(std::string id);
    
    /**
     * @brief Handles PATCH requests for purchase updates
     * @param req The HTTP request object
     * @param id Purchase identifier
     * @return Response indicating success or failure
     */
    static crow::response patch(const crow::request &req, std::string id);
    
    /**
     * @brief Handles POST requests for saving purchases
     * 
     * Body format:
     * {
     *   tourName: string,
     *   purchases: [{campus, item, price, quantity}, ...]
     * }
     * 
     * @param req The HTTP request object with purchase data
     * @param id Action identifier (e.g., "save")
     * @return Response indicating success or failure
     */
    static crow::response create(const crow::request &req, std::string id);
    
    /**
     * @brief Handles DELETE requests for purchase removal
     * @param req The HTTP request object
     * @param id Purchase identifier
     * @return Response indicating success or failure
     */
    static crow::response remove(const crow::request &req, std::string id);
};

#endif
