```
Course project of CSE221 Data Structures, UNIST
```

 In binary search trees, each node stores only one item, and the heightof the tree is afactor of ***log* n**. Such trees are inefficient when stored ina secondary storage device (e.g. hard disk) having slow access speed, because nodes that must be read to search are spread all around the address space. Therefore, one way to reduce the heightof the tree is to define these trees in a specific range, whichis referred to as a B tree.

# 1. Code Description

**About class `BPlusTree` in `bplustree.h`**

`BPlusTree(std::size_t degree)` : Constructor. Make a B+ tree with the given degree

`~BPlusTree()` : Destructor.

`void search(T data)` : Search a data in the tree

` void insert(T data)` : Insert a datain the tree

`void remove(T data)`: Remove a data in the tree

`intrange_search(Tstart, Tend, T* result_data, int arr_length)` : 

- Return the number of data between startand end. If the number of data is more than arr_length, return -1.

- arr_lengthis the length of the array result_data.

- After executing this function, result_datashould have all data which are between startand end.

- The range is a **closed interval**. This means that startand endare also in range.It can also be expressed as **[start, end]**.

# 2. Report

**All about b tree. (b-tree, b+tree, b*tree)**

The B tree is useful in places such as file systems and data bases. The B+ tree we implemented above complements the slow key traversal in the B tree.Therefore, in this report, we will examine various types of B trees.

## 0. Introduce

Before studying B tree, there were various trees such as binary tree and red-black tree. However, these trees are tree height is not certain how shall be moved to retrieve the data and predictions are impossible. If the amount of data can be put into memory at a time, it doesn't matter if you don't predict how much node searches. However, if there are millions or tens of millions of data, some of the data is loaded into memory and searched, if not, the next node is loaded and searched.


B tree is designed to hold large amounts of data on a single node for this purpose. It also consists of a structure that limits the maximum size of a node to the number of allocated data in memory at a time. In addition, b tree to retrieve the data equally by maintaining the height of the tree completely how movement must be predictable. Due to these advantages, B tree is actually being useful in places like file systems and databases.


## 1. B-Tree

### 1) Definition

> 1. Every node has at most *m* children.
> 2. Every non-leaf node (except root) has at least ⌈*m*/2⌉ child nodes.
> 3. The root has at least two children if it is not a leaf node.
> 4. A non-leaf node with *k* children contains *k* − 1 keys.
> 5. All leaves appear in the same level and carry no information.

- Internal node : Internal nodes are all nodes except for leaf nodes and the root node. They are usually represented as an ordered set of elements and child pointers. Each node have a **maximum of m** and a **minimum of m/2** **sub-trees**. Ensure that **at least half of each node of the tree is filled with key values**. Therefore, it allows key values **to be distributed to sub-trees as efficiently as possible** without generating nodes without sub-trees.

- Root node : The root node's number of children has the same upper limit as internal nodes, but has **no lower limit**. 

- Leaf node : The number of keys in **each Leaf Node is at least (m/2)-1, maximum m-1**.

  

### 2) Algorithm
**Search**
The search is performed in the same way as the binary search tree. Starting from the root, the process is to find the child pointer relative to the value of the target you want to explore in a top-down manner and the key value of each node.

**Insertion**
1. Explore where the node to be inserted will be inserted.
2. Insert into the corresponding parent node.
3. End the insertion process if there are no nodes that are in a state that is out of line with the node structure rules.
4. If a node is in a state that goes against the node structure rules, rearrange the tree to follow the rules through **separation**.

**Remove**

1. Explore where the node to be inserted will be removed.
2. remove into the corresponding parent node.
3. End the remove process if there are no nodes that are in a state that is out of line with the node structure rules.
4. If a node is in a state that goes against the node structure rules, rearrange the tree to follow the rules through **merge**.

### 3) Application

The B-tree has significant **advantages** over other implementations **if node access time is much longer than the computational time on the node**. This is common when most nodes are in **secondary storage**, such as **hard disks**. By maximizing the number of child nodes on each internal node, the height of the tree is reduced, balancing is less, and efficiency is increased. Usually this value is set so that each node occupies a single complete disk block or similar size in a secondary storage device.



## 2. B+Tree

### 1) Definition

The deformation structure of the B-tree consists of the index part and the sequential data part consisting of the leaf node. The key value in the index part is used to locate the key value in the leaf directly, and all key values are listed in the leaf node. In other words, the key value of the index part is also listed once again on the leaf node. Leaf nodes are configured sequentially with linked lists, enabling sequential processing.

- It consists of index node (index set) and data node (sequence set).

  - index node :  Path information for the Leaf node, reappears in the Leaf node. Key values only.
  - Data node :Leaf node, Key value and address pair - 쌍 Sequential access and direct access are both possible.

- Insertion and deletion and other features are almost similar to B-trees.


### 2) Algorithm

**Search**

All internal nodes in the tree represent the range of leaf nodes. We start from root and compare with the value and key value of the internal node we want to find, and follow the child pointer on each node and arrive at the leaf node.

**Insertion**

It's almost the same as B-trees.

1. Explore where the node to be inserted will be inserted. ( go to leaf node)

2. End the insertion process if there are no nodes that are in a state that is out of line with the node structure rules.

3. If a node is in a state that goes against the node structure rules, rearrange the tree to follow the rules through **separation**.

   - When a node splits, the middle key value should not only be raised to the parent node, but should also be included in the newly fragmented node.

   - The new node must also be inserted into the linked list between leaf nodes.

**Remove**

1. Explore where the node to be inserted will be removed.( go to leafr node)
2. End the remove process if there are no nodes that are in a state that is out of line with the node structure rules.
3. If a node is in a state that goes against the node structure rules, rearrange the tree to follow the rules through **merge**.
   - When a node splits, the middle key value should not only be raised to the parent node, but should also be included in the newly fragmented node.
   - The new node must also be inserted into the linked list between leaf nodes.

### 3) B-tree vs B+tree

#### Common Feature 

- All leaf node have same depth.
- Ecah node contain from k/2 to k items.
- The Algorithm is similar. 
  - Search is similar
  - Insertion, if overflow occurs during insert, then split the node
  - Remove, if underflow occurs during remove, then merge the node or redistribution node.

#### Differences

Data is stored on B-trees and each node, but in B+trees, index nodes and leaf nodes exist separately. And, the leafnodes are connected to each other, so the performance of random access and sequential access mode is excellent.

- On each node of the B-tree, data as well as keys can be entered. Here data can be a pointer to the disk block.
- Unlike B-trees, B+trees are only insert and delete in leafnode
- the B+tree in connected by the leaf node linked list.

#### B+tree pros and cons

**Pros**

It is very advantageous to explore the sequential range. - Leaf nodes are all connected.

**Cons**

For B Tree, the best case can end at the root, but for B+ Tree, it must go to the leaf node.



## 3. B*Tree

### 1) Definition

One access to each node in the B-tree is the same as one access to the block from the disk. Therefore, creating a small number of nodes is a way to increase the efficiency of the algorithm. **Thus, a B*Tree was created to reduce the number of nodes being created.**

B-Trees require auxiliary operations, such as division in the insertion process and merger in the remove process, to maintain their characteristics. **The B* Tree tried to reduce the collection by delaying such auxiliary operations as much as possible.**

- All nodes except the Root node must be filled at least 2/3 full. (B-Tree is at least 1/2)
- The B* Tree tries to reduce the auxiliary operation by reducing the division of the node. Therefore, when the nodes are full, they are relocated to neighboring brother nodes instead of dividing them.
  -  Relocation is the act of listing the keys of the parent node + the corresponding node + the sibling node, then sending the middle key value to the parent node and placing the remaining keys in half on the corresponding node and the sibling node.
- Delay division until one node is full and all adjacent nodes are full.

Therfore, the average storage utilization rate of the B* tree is 81%. 

### 2) Algorithm

**Insertion**

1. Insert it in the same way as in B-Tree.
2. When the nodes are full, inspect the neighboring sibling nodes and arrange and relocate any vacancies.
3. If a key overflow occurs on adjacent nodes and there are no more vacancies, divide the two full nodes and make them into three nodes that are about 2/3 full. 
   - In this process, relocation occurs twice (replacement between the first and second nodes and relocation between the second and third nodes when splitting the rich two nodes into three nodes).

**Remove**

If the number of keys is insufficient after deleting just like B-Tree, relocate them from the neighboring sibling node and merge them if they cannot be relocated. When merging, three nodes are merged into two nodes.



> ### Reference 
>
> - Wikipedia B-tree - https://en.wikipedia.org/wiki/B-tree
> - Wikipedia B+tree - https://en.wikipedia.org/wiki/B%2B_tree
> - 코딩과학 디모[youtube] - https://www.youtube.com/watch?v=Pnjlwy5o8fk
> - B-trees visualization https://www.cs.usfca.edu/~galles/visualization/BTree.html
> - B-tree 개념정리, jlog - https://hyungjoon6876.github.io/jlog/2018/07/20/btree.html
> - [잉구블로그]https://wangin9.tistory.com/entry/B-tree-B-tree
> - [HOONB]https://hoonb.tistory.com/6 
> - T.SteveTV님[youtube]https://www.youtube.com/watch?v=b2Ly05Fn7ks

