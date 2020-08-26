
# coding: utf-8

# In[9]:


def MakeDist(datas):
    
    import numpy as np
    
    count_list = []
    maximum = max(datas)
    for i in range(1,maximum+1):
        n = list(datas).count(i)
        if n != 0:
            count_list.append([i,n])
        else:
            count_list.append([i,0])

    basket = [0]
    x = 6
    count_list = np.transpose(count_list)

    while(1):
        a = int(1.2**x)
        if a >= maximum:
            basket.append(maximum)
            break
        basket.append(a)
        x += 1

    new_data = []
    for i in range(len(basket)-1):
        mean = np.mean(count_list[1,basket[i]:basket[i+1]])
        if basket[i] == 0:
            basket[i] = 1
        new_data.append([basket[i],mean])

    final_data = np.transpose(new_data)

    return final_data

