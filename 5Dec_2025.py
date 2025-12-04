import taipy as tp
from taipy import Gui
import pandas as pd


# =========================
# 1. DATA PROCESSING LOGIC
# =========================

def load_data():
    # Simulated dataset
    return pd.DataFrame({
        "day": ["Mon", "Tue", "Wed", "Thu", "Fri"],
        "sales": [120, 150, 180, 90, 200]
    })


def compute_summary(df):
    return {
        "total_sales": df["sales"].sum(),
        "average_sales": df["sales"].mean()
    }


# =========================
# 2. TAIPY TASKS & SCENARIO
# =========================

def build_scenario():
    # Task 1: Load data
    load_task = tp.Task(
        function=load_data,
        name="load_data_task"
    )

    # Task 2: Process summary
    summary_task = tp.Task(
        function=compute_summary,
        name="summary_task",
        input="load_data_task"
    )

    # Scenario combining tasks
    scenario = tp.Scenario(
        tasks=[load_task, summary_task],
        name="sales_pipeline"
    )
    return scenario


# =========================
# 3. GUI STATE FUNCTIONS
# =========================

def run_pipeline(state):
    # Execute the Taipy pipeline
    state.scenario.submit()

    # Update UI state
    state.data = state.scenario.tasks["load_data_task"].output
    state.summary = state.scenario.tasks["summary_task"].output


# =========================
# 4. INITIAL STATE
# =========================

scenario = build_scenario()
data = pd.DataFrame()
summary = {"total_sales": 0, "average_sales": 0}


# =========================
# 5. GUI PAGE
# =========================

page = """
# Sales Dashboard (Taipy Example)

## Run Pipeline
<button onclick="run_pipeline">Load & Process Data</button>

## Data Loaded
<|data|table|>

## Summary
**Total Sales:** {summary[total_sales]}
<br>
**Average Sales:** {summary[average_sales]}

## Chart
<|{data}|chart|x=day|y=sales|type=bar|>
"""


# =========================
# 6. RUN APP
# =========================

if __name__ == "__main__":
    Gui(page).run()
