stack: struct {
    arr: [char]
}

size (s: [char]): uint {
#cpp
    return s.size();
#cpp
}

size (s: stack): uint {
    return size s::arr
}

push (s: stack, c: char) {
#cpp
    s->arr.pushb(c);
#cpp
}

pop (s: stack): char {
    c: char
#cpp
    c = s->arr[s->arr.size() - 1];
    s->arr.popb();
#cpp
    return c
}

validParentheses (s: [char]): bool {
    st: stack
    @ i: int = 0, i < size s, i++ {
        ? s[i] == ')' && pop st != '(' {
            return fls
        } | s[i] == ']' && pop st != '[' {
            return fls
        } | s[i] == '}' && pop st != '{' {
            return fls
        }

        ? s[i] == '(' || s[i] == '[' || s[i] == '{' {
            push st s[i]
        }
    }

    return size (st) == 0
}

solve (s: [char]) {
    ? validParentheses s {
        dumpln "tru"
    } | {
        dumpln "fls"
    }
}

main {
    s: [char] = "()"
    solve s

    s = "()[]{}"
    solve s

    s = "(])"
    solve s
}