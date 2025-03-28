import matplotlib.pyplot as plt
import numpy as np

# Performance data
data = {
    '-Ofast': {
        'float': 2431.330322,
        'floatIntrinsics': 314.371002,
        'floatArr': 206.541214
    },
    '-O3': {
        'float': 2546.723877,
        'floatIntrinsics': 329.517426,
        'floatArr': 213.129440
    },
    '-O2': {
        'float': 2544.549561,
        'floatIntrinsics': 333.004608,
        'floatArr': 3592.113281
    },
    '-O1': {
        'float': 2480.524658,
        'floatIntrinsics': 328.854340,
        'floatArr': 3064.068848
    },
    'no opt flags': {
        'float': 4918.506836,
        'floatIntrinsics': 2724.700928,
        'floatArr': 16347.489258
    }
}

# Prepare data for plotting
functions = ['float', 'floatIntrinsics', 'floatArr']
opt_flags = list(data.keys())

# Create subplots
fig, axs = plt.subplots(nrows=1, ncols=3, figsize=(18, 5))

# Plot each function in a separate subplot
for i, func in enumerate(functions):
    times = [data[opt][func] for opt in opt_flags]
    axs[i].bar(opt_flags, times, color=['b', 'g', 'r', 'c', 'm'])
    axs[i].set_title(func)
    axs[i].set_xlabel('Optimization Flag')
    axs[i].set_ylabel('Performance Time')
    axs[i].tick_params(axis='x', rotation=45)

# Adjust layout
fig.suptitle('Performance by Function Name')
fig.tight_layout(rect=[0, 0.03, 1, 0.95])

plt.savefig("./plotsImages/groupedByFuncName.png")
plt.show()
