import matplotlib.pyplot as plt

def main():
    # plt.rcParams['figure.facecolor'] = 'black'
    # plt.rcParams['hatch.color'] = 'white'
    # plt.rcParams['grid.color'] = 'white'
    # print(plt.rcParams.keys())

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
    fig, axs = plt.subplots(nrows=1, ncols=5, figsize=(20, 5))

    # Plot each optimization flag in a separate subplot
    for i, opt in enumerate(opt_flags):
        times = [data[opt][func] for func in functions]
        # axs[i].set_facecolor("black")
        axs[i].bar(functions, times, color=['b', 'g', 'r'])
        axs[i].set_title(opt)
        axs[i].set_xlabel('Function')
        axs[i].set_ylabel('Performance Time')

    # Adjust layout
    fig.suptitle('Performance by Optimization Flag')
    fig.tight_layout(rect=[0, 0.03, 1, 0.95])

    plt.savefig("./plotsImages/groupedByOptFlag.png")
    plt.show()

if __name__ == "__main__":
    main()
