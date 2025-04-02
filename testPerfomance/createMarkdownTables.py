from matplotlib import pyplot
from pandas import DataFrame
from tabulate import tabulate

def main():
    # data = {
    #     '-Ofast': {
    #         'float': 2431.330322,
    #         'floatIntrinsics': 314.371002,
    #         'floatArr': 206.541214
    #     },
    #     '-O3': {
    #         'float': 2546.723877,
    #         'floatIntrinsics': 329.517426,
    #         'floatArr': 213.129440
    #     },
    #     '-O2': {
    #         'float': 2544.549561,
    #         'floatIntrinsics': 333.004608,
    #         'floatArr': 3592.113281
    #     },
    #     '-O1': {
    #         'float': 2480.524658,
    #         'floatIntrinsics': 328.854340,
    #         'floatArr': 3064.068848
    #     },
    #     'no opt flags': {
    #         'float': 4918.506836,
    #         'floatIntrinsics': 2724.700928,
    #         'floatArr': 16347.489258
    #     }
    # }
    #     arr = dict()
    #     for key in data['-O2']:
    #         arr[key] = dict()
    #     for key, val in data.items():
    #         for key2, val2 in val.items():
    #             print(key2, val2, arr[key2])
    #             arr[key2][key] = val2
    #
    #     for key, val in arr.items():
    #         print(f"     '{key}': ", "{")
    #         for key2, val2 in val.items():
    #             print(f"         '{key2}': {val2:.2f},")
    #         print("     },")


    data = {
        'Optimization flags': [
            '-Ofast',
            '-O3',
            '-O2',
            '-O1',
            'no opt flags'
        ],
        'float':  [
            2431,
            2546,
            2544,
            2480,
            4918,
        ],
        'floatIntrinsics':  [
            314,
            329,
            333,
            328,
            2724,
        ],
        'floatArr':  [
            180,
            187,
            3149,
            3515,
            16328,
        ],
    }

    dispersionTable = {
        '-Ofast': [
            19.16,
            8.383911,
            7.928277,
        ],
        '-O3': [
            8.028677,
            6.447448,
            4.512328,
        ],
        '-O2': [
            49.054939,
            5.936242,
            53.228645,
        ],
        '-O1': [
            64.678436,
            2.331454,
            28.810362,
        ],
        'no opt flags': [
            36.559334,
            4.805051,
            210.500427,
        ]
    }

    dataWithDisp = dict()
    ind = 0
    for key, val in data.items():
        if key[:3] == 'Opt':
            dataWithDisp[key] = val
            continue

        dataWithDisp[key] = []
        for ind2, val2 in enumerate(val):
            print(ind, ind2)
            key2 = data['Optimization flags'][ind2]
            print(key2, ind, val2, dispersionTable[key2])
            #print(key, ind, val2, key2, dispersionTable[key2])

            err = str(int(dispersionTable[key2][ind]))
            dataWithDisp[key].append(str(val2) + " &#177; " + err)
        #dataWithDisp[key] = [str(val2) + str(dispersionTable[key][ind]) for ind, val2 in enumerate(val)]
        ind += 1

    dataRelCol = dict()
    for key, val in data.items():
        if key[:3] == "Opt":
            dataRelCol[key] = val[:-1]
            continue

        # print(key, val)
        dataRelCol[key] = [ round(val[-1] / t, 2) for t in val[:-1]]

    dataRelRow = dict()
    for key, val in data.items():
        if key == "float":
            continue

        if key[:3] == 'Opt':
            dataRelRow[key] = val
        else:
            #print(key, data["float"][0], val)
            #print(list(enumerate(val)))
            dataRelRow[key] = [ round(data["float"][ind] / t, 2) for ind, t in list(enumerate(val)) ]

    print(dataWithDisp)
    df = DataFrame(dataWithDisp).set_index("Optimization flags")
    print(tabulate(df, tablefmt="pipe", headers="keys", numalign='left'))

    print()
    df = DataFrame(dataRelCol).set_index("Optimization flags")
    print(tabulate(df, tablefmt="pipe", headers="keys", numalign='left'))

    print()
    df = DataFrame(dataRelRow).set_index("Optimization flags")
    print(tabulate(df, tablefmt="pipe", headers="keys", numalign='left'))


if __name__ == "__main__":
    main()
