#  Python code to generate graphs from file stat_results.dat.
#  Vincenzo Afferrante 3/9/2018
#

from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import rc
import numpy as np
import re
import os

rc('font',**{'family':'sans-serif','sans-serif':['Helvetica']})
rc('text', usetex=True)


# We read from stat_results.dat, with w we indicate the number of observables, errors, and the stringline. With h we indicate the number of lines (so the number of set of parameters)

w= 11

def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

h = file_len('stat_results.dat')
print(h)

# We create a bidimensional array in which we store all the results

biarray = [[0 for x in range(w)] for y in range(h)]

a=0

with open('stat_results.dat','r') as f:
    for line in f:
    #    print(line.split())
        array = line.split()

        for i in range(1,w):
            array[i] = float(array[i])

    #    print(array)
        for j in range(w):
            biarray[a][j] = array[j]
        a+= 1

#        plt.plot([array[1], array[3], array[5]], 'ro')
#plt.errorbar([[array[1], array[3], array[5]])
#plt.axis([0, 4, 0, 1.5])
#plt.show()

w = 14

# We create another array in which we store length and parameters
# biNBKL = (phi^2, sigma(phi), hop, sigma(hop), phibar, sigma(phibar), N,B,K,L) in previous case, in general
# biNBKL = (observables, N,B,K,L)

biNBKL = [[0 for x in range(w)] for y in range(h)]

for i in range(h):
#    print(biarray[i][0])

    for j in range(w-4):
        biNBKL[i][j] = biarray[i][j+1]

    print(biarray[i][0])

    N = re.search(r'(.*)Nx(.*)Ny',biarray[i][0]).group(2)
    if N:
        #print(N)
        biNBKL[i][10] = float(N)

    B = re.search(r'(.*)B(.*)_K',biarray[i][0]).group(2)
    if B:
        #print(B)
        biNBKL[i][11] = float(B)

    K = re.search(r'(.*)K(.*)_L',biarray[i][0]).group(2)
    if K:
        #print(K)
        biNBKL[i][12] = float(K)

    L = re.search(r'(.*)L(.*).bin',biarray[i][0]).group(2)
    if L:
        #print(L)
        biNBKL[i][13] = float(L)

# Now we want to generate, for every set of parameter, array of observables and errors in function of N:
# phi_bar = (phi_bar(4),phi_bar(6),phi_bar(8),phi_bar(10),phi_bar(12))
# After we plot it to a file that contains BKL in the name

#print(biNBKL)

#we insert here the number of sets (B,k,L), for volume extraction. We need to insert the number of volumes used here
n_vol = 1

w = int(h/n_vol)

#Vol_inv = [1/4, 1/6,1/8]
#phi_2 = [0 for x in range(w)]
#phi_2_err = [0 for x in range(w)]
#hop = [0 for x in range(w)]
#hop_err = [0 for x in range(w)]
phi_bar = list(range(w))
phi_bar_err = list(range(w))
poly = list(range(w))
poly_err = list(range(w))
act = list(range(w))
act_err = list(range(w))
plaq = list(range(w))
plaq_err = list(range(w))
phi_plaq = list(range(w))
phi_plaq_err = list(range(w))
Kappa = list(range(w))
Lambda  = list(range(w))


par = [[0 for x in range(3)] for y in range(w)]

n=0
c=0

for i in range(h):
    #while n <100:
    #    par[n][0] = biNBKL[i][10]  #length N
        par[n][0] = biNBKL[i][11]  #beta B
        par[n][1] = biNBKL[i][12]  #kappa K
        par[n][2] = biNBKL[i][13]  #lambda l
        for l in [x for x in range(w) if x!=n]:
            if par[n][0] == par[l][0] and par[n][1] == par[l][1] and par[n][2]== par[l][2]:
                c += 1
        if c ==0:

#            print(biNBKL[i][7],biNBKL[i][8],biNBKL[i][9],phi_bar)
            n +=1

        else:
            c=0

#
#           print(n)
        if n == int(h/n_vol):
            break

print(par)

#sys.exit()

os.makedirs('polyakov', exist_ok=True)
os.makedirs('phi_bar', exist_ok=True)
os.makedirs('action', exist_ok=True)
os.makedirs('plaquette', exist_ok=True)
os.makedirs('phi_plaq', exist_ok=True)



m=0
for b in range(3): #loop over beta = 1.5,2.5
    beta = 2 + b/2
    for k in range(4): #loop over volumes
        if k <3:
            leng = 4 + 2*k
        if k ==3:
            leng =12
        for n in range(h):
            if biNBKL[n][10] == leng and biNBKL[n][11] == beta:
                    phi_bar[m] = biNBKL[n][0]
                    phi_bar_err[m] = biNBKL[n][1]
                    poly[m] = biNBKL[n][2]
                    poly_err[m] = biNBKL[n][3]
                    act[m] = biNBKL[n][4]
                    act_err[m] = biNBKL[n][5]
                    plaq[m] = biNBKL[n][6]
                    plaq_err[m] = biNBKL[n][7]
                    phi_plaq[m] = biNBKL[n][8]
                    phi_plaq_err[m] = biNBKL[n][9]
                    Kappa[m] = biNBKL[n][12]
                    Lambda[m] = biNBKL[n][13]
                    m = m +1
            if m == w:
                    m = 0
       # print('leng =' ,leng, 'n=',n,'par[n] =',par[n],'phi_bar =', phi_bar )
        #print(Kappa)
        print(beta)
        #print(n)
        #print(biNBKL[n][11])
        print(leng)
        #print(Lambda)

        figp = plt.figure()
        axp = figp.add_subplot(111, projection='3d')
        axp.scatter(Kappa,Lambda,phi_bar)
        phi_bar_path = 'phi_bar/B'+str(beta)+'L'+str(leng)+'.png'

        axp.set_xlabel('Kappa')
        axp.set_ylabel('Lambda')
        axp.set_zlabel('Phi_bar^2')
        figp.savefig(phi_bar_path)

    #    for angle in range(0, 360):
    #        axp.view_init(30, angle)
    #        plt.draw()
#            plt.pause(.001)


        figl = plt.figure()
        axl = figl.add_subplot(111, projection='3d')
        axl.scatter(Kappa,Lambda,poly)
        poly_path = 'polyakov/B'+str(beta)+'L'+str(leng)+'.png'

        axl.set_xlabel('Kappa')
        axl.set_ylabel('Lambda')
        axl.set_zlabel('Polyakov')
        figl.savefig(poly_path)

#        for angle in range(0, 360):
#            axl.view_init(30, angle)
#            plt.draw()
#            plt.pause(.001)


        figa = plt.figure()
        axa = figa.add_subplot(111, projection='3d')
        axa.scatter(Kappa,Lambda,act)
        act_path = 'action/B'+str(beta)+'L'+str(leng)+'.png'
        axa.set_xlabel('Kappa')
        axa.set_ylabel('Lambda')
        axa.set_zlabel('Action')
        figa.savefig(act_path)

#        for angle in range(0, 360):
#            axa.view_init4(30, angle)
#            plt.draw()
#            plt.pause(.001)


        figu = plt.figure()
        axu = figu.add_subplot(111, projection='3d')
        axu.scatter(Kappa,Lambda,plaq)
        plaq_path = 'plaquette/B'+str(beta)+'L'+str(leng)+'.png'
        axu.set_xlabel('Kappa')
        axu.set_ylabel('Lambda')
        axu.set_zlabel('Plaquette')
        figu.savefig(plaq_path)

#        for angle in range(0, 360):
#            axu.view_init(30, angle)
#            plt.draw()
#            plt.pause(.001)


        figp1 = plt.figure()
        axp1 = figp1.add_subplot(111, projection='3d')
        axp1.scatter(Kappa,Lambda,phi_plaq)
        phi_plaq_path = 'phi_plaq/B'+str(beta)+'L'+str(leng)+'.png'
        axp1.set_xlabel('Kappa')
        axp1.set_ylabel('Lambda')
        axp1.set_zlabel(r'Tr $(\Phi U)$')
        figp1.savefig(phi_plaq_path)

#        for angle in range(0, 360):
#            axp1.view_init(30, angle)
#            plt.draw()
#            plt.pause(.001)


        plt.close(figp)
        plt.close(figl)
        plt.close(figa)
        plt.close(figu)
        plt.close(figp1)



# We draw now p

phi_bar_vol = list(range(4))
phi_bar_vol_err = list(range(4))
poly_vol = list(range(4))
poly_vol_err = list(range(4))
plaq_vol = list(range(4))
plaq_vol_err = list(range(4))
phi_plaq_vol = list(range(4))
phi_plaq_vol_err = list(range(4))

dummy = list(range(3000))

Vol_inv = [1/4, 1/6,1/8,1/12]


os.makedirs('Volume_scaling', exist_ok=True)


for n in range(w):

    figv = plt.figure()
    figv.set_size_inches(28.5, 15.5)


    os.chdir("/hosts/nashome/vaf/Analyzer/sweep")

    # detect the current working directory
    path = os.getcwd()

    # read the entries
    with os.scandir(path) as listOfEntries:
        for entry in listOfEntries:
            # print all entries that are files
            if entry.is_file():
                #print(entry.name)
                #plt.plotfile(entry.name)
                #plt.show()

                if re.search(r'(.*)_output(.*)Nx',entry.name).group(1) == 'plaq':

                    N = re.search(r'(.*)Nx(.*)Ny',entry.name).group(2)
                    if N:
                        print(N)
                #biNBKL[i][10] = float(N)

                    B = re.search(r'(.*)B(.*)_K',entry.name).group(2)
                    if B:
                        print(B)
                        #print(par[n][0])
                #biNBKL[i][11] = float(B)

                    K = re.search(r'(.*)K(.*)_L',entry.name).group(2)
                    if K:
                        print(K)
                        #print(par[n][1])
                #biNBKL[i][12] = float(K)

                    L = re.search(r'(.*)L(.*).bin',entry.name).group(2)
                    if L:
                        print(L)
                        #print(par[n][2])

                    if float(B) == par[n][0] and K == str(par[n][1]) and L == str(par[n][2]):
                        print('ok')
                        dummy = np.loadtxt(entry.name, usecols=(0))
                        axsp = figv.add_subplot(2,4,3)
                        axsp.plot(dummy,label = N)
                        axsp.set_ylabel('Plaquette')
                        axsp.set_xlabel('measurements')
                        axsp.legend()

                if re.search(r'(.*)_output(.*)Nx',entry.name).group(1) == 'poly':

                    N = re.search(r'(.*)Nx(.*)Ny',entry.name).group(2)
                    if N:
                        print(N)
                #biNBKL[i][10] = float(N)

                    B = re.search(r'(.*)B(.*)_K',entry.name).group(2)
                    if B:
                        print(B)
                        #print(par[n][0])
                #biNBKL[i][11] = float(B)

                    K = re.search(r'(.*)K(.*)_L',entry.name).group(2)
                    if K:
                        print(K)
                #biNBKL[i][12] = float(K)

                    L = re.search(r'(.*)L(.*).bin',entry.name).group(2)
                    if L:
                        print(L)

                    if float(B) == par[n][0] and K == str(par[n][1]) and L == str(par[n][2]):
                        print('ok')
                        dummy = np.loadtxt(entry.name, usecols=(0))
                        axspol = figv.add_subplot(2,4,2)
                        axspol.plot(dummy,label = N)
                        axspol.set_ylabel('Polyakov')
                        axspol.set_xlabel('measurements')
                        axspol.legend()

                if re.search(r'(.*)_output(.*)Nx',entry.name).group(1) == 'check':

                    N = re.search(r'(.*)Nx(.*)Ny',entry.name).group(2)
                    if N:
                        print(N)
                #biNBKL[i][10] = float(N)

                    B = re.search(r'(.*)B(.*)_K',entry.name).group(2)
                    if B:
                        print(B)
                        #print(par[n][0])
                #biNBKL[i][11] = float(B)

                    K = re.search(r'(.*)K(.*)_L',entry.name).group(2)
                    if K:
                        print(K)
                #biNBKL[i][12] = float(K)

                    L = re.search(r'(.*)L(.*).bin',entry.name).group(2)
                    if L:
                        print(L)

                    if float(B) == par[n][0] and K == str(par[n][1]) and L == str(par[n][2]):
                        print('ok')
                        dummy = np.loadtxt(entry.name, usecols=(0))
                        axspba = figv.add_subplot(2,4,1)
                        axspba.plot(dummy, label = N)
                        axspba.set_ylabel(r'$\bar \Phi^2$')
                        axspba.set_xlabel('measurements')
                        axspba.legend()

                if re.search(r'(.*)_output(.*)Nx',entry.name).group(1) == 'phip':

                    N = re.search(r'(.*)Nx(.*)Ny',entry.name).group(2)
                    if N:
                        print(N)
                #biNBKL[i][10] = float(N)

                    B = re.search(r'(.*)B(.*)_K',entry.name).group(2)
                    if B:
                        print(B)
                        print(par[n][0])
                #biNBKL[i][11] = float(B)

                    K = re.search(r'(.*)K(.*)_L',entry.name).group(2)
                    if K:
                        print(K)
                #biNBKL[i][12] = float(K)

                    L = re.search(r'(.*)L(.*).bin',entry.name).group(2)
                    if L:
                        print(L)

                    if float(B) == par[n][0] and K == str(par[n][1]) and L == str(par[n][2]):
                        print('OK')
                        dummy = np.loadtxt(entry.name, usecols=(0))
                        axspp = figv.add_subplot(2,4,4)
                        axspp.plot(dummy,label = N)
                        axspp.set_ylabel(r'Tr $(\Phi U)$')
                        axspp.set_xlabel('measurements')
                        axspp.legend()

    os.chdir("/hosts/nashome/vaf/Analyzer")

    #volume = (4,6,8,12)


    for j in range(w):
        if biNBKL[j][11] == par[n][0] and biNBKL[j][12] == par[n][1] and biNBKL[j][13] == par[n][2]:
            print(biNBKL[j][11] )
            for k in range(4):
                if k <3:
                    leng = 4 + 2*k
                if k == 3:
                    leng = 12
                if biNBKL[j][10] == leng:
                    phi_bar_vol[k] = biNBKL[j][0]
                    phi_bar_vol_err[k] = biNBKL[j][1]
                    poly_vol[k] = biNBKL[j][2]
                    poly_vol_err[k] = biNBKL[j][3]
                    plaq_vol[k] = biNBKL[j][6]
                    plaq_vol_err[k] = biNBKL[j][7]
                    phi_plaq_vol[k] = biNBKL[j][8]
                    phi_plaq_vol_err[k] = biNBKL[j][9]
    #print('polyakov at ',par[n],' = ', phi_plaq_vol, phi_plaq_vol_err )



    axb = figv.add_subplot(2,4,5)
    axb.errorbar(Vol_inv,phi_bar_vol,yerr=phi_bar_vol_err, fmt='x', markersize=10)
    axb.set_ylabel(r'$\bar \Phi^2$')
    axb.set_yscale("log")
    axb.set_xlabel('1/L')
    axpol = figv.add_subplot(2,4,6)
    axpol.errorbar(Vol_inv,poly_vol,yerr=poly_vol_err, fmt='x', markersize=10)
    axpol.set_ylabel('Polyakov')
    axpol.set_yscale("log")
    axpol.set_xlabel('1/L')
    axpl = figv.add_subplot(2,4,7)
    axpl.errorbar(Vol_inv,plaq_vol,yerr=plaq_vol_err, fmt='x', markersize=10)
    axpl.set_ylabel('Plaquette')
    axpl.set_yscale("log")
    axpl.set_xlabel('1/L')
    axpp = figv.add_subplot(2,4,8)
    axpp.errorbar(Vol_inv,phi_plaq_vol,yerr=phi_plaq_vol_err, fmt='x', markersize=10)
    axpp.set_ylabel(r'Tr $(\Phi U)$')
    axpp.set_yscale("log")
    axpp.set_xlabel('1/L')
    phi_bar_path = 'Volume_scaling/B'+str(par[n][0])+'K'+str(par[n][1])+'L'+str(par[n][2])+'.png'
    figv.savefig(phi_bar_path, dpi=100)

    plt.close(figv)


h = file_len('O1minus_results.dat')

os.makedirs('O1minus', exist_ok=True)

a =0

biarray = [[0 for x in range(25)] for y in range(h)]

with open('O1minus_results.dat','r') as f:
    for line in f:
    #    print(line.split())
        array = line.split()

        Nt = int(re.search(r'(.*)Nt(.*)_B',array[0]).group(2))

        w = 2*Nt+1

        for i in range(1,w):
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
        C[t] = biarray[i][2*t+1]
        C_unc[t] = biarray[i][2*t+2]

    print(Nt,B,K,L)
    print('C=',C)
    print('sigma(C)=',C_unc)

    fig1 = plt.figure()
    axc = fig1.add_subplot(1,1,1)
    O1minus_path = 'O1minus/B'+str(B)+'K'+str(K)+'L'+str(L)+'.png'
    axc.errorbar(Tl,C,yerr=C_unc, fmt='x', markersize=10)
    axc.set_ylabel(r'C(t)')
#    axc.set_yscale("log")
    axc.set_xlabel('t')
    fig1.savefig(O1minus_path, dpi=100)
    plt.close(fig1)

h = file_len('O1minustr_results.dat')

os.makedirs('O1minustr', exist_ok=True)

a =0

biarray = [[0 for x in range(25)] for y in range(h)]

with open('O1minustr_results.dat','r') as f:
    for line in f:
    #    print(line.split())
        array = line.split()

        Nt = int(re.search(r'(.*)Nt(.*)_B',array[0]).group(2))

        w = 2*Nt+1

        for i in range(1,w):
            array[i] = float(array[i])



    #    print(array)
        for j in range(w):
            biarray[a][j] = array[j]
        a+= 1

#os.mkdir('O1minus')

for i in range(h):

    Nt = int(re.search(r'(.*)Nt(.*)_B',biarray[i][0]).group(2))
    B = re.search(r'(.*)B(.*)_K',biarray[i][0]).group(2)
    K = re.search(r'(.*)K(.*)_L',biarray[i][0]).group(2)
    L = re.search(r'(.*)L(.*).bin',biarray[i][0]).group(2)

    Tl = list(range(Nt))
    C = list(range(Nt))
    C_unc = list(range(Nt))

    for t in range(Nt):
        C[t] = biarray[i][2*t+1]
        C_unc[t] = biarray[i][2*t+2]

    print(Nt,B,K,L)
    print('C=',C)
    print('sigma(C)=',C_unc)

    fig1 = plt.figure()
    axc = fig1.add_subplot(1,1,1)
    O1minustr_path = 'O1minustr/B'+str(B)+'K'+str(K)+'L'+str(L)+'.png'
    axc.errorbar(Tl,C,yerr=C_unc, fmt='x', markersize=10)
    axc.set_ylabel(r'C(t)')
#    axc.set_yscale("log")
    axc.set_xlabel('t')
    fig1.savefig(O1minustr_path, dpi=100)
    plt.close(fig1)

h = file_len('SC1_results.dat')

os.makedirs('SC1', exist_ok=True)

a =0

biarray = [[0 for x in range(25)] for y in range(h)]

with open('SC1_results.dat','r') as f:
    for line in f:
    #    print(line.split())
        array = line.split()

        Nt = int(re.search(r'(.*)Nt(.*)_B',array[0]).group(2))

        w = 2*Nt+1

        for i in range(1,w):
            array[i] = float(array[i])



    #    print(array)
        for j in range(w):
            biarray[a][j] = array[j]
        a+= 1

#os.mkdir('SC1')

for i in range(h):

    Nt = int(re.search(r'(.*)Nt(.*)_B',biarray[i][0]).group(2))
    B = re.search(r'(.*)B(.*)_K',biarray[i][0]).group(2)
    K = re.search(r'(.*)K(.*)_L',biarray[i][0]).group(2)
    L = re.search(r'(.*)L(.*).bin',biarray[i][0]).group(2)

    Tl = list(range(Nt))
    C = list(range(Nt))
    C_unc = list(range(Nt))

    for t in range(Nt):
        C[t] = biarray[i][2*t+1]
        C_unc[t] = biarray[i][2*t+2]

    print(Nt,B,K,L)
    print('C=',C)
    print('sigma(C)=',C_unc)

    fig1 = plt.figure()
    axc = fig1.add_subplot(1,1,1)
    SC1_path = 'SC1/B'+str(B)+'K'+str(K)+'L'+str(L)+'.png'
    axc.errorbar(Tl,C,yerr=C_unc, fmt='x', markersize=10)
    axc.set_ylabel(r'C(t)')
#    axc.set_yscale("log")
    axc.set_xlabel('t')
    fig1.savefig(SC1_path, dpi=100)
    plt.close(fig1)

h = file_len('SC2_results.dat')

os.makedirs('SC2', exist_ok=True)

a =0

biarray = [[0 for x in range(25)] for y in range(h)]

with open('SC2_results.dat','r') as f:
    for line in f:
    #    print(line.split())
        array = line.split()

        Nt = int(re.search(r'(.*)Nt(.*)_B',array[0]).group(2))

        w = 2*Nt+1

        for i in range(1,w):
            array[i] = float(array[i])



    #    print(array)
        for j in range(w):
            biarray[a][j] = array[j]
        a+= 1

#os.mkdir('SC2')

for i in range(h):

    Nt = int(re.search(r'(.*)Nt(.*)_B',biarray[i][0]).group(2))
    B = re.search(r'(.*)B(.*)_K',biarray[i][0]).group(2)
    K = re.search(r'(.*)K(.*)_L',biarray[i][0]).group(2)
    L = re.search(r'(.*)L(.*).bin',biarray[i][0]).group(2)

    Tl = list(range(Nt))
    C = list(range(Nt))
    C_unc = list(range(Nt))

    for t in range(Nt):
        C[t] = biarray[i][2*t+1]
        C_unc[t] = biarray[i][2*t+2]

    print(Nt,B,K,L)
    print('C=',C)
    print('sigma(C)=',C_unc)

    fig1 = plt.figure()
    axc = fig1.add_subplot(1,1,1)
    SC2_path = 'SC2/B'+str(B)+'K'+str(K)+'L'+str(L)+'.png'
    axc.errorbar(Tl,C,yerr=C_unc, fmt='x', markersize=10)
    axc.set_ylabel(r'C(t)')
#    axc.set_yscale("log")
    axc.set_xlabel('t')
    fig1.savefig(SC2_path, dpi=100)
    plt.close(fig1)

h = file_len('SC3_results.dat')

os.makedirs('SC3', exist_ok=True)

a =0

biarray = [[0 for x in range(25)] for y in range(h)]

with open('SC3_results.dat','r') as f:
    for line in f:
    #    print(line.split())
        array = line.split()

        Nt = int(re.search(r'(.*)Nt(.*)_B',array[0]).group(2))

        w = 2*Nt+1

        for i in range(1,w):
            array[i] = float(array[i])



    #    print(array)
        for j in range(w):
            biarray[a][j] = array[j]
        a+= 1

#os.mkdir('SC3')

for i in range(h):

    Nt = int(re.search(r'(.*)Nt(.*)_B',biarray[i][0]).group(2))
    B = re.search(r'(.*)B(.*)_K',biarray[i][0]).group(2)
    K = re.search(r'(.*)K(.*)_L',biarray[i][0]).group(2)
    L = re.search(r'(.*)L(.*).bin',biarray[i][0]).group(2)

    Tl = list(range(Nt))
    C = list(range(Nt))
    C_unc = list(range(Nt))

    for t in range(Nt):
        C[t] = biarray[i][2*t+1]
        C_unc[t] = biarray[i][2*t+2]

    print(Nt,B,K,L)
    print('C=',C)
    print('sigma(C)=',C_unc)

    fig1 = plt.figure()
    axc = fig1.add_subplot(1,1,1)
    SC3_path = 'SC3/B'+str(B)+'K'+str(K)+'L'+str(L)+'.png'
    axc.errorbar(Tl,C,yerr=C_unc, fmt='x', markersize=10)
    axc.set_ylabel(r'C(t)')
#    axc.set_yscale("log")
    axc.set_xlabel('t')
    fig1.savefig(SC3_path, dpi=100)
    plt.close(fig1)
