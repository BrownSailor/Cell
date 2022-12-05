node: struct {
    val: int = 0
    next: node = null
}

add (root: node, val: int) {

    ? root == null {
        root = new node
        root.val = val
    } | {
        tmp: node = root
        @ tmp.next != null, tmp = tmp.next {}
        tmp.next = new node
        tmp.next.val = val
    }
}

print (root: node) {

    @ tmp: node = root, tmp != null, tmp = tmp.next {
        dump tmp.val
        dump ' '
    }
}

free (root: node) {
    ? root == null {
        return
    }

    free root.next
    delete root.next
}

size (ls: [node]): uint {
#cpp
    return ls.size();
#cpp
}

merge2 (l1: node, l2: node): node {
    l: node = new node
    root: node = l

    t1: node = l1
    t2: node = l2

    @ t1 != null && t2 != null {
        ? t1.val <= t2.val {
            l.next = new node
            l.next.val = t1.val
            l = l.next
            t1 = t1.next
        } | {
            l.next = new node
            l.next.val = t2.val
            l = l.next
            t2 = t2.next
        }
    }

    ? t1 != null {
        @ tmp: node = t1, tmp != null, tmp = tmp.next {
            l.next = new node
            l.next.val = tmp.val
            l = l.next
        }
    } | {
        @ tmp: node = t2, tmp != null, tmp = tmp.next {
            l.next = new node
            l.next.val = tmp.val
            l = l.next
        }
    }

    return root.next
}

solve (lists: [node]): node {
    ? size lists == 0 {
        return null
    }

    // bottom up merge sort algorithm
    @ i: int = 1, i < size lists, i = i * 2 {
        @ j: int = 0, j + i < size lists, j = j + 2 * i {
            lists[j] = merge2 lists[j] lists[j + i]
        }
    }

    return lists[0]
}

main {

    l1: node = new node
    add l1 1
    add l1 4
    add l1 5
    l1 = l1.next

    l2: node = new node
    add l2 1
    add l2 3
    add l2 4
    l2 = l2.next

    l3: node = new node
    add l3 2
    add l3 6
    l3 = l3.next

    ls: [node] = { l1 l2 l3 }

    dump "[ "
    print solve ls
    dumpln "]"

    em: [node] = { null null }

    dump "[ "
    print solve em
    dumpln "]"
}