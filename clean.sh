cd output_files
echo `rm poly*`
echo `rm plaq*`
echo `rm act*`
echo `rm check*`
echo `rm phip*`
echo `rm *.dat`
echo `rm -f -r .nfs*`
cd ..
gcc -o analyzer analyzer.c base_stat.c -lm
