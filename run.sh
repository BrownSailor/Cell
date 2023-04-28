./slang $1 | llc -filetype=obj - -o main.o && clang -o main main.o lib.o && ./main
rm main.o
rm main
