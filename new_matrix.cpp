#include <stdio.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>

 #include <sys/stat.h>

int getFileSize(const std::string &fileName);

double average(int n_meas,std::vector<double> vec);
double sigma0(int n_meas, std::vector<double> vec);



int main(int argc, char *argv[]){


  int Nt =24;
  int Nx =24;
  int Ny =24;
  int Nz =24;

  int n_meas=9993;
  int n_smear=6;
  int n_op=4;

  int n_mat= n_smear*n_op;

  std::vector<std::vector<std::vector<std::vector<double> > > > Br(n_meas,std::vector<std::vector<std::vector<double> > >(n_smear,std::vector<std::vector<double> > (Nt,std::vector<double> (3,0))));
  std::vector<std::vector<std::vector<std::vector<double> > > > B2r(n_meas,std::vector<std::vector<std::vector<double> > >(n_smear,std::vector<std::vector<double> > (Nt,std::vector<double> (3,0))));
  std::vector<std::vector<std::vector<std::vector<double> > > > Bphir(n_meas,std::vector<std::vector<std::vector<double> > >(n_smear,std::vector<std::vector<double> > (Nt,std::vector<double> (3,0))));
  std::vector<std::vector<std::vector<std::vector<double> > > > B2pr(n_meas,std::vector<std::vector<std::vector<double> > >(n_smear,std::vector<std::vector<double> > (Nt,std::vector<double> (3,0))));

  std::vector<std::vector<double>> O_avg(n_smear,std::vector<double> (Nt,0));
  std::vector<std::vector<double>> O1_avg(n_smear,std::vector<double> (Nt,0));
  std::vector<std::vector<double>> O2_avg(n_smear,std::vector<double> (Nt,0));
  std::vector<std::vector<double>> O3_avg(n_smear,std::vector<double> (Nt,0));

  std::vector<std::vector<std::vector<double> > > O(n_meas,std::vector<std::vector<double> >(n_smear,std::vector<double>(Nt,0)));
  std::vector<std::vector<std::vector<double> > > O1(n_meas,std::vector<std::vector<double> >(n_smear,std::vector<double>(Nt,0)));
  std::vector<std::vector<std::vector<double> > > O2(n_meas,std::vector<std::vector<double> >(n_smear,std::vector<double>(Nt,0)));
  std::vector<std::vector<std::vector<double> > > O3(n_meas,std::vector<std::vector<double> >(n_smear,std::vector<double>(Nt,0)));
  std::vector<std::vector<std::vector<double> > > O_sq(n_meas,std::vector<std::vector<double> >(n_smear,std::vector<double>(Nt,0)));


  std::vector<std::vector<std::vector<double> > > c_avg(n_mat,std::vector<std::vector<double> >(n_mat,std::vector<double>(Nt,0)));
  std::vector<std::vector<std::vector<double> > > c_err(n_mat,std::vector<std::vector<double> >(n_mat,std::vector<double>(Nt,0)));

std::vector<double> vec(n_meas);

  double B_p[Nt][3];
  double B2[Nt][3];
  double B_2p[Nt][3];
  double Bphi[Nt][3];

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




int n_max;

int size;


sprintf(O1minus_name,"O1minus_output_files_new/output_Nt%d_Nx%d_Ny%d_Nz%d_B%f_K%f_L%f_frodo.bin",Nt,Nx,Ny,Nz, beta, kappa,lambda );
sprintf(out_avg_name,"cross_new/matrix_c_L%d_k%f.txt",Nt,kappa);
sprintf(out_err_name,"cross_new/matrix_c_L%d_k%f_err.txt",Nt,kappa);


std::cout << O1minus_name << '\n';
std::ifstream O1minusf;
O1minusf.open(O1minus_name,std::ios::in|std::ios::binary);

size = getFileSize(O1minus_name);

std::cout << "Filesize =" << size << '\n';


std::ofstream outf;
std::ofstream outf_err;

// std::cout << "Br =" << Br[n_meas-1][n_smear-1][Nt-1][2] << '\n';
// std::cout << "size of" << sizeof(double) << '\n';
n_max = (int) size/(sizeof(B_p)*n_op*(n_smear));
std::cout << "Max measurements of file =" << n_max << '\n';


if(n_meas> n_max){
  std::cout << "[WARNING] The number of measurements exceeds the file size" << '\n';
}

for(int n=0;n<n_meas;n++){
//  std::cout << "n=" <<n << '\n';
  for(int ns=0;ns<n_smear;ns++){
    O1minusf.read((char*)&B_p, sizeof(B_p));
    O1minusf.read((char*)&B2, sizeof(B2));
    O1minusf.read((char*)&Bphi, sizeof(Bphi));
    O1minusf.read((char*)&B_2p, sizeof(B_2p));
    for(int t=0;t<Nt;t++){
      for(int k=0;k<3;k++){
        if(isnan(Br[n][ns][t][k])){
          printf("Found nan at measurement %d\n",n);
          continue;
        }
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





  for(int i=0;i<n_smear;i++){
    std::cout << "i= " <<i << '\n';
    for(int j=0;j<n_smear;j++){
      std::cout << "  j= " <<j <<'\n';
      for(int t=0;t<Nt;t++){
        // std::cout << "t= " <<t << '\n';


      for(int n=0; n<n_meas;n++){
        vec[n]=0;
        for(int t_pr=0;t_pr<Nt;t_pr++){
            vec[n] += (O [n][i][t_pr] * O [n][j][(t_pr+t)%Nt] + O [n][i][(t_pr+t)%Nt] * O [n][j][t_pr]) / (2*Nt);
          }
        }
        c_avg[i][j][t]=average(n_meas,vec);
        c_err[i][j][t]=sigma0(n_meas,vec);

      for(int n=0; n<n_meas;n++){
        vec[n]=0;
        for(int t_pr=0;t_pr<Nt;t_pr++){
              vec[n] += (O1[n][i][t_pr] * O1[n][j][(t_pr+t)%Nt] + O1[n][i][(t_pr+t)%Nt] * O1[n][j][t_pr]) / (2*Nt);
            }
          }
      c_avg[i+n_smear][j+n_smear][t]=average(n_meas,vec);
      c_err[i+n_smear][j+n_smear][t]=sigma0(n_meas,vec);


      for(int n=0; n<n_meas;n++){
        vec[n]=0;
            for(int t_pr=0;t_pr<Nt;t_pr++){
                vec[n] += (O2[n][i][t_pr] * O2[n][j][(t_pr+t)%Nt] + O2[n][i][(t_pr+t)%Nt] * O2[n][j][t_pr]) / (2*Nt);
              }
            }
            c_avg[i+2*n_smear][j+2*n_smear][t]=average(n_meas,vec);
            c_err[i+2*n_smear][j+2*n_smear][t]=sigma0(n_meas,vec);

      for(int n=0; n<n_meas;n++){
        vec[n]=0;
              for(int t_pr=0;t_pr<Nt;t_pr++){
                  vec[n] += (O3[n][i][t_pr] * O3[n][j][(t_pr+t)%Nt] + O3[n][i][(t_pr+t)%Nt] * O3[n][j][t_pr]) / (2*Nt);
                }
              }
              c_avg[i+3*n_smear][j+3*n_smear][t]=average(n_meas,vec);
              c_err[i+3*n_smear][j+3*n_smear][t]=sigma0(n_meas,vec);

      for(int n=0; n<n_meas;n++){
                vec[n]=0;
                for(int t_pr=0;t_pr<Nt;t_pr++){
                    vec[n] += (O[n][i][t_pr] * O1[n][j][(t_pr+t)%Nt] + O[n][i][(t_pr+t)%Nt] * O1[n][j][t_pr]) / (2*Nt);
                  }
                }
                c_avg[i][j+n_smear][t]=average(n_meas,vec);
                c_err[i][j+n_smear][t]=sigma0(n_meas,vec);

      for(int n=0; n<n_meas;n++){
                  vec[n]=0;
                  for(int t_pr=0;t_pr<Nt;t_pr++){
                      vec[n] += (O[n][i][t_pr] * O2[n][j][(t_pr+t)%Nt] + O[n][i][(t_pr+t)%Nt] * O2[n][j][t_pr]) / (2*Nt);
                    }
                  }
                  c_avg[i][j+2*n_smear][t]=average(n_meas,vec);
                  c_err[i][j+2*n_smear][t]=sigma0(n_meas,vec);

                  for(int n=0; n<n_meas;n++){
                    vec[n]=0;
                    for(int t_pr=0;t_pr<Nt;t_pr++){
                        vec[n] += (O[n][i][t_pr] * O3[n][j][(t_pr+t)%Nt] + O[n][i][(t_pr+t)%Nt] * O3[n][j][t_pr]) / (2*Nt);
                      }
                    }
                    c_avg[i][j+3*n_smear][t]=average(n_meas,vec);
                    c_err[i][j+3*n_smear][t]=sigma0(n_meas,vec);

                    for(int n=0; n<n_meas;n++){
                      vec[n]=0;
                      for(int t_pr=0;t_pr<Nt;t_pr++){
                          vec[n] += (O1[n][i][t_pr] * O[n][j][(t_pr+t)%Nt] + O1[n][i][(t_pr+t)%Nt] * O[n][j][t_pr]) / (2*Nt);
                        }
                      }
                      c_avg[i+n_smear][j][t]=average(n_meas,vec);
                      c_err[i+n_smear][j][t]=sigma0(n_meas,vec);

                      for(int n=0; n<n_meas;n++){
                        vec[n]=0;
                        for(int t_pr=0;t_pr<Nt;t_pr++){
                            vec[n] += (O2[n][i][t_pr] * O[n][j][(t_pr+t)%Nt] + O2[n][i][(t_pr+t)%Nt] * O[n][j][t_pr]) / (2*Nt);
                          }
                        }
                        c_avg[i+2*n_smear][j][t]=average(n_meas,vec);
                        c_err[i+2*n_smear][j][t]=sigma0(n_meas,vec);

                        for(int n=0; n<n_meas;n++){
                          vec[n]=0;
                          for(int t_pr=0;t_pr<Nt;t_pr++){
                              vec[n] += (O3[n][i][t_pr] * O[n][j][(t_pr+t)%Nt] + O3[n][i][(t_pr+t)%Nt] * O[n][j][t_pr]) / (2*Nt);
                            }
                          }
                          c_avg[i+3*n_smear][j][t]=average(n_meas,vec);
                          c_err[i+3*n_smear][j][t]=sigma0(n_meas,vec);

                        for(int n=0; n<n_meas;n++){
                          vec[n]=0;
                          for(int t_pr=0;t_pr<Nt;t_pr++){
                              vec[n] += (O1[n][i][t_pr] * O2[n][j][(t_pr+t)%Nt] + O1[n][i][(t_pr+t)%Nt] * O2[n][j][t_pr]) / (2*Nt);
                            }
                          }
                        c_avg[i+n_smear][j+2*n_smear][t]=average(n_meas,vec);
                        c_err[i+n_smear][j+2*n_smear][t]=sigma0(n_meas,vec);

                        for(int n=0; n<n_meas;n++){
                          vec[n]=0;
                          for(int t_pr=0;t_pr<Nt;t_pr++){
                              vec[n] += (O1[n][i][t_pr] * O3[n][j][(t_pr+t)%Nt] + O1[n][i][(t_pr+t)%Nt] * O3[n][j][t_pr]) / (2*Nt);
                            }
                          }
                        c_avg[i+n_smear][j+3*n_smear][t]=average(n_meas,vec);
                        c_err[i+n_smear][j+3*n_smear][t]=sigma0(n_meas,vec);

                        for(int n=0; n<n_meas;n++){
                          vec[n]=0;
                          for(int t_pr=0;t_pr<Nt;t_pr++){
                              vec[n] += (O2[n][i][t_pr] * O3[n][j][(t_pr+t)%Nt] + O2[n][i][(t_pr+t)%Nt] * O3[n][j][t_pr]) / (2*Nt);
                            }
                          }
                        c_avg[i+2*n_smear][j+3*n_smear][t]=average(n_meas,vec);
                        c_err[i+2*n_smear][j+3*n_smear][t]=sigma0(n_meas,vec);

                        for(int n=0; n<n_meas;n++){
                          vec[n]=0;
                          for(int t_pr=0;t_pr<Nt;t_pr++){
                              vec[n] += (O2[n][i][t_pr] * O1[n][j][(t_pr+t)%Nt] + O2[n][i][(t_pr+t)%Nt] * O1[n][j][t_pr]) / (2*Nt);
                            }
                          }
                        c_avg[i+2*n_smear][j+n_smear][t]=average(n_meas,vec);
                        c_err[i+2*n_smear][j+n_smear][t]=sigma0(n_meas,vec);

                        for(int n=0; n<n_meas;n++){
                          vec[n]=0;
                          for(int t_pr=0;t_pr<Nt;t_pr++){
                              vec[n] += (O3[n][i][t_pr] * O1[n][j][(t_pr+t)%Nt] + O3[n][i][(t_pr+t)%Nt] * O1[n][j][t_pr]) / (2*Nt);
                            }
                          }
                        c_avg[i+3*n_smear][j+n_smear][t]=average(n_meas,vec);
                        c_err[i+3*n_smear][j+n_smear][t]=sigma0(n_meas,vec);

                  for(int n=0; n<n_meas;n++){
                          vec[n]=0;
                          for(int t_pr=0;t_pr<Nt;t_pr++){
                              vec[n] += (O3[n][i][t_pr] * O2[n][j][(t_pr+t)%Nt] + O3[n][i][(t_pr+t)%Nt] * O2[n][j][t_pr]) / (2*Nt);
                            }
                          }
              c_avg[i+3*n_smear][j+2*n_smear][t]=average(n_meas,vec);
                c_err[i+3*n_smear][j+2*n_smear][t]=sigma0(n_meas,vec);


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

int getFileSize(const std::string &fileName)
{
    std::ifstream file(fileName.c_str(), std::ifstream::in | std::ifstream::binary);

    if(!file.is_open())
    {
        return -1;
    }

    file.seekg(0, std::ios::end);
    int fileSize = file.tellg();
    file.close();

    return fileSize;
}

double average(int n_meas, std::vector<double> vec){

  double avg=0;
  for (int n = 0; n < n_meas; n++) {
    avg += vec[n];
  }
  avg= (double)avg/n_meas;
  return avg;

}

double sigma0(int n_meas, std::vector<double> vec){

  double sigma=0;
  double avg=0;
  double norm = (double) 1.0/n_meas;
  for (int n = 0; n < n_meas; n++) {
    avg+= vec[n];
    sigma+= vec[n]*vec[n];
  }
  avg*=norm;
  sigma=norm*sigma-avg*avg;
  sigma= sqrt(sigma*norm);

  return sigma;

}
