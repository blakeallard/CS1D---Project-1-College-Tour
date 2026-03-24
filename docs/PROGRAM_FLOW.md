# Program Flow Documentation

This document describes the system architecture and data flow of the College Tour application.

---

## System Architecture

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                              CLIENT (Browser)                                │
│  ┌─────────────────────────────────────────────────────────────────────┐    │
│  │                         React Frontend                               │    │
│  │  ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌──────────┐ ┌──────────┐  │    │
│  │  │  Home    │ │ Campuses │ │  Tours   │ │ Souvenir │ │  Admin   │  │    │
│  │  │  Page    │ │  Page    │ │  Pages   │ │ Purchase │ │  Panel   │  │    │
│  │  └──────────┘ └──────────┘ └──────────┘ └──────────┘ └──────────┘  │    │
│  └─────────────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────────┘
                                      │
                                      │ HTTP Requests (axios)
                                      ▼
┌─────────────────────────────────────────────────────────────────────────────┐
│                           SERVER (C++ / Crow)                                │
│  ┌─────────────────────────────────────────────────────────────────────┐    │
│  │                         REST API Layer                               │    │
│  │  ┌──────────────┐ ┌──────────────┐ ┌──────────────┐ ┌────────────┐ │    │
│  │  │ University   │ │    Tour      │ │  Souvenir    │ │  Purchase  │ │    │
│  │  │ Controller   │ │  Controller  │ │  Controller  │ │ Controller │ │    │
│  │  └──────────────┘ └──────────────┘ └──────────────┘ └────────────┘ │    │
│  └─────────────────────────────────────────────────────────────────────┘    │
│                                      │                                       │
│  ┌─────────────────────────────────────────────────────────────────────┐    │
│  │                        Business Logic Layer                          │    │
│  │  ┌──────────────────────┐    ┌──────────────────────┐               │    │
│  │  │     TourPlanner      │    │      QueryData       │               │    │
│  │  │  - visit()           │    │  - selectRows()      │               │    │
│  │  │  - twoOpt()          │    │  - insertRow()       │               │    │
│  │  │  - calculateTour()   │    │  - updateRows()      │               │    │
│  │  └──────────────────────┘    └──────────────────────┘               │    │
│  └─────────────────────────────────────────────────────────────────────┘    │
│                                      │                                       │
│  ┌─────────────────────────────────────────────────────────────────────┐    │
│  │                          Data Layer                                  │    │
│  │  ┌──────────────┐ ┌──────────────┐ ┌──────────────┐                 │    │
│  │  │ distances.db │ │ souvenirs.db │ │   users.db   │                 │    │
│  │  │  - colleges  │ │  - souvenirs │ │   - users    │                 │    │
│  │  │  - distances │ │              │ │  - purchases │                 │    │
│  │  └──────────────┘ └──────────────┘ └──────────────┘                 │    │
│  └─────────────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## Tour Calculation Flow

### Sequence Diagram: N-Nearest Campus Tour

```
┌──────────┐     ┌──────────────┐     ┌────────────────┐     ┌─────────────┐
│ Frontend │     │TourController│     │  TourPlanner   │     │  Database   │
└────┬─────┘     └──────┬───────┘     └───────┬────────┘     └──────┬──────┘
     │                  │                     │                     │
     │ POST /api/Tour/  │                     │                     │
     │ calculate-n      │                     │                     │
     │ {startCampus,    │                     │                     │
     │  numberOfCampuses}                     │                     │
     │─────────────────>│                     │                     │
     │                  │                     │                     │
     │                  │ Query distances     │                     │
     │                  │ from startCampus    │                     │
     │                  │─────────────────────────────────────────>│
     │                  │                     │                     │
     │                  │<─────────────────────────────────────────│
     │                  │ Return distances    │                     │
     │                  │                     │                     │
     │                  │ Sort by distance    │                     │
     │                  │ Select N-1 nearest  │                     │
     │                  │                     │                     │
     │                  │ calculateOptimalTour│                     │
     │                  │ (startCampus,       │                     │
     │                  │  selectedCampuses)  │                     │
     │                  │────────────────────>│                     │
     │                  │                     │                     │
     │                  │                     │ getAllDistances()   │
     │                  │                     │────────────────────>│
     │                  │                     │<────────────────────│
     │                  │                     │                     │
     │                  │                     │ Build distance      │
     │                  │                     │ matrix (n x n)      │
     │                  │                     │                     │
     │                  │                     │ visit() - greedy    │
     │                  │                     │ nearest-neighbor    │
     │                  │                     │                     │
     │                  │                     │ twoOpt() - local    │
     │                  │                     │ optimization        │
     │                  │                     │                     │
     │                  │<────────────────────│                     │
     │                  │ TourResult          │                     │
     │                  │ {stops, totalDist}  │                     │
     │                  │                     │                     │
     │<─────────────────│                     │                     │
     │ JSON Response    │                     │                     │
     │ {route, distance}│                     │                     │
     │                  │                     │                     │
```

---

## Greedy Nearest-Neighbor Algorithm Flow

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                         visit() Algorithm Flow                               │
└─────────────────────────────────────────────────────────────────────────────┘

                              ┌─────────────┐
                              │    Start    │
                              │  (campus 0) │
                              └──────┬──────┘
                                     │
                                     ▼
                         ┌───────────────────────┐
                         │ Add current campus    │
                         │ to route              │
                         │ Mark as visited       │
                         └───────────┬───────────┘
                                     │
                                     ▼
                         ┌───────────────────────┐
                         │ Build min-heap of     │
                         │ distances to all      │
                         │ unvisited campuses    │
                         └───────────┬───────────┘
                                     │
                                     ▼
                           ┌─────────────────┐
                           │ Heap empty?     │
                           └────────┬────────┘
                                    │
                        ┌───────────┴───────────┐
                        │                       │
                       YES                      NO
                        │                       │
                        ▼                       ▼
               ┌─────────────────┐   ┌─────────────────────┐
               │ Return          │   │ Extract minimum     │
               │ (base case)     │   │ (nearest unvisited) │
               └─────────────────┘   └──────────┬──────────┘
                                                │
                                                ▼
                                     ┌─────────────────────┐
                                     │ Recursive call:     │
                                     │ visit(nearest)      │
                                     └─────────────────────┘


Example with 4 campuses:

    Step 1: Start at A           Step 2: Visit nearest (B)
    
         A ●──────● B                 A ●──────● B
          \      /                     \      /
           \    /                       \    /    ← Route: A → B
            \  /                         \  /
             ● C                          ● C
             │                            │
             ● D                          ● D

    Step 3: From B, visit C      Step 4: From C, visit D
    
         A ●──────● B                 A ●──────● B
          \      /                     \      /
           \    /                       \    /
            \  /   ← Route: A→B→C        \  /   ← Route: A→B→C→D
             ● C                          ● C
             │                            │
             ● D                          ● D
```

---

## 2-Opt Optimization Flow

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                         2-Opt Optimization                                   │
└─────────────────────────────────────────────────────────────────────────────┘

Before 2-Opt:                    After 2-Opt:
                                 
    A ────────── B                   A ────────── B
     \          │                     \            \
      \         │  Crossing            \            \  No crossing
       \        │  edges                \            \
        C ──────D                        C ──────────D


Algorithm:

    For each pair of edges (A-B) and (C-D):
    
        Current:  dist(A,B) + dist(C,D)
        Proposed: dist(A,C) + dist(B,D)
        
        If proposed < current:
            Reverse segment B...C
            Mark improved = true
    
    Repeat until no improvement found
```

---

## Data Flow: Souvenir Purchase

```
┌──────────┐                    ┌──────────────────┐                ┌──────────┐
│ Frontend │                    │ PurchaseController│                │ Database │
└────┬─────┘                    └────────┬─────────┘                └────┬─────┘
     │                                   │                               │
     │  User selects souvenirs           │                               │
     │  and quantities                   │                               │
     │                                   │                               │
     │  POST /api/Purchase/save          │                               │
     │  {                                │                               │
     │    tourName: "UCI Tour",          │                               │
     │    purchases: [                   │                               │
     │      {campus, item, price, qty},  │                               │
     │      ...                          │                               │
     │    ]                              │                               │
     │  }                                │                               │
     │──────────────────────────────────>│                               │
     │                                   │                               │
     │                                   │  INSERT INTO user_purchases   │
     │                                   │  for each item                │
     │                                   │──────────────────────────────>│
     │                                   │                               │
     │                                   │<──────────────────────────────│
     │                                   │  Success                      │
     │                                   │                               │
     │<──────────────────────────────────│                               │
     │  {success: true,                  │                               │
     │   message: "Saved"}               │                               │
     │                                   │                               │
     │  Store in localStorage            │                               │
     │  for session persistence          │                               │
     │                                   │                               │
```

---

## Database Schema

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                              distances.db                                    │
├─────────────────────────────────────────────────────────────────────────────┤
│  colleges                        │  distances                               │
│  ┌─────────────────────────┐     │  ┌────────────────────────────────────┐ │
│  │ college (TEXT, PK)      │     │  │ starting_college (TEXT)            │ │
│  └─────────────────────────┘     │  │ ending_college (TEXT)              │ │
│                                  │  │ distance (REAL)                    │ │
│                                  │  └────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                              souvenirs.db                                    │
├─────────────────────────────────────────────────────────────────────────────┤
│  souvenirs                                                                   │
│  ┌──────────────────────────────────────────────────────────────────────┐   │
│  │ college (TEXT)                                                        │   │
│  │ item (TEXT)                                                           │   │
│  │ price (REAL)                                                          │   │
│  └──────────────────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────────────────────┐
│                                users.db                                      │
├─────────────────────────────────────────────────────────────────────────────┤
│  users                           │  user_purchases                          │
│  ┌─────────────────────────┐     │  ┌────────────────────────────────────┐ │
│  │ username (TEXT)         │     │  │ id (INTEGER, PK)                   │ │
│  │ password (TEXT)         │     │  │ user_id (INTEGER)                  │ │
│  └─────────────────────────┘     │  │ campus (TEXT)                      │ │
│                                  │  │ item (TEXT)                        │ │
│                                  │  │ price (REAL)                       │ │
│                                  │  │ quantity (INTEGER)                 │ │
│                                  │  │ purchase_date (TEXT)               │ │
│                                  │  └────────────────────────────────────┘ │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## Frontend Component Hierarchy

```
App
├── Home
│   └── Navigation Links
│
├── Campuses (View Distances)
│   └── SouvenirInfo (modal)
│
├── Tours
│   ├── SaddlebackStart
│   │   ├── Tour Form (number input)
│   │   ├── Tour Result Display
│   │   └── Souvenir Purchase Section
│   │
│   ├── ASUStart
│   │   ├── Tour Form (number input)
│   │   ├── Tour Result Display
│   │   └── Souvenir Purchase Section
│   │
│   ├── UCIStart
│   │   ├── Tour Form (number input)
│   │   ├── Tour Result Display
│   │   └── Souvenir Purchase Section
│   │
│   └── CustomTour
│       ├── Starting Campus Dropdown
│       ├── Campus Checkboxes
│       ├── Tour Result Display
│       └── Souvenir Purchase Section
│
└── Admin (Password Protected)
    ├── Campus Import
    ├── Souvenir Management
    └── Price Modification
```
