import matplotlib.pyplot as plt
import csv
import os
import numpy as np
  

fileNames = ['10000000wordsDense.csv'  ,'10000000wordsSparse.csv',
     '50000000wordsDense.csv'  , '50000000wordsSparse.csv',
     '100000000wordsDense.csv' ,'100000000wordsSparse.csv',
     '1000000000wordsDense.csv', '1000000000wordsSparse.csv']


fileSize = [10000000, 50000000, 100000000, 1000000000]

normalizedFileSize = []
value = 0
for item in fileSize:
    value = item / 10000000
    normalizedFileSize.append(value)

totalTime = []

partialTime = 0
items = 0

fig, axes = plt.subplots()

Chunk1path10Cores = os.getcwd() + "\\results\encodingOutputsParallel\\1 chunk\\1 cpu 10 cores"
Chunk1path20Cores = os.getcwd() + "\\results\encodingOutputsParallel\\1 chunk\\1 cpu 20 cores"
Chunk1path40Cores = os.getcwd() + "\\results\encodingOutputsParallel\\1 chunk\\1 cpu 40 cores"
Chunk1path50Cores = os.getcwd() + "\\results\encodingOutputsParallel\\1 chunk\\1 cpu 50 cores"

chunk2PackExcl10Cores = os.getcwd() + "\\results\encodingOutputsParallel\\2 chunks pack excl\\10 cores"
chunk2PackExcl20Cores = os.getcwd() + "\\results\encodingOutputsParallel\\2 chunks pack excl\\20 cores"


chunk2ScatterExcl10Cores = os.getcwd() + "\\results\encodingOutputsParallel\\2 chunks scatter excl\\10 cores"
chunk2ScatterExcl20Cores = os.getcwd() + "\\results\encodingOutputsParallel\\2 chunks scatter excl\\20 cores"


chunk2Scatter10Cores = os.getcwd() + "\\results\encodingOutputsParallel\\2 chunks scatter\\10 cores"
chunk2Scatter20Cores = os.getcwd() + "\\results\encodingOutputsParallel\\2 chunks scatter\\20 cores"
chunk2Scatter40Cores = os.getcwd() + "\\results\encodingOutputsParallel\\2 chunks scatter\\40 cores"
chunk2Scatter50Cores = os.getcwd() + "\\results\encodingOutputsParallel\\2 chunks scatter\\50 cores"

serialPath = os.getcwd() + "\\results\serialResults"

'''10 CORES'''
for filename in fileNames:
    if "Dense" in filename:
        with open(os.path.join(Chunk1path10Cores,filename), 'r') as csvfile:
            plots = csv.reader(csvfile, delimiter = ',')
            next(plots, None)

            for row in plots:
                partialTime += float(row[3])
                items += 1

            partialTime = partialTime / items
            #partialTime = partialTime / 60
            print(partialTime)
            items = 0
            totalTime.append(partialTime)
            partialTime = 0

axes.plot(totalTime,normalizedFileSize, label="dense data 1chunk 10 cores", linestyle=":")
totalTime = []
partialTime = 0

'''20 CORES'''
for filename in fileNames:
    if "Dense" in filename:
        with open(os.path.join(Chunk1path20Cores,filename), 'r') as csvfile:
            plots = csv.reader(csvfile, delimiter = ',')
            next(plots, None)

            for row in plots:
                partialTime += float(row[3])
                items += 1

            partialTime = partialTime / items
            #partialTime = partialTime / 60
            print(partialTime)
            items = 0
            totalTime.append(partialTime)
            partialTime = 0

axes.plot(totalTime,normalizedFileSize, label="dense data 1chunk 20 cores", linestyle=":")
totalTime = []
partialTime = 0

'''40 CORES'''
for filename in fileNames:
    if "Dense" in filename:
        with open(os.path.join(Chunk1path40Cores,filename), 'r') as csvfile:
            plots = csv.reader(csvfile, delimiter = ',')
            next(plots, None)

            for row in plots:
                partialTime += float(row[3])
                items += 1

            partialTime = partialTime / items
            #partialTime = partialTime / 60
            print(partialTime)
            items = 0
            totalTime.append(partialTime)
            partialTime = 0

axes.plot(totalTime,normalizedFileSize, label="dense data 1chunk 40 cores", linestyle=":")
totalTime = []
partialTime = 0

'''50 CORES'''
for filename in fileNames:
    if "Dense" in filename:
        with open(os.path.join(Chunk1path50Cores,filename), 'r') as csvfile:
            plots = csv.reader(csvfile, delimiter = ',')
            next(plots, None)

            for row in plots:
                partialTime += float(row[3])
                items += 1

            partialTime = partialTime / items
           # partialTime = partialTime / 60
            print(partialTime)
            items = 0
            totalTime.append(partialTime)
            partialTime = 0

axes.plot(totalTime,normalizedFileSize, label="dense data 1chunk 50 cores", linestyle=":")
totalTime = []
partialTime = 0

'''for filename in fileNames:
    if "Sparse" in filename:
        with open(os.path.join(serialPath,filename), 'r') as csvfile:
            plots = csv.reader(csvfile, delimiter = ',')
            next(plots, None)

            for row in plots:
                partialTime += float(row[3])
                items += 1

            partialTime = partialTime / items
            #partialTime = partialTime / 60
            print(partialTime)
            items = 0
            totalTime.append(partialTime)
            partialTime = 0

axes.plot(totalTime,normalizedFileSize, label="sparse data")
'''
axes.legend()
axes.set_ylabel('file size')
axes.set_xlabel('total time(seconds)')
axes.set_ylim(0,110)
axes.set_xlim(0.0, 25.0)

#axes.grid(True)
axes.set_title("ramp-up comparison of 1 chunk with different cores")

plt.savefig('ramp-up-comparison-of-1-chunk-with-different-cores')

plt.show()