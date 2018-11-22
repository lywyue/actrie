#ifndef BASIC_TRIE_HPP
#define BASIC_TRIE_HPP

#include <typeinfo>
#include <memory>
#include <string>
#include <map>
#include <queue>

namespace mytrie {

using uchar = unsigned char;

template<typename CharType>
class result{
public:
    std::basic_string<CharType> sentence;
    unsigned int start;
    unsigned int fin;
public:
    explicit result(){
        start = 0;
        fin = 0;
    }
    explicit result(const std::basic_string<CharType>& sentence, unsigned int start, unsigned int fin):
        sentence(sentence), start(start), fin(fin){}
};

template<typename CharType>
struct Node{
    std::basic_string<CharType> key;
    std::map<uchar, std::unique_ptr<Node<CharType>>> child;
    Node<CharType>* failure;
    explicit Node(){
        failure = nullptr;
    }
};

template<typename CharType>
class basic_trie{
private:
    std::unique_ptr<Node<CharType>> _root;
    bool module = typeid(uchar).hash_code() == typeid(CharType).hash_code() || typeid(char).hash_code() == typeid(CharType).hash_code();
public:
    basic_trie():_root(new Node<CharType>()){
        _root.get()->failure = _root.get();
    }

    void insert(const std::basic_string<CharType>& key){
        Node<CharType>* root = _root.get();
        if(nullptr == root || key.empty()){
            return ;
        }

        int len;
        if(module){
            len = key.size();
        } else {
            len = key.size() * sizeof(CharType);
        }

        uchar* p = reinterpret_cast<uchar*>(const_cast<CharType*>(key.c_str()));
        bool flag = false;
        for(auto i = 0; i < len; ++i, ++p){
            if(!root->child[*p]){
                root->child[*p] = std::unique_ptr<Node<CharType>>(new Node<CharType>());
                flag = true;
            }
            root = root->child[*p].get();
        }

        if(flag){
            root->key = key;
        }
        return ;
    }

    void settingfailure(){
        std::queue<Node<CharType>*> q;
        q.push(_root.get());

        while (!q.empty()) {
            Node<CharType>* father = q.front();
            q.pop();
            for(auto&& row : father->child){
                q.push(row.second.get());
                Node<CharType>* candidate = father->failure;
                while (true) {
                    if(father == _root.get()){
                        candidate = _root.get();
                        break;
                    }

                    if(candidate->child[row.first]){
                        candidate = candidate->child[row.first].get();
                        break;
                    } else {
                        if(candidate == _root.get()){
                            break;
                        }
                        candidate = candidate->failure;
                    }
                }

                row.second.get()->failure = candidate;
            }
        }
    }

    std::vector<result<CharType>> search(const std::basic_string<CharType>& content){
        Node<CharType>* node = _root.get();
        if(nullptr == node || content.empty()){
            return std::vector<result<CharType>>();
        }

        int len;
        if(module){
            len = content.size();
        } else {
            len = content.size() * sizeof(CharType);
        }

        uchar* p = reinterpret_cast<uchar*>(const_cast<CharType*>(content.c_str()));
        std::vector<result<CharType>> res;

        int i = 0;
        while(i < len){
            if(node->child[*p]){
                Node<CharType>* temp = node->child[*p].get()->failure;
                while (temp != _root.get()) {
                    if(!temp->key.empty()){
                        res.emplace_back(result<CharType>(temp->key, i - temp->key.size() * sizeof(CharType), i));
                    }
                    temp = temp->failure;
                }
                node = node->child[*p].get();
                ++p;
                ++i;
                if(!node->key.empty()){
                    res.emplace_back(result<CharType>(node->key, i - node->key.size() * sizeof(CharType), i));
                }
            } else {
                if(node == _root.get()){
                    ++p;
                    ++i;
                } else {
                    node = node->failure;
                }
            }
        }
        return res;
    }
};

}

#endif // BASIC_TRIE_HPP
