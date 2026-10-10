#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <map>
#include <limits>
#include <cmath>

using namespace std;

/*
============================================================
              ELECTRICITY MANAGEMENT SYSTEM
                  C++ + DSA Project
============================================================

Files:
    1. main.cpp
    2. electricity_management_data.csv

CSV Format:
    id,name,category,power_watts,hours_per_day,priority,status

Status:
    0 = OFF
    1 = ON

Priority:
    1 = Very High
    2 = High
    3 = Medium
    4 = Low

DSA Used:
    - vector
    - map
    - searching
    - sorting

Features:
    - Read appliance data from CSV
    - Display appliances
    - Search appliance
    - Sort by power consumption
    - Sort by energy consumption
    - Calculate energy consumption
    - Calculate electricity cost
    - Calculate total consumption
    - Identify high-consumption appliances
    - Budget analysis
    - Category analysis
    - Energy-saving recommendations
    - Smart energy insights
    - Generate complete report
============================================================
*/

// ============================================================
// APPLIANCE CLASS
// ============================================================

class Appliance
{
public:
  int id;
  string name;
  string category;

  double power;      // Watts
  double dailyHours; // Hours per day

  int priority;
  bool isOn;

  double monthlyEnergy;
  double monthlyCost;

  // --------------------------------------------------------
  // Constructor
  // --------------------------------------------------------

  Appliance()
  {
    id = 0;
    name = "";
    category = "";

    power = 0;
    dailyHours = 0;

    priority = 0;
    isOn = false;

    monthlyEnergy = 0;
    monthlyCost = 0;
  }

  // --------------------------------------------------------
  // Calculate Monthly Energy
  // --------------------------------------------------------

  double calculateMonthlyEnergy(int days)
  {
    /*
        Energy (kWh) =
        Power (Watts) × Hours × Days / 1000
    */

    monthlyEnergy =
        (power * dailyHours * days) / 1000.0;

    return monthlyEnergy;
  }

  // --------------------------------------------------------
  // Calculate Monthly Cost
  // --------------------------------------------------------

  double calculateMonthlyCost(double tariff, int days)
  {
    monthlyEnergy =
      calculateMonthlyEnergy(days);

    monthlyCost =
      monthlyEnergy * tariff;

    return monthlyCost;
  }
};

// ============================================================
// GLOBAL DATA
// ============================================================

vector<Appliance> appliances;

// ============================================================
// SYSTEM SETTINGS
// ============================================================

/*
    Since you are using only ONE CSV file and that CSV contains
    appliance information only, these settings are kept here.
*/

double tariff = 6.0;         // Rs. per kWh
double monthlyBudget = 2500; // Rs.
int billingDays = 30;

// ============================================================
// HELPER FUNCTIONS
// ============================================================

// ------------------------------------------------------------
// Convert priority number to text
// ------------------------------------------------------------

string getPriorityText(int priority)
{
  switch (priority)
  {
  case 1:
    return "Very High";

  case 2:
    return "High";

  case 3:
    return "Medium";

  case 4:
    return "Low";

  default:
    return "Unknown";
  }
}

// ------------------------------------------------------------
// Trim spaces from CSV values
// ------------------------------------------------------------

string trim(const string &str)
{
  size_t first = str.find_first_not_of(" \t\r\n");

  if (first == string::npos)
    return "";

  size_t last = str.find_last_not_of(" \t\r\n");

  return str.substr(first, last - first + 1);
}

// ============================================================
// LOAD DATA FROM CSV
// ============================================================

bool loadData()
{
  ifstream file("electricity_management_data.csv");

  // --------------------------------------------------------
  // Check whether file opened successfully
  // --------------------------------------------------------

  if (!file)
  {
    cout << "\nERROR: electricity_management_data.csv file not found.\n";

    cout << "Make sure the CSV file is in the same folder as the executable.\n";

    return false;
  }

  string line;

  // --------------------------------------------------------
  // Read and skip CSV header
  // --------------------------------------------------------

  if (!getline(file, line))
  {
    cout << "\nERROR: CSV file is empty.\n";

    return false;
  }

  // --------------------------------------------------------
  // Read every appliance record
  // --------------------------------------------------------

  while (getline(file, line))
  {
    // Remove carriage return if present
    if (!line.empty() && line.back() == '\r')
    {
      line.pop_back();
    }

    // Ignore empty lines
    if (trim(line).empty())
    {
      continue;
    }

    stringstream ss(line);

    Appliance appliance;

    string value;

    try
    {
      // ------------------------------------------------
      // ID
      // ------------------------------------------------

      if (!getline(ss, value, ','))
        continue;

      appliance.id = stoi(trim(value));

      // ------------------------------------------------
      // Name
      // ------------------------------------------------

      if (!getline(ss, appliance.name, ','))
        continue;

      appliance.name = trim(appliance.name);

      // ------------------------------------------------
      // Category
      // ------------------------------------------------

      if (!getline(ss, appliance.category, ','))
        continue;

      appliance.category = trim(appliance.category);

      // ------------------------------------------------
      // Power
      // ------------------------------------------------

      if (!getline(ss, value, ','))
        continue;

      appliance.power = stod(trim(value));

      // ------------------------------------------------
      // Hours per day
      // ------------------------------------------------

      if (!getline(ss, value, ','))
        continue;

      appliance.dailyHours = stod(trim(value));

      // ------------------------------------------------
      // Priority
      // ------------------------------------------------

      if (!getline(ss, value, ','))
        continue;

      appliance.priority = stoi(trim(value));

      // ------------------------------------------------
      // Status
      // ------------------------------------------------

      if (!getline(ss, value, ','))
        continue;

      appliance.isOn = (stoi(trim(value)) == 1);

      // ------------------------------------------------
      // Initialize calculated values
      // ------------------------------------------------

      appliance.monthlyEnergy = 0;
      appliance.monthlyCost = 0;

      // ------------------------------------------------
      // Store appliance
      // ------------------------------------------------

      appliances.push_back(appliance);
    }
    catch (const exception &)
    {
      cout << "\nWarning: Invalid CSV row skipped:\n";
      cout << line << "\n";
    }
  }

  file.close();

  // --------------------------------------------------------
  // Check whether any data was loaded
  // --------------------------------------------------------

  if (appliances.empty())
  {
    cout << "\nERROR: No appliance data found in CSV file.\n";

    return false;
  }

  return true;
}

// ============================================================
// DISPLAY APPLIANCES
// ============================================================

void displayAppliances()
{
  cout << "\n";
  cout << "================================================================================================================\n";
  cout << "                                      APPLIANCE INFORMATION\n";
  cout << "================================================================================================================\n";

  cout << left
       << setw(5) << "ID"
       << setw(22) << "Name"
       << setw(18) << "Category"
       << setw(12) << "Power(W)"
       << setw(12) << "Hours/Day"
       << setw(12) << "Priority"
       << setw(10) << "Status"
       << "\n";

  cout << string(91, '-') << "\n";

  for (const auto &appliance : appliances)
  {
    cout << left
         << setw(5) << appliance.id
         << setw(22) << appliance.name
         << setw(18) << appliance.category
         << setw(12) << appliance.power
         << setw(12) << appliance.dailyHours
         << setw(12) << getPriorityText(appliance.priority)
         << setw(10) << (appliance.isOn ? "ON" : "OFF")
         << "\n";
  }
}

// ============================================================
// ENERGY CONSUMPTION ANALYSIS
// ============================================================

void displayEnergyAnalysis()
{
  double totalEnergy = 0.0;
  double totalCost = 0.0;

  cout << "\n";
  cout << "===============================================================================================\n";
  cout << "                              ENERGY CONSUMPTION ANALYSIS\n";
  cout << "===============================================================================================\n";

  cout << left
       << setw(22) << "Appliance"
       << setw(15) << "Power(W)"
       << setw(15) << "Hours/Day"
       << setw(18) << "Monthly kWh"
       << setw(15) << "Monthly Cost"
       << "\n";

  cout << string(85, '-') << "\n";

  for (auto &appliance : appliances)
  {
    double energy =
        appliance.calculateMonthlyEnergy(billingDays);

    double cost =
        appliance.calculateMonthlyCost(
            tariff,
            billingDays);

    totalEnergy += energy;
    totalCost += cost;

    cout << left
         << setw(22) << appliance.name
         << setw(15) << appliance.power
         << setw(15) << appliance.dailyHours
         << setw(18) << fixed << setprecision(2) << energy
         << setw(15) << fixed << setprecision(2) << cost
         << "\n";
  }

  cout << string(85, '-') << "\n";

  cout << "\nTotal Monthly Energy : "
       << fixed << setprecision(2)
       << totalEnergy
       << " kWh\n";

  cout << "Total Monthly Cost   : Rs. "
       << fixed << setprecision(2)
       << totalCost
       << "\n";

  cout << "Electricity Tariff   : Rs. "
       << tariff
       << " / kWh\n";

  cout << "Billing Period       : "
       << billingDays
       << " days\n";
}

// ============================================================
// SEARCH APPLIANCE
// ============================================================

void searchAppliance()
{
  string searchName;

  cout << "\nEnter appliance name: ";

  cin.ignore(
      numeric_limits<streamsize>::max(),
      '\n');

  getline(cin, searchName);

  searchName = trim(searchName);

  bool found = false;

  for (const auto &appliance : appliances)
  {
    if (appliance.name == searchName)
    {
      cout << "\nAppliance Found\n";
      cout << "-----------------------------\n";

      cout << "ID          : "
           << appliance.id
           << "\n";

      cout << "Name        : "
           << appliance.name
           << "\n";

      cout << "Category    : "
           << appliance.category
           << "\n";

      cout << "Power       : "
           << appliance.power
           << " W\n";

      cout << "Daily Usage : "
           << appliance.dailyHours
           << " hours\n";

      cout << "Priority    : "
           << getPriorityText(appliance.priority)
           << "\n";

      cout << "Status      : "
           << (appliance.isOn ? "ON" : "OFF")
           << "\n";

      found = true;

      break;
    }
  }

  if (!found)
  {
    cout << "\nAppliance not found.\n";
  }
}

// ============================================================
// SORT BY POWER
// ============================================================

void sortByPower()
{
  sort(
      appliances.begin(),
      appliances.end(),

      [](const Appliance &a, const Appliance &b)
      {
        return a.power > b.power;
      });

  cout << "\nAppliances sorted by highest power consumption.\n";

  cout << "\n";

  cout << left
       << setw(5) << "ID"
       << setw(28) << "Appliance"
       << setw(18) << "Power(W)"
       << "\n";

  cout << string(51, '-') << "\n";

  for (const auto &appliance : appliances)
  {
    cout << left
         << setw(5) << appliance.id
         << setw(28) << appliance.name
         << setw(18) << appliance.power
         << "\n";
  }
}

// ============================================================
// SORT BY MONTHLY ENERGY
// ============================================================

void sortByEnergy()
{
  /*
      Important:
      Calculate monthly energy BEFORE sorting.
  */

  for (auto &appliance : appliances)
  {
    appliance.calculateMonthlyEnergy(billingDays);
  }

  sort(
      appliances.begin(),
      appliances.end(),

      [](const Appliance &a, const Appliance &b)
      {
        return a.monthlyEnergy >
               b.monthlyEnergy;
      });

  cout << "\n";
  cout << "====================================================================\n";
  cout << "                 APPLIANCES BY ENERGY CONSUMPTION\n";
  cout << "====================================================================\n";

  cout << left
       << setw(8) << "Rank"
       << setw(28) << "Appliance"
       << setw(20) << "Monthly Energy"
       << "\n";

  cout << string(56, '-') << "\n";

  int rank = 1;

  for (const auto &appliance : appliances)
  {
    cout << left
         << setw(8) << rank
         << setw(28) << appliance.name
         << setw(20) << fixed
         << setprecision(2)
         << appliance.monthlyEnergy
         << " kWh\n";

    rank++;
  }
}

// ============================================================
// HIGH CONSUMPTION ANALYSIS
// ============================================================

void highConsumptionAnalysis()
{
  if (appliances.empty())
    return;

  double totalEnergy = 0;

  // Calculate fresh energy values
  for (auto &appliance : appliances)
  {
    totalEnergy +=
        appliance.calculateMonthlyEnergy(
            billingDays);
  }

  cout << "\n";
  cout << "================================================================\n";
  cout << "                HIGH CONSUMPTION ANALYSIS\n";
  cout << "================================================================\n";

  cout << "\nTotal Energy Consumption: "
       << fixed << setprecision(2)
       << totalEnergy
       << " kWh\n";

  if (totalEnergy <= 0)
  {
    cout << "\nNo energy consumption recorded.\n";
    return;
  }

  cout << "\nMajor Energy Consumers:\n";

  bool found = false;

  for (const auto &appliance : appliances)
  {
    double energy =
        appliance.monthlyEnergy;

    double percentage =
        (energy / totalEnergy) * 100;

    if (percentage >= 10)
    {
      cout << "\n"
           << appliance.name
           << " consumes approximately "
           << fixed << setprecision(2)
           << percentage
           << "% of total energy.\n";

      found = true;
    }
  }

  if (!found)
  {
    cout << "\nNo single appliance contributes 10% or more of total consumption.\n";
  }
}

// ============================================================
// BUDGET ANALYSIS
// ============================================================

void budgetAnalysis()
{
  double totalCost = 0;

  for (auto &appliance : appliances)
  {
    totalCost +=
        appliance.calculateMonthlyCost(
            tariff,
            billingDays);
  }

  cout << "\n";
  cout << "================================================================\n";
  cout << "                       BUDGET ANALYSIS\n";
  cout << "================================================================\n";

  cout << "\nMonthly Budget : Rs. "
       << fixed << setprecision(2)
       << monthlyBudget;

  cout << "\nExpected Cost  : Rs. "
       << fixed << setprecision(2)
       << totalCost;

  double difference =
      monthlyBudget - totalCost;

  if (difference > 0)
  {
    cout << "\n\nStatus: WITHIN BUDGET";

    cout << "\nRemaining Budget: Rs. "
         << fixed << setprecision(2)
         << difference
         << "\n";
  }

  else if (fabs(difference) < 0.000001)
  {
    cout << "\n\nStatus: BUDGET LIMIT REACHED\n";
  }

  else
  {
    cout << "\n\nStatus: BUDGET EXCEEDED";

    cout << "\nExtra Cost: Rs. "
         << fixed << setprecision(2)
         << fabs(difference)
         << "\n";
  }
}

// ============================================================
// SMART ENERGY INSIGHTS
// ============================================================

void generateInsights()
{
  cout << "\n";
  cout << "================================================================\n";
  cout << "                    SMART ENERGY INSIGHTS\n";
  cout << "================================================================\n";

  double totalEnergy = 0;
  double totalCost = 0;

  // Calculate fresh values
  for (auto &appliance : appliances)
  {
    totalEnergy +=
        appliance.calculateMonthlyEnergy(
            billingDays);

    totalCost +=
        appliance.calculateMonthlyCost(
            tariff,
            billingDays);
  }

  // --------------------------------------------------------
  // Overall Consumption
  // --------------------------------------------------------

  cout << "\nOverall Consumption\n";
  cout << "-------------------------\n";

  cout << "Monthly Energy : "
       << fixed << setprecision(2)
       << totalEnergy
       << " kWh\n";

  cout << "Monthly Cost   : Rs. "
       << fixed << setprecision(2)
       << totalCost
       << "\n";

  // --------------------------------------------------------
  // Highest Energy Consumer
  // --------------------------------------------------------

  auto highest =
      max_element(
          appliances.begin(),
          appliances.end(),

          [](const Appliance &a, const Appliance &b)
          {
            return a.monthlyEnergy <
                   b.monthlyEnergy;
          });

  if (highest != appliances.end())
  {
    cout << "\nHighest Energy Consumer\n";
    cout << "-------------------------\n";

    cout << highest->name
         << " consumes "
         << fixed << setprecision(2)
         << highest->monthlyEnergy
         << " kWh per month.\n";
  }

  // --------------------------------------------------------
  // High Power Appliances
  // --------------------------------------------------------

  cout << "\nHigh Power Appliances\n";
  cout << "-------------------------\n";

  bool foundHighPower = false;

  for (const auto &appliance : appliances)
  {
    if (appliance.power >= 1000)
    {
      cout << "- "
           << appliance.name
           << " uses "
           << appliance.power
           << " W.\n";

      foundHighPower = true;
    }
  }

  if (!foundHighPower)
  {
    cout << "No appliance exceeds 1000 W.\n";
  }

  // --------------------------------------------------------
  // Long Usage Appliances
  // --------------------------------------------------------

  cout << "\nHigh Usage Appliances\n";
  cout << "-------------------------\n";

  bool foundHighUsage = false;

  for (const auto &appliance : appliances)
  {
    if (appliance.dailyHours >= 6)
    {
      cout << "- "
           << appliance.name
           << " runs approximately "
           << appliance.dailyHours
           << " hours/day.\n";

      foundHighUsage = true;
    }
  }

  if (!foundHighUsage)
  {
    cout << "No appliance has more than 6 hours of daily usage.\n";
  }

  // --------------------------------------------------------
  // Recommendations
  // --------------------------------------------------------

  cout << "\nRecommendations\n";
  cout << "-------------------------\n";

  if (totalCost > monthlyBudget)
  {
    cout << "1. Reduce usage of high-consumption appliances because "
         << "the estimated bill exceeds the budget.\n";
  }
  else
  {
    cout << "1. Current estimated consumption is within the monthly budget.\n";
  }

  cout << "2. Monitor appliances with high power ratings.\n";

  cout << "3. Reduce unnecessary operating hours of high-energy appliances.\n";

  cout << "4. Switch OFF appliances when they are not required.\n";

  cout << "5. Consider operating flexible appliances during lower-tariff periods.\n";
}

// ============================================================
// CATEGORY ANALYSIS
// ============================================================

void categoryAnalysis()
{
  map<string, double> categoryEnergy;

  for (auto &appliance : appliances)
  {
    double energy =
        appliance.calculateMonthlyEnergy(
            billingDays);

    categoryEnergy[appliance.category] += energy;
  }

  cout << "\n";
  cout << "================================================================\n";
  cout << "                       CATEGORY ANALYSIS\n";
  cout << "================================================================\n";

  cout << left
       << setw(25) << "Category"
       << setw(20) << "Energy (kWh)"
       << "\n";

  cout << string(45, '-') << "\n";

  for (const auto &item : categoryEnergy)
  {
    cout << left
         << setw(25) << item.first
         << setw(20) << fixed
         << setprecision(2)
         << item.second
         << "\n";
  }
}

// ============================================================
// GENERATE COMPLETE REPORT
// ============================================================

void generateReport()
{
  cout << "\n\n";
  cout << "================================================================\n";
  cout << "                ELECTRICITY MANAGEMENT REPORT\n";
  cout << "================================================================\n";

  double totalEnergy = 0;
  double totalCost = 0;

  // --------------------------------------------------------
  // Calculate fresh values
  // --------------------------------------------------------

  for (auto &appliance : appliances)
  {
    totalEnergy +=
        appliance.calculateMonthlyEnergy(
            billingDays);

    totalCost +=
        appliance.calculateMonthlyCost(
            tariff,
            billingDays);
  }

  // --------------------------------------------------------
  // System Summary
  // --------------------------------------------------------

  cout << "\nSYSTEM SUMMARY\n";
  cout << "-------------------------\n";

  cout << "Number of Appliances : "
       << appliances.size()
       << "\n";

  cout << "Billing Period       : "
       << billingDays
       << " days\n";

  cout << "Tariff               : Rs. "
       << fixed << setprecision(2)
       << tariff
       << " / kWh\n";

  cout << "Monthly Budget       : Rs. "
       << fixed << setprecision(2)
       << monthlyBudget
       << "\n";

  // --------------------------------------------------------
  // Consumption
  // --------------------------------------------------------

  cout << "\nCONSUMPTION\n";
  cout << "-------------------------\n";

  cout << "Total Energy         : "
       << fixed << setprecision(2)
       << totalEnergy
       << " kWh\n";

  cout << "Estimated Bill       : Rs. "
       << fixed << setprecision(2)
       << totalCost
       << "\n";

  // --------------------------------------------------------
  // Budget Status
  // --------------------------------------------------------

  cout << "\nBUDGET STATUS\n";
  cout << "-------------------------\n";

  if (totalCost <= monthlyBudget)
  {
    cout << "Within monthly budget.\n";

    cout << "Remaining amount: Rs. "
         << fixed << setprecision(2)
         << monthlyBudget - totalCost
         << "\n";
  }
  else
  {
    cout << "Monthly budget exceeded.\n";

    cout << "Exceeded by: Rs. "
         << fixed << setprecision(2)
         << totalCost - monthlyBudget
         << "\n";
  }

  // --------------------------------------------------------
  // Top Consumers
  // --------------------------------------------------------

  cout << "\nTOP CONSUMERS\n";
  cout << "-------------------------\n";

  /*
      Make a copy so that the original appliance order
      does not change when generating the report.
  */

  vector<Appliance> sortedAppliances =
      appliances;

  // Calculate energy before sorting
  for (auto &appliance : sortedAppliances)
  {
    appliance.calculateMonthlyEnergy(
        billingDays);
  }

  sort(
      sortedAppliances.begin(),
      sortedAppliances.end(),

      [](const Appliance &a, const Appliance &b)
      {
        return a.monthlyEnergy >
               b.monthlyEnergy;
      });

  int count = 0;

  for (const auto &appliance : sortedAppliances)
  {
    cout << count + 1
         << ". "
         << appliance.name
         << " - "
         << fixed << setprecision(2)
         << appliance.monthlyEnergy
         << " kWh\n";

    count++;

    if (count == 5)
      break;
  }

  cout << "\n";
}

// ============================================================
// MAIN MENU
// ============================================================

void showMenu()
{
  cout << "\n\n";

  cout << "====================================================\n";
  cout << "           ELECTRICITY MANAGEMENT SYSTEM\n";
  cout << "====================================================\n";

  cout << "1.  View Appliance Data\n";
  cout << "2.  Energy Consumption Analysis\n";
  cout << "3.  Search Appliance\n";
  cout << "4.  Sort Appliances by Power\n";
  cout << "5.  Sort Appliances by Energy\n";
  cout << "6.  High Consumption Analysis\n";
  cout << "7.  Budget Analysis\n";
  cout << "8.  Category Analysis\n";
  cout << "9.  Smart Energy Insights\n";
  cout << "10. Generate Complete Report\n";
  cout << "0.  Exit\n";

  cout << "====================================================\n";
}

// ============================================================
// MAIN FUNCTION
// ============================================================

int main()
{
  // --------------------------------------------------------
  // Load all information from CSV
  // --------------------------------------------------------

  if (!loadData())
  {
    return 1;
  }

  cout << "\nData loaded successfully.\n";

  cout << "CSV file: electricity_management_data.csv\n";

  cout << "Appliances loaded: "
       << appliances.size()
       << "\n";

  cout << "Tariff: Rs. "
       << fixed << setprecision(2)
       << tariff
       << " / kWh\n";

  cout << "Monthly Budget: Rs. "
       << fixed << setprecision(2)
       << monthlyBudget
       << "\n";

  cout << "Billing Period: "
       << billingDays
       << " days\n";

  int choice;

  do
  {
    showMenu();

    cout << "\nEnter your choice: ";

    cin >> choice;

    // ----------------------------------------------------
    // Handle invalid input
    // ----------------------------------------------------

    if (cin.fail())
    {
      cin.clear();

      cin.ignore(
          numeric_limits<streamsize>::max(),
          '\n');

      cout << "\nInvalid input. Please enter a number.\n";

      continue;
    }

    // ----------------------------------------------------
    // Menu
    // ----------------------------------------------------

    switch (choice)
    {
    case 1:

      displayAppliances();

      break;

    case 2:

      displayEnergyAnalysis();

      break;

    case 3:

      searchAppliance();

      break;

    case 4:

      sortByPower();

      break;

    case 5:

      sortByEnergy();

      break;

    case 6:

      highConsumptionAnalysis();

      break;

    case 7:

      budgetAnalysis();

      break;

    case 8:

      categoryAnalysis();

      break;

    case 9:

      generateInsights();

      break;

    case 10:

      generateReport();

      break;

    case 0:

      cout << "\nThank you for using the Electricity Management System.\n";

      break;

    default:

      cout << "\nInvalid choice. Please try again.\n";
    }

  } while (choice != 0);

  return 0;
}