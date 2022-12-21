testname=in
j=50001
echo "$((j-1)) $j $j" > $testname;
for((i=0;i<j/2;i++)); do
    echo "$((j/2-i)) $((i+1)) $((j-i-1))" >> $testname
    echo "$((j/2+i+1)) $((i+1)) $((j-i-1))" >> $testname
done
