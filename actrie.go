type Result struct {
        Sentence string
        Start    int
        Fin      int
}

type Node struct {
        Key     string
        Child   map[byte]*Node
        Failure *Node
}

type BasicTrie struct {
        Root Node
}

func DefaultConstructBasicTrie() *BasicTrie {
        var basic BasicTrie
        basic.Root.Failure = &basic.Root
        basic.Root.Child = make(map[byte]*Node)
        return &basic
}

func (ac *BasicTrie) Insert(key string) {
        root := &ac.Root
        if ac == nil || len(key) == 0 {
                return
        }
        bytes := []byte(key)

        flag := false
        for _, v := range bytes {
                if _, ok := root.Child[v]; !ok {
                        root.Child[v] = &Node{Child: make(map[byte]*Node)}
                        flag = true
                }
                root = root.Child[v]
        }

        if flag {
                root.Key = key
        }
}

func (ac *BasicTrie) SettingFailure() {
        var q []*Node
        q = append(q, &ac.Root)

        for len(q) != 0 {
                father := q[0]
                q = q[1:]
                for k, v := range father.Child {
                        q = append(q, v)
                        candidate := father.Failure
                        for {
                                if father == &ac.Root {
                                        candidate = &ac.Root
                                        break
                                }

                                if vv, ok := candidate.Child[k]; ok {
                                        candidate = vv
                                        break
                                } else {
                                        if candidate == &ac.Root {
                                                break
                                        }
                                        candidate = candidate.Failure
                                }
                        }
                        v.Failure = candidate
                }
        }
}

func (ac *BasicTrie) Search(content string) []Result {
        node := &ac.Root
        if node == nil || len(content) == 0 {
                return []Result{}
        }

        var res []Result
        bytes := []byte(content)

        length := len(bytes)
        i := 0
        for i < length {
                if v, ok := node.Child[bytes[i]]; ok {
                        temp := v.Failure
                        for temp != &ac.Root {
                                if len(temp.Key) > 0 {
                                        res = append(res, Result{Sentence: temp.Key, Start: i - len(temp.Key), Fin: i})
                                }
                                temp = temp.Failure
                        }
                        node = v
                        i++
                        if len(node.Key) > 0 {
                                res = append(res, Result{Sentence: node.Key, Start: i - len(node.Key), Fin: i})
                        }
                } else {
                        if node == &ac.Root {
                                i++
                        } else {
                                node = node.Failure
                        }
                }
        }
        return res
}
