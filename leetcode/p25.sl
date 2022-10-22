node: struct {
    val: int = 0
    next: node = null
}

add (root: node, val: int): node {
    ? root == null {
        root = new node
        root::val = val
    } | {
        head: node = root
        @ head::next != null {
            head = head::next
        }
        head::next = new node
        head::next::val = val
    }

    return root
}

print (root: node) {
    @ tmp: node = root, tmp != null, tmp = tmp::next {
        dump tmp::val
        dump ' '
    }
    dumpln ""
}

reverseKHelper (root: node, num: int, size: int): node {
    ? root == null || size < num {
        return root
    }

    prev: node = null
    curr: node = root
    next: node = curr::next

    @ i: int = 0, i < num && curr != null, i++ {
        curr::next = prev
        prev = curr
        curr = next
        next = next::next
    }

    root::next = reverseKHelper curr num (size - num)
    return prev
}

reverseKGroup (root: node, k: int): node {
    size: int = 0
    @ tmp: node = root, tmp != null, tmp = tmp::next {
        size++
    }

    return reverseKHelper root k size
}

solve (root: node, k: int) {
    root = reverseKGroup root k
    print root
}

main {
    l1: node = add l1 1
    l1 = add l1 2
    l1 = add l1 3
    l1 = add l1 4
    l1 = add l1 5

    solve l1 2

    l2: node = add l2 1
    l2 = add l2 2
    l2 = add l2 3 
    l2 = add l2 4
    l2 = add l2 5

    solve l2 3
}