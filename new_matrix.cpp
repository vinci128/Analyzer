#include <stdio.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>

#include "statistics.h"


const int Nt =8;
const int Nx =8;
const int Ny =8;
const int Nz =8;

const int n_meas=20000;
const int n_smear=5;
const int n_op=4;

const int n_mat= 20;

// double Br[n_meas][n_smear][Nt][3];
// double B2r[n_meas][n_smear][Nt][3];
// double Bphir[n_meas][n_smear][Nt][3];
// double B2pr[n_meas][n_smear][Nt][3];
std::vector<std::vector<std::vector<std::vector<double> > > > Br(n_meas,std::vector<std::vector<std::vector<double> > >(n_smear,std::vector<std::vector<double> > (Nt,std::vector<double> (3,0))));
std::vector<std::vector<std::vector<std::vector<double> > > > B2r(n_meas,std::vector<std::vector<std::vector<double> > >(n_smear,std::vector<std::vector<double> > (Nt,std::vector<double> (3,0))));
std::vector<std::vector<std::vector<std::vector<double> > > > Bphir(n_meas,std::vector<std::vector<std::vector<double> > >(n_smear,std::vector<std::vector<double> > (Nt,std::vector<double> (3,0))));
std::vector<std::vector<std::vector<std::vector<double> > > > B2pr(n_meas,std::vector<std::vector<std::vector<double> > >(n_smear,std::vector<std::vector<double> > (Nt,std::vector<double> (3,0))));

// double O[n_meas][n_smear][Nt];
// double O1[n_meas][n_smear][Nt];
// double O2[n_meas][n_smear][Nt];
// double O3[n_meas][n_smear][Nt];
// double O_sq[n_meas][n_smear][Nt];
std::vector<std::vector<std::vector<double> > > O(n_meas,std::vector<std::vector<double> >(n_smear,std::vector<double>(Nt,0)));
std::vector<std::vector<std::vector<double> > > O1(n_meas,std::vector<std::vector<double> >(n_smear,std::vector<double>(Nt,0)));
std::vector<std::vector<std::vector<double> > > O2(n_meas,std::vector<std::vector<double> >(n_smear,std::vector<double>(Nt,0)));
std::vector<std::vector<std::vector<double> > > O3(n_meas,std::vector<std::vector<double> >(n_smear,std::vector<double>(Nt,0)));
std::vector<std::vector<std::vector<double> > > O_sq(n_meas,std::vector<std::vector<double> >(n_smear,std::vector<double>(Nt,0)));

std::vector<std::vector<double>> O_avg(n_smear,std::vector<double> (Nt,0));
std::vector<std::vector<double>> O1_avg(n_smear,std::vector<double> (Nt,0));
std::vector<std::vector<double>> O2_avg(n_smear,std::vector<double> (Nt,0));
std::vector<std::vector<double>> O3_avg(n_smear,std::vector<double> (Nt,0));

 // double c_var[n_meas][n_mat][n_mat][Nt];
 std::vector<std::vector<std::vector<std::vector<double>> > > c_var(n_meas,std::vector<std::vector<std::vector<double> > >(n_mat,std::vector<std::vector<double> > (n_mat,std::vector<double> (Nt,0))));

double vec[n_meas];
//std::vector<double> vec(n_meas);

double B_p[Nt][3];
double B2[Nt][3];
double B_2p[Nt][3];
double Bphi[Nt][3];
// std::vector<std::vector<double> > B_p(Nt,std::vector<double> (3,0));
// std::vector<std::vector<double> > B2(Nt,std::vector<double> (3,0));
// std::vector<std::vector<double> > B_2p(Nt,std::vector<double> (3,0));
// std::vector<std::vector<double> > Bphi(Nt,std::vector<double> (3,0));



// double c_avg[n_mat][n_mat][Nt];
// double c_err[n_mat][n_mat][Nt];
std::vector<std::vector<std::vector<double> > > c_avg(n_mat,std::vector<std::vector<double> >(n_mat,std::vector<double>(Nt,0)));
std::vector<std::vector<std::vector<double> > > c_err(n_mat,std::vector<std::vector<double> >(n_mat,std::vector<double>(Nt,0)));

int main(int argc, char *argv[]){

  double* tau;
  int* flag;
tau =(double*)malloc(sizeof(double));
flag =(int*)malloc(sizeof(int));

char O1minus_name[128];
char out_avg_name[128];
char out_err_name[128];

double beta=4;
double lambda=1;
double kappa;
kappa= 0.65;

sprintf(O1minus_name,"O1minus_output_files_new/output_Nt%d_Nx%d_Ny%d_Nz%d_B%f_K%f_L%f.bin",Nt,Nx,Ny,Nz, beta, kappa,lambda );
sprintf(out_avg_name,"cross_new/matrix_c_L%d_k%f.txt",Nt,kappa);
sprintf(out_err_name,"cross_new/matrix_c_L%d_k%f_err.txt",Nt,kappa);


std::cout << O1minus_name << '\n';
std::ifstream O1minusf;
O1minusf.open(O1minus_name,std::ios::in|std::ios::binary);

std::ofstream outf;
std::ofstream outf_err;

std::cout << "Br =" << Br[n_meas-1][n_smear-1][Nt-1][2] << '\n';
std::cout << "size of" << sizeof(double) << '\n';
for(int n=0;n<n_meas;n++){
//  std::cout << "n=" <<n << '\n';
  for(int ns=0;ns<n_smear;ns++){
    O1minusf.read((char*)&B_p, sizeof(B_p));
    O1minusf.read((char*)&B2, sizeof(B2));
    O1minusf.read((char*)&Bphi, sizeof(Bphi));
    O1minusf.read((char*)&B_2p, sizeof(B_2p));
    for(int t=0;t<Nt;t++){
      for(int k=0;k<3;k++){
        Br[n][ns][t][k]=B_p[t][k];
        B2r[n][ns][t][k]=B2[t][k];
        B2pr[n][ns][t][k]=B_2p[t][k];
        Bphir[n][ns][t][k]=Bphi[t][k];
      }
    }

  }
}
std::cout << "Files have been read." << '\n';
std::cout << Br[n_meas-1][0][0][0] << '\n';

// for(int n=0; n<n_meas;n++){
//   for(int ns=0;ns<n_smear;ns++){
//       for(int t=0;t<Nt;t++){
//         O[n][ns][t]=0;
//         O1[n][ns][t]=0;
//         O3[n][ns][t]=0;
//         O2[n][ns][t]=0;
//         O_sq[n][ns][t]=0;
//       }
//     }
//   }
for(int n=0; n<n_meas;n++){
  for(int ns=0;ns<n_smear;ns++){
      for(int t=0;t<Nt;t++){
        for(int k=0;k<2;k++){
                O[n][ns][t] += Br[n][ns][t][k]/2;
                O1[n][ns][t] += Bphir[n][ns][t][k]/2;
                O3[n][ns][t] += B2pr[n][ns][t][k]/2;
              }
            O_sq[n][ns][t] = O[n][ns][t]*O[n][ns][t];
            for(int k=0;k<2;k++){
                O2[n][ns][t] += (O_sq[n][ns][t]*Br[n][ns][t][k])/2;
                }
              }
            }
          }

std::cout << O[n_meas-1][0][0] << '\n';

for (int ns = 0; ns < n_smear; ns++) {
  for (int t = 0; t < Nt; t++) {
    for (int n = 0; n < n_meas; n++) {
      vec[n]=O[n][ns][t];
    }
    O_avg[ns][t]=average(n_meas,vec);
    for (int n = 0; n < n_meas; n++) {
      vec[n]=O1[n][ns][t];
    }
    O1_avg[ns][t]=average(n_meas,vec);
    for (int n = 0; n < n_meas; n++) {
      vec[n]=O2[n][ns][t];
    }
    O2_avg[ns][t]=average(n_meas,vec);
    for (int n = 0; n < n_meas; n++) {
      vec[n]=O3[n][ns][t];
    }
    O3_avg[ns][t]=average(n_meas,vec);
    for (int n = 0; n < n_meas; n++) {
      O[n][ns][t] -= O_avg[ns][t];
      O1[n][ns][t] -= O1_avg[ns][t];
      O2[n][ns][t] -= O2_avg[ns][t];
      O3[n][ns][t] -= O3_avg[ns][t];
    }
  }
}

// for(int n=0; n<n_meas;n++){
//   for(int i=0;i<n_mat;i++){
//     for(int j=0;j<n_mat;j++){
//       for(int t=0;t<Nt;t++){
//             c_var[n][i][j][t]=0;
//           }
//         }
//       }
//     }


for(int n=0; n<n_meas;n++){
  for(int i=0;i<n_smear;i++){
    for(int j=0;j<n_smear;j++){
      for(int t=0;t<Nt;t++){
        for(int t_pr=0;t_pr<Nt;t_pr++){
                        c_var[n][i][j][t]                     += (O [n][i][t_pr] * O [n][j][(t_pr+t)%Nt] + O [n][i][(t_pr+t)%Nt] * O [n][j][t_pr]) / (2*Nt);
                        c_var[n][i+n_smear][j+n_smear][t]     += (O1[n][i][t_pr] * O1[n][j][(t_pr+t)%Nt] + O1[n][i][(t_pr+t)%Nt] * O1[n][j][t_pr]) / (2*Nt);
                        c_var[n][i+2*n_smear][j+2*n_smear][t] += (O2[n][i][t_pr] * O2[n][j][(t_pr+t)%Nt] + O2[n][i][(t_pr+t)%Nt] * O2[n][j][t_pr]) / (2*Nt);
                        c_var[n][i+3*n_smear][j+3*n_smear][t] += (O3[n][i][t_pr] * O3[n][j][(t_pr+t)%Nt] + O3[n][i][(t_pr+t)%Nt] * O3[n][j][t_pr]) / (2*Nt);
                        c_var[n][i][j+n_smear][t]             += (O [n][i][t_pr] * O1[n][j][(t_pr+t)%Nt] + O [n][i][(t_pr+t)%Nt] * O1[n][j][t_pr]) / (2*Nt);
                        c_var[n][i][j+2*n_smear][t]           += (O [n][i][t_pr] * O2[n][j][(t_pr+t)%Nt] + O [n][i][(t_pr+t)%Nt] * O2[n][j][t_pr]) / (2*Nt);
                        c_var[n][i][j+3*n_smear][t]           += (O [n][i][t_pr] * O3[n][j][(t_pr+t)%Nt] + O [n][i][(t_pr+t)%Nt] * O3[n][j][t_pr]) / (2*Nt);
                        c_var[n][i+n_smear][j][t]             += (O1[n][i][t_pr] * O [n][j][(t_pr+t)%Nt] + O1[n][i][(t_pr+t)%Nt] * O [n][j][t_pr]) / (2*Nt);
                        c_var[n][i+3*n_smear][j][t]           += (O3[n][i][t_pr] * O [n][j][(t_pr+t)%Nt] + O3[n][i][(t_pr+t)%Nt] * O [n][j][t_pr]) / (2*Nt);
                        c_var[n][i+2*n_smear][j][t]           += (O2[n][i][t_pr] * O [n][j][(t_pr+t)%Nt] + O2[n][i][(t_pr+t)%Nt] * O [n][j][t_pr]) / (2*Nt);
                        c_var[n][i+n_smear][j+2*n_smear][t]   += (O1[n][i][t_pr] * O2[n][j][(t_pr+t)%Nt] + O1[n][i][(t_pr+t)%Nt] * O2[n][j][t_pr]) / (2*Nt);
                        c_var[n][i+n_smear][j+3*n_smear][t]   += (O1[n][i][t_pr] * O3[n][j][(t_pr+t)%Nt] + O1[n][i][(t_pr+t)%Nt] * O3[n][j][t_pr]) / (2*Nt);
                        c_var[n][i+2*n_smear][j+3*n_smear][t] += (O2[n][i][t_pr] * O3[n][j][(t_pr+t)%Nt] + O2[n][i][(t_pr+t)%Nt] * O3[n][j][t_pr]) / (2*Nt);
                        c_var[n][i+2*n_smear][j+n_smear][t]   += (O2[n][i][t_pr] * O1[n][j][(t_pr+t)%Nt] + O2[n][i][(t_pr+t)%Nt] * O1[n][j][t_pr]) / (2*Nt);
                        c_var[n][i+3*n_smear][j+n_smear][t]   += (O3[n][i][t_pr] * O1[n][j][(t_pr+t)%Nt] + O3[n][i][(t_pr+t)%Nt] * O1[n][j][t_pr]) / (2*Nt);
                        c_var[n][i+3*n_smear][j+2*n_smear][t] += (O3[n][i][t_pr] * O2[n][j][(t_pr+t)%Nt] + O3[n][i][(t_pr+t)%Nt] * O2[n][j][t_pr]) / (2*Nt);
                      }
                    }
                  }
                }
              }
std::cout << "Cross-correlation matrix filled." << '\n';
std::cout << c_var[n_meas-1][0][0][0] << '\n';

  for(int i=0;i<n_mat;i++){
    std::cout << "i= " << i << '\n';
  for(int k=0;k<n_mat;k++){
    std::cout << "k=" << k << '\n';
      for(int t=0;t<Nt;t++){
        for (int n = 0; n < n_meas; n++) {
          vec[n]=c_var[n][i][k][t];
        }

      c_avg[i][k][t]=average(n_meas,vec);
      //c_err[i][k][t]=sigma(n_meas,vec,tau,flag);
      c_err[i][k][t]=sigma0(n_meas,vec);
      // std::cout << "tau =" << *tau << '\n';
      // std::cout << "flag =" << *flag << '\n';
    }
  }
}

std::cout << "Error and uncertainties matrices obtained." << '\n';



outf.open(out_avg_name,std::ios::out);

outf_err.open(out_err_name,std::ios::out);

for(int t=0;t<Nt;t++){
  for(int i=0;i<n_mat;i++){
    for(int j=0;j<n_mat;j++){
      outf << c_avg[i][j][t] << ' ';
      outf_err << c_err[i][j][t] << ' ';
    }
    outf << "\n";
    outf_err <<"\n";
  }

  outf << "# New time slice" << '\n';
  outf_err << "# New time slice" << '\n';
}



O1minusf.close();
outf.close();
outf_err.close();

}
