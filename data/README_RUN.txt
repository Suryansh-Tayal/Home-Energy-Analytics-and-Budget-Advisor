HOME ENERGY ANALYTICS & BUDGET ADVISOR
C++17 backend + HTML/CSS/JavaScript frontend

RUN ON ANDROID (TERMUX)
1. Extract this ZIP. Open Termux and go to the extracted home_energy_cpp folder.
2. If needed, allow storage access once: termux-setup-storage
3. Install compiler if needed: pkg install clang
4. Compile: clang++ -std=c++17 main.cpp -o home_energy
5. Start server: ./home_energy
6. Keep Termux open. Open Chrome and visit: http://127.0.0.1:8080
IMPORTANT: This project runs on PORT 8080 (not 8765).
7. Stop server with CTRL+C in Termux.

RUN ON LINUX / MAC (with a C++ compiler)
clang++ -std=c++17 main.cpp -o home_energy
./home_energy
Open http://127.0.0.1:8080

PROJECT FEATURES
- Home page with separate sections and page navigation.
- Appliance catalogue and schedule with start/end times.
- Daily and monthly energy use, estimated daily and monthly appliance costs.
- Add-your-own appliance form; custom appliance details are saved in data/custom_appliances.csv and loaded again on restart.
- Energy analysis, budget comparison, smart insights and complete report.
- Solar ON/OFF switch directly inside Complete Report; report recalculates immediately. This is a software estimate only and does not control real solar hardware.
- Complete Report cards and appliance rows can be tapped for calculation details.
- WattBot assistant (🤖) at the bottom-right; supports Hinglish, Hindi and English.
- Tap-to-understand explanations under page terms, headings, labels, cards and descriptions.
- User data is stored in data/usage.csv and data/settings.txt.

IMPORTANT ESTIMATION NOTES
- Example tariff: ₹8 per unit. Update the tariff in main.cpp if needed.
- Monthly estimates assume the entered schedule repeats daily for 30 days.
- Solar ON assumes 60% solar / 40% grid during 6 AM–6 PM and 40% solar / 60% grid during 6 PM–6 AM, subject to the demo solar-generation cap (225 kWh/month).
- Solar and bill figures are estimates, not live meter or inverter readings. Actual bills may include slab rates, fixed charges and taxes.

DATA FILES
data/electricity_management_data.csv - reference appliance dataset
data/custom_appliances.csv - saved custom appliance catalogue
data/usage.csv - saved appliance schedules
data/settings.txt - solar ON/OFF preference

Do not delete the data folder if you want to keep saved entries and settings.
