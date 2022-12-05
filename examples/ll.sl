node: struct {
    val: int = 0
    next: node = null
}

list: struct {
    front: node = null
    back: node = null
    size: int = 0
}

size (root: list): uint {
    return root.size
}

add (root: list, val: int) {

    ? root.size == 0 {
        root.front = new node
        root.front.val = val
        root.back = root.front
    } | {
        root.back.next = new node
        root.back.next.val = val
        root.back = root.back.next
    }

    root.size++
}

reverse (root: list) {
    root.back = root.front

    prev: node = null
    curr: node = root.front
    next: node = root.front.next

    @ curr != null {
        curr.next = prev
        prev = curr
        curr = next
        next = next.next
    }

    root.front = prev
}

print (root: list) {

    dump "List size: "
    dump root.size
    dumpln ""

    @ tmp: node = root.front, tmp != null, tmp = tmp.next {
        dump tmp.val
        dump ' '
    }
    dumpln ""
}

free (root: node) {
    ? root == null {
        return
    }

    free root.next
    delete root.next
}

main {
    ls: list = new list

    @ i: int = 0, i < 10, i++ {
        add ls i
    }


    print ls
    reverse ls
    print ls

    free ls.front
    delete ls
}