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

    echo "--Slang--" > $data/$file
    echo "Compiling to object file" >> $data/$file
    (time ./slang $f | llc -O2 -filetype=obj - -o main.o) |& grep 'real' | awk '{ print $2 }' >> $data/$file
    echo "" >> $data/$file
    
    echo "Linking executable" >> $data/$file
    (time clang -O2 -o main main.o lib.o) |& grep 'real' | awk '{ print $2 }' >> $data/$file
    echo "" >> $data/$file
    
    echo "Running executable" >> $data/$file
    (time ./main > /dev/null) |& grep 'real' | awk '{ print $2 }' >> $data/$file
    echo "" >> $data/$file

    echo "--C--" >> $data/$file
    echo "Compiling to object file" >> $data/$file
    (time clang -O0 -c $c/${f:13:-3}.c -o main.o 2> /dev/null) |& grep 'real' | awk '{ print $2 }' >> $data/$file
    echo "" >> $data/$file
    
    echo "Linking executable" >> $data/$file
    (time clang -O0 -o main main.o) |& grep 'real' | awk '{ print $2 }' >> $data/$file
    echo "" >> $data/$file
    
    echo "Running executable" >> $data/$file
    (time ./main) |& grep 'real' | awk '{ print $2 }' >> $data/$file
    echo "" >> $data/$file
done

rm main.o
rm main
