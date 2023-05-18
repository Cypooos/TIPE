from matrix import Matrix, to_matrix
import math

from random import randint

def get_dist_max(mat):
    mat.data[1][1] = 1
    borne_inf = Matrix.IDENTITY(mat.x)
    borne_inf_counter = 1
    times = mat
    times_counter = 1

    NB_PROD = 0
    while True:
        NB_PROD += 1
        k = borne_inf * times
        if k.data[0][1] != 0:
            if times == mat:
                return borne_inf_counter, NB_PROD
            times = mat
            times_counter = 1
        else:
            NB_PROD += 1
            times *= times
            borne_inf_counter += times_counter
            times_counter += times_counter
            borne_inf = k


def get_dist_max_mem(mat):
    mat.data[1][1] = 1
    borne_inf = Matrix.IDENTITY(mat.x)
    borne_inf_counter = 1
    times = mat
    times_counter = 1
    times_index = 0

    saving = {}

    NB_PROD = 0

    while True:
        NB_PROD += 1
        k = borne_inf * times
        if k.data[0][1] != 0:
            if times == mat:
                return borne_inf_counter, NB_PROD
            times = mat
            times_counter = 1
            times_index = 0
        else:
            if saving.get(times_index,Matrix.NULL(mat.x)) == Matrix.NULL(mat.x):
                NB_PROD += 1
                times *= times
                saving[times_index] = times
            else:
                times = saving[times_index]
            borne_inf_counter += times_counter
            times_counter += times_counter
            borne_inf = k
            times_index += 1

def test_both(mat):
    dist,nb = get_dist_max(mat)
    dist_mem,nb_mem = get_dist_max_mem(mat)
    if dist != dist_mem:
        print("ERROR !!!")
    else:
        print(dist,nb,nb_mem)


def gen_graph(nb_s,goal_arr):
    ret = Matrix(nb_s,nb_s)
    current = 0
    nb_arr = 0
    for _ in range(goal_arr-1):
        next = randint(0,nb_s-1)
        if ret.data[current][next] == 0:
            nb_arr+=1
            ret.data[current][next]= 1
        current = next
    if ret.data[current][1] == 0:
        ret.data[current][1] = 1
        nb_arr+=1
    return ret,nb_arr

def write_csv(file):
    fp = open(file,"w+")
    fp.write("# nb_sommet, nb_arrettes, distance, nb_prod, nb_prod_dyn\n")
    for nb_s in range(2,513):
        mat = to_matrix("\n".join([str(x+1)+" " +str(x) for x in range(1,nb_s-1)]+["0 "+str(nb_s-1)]))
        dist,nb = get_dist_max(mat)
        dist_mem,nb_mem = get_dist_max_mem(mat)
        if dist != dist_mem:
            print("ERROR !!!")
            exit()
        else:
            fp.write(str(nb_s)+", "+str(nb_s-1)+", "+str(dist)+", "+str(nb)+", "+str(nb_mem)+"\n")
            print(nb_s,nb_s-1,dist,nb,nb_mem)

def draw_csv(file):
    import numpy as np
    from numpy import genfromtxt
    import matplotlib.pyplot as plt

    dtypes = [("nb_sommet",int), ("nb_arrettes",int), ("distance",int),("nb_prod",int), ("nb_prod_dyn",int)]
    data = genfromtxt(file,dtype=dtypes, comments='#', delimiter=',')
    data.sort(order="distance")

    means = {}
    for i in np.unique(data["distance"]):
        tmp = data[np.where(data["distance"] == i)]
        means[i] = (np.mean(tmp["nb_prod"]), np.mean(tmp["nb_prod_dyn"]))

    means = np.array([(k,v[0],v[1]) for k,v in means.items()],dtype=[("distance",int),("nb_prod",int),("nb_prod_dyn",int)])
    _fig, axis = plt.subplots()
    axis.set_title("Comparaison du nombre de produits avec la mémoisation", loc='left')

    axis.plot(means["distance"], means["nb_prod"], 'b-', lw=1,label="Distance classique")
    axis.plot(means["distance"], means["nb_prod_dyn"], 'orange', lw=1,label="Distance mémoisé")
    
    axis.plot(means["distance"], means["nb_prod"]-means["nb_prod_dyn"], 'green', lw=1,label="Différence")
    axis.plot(means["distance"], np.power(np.log2(means["distance"]),2), 'r:', lw=1,label="Complexité max. $(\log_2(n)^2)$")
    axis.plot(means["distance"], 2*np.log2(means["distance"]), 'y:', lw=1,label="Complexité min. $(2\\times\log_2(n))$")

    axis.set_ylabel("Nombre de produits")
    axis.set_xlabel("Distance")
    axis.legend()
    axis.grid(True)

    plt.show()


if __name__ == "__main__":
    #write_csv("algo_prod_test.csv")
    draw_csv("matrix_processor/algo_prod_test.csv")
    exit()