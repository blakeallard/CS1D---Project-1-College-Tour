# Fixes Summary

## Issue 1: Graduation Cap Image Not Showing on Home Page

**Problem:**
The graduation cap logo (`grad-cap-logo.svg`) was not displaying on the home page.

**Root Cause:**
The image was being imported with a relative path in React, but Vite requires assets in the `public` directory to be referenced with absolute paths starting from root.

**Solution:**
1. Created `frontend/public/` directory
2. Copied `grad-cap-logo.svg` to `frontend/public/`
3. Updated `Home.jsx` to reference the image as `/grad-cap-logo.svg`

**Files Changed:**
- `frontend/src/pages/Home/Home.jsx`

**Code Change:**
```jsx
// Before:
import gradCapLogo from '../../assets/grad-cap-logo.svg';
<img className="menu_logo" src={gradCapLogo} alt="graduation cap" />

// After:
<img className="menu_logo" src="/grad-cap-logo.svg" alt="graduation cap" />
```

---

## Issue 2: Remove new_campuses Database

**Problem:**
The application was using a `new_campuses.db` database in addition to the original `distances.db`, which was unnecessary and added complexity.

**Root Cause:**
The old `Database` class was attempting to query both databases for distance information.

**Solution:**
Removed all references to `new_campuses.db` and `new_campuses_db` throughout the codebase, ensuring only the original `distances.db` is used.

**Files Changed:**
1. `src/Database/headers/Queries.h` - Removed `NEW_DISTANCES` constant
2. `include/database.h` - Removed `new_campuses_db` member variable and parameter from `ConnectToDB()`
3. `src/database.cpp` - Removed all `new_campuses_db` references:
   - Updated `ConnectToDB()` to only accept 3 parameters (removed `newCampusesFile`)
   - Updated `CloseDB()` to remove `new_campuses_db` cleanup
   - Updated `CampusExists()` to only check `distances_db`
   - Updated `GetDistance()` to only query `distances_db`
   - Updated `GetAllDistances()` to only query `distances_db`

**Summary of Changes:**
- Removed 1 database constant definition
- Removed 1 member variable from `Database` class
- Removed 1 parameter from `ConnectToDB()` method signature
- Removed ~25 lines of redundant database querying code
- Simplified the distance lookup logic

---

## Testing

Both issues have been resolved:

1. **Graduation Cap Image**: Will now load correctly from `/public/grad-cap-logo.svg` when the frontend dev server is running
2. **Database Simplification**: Backend now only uses the original `distances.db` for all distance queries

To test:
```bash
# Terminal 1 - Backend
cd /Users/blakeallard/CS1D---Project-1-College-Tour
./build/execBinary

# Terminal 2 - Frontend
cd /Users/blakeallard/CS1D---Project-1-College-Tour/frontend
npm run dev
```

Visit `http://localhost:5173` and verify:
- Graduation cap logo appears on home page
- All tour features work correctly with only the original databases
