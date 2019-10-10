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

FILE    *O0plusf;
O0plusf = fopen("O0plus_results.dat", "w");
//  checkf = fopen("check.dat","w");
if (O0plusf == NULL)
{
    fprintf(stderr, "Error : Failed to open stat_results - %s\n", strerror(errno));

    return 1;
}
      if (NULL == (FD = opendir ("/home/vincenzo/Analyzer/O0plus_output_files")))
      {
          fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
          fclose(O0plusf);

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
          chdir("/home/vincenzo/Analyzer/O0plus_output_files");

          printf("%s\n", in_file->d_name);
          entry_file = fopen(in_file->d_name, "rb");
          char aChar = (char)in_file->d_name[9];


          if (entry_file == NULL)
          {
              fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
              fclose(O0plusf);

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

          printf("Nt=%d\n",T );

          double SC1[T];


          int n_meas = (int) size/ (sizeof(SC1)*n_smear*3);

          printf("n_meas = %d \n", n_meas );

          //Br = (double*)malloc(n_meas, sizeof(B));
          //double Br[n_meas][n_smear][T][3];
          //double C[n_meas][n_smear][T];
          double C1_avg[n_smear][T];
          double C1_unc[n_smear][T];

          double ***SC1r;
          double ***C1;

          allocate_3d(n_meas,n_smear,T,&SC1r);
          allocate_3d(n_meas,n_smear,T,&C1);

          double C2_avg[n_smear][T];
          double C2_unc[n_smear][T];

          double ***SC2r;
          double ***C2;

          allocate_3d(n_meas,n_smear,T,&SC2r);
          allocate_3d(n_meas,n_smear,T,&C2);

          double C3_avg[n_smear][T];
          double C3_unc[n_smear][T];

          double ***SC3r;
          double ***C3;

          allocate_3d(n_meas,n_smear,T,&SC3r);
          allocate_3d(n_meas,n_smear,T,&C3);

          for(int j =0; j < n_meas; j++){
//fread(&Br[j], sizeof(B),1,entry_file);

for(int l=0; l< n_smear;l++ ){

for (int t = 0; t < T; t++) {
 C1[j][l][t] =0;
 C2[j][l][t] =0;
 C3[j][l][t] =0;



    fread(&SC1r[j][l][t],sizeof(double),1,entry_file);
//    printf("SC1[%d][%d][%d] = %f\n",j,l,t,SC1r[j][l][t] );
}

for (int t = 0; t < T; t++) {
    fread(&SC2r[j][l][t],sizeof(double),1,entry_file);
//    printf("SC2[%d][%d][%d] = %f\n",j,l,t,SC2r[j][l][t] );
}
for (int t = 0; t < T; t++) {
    fread(&SC3r[j][l][t],sizeof(double),1,entry_file);
//    printf("SC3[%d][%d][%d] = %f \n",j,l,t,SC3r[j][l][t] );
}
   // printf("%f\n",Br[j][l][t][k] );




for (int t = 0; t < T; t++) {
  for (int t1 = 0; t1 < T; t1++){
      int t_pr = t1 +t;
if(t_pr > T -1){
C1[j][l][t] += SC1r[j][l][t1]*SC1r[j][l][t_pr-T];
C2[j][l][t] += SC2r[j][l][t1]*SC2r[j][l][t_pr-T];
C3[j][l][t] += SC3r[j][l][t1]*SC3r[j][l][t_pr-T];
} else{
C1[j][l][t] += SC1r[j][l][t1]*SC1r[j][l][t_pr];
C2[j][l][t] += SC2r[j][l][t1]*SC2r[j][l][t_pr];
C3[j][l][t] += SC3r[j][l][t1]*SC3r[j][l][t_pr];
  }
}
C1[j][l][t]  = (double) C1[j][l][t]/T;
C2[j][l][t]  = (double) C2[j][l][t]/T;
C3[j][l][t]  = (double) C3[j][l][t]/T;
}
}
}



free_3d(n_meas,n_smear,T,&SC1r);

free_3d(n_meas,n_smear,T,&SC2r);

free_3d(n_meas,n_smear,T,&SC3r);


for(int l=0; l< n_smear;l++ ){

for (int t = 0; t < T; t++) {

 C1_avg[l][t]=0;
 C1_unc[l][t]=0;
 C2_avg[l][t]=0;
 C2_unc[l][t]=0;
 C3_avg[l][t]=0;
 C3_unc[l][t]=0;

for(int j =0; j < n_meas; j++){
C1_avg[l][t]+= C1[j][l][t];
C2_avg[l][t]+= C2[j][l][t];
C3_avg[l][t]+= C3[j][l][t];
}


C1_avg[l][t] = C1_avg[l][t]/ n_meas;
C2_avg[l][t] = C2_avg[l][t]/ n_meas;
C3_avg[l][t] = C3_avg[l][t]/ n_meas;


for(int j =0; j < n_meas; j++){
C1_unc[l][t] += (C1[j][l][t] - C1_avg[l][t])*(C1[j][l][t] - C1_avg[l][t]);
C2_unc[l][t] += (C2[j][l][t] - C2_avg[l][t])*(C2[j][l][t] - C2_avg[l][t]);
C3_unc[l][t] += (C3[j][l][t] - C3_avg[l][t])*(C3[j][l][t] - C3_avg[l][t]);
}
//printf("C_avg[%d]: %f\n",t,cavg );

C1_unc[l][t] = sqrt(C1_unc[l][t]/((n_meas-1.)*n_meas));
C2_unc[l][t] = sqrt(C2_unc[l][t]/((n_meas-1.)*n_meas));
C3_unc[l][t] = sqrt(C3_unc[l][t]/((n_meas-1.)*n_meas));


}
}

free_3d(n_meas,n_smear,T,&C1);

free_3d(n_meas,n_smear,T,&C2);

free_3d(n_meas,n_smear,T,&C3);

chdir("/home/vincenzo/Analyzer");


for(int l=0; l< n_smear;l++ ){
  fprintf(O0plusf, "%s ",in_file->d_name);
  fprintf(O0plusf, "SC1 " );
  fprintf(O0plusf, "%d ",l);
for (int t = 0; t < T; t++) {
fprintf(O0plusf, "%f ",C1_avg[l][t] );
fprintf(O0plusf, "%f ",C1_unc[l][t] );
}
fprintf(O0plusf, "\n" );
}

for(int l=0; l< n_smear;l++ ){
  fprintf(O0plusf, "%s ",in_file->d_name);
  fprintf(O0plusf, "SC2 " );
  fprintf(O0plusf, "%d ",l);
for (int t = 0; t < T; t++) {
fprintf(O0plusf, "%f ",C2_avg[l][t] );
fprintf(O0plusf, "%f ",C2_unc[l][t] );
}
fprintf(O0plusf, "\n" );
}

for(int l=0; l< n_smear;l++ ){
  fprintf(O0plusf, "%s ",in_file->d_name);
  fprintf(O0plusf, "SC3 " );
  fprintf(O0plusf, "%d ",l);
for (int t = 0; t < T; t++) {
fprintf(O0plusf, "%f ",C3_avg[l][t] );
fprintf(O0plusf, "%f ",C3_unc[l][t] );
}
fprintf(O0plusf, "\n" );
}



}
fclose(O0plusf);

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

  //  *data = (double ***)malloc(count2*sizeof((*data)));
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
