tree: struct {
    val: int = 0
    left: tree = null
    right: tree = null
}

size (arr: [tree]): uint {
#cpp
    return arr.size();
#cpp
}

pushb (arr: [tree], val: tree) {
#cpp
    arr.pushb(val);
#cpp
}

popf (arr: [tree]) {
#cpp
    arr.popf();
#cpp
}

_insert (root: tree, val: int): tree {
    ? root == null {
        r: tree
        r::val = val
        root = r
    } | root::val > val {
        root::left = _insert (root::left) val
    } | {
        root::right = _insert (root::right) val
    }

    return root
}

insert (root: tree, val: int) {
    root = _insert root val
}

printPreOrder (root: tree) {
    ? root == null {
        return
    }

    dump root::val
    dump ' '

    printPreOrder (root::left)
    printPreOrder (root::right)
}

printInOrder (root: tree) {
    ? root == null {
        return
    }

    printInOrder (root::left)

    dump root::val
    dump ' '

    printInOrder (root::right)
}

printPostOrder (root: tree) {
    ? root == null {
        return
    }

    printPostOrder (root::left)
    printPostOrder (root::right)

    dump root::val
    dump ' '
}

printLevelOrder (root: tree) {
    queue: [tree]
    pushb queue root

    @ size queue {
        root = queue[0]
#cpp
        printf("%lld ", root->val);
#cpp
        popf queue

        left: tree = root::left
        right: tree = root::right

        ? left != null {
            pushb queue left
        }

        ? right != null {
            pushb queue right
        }
    }
}

invert (root: tree) {
    ? root::left == root::right {
        return
    }

    tmp: int = root::left::val
    root::left::val = root::right::val
    root::right::val = tmp

    invert (root::left)
    invert (root::right)
}

free (root: tree) {
    ? root == null {
        return
    }

    free root::left
    free root::right

    delete root
}

main {
    t: tree = new tree
    t::val = 4

    insert t 2
    insert t 6
    insert t 1
    insert t 3
    insert t 5
    insert t 7

    printPreOrder t
    dumpln ""

    printInOrder t
    dumpln ""

    printPostOrder t
    dumpln ""

    printLevelOrder t
    dumpln ""

    invert t
    printLevelOrder t
    dumpln ""

    free t
}
