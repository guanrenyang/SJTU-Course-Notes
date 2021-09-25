#include<bits/stdc++.h>
using namespace std;


// ==================== trie (prefix tree) ======================
struct TrieNode {
    bool flag=0;
    map<char, TrieNode*> next;
};
class Trie {
public:
    TrieNode* root;
    Trie();
    
    // Inserts a word into the trie.
    void insert(string word);
    void insert(string word,TrieNode * & item);
    // Returns true if the word is in the trie.
    bool search(string word);
    bool search(string word,TrieNode * item);
    // Returns true if there is any word in the trie that starts with the given prefix.
    bool startsWith(string prefix);
    bool startsWith(string prefix,TrieNode * item);
};
Trie::Trie()
{
    this->root=new TrieNode;
}
void Trie::insert(string word)
{
    insert(word,this->root);
}
bool Trie::search(string word)
{
    return search(word,this->root);
}
bool Trie::startsWith(string word)
{
    return startsWith(word,this->root);
}
void Trie::insert(string word,TrieNode * & item)
{
    int isExist=((item->next).count('a'));
    TrieNode * temp;
    switch(isExist)
    {
        case 0:
            temp=new TrieNode;
            item->next.insert(pair<char,TrieNode*>(word[0],temp));
            if (word.size()==1)
                ((item->next)[word[0]])->flag=1;
            else 
            {
                insert(word.substr(1),item->next[word[0]]);
            }
            break;
        case 1:
            if(word.size()==1)
                ((item->next)[word[0]])->flag=1;
            else
                insert(word.substr(1),item->next[word[0]]);
            break;        
    }
}
bool Trie::search(string word,TrieNode * item)
{
    bool isExist=item->next.count(word[0]);

    switch(isExist)
    {
        case 0:
            return 0;
            break;
        case 1:
            if(word.size()==1)
                return ((item->next)[word[0]])->flag;
            else
                return search(word.substr(1),item->next[word[0]]);
            break;        
    }
}
bool Trie::startsWith(string word,TrieNode * item)
{
    bool isExist=item->next.count(word[0]);

    switch(isExist)
    {
        case 0:
            return 0;
            break;
        case 1:
            if(word.size()==1)
                return 1;
            else
                return startsWith(word.substr(1),item->next[word[0]]);
            break;        
    }
}
int main()
{
    Trie* obj1 = new Trie();
    obj1->insert("apple");
    cout << obj1->search("apple") << ' ';
    cout << obj1->search("app") << ' ';  // =0 because "app" is not a word, but a prefix
    cout << obj1->startsWith("app") << ' ';
    obj1->insert("app");
    cout << obj1->search("app") << ' ';  // =1 because now "app" is a inserted word
    
    cout << endl;
    // Case2: output = 0 0 1 1 1
    Trie* obj2 = new Trie();
    obj2->insert("import");
    obj2->insert("important");
    obj2->insert("importance");
    cout << obj2->search("importantly") << ' ';
    cout << obj2->startsWith("impp") << ' ';
    cout << obj2->startsWith("import") << ' ';
    cout << obj2->startsWith("importan") << ' ';
    cout << obj2->search("import") << ' ';
    
    cout << endl;
    // Case3: output = 0 1 0 1
    Trie* obj3 = new Trie();
    cout << obj3->search("cat") << ' ';
    obj3->insert("cate");
    cout << obj3->startsWith("cat") << ' ';
    obj3->insert("category");
    cout << obj3->search("cater") << ' ';
    cout << obj3->search("category") << ' ';
    
    cout << endl;
    // Case4: output = 0 0 0
    Trie* obj4 = new Trie();
    obj4->insert("bad");
    obj4->insert("dad");
    obj4->insert("mad");
    cout << obj4->search("ad") << ' ';
    cout << obj4->startsWith("ad") << ' ';
    cout << obj4->search("pad") << ' ';
    
    cout << endl;
    // Case5: output = 0 1 0
    Trie* obj5 = new Trie();
    obj5->insert("go");
    cout << obj5->startsWith("goo") << ' ';
    obj5->insert("google");
    cout << obj5->startsWith("goo") << ' ';
    cout << obj5->search("goo") << ' ';

    system("pause");
    return 0;
}

