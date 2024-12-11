#pragma once
#include <iostream>
using namespace std;
struct RBN {
	int key;
	RBN* parent = nullptr;
	RBN* left = nullptr;
	RBN* right = nullptr;
	bool red = true;
	RBN(int key):key(key) {};
};


class RBT {
	RBN* root;
	bool isLeftChild(RBN* node);
	bool isNodeBlack(RBN* node);
	bool hasChild(RBN* node);
	void insertKey(RBN* node, int key);
	void srotate(RBN* node);
	void insertFixup(RBN* node);
	RBN* search(int key);
	RBN* predecessor(RBN* node);
	RBN* successor(RBN* node);
	RBN* moveToLeaf(RBN* node);
	void deleteNode(RBN* node);
	void deleteFixup(RBN* node);
	void printTree(RBN* node);
public:
	void insert(int key);
	void delete_(int key);
	void print();
	RBN* getRoot();
};


void RBT::insert(int key) {
	if (root == nullptr) {
		root = new RBN(key);
		root->red = false;
		return;
	}
	insertKey(root, key);
}


void RBT::insertKey(RBN* node, int key) {
	RBN*& left = node->left;
	RBN*& right = node->right;
	RBN*& dir = key < node->key ? left : right;
	if (dir == nullptr) {
		dir = new RBN(key);
		dir->parent = node;
		insertFixup(dir);
	}
	else {
		insertKey(dir, key);
	};
};


bool RBT::isLeftChild(RBN* node) {
	return node->parent->left == node;
};


bool RBT::isNodeBlack(RBN* node) {
	return node == nullptr || !node->red;
};

bool RBT::hasChild(RBN* node) {
	return node->left != nullptr || node->right != nullptr;
};


void RBT::insertFixup(RBN* node) {
	RBN* parent = node->parent;
	if (parent == nullptr) return;
	RBN* grandParent = node->parent->parent;
	if (grandParent == nullptr) return;
	RBN* uncle = parent == grandParent->left ? grandParent->right : grandParent->left;
	if (parent->red && node->red) {
		bool isUncleBlack = uncle == nullptr || !uncle->red;
		if (isUncleBlack) {
			if (isLeftChild(node) && isLeftChild(parent) || !isLeftChild(node) && !isLeftChild(parent)) {
				cout << "single rotation+\n";
				srotate(parent);
				parent->red = !parent->red;
				grandParent->red = !grandParent->red;
				//insertFixup();
			}
			else {
				cout << "double rotation\n";
				srotate(node);
				srotate(node);
				node->red = !node->red;
				grandParent->red = !grandParent->red;
				//insertFixup();
			}
		}
		else {
			cout << "Recoloring\n";
			grandParent->red = !grandParent->red;
			node->parent->red = !node->red;
			uncle->red = !uncle->red;
			insertFixup(grandParent);
		}
	};
	root->red = false;
};


RBN* RBT::moveToLeaf(RBN* node) {
	RBN* nodeToDelete = node;
	RBN* leaf = node;
	while (hasChild(leaf)){
		leaf = leaf->right == nullptr ? predecessor(leaf) : successor(leaf);
		swap(leaf->key, nodeToDelete->key);
		nodeToDelete = leaf;
	}
	return nodeToDelete;
}

void RBT::deleteNode(RBN* node) {
	if (node == root) root = nullptr;
	if (node->parent) {
		if (isLeftChild(node)) {
			node->parent->left = nullptr;
		}
		else {
			node->parent->right = nullptr;
		}
	}
	delete node;
};



void RBT::delete_(int key) {
	RBN* node = search(key);
	if (node == nullptr) return;
	RBN* nodeToDelete = moveToLeaf(node);
	if (nodeToDelete->red || nodeToDelete == root) {
		deleteNode(nodeToDelete);
	}
	else {
		 //double black
		deleteFixup(nodeToDelete);
		deleteNode(nodeToDelete);
	}
	root->red = false;
}



void RBT::deleteFixup(RBN* node) {
	RBN* parent = node->parent;
	RBN* sibiling = isLeftChild(node) ? node->parent->right : node->parent->left;
	bool isSibilingRed = !isNodeBlack(sibiling);
	if (isSibilingRed) {
		// case 1
		cout << "delete case 1 " << endl;
		srotate(sibiling);
		swap(parent->red, sibiling->red);
		deleteFixup(node);
	}
	else {
		if (hasChild(sibiling)) {
			if (isNodeBlack(sibiling->left) && isNodeBlack(sibiling->right)) {
				//case 2
				cout << "delete case 2" << endl;
				sibiling->red = !sibiling->red;
				if (parent == root) return;
				if (!isNodeBlack(parent)) {
					parent->red = false;
				}
				else {
					// double black;
					deleteFixup(parent);
				}
			}
			else {
				// has a red child 
				RBN* child = isNodeBlack(sibiling->right) ? sibiling->left : sibiling->right;
				RBN* oldRoot = sibiling->parent;
				if ((isLeftChild(child) && isLeftChild(node)) || (!isLeftChild(child) && !isLeftChild(node)) ) {
					// case 3
					cout << "delete case 3" << endl;
					child->red = oldRoot->red;
					oldRoot->red = false;
					srotate(child);
					srotate(child);
				}
				else {
					// case 4
					cout << "delete case 4" << endl;
					sibiling->red = oldRoot->red;
					oldRoot->red = false;
					child->red = false;
					srotate(sibiling);
				}
			}
		}
	}
}

RBN* RBT::search(int key) {
	RBN* node = root;
	while (node != nullptr) {
		if (node->key == key)
			return node;
		if (key < node->key)
			node = node->left;
		else
			node = node->right;
	}
	return nullptr;
}


RBN* RBT::predecessor(RBN* node) {
	if (node->left == nullptr) return node;
	RBN* mostRight = node->left;
	while (mostRight->right != nullptr) {
		mostRight = mostRight->right;
	};
	return mostRight;
}

RBN* RBT::successor(RBN* node) {
	if (node->right == nullptr) return node;
	RBN* mostLeft = node->right;
	while (mostLeft->left != nullptr) {
		mostLeft = mostLeft->left;
	};
	return mostLeft;
}



void RBT::srotate(RBN* node) {
	RBN* parent = node->parent;
	if (isLeftChild(node)) {
		RBN* sibiling = node->right;
		parent->left = sibiling;
		if (sibiling != nullptr) {
			sibiling->parent = parent;
		}
		node->right = parent;
	
		if (parent->parent == nullptr){
			root = node;
			node->parent = nullptr;
		}
		else if (isLeftChild(parent)) {
			parent->parent->left = node;
			node->parent = parent->parent;
		}
		else {
			parent->parent->right = node;
			node->parent = parent->parent;
		}

		parent->parent = node;
	}else{
		RBN* sibiling = node->left;
		parent->right = sibiling;
		if (sibiling != nullptr) {
			sibiling->parent = parent;
		}
		node->left = parent;
	
		if (parent->parent == nullptr){
			root = node;
			node->parent = nullptr;
		}
		else if (isLeftChild(parent)) {
			parent->parent->left = node;
			node->parent = parent->parent;
		}
		else {
			parent->parent->right = node;
			node->parent = parent->parent;
		}

		parent->parent = node;
	}
};

void RBT::print() {
	printTree(root);
};

RBN* RBT::getRoot() {
	return root;
};


void RBT::printTree(RBN* node) {
	
};

