
## Smart City Smart Bin Dashboard

Object-oriented programming project demonstrating a smart waste-bin monitoring system with:

- C++ backend (OOP, in-memory data, simple HTTP server)
- HTML/CSS/JS frontend (dashboard UI, glassmorphism, city map, bin icons)
- Features: view bins, add bins, simulate fill-up, sort waste, and generate optimized routes

***

## Project details (college submission)

This is an Object Oriented Programming (OOP) project submitted by:

- **Naman Tuslyan (24/CSE/298)**
- **Krishna Joshi (24/CSE/250)**  

to **Prof. Rohit Beniwal**, Department of Computer Science and Engineering.

***

## 1. Folder structure

Recommended structure:

```text
smart-bin-project/
├── backend/
│   ├── Bin.h
│   ├── Bin.cpp
│   └── main.cpp
└── frontend/
    └── index.html
```

You can also add:

```text
smart-bin-project/
├── README.md
```

Place this README content in `README.md` at the project root.

***

## 2. Backend overview (C++ OOP)

### 2.1. Technologies

- C++ (OOP style)
- Simple TCP socket–based HTTP server
- In-memory storage for bins (no database)
- Very basic JSON string building (no external JSON library)

### 2.2. Main OOP classes

- **`Bin` (in `Bin.h` / `Bin.cpp`)**
  - Attributes:
    - `id` (int)
    - `location` (string)
    - `type` (enum `WasteType`: PLASTIC, FOOD, METAL, MIXED)
    - `fillLevel` (double, 0–100)
    - `needsCollection` (bool, true if fill ≥ 80)
    - `recyclableAmount` (double)
    - `nonRecyclableAmount` (double)
  - Key methods:
    - `increaseFill(double amount)`
    - `sortWaste()` – calculates recyclable and non-recyclable portions
    - `updateStatus()` – decides if bin needs collection

- **`BinService` (inside `main.cpp`)**
  - Manages a list of `Bin` objects.
  - Responsibilities:
    - Add a new bin (`addBin`)
    - Find bin by ID (`findById`)
    - Return all bins (`getAll`)
    - Generate a simple optimized route (`generateRoute`) by listing bins needing collection in ID order.

### 2.3. API endpoints

The backend exposes a minimal HTTP API on port **8080**:

- `GET /bins`
  - Returns JSON array of all bins.
- `POST /bins`
  - Expects body in form-encoded format:  
    `location=...&type=...&fill=...`
  - Adds a new bin, returns the created bin in JSON.
- `POST /bins/{id}/fill`
  - Simulates waste being added (e.g., +10% fill).
- `POST /bins/{id}/sort`
  - Runs the “sorting” logic and updates recyclable/non-recyclable amounts.
- `GET /route`
  - Returns a simple “optimized” route: list of IDs of bins needing collection.

***

## 3. Frontend overview (HTML/CSS/JS)

### 3.1. Technologies

- HTML5
- CSS (glassmorphism, smart-city themed, responsive dashboard)
- Vanilla JavaScript with `fetch` for API calls

### 3.2. Key UI features

- **Smart City Dashboard theme**
  - Gradient background
  - Glassmorphism panels
  - City map background + mini-map section
  - Dustbin icons for each bin

- **Bins dashboard (left panel)**
  - Table of all bins with:
    - ID, location, type
    - Fill % with animated bar
    - Status badge (“OK” or “Needs Collection”)
  - Buttons per row:
    - `+ Fill` → calls `/bins/{id}/fill`
    - `Sort` → calls `/bins/{id}/sort`

- **Stats**
  - Total bins
  - Number of bins needing collection
  - Average fill level

- **Route generation**
  - “Generate Optimised Route” button
  - Shows sequence of bin IDs as recommended collection order

- **Control panel (right panel)**
  - Form to add new bins:
    - Location / landmark
    - Waste type
    - Initial fill level
  - Mini map showing “high priority cluster” explanation (visual enhancement for viva/demo)

***

## 4. How to run the project

### 4.1. Prerequisites

- A C++ compiler (e.g., `g++`)
- Linux or WSL recommended (code uses `<netinet/in.h>` and POSIX sockets)
- Any modern web browser (Chrome, Edge, Firefox, etc.)

### 4.2. Compile and run backend

1. Open a terminal and go to the backend folder:

   ```bash
   cd smart-bin-project/backend
   ```

2. Compile:

   ```bash
   g++ main.cpp Bin.cpp -o server
   ```

3. Run the server:

   ```bash
   ./server
   ```

4. You should see something like:

   ```text
   Server running on port 8080
   ```

   Keep this terminal window open while using the frontend.

> Notes:
> - If port 8080 is already in use, change `port` in `main.cpp` and also change the `API` constant in `index.html` accordingly.
> - On Windows without WSL, you may need to adjust the socket headers and calls.

### 4.3. Open frontend

1. In another window, go to the frontend folder:

   ```bash
   cd smart-bin-project/frontend
   ```

2. Open `index.html` directly by double-clicking it or using:

   - Right-click → “Open with browser”

   If the browser blocks requests due to CORS or `file://` origin, use Option B.

4. Make sure:

   - The C++ server is running on `http://localhost:8080`
   - The `API` constant in `index.html` is:

     ```js
     const API = "http://localhost:8080";
     ```

***

## 5. How to use the system

### 5.1. Initial state

- When the backend starts, it creates one sample bin:
  - Location: “Campus Gate”
  - Type: plastic
  - Fill level: 40%

### 5.2. View all smart bins

- Open the dashboard (frontend).
- The left panel table shows:
  - Each bin with dustbin icon, location, type, fill %, and status.

### 5.3. Add a new bin

1. Go to the right panel “Add / Simulate Bin”.
2. Enter:
   - **Location / landmark** (e.g., “Hostel 3”, “Library”)
   - **Waste type** (plastic / food / metal / mixed)
   - **Initial fill level** (0–100)
3. Click **“Add Smart Bin”**.
4. The table on the left updates automatically.

### 5.4. Simulate bin fill-up

- In the table row for a bin, click **“+ Fill”**.
- Backend increases the fill level (e.g., +10%).
- When fill ≥ 80%, the bin’s status changes to **“Needs Collection”** and the stats update.

### 5.5. Sort waste

- In a bin row, click **“Sort”**.
- Backend calculates recyclable vs non-recyclable amounts based on waste type.
- A popup shows the values for that bin.

### 5.6. Generate optimized collection route

1. Click **“Generate Optimised Route”**.
2. The route area updates with an icon and a sequence of IDs (for bins needing collection).
3. This can be explained as a simple, ID-based approximation of an optimized path (for demo/OOP level).

***

## 6. OOP concepts demonstrated

This project is explicitly designed as an OOP assignment:

- **Encapsulation**
  - `Bin` class encapsulates fields and behavior related to a bin.
  - Access through getters/setters and methods like `increaseFill`, `sortWaste`, `updateStatus`.

- **Abstraction**
  - `BinService` abstracts management of multiple bins (add, find, generate route) from the HTTP handling code.

- **Modularity**
  - `Bin.h` / `Bin.cpp` separate class definition and implementation.
  - HTTP server logic in `main.cpp` interacts with `BinService` rather than raw data structures.

You can also add UML class diagrams and sequence diagrams in your report to strengthen the OOP explanation.

***

## 8. Credits

- Project authors:
  - **Naman Tuslyan (24/CSE/298)**
  - **Krishna Joshi (24/CSE/250)**
- Submitted to:
  - **Prof. Rohit Beniwal**, Department of Computer Science and Engineering.
- Icons and map illustrations:
  - City maps and dustbin icons can be sourced from free icon/vector platforms (e.g., Flaticon, Freepik, Vecteezy, amCharts free maps) with appropriate attribution and local copies added to the project directory.