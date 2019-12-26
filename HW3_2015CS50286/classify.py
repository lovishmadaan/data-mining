
# coding: utf-8

# In[2]:


import numpy as np
import networkx as nx
from networkx.algorithms import isomorphism
from subprocess import call
from multiprocessing import Pool
from scipy import stats
import sys


# In[3]:


def process(inputfile, outputfile, nodes=None):
    g_list, lines, indices = {}, [], []

    if nodes:
        with open(nodes, 'r') as fr:
            for line in fr:
                num = line.strip()
                g_list[num] = num

        fr.close()

    lines = []
    with open(inputfile, 'r') as fr:
        for line in fr:
            line = line.strip().split()
            lines.append(line)

    fr.close()

    index = 0
    l = len(lines)
    count = 0

    with open(outputfile, 'w') as fw:
        flag = False
        while index < l:
            if lines[index][0][1 : ] in g_list or not nodes:
                indices.append(count)
                flag = True
            
            count += 1
            
            if flag:
                fw.write(f't # {lines[index][0][1 : ]}\n')
            
            index += 1
            vert = int(lines[index][0])
            index += 1
            count = 0
            v_map = {}
            for i in range(vert):
                if flag and lines[index][0] != 'H':
                    fw.write(f'v {i} {lines[index][0]}\n')
                    v_map[i] = count
                    count += 1
                index += 1
            edges = int(lines[index][0])
            index += 1
            for i in range(edges):
                if flag and int(lines[index][0]) in v_map and int(lines[index][1]) in v_map:
                    fw.write(f'u {v_map[int(lines[index][0])]} {v_map[int(lines[index][1])]} {lines[index][2]}\n')
                index += 1

            flag = False

    fw.close()

    return indices


# In[4]:


def write_featuresy(filename, X, y):
    with open(filename, 'w') as fw:
        for i in range(X.shape[0]):
            fw.write(f'{y[i]} ')
            for j in range(X.shape[1]):
                fw.write(f'{j}:{X[i][j]} ')
            fw.write('\n')

    fw.close()

def write_features(filename, X):
    with open(filename, 'w') as fw:
        for i in range(X.shape[0]):
            for j in range(X.shape[1]):
                fw.write(f'{j}:{X[i][j]} ')
            fw.write('\n')

    fw.close()


# In[5]:


def read_subgraphs(inputfile):
    g_list = []
    i, num_t = 0, -1
    with open(inputfile, 'r') as fr:
        for line in fr:
            i += 1
            line = line.strip().split()
            if i == 6:
                num_t = int(line[5])

            if line[0] == 't':
                g_list.append(nx.Graph())
            elif line[0] == 'v':
                g_list[-1].add_node(int(line[1]), label=line[2])
            elif line[0] == 'u':
                g_list[-1].add_edge(int(line[1]), int(line[2]), label=line[3])
    
    return g_list, num_t


# In[6]:


def read_isomorphism_res(file):
    tidl = []
    with open(file, 'r') as fr:
        for line in fr:
            line = line.strip().split()
            tidl.append([])
            for num in line:
                tidl[-1].append(int(num))
    
    return tidl
    fr.close()


# In[7]:


def write_subgraphs(outfile, subgraphs):
    with open(outfile, 'w') as fr:
        for g in subgraphs:
            fr.write(f't\n')
            for v in g.nodes:
                v_label = g.nodes[v]['label']
                fr.write(f'v {v} {v_label}\n')
            for u, v in g.edges:
                edge_label = g.edges[u, v]['label']
                fr.write(f'u {u} {v} {edge_label} \n')
    
    fr.close()


# In[8]:


graph_file = sys.argv[1]
ca_file = sys.argv[2]
ci_file = sys.argv[3]
test_file = sys.argv[4]

indices_active = process(graph_file, 'active_out.txt', ca_file)
indices_inactive = process(graph_file, 'inactive_out.txt', ci_file)

print(len(indices_active))
print(len(indices_inactive))


# In[38]:


sup_ac, sup_ic = 10, 10

if len(indices_active) < 600:
    sup_ac = (60 / (len(indices_active))) * 100
    sup_ic = (60 / (len(indices_active))) * 65

pool = Pool(2)
pool.map(call, [['./fsg', 'active_out.txt', '-s', str(sup_ac)],
                ['./fsg', 'inactive_out.txt', '-s', str(sup_ic)]])
pool.close()
pool.join()


# In[39]:


g_list_acs, num_ac = read_subgraphs('active_out.fp')


# In[40]:


# In[41]:


call(['./getFT', 'inactive_out.fp', 'active_out.fp', 'prune_acs_ics_ft.txt'])


# In[42]:


prune_acs_ics = np.int32(read_isomorphism_res('prune_acs_ics_ft.txt'))


# In[43]:


prune_count_ac = np.sum(prune_acs_ics, axis=1)
prune_g_list_acs, prune_g_list_ac_ind = [], []
for i in range(len(g_list_acs)):
    if prune_count_ac[i] == 0:
        prune_g_list_acs.append(g_list_acs[i])
        prune_g_list_ac_ind.append(i)

print(len(prune_g_list_acs))


# In[44]:


write_subgraphs('active_subgraphs.txt', prune_g_list_acs)


# In[45]:


# In[46]:


if len(prune_g_list_acs) > 1500:
    print('Gone here ******************')
    call(['./getFT', 'active_subgraphs.txt', 'active_subgraphs.txt', 'prune_acs_acs.txt'])
    prune_acs_acs = np.int32(read_isomorphism_res('prune_acs_acs.txt'))
    prune_g_list_acs_more_ind = np.zeros(len(prune_g_list_acs), dtype=int)

    for i in range(prune_acs_acs.shape[0]):
        for j in range(prune_acs_acs.shape[1]):
            if i != j and prune_acs_acs[i][j] == 1:
                prune_g_list_acs_more_ind[j] = 1

    prune_g_list_acs_more = []
    for i in range(len(prune_g_list_acs)):
        if prune_g_list_acs_more_ind[i] == 0:
            prune_g_list_acs_more.append(prune_g_list_acs[i])

    len(prune_g_list_acs_more)
else :
    prune_g_list_acs_more = prune_g_list_acs


# In[47]:

print(len(prune_g_list_acs_more))


write_subgraphs('active_subgraphs_more.txt', prune_g_list_acs_more)
pool = Pool(2)
pool.map(call, [['./getFT', 'inactive_out.txt', 'active_subgraphs_more.txt', 'tidl_acs_ic_ft.txt'], 
                         ['./getFT', 'active_out.txt', 'active_subgraphs_more.txt', 'tidl_ac_ft.txt']])
pool.close()
pool.join()


# In[48]:


tidl_acs_ic = read_isomorphism_res('tidl_acs_ic_ft.txt')
tidl_ac = read_isomorphism_res('tidl_ac_ft.txt')


# In[49]:


tidl_ac = np.array(tidl_ac, dtype=int)
tidl_ic = np.array(tidl_acs_ic, dtype=int)


# In[50]:



# In[51]:


feature_subgraphl = prune_g_list_acs_more


# In[52]:


write_subgraphs('feature_subg.txt', feature_subgraphl)


# In[53]:


process(test_file, 'test_graph.txt')


# In[54]:


call(['./getFT', 'test_graph.txt', 'feature_subg.txt', 'ft_test_ic.txt'])


# In[55]:


ft_test_ic = np.array(read_isomorphism_res('ft_test_ic.txt'), dtype=int).T


# In[56]:



# In[57]:





# In[58]:

X = np.hstack((tidl_ac, tidl_ic)).T
y = np.hstack((np.ones(tidl_ac.shape[1], dtype=int),  -1 * np.ones(tidl_ic.shape[1], dtype=int)))

print(len(y[y<0]))

write_featuresy('train.txt', X, y)
write_features('test.txt', ft_test_ic)






# from svmutil import *
# prob  = svm_problem(y.tolist(), X[:, :].tolist())
# param = svm_parameter('-t 0')
# m = svm_train(prob, param)


# # In[62]:


# (ft_test_ic[:, :]).shape


# # In[63]:


# labels, _, _ = svm_predict([0] * len(ft_test_ic), (ft_test_ic[:, :]).tolist(), m)
# len(labels)


# # In[64]:


# count = 0
# for i in range(len(labels)):
#     if labels[i] < 0.5:
#         count += 1
# count


# # In[65]:


# labels, _, _ = svm_predict([0] * len(ft_test_ac), (ft_test_ac[:, :]).tolist(), m)
# len(labels)


# # In[66]:


# count = 0
# for i in range(len(labels)):
#     if labels[i] > 0.5:
#         count += 1
# count

