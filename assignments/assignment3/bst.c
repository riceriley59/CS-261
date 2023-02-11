/*
 * This file is where you should implement your binary search tree.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name: Riley Rice
 * Email: riceri@oregonstate.edu
 */

#include <stdlib.h>

#include "bst.h"
#include "stack.h"

/*
 * This structure represents a single node in a BST.  In addition to containing
 * pointers to its two child nodes (i.e. `left` and `right`), it contains two
 * fields representing the data stored at this node.  The `key` field is an
 * integer value that should be used as an identifier for the data in this
 * node.  Nodes in the BST should be ordered based on this `key` field.  The
 * `value` field stores data associated with the key.
 *
 * You should not modify this structure.
 */
struct bst_node {
  int key;
  void* value;
  struct bst_node* left;
  struct bst_node* right;
};


/*
 * This structure represents an entire BST.  It specifically contains a
 * reference to the root node of the tree.
 *
 * You should not modify this structure.
 */
struct bst {
  struct bst_node* root;
};

/*
 * This function should allocate and initialize a new, empty, BST and return
 * a pointer to it.
 */
struct bst* bst_create() {
  //create new bst pointer and initialize the root to null
  struct bst* bst = malloc(sizeof(struct bst));
  bst->root = NULL;

  //return that pointer
  return bst;
}

/*
 * This function takes in the root node of a BST and then recursively iterates
 * though it in post-order. This means it will hit the left sub-node, right sub-node,
 * and then the root node and it will do this for every tree. When getting to the nodes
 * it will free the nodes memory in the same order that I explained previously.
 *
 * Params:
 *   root - the root of every sub-tree to be destroyed starting at the whole tree's
 *    root.
 */
void free_tree(struct bst_node* root){
  //base case - we have hit a leaf node
  if(root == NULL){
    return;
  }

  //recurse down left sub-tree
  free_tree(root->left);

  //recurse down right sub-tree
  free_tree(root->right);

  //free nodes in post-order
  free(root);
  root = NULL;
}

/*
 * This function should free the memory associated with a BST.  While this
 * function should up all memory used in the BST itself, it should not free
 * any memory allocated to the pointer values stored in the BST.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   bst - the BST to be destroyed.  May not be NULL.
 */
void bst_free(struct bst* bst) {
  //call function to free all nodes
  free_tree(bst->root);

  //free the bst data structure
  free(bst);
  bst = NULL;

  return;
}

/*
 * This functions count's the amount of nodes in the binary tree. It 
 * recursively iterates through summing the nodes in the left and right 
 * sub-trees plus 1 for the root node of every tree.
 *
 * Params:
 *   root - the root of every sub-tree to be counted starting at the whole tree's
 *    root.
 */
int count_tree(struct bst_node* root){
  if(root == NULL){ //base case - we hit a leaf node
    return 0;
  }else{
    //return the count of all the nodes in the left and right sub trees plus 1 for the root
    return 1 + count_tree(root->left) + count_tree(root->right);
  }
}

/*
 * This function should return the total number of elements stored in a given
 * BST.
 *
 * Params:
 *   bst - the BST whose elements are to be counted.  May not be NULL.
 */
int bst_size(struct bst* bst) {
  //return the count of nodes from the count_tree function giving it the root
  return count_tree(bst->root);
}

/*
 * This functions adds a new node to the tree in the correct position. It will
 * recursively iterate through the tree going through the left or right sub-tree
 * based on the key value until it finds the right spot, meaning it's at a leaf
 * where root will be null then return that to the last function call setting the 
 * root's right or left pointer equal to that newnode.
 *
 * Params:
 *   root - the root of the sub-tree that we will go through depending on the new
 *    nodes key value.
 *  newnode - the new node that will be inserted into the tree with the correct key 
 *    and value.
 */
struct bst_node* insert_recursive(struct bst_node* root, struct bst_node* newnode){
  //base case - we hit a leaf node
  if(root == NULL){
    //once going to the right place and getting to the last leaf node
    //return the newnode so the leaf node will now point to this new node
    return newnode;
  }

  //if the key were searching for is less than the root's key then recurse down left sub tree
  //otherwise recurse down the right sub tree
  if(newnode->key < root->key){
    root->left = insert_recursive(root->left, newnode);
  }else{
    root->right = insert_recursive(root->right, newnode);
  }

  //this will break out of the original recursive calls maintaining the prev tree structure.
  return root;
}

/*
 * This function should insert a new key/value pair into the BST.  The key
 * should be used to order the key/value pair with respect to the other data
 * stored in the BST.  The value should be stored along with the key, once the
 * right location in the tree is found.
 *
 * Params:
 *   bst - the BST into which a new key/value pair is to be inserted.  May not
 *     be NULL.
 *   key - an integer value that should be used to order the key/value pair
 *     being inserted with respect to the other data in the BST.
 *   value - the value being inserted into the BST.  This should be stored in
 *     the BST alongside the key.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void bst_insert(struct bst* bst, int key, void* value) {
  //create a new node on the heap
  struct bst_node* newnode = malloc(sizeof(struct bst_node));
  
  //initialize values
  newnode->key = key;
  newnode->value = value;
  newnode->right = NULL;
  newnode->left = NULL;

  //insert the node into the tree using the recursive helper function
  bst->root = insert_recursive(bst->root, newnode);

  return;
}

/*
 * This function returns the next inorder node in the tree 
 * of the given node. 
 *
 * Params:
 *   root - the node that you want to find the next inorder node
 *    for.
 */
struct bst_node* next_inorder(struct bst_node* root){
  //start at the given node called root
  struct bst_node* curr = root;

  //keep on iterating through the left subtree of given node
  while(curr != NULL && curr->left != NULL){
    curr = curr->left;
  }

  //return the last node or the leaf in the left mose subtree of the given
  //node. This will be the next in-order node
  return curr;
}

/*
 * This function recursively iterates through the tree using the key value
 * of every sub-tree and the fact that BTS is sorted to find the key for the 
 * node that should be deleted. Once it finds the node there are three possibilities:
 * 
 * 1. The node we want to delete is a leaf node and has no children in which we just 
 *  remove and free it.
 * 2. The node we want to delete has one child node in which we just swap the parent 
 *  and child node and then remove the child node.
 * 3. The node we want to delete has two children in which we have to find the next
 *  node in-order to the node we want to delete and then swap that node and the parent
 *  node.
 *
 * Params:
 *   root - the root of the sub-tree that we will go through depending on the new
 *    nodes key value. and the node we want to delete once we find it.
 *   key - This is the key of the node that we want to delete so we can find and 
 *    identify it.
 */
struct bst_node* remove_recurse(struct bst_node* root, int key){
  if(root == NULL) return root; //if we don't finf the node then return
  //go to left subtree if key is less than
  if(key < root->key) root->left = remove_recurse(root->left, key);
  //go to right subtree if key is greater than
  else if(key > root->key) root->right = remove_recurse(root->right, key);
  else{ //we have found the node with the correct key that we were looking for
    //these handle if the node has one to no children nodes
    //if there is no left node then take the right node and switch that with the node 
    //we want to remove. If there are no children it will just swap the node with NULL
    //and remove it.
    if(root->left == NULL){
      struct bst_node* temp = root->right;
      free(root);
      return temp;
    }else if(root->right == NULL){ //same thing but if the one child is on the left side
      struct bst_node* temp = root->left;
      free(root);
      return temp;
    }

    //otherwise if the node we want to delete has two children we have to swap it with 
    //the next inorder node.
    struct bst_node* temp = next_inorder(root->right); //get the next inorder node
    root->key = temp->key; //swap the values with the next inorder node
    root->value = temp->value; //swap the key with the next inorder node
    
    //now that we have swapped the values we now need to remove the next inorder node we 
    //swapped values with so we will recursively call the same function to do so giving it
    //the next inorder node's key.
    root->right = remove_recurse(root->right, temp->key);
  }

  //after all is done return the roots from all the other recursive calls to find the node
  //in order to maintain the previous tree structure that was unmodified.
  return root;
}

/*
 * This function should remove a key/value pair with a specified key from a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should remove the first one it encounters (i.e. the one closest to
 * the root of the tree).
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair to be removed from the BST.
 */
void bst_remove(struct bst* bst, int key) {
  remove_recurse(bst->root, key);

  return;
}

/*
 * This function recursively iterates through the tree until it finds the node 
 * with the key that is given and then returns it. If the node isn't in the tree 
 * it returns NULL.
 *
 * Params:
 *  root - the root of the sub-tree that we will go through depending on the
 *    key value.
 *  key - the key of the node that we want to get so we can identify and find the
 *    node.
 */
struct bst_node* bst_get_recursive(struct bst_node* root, int key){
  if(root == NULL){ //base case - we passed a leaf node
    //if we got to a leaf node and haven't found the node yet then it 
    //doesn't exist in this tree so we return NULL.
    return NULL;
  }else if(root->key == key){ //if the key of current node were checking is equal then we found the node
    return root; //return that node.
  }else if(key < root->key){ //if the key of the current node is less than then recurse left subtree
    return bst_get_recursive(root->left, key);
  }else{ //if the key of the current node is greater than then recurse the right subtree
    return bst_get_recursive(root->right, key);
  }
}

/*
 * This function should return the value associated with a specified key in a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should return the first one it encounters (i.e. the one closest to
 * the root of the tree).  If the BST does not contain the specified key, this
 * function should return NULL.
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair whose value is to be returned.
 *
 * Return:
 *   Should return the value associated with the key `key` in `bst` or NULL,
 *   if the key `key` was not found in `bst`.
 */
void* bst_get(struct bst* bst, int key) {
  //call recursive function call 
  return bst_get_recursive(bst->root, key);
}

/*****************************************************************************
 **
 ** BST puzzle functions
 **
 *****************************************************************************/

/*
 * This function recursively iterates through the tree and returns the path 
 * with the longest depth and then returns that. It will iterate through each 
 * left and right subtree determine which subtree has a longer depth and then return
 * the longest one plus one for the root node.
 *
 * Params:
 *   root - the root of the sub-tree that we will go through starting at the main tree's
 *    root node.
 */
int bst_height_recurse(struct bst_node* root){
  if(root == NULL){ //base case - we hit a leaf node
    return 0; //since we are past a leaf node we return 0 so as this doesn't count as depth
  }else{
    //get the depth of the left subtree
    int l = bst_height_recurse(root->left);

    //get the depth of the right subtree
    int r = bst_height_recurse(root->right);

    //if the left subtree is deeper then return that depth plus one for the root
    if(l > r){
      return (l + 1);
    }else{ //otherwise return the depth of the right subtree plus one for the root
      return (r + 1);
    }
  }

}

/*
 * This function should return the height of a given BST, which is the maximum
 * depth of any node in the tree (i.e. the number of edges in the path from
 * the root to that node).  Note that the height of an empty tree is -1 by
 * convention.
 *
 * Params:
 *   bst - the BST whose height is to be computed
 *
 * Return:
 *   Should return the height of bst.
 */
 int bst_height(struct bst* bst) {
  //call recursive helper function
  return bst_height_recurse(bst->root) - 1;
 }

/*
 * This function recursively iterates through the tree and returns whether there
 * is a path from the root to a leaf node where the keys of the node sum up to the
 * given sum. It will iterate through each subtree and for each tree it will calculate 
 * the sum minus the node's key value and store that and put that as the new sum into the
 * next function call. If the difference equals zero and it's a leaf node then there is a 
 * path so it returns 1 and that will be returned to the first recursive call.
 *
 * Params:
 *  root - the root of the sub-tree that we will go through starting at the whole tree's
 *    start node.
 *  sum - the sum which we will check can be added up to through any paths in the tree.
 */
int get_path_sum(struct bst_node* root, int sum){
  if(root == NULL){ //base case - went past leaf node
    return 0; //if we hit a leaf node return 0 as we have traversed the tree
  }

  //store answer which we will return
  int answer = 0;

  //calculate the difference of the passed sum and key of the current node we are on
  int difference = sum - root->key;

  //if the difference is 0 meaning we found a path that adds to the sum and its a leaf node
  if((difference == 0) && (root->left == NULL) && (root->right == NULL)){
    return 1; //return true since there is a path
  }

  //if there is a left subtree then recurse down that subtree
  if(root->left != NULL){
    //recursively call on left subtree and OR the previous answer with the new answer so 
    //if a 1, or multiple 1s, ever comes up in a recursive call it will maintain all the 
    //way through the original call and it will return the correct answer.
    answer = (answer || get_path_sum(root->left, difference));
  }

  //if there is right subtree then recurse down that subtree
  if(root->right != NULL){
    //recursively call on right subtree and OR the previous answer with the new answer so 
    //if a 1, or multiple 1s, ever comes up in a recursive call it will maintain all the 
    //way through the original call and it will return the correct answer.
    answer = (answer || get_path_sum(root->right, difference));
  }

  //return the answer 
  return answer;
}

/*
 * This function should determine whether a specified value is a valid path
 * sum within a given BST.  In other words, this function should check whether
 * the given BST contains any path from the root to a leaf in which the keys
 * sum to the specified value.
 *
 * Params:
 *   bst - the BST whose paths sums to search
 *   sum - the value to search for among the path sums of `bst`
 *
 * Return:
 *   Should return 1 if `bst` contains any path from the root to a leaf in
 *   which the keys add up to `sum`.  Should return 0 otherwise.
 */
int bst_path_sum(struct bst* bst, int sum) {
  //call recursive helper function
  return get_path_sum(bst->root, sum);
}

/*
 * This function recursively iterates through a tree and returns the sum of all the 
 * nodes key values within a given upper and lower bound. It creates a sum and then 
 * adds to the sum the values of their key if the given root is in the range. Additionally,
 * it will only recurse down the certain sub-trees if it's within the range which makes it
 * so there is less checking, this is called tree-pruning. it then returns the added sum from 
 * all the recursive calls.
 *
 * Params:
 *   root - the root of the sub-tree that we will go through starting at the whole tree's 
 *    start node.
 *   lower - this is the lower bound for the range for which we want to check. Must be lower
 *    than the upper parameter.
 *   upper - this is the upper bound for the range for which we want to check. Must be higher
 *    than the lower parameter.
 */
int get_range_sum(struct bst_node* root, int lower, int upper){
  if(root == NULL){ //base case - passed leaf node
    return 0; //if hit leaf node then we have iterated through the tree so return
  }

  //create a sum variable to keep track of the sum within the range
  int sum = 0;

  //if the key of the current node is above the lower bound then recurse down left subtree
  //and add that to the initial sum.
  if(root->key > lower){
    sum += get_range_sum(root->left, lower, upper);
  }
  
  //if the key of the current node is below the upper bound then recurse down right subtree
  //and add that to the initial sum.
  if(root->key < upper){
    sum += get_range_sum(root->right, lower, upper);
  }

  //if the current node we are at is within the range then add the key of that node 
  //to the sum
  if((lower <= root->key) && (upper >= root->key)){
    sum += root->key;
  }

  //return the final sum of all the nodes within the range
  return sum;
}

/*
 * This function should compute a range sum in a given BST.  Specifically, it
 * should compute the sum of all keys in the BST between a given lower bound
 * and a given upper bound.  For full credit, you should not process any subtree
 * whose keys cannot be included in the range sum.
 *
 * Params:
 *   bst - the BST within which to compute a range sum
 *   lower - the lower bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *   upper - the upper bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *
 * Return:
 *   Should return the sum of all keys in `bst` between `lower` and `upper`.
 */
int bst_range_sum(struct bst* bst, int lower, int upper) {
  //call recursive helper function
  return get_range_sum(bst->root, lower, upper);
}

/*****************************************************************************
 **
 ** BST iterator definition (extra credit only)
 **
 *****************************************************************************/

/*
 * Structure used to represent a binary search tree iterator.  It contains
 * only a reference to a stack to be used to implement the iterator.
 *
 * You should not modify this structure.
 */
struct bst_iterator {
  struct stack* stack;
};

/*
 * This function should allocate and initialize an iterator over a specified
 * BST and return a pointer to that iterator.
 *
 * Params:
 *   bst - the BST for over which to create an iterator.  May not be NULL.
 */
struct bst_iterator* bst_iterator_create(struct bst* bst) {
  //create iterator on heap and then initialize it's stack
  struct bst_iterator* iterator = malloc(sizeof(struct bst_iterator));
  iterator->stack = stack_create();

  //set the current node to the root of the given tree
  struct bst_node* curr = bst->root;

  //push all the nodes in the left subtree into the stack which will put the 
  //first inorder node at the top of the stack.
  while(curr != NULL){
    stack_push(iterator->stack, (void*)curr);
    curr = curr->left;
  }

  //return the iterator pointer
  return iterator;
}

/*
 * This function should free all memory allocated to a given BST iterator.
 * It should NOT free any memory associated with the BST itself.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   iter - the BST iterator to be destroyed.  May not be NULL.
 */
void bst_iterator_free(struct bst_iterator* iter) {
  //free all the stack data
  stack_free(iter->stack);

  //free the iterator
  free(iter);
  iter = NULL;
}

/*
 * This function should indicate whether a given BST iterator has more nodes
 * to visit.  It should specifically return 1 (true) if the iterator has at
 * least one more node to visit or 0 (false) if it does not have any more
 * nodes to visit.
 *
 * Param:
 *   iter - the BST iterator to be checked for remaining nodes to visit.  May
 *     not be NULL.
 */
int bst_iterator_has_next(struct bst_iterator* iter) {
  //returns true if it the stack isn't empty and false if otherwise
  return !stack_isempty(iter->stack);
}

/*
 * This function should return both the value and key associated with the
 * current node pointed to by the specified BST iterator and advnce the
 * iterator to point to the next node in the BST (in in-order order).
 *
 * Because a function can't return two things, the key associated with the
 * current node should be returned the normal way, while its value should be
 * returned via the argument `value`.  Specifically, the argument `value`
 * is a pointer to a void pointer.  The current BST node's value (a void
 * pointer) should be stored at the address represented by `value` (i.e. by
 * dereferencing `value`).  This will look something like this:
 *
 *   *value = current_node->value;
 *
 * Parameters:
 *   iter - BST iterator.  The key and value associated with this iterator's
 *     current node should be returned, and the iterator should be updated to
 *     point to the next node in the BST (in in-order order).  May not be NULL.
 *   value - pointer at which the current BST node's value should be stored
 *     before this function returns.
 *
 * Return:
 *   This function should return the key associated with the current BST node
 *   pointed to by `iter`.
 */
int bst_iterator_next(struct bst_iterator* iter, void** value) {
  //set curr to NULL
  struct bst_node* curr = NULL;

  //if there isn't another node in the stack then you can't go next so return 0
  if(bst_iterator_has_next(iter) == 0){
    return 0;
  }else{
    //pop from the stack and store it into curr
    curr = stack_pop(iter->stack);

    //if the node we popped has a right subtree then push that to the stack 
    //and then any nodes from the left subtree that the right node might have.
    //this will put the tree nodes into the stack in inorder fashion.
    if(curr->right != NULL){
      struct bst_node* temp = curr->right;

      while(temp != NULL){
        stack_push(iter->stack, temp);
        temp = temp->left;
      }
    }
  }

  //dereference value and store curr's value in it
  *value = curr->value;

  //return curr's key value
  return curr->key;
}