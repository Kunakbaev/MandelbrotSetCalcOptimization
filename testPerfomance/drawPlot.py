import matplotlib.pyplot as plt
import sys
import numpy as np

PLOTS_DIR = "plotsImages"

def main():
    ax = plt.gca()
    ax.set_facecolor("black")

    plt.xlabel("unroll batch size")
    plt.ylabel("time")
    plt.grid(color='w', linestyle='-', linewidth=0.3)

    arr = [
        [16, 1511.334717],
        [20, 234.272888],
        [24, 179.718277],
        [28, 200.442352],
        [32, 210.692871],
        [36, 256.747375],
        [40, 235.421631],
        [44, 261.924194],
        [48, 250.377792],
        [52, 271.956665],
        [56, 259.279053]
    ]

    arr = arr[1:]

    data = np.transpose(arr)
    plt.plot(data[0], data[1], 'go--')

    plt.title("different unrolling batch sizes")
    #plt.legend(names, loc="upper left")
    filePath = "testPerfomance/" + PLOTS_DIR + "/" + "unrollBatchSizes" + ".png"
    print(filePath)
    plt.savefig(filePath)
    plt.show()

if __name__ == "__main__":
    main()
