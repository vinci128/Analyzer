#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <regex.h>


#include "statistics.h"

void allocate_4d(int count3, int count2, int count1, int count0, double *****data);
void allocate_3d(int count2, int count1, int count0, double ****data);

void free_4d(int count3, int count2, int count1, int count0, double *****data);
void free_3d(int count2, int count1, int count0, double ****data);

int main(int argc, char **argv)
{
  DIR* FD;
  struct dirent* in_file;

        FILE    *entry_file;

      struct stat st;

FILE    *O1minusf;
O1minusf = fopen("O1minus_results.dat", "w");
//  checkf = fopen("check.dat","w");
if (O1minusf == NULL)
{
    fprintf(stderr, "Error : Failed to open stat_results - %s\n", strerror(errno));

    return 1;
}
      if (NULL == (FD = opendir ("/home/vincenzo/Analyzer/O1minus_output_files")))
      {
          fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
          fclose(O1minusf);

          return 1;
      }while ((in_file = readdir(FD)))
      {
          /* On linux/Unix we don't want current and parent directories
           * On windows machine too, thanks Greg Hewgill
           */
          if (!strcmp (in_file->d_name, "."))
              continue;
          if (!strcmp (in_file->d_name, ".."))
              continue;
          /* Open directory entry file for common operation */
          /* TODO : change permissions to meet your need! */
          chdir("/home/vincenzo/Analyzer/O1minus_output_files");

          printf("%s\n", in_file->d_name);
          entry_file = fopen(in_file->d_name, "rb");
          char aChar = (char)in_file->d_name[9];


          if (entry_file == NULL)
          {
              fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
              fclose(O1minusf);

              return 1;
          }



          stat(in_file->d_name, &st);
          int size = st.st_size;

          int T;
          int n_smear =5;

          if(in_file->d_name[9] == 52)(T=4);
          if(in_file->d_name[9] == 54)(T=6);
          if(in_file->d_name[9] == 56)(T=8);
          if(in_file->d_name[9] == 49){
            if(in_file->d_name[9]==50)(T=12);
          }

          printf("Nt = %d\n",T );

          double B[T][3];
          double T1[T][3];
          double T2[T][3];
          double T3[T][3];

          int n_meas = (int) size/ (sizeof(B)*n_smear*4);
          printf("n_meas = %d\n",n_meas );

          //Br = (double*)malloc(n_meas, sizeof(B));
          //double Br[n_meas][n_smear][T][3];
          //double C[n_meas][n_smear][T];
          double C_avg[n_smear][T];
          double C_unc[n_smear][T];

          double ****Br;
          double ***C;

          allocate_4d(n_meas,n_smear,T,3,&Br);
          allocate_3d(n_meas,n_smear,T,&C);


          //double T1r[n_meas][n_smear][T][3];
          //double CT1[n_meas][n_smear][T];
          double CT1_avg[n_smear][T];
          double CT1_unc[n_smear][T];

          double ****T1r;
          double ***CT1;

          allocate_4d(n_meas,n_smear,T,3,&T1r);
          allocate_3d(n_meas,n_smear,T,&CT1);


          //double T2r[n_meas][n_smear][T][3];
          //double CT2[n_meas][n_smear][T];
          double CT2_avg[n_smear][T];
          double CT2_unc[n_smear][T];

          double ****T2r;
          double ***CT2;

          allocate_4d(n_meas,n_smear,T,3,&T2r);
          allocate_3d(n_meas,n_smear,T,&CT2);


          //double T3r[n_meas][n_smear][T][3];
          //double CT3[n_meas][n_smear][T];
          double CT3_avg[n_smear][T];
          double CT3_unc[n_smear][T];

          double ****T3r;
          double ***CT3;

          allocate_4d(n_meas,n_smear,T,3,&T3r);
          allocate_3d(n_meas,n_smear,T,&CT3);

          for(int j =0; j < 1; j++){
//fread(&Br[j], sizeof(B),1,entry_file);

for(int l=0; l< n_smear;l++ ){

for (int t = 0; t < T; t++) {
 C[j][l][t] =0;
 CT1[j][l][t] =0;
 CT2[j][l][t] =0;
 CT3[j][l][t] =0;

  for (int k = 0; k < 3; k++) {
    fread(&Br[j][l][t][k],sizeof(double),1,entry_file);
 printf("B[%d][%d][%d][%d] = %f \n ", j,l,t,k, Br[j][l][t][k] );
  }
}

for (int t = 0; t < T; t++) {

  for (int k = 0; k < 3; k++) {
  fread(&T1r[j][l][t][k],sizeof(double),1,entry_file);
   printf("T1[%d][%d][%d][%d] = %f \n ", j,l,t,k, T1r[j][l][t][k] );
}
}
for (int t = 0; t < T; t++) {
  for (int k = 0; k < 3; k++) {

   fread(&T2r[j][l][t][k],sizeof(double),1,entry_file);

     printf("T2[%d][%d][%d][%d] = %f \n", j,l,t,k, T2r[j][l][t][k] );
}
}
for (int t = 0; t < T; t++) {
  for (int k = 0; k < 3; k++) {
   fread(&T3r[j][l][t][k],sizeof(double),1,entry_file);

   printf("T3[%d][%d][%d][%d] = %f \n", j,l,t,k, T3r[j][l][t][k] );
}
}
  // printf("%f\n",Br[j][l][t][k] );




for (int t = 0; t < T; t++) {
  for (int t1 = 0; t1 < T; t1++){
    for (int k = 0; k < 3; k++) {

      int t_pr = t1 +t;
if(t_pr > T -1){
C[j][l][t] += Br[j][l][t1][k]*Br[j][l][t_pr-T][k];
CT1[j][l][t] += T1r[j][l][t1][k]*T1r[j][l][t_pr-T][k];
CT2[j][l][t] += T2r[j][l][t1][k]*T2r[j][l][t_pr-T][k];
CT3[j][l][t] += T3r[j][l][t1][k]*T3r[j][l][t_pr-T][k];
printf("%f\n", CT1[j][l][t]);

} else{
  C[j][l][t] += Br[j][l][t1][k]*Br[j][l][t_pr][k];
  CT1[j][l][t] += T1r[j][l][t1][k]*T1r[j][l][t_pr][k];
  CT2[j][l][t] += T2r[j][l][t1][k]*T2r[j][l][t_pr][k];
  CT3[j][l][t] += T3r[j][l][t1][k]*T3r[j][l][t_pr][k];
printf("%f\n", CT1[j][l][t]);

}
  }
printf("\n");
}
C[j][l][t]  = (double) C[j][l][t]/T;
CT1[j][l][t]  = (double) CT1[j][l][t]/T;
CT2[j][l][t]  = (double) CT2[j][l][t]/T;
CT3[j][l][t]  = (double) CT3[j][l][t]/T;
}
}
}


free_4d(n_meas,n_smear,T,3,&Br);


free_4d(n_meas,n_smear,T,3,&T1r);


free_4d(n_meas,n_smear,T,3,&T2r);


free_4d(n_meas,n_smear,T,3,&T3r);


for(int l=0; l< n_smear;l++ ){

for (int t = 0; t < T; t++) {
//C_avg[t] = average(n_meas,C[t]);
//printf("C[%d]: %f\n",t,  C_avg[t]);
//C_unc[t] = sigma0(n_meas,C[t]);
 C_avg[l][t]=0;
 C_unc[l][t]=0;
 CT1_avg[l][t]=0;
 CT1_unc[l][t]=0;
 CT2_avg[l][t]=0;
 CT2_unc[l][t]=0;
 CT3_avg[l][t]=0;
 CT3_unc[l][t]=0;
for(int j =0; j < n_meas; j++){
C_avg[l][t]+= C[j][l][t];
CT1_avg[l][t]+= CT1[j][l][t];
CT2_avg[l][t]+= CT2[j][l][t];
CT3_avg[l][t]+= CT3[j][l][t];
}


C_avg[l][t] = C_avg[l][t]/ n_meas;
CT1_avg[l][t] = CT1_avg[l][t]/ n_meas;
CT2_avg[l][t] = CT2_avg[l][t]/ n_meas;
CT3_avg[l][t] = CT3_avg[l][t]/ n_meas;

for(int j =0; j < n_meas; j++){
C_unc[l][t] += (C[j][l][t] - C_avg[l][t])*(C[j][l][t] - C_avg[l][t]);
CT1_unc[l][t] += (CT1[j][l][t] - CT1_avg[l][t])*(CT1[j][l][t] - CT1_avg[l][t]);
CT2_unc[l][t] += (CT2[j][l][t] - CT2_avg[l][t])*(CT2[j][l][t] - CT2_avg[l][t]);
CT3_unc[l][t] += (CT3[j][l][t] - CT3_avg[l][t])*(CT3[j][l][t] - CT3_avg[l][t]);

}
//printf("C_avg[%d]: %f\n",t,cavg );

C_unc[l][t] = sqrt(C_unc[l][t]/((n_meas-1.)*n_meas));
CT1_unc[l][t] = sqrt(CT1_unc[l][t]/((n_meas-1.)*n_meas));
CT2_unc[l][t] = sqrt(CT2_unc[l][t]/((n_meas-1.)*n_meas));
CT3_unc[l][t] = sqrt(CT3_unc[l][t]/((n_meas-1.)*n_meas));

}
}

free_3d(n_meas,n_smear,T,&C);
free_3d(n_meas,n_smear,T,&CT1);
free_3d(n_meas,n_smear,T,&CT2);
free_3d(n_meas,n_smear,T,&CT3);

chdir("/home/vincenzo/Analyzer");


for(int l=0; l< n_smear;l++ ){
  fprintf(O1minusf, "%s ",in_file->d_name);
  fprintf(O1minusf, "B " );
  fprintf(O1minusf, "%d ",l);
for (int t = 0; t < T; t++) {
fprintf(O1minusf, "%f ",C_avg[l][t] );
fprintf(O1minusf, "%f ",C_unc[l][t] );
}
fprintf(O1minusf, "\n" );
}



for(int l=0; l< n_smear;l++ ){
  fprintf(O1minusf, "%s ",in_file->d_name);
  fprintf(O1minusf, "T1 " );
  fprintf(O1minusf, "%d ",l);
for (int t = 0; t < T; t++) {
fprintf(O1minusf, "%f ",CT1_avg[l][t] );
fprintf(O1minusf, "%f ",CT1_unc[l][t] );
}
fprintf(O1minusf, "\n" );
}


for(int l=0; l< n_smear;l++ ){
  fprintf(O1minusf, "%s ",in_file->d_name);
  fprintf(O1minusf, "T2 " );
  fprintf(O1minusf, "%d ",l);
for (int t = 0; t < T; t++) {
fprintf(O1minusf, "%f ",CT2_avg[l][t] );
fprintf(O1minusf, "%f ",CT2_unc[l][t] );
}
fprintf(O1minusf, "\n" );
}


for(int l=0; l< n_smear;l++ ){
  fprintf(O1minusf, "%s ",in_file->d_name);
  fprintf(O1minusf, "T3 " );
  fprintf(O1minusf, "%d ",l);
for (int t = 0; t < T; t++) {
fprintf(O1minusf, "%f ",CT3_avg[l][t] );
fprintf(O1minusf, "%f ",CT3_unc[l][t] );
}
fprintf(O1minusf, "\n" );
}


}

fclose(O1minusf);

}


void allocate_4d(int count3, int count2, int count1, int count0, double *****data)
{
    int i, j, k, l;
    *data = (double ****)malloc(count3*sizeof((*data)));
    if ((*data) != NULL) {
        for(i = 0; i < count3; i++) {
            (*data)[i] = (double ***)malloc(count2*sizeof((*data)));
            if ((*data)[i] != NULL) {
                for(j = 0; j < count2; j++) {
                    (*data)[i][j] = (double **)malloc(count1*sizeof((*data)));
                    if ((*data)[i][j] != NULL) {
                        for (k = 0; k < count1; k++) {
                            (*data)[i][j][k] = (double *)malloc(count0*sizeof((*data)));
                            if ((*data)[i][j][k] == NULL) {
                               printf("Mem allocation failed\n");
                               exit(1);
                            }
                        }
                    }
                    else {
                         printf("Mem allocation failed\n");
                         exit(1);
                         }
                }
            }
            else {
                 printf("Mem allocation failed\n");
                 exit(1);
                 }
        }
    }
    else {
          printf("Mem allocation failed\n");
          exit(1);
    }
}
    void allocate_3d(int count2, int count1, int count0, double ****data)
    {
        int i, j, k;

        *data = (double ***)malloc(count2*sizeof((*data)));
        if ((*data) != NULL) {
            for(i = 0; i < count2; i++) {
                (*data)[i] = (double **)malloc(count1*sizeof((*data)));
                if ((*data)[i] != NULL) {
                    for(j = 0; j < count1; j++) {
                        (*data)[i][j] = (double *)malloc(count0*sizeof((*data)));
                        if ((*data)[i][j] == NULL) {
                           printf("Mem allocation failed\n");
                           exit(1);
                        }
                    }
                }
                else {
                     printf("Mem allocation failed\n");
                     exit(1);
                     }
            }
        }
        else {
              printf("Mem allocation failed\n");
              exit(1);
        }


}


void free_4d(int count3, int count2, int count1, int count0, double *****data)
{
    int i, j, k;

        for(i = 0; i < count3; i++) {
                for(j = 0; j < count2; j++) {
                      for (k = 0; k < count1; k++) {
                        free((*data)[i][j][k]);
                        }
                      }
                    }


for(i = 0; i < count3; i++) {
        for(j = 0; j < count2; j++) {
                    free((*data)[i][j]);
}
}

for(i = 0; i < count3; i++) {
                free ((*data)[i]);
}
              free(*data);


}


void free_3d(int count2, int count1, int count0, double ****data)
{
    int i, j, k;

  //  *data = (double ***)malloc(count2*sizeof((*data)));
  for(i = 0; i < count2; i++) {
    for(j = 0; j < count1; j++) {
                free((*data)[i][j]);
  }
  }

  for(i = 0; i < count2; i++) {
            free ((*data)[i]);
  }

          free(*data);


}
