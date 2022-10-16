node: struct {
    val: int = 0
    next: node
}

printList (root: node) {

    @ tmp: node = root, tmp != null, tmp = tmp::next {
        dump tmp::val
        dump ' '
    }
    dumpln ""
}

main {

    root: node
    head: node = root

    @ i: int = 1, i <= 10, i++ {
#cpp
        head->next = new node;
#cpp
        head::next::val = i
        head = head::next
    }

    printList (root)

    return 0
}
