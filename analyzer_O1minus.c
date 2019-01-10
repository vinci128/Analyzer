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

          printf("a\n" );


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

          double B[T][3];

          int n_meas = (int) size/ (sizeof(B)*n_smear);

          //Br = (double*)calloc(n_meas, sizeof(B));
          double Br[n_meas][n_smear][T][3];
          double C[n_meas][n_smear][T];
          double C_avg[n_smear][T];
          double C_unc[n_smear][T];

printf("b\n" );
          for(int j =0; j < n_meas; j++){
//fread(&Br[j], sizeof(B),1,entry_file);

for(int l=0; l< n_smear;l++ ){

for (int t = 0; t < T; t++) {
 C[l][j][t] =0;

  for (int k = 0; k < 3; k++) {
    fread(&Br[j][l][t][k],sizeof(double),1,entry_file);
    printf("%f\n",Br[j][l][t][k] );

  }
}

for (int t = 0; t < T; t++) {
  for (int k = 0; k < 3; k++) {
    for (int t1 = 0; t1 < T; t1++){
      int t_pr = t1 +t;
if(t_pr > T -1){
C[l][j][t] += Br[j][l][t1][k]*Br[j][l][t_pr-T][k];
//printf("%f\n", C[j][t]);

} else{
  C[l][j][t] += Br[j][l][t1][k]*Br[j][l][t_pr][k];
//printf("%f\n", C[j][t]);

}
  }
//printf("\n");
}
C[l][j][t]  = (double) C[l][j][t]/T;
}

}
}

for(int l=0; l< n_smear;l++ ){

for (int t = 0; t < T; t++) {
//C_avg[t] = average(n_meas,C[t]);
//printf("C[%d]: %f\n",t,  C_avg[t]);
//C_unc[t] = sigma0(n_meas,C[t]);
 C_avg[l][t]=0;
 C_unc[l][t]=0;

for(int j =0; j < n_meas; j++){
C_avg[l][t]+= C[l][j][t];
}


C_avg[l][t] = C_avg[l][t]/ n_meas;

for(int j =0; j < n_meas; j++){
C_unc[l][t] += C[l][j][t]*C[l][j][t] - C_avg[l][t]*C_avg[l][t];

}
//printf("C_avg[%d]: %f\n",t,cavg );

C_unc[l][t] = sqrt(C_unc[l][t]/(n_meas-1.));

}
}

chdir("/home/vincenzo/Analyzer");

fprintf(O1minusf, "%s ",in_file->d_name);
for(int l=0; l< n_smear;l++ ){
for (int t = 0; t < T; t++) {
fprintf(O1minusf, "%f ",C_avg[l][t] );
fprintf(O1minusf, "%f ",C_unc[l][t] );
}
fprintf(O1minusf, "\n" );
}
fprintf(O1minusf, "\n" );
}

fclose(O1minusf);

}
