# College Tour - CS1D Group Project

A full-stack application designed to help prospective college students plan efficient campus visits and purchase traditional souvenirs.

## Tech Stack

- **Backend**: C++ with Crow HTTP Framework
- **Frontend**: React/TypeScript with Vite
- **Database**: SQLite (persistent storage)
- **Build System**: CMake

## Quick Start

### Build & Run

```bash
# Clone the repository
git clone https://github.com/blakeallard/CS1D---Project-1-College-Tour.git
cd CS1D---Project-1-College-Tour

# Build backend
cmake . -B build
cmake --build build

# Build frontend
cd frontend && npm install && npm run build && cd ..

# Run the application
./build/execBinary
```

Access the application at `http://localhost:18080`

## Features

### Tour Planning
- **Saddleback Tour**: Start at Saddleback College, visit N nearest campuses
- **ASU Tour**: Start at Arizona State University, visit N nearest campuses
- **UCI Tour**: Start at UC Irvine, visit N nearest campuses
- **Custom Tour**: Select your own starting campus and destinations

### Souvenir System
- Browse souvenirs at each campus
- Purchase multiple items with quantity tracking
- View spending per campus and grand total
- Persistent purchase history

### Administrator Features (Password Protected)
- Import new campuses from database files
- Modify souvenir prices
- Add/delete souvenirs

## Project Documentation

| Document | Description |
|----------|-------------|
| [Data Structures](docs/DATA_STRUCTURES.md) | All data structures used with descriptions |
| [Big-O Analysis](docs/BIG_O_ANALYSIS.md) | Time complexity analysis for key algorithms |
| [Program Flow](docs/PROGRAM_FLOW.md) | System architecture and data flow diagrams |

## Project Structure

```
CS1D---Project-1-College-Tour/
├── src/
│   ├── main.cpp                 # Application entry point
│   ├── RouteRegister.h          # API route definitions
│   ├── Controllers/             # REST API controllers
│   │   ├── headers/
│   │   └── src/
│   └── Database/                # Database operations & algorithms
│       ├── headers/
│       └── src/
├── frontend/                    # React frontend
│   └── src/
│       ├── pages/               # Page components
│       └── components/          # Reusable components
├── Databases/                   # SQLite database files
│   ├── distances.db             # Campus distances
│   ├── souvenirs.db             # Souvenir data
│   └── users.db                 # User/admin data
├── vendor/                      # Third-party libraries
│   ├── crow/                    # Crow HTTP framework
│   └── sqlite3/                 # SQLite library
└── docs/                        # Project documentation
```

## API Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET | `/api/University/all` | Get all campuses with distances |
| GET | `/api/University/from-{campus}` | Get campuses reachable from start |
| GET | `/api/Souvenir/{campus}` | Get souvenirs for a campus |
| POST | `/api/Tour/calculate` | Calculate custom tour route |
| POST | `/api/Tour/calculate-n` | Calculate N-nearest campus tour |
| POST | `/api/Purchase/save` | Save purchase records |
| POST | `/api/User/admin` | Admin authentication |

## Team

CS1D Data Structures & Algorithms - Spring 2026

## License

Educational project - Saddleback College
