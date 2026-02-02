# College Tour - CS Group Project

A C++ application designed to help prospective college students plan their college campus visits and purchase traditional souvenirs.

## Project Overview

This software manages data for at least 15 college campuses (starting with 11) and their traditional souvenirs (up to 7 per campus). The program allows users to plan efficient campus tours and purchase souvenirs while tracking expenses.

## Core Features

### 1. Display Information
- List all college campuses with distances from Saddleback College
- Display traditional souvenirs and prices for any selected campus

### 2. Trip Planning Options

#### Standard Tour (11 Campuses)
- Start at Saddleback College
- Visit all 11 campuses in most efficient order (recursive nearest-neighbor algorithm)
- Display total distance traveled

#### ASU Custom Tour
- Start at Arizona State University
- Specify number of campuses to visit
- Visit campuses using nearest-neighbor selection
- Purchase souvenirs at each stop
- Track spending per campus and grand total

#### Fully Custom Tour
- Select starting campus
- Choose which campuses to visit
- Automatic efficient route planning
- Purchase souvenirs with expense tracking

#### UCI Tour (13 Campuses)
- Start at University of California, Irvine
- Visit all 13 campuses
- Display total distance and enable souvenir purchases

### 3. Souvenir Purchase System
- Buy multiple souvenirs at each campus
- Track quantity purchased per campus
- Display spending per campus and grand total

### 4. Administrator Features (Password Protected)
- Add new campuses from input file
- Modify souvenir prices
- Add new souvenirs
- Delete existing souvenirs

## Technical Requirements

### Data Structures
- Must use multiple C++ data structures (minimum 3 for full credit)
- Support for at least 15 campuses
- Up to 7 souvenirs per campus
- **Persistent data storage** between executions

### UML Diagrams Required
- 3 Use Case diagrams
- 3 Activity diagrams
- 1 Class diagram

### Documentation
- 10+ Agile stories (description, tasks, test scenarios, story points)
- Big-Oh analysis for at least 3 methods
- Doxygen automated documentation
- Data structures identification

### Development Tools
- **Version Control:** GitHub (private team repository)
- **GUI Framework:** (Team choice)
- **Documentation:** Doxygen
- **Agile Management:** (e.g., Twilio or similar)
- **Optional:** Continuous Integration (3 points extra credit)

## Agile/Scrum Process

### Team Roles
- **Scrum Master:** Document all meetings
- **Product Owner:** Maintain backlog

### Required Artifacts
- Agile stories
- Scrum log
- Retrospective
- Coding standards
- Team rules

## Project Schedule

| Milestone | Date | Points |
|-----------|------|--------|
| Team Partner Submission | January 27 | -3 if late |
| First Checkpoint | February 16 | 3 |
| Second Checkpoint | February 25 | 3 |
| Final Checkpoint/Demo | March 11 | - |

**No late projects accepted**

## Grading Rubric

| Category | Points |
|----------|--------|
| Checkpoint 1 | 3 |
| Checkpoint 2 | 3 |
| Meet Requirements | 75 |
| Coding Style/Style Guide | 2 |
| Multiple C++ Data Structures | 3 (1 per structure) |
| User Interface | 2 |
| Adherence to Scrum/Team Rules | 5 |
| UML Diagrams | 3 |
| Doxygen Documentation | 1 |
| Big-Oh Analysis | 3 (1 per method) |
| **Total** | **100** |
| Continuous Integration (Extra Credit) | 3 |
| **Maximum** | **103** |

## Final Demonstration (20 minutes)

### Requirements
- All team members present
- Demonstrate all project requirements
- Show Doxygen and Agile management tool
- Each member identifies their contributions
- Submit teammate assessments via email

### Submission Artifacts
- Source code
- Agile stories
- Scrum log
- Retrospective
- Coding standards
- Team rules
- UML diagrams (class, use cases, activity)
- Data structures documentation
- Doxygen report
- Big-Oh analysis

## Quality Standards

- **Readable, user-friendly interface**
- **Robust error handling** for invalid input
- **Clean code** following team coding standards
- **Persistent data** across program executions

## Team Information

**Partners must be submitted by January 27th** (3-point penalty if late)

Note: Individual team members may receive different grades based on contributions.

---

**Due Date:** March 11th | **No late submissions accepted**
