(./slang $1) | clang -O3 -x ir - -o main
./main
rm main
