# Parallel Data Processing with Dask (Complete Example)

import dask
import dask.array as da
import dask.dataframe as dd
from dask.distributed import Client, LocalCluster
import numpy as np
import pandas as pd

# -----------------------------
# 1. Start a Local Dask Cluster
# -----------------------------
# This simulates a real distributed environment on a single machine
cluster = LocalCluster(
    n_workers=4,        # number of worker processes
    threads_per_worker=2,
    memory_limit="1GB"
)
client = Client(cluster)
print(client)

# -----------------------------
# 2. Parallel Array Computation
# -----------------------------
# Create a large NumPy-like array split into chunks
array = da.random.random(
    (50_000, 50_000),
    chunks=(5_000, 5_000)
)

# Define lazy computations
array_mean = array.mean()
array_std = array.std()

# Trigger parallel execution
mean_result, std_result = dask.compute(array_mean, array_std)

print("Array Mean:", mean_result)
print("Array Std Dev:", std_result)

# -----------------------------
# 3. Parallel DataFrame Processing
# -----------------------------
# Create a large Pandas DataFrame
pdf = pd.DataFrame({
    "user_id": np.arange(1_000_000),
    "value": np.random.randint(1, 100, size=1_000_000)
})

# Convert to Dask DataFrame with partitions
ddf = dd.from_pandas(pdf, npartitions=8)

# Lazy transformations
filtered = ddf[ddf["value"] > 50]
grouped = filtered.groupby("value").value.count()

# Execute computation in parallel
result_df = grouped.compute()
print(result_df.head())

# -----------------------------
# 4. Custom Parallel Function
# -----------------------------
def expensive_computation(x):
    return x * x + np.sqrt(x)

# Create delayed tasks
tasks = [dask.delayed(expensive_computation)(i) for i in range(1000)]

# Execute tasks in parallel
results = dask.compute(*tasks)
print("Sample delayed result:", results[0])

# -----------------------------
# 5. Cleanup
# -----------------------------
client.close()
cluster.close()
