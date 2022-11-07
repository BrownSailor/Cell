import subprocess
from os import listdir
from os.path import isfile

def test_program(file):
    print(f"\nTesting {file}")
    name = "tests/out/" + file[file.index('/') + 1:file.index('.')] + ".t"
    orig = "tests/chk/" + file[file.index('/') + 1:file.index('.')] + ".t"
    subprocess.run(["./slang", "main", file])
    subprocess.run(["mkdir", "-p", "tests/out"])
    subprocess.run(["touch", name])
    subprocess.run([f"./main > {name}"], shell=True)

    ret = subprocess.call([f"diff {orig} {name} > /dev/null"], shell=True)
    if ret != 0:
        print("Test failed")


def clean():
    subprocess.run(["rm", "tests/out/*.t"])


def main():
    files = [file for file in listdir("tests") if isfile(f"tests/{file}")]
    
    for file in files:
        test_program(f"tests/{file}")


main()