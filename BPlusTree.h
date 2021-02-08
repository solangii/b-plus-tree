#ifndef BPlusTree_H
#define BPlusTree_H

#include <iostream>

template <typename T>
struct Node {
    bool is_leaf;
    std::size_t degree; // maximum number of children
    std::size_t size; // current number of item
    T* item;
    Node<T>** children;
    Node<T>* parent;

public:
    Node(std::size_t _degree) {// Constructor
        this->is_leaf = false;
        this->degree = _degree;
        this->size = 0;

        T* _item = new T[degree-1];
        for(int i=0; i<degree-1; i++){
            _item[i] = 0;
        }
        this->item = _item;

        Node<T>** _children = new Node<T>*[degree];
        for(int i=0; i<degree; i++){
            _children[i] = nullptr;
        }
        this->children = _children;

        this->parent = nullptr;

    }
};

template <typename T>
class BPlusTree {
    Node<T>* root;
    std::size_t degree;

public:
    BPlusTree(std::size_t _degree) {// Constructor
        this->root = nullptr;
        this->degree = _degree;
    }
    ~BPlusTree() { // Destructor
        clear(this->root);
    }

    Node<T>* getroot(){
        return this->root;
    }

    Node<T>* BPlusTreeSearch(Node<T>* node, T key){
        if(node == nullptr) { // if root is null, return nullptr
            return nullptr;
        }
        else{
            Node<T>* cursor = node; // cursor finding key

            while(!cursor->is_leaf){ // until cusor pointer arrive leaf
                for(int i=0; i<cursor->size; i++){ //in this index node, find what we want key
                    if(key < cursor->item[i]){ //find some range, and let find their child also.
                        cursor = cursor->children[i];
                        break;
                    }
                    if(i == (cursor->size)-1){
                        cursor = cursor->children[i+1];
                        break;
                    }
                }
            }

            //search for the key if it exists in leaf node.
            for(int i=0; i<cursor->size; i++){
                if(cursor->item[i] == key){
                    return cursor;
                }
            }

            return nullptr;
        }
    }
    Node<T>* BPlusTreeRangeSearch(Node<T>* node, T key){
        if(node == nullptr) { // if root is null, return nullptr
            return nullptr;
        }
        else{
            Node<T>* cursor = node; // cursor finding key

            while(!cursor->is_leaf){ // until cusor pointer arrive leaf
                for(int i=0; i<cursor->size; i++){ //in this index node, find what we want key
                    if(key < cursor->item[i]){ //find some range, and let find their child also.
                        cursor = cursor->children[i];
                        break;
                    }
                    if(i == (cursor->size)-1){
                        cursor = cursor->children[i+1];
                        break;
                    }
                }
            }
            return cursor;
        }
    }
    int range_search(T start, T end, T* result_data, int arr_length) {
        int index=0;

        Node<T>* start_node = BPlusTreeRangeSearch(this->root,start);
        Node<T>* cursor = start_node;
        T temp= cursor->item[0];

        while(temp<=end){
            if(cursor == nullptr){
                break;
            }
            for(int i=0; i< cursor->size;i++){
                temp = cursor->item[i];
                if((temp >= start)&&(temp <= end)){
                    result_data[index] = temp;
                    index++;
                }
            }
            cursor = cursor->children[cursor->size];
        }
        return index;
    }
    bool search(T data) {  // Return true if the item exists. Return false if it does not.
        return BPlusTreeSearch(this->root, data) != nullptr;
    }

    int find_index(T* arr, T data, int len){
        int index = 0;
        for(int i=0; i<len; i++){
            if(data < arr[i]){
                index = i;
                break;
            }
            if(i==len-1){
                index = len;
                break;
            }
        }
        return index;
    }
    T* item_insert(T* arr, T data, int len){
        int index = 0;
        for(int i=0; i<len; i++){
            if(data < arr[i]){
                index = i;
                break;
            }
            if(i==len-1){
                index = len;
                break;
            }
        }

        for(int i = len; i > index; i--){
            arr[i] = arr[i-1];
        }

        arr[index] = data;

        return arr;
    }
    Node<T>** child_insert(Node<T>** child_arr, Node<T>*child,int len,int index){
        for(int i= len; i > index; i--){
            child_arr[i] = child_arr[i - 1];
        }
        child_arr[index] = child;
        return child_arr;
    }
    Node<T>* child_item_insert(Node<T>* node, T data, Node<T>* child){
        int item_index=0;
        int child_index=0;
        for(int i=0; i< node->size; i++){
            if(data < node->item[i]){
                item_index = i;
                child_index = i+1;
                break;
            }
            if(i==node->size-1){
                item_index = node->size;
                child_index = node->size+1;
                break;
            }
        }
        for(int i = node->size;i > item_index; i--){
            node->item[i] = node->item[i-1];
        }
        for(int i=node->size+1;i>child_index;i--){
            node->children[i] = node->children[i-1];
        }

        node->item[item_index] = data;
        node->children[child_index] = child;

        return node;
    }
    void InsertPar(Node<T>* par,Node<T>* child, T data){
        //overflow check
        Node<T>* cursor = par;
        if(cursor->size < this->degree-1){//not overflow, just insert in the correct position
            //insert item, child, and reallocate
            cursor = child_item_insert(cursor,data,child);
            cursor->size++;
        }
        else{//overflow
            //make new node
            auto* Newnode = new Node<T>(this->degree);
            Newnode->parent = cursor->parent;

            //copy item
            T* item_copy = new T[cursor->size+1];
            for(int i=0; i<cursor->size; i++){
                item_copy[i] = cursor->item[i];
            }
            item_copy = item_insert(item_copy,data,cursor->size);

            auto** child_copy = new Node<T>*[cursor->size+2];
            for(int i=0; i<cursor->size+1;i++){
                child_copy[i] = cursor->children[i];
            }
            child_copy[cursor->size+1] = nullptr;
            child_copy = child_insert(child_copy,child,cursor->size+1,find_index(item_copy,data,cursor->size+1));

            //split nodes
            cursor->size = (this->degree)/2;
            if((this->degree) % 2 == 0){
                Newnode->size = (this->degree) / 2 -1;
            }
            else{
                Newnode->size = (this->degree) / 2;
            }

            for(int i=0; i<cursor->size;i++){
                cursor->item[i] = item_copy[i];
                cursor->children[i] = child_copy[i];
            }
            cursor->children[cursor->size] = child_copy[cursor->size];
            //todo 안지워짐. 뒤에것.

            for(int i=0; i < Newnode->size; i++){
                Newnode->item[i] = item_copy[cursor->size + i +1];
                Newnode->children[i] = child_copy[cursor->size+i+1];
                Newnode->children[i]->parent=Newnode;
            }
            Newnode->children[Newnode->size] = child_copy[cursor->size+Newnode->size+1];
            Newnode->children[Newnode->size]->parent=Newnode;

            T paritem = item_copy[this->degree/2];

            delete[] item_copy;
            delete[] child_copy;

            //parent check
            if(cursor->parent == nullptr){//if there are no parent node(root case)
                auto* Newparent = new Node<T>(this->degree);
                cursor->parent = Newparent;
                Newnode->parent = Newparent;

                Newparent->item[0] = paritem;
                Newparent->size++;

                Newparent->children[0] = cursor;
                Newparent->children[1] = Newnode;

                this->root = Newparent;

                //delete Newparent;
            }
            else{//if there already have parent node
                InsertPar(cursor->parent, Newnode, paritem);
            }
        }
    }
    void insert(T data) {
        if(this->root == nullptr){ //if the tree is empty
            this->root = new Node<T>(this->degree);
            this->root->is_leaf = true;
            this->root->item[0] = data;
            this->root->size = 1; //
        }
        else{ //if the tree has at least one node
            Node<T>* cursor = this->root;

            //move to leaf node
            cursor = BPlusTreeRangeSearch(cursor, data);

            //overflow check
            if(cursor->size < (this->degree-1)){ // not overflow, just insert in the correct position
                //item insert and rearrange
                cursor->item = item_insert(cursor->item,data,cursor->size);
                cursor->size++;
                //edit pointer(next node)
                cursor->children[cursor->size] = cursor->children[cursor->size-1];
                cursor->children[cursor->size-1] = nullptr;
            }
            else{//overflow case
                //make new node
                auto* Newnode = new Node<T>(this->degree);
                Newnode->is_leaf = true;
                Newnode->parent = cursor->parent;

                //copy item
                T* item_copy = new T[cursor->size+1];
                for(int i=0; i<cursor->size; i++){
                    item_copy[i] = cursor->item[i];
                }

                //insert and rearrange
                item_copy = item_insert(item_copy,data,cursor->size);

                //split nodes
                cursor->size = (this->degree)/2;
                if((this->degree) % 2 == 0){
                    Newnode->size = (this->degree) / 2;
                }
                else{
                    Newnode->size = (this->degree) / 2 + 1;
                }

                for(int i=0; i<cursor->size;i++){
                    cursor->item[i] = item_copy[i];
                }
                for(int i=0; i < Newnode->size; i++){
                    Newnode->item[i] = item_copy[cursor->size + i];
                }

                cursor->children[cursor->size] = Newnode;
                Newnode->children[Newnode->size] = cursor->children[this->degree-1];
                cursor->children[this->degree-1] = nullptr;

                delete[] item_copy;

                //parent check
                T paritem = Newnode->item[0];

                if(cursor->parent == nullptr){//if there are no parent node(root case)
                    auto* Newparent = new Node<T>(this->degree);
                    cursor->parent = Newparent;
                    Newnode->parent = Newparent;

                    Newparent->item[0] = paritem;
                    Newparent->size++;

                    Newparent->children[0] = cursor;
                    Newparent->children[1] = Newnode;

                    this->root = Newparent;
                }
                else{//if there already have parent node
                    InsertPar(cursor->parent, Newnode, paritem);
                }
            }
        }
    }

    void remove(T data) { // Remove an item from the tree.
        //make cursor
        Node<T>* cursor = this->root;

        //move to leaf node
        cursor = BPlusTreeRangeSearch(cursor,data);

        //make sibling index
        int sib_index =-1;
        for(int i=0; i<cursor->parent->size+1;i++){
            if(cursor == cursor->parent->children[i]){
                sib_index = i;
            }
        }
        int left=sib_index-1;
        int right=sib_index+1;


        //find data
        int del_index=-1;
        for(int i=0; i< cursor->size; i++){
            if(cursor->item[i] == data){
                del_index = i;
                break;
            }
        }
        //if data dosen't exist, nothing happen
        if(del_index==-1){
            return; // there is no match remove value
        }

        //remove data
        for(int i=del_index; i<cursor->size-1;i++){
            cursor->item[i] = cursor->item[i+1];
        }
        cursor->item[cursor->size-1] = 0;
        cursor->size--;

        //if cursor is root, and there are no more data -> clean!
        if(cursor == this->root && cursor->size==0){//root case
            clear(this->root);
            this->root = nullptr;
            return;
        }
        cursor->children[cursor->size] = cursor->children[cursor->size+1];
        cursor->children[cursor->size+1] = nullptr;


        //underflow check
        if(cursor == this->root){
            return;
        }
        if(cursor->size < degree/2){//underflow case

            if(left >= 0){// left_sibiling exists
                Node<T>* leftsibling= cursor->parent->children[left];

                if(leftsibling->size > degree/2){ //if data number is enough to use this node
                    T* temp = new T[cursor->size+1];

                    //copy item
                    for(int i=0; i<cursor->size; i++){
                        temp[i]=cursor->item[i];
                    }

                    //insert and rearrange
                    item_insert(temp,leftsibling->item[leftsibling->size -1],cursor->size);
                    for(int i=0; i<cursor->size+1; i++){
                        cursor->item[i] = temp[i];
                    }
                    cursor->size++;
                    delete[] temp;

                    //pointer edit
                    cursor->children[cursor->size] = cursor->children[cursor->size-1];
                    cursor->children[cursor->size-1] = nullptr;

                    //sibling property edit
                    leftsibling->item[leftsibling->size-1] = 0;
                    leftsibling->size--;
                    leftsibling->children[leftsibling->size] = leftsibling->children[leftsibling->size+1]; //cursor
                    leftsibling->children[leftsibling->size+1]= nullptr;

                    //parent property edit
                    cursor->parent->item[left] = cursor->item[0];

                    return;
                }
            }
            if(right <= cursor->parent->size){// right_sibiling exists
                Node<T>* rightsibling = cursor->parent->children[right];

                if(rightsibling->size >degree/2){//if data number is enough to use this node
                    T* temp = new T[cursor->size+1];

                    //copy item
                    for(int i=0; i<cursor->size; i++){
                        temp[i]=cursor->item[i];
                    }
                    //insert and rearrange
                    item_insert(temp,rightsibling->item[0],cursor->size);
                    for(int i=0; i<cursor->size+1; i++){
                        cursor->item[i] = temp[i];
                    }
                    cursor->size++;
                    delete[] temp;

                    //pointer edit
                    cursor->children[cursor->size] = cursor->children[cursor->size-1];
                    cursor->children[cursor->size-1] = nullptr;

                    //sibling property edit
                    for(int i=0; i<rightsibling->size-1;i++){
                        rightsibling->item[i] = rightsibling->item[i+1];
                    }
                    rightsibling->item[rightsibling->size-1] = 0;
                    rightsibling->size--;
                    rightsibling->children[rightsibling->size] = rightsibling->children[rightsibling->size+1]; //cursor
                    rightsibling->children[rightsibling->size+1]= nullptr;

                    //parent property edit
                    cursor->parent->item[right-1] = rightsibling->item[0];

                    return;
                }
            }

            //if sibling is not enought to use their data
            //we have to merge step

            if(left>=0){ // left_sibling exists
                Node<T>* leftsibling = cursor->parent->children[left];

                //merge two leaf node
                for(int i=0; i<cursor->size; i++){
                    leftsibling->item[leftsibling->size+i]=cursor->item[i];
                }
                //edit pointer
                leftsibling->children[leftsibling->size] = nullptr;
                leftsibling->size = leftsibling->size+cursor->size;
                leftsibling->children[leftsibling->size] = cursor->children[cursor->size];

                //parent property edit
                Removepar(cursor, left, cursor->parent);
                for(int i=0; i<cursor->size;i++){
                    cursor->item[i]=0;
                    cursor->children[i] = nullptr;
                }
                cursor->children[cursor->size] = nullptr;

                delete[] cursor->item;
                delete[] cursor->children;
                delete cursor;

                return;

            }
            if(right<=cursor->parent->size){ // right_sibiling exists
                Node<T>* rightsibling = cursor->parent->children[right];

                //merge two leaf node
                for(int i=0; i<rightsibling->size; i++){
                    cursor->item[i+cursor->size]=rightsibling->item[i];
                }
                //edit pointer
                cursor->children[cursor->size] = nullptr;
                cursor->size = rightsibling->size+cursor->size;
                cursor->children[cursor->size] = rightsibling->children[rightsibling->size];

                //parent property edit
                Removepar(rightsibling, right-1, cursor->parent);

                for(int i=0; i<rightsibling->size;i++){
                    rightsibling->item[i]=0;
                    rightsibling->children[i] = nullptr;
                }
                rightsibling->children[rightsibling->size] = nullptr;

                delete[] rightsibling->item;
                delete[] rightsibling->children;
                delete rightsibling;
                return;

            }

        }
        else{
            return;
        }
    }

    void Removepar(Node<T>* node, int index, Node<T>* par){
        Node<T>* remover = node;
        Node<T>* cursor = par;
        T target = cursor->item[index];

        //if cursor is root, and there are no more data -> child node is to be root!
        if(cursor == this->root && cursor->size==1){//root case
            if(remover == cursor->children[0]){
                delete[] remover->item;
                delete[] remover->children;
                delete remover;
                this->root = cursor->children[1];
                delete[] cursor->item;
                delete[] cursor->children;
                delete cursor;
                return;
            }
            if(remover == cursor->children[1]){
                delete[] remover->item;
                delete[] remover->children;
                delete remover;
                this->root = cursor->children[0];
                delete[] cursor->item;
                delete[] cursor->children;
                delete cursor;
                return;
            }
        }

        //remove data
        for(int i=index; i<cursor->size-1;i++){
            cursor->item[i] = cursor->item[i+1];
        }
        cursor->item[cursor->size-1] = 0;

        //remove pointer
        int rem_index = -1;
        for(int i=0; i<cursor->size+1;i++){
            if(cursor->children[i] == remover){
                rem_index = i;
            }
        }
        if(rem_index == -1){
            return;
        }
        for(int i=rem_index; i<cursor->size;i++){
            cursor->children[i] = cursor->children[i+1];
        }
        cursor->children[cursor->size] = nullptr;
        cursor->size--;

        //underflow check
        if(cursor == this->root){
            return;
        }
        if(cursor->size < degree/2){//underflow case

            int sib_index =-1;
            for(int i=0; i<cursor->parent->size+1;i++){
                if(cursor == cursor->parent->children[i]){
                    sib_index = i;
                }
            }
            int left=sib_index-1;
            int right=sib_index+1;

            if(left >= 0){// left_sibiling exists
                Node<T>* leftsibling= cursor->parent->children[left];

                if(leftsibling->size > degree/2){ //if data number is enough to use this node
                    T* temp = new T[cursor->size+1];

                    //copy item
                    for(int i=0; i<cursor->size; i++){
                        temp[i]=cursor->item[i];
                    }

                    //insert and rearrange at cursor
                    item_insert(temp, cursor->parent->item[left],cursor->size);
                    for(int i=0; i<cursor->size+1; i++){
                        cursor->item[i] = temp[i];
                    }
                    cursor->parent->item[left] = leftsibling->item[leftsibling->size-1];
                    delete[] temp;

                    Node<T>** child_temp = new Node<T>*[cursor->size+2];
                    //copy child node
                    for(int i=0; i<cursor->size+1; i++){
                        child_temp[i]=cursor->children[i];
                    }
                    //insert and rearrange at child
                    child_insert(child_temp,leftsibling->children[leftsibling->size],cursor->size,0);

                    for(int i=0; i<cursor->size+2; i++){
                        cursor->children[i] = child_temp[i];
                    }
                    delete[] child_temp;

                    //size edit
                    cursor->size++;
                    leftsibling->size--;
                    return;

                }
            }

            if(right <= cursor->parent->size){// right_sibiling exists
                Node<T>* rightsibling = cursor->parent->children[right];

                if(rightsibling->size > degree/2){//if data number is enough to use this node
                    T* temp = new T[cursor->size+1];

                    //copy item
                    for(int i=0; i<cursor->size; i++){
                        temp[i]=cursor->item[i];
                    }
                    //insert and rearrange at cursor
                    item_insert(temp,cursor->parent->item[sib_index],cursor->size);
                    for(int i=0; i<cursor->size+1; i++){
                        cursor->item[i] = temp[i];
                    }
                    cursor->parent->item[sib_index] = rightsibling->item[0];
                    delete[] temp;

                    //insert and reaarange at child

                    cursor->children[cursor->size+1] = rightsibling->children[0];
                    for(int i=0; i<rightsibling->size; i++){
                        rightsibling->children[i] = rightsibling->children[i+1];
                    }
                    rightsibling->children[rightsibling->size] = nullptr;

                    cursor->size++;
                    rightsibling->size--;
                    return;

                }
            }

            //if sibling is not enought to use their data
            //we have to merge step
            if(left>=0){ // left_sibling exists
                Node<T>* leftsibling = cursor->parent->children[left];

                leftsibling->item[leftsibling->size] = cursor->parent->item[left];
                //merge two leaf node
                for(int i=0; i<cursor->size; i++){
                    leftsibling->item[leftsibling->size+i+1]=cursor->item[i];
                }
                for(int i=0; i<cursor->size+1;i++){
                    leftsibling->children[leftsibling->size+i+1] = cursor->children[i];
                    cursor->children[i]->parent = leftsibling;
                }
                for(int i=0; i<cursor->size+1; i++){
                    cursor->children[i] = nullptr;
                }
                leftsibling->size = leftsibling->size+cursor->size+1;
                //delete recursion
                Removepar(cursor, left,cursor->parent);
                return;

            }
            if(right<=cursor->parent->size){ // right_sibiling exists
                Node<T>* rightsibling = cursor->parent->children[right];

                cursor->item[cursor->size] = cursor->parent->item[right-1];
                //merge two leaf node
                for(int i=0; i<rightsibling->size; i++){
                    cursor->item[cursor->size+1+i]=rightsibling->item[i];
                }
                for(int i=0; i<rightsibling->size+1;i++){
                    cursor->children[cursor->size+i+1] = rightsibling->children[i];
                    rightsibling->children[i]->parent=rightsibling;
                }
                for(int i=0; i<rightsibling->size+1; i++){
                    rightsibling->children[i] = nullptr;
                }
                //edit pointer
                rightsibling->size = rightsibling->size+cursor->size+1;
                //parent property edit
                Removepar(rightsibling, right-1,cursor->parent);
                return;
            }
        }
        else{
            return;
        }
    }

    void clear(Node<T>* cursor){
        if(cursor != nullptr){
            if(!cursor->is_leaf){
                for(int i=0; i <= cursor->size; i++){
                    clear(cursor->children[i]);
                }
            }
            delete[] cursor->item;
            delete[] cursor->children;
            delete cursor;
        }
    }
    void bpt_print(){
        print(this->root);
    }
    void print(Node<T>* cursor) {
        // You must NOT edit this function.
        if (cursor != NULL) {
            for (int i = 0; i < cursor->size; ++i) {
                std::cout << cursor->item[i] << " ";
            }
            std::cout << "\n";

            if (!cursor->is_leaf) {
                for (int i = 0; i < cursor->size + 1; ++i) {
                    print(cursor->children[i]);
                }
            }
        }
    }
};

#endif
