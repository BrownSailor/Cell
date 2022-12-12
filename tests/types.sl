node T: type = {
    val: T
    next: node T
}

list T: type = {
    front: node T
    back: node T
    size: uint
}

queue T: type = {
    data: list T
}

stack T: type = {
    data: [T]
}

size (arr: [char]): uint {
#cpp
    return arr.size();
#cpp
}

pushb (arr: [char], val: char) {
#cpp
    arr.pushb(val);
#cpp
}

popb (arr: [char]): char {
    val: char = arr[size arr - 1]
#cpp
    arr.popb();
#cpp
    return val
}

size (stk: stack char): uint {
    return size stk.data
}

push (stk: stack char, val: char) {
    pushb stk.data val
}

pop (stk: stack char): char {
    return popb stk.data
}

top (stk: stack char): char {
    return stk.data[size stk - 1]
}

pair K V: type = {
    key: K
    val: V
}

map K V: type = {
    data: [pair K V]
}

solve: bool {
    s: stack char = new stack char
    t: str = "()()()()()"

    @ i: int = 0, i < size t - 1, i++ {
        ? t[i] == '(' {
            push s t[i]
        } | pop s != '(' {
            return fls
        }
    }

    return size s == 0
}

main {
    dumpln solve
}
