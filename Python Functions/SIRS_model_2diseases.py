
# coding: utf-8

# In[4]:


import random

def Initialize():
    neighbourhood = ((-1,-1), (-1,0), (-1,1), (0,-1), (0, 1), (1,-1), (1,0), (1,1))
    S, A, B, AB, a, b, Ab, aB, ab = 0, 1, 2, 3, 4, 5, 6, 7, 8

def make_site_set(length,width):
    s = set()
    for i in range(0,length):
        for j in range(0,width):
            s.add(tuple([i,j]))
            
    return s

def health(matrix,a_count,b_count):
    import numpy as np
    from copy import deepcopy

    immune_list = deepcopy(immune)
    for ind in immune_list:
        if (matrix[ind[0],ind[1]] == ab):
            d = np.random.choice([a, b], p=(0.5, 0.5))
            if np.random.random() < r:
                if d == a:
                    a_count += 1
                elif d == b:
                    b_count += 1
                matrix[ind[0],ind[1]] = d
                    
        elif ((matrix[ind[0],ind[1]] == a) or ( matrix[ind[0],ind[1]] == b)):
            if np.random.random() < r:
                matrix[ind[0],ind[1]] = S
                immune.remove(ind)

def lightning(matrix,susceptibles):   
    import random
    import numpy as np
    
    for _ in range(1):
        seed = random.sample(susceptibles,1)[0]
        
        matrix[seed[0],seed[1]] = np.random.choice([AB,A,B])
        sicks.add(seed)

def SIRS_Model_2darray(mat,sites,Probabilities):#prime
    import numpy as np
    from copy import deepcopy
    
    outbreak, m_outbreak, h = Probabilities[0], Probabilities[1], Probabilities[2]
    r, f = Probabilities[3], Probabilities[4]
    
    a_count, b_count, ab_count = 0, 0, 0
    A_count, B_count, AB_count = 0, 0, 0
    aB_count, Ab_count = 0, 0
    
    new_mat = np.copy(mat)
    sicks_list = deepcopy(sicks)
    
    for sick in sicks_list:
        
        ############### AB
        if mat[sick[0],sick[1]] == AB:
            
            if np.random.random() < h:
                c = np.random.choice([Ab,aB],p=(0.5,0.5))
                new_mat[sick[0],sick[1]] = c
            
            for dx,dy in neighbourhood:
                neig = [(sick[0]+dy)%length,(sick[1]+dx)%width]
                
                if (mat[neig[0],neig[1]] == AB or mat[neig[0],neig[1]] == Ab
                    or mat[neig[0],neig[1]] == aB or mat[neig[0],neig[1]] == ab):
                    
                    continue
                    
                elif mat[neig[0],neig[1]] == A or mat[neig[0],neig[1]] == B:
                    if np.random.random() < m_outbreak:
                        new_mat[neig[0],neig[1]] = AB
                        AB_count += 1
                    continue
                    
                elif mat[neig[0],neig[1]] == a:
                    if np.random.random() < m_outbreak:
                        new_mat[neig[0],neig[1]] = aB
                        aB_count += 1
                        if (tuple([neig[0],neig[1]]) in immune):
                            immune.remove(tuple([neig[0],neig[1]]))
                        sicks.add(tuple([neig[0],neig[1]]))
                    continue
                        
                elif mat[neig[0],neig[1]] == b:
                    if np.random.random() < m_outbreak:
                        new_mat[neig[0],neig[1]] = Ab
                        Ab_count += 1
                        if (tuple([neig[0],neig[1]]) in immune):
                            immune.remove(tuple([neig[0],neig[1]]))
                        sicks.add(tuple([neig[0],neig[1]]))
                    continue
                
                elif mat[neig[0],neig[1]] == S:
                    if np.random.random() < outbreak:
                        c = np.random.choice([A,B],p=(0.5,0.5))
                        if c == A:
                            A_count += 1
                        elif c == B:
                            B_count += 1
                        new_mat[neig[0],neig[1]] = c
                        sicks.add(tuple([neig[0],neig[1]]))
                    continue
                    
                    
        ############### Ab or A
        elif (mat[sick[0],sick[1]] == Ab or mat[sick[0],sick[1]] == A):
            
            if np.random.random() < h:
                sicks.remove(tuple([sick[0],sick[1]]))
                immune.add(tuple([sick[0],sick[1]]))
                
                if mat[sick[0],sick[1]] == Ab:
                    ab_count += 1
                    new_mat[sick[0],sick[1]] = ab
                elif mat[sick[0],sick[1]] == A:
                    a_count += 1
                    new_mat[sick[0],sick[1]] = a
                    
                
            
            for dx,dy in neighbourhood:
                neig = [(sick[0]+dy)%length,(sick[1]+dx)%width]
                
                if (mat[neig[0],neig[1]] == AB or mat[neig[0],neig[1]] == Ab
                    or mat[neig[0],neig[1]] == aB or mat[neig[0],neig[1]] == ab
                    or mat[neig[0],neig[1]] == A or mat[neig[0],neig[1]] == a):
                    continue
                        
                elif mat[neig[0],neig[1]] == B:
                    if np.random.random() < m_outbreak:
                        AB_count += 1
                        new_mat[neig[0],neig[1]] = AB
                    continue
                    
                elif mat[neig[0],neig[1]] == b:
                    if np.random.random() < m_outbreak:
                        Ab_count += 1
                        new_mat[neig[0],neig[1]] = Ab
                        if (tuple([neig[0],neig[1]]) in immune):
                            immune.remove(tuple([neig[0],neig[1]]))
                        sicks.add(tuple([neig[0],neig[1]]))
                    continue
                    
                elif mat[neig[0],neig[1]] == S:
                    if np.random.random() < outbreak:
                        A_count += 1
                        new_mat[neig[0],neig[1]] = A
                        sicks.add(tuple([neig[0],neig[1]]))
                    continue
    
        ############### aB or B
        elif (mat[sick[0],sick[1]] == aB or mat[sick[0],sick[1]] == B):

            if np.random.random() < h:
                sicks.remove(tuple([sick[0],sick[1]]))
                immune.add(tuple([sick[0],sick[1]]))
                
                if mat[sick[0],sick[1]] == aB:
                    ab_count += 1
                    new_mat[sick[0],sick[1]] = ab
                elif mat[sick[0],sick[1]] == B:
                    b_count += 1
                    new_mat[sick[0],sick[1]] = b
                    

            
            for dx,dy in neighbourhood:
                neig = [(sick[0]+dy)%length,(sick[1]+dx)%width]
                
                if (mat[neig[0],neig[1]] == AB or mat[neig[0],neig[1]] == Ab
                    or mat[neig[0],neig[1]] == aB or mat[neig[0],neig[1]] == ab
                    or mat[neig[0],neig[1]] == B or mat[neig[0],neig[1]] == b):
                    continue
                        
                elif mat[neig[0],neig[1]] == A:
                    if np.random.random() < m_outbreak:
                        AB_count += 1
                        new_mat[neig[0],neig[1]] = AB
                    continue
                    
                elif mat[neig[0],neig[1]] == a:
                    if np.random.random() < m_outbreak:
                        aB_count += 1
                        new_mat[neig[0],neig[1]] = aB
                        if(tuple([neig[0],neig[1]]) in immune):
                            immune.remove(tuple([neig[0],neig[1]]))
                        sicks.add(tuple([neig[0],neig[1]]))
                    continue
                    
                elif mat[neig[0],neig[1]] == S:
                    if np.random.random() < outbreak:
                        B_count += 1
                        new_mat[neig[0],neig[1]] = B
                        sicks.add(tuple([neig[0],neig[1]]))
                    continue 
    health(new_mat,a_count,b_count)
    
    if np.random.random() < f:
        susceptibles = deepcopy(sites)
        for s in sicks:
#             print(s)
            susceptibles.remove(s)
        for i in immune:
            susceptibles.remove(i)
        lightning(new_mat,susceptibles)
        
    return (new_mat, [a_count, b_count, ab_count, A_count, B_count, AB_count, aB_count, Ab_count])


# In[5]:


def make_adjmat(List):
    
    import numpy as np    

    AdjList = []
    if len(list(List)[0]) == 2:
        l = int(np.sqrt(len(List)))
        for j in List:
            neighbors = []
            for i in List[j]:
                neighbors.append(l * i[0] + i[1])
            AdjList.append(neighbors)
    else:
        for j in List:
             AdjList.append(List[j])
                
    return np.array(AdjList).astype(int)



def SIRS_Model_AdjMatrix(AdjMat, states, Probabilities):
	
    '''
    ----- Parameters

     	states: list of state of each indivisual
	Probabilities: list of (outbreak(p), m_outbreak(q), h, r, f)
    '''

    import numpy as np
    from copy import deepcopy
    import networkx as nx

    #adj = nx.to_dict_of_lists(Graph)
    #AdjMat = make_adjmat(adj)

    outbreak, m_outbreak, h = Probabilities[0], Probabilities[1], Probabilities[2]
    r, f = Probabilities[3], Probabilities[4]
    
    new_states = deepcopy(states)
    for sick_place in (np.where((states&8)|(states&1))[0]):
        neigh_places = AdjMat[sick_place]
        ##### AB
        if (bool(states[sick_place]&8) and bool(states[sick_place]&1)) :
            if np.random.random() < h:
                new_states[sick_place] = np.random.choice([(states[sick_place]^5),(states[sick_place]^10)])
                       
            for neigh in neigh_places:
                if (bool(states[neigh]&8) and not(states[neigh]&1) and not(states[neigh]&4)):# A
                    if np.random.random() < m_outbreak:
                        new_states[neigh] = deepcopy(states[neigh] | states[sick_place])
                elif (bool(states[neigh]&1) and not(states[neigh]&8) and not(states[neigh]&2)):# B
                    if np.random.random() < m_outbreak:
                        new_states[neigh] = deepcopy(states[neigh] | states[sick_place])
                elif (bool(states[neigh]&4) and not(states[neigh]&2) and not(states[neigh]&8)):# b
                    if np.random.random() < m_outbreak:
                        new_states[neigh] = deepcopy((states[neigh] | states[sick_place])^1)
                elif (bool(states[neigh]&2) and not(states[neigh]&1) and not(states[neigh]&4)):# a
                    if np.random.random() < m_outbreak:
                        new_states[neigh] = deepcopy((states[neigh] | states[sick_place])^8)
                elif (not(states[neigh]^0)):# S
                    if np.random.random() < outbreak:
                        new_states[neigh] = deepcopy(np.random.choice([(states[sick_place]&8),(states[sick_place]&1)]))
        ##### A or Ab                                               
        elif (bool(states[sick_place]&8) and (not(states[sick_place]&1))):
            if np.random.random() < h:
                if states[sick_place]&4:
                    new_states[sick_place] = states[sick_place]>>1
                else:
                    new_states[sick_place] = states[sick_place]>>2
            
            for neigh in neigh_places:
                if (bool(states[neigh]&4) and not(states[neigh]&2) and not(states[neigh]&8)):# b
                    if np.random.random() < m_outbreak:
                        new_states[neigh] = deepcopy((states[neigh] | states[sick_place]))
                elif (bool(states[neigh]&1) and not(states[neigh]&8) and not(states[neigh]&2)):# B
                    if np.random.random() < m_outbreak:
                        new_states[neigh] = deepcopy((states[neigh] | states[sick_place])&9)                                    
                elif (not(states[neigh]^0)):# S
                        if np.random.random() < outbreak:
                            new_states[neigh] = deepcopy(states[sick_place]&8)
        ##### B or aB                                                
        elif (bool(states[sick_place]&1) and (not(states[sick_place]&8))):
            if np.random.random() < h:
                if states[sick_place]&2:
                    new_states[sick_place] = states[sick_place]<<1
                else:
                    new_states[sick_place] = states[sick_place]<<2
            
            for neigh in neigh_places:
                if (bool(states[neigh]&2) and not(states[neigh]&1) and not(states[neigh]&4)):# a
                    if np.random.random() < m_outbreak:
                        new_states[neigh] = deepcopy((states[neigh] | states[sick_place]))
                elif (bool(states[neigh]&8) and not(states[neigh]&1) and not(states[neigh]&4)):# A
                    if np.random.random() < m_outbreak:
                        new_states[neigh] = deepcopy((states[neigh] | states[sick_place])&9)                                         
                elif (not(states[neigh]^0)):#S
                        if np.random.random() < outbreak:
                            new_states[neigh] = deepcopy(states[sick_place]&1)                                         
                                                         
    for immune_place in (np.where((states&4)|(states&2))[0]):
        ##### ab
        if (bool(states[immune_place]&4) and bool(states[immune_place]&2)):
            if np.random.random() < r:
                new_states[immune_place] = deepcopy(np.random.choice([((states[immune_place]>>2)<<1),((states[immune_place]>>2)<<2)]
                                                                          ,p=(0.5,0.5)))
        ##### a or b
        else:
            if np.random.random() < r:
                new_states[immune_place] = deepcopy(states[immune_place]>>4) 
                         
    if np.random.rand() < f:
        position = np.random.choice(np.where(new_states==0)[0])
        new_states[position] = np.random.choice([new_states[position]|8,new_states[position]|9,new_states[position]|1])
                         
    return new_states

