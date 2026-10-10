# Electricity Management System

A C++-based Electricity Management System that reads simulated household
appliance data from a CSV file and performs energy consumption,
electricity cost, sorting, searching, category, budget, and rule-based
insight analysis.

## 1. Project Structure

``` text
ElectricityManagement/
├── main.cpp
└── electricity_management_data.csv
```

### `main.cpp`

Contains the complete application logic: - Appliance data model - CSV
loading - Energy and cost calculations - Searching - Sorting - Category
analysis - High-consumption analysis - Budget analysis - Smart energy
insights - Complete report generation - Interactive menu

### `electricity_management_data.csv`

Contains 100 simulated appliance records.

------------------------------------------------------------------------

## 2. Project Objective

The project simulates a household electricity management system without
requiring physical smart meters, sensors, or IoT devices.

It uses predefined appliance information to estimate: - Monthly
electricity consumption - Monthly electricity cost - Major
energy-consuming appliances - Category-wise consumption - Budget
status - Top energy consumers - Basic energy-saving recommendations

------------------------------------------------------------------------

## 3. Dataset Format

The CSV header is:

``` csv
id,name,category,power_watts,hours_per_day,priority,status
```

Example:

``` csv
1,AC_1,Cooling,1500,6,1,1
```

  Field             Meaning
  ----------------- ----------------------------------
  `id`              Unique appliance identifier
  `name`            Appliance name
  `category`        Functional appliance category
  `power_watts`     Rated power in watts
  `hours_per_day`   Estimated daily operating hours
  `priority`        Importance level from 1 to 4
  `status`          Simulated state: 0 = OFF, 1 = ON

### Priority values

    Value Meaning
  ------- -----------
        1 Very High
        2 High
        3 Medium
        4 Low

The dataset contains 100 appliances, with five records for each of these
groups:

-   AC
-   Fan
-   Refrigerator
-   Heater
-   TV
-   Light
-   Washing Machine
-   Microwave
-   Computer
-   Water Pump
-   Iron
-   Kettle
-   Air Purifier
-   Water Heater
-   Dishwasher
-   Printer
-   Vacuum Cleaner
-   Toaster
-   Router
-   EV Charger

------------------------------------------------------------------------

## 4. Important Dataset Assumption

The `status` field represents the current simulated appliance state, but
**the current monthly energy formula does not use status**.

The current interpretation is:

``` text
status       = current simulated state
hours_per_day = expected/scheduled daily usage
```

Monthly energy is calculated from the expected daily usage:

``` text
Energy = Power × Hours/Day × Billing Days / 1000
```

Therefore, an appliance with `status = 0` can still contribute to
estimated monthly energy.

A future real-time simulation can change this behavior so that an OFF
appliance consumes zero current runtime energy.

------------------------------------------------------------------------

## 5. System Settings

Because the project intentionally uses only one CSV file, system
settings are stored directly in `main.cpp`:

``` cpp
double tariff = 6.0;
double monthlyBudget = 2500;
int billingDays = 30;
```

Current configuration:

  Setting                      Value
  -------------------- -------------
  Electricity tariff     ₹6.00 / kWh
  Monthly budget               ₹2500
  Billing period             30 days

These values are not stored in the CSV.

------------------------------------------------------------------------

## 6. Core Calculation Logic

### Monthly Energy

The project uses:

``` text
Monthly Energy (kWh)
= Power (W) × Hours per Day × Billing Days / 1000
```

Example:

``` text
Power = 1500 W
Hours/day = 6
Days = 30

Energy = 1500 × 6 × 30 / 1000
       = 270 kWh
```

### Monthly Cost

``` text
Monthly Cost
= Monthly Energy × Tariff
```

For the previous example:

``` text
Cost = 270 × ₹6
     = ₹1620
```

------------------------------------------------------------------------

## 7. CSV Loading Logic

The program opens:

``` cpp
ifstream file("electricity_management_data.csv");
```

It skips the CSV header and reads each remaining line.

`stringstream` separates the comma-delimited fields.

The values are converted into an `Appliance` object and stored in:

``` cpp
vector<Appliance> appliances;
```

The program also handles malformed rows by skipping them and displaying
a warning.

The CSV must be in the program's working directory.

------------------------------------------------------------------------

## 8. Main Data Structure

The primary storage structure is:

``` cpp
vector<Appliance> appliances;
```

It stores all appliance objects and is used throughout the program for
traversal, calculations, searching, and sorting.

------------------------------------------------------------------------

## 9. DSA Concepts Used

### Vector

``` cpp
vector<Appliance>
```

Used to store all appliance records.

### Searching

The program searches for an appliance by exact name using a linear scan.

Complexity:

``` text
O(n)
```

### Sorting

`std::sort()` is used to sort appliances: - By power - By monthly energy

Typical complexity:

``` text
O(n log n)
```

### Map

Category analysis uses:

``` cpp
map<string, double>
```

to aggregate monthly energy by category.

### Traversal

Vector traversal is used for: - Total energy - Total cost - High-power
detection - High-usage detection - Budget analysis - Report generation

------------------------------------------------------------------------

## 10. Program Flow

``` text
Start
  |
  v
Open electricity_management_data.csv
  |
  v
Skip CSV Header
  |
  v
Read Appliance Records
  |
  v
Create Appliance Objects
  |
  v
Store in vector
  |
  v
Display Menu
  |
  +--> View Appliance Data
  |
  +--> Energy Consumption Analysis
  |
  +--> Search Appliance
  |
  +--> Sort by Power
  |
  +--> Sort by Energy
  |
  +--> High Consumption Analysis
  |
  +--> Budget Analysis
  |
  +--> Category Analysis
  |
  +--> Smart Energy Insights
  |
  +--> Generate Complete Report
  |
  v
Exit
```

------------------------------------------------------------------------

## 11. Main Menu

``` text
1.  View Appliance Data
2.  Energy Consumption Analysis
3.  Search Appliance
4.  Sort Appliances by Power
5.  Sort Appliances by Energy
6.  High Consumption Analysis
7.  Budget Analysis
8.  Category Analysis
9.  Smart Energy Insights
10. Generate Complete Report
0.  Exit
```

### Option 1 --- View Appliance Data

Displays: - ID - Name - Category - Power - Daily usage - Priority -
Status

### Option 2 --- Energy Consumption Analysis

Calculates: - Monthly energy for each appliance - Monthly cost for each
appliance - Total monthly energy - Total monthly cost

### Option 3 --- Search Appliance

Performs an exact name search.

Example:

``` text
AC_1
```

### Option 4 --- Sort by Power

Sorts from highest rated power to lowest rated power.

### Option 5 --- Sort by Energy

Calculates monthly energy first, then sorts from highest to lowest
consumption.

### Option 6 --- High Consumption Analysis

Identifies appliances contributing at least 10% of total monthly energy.

### Option 7 --- Budget Analysis

Compares estimated monthly cost against the ₹2500 budget.

### Option 8 --- Category Analysis

Uses a `map` to calculate total monthly energy for each category.

### Option 9 --- Smart Energy Insights

Reports: - Total energy - Total cost - Highest energy consumer -
Appliances with power \>= 1000 W - Appliances with usage \>= 6
hours/day - Rule-based recommendations

### Option 10 --- Generate Complete Report

Produces a consolidated report containing system settings, consumption,
budget status, and the top five energy consumers.

### Option 0 --- Exit

Terminates the application.

------------------------------------------------------------------------

## 12. High Consumption Analysis

For every appliance:

``` text
Percentage Contribution
= Appliance Monthly Energy / Total Monthly Energy × 100
```

An appliance is considered a major consumer when its contribution is:

``` text
>= 10%
```

------------------------------------------------------------------------

## 13. Budget Analysis

The system compares:

``` text
Estimated Monthly Cost
            vs
Monthly Budget
```

Possible results:

``` text
Within budget
Budget limit reached
Budget exceeded
```

If the budget is exceeded, the program reports the excess amount.

------------------------------------------------------------------------

## 14. Category Analysis

The program aggregates energy by category.

Conceptually:

``` text
Cooling       -> total cooling energy
Kitchen       -> total kitchen energy
Heating       -> total heating energy
Computing     -> total computing energy
...
```

The key operation is:

``` cpp
categoryEnergy[appliance.category] += energy;
```

This demonstrates use of a map for grouped analysis.

------------------------------------------------------------------------

## 15. Smart Energy Insights

The insights module uses fixed rules rather than machine learning.

### High-power rule

``` text
Power >= 1000 W
```

### High-usage rule

``` text
Hours per day >= 6
```

### Major-consumer rule

``` text
Energy contribution >= 10%
```

It then produces simple recommendations such as reducing unnecessary
runtime and monitoring high-power appliances.

------------------------------------------------------------------------

## 16. Complete Report

The report contains:

``` text
System Summary
    |
    +-- Number of appliances
    +-- Billing period
    +-- Tariff
    +-- Monthly budget
    |
Consumption
    |
    +-- Total energy
    +-- Estimated bill
    |
Budget Status
    |
    +-- Within budget / exceeded
    +-- Remaining / exceeded amount
    |
Top Consumers
    |
    +-- Top 5 appliances by monthly energy
```

The report sorts a copy of the appliance vector, so report generation
does not permanently change the main vector's order.

------------------------------------------------------------------------

## 17. Complexity Overview

Let `n` be the number of appliances.

  Operation                   Complexity
  --------------------------- ------------
  Load CSV                    O(n)
  Display appliances          O(n)
  Search by name              O(n)
  Energy calculation          O(n)
  High-consumption analysis   O(n)
  Budget analysis             O(n)
  Category aggregation        O(n log k)
  Sorting                     O(n log n)
  Top-consumer report         O(n log n)

Here `k` is the number of unique categories.

------------------------------------------------------------------------

## 18. Example Data-to-Output Flow

For:

``` csv
1,AC_1,Cooling,1500,6,1,1
```

The program reads:

``` text
Power       = 1500 W
Usage       = 6 hours/day
Days        = 30
Tariff      = ₹6/kWh
```

Calculates:

``` text
Monthly Energy
= 1500 × 6 × 30 / 1000
= 270 kWh
```

Then:

``` text
Monthly Cost
= 270 × 6
= ₹1620
```

That result contributes to: - Total energy - Total cost - Cooling
category - High-consumption analysis - Budget analysis - Smart
insights - Top-consumer ranking

------------------------------------------------------------------------

## 19. Compilation and Execution

Using GCC/G++:

``` bash
g++ main.cpp -o electricity
```

Linux/macOS:

``` bash
./electricity
```

Windows:

``` bash
electricity.exe
```

Make sure both files are accessible from the executable's working
directory:

``` text
main.cpp
electricity_management_data.csv
```

------------------------------------------------------------------------

## 20. Modifying the Dataset

New appliances can be added directly to the CSV.

Example:

``` csv
101,New_Fan,Cooling,100,8,3,1
```

The C++ code does not need to change just because another valid
appliance row is added.

The row must follow:

``` text
id,name,category,power_watts,hours_per_day,priority,status
```

------------------------------------------------------------------------

## 21. Modifying System Settings

Change these values in `main.cpp`:

``` cpp
double tariff = 6.0;
double monthlyBudget = 2500;
int billingDays = 30;
```

For example:

``` cpp
double tariff = 7.0;
double monthlyBudget = 3000;
int billingDays = 30;
```

------------------------------------------------------------------------

## 22. Limitations

This is a software simulation and not a real smart-meter system.

Current limitations:

1.  No real-time sensor data.
2.  No connection to physical appliances.
3.  No historical daily/monthly readings.
4.  Current `status` does not affect monthly energy calculations.
5.  Tariff is fixed in the C++ source.
6.  Budget is fixed in the C++ source.
7.  Insights are rule-based, not AI/ML predictions.
8.  The CSV parser assumes simple comma-separated values.
9.  The monthly bill is an estimate based on predefined power and usage
    assumptions.

------------------------------------------------------------------------

## 23. Possible Future Improvements

The current architecture can later be extended with:

-   Real-time ON/OFF simulation
-   Appliance scheduling
-   Queue-based scheduling
-   `priority_queue` for priority-based management
-   Historical consumption records
-   Daily usage logs
-   Dynamic peak/off-peak tariffs
-   What-if energy-saving analysis
-   Energy-saving optimization
-   GUI dashboard
-   Web interface
-   IoT/smart-meter integration

For example:

``` text
Current Version
CSV
  ↓
C++ DSA Engine
  ↓
Analysis
  ↓
Console Report
```

A future version could become:

``` text
Smart Home Simulator
        ↓
Real-time Appliance State
        ↓
C++ DSA Engine
        ↓
Scheduling + Priority Management
        ↓
Energy Optimization
        ↓
Dashboard / Reports
```

------------------------------------------------------------------------

## 24. Project Architecture

``` text
┌──────────────────────────────────────────────┐
│       electricity_management_data.csv       │
│                                              │
│  100 simulated appliance records             │
└──────────────────────┬───────────────────────┘
                       │
                       │ CSV Input
                       ▼
┌──────────────────────────────────────────────┐
│                  main.cpp                    │
│                                              │
│  CSV Loading                                 │
│       ↓                                      │
│  Appliance Objects                           │
│       ↓                                      │
│  vector<Appliance>                           │
│       ↓                                      │
│  Energy & Cost Calculation                   │
│       ↓                                      │
│  Searching / Sorting                         │
│       ↓                                      │
│  map-based Category Analysis                 │
│       ↓                                      │
│  Budget & High Consumption Analysis          │
│       ↓                                      │
│  Smart Insights                              │
│       ↓                                      │
│  Complete Report                             │
└──────────────────────┬───────────────────────┘
                       │
                       ▼
┌──────────────────────────────────────────────┐
│              Console Output                  │
└──────────────────────────────────────────────┘
```

------------------------------------------------------------------------

## 25. Project Explanation for Viva

A concise explanation is:

> This project is a C++-based Electricity Management System that
> simulates household appliance energy consumption. Appliance
> information is stored in a CSV file and loaded into a vector of
> appliance objects. The system calculates monthly electricity
> consumption using power rating, daily operating hours, and billing
> days, and then calculates estimated electricity cost using a fixed
> tariff. DSA concepts such as vectors, maps, searching, and sorting are
> used for appliance management and analysis. The system also provides
> category-wise analysis, budget monitoring, high-consumption detection,
> rule-based energy insights, and a complete report.

------------------------------------------------------------------------

## 26. Summary

The complete processing pipeline is:

``` text
CSV Dataset
    ↓
CSV Parsing
    ↓
Appliance Objects
    ↓
Vector Storage
    ↓
Energy Calculation
    ↓
Cost Calculation
    ↓
DSA Operations
    ├── Searching
    ├── Sorting
    └── Map-based Aggregation
    ↓
Analysis
    ├── High Consumption
    ├── Category Analysis
    ├── Budget Analysis
    └── Smart Insights
    ↓
Reports
```

The current project focuses on demonstrating **C++ file handling,
object-oriented modeling, STL data structures, algorithms, numerical
calculations, and rule-based analysis** using a realistic simulated
electricity-management problem.
