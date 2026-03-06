# CRITICAL FIXES DOCUMENTATION

## Issue 1: Custom Tour Displays Nothing

### DIAGNOSIS
**Status:** Awaiting user testing with diagnostic logging

**Root Cause:** Unknown until logs are examined

**What Was Done:**
- Added comprehensive logging throughout data flow:
  - Frontend: Console logs at fetch, state update, and render
  - Backend: Logs showing campus count and list returned
  
**Expected Logs:**
```
Browser Console:
[CustomTour] Fetching campuses from /api/Tour/campuses
[CustomTour] Response received: {campuses: Array(15)}
[CustomTour] State updated with 15 campuses

Terminal:
[TourController] GET /campuses - Found 15 campuses
```

**If campuses don't appear:**
- Check if array is empty (database issue)
- Check if state update fails (React issue)
- Check if CSS makes elements invisible

---

## Issue 2: Admin Login Credentials Not Working

### ROOT CAUSE IDENTIFIED

**Problem:** Request body was not being captured by Crow routes

**Evidence:**
```
[AdminController] POST request received
[AdminController] Body: login        <-- This is the URL parameter, NOT request body!
[AdminController] ERROR: Invalid JSON received
```

### WHY THIS HAPPENED

**Broken Code in `src/RouteRegister.h`:**

```cpp
// BEFORE (BROKEN):
app.route_dynamic("/api/" + basePath + "<string>")
    .methods(crow::HTTPMethod::POST)(
        [](std::string id)  // <-- Only captures URL parameter
        {
            std::string url = decodeURL(id);
            return Controller::create(id);  // <-- Passes "login" not JSON body
        });
```

**Request Flow:**
1. Frontend sends: `POST /api/Admin/login` with body `{"username":"admin","password":"admin123"}`
2. Crow captures URL parameter: `<string>` = "login"
3. Lambda function signature `[](std::string id)` only has URL parameter
4. `req.body` (the actual JSON) is never accessed
5. Controller receives "login" string instead of JSON
6. JSON parser fails
7. Login fails

### THE FIX

**Updated Code:**

```cpp
// AFTER (FIXED):
app.route_dynamic("/api/" + basePath + "<string>")
    .methods(crow::HTTPMethod::POST)(
        [](const crow::request& req, std::string id)  // <-- Added req parameter
        {
            // Pass request body containing JSON data
            return Controller::create(req.body);  // <-- Now passes actual JSON
        });
```

**What Changed:**
1. Added `const crow::request& req` to lambda signature
2. Changed `Controller::create(id)` to `Controller::create(req.body)`
3. Now passes actual POST body instead of URL parameter

**Same Fix Applied to PATCH:**

```cpp
app.route_dynamic("/api/" + basePath + "<string>")
    .methods(crow::HTTPMethod::PATCH)(
        [](const crow::request& req, std::string id)  // <-- Added req
        {
            return Controller::patch(req.body);  // <-- Pass body not URL
        });
```

### WHY THIS FIX WORKS

**Crow Framework Request Handling:**
- `const crow::request& req` gives access to full HTTP request
- `req.body` contains the raw POST/PATCH body
- URL parameters are separate from body content

**Before:**
```
Frontend: {"username":"admin","password":"admin123"}
Backend receives: "login" (URL parameter)
Result: JSON parse fails
```

**After:**
```
Frontend: {"username":"admin","password":"admin123"}
Backend receives: {"username":"admin","password":"admin123"} (request body)
Result: JSON parses successfully, credentials match
```

### VERIFICATION

After rebuild, terminal should show:
```
[AdminController] POST request received
[AdminController] Body: {"username":"admin","password":"admin123"}
[AdminController] JSON parsed successfully
[AdminController] Has username: 1
[AdminController] Has password: 1
[AdminController] Username: 'admin'
[AdminController] Password: 'admin123'
[AdminController] Login SUCCESS
```

---

## REBUILD INSTRUCTIONS

```bash
# Rebuild backend (RouteRegister.h changed)
cmake --build build

# Frontend doesn't need rebuild (no changes)

# Restart server
killall execBinary
./build/execBinary
```

---

## TEST INSTRUCTIONS

### Test Admin Login:
1. Go to http://localhost:18080
2. Click "Admin"
3. Enter: admin / admin123
4. Click "Login"
5. **Expected:** Should authenticate and show admin dashboard
6. **Check terminal:** Should show `[AdminController] Login SUCCESS`

### Test Custom Tour:
1. Click "Tours" -> "Create a Custom Tour"
2. **Check console (F12):** Should see campus list loading
3. **Check terminal:** Should see `[TourController] GET /campuses`
4. Select some campuses
5. Click "Generate Custom Tour"
6. **Expected:** Tour route should display
7. **Check logs:** Full request/response flow visible

---

## IMPACT ANALYSIS

### What Was Broken:
- ALL POST requests to any controller (Admin, Tour)
- ALL PATCH requests (souvenir updates)
- Request bodies were being ignored
- URL parameters were passed instead

### What Is Now Fixed:
- POST /api/Admin/login - receives JSON body correctly
- POST /api/Admin/souvenir - receives souvenir data correctly
- POST /api/Tour/custom - receives tour parameters correctly
- PATCH /api/Admin/souvenir - receives update data correctly

### What Still Works:
- GET requests (unchanged, don't need body)
- DELETE requests (uses URL parameters, unchanged)

---

## TECHNICAL DETAILS

### Crow Framework Lambda Signatures:

**For GET/DELETE (URL only):**
```cpp
[](std::string id) { ... }
```

**For POST/PATCH (need body):**
```cpp
[](const crow::request& req, std::string id) { ... }
```

### Key Crow API:
- `req.body` - raw request body as string
- `req.url_params` - query parameters
- `crow::json::load(req.body)` - parse JSON from body

---

## LESSONS LEARNED

1. **Always verify request body access** in REST frameworks
2. **Lambda signatures matter** - different signatures capture different data
3. **Diagnostic logging is essential** - revealed exact failure point immediately
4. **URL parameters ≠ request body** - they're separate HTTP components

---

## STATUS

- Issue 1 (Custom Tour): DIAGNOSTIC LOGGING ADDED - awaiting user test results
- Issue 2 (Admin Login): ROOT CAUSE IDENTIFIED AND FIXED

**Ready for testing after rebuild.**
