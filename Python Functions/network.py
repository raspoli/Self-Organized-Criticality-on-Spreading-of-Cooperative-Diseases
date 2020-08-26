import networkx as nx

def make_adjmat_dict(List):
    
    import numpy as np    

    AdjList = dict()

    l1 = int(np.sqrt(len(List)))
    j = 0
    for neigh in (List):
        neighbors = []
        for i in List[neigh]:
            neighbors.append(l1 * i[0] + i[1])
        AdjList[j] = neighbors
        j += 1

    return AdjList

l = int(input('Enter lattice size\n'))
G = nx.grid_2d_graph(n=l,m=l,periodic=True)
L = nx.to_dict_of_lists(G)
AdjMat = make_adjmat_dict(L)

f = open("AdjList"+str(l)+"lattice.txt","w")
for v in AdjMat.values():
   f.write(str(v)+"\n")
f.close()
