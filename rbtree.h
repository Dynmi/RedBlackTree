//
// Interface of Red-Black Tree
//
// author: Haris Wang, dynmiw@gmail.com
//
#ifndef RBTREE_H_
#define RBTREE_H_

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <queue>
#include <vector>
#include <iostream>

enum Color {
	BLACK,
	RED
};


template<class K, class T>
struct rbNode {
	K     key;
	T     val;
	Color	color;
	rbNode	*parent;
	rbNode	*left;
	rbNode	*right;

	rbNode():parent(nullptr),left(nullptr),right(nullptr),color(RED){}
	~rbNode()
   	{
        	if (left != nullptr)
        	    delete left;
        	if (right != nullptr)
        	    delete right;
    	}
};


template<class K, class T> 
class rbTree {
	public:
		rbTree():root(nullptr),size(0){};
		~rbTree();
		void	insert(K &key, T &val);
		bool	remove(const K &key);
		bool	search(const K &key, T &val) const;
        	void	clear();
		void	printTree() const;
		int     getSize() const;
	private:
		int   size;
		rbNode<K, T> *root;
		
		int 	cmp(const K &a, const K &b) const;
		void	leftRotate(rbNode<K, T> *node);	
		void	rightRotate(rbNode<K, T> *node);
		void	removeNode(rbNode<K, T> *node);
};


// Destructor of 'rbTree'
template<class K, class T>
rbTree<K,T>::~rbTree()
{
	if (root != nullptr)
		delete root;
}


// insert (key, val) pair to appropriate position, then
// update the tree to keep its properties if needed.
// this->size will increase by 1.
template<class K, class T>
void rbTree<K,T>::insert(K &key, T &val)
{
	auto *node = new rbNode<K,T>;
	node->key = key;
	node->val = val;
	
	// whether first node
	if (root == nullptr) {
		root = node;
		node->color = BLACK;
		return;
	}

	// navigate to the desired positon of new node
	rbNode<K,T> *curr = root;
	while (curr->left != nullptr | curr->right != nullptr)
	{
		if (cmp(key, curr->key) == -1)
			curr = curr->left;
		else
			curr = curr->right;
	}
	
	// link new node and its parent
	node->parent = curr;
	if (cmp(key, curr->key) == -1)
		curr->left = node;
	else
		curr->right = node;

	// update the tree recursively to keep its properties if needed
	while (curr->color == RED && curr->parent != nullptr)
	{
		bool isRight = (curr == curr->parent->right);
        	rbNode<K,T> *uncle;
        	if (isRight)
       	        	uncle = curr->parent->left;
       	 	else
            		uncle = curr->parent->right;

        	if (uncle == nullptr) {
            		curr->color = BLACK;
            		curr->parent->color = RED; 
            		if (uncle == curr->parent->right) {
                		rightRotate(curr->parent);
            		}else {
                		leftRotate(curr->parent);
            		}
            		break;
        	}else if (uncle->color == RED) {
			curr->color = BLACK;
			uncle->color = BLACK;
      			curr->parent->color = RED;
			curr = curr->parent;
		}else {
			curr->color = BLACK;
			curr->parent->color = RED;

			if (isRight) {
				if (node == curr->left) {
					rightRotate(curr);
					curr = node;
				}
				leftRotate(curr->parent);
			}else {
				if (node == curr->right) {
					leftRotate(curr);
					curr = node;
				}
				rightRotate(curr->parent);
			}
		}
    		root->color = BLACK;
	}

	this->size++;
}


// return 1 if success
// return 0 if not found
// remove the node with 'key' from tree, then
// update the tree to keep its properties if needed.
// this->size will decrease by 1
template<class K, class T>
bool rbTree<K,T>::remove(const K &key)
{  
	// find the node to be deleted
	auto curr = root;
	while (curr->left != nullptr | curr->right != nullptr)
	{
		if (curr->key == key)
			break;

		if (cmp(key, curr->key) >= 0)
			curr = curr->right;
		else
			curr = curr->left;
	}
	if (curr->key != key)
		return 0;
	
  	this->removeNode(curr);
  	(this->size)--;
    	return 1;
}


// remove given node from this tree. 'node' must
// be in this tree
template<class K, class T>
void rbTree<K,T>::removeNode(rbNode<K, T> *node)
{
	if (node->color == RED) {
		if (node->left != nullptr && node->right != nullptr) {
            		// child x 2
            		// find successor, then fill 'node' with successor
            		auto successor = node->right;
            		while (successor->left != nullptr)
                		successor = successor->left;
            		node->key = successor->key;
            		node->val = successor->val;
            		this->removeNode(successor);
        	}else if (node->left != nullptr) {
            		// only left child
            		// fill 'node' with left child
            		node->key = node->left->key;
            		node->val = node->left->val;
            		node->color = node->left->color;
           		this->removeNode(node->left);
        	}else if (node->right != nullptr) {
            		// only right child
            		// fill 'node' with right child
            		node->key = node->right->key;
            		node->val = node->right->val;
            		node->color = node->right->color;        
            		this->removeNode(node->right);
        	}else {
            		// no child
            		if (node->parent == nullptr) {
                		free(node);
                		root = nullptr;
				return;
            		}

                	if (node->parent->left == node)
                		node->parent->left = nullptr;
                	else
                    		node->parent->right = nullptr;
                	
			free(node);
		}
 	}else {
        	if (node->left != nullptr && node->right != nullptr) {
        		// child x 2
            		// find successor, then fill 'node' with successor
            		auto successor = node->right;
            		while (successor->left != nullptr)
                		successor = successor->left;
            		node->key = successor->key;
            		node->val = successor->val;
            		this->removeNode(successor);
        	}else if (node->left != nullptr) {
            		// only left child
            		// fill 'node' with left child
            		node->key = node->left->key;
            		node->val = node->left->val;
            		this->removeNode(node->left);
        	}else if (node->right != nullptr) {
            		// only right child
            		// fill 'node' with right child
            		node->key = node->right->key;
            		node->val = node->right->val;
            		this->removeNode(node->right);
        	}else {
            		// no child. As the black node deleted is a leaf, fixup
            		// is neccesary after delete.
            		
			if (node->parent == nullptr) {
                		// 'node' is root
				free(node);
                		root = nullptr;
            			return;
			}

                	if (node->parent->left == node) {
                    		node->parent->left = nullptr;    
                    		if (node->parent->right != nullptr
                        	&& node->parent->right->color == RED) {
                        		node->parent->right->color = BLACK;
                        		leftRotate(node->parent);
                    		}
                	}else {
                		node->parent->right = nullptr;
                    		if (node->parent->left != nullptr
                        	&& node->parent->left->color == RED) {
                        		node->parent->left->color = BLACK;
                        		rightRotate(node->parent);
                    		}
                	}

                	if (node->parent->left == nullptr
                    	&& node->parent->right == nullptr
                    	&& node->parent->parent != nullptr) {
                    		// you can guess, 'node->parent->parent' must be RED
                    		rightRotate(node->parent->parent);
                	}

                	free(node);
        	}    
	}
}


// return 1 if success
// return 0 if not found
// find the node with 'key' in this tree, then set
// 'val' to the val of this node
template<class K, class T>
bool rbTree<K,T>::search(const K &key, T &val) const
{
	auto curr = root;
	while (curr->left != nullptr | curr->right != nullptr)
	{
		if (curr->key == key) {
			val = curr->val;
			break;
		}

		if (cmp(key, curr->key) < 0)
			curr = curr->left;
        	else
			curr = curr->right;
	}
    
	if (curr->key != key)
    		return 0;
    	return 1;
}


// compare 'a' and 'b'
// return -1 if a < b
// return 0 if a = b
// return 1 if a > b
template<class K, class T>
int rbTree<K,T>::cmp(const K &a, const K &b) const
{
	if (typeid(a) == typeid(char*)) {
		// string type
		char *x = (char *)a;
		char *y = (char *)b;
		return strcmp(x, y);
  	}else {
		if (a < b) return -1;
		if (a == b) return 0;
		if (a > b) return 1;
	}
  	return 0;
}


// take 'node' as the center, make a left rotation
template<class K, class T>
void rbTree<K,T>::leftRotate(rbNode<K,T> *node)
{
	assert( node->right != nullptr);
	auto temp = node->right;

  	// update the two nodes
	node->right = temp->left;
  	if (temp->left != nullptr)
  		temp->left->parent = node;
	temp->left = node;
	temp->parent = node->parent;
	node->parent = temp;

  	// update the parent
  	if (root == node) {
  		root = temp;
		return;
 	}
	if (temp->parent->left == node)
    		temp->parent->left = temp;
       	else
           	temp->parent->right = temp;
}


// take 'node' as the center, make a right rotation
template<class K, class T>
void rbTree<K,T>::rightRotate(rbNode<K,T> *node)
{
	assert( node->left != nullptr);
	auto temp = node->left;

  	// update the two nodes
	node->left = temp->right;
	if (temp->right != nullptr)
    		temp->right->parent = node;
	temp->right = node;
	temp->parent = node->parent;
	node->parent = temp;

	// update the parent
	if (root == node) {
		root = temp;
		return;
	}
	if (temp->parent->left == node)
		temp->parent->left = temp;
	else
      		temp->parent->right = temp;
}


// From the root to the leaf, send 'node',
// 'node's left sub-tree,'node's right sub-tree to stdout
template<class K, class T>
void rbTree<K,T>::printTree() const
{
	std::cout << "----------------" << std::endl;
	std::queue<rbNode<K,T>*> q;
	q.push(root);
	while (!q.empty())
	{
		auto top = q.front();
		q.pop();
		if (top->color == RED)
			std::cout << "R" ;
		else
			std::cout << "B" ;
		std::cout << top->key;
		std::cout << " ";
		if (top->left != nullptr) {
			q.push(top->left);
			if (top->left->color == RED)
				std::cout << "R" ;
			else
				std::cout << "B" ;
			std::cout << top->left->key;
			std::cout << " ";
		}else {
			std::cout << "NULL" << " ";
		}
		if (top->right != nullptr) {
			q.push(top->right);
			if (top->right->color == RED)
				std::cout << "R" ;
			else
				std::cout << "B" ;
			std::cout << top->right->key;
			std::cout << " ";
		}else {
			std::cout << "NULL" << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "----------------" << std::endl;
}


// return the number of nodes in this tree
template<class K, class T>
int rbTree<K,T>::getSize() const
{
	return this->size;
}


// delete all nodes in this tree, recycle the memory
// space token by them.
template<class K, class T>
void rbTree<K,T>::clear()
{
	delete this->root;
	this->root = nullptr;
}


#endif  // RBTREE_H_
