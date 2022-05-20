#include <vector>
#include <string>
#include <iostream>
#include <utility>
#include <fstream>
#include <chrono>

class TItem {
public:
    bool operator == (const TItem & rhs) const;
    bool operator > (const TItem & rhs) const;
    bool operator < (const TItem & rhs) const;
    std::string key;
    unsigned long long value;
    TItem();
    TItem(std::string _key, unsigned long long _val);
    TItem(const TItem & item);
    friend std::ostream &operator<<(std::ostream &output, const TItem &itemOut);
};

TItem::TItem() {
    this->key = "";
    this->value = 0;
}

TItem::TItem(const TItem &item) {
    this->key = item.key;
    this->value = item.value;
}

bool TItem::operator==(const TItem &rhs) const {
    return (this->key == rhs.key);
}

bool TItem::operator>(const TItem &rhs) const {
    return (this->key.compare(rhs.key) > 0);
}

bool TItem::operator<(const TItem &rhs) const {
    return (this->key.compare(rhs.key) < 0);
}

TItem::TItem(std::string _key, unsigned long long _val) {
    key = std::move(_key);
    value = _val;
}

std::ostream &operator<<(std::ostream &output, const TItem &itemOut) {
    output << "{'" << itemOut.key << "'" << " " << itemOut.value << "}";
    return output;
}

class TNode {

public:
    std::vector<TItem> currentNode;
    std::vector<TNode*> child;
    void Traverse();
    int t;
    int n;
    bool isLeaf;
    TNode(int _t, bool _leaf);
    TNode *Search(TItem findingItem);
    void AddNodeNonFull(TItem addItem);
    void SplitChild(int i, TNode *y);
    void Remove(TItem removeItem);
    int FindKey(TItem removeItem);
    void RemoveFromLeaf(int idx);
    void RemoveFromNonLeaf(int idx);
    TItem GetPred(int idx);
    TItem GetSucc(int idx);
    void FillNode(int idx);
    void BorrowFromPrev(int idx);
    void BorrowFromNext(int idx);
    void Merge(int idx);
};

TNode::TNode(int _t, bool _leaf) {
    t = _t;
    isLeaf = _leaf;
    TItem tItem("", 0);
    std::vector<TItem> temp(2 * t - 1, tItem);
    std::vector<TNode*> nulled(2 * t);
    currentNode = temp;
    child = nulled;
    n = 0;
}

TNode *TNode::Search(TItem findingItem) {
    int i = 0;
    while(i < n && findingItem > currentNode[i]){
        ++i;
    }
    if(currentNode[i] == findingItem){
        return this;
    }
    if(isLeaf){
        return nullptr;
    }
    return child[i]->Search(findingItem);
}

void TNode::AddNodeNonFull(TItem addItem) {
    int i = n-1;
    if(isLeaf){
        while(i >= 0 && addItem < currentNode[i]){
            currentNode[i + 1] = currentNode[i];
            --i;
        }
        currentNode[i+1] = addItem;
        ++n;
    } else{
        while(i >= 0 && addItem < currentNode[i]){
            --i;
        }
        if(child[i + 1]->n == 2 * t - 1){
            SplitChild(i + 1, child[i + 1]);
            if(addItem > currentNode[i + 1]) {
                ++i;
            }
        }
        child[i + 1]->AddNodeNonFull(addItem);
    }
}

void TNode::SplitChild(int i, TNode *y) {
    auto* newNode = new TNode(y->t, y->isLeaf);
    newNode->n = t - 1;
    for (int j = 0; j < t - 1; ++j){
        newNode->currentNode[j] = y->currentNode[j + t];
    }
    if(!y->isLeaf){
        for (int j = 0; j < t; ++j){
            newNode->child[j] = y->child[j + t];
        }
    }
    y->n = t - 1;
    for(int j = n; j >= i + 1; --j){
        child[j + 1] = child[j];
    }
    child[i + 1] = newNode;
    for (int j = n - 1; j >= i; --j) {
        currentNode[j + 1] = currentNode[j];
    }
    currentNode[i] = y->currentNode[t - 1];
    n = n + 1;
}

void TNode::Traverse() {
    int i;
    for (i = 0; i < n; ++i)
    {
        if (!isLeaf)
            child[i]->Traverse();
        std::cout << " " << currentNode[i];
    }

    if (!isLeaf) {
        child[i]->Traverse();
    }
}

void TNode::Remove(TItem removeItem) {
    int idx = FindKey(removeItem);
    if(idx < n && currentNode[idx] == removeItem){
        if(isLeaf){
            RemoveFromLeaf(idx);
        } else{
            RemoveFromNonLeaf(idx);
        }
    } else{
        if(isLeaf){
            return;
        }
        bool flag = (idx == n);
        if(child[idx]->n < t){
            FillNode(idx);
        }
        if(flag && idx > n){
            child[idx - 1]->Remove(removeItem);
        } else {
            child[idx]->Remove(removeItem);
        }
    }
}

void TNode::RemoveFromLeaf(int idx) {
    for(int i = idx; i < n - 1; ++i){
        currentNode[i] = currentNode[i + 1];
    }
    --n;
}

void TNode::RemoveFromNonLeaf(int idx) {
    TItem k = currentNode[idx];
    if(child[idx]->n >= t){
        TItem pred = GetPred(idx);
        currentNode[idx] = pred;
        child[idx]->Remove(pred);
    } else if (child[idx + 1]->n >= t){
        TItem succ = GetSucc(idx);
        currentNode[idx] = succ;
        child[idx + 1]->Remove(succ);
    } else{
        Merge(idx);
        child[idx]->Remove(k);
    }
}

TItem TNode::GetPred(int idx) {
    TNode *cur = child[idx];
    while(!cur->isLeaf) {
        cur = cur->child[cur->n];
    }
    return cur->currentNode[cur->n - 1];
}

TItem TNode::GetSucc(int idx) {
    TNode *cur = child[idx + 1];
    while(!cur->isLeaf) {
        cur = cur->child[0];
    }
    return cur->currentNode[0];
}

void TNode::FillNode(int idx) {
    if(idx != 0 && child[idx - 1]->n >= t) {
        BorrowFromPrev(idx);
    }
    else if(idx != n && child[idx + 1]->n >= t) {
        BorrowFromNext(idx);
    }
    else{
        if(idx != n) {
            Merge(idx);
        } else {
            Merge(idx - 1);
        }
    }
}

int TNode::FindKey(TItem removeItem) {
    int idx = 0;
    while(idx < n && currentNode[idx] < removeItem) {
        ++idx;
    }
    return idx;
}

void TNode::BorrowFromPrev(int idx) {
    TNode *c = child[idx];
    TNode *brother = child[idx - 1];
    for(int i = c->n - 1; i >= 0; --i){
        c->currentNode[i + 1] = c->currentNode[i];
    }
    if(!c->isLeaf){
        for(int i = c->n; i >= 0; --i){
            c->child[i + 1] = c->child[i];
        }
    }
    c->currentNode[0] = currentNode[idx - 1];
    if(!c->isLeaf) {
        c->child[0] = brother->child[brother->n];
    }
    currentNode[idx - 1] = brother->currentNode[brother->n-1];
    c->n += 1;
    brother->n -= 1;
}

void TNode::BorrowFromNext(int idx) {
    TNode *c = child[idx];
    TNode *brother = child[idx + 1];
    c->currentNode[c->n] = currentNode[idx];
    if(!c->isLeaf) {
        c->child[c->n + 1] = brother->child[0];
    }
    currentNode[idx] = brother->currentNode[0];
    for(int i = 1; i < brother->n; ++i) {
        brother->currentNode[i - 1] = brother->currentNode[i];
    }
    if(!brother->isLeaf){
        for(int i = 1; i <= brother->n; ++i) {
            brother->child[i - 1] = brother->child[i];
        }
    }
    c->n += 1;
    brother->n -= 1;
}

void TNode::Merge(int idx) {
    TNode *c = child[idx];
    TNode *brother = child[idx + 1];

    c->currentNode[t - 1] = currentNode[idx];
    for(int i = 0; i < brother->n; ++i){
        c->currentNode[i + t] = brother->currentNode[i];
    }
    if(!c->isLeaf){
        for(int i = 0; i <= brother->n; ++i){
            c->child[i + t] = brother->child[i];
        }
    }
    for(int i = idx + 1; i < n; ++i){
        currentNode[i - 1] = currentNode[i];
    }
    for(int i = idx + 2; i <= n; ++i){
        child[i - 1] = child[i];
    }
    c->n += brother->n + 1;
    --n;
    delete(brother);
}


class TBtree {
    TNode *root;
    int t;

public:
    TBtree(int _t);
    void Add(const TItem& addItem, std::string flag);
    TNode* Search(TItem findingItem);
    void Traverse();
    void Remove(TItem removeItem);
    void WriteTree(TNode* curNode, std::ofstream &file);
    void ReadTree(std::ifstream& source);
    void CountNumber(TNode* curr, int& number);
    void CountElementsTree(int& number);
    void WriteTreeToFile(std::ofstream &file);
    void DeleteTree(TNode* root);
};


TBtree::TBtree(int _t) {
    root = nullptr;
    t = _t;
}

TNode *TBtree::Search(TItem findingItem) {
    if(root == nullptr){
        return nullptr;
    }
    return root->Search(findingItem);
}

void TBtree::Add(const TItem& addItem, std::string flag) {
    if(root == nullptr){
        root = new TNode(t, true);
        root->currentNode[0] = addItem;
        root->n = 1;
    } else{
        if(root->n == 2 * t - 1){
            auto *newNode = new TNode(t, false);
            newNode->child[0] = root;
            newNode->SplitChild(0, root);
            int i = 0;
            if(newNode->currentNode[i] < addItem)
                ++i;
            newNode->child[i]->AddNodeNonFull(addItem);
            root = newNode;
        } else{
            root->AddNodeNonFull(addItem);
        }
    }
    if (flag == "ReadingAdd"){
        return;
    }
    std::cout << "OK" << std::endl;
}

void TBtree::Traverse() {
    if(root != nullptr) {
        root->Traverse();
    }
}

void TBtree::Remove(TItem removeItem) {
    if(!root){
        return;
    }
    root->Remove(removeItem);
    if(root->n == 0){
        TNode *tmp = root;
        if(root->isLeaf){
            root = nullptr;
        } else{
            root = root->child[0];
        }
        delete tmp;
    }
    std::cout << "OK" << std::endl;
}

void TBtree::WriteTree(TNode* curNode, std::ofstream &file) {
    for(int i = 0; i < curNode->n; ++i){
        int size = curNode->currentNode[i].key.size();
        file.write(reinterpret_cast<const char *>(&size), sizeof(int));
        file.write(curNode->currentNode[i].key.c_str(), size);
        file.write(reinterpret_cast<const char *>(&curNode->currentNode[i].value),
                   sizeof(unsigned long long));
        file.flush();
    }
    if(curNode->child[0] != nullptr){
        for(int i = 0; i < curNode->n; ++i){
            WriteTree(curNode->child[i], file);
        }
    }
}

void TBtree::ReadTree(std::ifstream& source) {
    DeleteTree(root);
    root = nullptr;
    int amount = 0;
    source.read(reinterpret_cast<char *>(&amount), sizeof(int));
    for(int i = 0; i < amount; ++i){
        int size = 0;
        std::string key;
        unsigned long long value = 0;
        source.read(reinterpret_cast<char *>(&size), sizeof(int));
        key.resize(size);
        source.read(key.data(), size);
        source.read(reinterpret_cast<char *>(&value),
                    sizeof(unsigned long long ));
        TItem toAdd(key, value);
        this->Add(toAdd, "ReadingAdd");
    }
}

void TBtree::CountNumber(TNode* curr, int &number) {
    if(curr == nullptr){
        return;
    }
    number += curr->n;
    for(int i = 0; i < curr->n; ++i){
        CountNumber(curr->child[i], number);
    }
}

void TBtree::CountElementsTree(int &number) {
    CountNumber(root, number);
}

void TBtree::WriteTreeToFile(std::ofstream &file) {
    WriteTree(root, file);
}

void TBtree::DeleteTree(TNode *root) {
    if (root == nullptr) {
        return;
    }
    if (!root->isLeaf) {
        for (int i = 0; i < 2 * t + 1; ++i) {
            if (root->child[i] != nullptr) {
                DeleteTree(root->child[i]);
                root->child[i] = nullptr;
            }
        }
    }
    delete root;
}


int main() {
    std::string command;
    TBtree t(70);
    unsigned amount = 0;
    std::cin >> amount;
    std::chrono::steady_clock::time_point pushStart = std::chrono::steady_clock::now();
    for (unsigned i = 0; i < amount; ++i) {
        std::string key;
        unsigned long long value;
        std::cin >> key >> value;
        std::for_each(key.begin(), key.end(), [](char &c) {
            c = ::tolower(c);
        });
        TItem toAdd(key, value);
        if (t.Search(toAdd) != nullptr) {
            std::cout << "Exist" << std::endl;
        } else {
            t.Add(toAdd, "");
        }
    }
    std::chrono::steady_clock::time_point pushFinish = std::chrono::steady_clock::now();
    unsigned timeOfPush = std::chrono::duration_cast<std::chrono::milliseconds>(pushFinish - pushStart).count();

    std::chrono::steady_clock::time_point findStart = std::chrono::steady_clock::now();
    for(unsigned i = 0; i < amount; ++i){
        std::for_each(command.begin(), command.end(), [](char & c) {
                c = ::tolower(c);
        });
        TItem toFind(command, 0);
        TNode* found = t.Search(toFind);
        if(found != nullptr){
            std::cout << "OK: " << std::find(found->currentNode.begin(),
                                             found->currentNode.end(), toFind)->value << std::endl;
        }
        else{
            std::cout << "NoSuchWord" << std::endl;
        }
    }
    std::chrono::steady_clock::time_point findFinish = std::chrono::steady_clock::now();
    unsigned timeOfFind = std::chrono::duration_cast<std::chrono::milliseconds>(findFinish - findStart).count();

    std::chrono::steady_clock::time_point popStart = std::chrono::steady_clock::now();
    for(unsigned i = 0; i < amount; ++i){
        std::string key;
        std::cin >> key;
        std::for_each(key.begin(), key.end(), [](char & c) {
            c = ::tolower(c);
        });
        TItem toRemove(key, 0);
        if(t.Search(toRemove) == nullptr){
            std::cout << "NoSuchWord" << std::endl;
        } else{
            t.Remove(toRemove);
        }

    }
    std::chrono::steady_clock::time_point popFinish = std::chrono::steady_clock::now();
    unsigned timeOfPop = std::chrono::duration_cast<std::chrono::milliseconds>(popFinish - popStart).count();

    std::cout << "!!!!! Time to push " << amount << " elements: " << timeOfPush << std::endl;
    std::cout << "!!!!! Time to find " << amount << " elements: " << timeOfFind << std::endl;
    std::cout << "!!!!! Time to pop " << amount << " elements: " << timeOfPop << std::endl;
    return 0;
}



