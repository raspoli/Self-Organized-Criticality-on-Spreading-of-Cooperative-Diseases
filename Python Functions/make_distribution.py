def MakeDist(data):
#     cleaning data by chose seed
    import numpy as np
    
    unique, counts = np.unique(data[(data!=0)], return_counts=True)

    basket = list(np.arange(1,10))
    x = 13
    a = int(np.power(1.2543,x))
    while(a < np.max(unique)):
        basket.append(a)
        x += 1 
        a = int(np.power(1.2543,x))
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