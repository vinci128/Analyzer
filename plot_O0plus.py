import matplotlib.pyplot as plt
from matplotlib import rc
import numpy as np
import re
import os

#rc('font',**{'family':'sans-serif','sans-serif':['Helvetica']})
#rc('text', usetex=True)

n_op = 3
smear = 5

def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

h = file_len('O0plus_results.dat')

print(h)

os.makedirs('O0plus', exist_ok=True)

a =0

biarray = [[0 for x in range(25)] for y in range(h)]

with open('O0plus_results.dat','r') as f:
    for line in f:
    #    print(line.split())
        array = line.split()
        print(array)
        Nt = int(re.search(r'(.*)Nt(.*)_Nx',array[0]).group(2))

        w = 2*Nt+3
        #n_smear = int(array[1])

        for i in range(2,w):
            array[i] = float(array[i])

    #    print(array)
        for j in range(w):
            biarray[a][j] = array[j]
        a+= 1


for k in range(n_op):
    fig1 = plt.figure()
    figm = plt.figure()

    i = k*smear

    Op_type = biarray[i][1]

    Nt = int(re.search(r'(.*)Nt(.*)_Nx',biarray[i][0]).group(2))
    B = re.search(r'(.*)B(.*)_K',biarray[i][0]).group(2)
    K = re.search(r'(.*)K(.*)_L',biarray[i][0]).group(2)
    L = re.search(r'(.*)L(.*).bin',biarray[i][0]).group(2)

    O0plus_path = 'O0plus/Nt'+str(Nt)+'B'+str(B)+'K'+str(K)+'L'+str(L)+'Op_'+str(Op_type)+'.png'
    O0plus_m_path = 'O0plus/M_eff_Nt'+str(Nt)+'B'+str(B)+'K'+str(K)+'L'+str(L)+'Op_'+str(Op_type)+'.png'

    for j in range(smear):
        i = j +smear*k

        Tl = list(range(Nt))
        C = list(range(Nt))
        C_unc = list(range(Nt))
        Tlm = list(range(Nt-1))
        m = list(range(Nt-1))
        m_unc = list(range(Nt-1))
        n_smear = int(biarray[i][2])


        for t in range(Nt):
            C[t] = biarray[i][2*t+3]/ biarray[i][3];
        #C_unc[t] = biarray[i][2*t+4]
            C_unc[t] = np.sqrt(biarray[i][2*t+4]*biarray[i][2*t+4] +C[t]*C[t]*biarray[i][4]*biarray[i][4])/biarray[i][3]
        for t in range(Nt-1):
            m[t] = np.log(C[t]/C[t+1])
            m_unc[t] = C_unc[t]/C[t] + C_unc[t+1]/C[t+1]

        print(Nt,B,K,L)
        print('C=',C)
        print('sigma(C)=',C_unc)




        axc = fig1.add_subplot(1,1,1)

        axc.errorbar(Tl,C,yerr=C_unc, fmt='x', markersize=10, label= n_smear )
        axc.set_ylabel(r'C(t)')
        axc.set_yscale("log")
        axc.set_xlabel('t')
        axc.legend()


        print('m=',m)
        print('sigma(m)=',m_unc)


        axm = figm.add_subplot(1,1,1)

        axm.errorbar(Tlm,m,yerr=m_unc, fmt='x', markersize=10, label= n_smear)
        axm.set_ylabel(r'm(t)')
        #axc.set_yscale("log")
        axm.set_xlabel('t')
        axm.legend()


    fig1.savefig(O0plus_path, dpi=100)
    plt.close(fig1)

    figm.savefig(O0plus_m_path, dpi=100)
    plt.close(figm)
