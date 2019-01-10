
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

  /* This is just a sample code, modify it to meet your need */
  int main(int argc, char **argv)
  {
      DIR* FD;
      struct dirent* in_file;
      FILE    *statf;

      FILE    *entry_file;
      FILE    *checkf;
      FILE    *polyf;
      FILE    *actf;
      FILE    *plaqf;
      FILE    *phipf;
  //    char    buffer[BUFSIZ];

      struct stat st;
      char check_name[512];
      char poly_name[512];
      char act_name[512];
      char plaq_name[512];
      char phip_name[512];


      int size;

      int n_meas;

      int n_file=0;

  //    double *phi2r;
      double *checkr;
  //    double *hopr;
      double *polyr;
      double *actr;
      double *plaqr;
      double *phipr;

  //    double phi2_avg;
      double check_avg;
  //    double hop_avg;
    double poly_avg;
    double act_avg;
    double plaq_avg;
    double phip_avg;

  //    double phi2_err;
      double check_err;
  //    double hop_err;
  double poly_err;
  double act_err;
  double plaq_err;
  double phip_err;
     printf("e\n");

      /* Opening common file for writing */
      statf = fopen("stat_results.dat", "w");
    //  checkf = fopen("check.dat","w");
      if (statf == NULL)
      {
          fprintf(stderr, "Error : Failed to open stat_results - %s\n", strerror(errno));

          return 1;
      }

      /* Scanning the in directory */
      if (NULL == (FD = opendir ("/hosts/nashome/vaf/Analyzer/output_files")))
      {
          fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
          fclose(statf);

          return 1;
      }
      while ((in_file = readdir(FD)))
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
          chdir("/hosts/nashome/vaf/Analyzer/output_files");

          printf("%s\n", in_file->d_name);
          entry_file = fopen(in_file->d_name, "rb");

          if (entry_file == NULL)
          {
              fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
              fclose(statf);

              return 1;
          }

          stat(in_file->d_name, &st);
          size = st.st_size;

          n_file ++;
          printf("%d\n",n_file );

          sprintf(check_name, "check_%s.dat", in_file->d_name);
          //printf("%s\n",check_name );
          sprintf(poly_name, "poly_%s.dat", in_file->d_name);
          sprintf(act_name, "act_%s.dat", in_file->d_name);
          sprintf(plaq_name, "plaq_%s.dat", in_file->d_name);
          sprintf(phip_name, "phip_%s.dat", in_file->d_name);


          checkf = fopen(check_name,"wr");
          polyf = fopen(poly_name,"wr");
          actf = fopen(act_name,"wr");
          plaqf = fopen(plaq_name,"wr");
          phipf = fopen(phip_name,"wr");

// n_meas is the total size divided by the number of observable time the size of a double

         n_meas = (int) size/ (5.*sizeof(double));
         printf("n meas=%d\n",n_meas );
         //phi2r = (double*)calloc(n_meas, sizeof(double));
         //hopr = (double*)calloc(n_meas, sizeof(double));
         checkr = (double*)calloc(n_meas, sizeof(double));
         polyr = (double*)calloc(n_meas, sizeof(double));
         actr = (double*)calloc(n_meas, sizeof(double));
         plaqr = (double*)calloc(n_meas, sizeof(double));
         phipr = (double*)calloc(n_meas, sizeof(double));
         //checkr = (double*)calloc(n_meas, sizeof(double));

      for(int j =0; j < n_meas; j++){
        //  fread(&phi2r[j], sizeof(double),1,entry_file);
        //  fread(&hopr[j], sizeof(double),1,entry_file);
          fread(&checkr[j], sizeof(double),1,entry_file);
          fread(&polyr[j], sizeof(double),1,entry_file);
          fread(&actr[j], sizeof(double),1,entry_file);
          fread(&plaqr[j], sizeof(double),1,entry_file);
          fread(&phipr[j], sizeof(double),1,entry_file);
}




      for(int j =0; j < n_meas; j++){
fprintf(checkf,"%f\n",checkr[j]);
fprintf(polyf,"%f\n",polyr[j]);
fprintf(actf,"%f\n",actr[j]);
fprintf(plaqf,"%f\n",plaqr[j]);
fprintf(phipf,"%f\n",phipr[j]);
}




//phi2_avg = average(n_meas,phi2r);
//hop_avg = average(n_meas,hopr);
check_avg = average(n_meas,checkr);
poly_avg = average(n_meas,polyr);
act_avg = average(n_meas,actr);
plaq_avg = average(n_meas,plaqr);
phip_avg = average(n_meas,phipr);



//phi2_err = sigma0(n_meas,phi2r);
//hop_err = sigma0(n_meas,hopr);
check_err = sigma0(n_meas,checkr);
poly_err = sigma0(n_meas,polyr);
act_err = sigma0(n_meas,actr);
plaq_err = sigma0(n_meas,plaqr);
phip_err = sigma0(n_meas,phipr);


chdir("/hosts/nashome/vaf/Analyzer");

//fprintf(statf, " %s %f %f %f %f %f %f\n",in_file->d_name, phi2_avg, phi2_err, hop_avg, hop_err, check_avg, check_err );
fprintf(statf, " %s %f %f %f %f %f %f %f %f %f %f\n",in_file->d_name,  check_avg, check_err, poly_avg, poly_err,act_avg, act_err, plaq_avg, plaq_err,phip_avg,phip_err);

          /* When you finish with the file, close it */
          fclose(entry_file);
          fclose(checkf);
          fclose(polyf);
          fclose(actf);
          fclose(plaqf);
          fclose(phipf);
      }



      /* Don't forget to close common file before leaving */
      fclose(statf);

//we now open the folders containing the results of the spectroscopy


FILE    *O1minusf;
O1minusf = fopen("O1minus_results.dat", "w");
//  checkf = fopen("check.dat","w");
if (O1minusf == NULL)
{
    fprintf(stderr, "Error : Failed to open stat_results - %s\n", strerror(errno));

    return 1;
}
      if (NULL == (FD = opendir ("/hosts/nashome/vaf/Analyzer/O1minus_output_files")))
      {
          fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
          fclose(statf);

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
          chdir("/hosts/nashome/vaf/Analyzer/O1minus_output_files");

          printf("%s\n", in_file->d_name);
          entry_file = fopen(in_file->d_name, "rb");


          char aChar = (char)in_file->d_name[9];


          if (entry_file == NULL)
          {
              fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
              fclose(statf);

              return 1;
          }

          printf("a\n" );


          stat(in_file->d_name, &st);
          size = st.st_size;

          int T;
          int n_smear =5;

          if(in_file->d_name[9] == 52)(T=4);
          if(in_file->d_name[9] == 54)(T=6);
          if(in_file->d_name[9] == 56)(T=8);
          if(in_file->d_name[9] == 49){
            if(in_file->d_name[9]==50)(T=12);
          }

          double B[T][3];

          n_meas = (int) size/ (sizeof(B)*n_smear);

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
    fread(&Br[j][t][k],sizeof(double),1,entry_file);

  }
}

for (int t = 0; t < T; t++) {
  for (int k = 0; k < 3; k++) {
    for (int t1 = 0; t1 < T; t1++){
      int t_pr = t1 +t;
if(t_pr > T -1){
C[j][t] += Br[j][t1][k]*Br[j][t_pr-T][k];
//printf("%f\n", C[j][t]);

} else{
  C[j][t] += Br[j][t1][k]*Br[j][t_pr][k];
//printf("%f\n", C[j][t]);

}
  }
//printf("\n");
}
C[j][t]  = (double) C[j][t]/T;
}

}
}

for (int t = 0; t < T; t++) {
//C_avg[t] = average(n_meas,C[t]);
//printf("C[%d]: %f\n",t,  C_avg[t]);
//C_unc[t] = sigma0(n_meas,C[t]);
 C_avg[t]=0;
 C_unc[t]=0;

for(int j =0; j < n_meas; j++){
C_avg[t]+= C[j][t];
}


C_avg[t] = C_avg[t]/ n_meas;

for(int j =0; j < n_meas; j++){
C_unc[t] += C[j][t]*C[j][t] - C_avg[t]*C_avg[t];

}
//printf("C_avg[%d]: %f\n",t,cavg );

C_unc[t] = sqrt(C_unc[t]/(n_meas-1.));

}



chdir("/hosts/nashome/vaf/Analyzer");

fprintf(O1minusf, "%s ",in_file->d_name);

for (int t = 0; t < T; t++) {
fprintf(O1minusf, "%f ",C_avg[t] );
fprintf(O1minusf, "%f ",C_unc[t] );
}
fprintf(O1minusf, "\n" );

}

fclose(O1minusf);

FILE    *O1minustrf;
O1minustrf = fopen("O1minustr_results.dat", "w");
//  checkf = fopen("check.dat","w");
if (O1minustrf == NULL)
{
    fprintf(stderr, "Error : Failed to open stat_results - %s\n", strerror(errno));

    return 1;
}
      if (NULL == (FD = opendir ("/hosts/nashome/vaf/Analyzer/O1tr_output_files")))
      {
          fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
          fclose(statf);

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
          chdir("/hosts/nashome/vaf/Analyzer/O1tr_output_files");

          printf("%s\n", in_file->d_name);
          entry_file = fopen(in_file->d_name, "rb");


          char aChar = (char)in_file->d_name[9];


          if (entry_file == NULL)
          {
              fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
              fclose(statf);

              return 1;
          }

printf("c\n" );


          stat(in_file->d_name, &st);
          size = st.st_size;

          int T;

          if(in_file->d_name[9] == 52)(T=4);
          if(in_file->d_name[9] == 54)(T=6);
          if(in_file->d_name[9] == 56)(T=8);
          if(in_file->d_name[9] == 49){
            if(in_file->d_name[9]==50)(T=12);
          }

          double Btr[T][3];

          n_meas = (int) size/ (sizeof(Btr));

          //Br = (double*)calloc(n_meas, sizeof(B));
          double Btrr[n_meas][T][3];
          double Ctr[n_meas][T];
          double Ctr_avg[T];
          double Ctr_unc[T];

printf("c1\n" );

          for(int j =0; j < n_meas; j++){
//fread(&Br[j], sizeof(B),1,entry_file);

for (int t = 0; t < T; t++) {
 Ctr[j][t] =0;
int g;
  for (int k = 0; k < 3; k++) {
    if(fread(&Btrr[j][t][k],sizeof(double),1,entry_file) ==1)(g =0);

  }
}

for (int t = 0; t < T; t++) {
  for (int k = 0; k < 3; k++) {
    for (int t1 = 0; t1 < T; t1++){
      int t_pr = t1 +t;
if(t_pr > T -1){
Ctr[j][t] += Btrr[j][t1][k]*Btrr[j][t_pr-T][k];
//printf("%f\n", C[j][t]);

} else{
  Ctr[j][t] += Btrr[j][t1][k]*Btrr[j][t_pr][k];
//printf("%f\n", C[j][t]);

}
  }
//printf("\n");
}
Ctr[j][t]  = (double) Ctr[j][t]/T;
}

}
printf("c2\n" );
for (int t = 0; t < T; t++) {
//C_avg[t] = average(n_meas,C[t]);
//printf("C[%d]: %f\n",t,  C_avg[t]);
//C_unc[t] = sigma0(n_meas,C[t]);
 Ctr_avg[t]=0;
 Ctr_unc[t]=0;

for(int j =0; j < n_meas; j++){
Ctr_avg[t]+= Ctr[j][t];
}


Ctr_avg[t] = Ctr_avg[t]/ n_meas;

for(int j =0; j < n_meas; j++){
Ctr_unc[t] += Ctr[j][t]*Ctr[j][t] - Ctr_avg[t]*Ctr_avg[t];

}
//printf("C_avg[%d]: %f\n",t,cavg );

Ctr_unc[t] = sqrt(Ctr_unc[t]/(n_meas-1.));

}

printf("c21\n" );


chdir("/hosts/nashome/vaf/Analyzer");

fprintf(O1minustrf, "%s ",in_file->d_name);
printf("c22\n" );
for (int t = 0; t < T; t++) {
fprintf(O1minustrf, "%f ",Ctr_avg[t] );
fprintf(O1minustrf, "%f ",Ctr_unc[t] );
}
fprintf(O1minustrf, "\n" );

}

fclose(O1minustrf);

FILE    *SC1f;
SC1f = fopen("SC1_results.dat", "w");
//  checkf = fopen("check.dat","w");
if (SC1f == NULL)
{
    fprintf(stderr, "Error : Failed to open stat_results - %s\n", strerror(errno));

    return 1;
}
      if (NULL == (FD = opendir ("/hosts/nashome/vaf/Analyzer/SC1_output_files")))
      {
          fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
          fclose(statf);

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
          chdir("/hosts/nashome/vaf/Analyzer/SC1_output_files");
printf("c23\n" );
          printf("%s\n", in_file->d_name);
          entry_file = fopen(in_file->d_name, "rb");

printf("c3\n" );

          char aChar = (char)in_file->d_name[9];


          if (entry_file == NULL)
          {
              fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
              fclose(statf);

              return 1;
          }

printf("d\n" );


          stat(in_file->d_name, &st);
          size = st.st_size;

          int T;

          if(in_file->d_name[9] == 52)(T=4);
          if(in_file->d_name[9] == 54)(T=6);
          if(in_file->d_name[9] == 56)(T=8);
          if(in_file->d_name[9] == 49){
            if(in_file->d_name[9]==50)(T=12);
          }

          double SC1[T];

          n_meas = (int) size/ (sizeof(SC1));

          //Br = (double*)calloc(n_meas, sizeof(B));
          double SC1r[n_meas][T];
          double C1[n_meas][T];
          double C1_avg[T];
          double C1_unc[T];


          for(int j =0; j < n_meas; j++){
//fread(&Br[j], sizeof(B),1,entry_file);

for (int t = 0; t < T; t++) {
 C1[j][t] =0;
int g;
    if(fread(&SC1r[j][t],sizeof(double),1,entry_file) ==1)(g =0);
}

for (int t = 0; t < T; t++) {
    for (int t1 = 0; t1 < T; t1++){
      int t_pr = t1 +t;
if(t_pr > T -1){
C1[j][t] += SC1r[j][t1]*SC1r[j][t_pr-T];
//printf("%f\n", C[j][t]);

} else{
  C1[j][t] += SC1r[j][t1]*SC1r[j][t_pr];
//printf("%f\n", C[j][t]);

}
//printf("\n");
}
C1[j][t]  = (double) C1[j][t]/T;
}

}

for (int t = 0; t < T; t++) {
//C_avg[t] = average(n_meas,C[t]);
//printf("C[%d]: %f\n",t,  C_avg[t]);
//C_unc[t] = sigma0(n_meas,C[t]);
 C1_avg[t]=0;
 C1_unc[t]=0;

for(int j =0; j < n_meas; j++){
C1_avg[t]+= C1[j][t];
}


C1_avg[t] = C1_avg[t]/ n_meas;

for(int j =0; j < n_meas; j++){
C1_unc[t] += C1[j][t]*C1[j][t] - C1_avg[t]*C1_avg[t];

}
//printf("C_avg[%d]: %f\n",t,cavg );

C1_unc[t] = sqrt(C1_unc[t]/(n_meas-1.));

}



chdir("/hosts/nashome/vaf/Analyzer");

fprintf(SC1f, "%s ",in_file->d_name);

for (int t = 0; t < T; t++) {
fprintf(SC1f, "%f ",C1_avg[t] );
fprintf(SC1f, "%f ",C1_unc[t] );
}
fprintf(SC1f, "\n" );

}

fclose(SC1f);

FILE    *SC2f;
SC2f = fopen("SC2_results.dat", "w");

if (SC2f == NULL)
{
    fprintf(stderr, "Error : Failed to open stat_results - %s\n", strerror(errno));

    return 1;
}
      if (NULL == (FD = opendir ("/hosts/nashome/vaf/Analyzer/SC2_output_files")))
      {
          fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
          fclose(statf);

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
          chdir("/hosts/nashome/vaf/Analyzer/SC2_output_files");

          printf("%s\n", in_file->d_name);
          entry_file = fopen(in_file->d_name, "rb");


          char aChar = (char)in_file->d_name[9];


          if (entry_file == NULL)
          {
              fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
              fclose(statf);

              return 1;
          }



          stat(in_file->d_name, &st);
          size = st.st_size;

          int T;

          if(in_file->d_name[9] == 52)(T=4);
          if(in_file->d_name[9] == 54)(T=6);
          if(in_file->d_name[9] == 56)(T=8);
          if(in_file->d_name[9] == 49){
            if(in_file->d_name[9]==50)(T=12);
          }

          double SC2[T];

          n_meas = (int) size/ (sizeof(SC2));

          //Br = (double*)calloc(n_meas, sizeof(B));
          double SC2r[n_meas][T];
          double C2[n_meas][T];
          double C2_avg[T];
          double C2_unc[T];


          for(int j =0; j < n_meas; j++){
//fread(&Br[j], sizeof(B),1,entry_file);

for (int t = 0; t < T; t++) {
 C2[j][t] =0;
int g;
    if(fread(&SC2r[j][t],sizeof(double),1,entry_file) ==1)(g =0);
}

for (int t = 0; t < T; t++) {
    for (int t1 = 0; t1 < T; t1++){
      int t_pr = t1 +t;
if(t_pr > T -1){
C2[j][t] += SC2r[j][t1]*SC2r[j][t_pr-T];
//printf("%f\n", C[j][t]);

} else{
  C2[j][t] += SC2r[j][t1]*SC2r[j][t_pr];
//printf("%f\n", C[j][t]);

}
//printf("\n");
}
C2[j][t]  = (double) C2[j][t]/T;
}

}

for (int t = 0; t < T; t++) {
//C_avg[t] = average(n_meas,C[t]);
//printf("C[%d]: %f\n",t,  C_avg[t]);
//C_unc[t] = sigma0(n_meas,C[t]);
 C2_avg[t]=0;
 C2_unc[t]=0;

for(int j =0; j < n_meas; j++){
C2_avg[t]+= C2[j][t];
}


C2_avg[t] = C2_avg[t]/ n_meas;

for(int j =0; j < n_meas; j++){
C2_unc[t] += C2[j][t]*C2[j][t] - C2_avg[t]*C2_avg[t];

}
//printf("C_avg[%d]: %f\n",t,cavg );

C2_unc[t] = sqrt(C2_unc[t]/(n_meas-1.));

}



chdir("/hosts/nashome/vaf/Analyzer");

fprintf(SC2f, "%s ",in_file->d_name);

for (int t = 0; t < T; t++) {
fprintf(SC2f, "%f ",C2_avg[t] );
fprintf(SC2f, "%f ",C2_unc[t] );
}
fprintf(SC2f, "\n" );

}

fclose(SC2f);

FILE    *SC3f;
SC3f = fopen("SC3_results.dat", "w");

if (SC3f == NULL)
{
    fprintf(stderr, "Error : Failed to open stat_results - %s\n", strerror(errno));

    return 1;
}
      if (NULL == (FD = opendir ("/hosts/nashome/vaf/Analyzer/SC3_output_files")))
      {
          fprintf(stderr, "Error : Failed to open input directory - %s\n", strerror(errno));
          fclose(statf);

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
          chdir("/hosts/nashome/vaf/Analyzer/SC3_output_files");

          printf("%s\n", in_file->d_name);
          entry_file = fopen(in_file->d_name, "rb");


          char aChar = (char)in_file->d_name[9];


          if (entry_file == NULL)
          {
              fprintf(stderr, "Error : Failed to open entry file - %s\n", strerror(errno));
              fclose(statf);

              return 1;
          }



          stat(in_file->d_name, &st);
          size = st.st_size;

          int T;

          if(in_file->d_name[9] == 52)(T=4);
          if(in_file->d_name[9] == 54)(T=6);
          if(in_file->d_name[9] == 56)(T=8);
          if(in_file->d_name[9] == 49){
            if(in_file->d_name[9]==50)(T=12);
          }

          double SC3[T];

          n_meas = (int) size/ (sizeof(SC3));

          //Br = (double*)calloc(n_meas, sizeof(B));
          double SC3r[n_meas][T];
          double C3[n_meas][T];
          double C3_avg[T];
          double C3_unc[T];


          for(int j =0; j < n_meas; j++){
//fread(&Br[j], sizeof(B),1,entry_file);

for (int t = 0; t < T; t++) {
 C3[j][t] =0;
int g;
    if(fread(&SC3r[j][t],sizeof(double),1,entry_file) ==1)(g =0);
}

for (int t = 0; t < T; t++) {
    for (int t1 = 0; t1 < T; t1++){
      int t_pr = t1 +t;
if(t_pr > T -1){
C3[j][t] += SC3r[j][t1]*SC3r[j][t_pr-T];
//printf("%f\n", C[j][t]);

} else{
  C3[j][t] += SC3r[j][t1]*SC3r[j][t_pr];
//printf("%f\n", C[j][t]);

}
//printf("\n");
}
C3[j][t]  = (double) C3[j][t]/T;
}

}

for (int t = 0; t < T; t++) {
//C_avg[t] = average(n_meas,C[t]);
//printf("C[%d]: %f\n",t,  C_avg[t]);
//C_unc[t] = sigma0(n_meas,C[t]);
 C3_avg[t]=0;
 C3_unc[t]=0;

for(int j =0; j < n_meas; j++){
C3_avg[t]+= C3[j][t];
}


C3_avg[t] = C3_avg[t]/ n_meas;

for(int j =0; j < n_meas; j++){
C3_unc[t] += C3[j][t]*C3[j][t] - C3_avg[t]*C3_avg[t];

}
//printf("C_avg[%d]: %f\n",t,cavg );

C3_unc[t] = sqrt(C3_unc[t]/(n_meas-1.));

}



chdir("/hosts/nashome/vaf/Analyzer");

fprintf(SC3f, "%s ",in_file->d_name);

for (int t = 0; t < T; t++) {
fprintf(SC3f, "%f ",C3_avg[t] );
fprintf(SC3f, "%f ",C3_unc[t] );
}
fprintf(SC3f, "\n" );

}

fclose(SC3f);

      return 0;
  }
