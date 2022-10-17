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

addTwoNums (l1: node, l2: node): node {
    ans: node = new node
    head: node = ans

    carry: int = 0
    @ l1 != null && l2 != null {
        head::val = l1::val + l2::val + carry

        carry = head::val / 10
        head::val = head::val % 10

        l1 = l1::next
        l2 = l2::next

        ? l1 != null || l2 != null {
            head::next = new node
            head = head::next
        }
    }

    @ l1 != null {
        head::val = l1::val + carry
        carry = head::val / 10
        head::val = head::val % 10

        l1 = l1::next

        ? l1 != null {
            head::next = new node
            head = head::next
        }
    }

    @ l2 != null {
        head::val = l2::val + carry
        carry = head::val / 10
        head::val = head::val % 10

        l2 = l2::next

        ? l2 != null {
            head::next = new node
            head = head::next
        }
    }
    
    ? carry {
        head::next = new node
        head::next::val = carry
    }

    return ans
}

print (root: node) {
    @ tmp: node = root, tmp != null, tmp = tmp::next {
        dump tmp::val
        dump ' '
    }
    dumpln ""
}

solve (l1: node, l2: node) {
    print l1
    print l2
    ans: node = addTwoNums l1 l2
    print ans
}

main {
    l1: node = add l1 2
    l1 = add l1 4
    l1 = add l1 3

    l2: node = add l2 5
    l2 = add l2 6
    l2 = add l2 4

    solve l1 l2

    l3: node = add l3 9
    l3 = add l3 9
    l3 = add l3 9
    l3 = add l3 9
    l3 = add l3 9
    l3 = add l3 9
    l3 = add l3 9

    l4: node = add l4 9
    l4 = add l4 9
    l4 = add l4 9
    l4 = add l4 9

    solve l3 l4

    l5: node = add l5 0
    l6: node = add l6 0

    solve l5 l6
}