from flask import Flask, render_template, jsonify
import csv
from collections import defaultdict
from datetime import datetime

app = Flask(__name__)

TARIFF = 6.0
MONTHLY_BUDGET = 2500

def load_raw_data():
    rows = []
    with open("electricity_management_data.csv", newline="", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for row in reader:
            try:
                rows.append({
                    "date": row["date"],
                    "appliance_id": int(row["appliance_id"]),
                    "name": row["name"].strip(),
                    "category": row["category"].strip(),
                    "power": float(row["power_watts"]),
                    "hours": float(row["hours_per_day"]),
                    "priority": int(row["priority"]),
                    "status": int(row["status"]),
                    "start_time": row.get("start_time", ""),
                    "end_time": row.get("end_time", ""),
                    "peak_time": row.get("peak_time", ""),
                    "efficiency": float(row.get("efficiency_rating", 0)),
                    "standby": float(row.get("standby_watts", 0)),
                })
            except Exception:
                continue
    return rows

RAW = load_raw_data()

# Aggregate per appliance
def get_appliances_summary():
    apps = {}
    for r in RAW:
        aid = r["appliance_id"]
        if aid not in apps:
            apps[aid] = {
                "id": aid,
                "name": r["name"],
                "category": r["category"],
                "power": r["power"],
                "efficiency": r["efficiency"],
                "standby": r["standby"],
                "total_hours": 0,
                "total_energy": 0,
                "days": 0,
                "records": []
            }
        energy = (r["power"] * r["hours"]) / 1000.0
        apps[aid]["total_hours"] += r["hours"]
        apps[aid]["total_energy"] += energy
        apps[aid]["days"] += 1
        apps[aid]["records"].append(r)
    # averages
    for a in apps.values():
        a["avg_hours"] = round(a["total_hours"] / a["days"], 2) if a["days"] else 0
        a["total_energy"] = round(a["total_energy"], 2)
        a["total_cost"] = round(a["total_energy"] * TARIFF, 2)
    return list(apps.values())

APPLIANCES = get_appliances_summary()

def get_weekly_data():
    weeks = {1: [], 2: [], 3: [], 4: []}
    for r in RAW:
        dt = datetime.strptime(r["date"], "%Y-%m-%d")
        day = dt.day
        week = 1 if day <= 7 else 2 if day <= 14 else 3 if day <= 21 else 4
        energy = (r["power"] * r["hours"]) / 1000.0
        weeks[week].append({**r, "energy": energy})
    return weeks

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/api/appliances")
def api_appliances():
    data = []
    for a in APPLIANCES:
        data.append({
            "id": a["id"],
            "name": a["name"],
            "category": a["category"],
            "power": a["power"],
            "avg_hours": a["avg_hours"],
            "total_energy": a["total_energy"],
            "total_cost": a["total_cost"],
            "efficiency": a["efficiency"],
            "standby": a["standby"],
        })
    return jsonify(sorted(data, key=lambda x: -x["total_cost"]))

@app.route("/api/appliance/<int:aid>")
def api_appliance_detail(aid):
    records = [r for r in RAW if r["appliance_id"] == aid]
    if not records:
        return jsonify({"error": "not found"}), 404
    name = records[0]["name"]
    daily = []
    for r in sorted(records, key=lambda x: x["date"]):
        energy = round((r["power"] * r["hours"]) / 1000.0, 2)
        daily.append({
            "date": r["date"],
            "hours": r["hours"],
            "energy": energy,
            "start_time": r["start_time"],
            "end_time": r["end_time"],
            "peak_time": r["peak_time"],
        })
    total_energy = round(sum(d["energy"] for d in daily), 2)
    return jsonify({
        "name": name,
        "category": records[0]["category"],
        "power": records[0]["power"],
        "daily": daily,
        "total_energy": total_energy,
        "total_cost": round(total_energy * TARIFF, 2),
    })

@app.route("/api/summary")
def api_summary():
    total_energy = sum(a["total_energy"] for a in APPLIANCES)
    total_cost = sum(a["total_cost"] for a in APPLIANCES)
    remaining = MONTHLY_BUDGET - total_cost
    highest = max(APPLIANCES, key=lambda x: x["total_energy"], default=None)
    return jsonify({
        "totalAppliances": len(APPLIANCES),
        "totalEnergy": round(total_energy, 2),
        "totalCost": round(total_cost, 2),
        "budget": MONTHLY_BUDGET,
        "remaining": round(remaining, 2),
        "status": "Within Budget" if remaining >= 0 else "Budget Exceeded",
        "highestConsumer": highest["name"] if highest else None,
        "highestEnergy": highest["total_energy"] if highest else 0,
    })

@app.route("/api/energy")
def api_energy():
    result = []
    for a in APPLIANCES:
        # pick most common peak / best time from records
        peaks = [r["peak_time"] for r in a["records"] if r["peak_time"]]
        starts = [r["start_time"] for r in a["records"] if r["start_time"]]
        best = max(set(starts), key=starts.count) if starts else "-"
        peak = max(set(peaks), key=peaks.count) if peaks else "-"
        result.append({
            "name": a["name"],
            "category": a["category"],
            "power": a["power"],
            "avg_hours": a["avg_hours"],
            "total_energy": a["total_energy"],
            "total_cost": a["total_cost"],
            "best_time": best,
            "peak_hour": peak,
            "efficiency": a["efficiency"],
        })
    return jsonify(sorted(result, key=lambda x: -x["total_cost"]))

@app.route("/api/category")
def api_category():
    cat = defaultdict(float)
    for a in APPLIANCES:
        cat[a["category"]] += a["total_cost"]
    data = [{"category": k, "cost": round(v, 2)} for k, v in sorted(cat.items(), key=lambda x: -x[1])]
    return jsonify(data)

@app.route("/api/weekly")
def api_weekly():
    weeks = get_weekly_data()
    result = {}
    for w, records in weeks.items():
        by_app = defaultdict(float)
        for r in records:
            by_app[r["name"]] += r["energy"]
        top5 = sorted(by_app.items(), key=lambda x: -x[1])[:5]
        total = sum(by_app.values())
        result[w] = {
            "total_energy": round(total, 2),
            "total_cost": round(total * TARIFF, 2),
            "top5": [{"name": n, "energy": round(e, 2)} for n, e in top5],
            "recommendation": "Reduce AC / Heater usage" if total > 200 else "Usage looks controlled this week"
        }
    return jsonify(result)

@app.route("/api/insights")
def api_insights():
    total_cost = sum(a["total_cost"] for a in APPLIANCES)
    highest = max(APPLIANCES, key=lambda x: x["total_energy"])
    high_power = [a["name"] for a in APPLIANCES if a["power"] >= 1000]
    return jsonify({
        "total_cost": round(total_cost, 2),
        "totalEnergy": round(sum(a["total_energy"] for a in APPLIANCES), 2),
        "highestConsumer": highest["name"],
        "highestEnergy": highest["total_energy"],
        "highPower": high_power,
        "recommendations": [
            "Check weekly breakdowns for peak usage patterns.",
            "Focus on high-power appliances during off-peak hours.",
            "Reduce standby consumption where possible.",
            "Monitor AC and Water Heater as they dominate cost.",
        ]
    })

@app.route("/api/report")
def api_report():
    total_energy = sum(a["total_energy"] for a in APPLIANCES)
    total_cost = sum(a["total_cost"] for a in APPLIANCES)
    top5 = sorted(APPLIANCES, key=lambda a: -a["total_energy"])[:5]
    
    # Daily totals for hover info
    daily_totals = defaultdict(float)
    daily_max_app = {}
    for r in RAW:
        e = (r["power"] * r["hours"]) / 1000.0
        daily_totals[r["date"]] += e
        if r["date"] not in daily_max_app or e > daily_max_app[r["date"]][1]:
            daily_max_app[r["date"]] = (r["name"], e)

    avg_daily = round(sum(daily_totals.values()) / len(daily_totals), 2) if daily_totals else 0
    max_day = max(daily_totals.items(), key=lambda x: x[1]) if daily_totals else ("-", 0)

    return jsonify({
        "appliancesCount": len(APPLIANCES),
        "totalEnergy": round(total_energy, 2),
        "totalCost": round(total_cost, 2),
        "budget": MONTHLY_BUDGET,
        "status": "Within Budget" if total_cost <= MONTHLY_BUDGET else "Budget Exceeded",
        "top5": [{"name": a["name"], "energy": a["total_energy"]} for a in top5],
        "chart_data": [{"name": a["name"], "energy": a["total_energy"]} for a in sorted(APPLIANCES, key=lambda x: -x["total_energy"])],
        "max_day": {"date": max_day[0], "energy": round(max_day[1], 2), "appliance": daily_max_app.get(max_day[0], ("-",0))[0]},
        "avg_daily": avg_daily,
        "paragraph": f"Total monthly consumption is {round(total_energy,1)} kWh costing approximately ₹{round(total_cost,0)}. The highest single-day consumption was on {max_day[0]} ({round(max_day[1],1)} kWh), mainly driven by {daily_max_app.get(max_day[0], ('-',0))[0]}. Average daily usage stands at {avg_daily} kWh."
    })

@app.route("/api/budget-comparison")
def api_budget_comparison():
    total_cost = sum(a["total_cost"] for a in APPLIANCES)
    return jsonify({
        "expectedBudget": MONTHLY_BUDGET,
        "realConsumptionCost": round(total_cost, 2),
        "difference": round(MONTHLY_BUDGET - total_cost, 2),
        "status": "Within Budget" if total_cost <= MONTHLY_BUDGET else "Exceeded",
    })


@app.route("/api/category-energy")
def api_category_energy():
    cat = defaultdict(float)
    for a in APPLIANCES:
        cat[a["category"]] += a["total_energy"]
    data = [{"category": k, "energy": round(v, 2)} for k, v in sorted(cat.items(), key=lambda x: -x[1])]
    return jsonify(data)

if __name__ == "__main__":
    app.run(debug=True, port=5000)
