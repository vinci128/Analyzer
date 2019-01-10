import matplotlib.pyplot as plt
from matplotlib import rc
import numpy as np
import re
import os

rc('font',**{'family':'sans-serif','sans-serif':['Helvetica']})
#rc('text', usetex=True)

def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

h = file_len('O1minus_results.dat')

os.makedirs('O1minus', exist_ok=True)

a =0

biarray = [[0 for x in range(25)] for y in range(h)]

with open('O1minus_results.dat','r') as f:
    for line in f:
    #    print(line.split())
        array = line.split()
        print(array)
        Nt = int(re.search(r'(.*)Nt(.*)_B',array[0]).group(2))

        w = 2*Nt+2
        n_smear = int(array[1])

        for i in range(2,w):
            array[i] = float(array[i])

    #    print(array)
        for j in range(w):
            biarray[a][j] = array[j]
        a+= 1


for i in range(h):

    Nt = int(re.search(r'(.*)Nt(.*)_B',biarray[i][0]).group(2))
    B = re.search(r'(.*)B(.*)_K',biarray[i][0]).group(2)
    K = re.search(r'(.*)K(.*)_L',biarray[i][0]).group(2)
    L = re.search(r'(.*)L(.*).bin',biarray[i][0]).group(2)

    Tl = list(range(Nt))
    C = list(range(Nt))
    C_unc = list(range(Nt))

    for t in range(Nt):
        C[t] = biarray[i][2*t+2]
        C_unc[t] = biarray[i][2*t+3]

    print(Nt,B,K,L)
    print('C=',C)
    print('sigma(C)=',C_unc)

    fig1 = plt.figure()
    axc = fig1.add_subplot(1,1,1)
    O1minus_path = 'O1minus/B'+str(B)+'K'+str(K)+'L'+str(L)+'n_sm'+str(n_smear)+'.png'
    axc.errorbar(Tl,C,yerr=C_unc, fmt='x', markersize=10)
    axc.set_ylabel(r'C(t)')
#    axc.set_yscale("log")
    axc.set_xlabel('t')
    fig1.savefig(O1minus_path, dpi=100)
    plt.close(fig1)
