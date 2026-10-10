# Electricity Management System — Project Logic

## 1. Project Overview

The **Electricity Management System** is a C++ + DSA based software project that simulates household electricity consumption.

The system reads appliance information from a CSV file, stores the records in a vector, calculates estimated monthly energy consumption and electricity cost, performs analysis, and generates energy-saving insights.

### Project Files

```text
ElectricityManagement/
├── main.cpp
└── electricity_management_data.csv
```

---

# 2. Dataset Structure

The CSV file contains the following fields:

```text
id,name,category,power_watts,hours_per_day,priority,status
```

| Field | Meaning |
|---|---|
| `id` | Unique appliance identifier |
| `name` | Appliance name |
| `category` | Appliance category |
| `power_watts` | Rated power of appliance in Watts |
| `hours_per_day` | Expected/scheduled daily operating time |
| `priority` | Importance level of appliance |
| `status` | Current simulated ON/OFF state |

---

# 3. Power Rating

## Definition

**Power Rating** represents the electrical power consumed by an appliance while operating.

It is measured in **Watts (W)**.

Examples:

```text
Fan        → 75 W
TV         → 120 W
AC         → 1500 W
EV Charger → 3500 W
```

The project stores this value in:

```cpp
double power;
```

---

# 4. Daily Energy Consumption

Energy consumption depends on both the appliance's power rating and the amount of time it operates.

### Formula

```text
Daily Energy (kWh)
= Power (W) × Hours per Day / 1000
```

### Example

Suppose an appliance has:

```text
Power = 1500 W
Usage = 4 hours/day
```

Then:

```text
Daily Energy
= 1500 × 4 / 1000
= 6 kWh/day
```

The division by `1000` converts Watt-hours into kilowatt-hours.

---

# 5. Monthly Energy Consumption

The project estimates monthly electricity consumption using the appliance power, daily usage, and billing period.

### Formula

```text
Monthly Energy (kWh)
= Power (W) × Hours per Day × Billing Days / 1000
```

The current billing period is:

```text
Billing Days = 30
```

### Example

```text
Power       = 1500 W
Hours/Day   = 4
Billing Days = 30

Monthly Energy
= 1500 × 4 × 30 / 1000
= 180 kWh
```

This calculation is implemented by:

```cpp
calculateMonthlyEnergy(int days)
```

---

# 6. Electricity Tariff

## Definition

The **electricity tariff** is the assumed price of one unit of electricity.

In this project:

```text
Tariff = ₹6 per kWh
```

The tariff is stored in:

```cpp
double tariff = 6.0;
```

---

# 7. Monthly Electricity Cost

The estimated electricity cost is calculated from monthly energy consumption and the electricity tariff.

### Formula

```text
Monthly Cost
= Monthly Energy Consumption × Electricity Tariff
```

### Example

If:

```text
Monthly Energy = 180 kWh
Tariff = ₹6/kWh
```

Then:

```text
Monthly Cost
= 180 × 6
= ₹1080
```

The calculation is implemented by:

```cpp
calculateMonthlyCost(double tariff, int days)
```

---

# 8. Total Monthly Energy Consumption

The system calculates the total estimated energy consumption of all appliances.

### Formula

```text
Total Energy
= Energy of Appliance 1
+ Energy of Appliance 2
+ ...
+ Energy of Appliance N
```

In simplified form:

```text
Total Energy = Σ Appliance Monthly Energy
```

The result is displayed in:

```text
kWh/month
```

---

# 9. Total Monthly Electricity Cost

The system also calculates the estimated total electricity bill.

### Formula

```text
Total Cost
= Cost of Appliance 1
+ Cost of Appliance 2
+ ...
+ Cost of Appliance N
```

Or:

```text
Total Cost = Σ Appliance Monthly Cost
```

---

# 10. Energy Consumption Percentage

The project determines how much of the total electricity consumption is contributed by an individual appliance.

### Formula

```text
Consumption Percentage
= Appliance Energy / Total Energy × 100
```

### Example

Suppose:

```text
Appliance Energy = 100 kWh
Total Energy = 500 kWh
```

Then:

```text
Consumption Percentage
= 100 / 500 × 100
= 20%
```

The project uses this value to identify major energy consumers.

---

# 11. High-Consumption Appliance Detection

The system identifies appliances that make a significant contribution to total energy consumption.

### Rule

```text
If Consumption Percentage >= 10%
→ Major Energy Consumer
```

This is a **rule-based analysis**.

It does not use machine learning or predictive AI.

---

# 12. High-Power Appliance Detection

The system identifies appliances with relatively high power ratings.

### Rule

```text
If Power >= 1000 W
→ High-Power Appliance
```

Examples may include:

```text
AC
Water Heater
EV Charger
Heater
```

This is a predefined threshold used by the Smart Energy Insights module.

---

# 13. High-Usage Appliance Detection

The system also identifies appliances that operate for a relatively long period each day.

### Rule

```text
If Hours per Day >= 6
→ High-Usage Appliance
```

This helps identify appliances whose operating duration may significantly affect energy consumption.

---

# 14. Budget Analysis

The system compares the estimated monthly electricity cost with a predefined monthly budget.

Current project setting:

```text
Monthly Budget = ₹2500
```

### Logic

```text
If Total Cost < Budget
    → Within Budget

If Total Cost = Budget
    → Budget Limit Reached

If Total Cost > Budget
    → Budget Exceeded
```

### Remaining Budget

When the estimated cost is below the budget:

```text
Remaining Budget
= Monthly Budget - Total Cost
```

### Excess Cost

When the estimated cost exceeds the budget:

```text
Extra Cost
= Total Cost - Monthly Budget
```

---

# 15. Category-wise Energy Analysis

Appliances are grouped according to their category.

Examples:

```text
Cooling
Kitchen
Entertainment
Lighting
Vehicle
```

The system calculates the total energy consumed by each category.

### Logic

```text
Category Energy
= Sum of Monthly Energy of all appliances
  belonging to that category
```

The project uses:

```cpp
map<string, double>
```

to store category-wise energy totals.

---

# 16. Consumption Ranking

The system can rank appliances according to their energy consumption.

### Process

```text
1. Calculate monthly energy
2. Compare appliance energy values
3. Sort appliances
4. Display highest-consuming appliances first
```

The project uses:

```cpp
std::sort()
```

for sorting.

The sorting order is:

```text
Highest Energy → Lowest Energy
```

---

# 17. Power-Based Sorting

Appliances can also be sorted according to their power rating.

### Sorting Rule

```text
Highest Power → Lowest Power
```

For example:

```text
EV Charger
AC
Water Heater
Heater
...
```

The project uses `std::sort()` with a comparison function.

---

# 18. Maximum Energy Consumer

The system identifies the appliance with the highest monthly energy consumption.

The project uses:

```cpp
max_element()
```

to find the appliance having the maximum `monthlyEnergy` value.

This gives the:

```text
Highest Energy Consumer
```

---

# 19. Appliance Priority Classification

Each appliance has a priority level.

```text
1 → Very High
2 → High
3 → Medium
4 → Low
```

Priority represents the relative importance of the appliance in the simulated system.

The project converts the numerical priority into a readable description.

---

# 20. Appliance State

The CSV contains the current simulated appliance state.

```text
0 → OFF
1 → ON
```

The corresponding C++ field is:

```cpp
bool isOn;
```

### Important Assumption

In the current version of the project, the `status` value is displayed as the appliance's current simulated state, but it is **not used in the monthly energy calculation**.

The monthly energy calculation is based on:

```text
Power × Hours per Day × Billing Days
```

Therefore:

```text
status = current simulated state
hours_per_day = expected/scheduled daily usage
```

A future version could connect appliance status to real-time simulation.

---

# 21. CSV Data Management

The project uses a CSV file as its persistent appliance dataset.

### Data Flow

```text
electricity_management_data.csv
            ↓
       Open File
            ↓
       Read Each Row
            ↓
      Parse CSV Fields
            ↓
     Create Appliance Object
            ↓
      Store in vector
            ↓
      Perform Analysis
```

The project uses:

```cpp
ifstream
stringstream
```

for file reading and CSV parsing.

---

# 22. Vector-Based Storage

The project uses:

```cpp
vector<Appliance> appliances;
```

to store all appliance objects in memory.

### Why Vector?

A vector provides:

- Dynamic storage
- Sequential access
- Easy traversal
- Compatibility with STL algorithms
- Easy sorting

---

# 23. Linear Search

The appliance search feature searches for an appliance by name.

### Logic

```text
Start from first appliance
        ↓
Compare name
        ↓
Match?
   ↓ Yes       ↓ No
Display       Continue
details       searching
```

The current implementation performs a **linear search**.

### Time Complexity

```text
O(n)
```

where `n` is the number of appliances.

---

# 24. Sorting

The project uses the C++ Standard Template Library sorting algorithm:

```cpp
std::sort()
```

Sorting is used for:

```text
1. Power-based ranking
2. Energy-based ranking
3. Top energy consumers
```

### Time Complexity

Typical complexity of `std::sort()`:

```text
O(n log n)
```

---

# 25. Map-Based Aggregation

The category analysis uses:

```cpp
map<string, double>
```

The map stores:

```text
Category → Total Energy
```

Example:

```text
Cooling   → 450.25 kWh
Kitchen   → 220.50 kWh
Vehicle   → 850.00 kWh
```

This is an example of **key-value based aggregation**.

---

# 26. Rule-Based Energy Insights

The Smart Energy Insights module uses predefined rules rather than machine learning.

Examples:

```text
Power >= 1000 W
        ↓
High-Power Appliance

Hours/Day >= 6
        ↓
High-Usage Appliance

Consumption >= 10%
        ↓
Major Energy Consumer

Total Cost > Budget
        ↓
Budget Warning
```

These rules produce simple energy-management recommendations.

---

# 27. Energy-Saving Recommendations

The system generates recommendations based on calculated values.

Examples include:

- Reduce usage of high-consumption appliances.
- Monitor appliances with high power ratings.
- Reduce unnecessary operating hours.
- Switch OFF appliances when they are not required.
- Consider lower-tariff periods for flexible appliances.

These are **rule-based recommendations**, not predictions.

---

# 28. Report Generation

The system generates a complete report containing:

```text
System Summary
       ↓
Number of Appliances
       ↓
Billing Period
       ↓
Tariff
       ↓
Monthly Budget
       ↓
Total Energy
       ↓
Estimated Bill
       ↓
Budget Status
       ↓
Top Energy Consumers
```

The report provides a consolidated view of the simulated electricity usage.

---

# 29. Important Project Assumptions

The current implementation uses the following assumptions:

### Fixed Tariff

```text
₹6/kWh
```

### Fixed Billing Period

```text
30 days
```

### Fixed Monthly Budget

```text
₹2500
```

### Expected Daily Usage

`hours_per_day` is treated as the expected/scheduled daily operating duration.

### Appliance Status

`status` represents the current simulated state but does not currently modify the monthly energy calculation.

### No Real-Time Meter

The project does not directly receive electricity readings from physical smart meters or sensors.

### No Machine Learning

The current "Smart Energy Insights" module is based on predefined rules and thresholds.

---

# 30. Core Calculation Summary

The complete calculation pipeline is:

```text
Power Rating (W)
       +
Daily Operating Hours
       +
Billing Period
       ↓
Monthly Energy Consumption
       ↓
Electricity Tariff
       ↓
Estimated Monthly Cost
       ↓
Budget Comparison
       ↓
Energy Analysis
       ↓
Rule-Based Insights
```

### Main Formulas

```text
Daily Energy (kWh)
= Power × Hours/Day / 1000
```

```text
Monthly Energy (kWh)
= Power × Hours/Day × Billing Days / 1000
```

```text
Monthly Cost
= Monthly Energy × Tariff
```

```text
Consumption Percentage
= Appliance Energy / Total Energy × 100
```

```text
Remaining Budget
= Budget - Total Cost
```

```text
Budget Excess
= Total Cost - Budget
```

---

# 31. DSA Concepts Used

| DSA / Programming Concept | Usage in Project |
|---|---|
| `vector` | Store appliance objects |
| `map` | Category-wise energy aggregation |
| Linear Search | Search appliance by name |
| `sort()` | Sort by power and energy |
| `max_element()` | Find highest energy consumer |
| Iteration | Calculate totals and analysis |
| Classes / Objects | Represent appliances |
| File Handling | Read CSV dataset |
| String Parsing | Parse CSV records |

---

# 32. Complexity Summary

Let `n` be the number of appliances.

| Operation | Complexity |
|---|---:|
| Load CSV | O(n) |
| Display appliances | O(n) |
| Search appliance | O(n) |
| Energy calculation | O(n) |
| Total calculation | O(n) |
| Category aggregation | O(n log k) approximately |
| Sort by power | O(n log n) |
| Sort by energy | O(n log n) |
| Find maximum | O(n) |
| Report generation | O(n log n) |

Here, `k` represents the number of unique categories.

---

# 33. Overall System Logic

```text
                 ┌─────────────────────────┐
                 │ CSV Appliance Dataset   │
                 └────────────┬────────────┘
                              ↓
                 ┌─────────────────────────┐
                 │ CSV Data Loading        │
                 │ ifstream + stringstream │
                 └────────────┬────────────┘
                              ↓
                 ┌─────────────────────────┐
                 │ vector<Appliance>       │
                 └────────────┬────────────┘
                              ↓
              ┌───────────────┴────────────────┐
              ↓                                ↓
       ┌───────────────┐                ┌───────────────┐
       │ Energy        │                │ Appliance     │
       │ Calculation   │                │ Analysis      │
       └───────┬───────┘                └───────┬───────┘
               ↓                                ↓
       Monthly Energy                    Search / Sort
               ↓                        Category Analysis
       Electricity Cost                  High Consumption
               ↓                                ↓
       Budget Analysis                  Smart Insights
               └───────────────┬────────────────┘
                               ↓
                    ┌─────────────────────┐
                    │ Complete Report     │
                    └─────────────────────┘
```

---

# 34. Short Viva Explanation

If asked **"How does your project calculate electricity consumption?"**, explain:

> The system stores the power rating and expected daily operating hours of each appliance. Monthly energy consumption is calculated using the formula Power in Watts multiplied by Hours per Day multiplied by Billing Days, divided by 1000 to convert the result into kWh. The monthly energy is then multiplied by the electricity tariff to estimate the monthly cost. The system aggregates these values for all appliances and performs budget, category, ranking, and high-consumption analysis.

If asked **"What DSA concepts did you use?"**, explain:

> I used a vector to store appliance objects, linear search to find appliances by name, sorting using `std::sort()` for power and energy ranking, a map for category-wise energy aggregation, and `max_element()` to identify the highest energy-consuming appliance.

If asked **"Is the smart analysis AI-based?"**, explain:

> No. The current version uses rule-based analysis with predefined thresholds such as 1000 watts for high-power appliances, 6 hours per day for high-usage appliances, and 10 percent contribution for major energy consumers.

---

# 35. One-Line Project Definition

> **A C++ and DSA-based electricity management system that estimates appliance-wise and household-level energy consumption and cost from power ratings and usage patterns, and provides rule-based analysis, budget monitoring, ranking, and energy-saving insights.**
