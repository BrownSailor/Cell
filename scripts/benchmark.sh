benchmark=benchmark
data=$benchmark/data
c=$benchmark/c
sl=$benchmark/sl

rm -rf $data
mkdir $data

for f in $sl/*
do
    file=${f:13:-3}
    touch $data/$file

    echo "--C--" > $data/$file
    echo "Compiling executable" >> $data/$file
    (time clang -O3 $c/${f:13:-3}.c -o main 2> /dev/null) |& grep 'real' | awk '{ print $2 }' >> $data/$file
    echo "" >> $data/$file
    
    echo "Running executable" >> $data/$file
    (time ./main > /dev/null) |& grep 'real' | awk '{ print $2 }' >> $data/$file
    echo "" >> $data/$file

    echo "--Slang--" >> $data/$file
    echo "Compiling executable" >> $data/$file
    (time (./slang $f | clang -O3 -x ir - -o main)) |& grep 'real' | awk '{ print $2 }' >> $data/$file
    echo "" >> $data/$file
    
    echo "Running executable" >> $data/$file
    (time ./main > /dev/null) |& grep 'real' | awk '{ print $2 }' >> $data/$file
    echo "" >> $data/$file

done

rm main
