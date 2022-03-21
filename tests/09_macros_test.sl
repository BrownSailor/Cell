def write
    1 sys_write
end

"Hello World!\n" write

def N 100 end

N 0 while dup2 > do
    dup dump inc
end
