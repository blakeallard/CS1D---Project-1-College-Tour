# Quick Start Guide

## Get Your Application Running in 3 Steps

### Step 1: Build the Backend
```bash
# From project root directory
cmake . -B build
cmake --build build
```

### Step 2: Build the Frontend
```bash
cd frontend
npm install
npm run build
cd ..
```

### Step 3: Run the Server
```bash
./build/execBinary
```

That's it!

---

## Access Your Application

Open your browser and navigate to:
```
http://localhost:18080
```

---

## Test the Features

### 1. Tour Planning
- Click "Tours" from home page
- Try each tour type:
  - **Saddleback** - Auto-loads optimal route
  - **ASU** - Enter number of campuses (try 5)
  - **UCI** - Shows full 13-campus tour
  - **Custom** - Select your own campuses

### 2. View Campus Information
- Click "Campuses" from home page
- Click any campus name to see souvenirs
- Check prices for each item

### 3. Admin Features
- Click "Admin" from home page
- **Login with:**
  - Username: `admin`
  - Password: `admin123`
- Try these operations:
  - **Add Souvenir**: Select campus, enter name and price
  - **Edit Souvenir**: Select campus, click Edit, change price
  - **Delete Souvenir**: Select campus, click Delete, confirm

---

## Quick Test APIs

### Test Tour Endpoint
```bash
curl http://localhost:18080/api/Tour/saddleback
```

### Test Admin Login
```bash
curl "http://localhost:18080/api/Admin/login?username=admin&password=admin123"
```

### Test Campus List
```bash
curl http://localhost:18080/api/University/all
```

---

## Development Mode

If you want to modify the frontend and see live changes:

### Terminal 1 (Backend):
```bash
./build/execBinary
```

### Terminal 2 (Frontend Dev Server):
```bash
cd frontend
npm run dev
```
Frontend dev server runs on http://localhost:5173

---

## Troubleshooting

### Backend won't compile?
- Check that you have CMake installed: `cmake --version`
- Check for C++17 support: `g++ --version` or `clang++ --version`
- On MSVC, ignore warnings (they're expected)

### Port 18080 already in use?
- Stop any existing instances: `killall execBinary`
- Or change port in `src/main.cpp` line 39

### Frontend won't build?
- Check Node version: `node --version` (need 16+)
- Delete `node_modules` and reinstall: `rm -rf node_modules && npm install`

### Database errors?
- Ensure databases are in `Databases/` folder
- Check file permissions: `ls -l Databases/`

### API calls failing?
- Open browser console (F12) to see errors
- Check that backend is running
- Verify URL is http://localhost:18080

---

## File Structure Overview

```
CS1D---Project-1-College-Tour/
├── build/                    # Build output (generated)
│   └── execBinary           # Compiled server executable
├── Databases/               # SQLite databases
│   ├── distances.db
│   ├── souvenirs.db
│   ├── users.db
│   └── new_campuses.db
├── frontend/
│   ├── dist/               # Built frontend (generated)
│   ├── src/                # React source code
│   └── package.json
├── src/
│   ├── Controllers/        # API controllers
│   ├── Database/          # Database & tour logic
│   └── main.cpp           # Server entry point
└── CMakeLists.txt         # Build configuration
```

---

## What's Working

**Tour Planning**
- All 4 tour types functional
- Optimal route calculation
- Distance tracking

**Admin System**
- Login authentication
- Add/Edit/Delete souvenirs
- Session management

**Campus Info**
- View all campuses
- See souvenirs and prices
- Distance from Saddleback

**UI/UX**
- Loading states
- Error handling
- Success messages
- Responsive design

---

## Next Steps

1. **Try all features** - Tour planning, campus info, admin operations
2. **Check the documentation** - See BACKEND_INTEGRATION.md for details
3. **Test the APIs** - Use curl commands above
4. **Customize** - Add your own campuses/souvenirs via admin panel

---

## Need Help?

1. Check `IMPLEMENTATION_SUMMARY.md` - High-level overview
2. Check `BACKEND_INTEGRATION.md` - Detailed technical docs
3. Check browser console - Frontend errors
4. Check terminal output - Backend errors

---

## Admin Credentials (Don't Forget!)

```
Username: admin
Password: admin123
```

---

**Enjoy your fully integrated College Tour application!**
