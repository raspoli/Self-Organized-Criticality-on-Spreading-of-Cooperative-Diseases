import numpy as np
import pandas as pd
from matplotlib import pyplot as plt

def MakeDist(data):
#     cleaning data by chose seed
    import numpy as np
    
    unique, counts = np.unique(data[(data!=0)], return_counts=True)

    basket = list(np.arange(1,11))
    x = 1
    a = int(10 * np.power(1.2543,x))
    while(a < np.max(unique)):
        basket.append(a)
        x += 1 
        a = int(10 * np.power(1.2543,x))
    basket_1 = [0] + basket
    basket.append(np.max(unique))

    dist = [] 
    for seed in zip(basket_1,basket):
        range_counts = counts[(unique >seed[0]) & (unique <= seed[1])]
        mean = np.sum(range_counts) / (seed[1] - seed[0])
        std = np.sqrt(np.sum(np.square(range_counts - mean)) / (range_counts.shape[0]))
        dist.append([mean,std])
        
    result = np.concatenate((np.array(basket).reshape(len(basket),1),np.array(dist)),axis = 1)

    return result[~np.isnan(result).any(axis=1)]




size = [32,64,128,256,512,1024]

path = input("enter deta path \n")


data = pd.read_csv(path, header = None)
data = data.drop(8,axis=1)

A = MakeDist(data[0])
B = MakeDist(data[1])


#coefA = np.polyfit(np.log10(A[11:-10,0]),np.log10(A[11:-10,1]),1)
#coefB = np.polyfit(np.log10(B[11:-10,0]),np.log10(B[11:-10,1]),1)

fig,axs = plt.subplots(1,2)
axs[0].set_title("A_cluster")
axs[0].plot(np.log10(A[:,0]),np.log10(A[:,1] / np.sum(A[:,1])),"o--")

axs[0].set_xlabel("s")
axs[0].set_ylabel("P(s)")


axs[1].set_title("B_cluster")
axs[1].plot(np.log10(B[:,0]),np.log10(B[:,1] / np.sum(B[:,1])),"o--")

axs[1].set_xlabel("s")
axs[1].set_ylabel("P(s)")


plt.show()

