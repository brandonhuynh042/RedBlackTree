#include <iostream>
#include "Node.h"
#include <cstring>
#include <fstream>
using namespace std;
/* This program implements insertion for a red black tree.
 * Brandon Huynh, last edited 4/11/2025 */

void add(Node* &actualRoot, int addInput);
Node* insert(Node* root, int value, Node* parent, Node* &added);
void fixTree(Node* &actualRoot, Node* added);
void printTree(Node* root, int depth);
void rotateLeft(Node* &actualRoot, Node* &u);
void rotateRight(Node* &actualRoot, Node* &u);

int main() {
  bool quit = false;
  char input[30];
  Node* root = NULL;
  do {
    // what do they want to do?
     cout << "You can: \'ADD\' \'PRINT\' \'QUIT\'" << endl;
    cin >> input;
    // quit...
    if (strcmp(input, "QUIT") == 0) {
      quit = true;
    }
    else if (strcmp(input, "ADD") == 0) {
       // which way do they want to add?
      cout << "You can add: \'MANUAL\' \'FILE\'" << endl;
      char choiceInput[8];
      cin >> choiceInput;
      if (strcmp(choiceInput, "MANUAL") == 0) {
	// take input, add.
	cout << "What is the number you'd like to add?" << endl;
	int addInput;
	cin >> addInput;
	add(root, addInput);
      }
      else if (strcmp(choiceInput, "FILE") == 0) {
	// read in file, add one number at a time
	ifstream sequence("numbers.txt");
	int number;
	while (sequence >> number) {
	  add(root, number);
	}
      }
    }
    else if (strcmp(input, "PRINT") == 0) {
      printTree(root, 0);
    }
  } while (quit != true);
  return 0;
}

void add(Node* &actualRoot, int addInput) {
  Node* added = NULL;
  // insertion like a binary search tree
  actualRoot = insert(actualRoot, addInput, NULL, added);
  // fixing it to red black tree rules
  fixTree(actualRoot, added);
}
Node* insert(Node* root, int addInput, Node* parent, Node* &added) {
  // reached a leaf...
  if (root == NULL) {
    Node* newNode = new Node(addInput);
    newNode->setColor(0);
    newNode->setParent(parent);
    added = newNode;
    return newNode;
  }
  // recurse left if it's smaller
  if (addInput < root->getValue()) {
    Node* leftAdd = insert(root->getLeft(), addInput, root, added);
    root->setLeft(leftAdd);
  }
  // recurse right if it's bigger
  else {
    Node* rightAdd = insert(root->getRight(), addInput, root, added);
    root->setRight(rightAdd);
  }
  return root;
}

void fixTree(Node* &actualRoot, Node* added) {
  // going bottom up iteratively
  while (added != actualRoot && added->getParent() && added->getParent()->getColor() == 0) {
    Node* parent = added->getParent();
    Node* grandparent = parent->getParent();
    // hit the top of the tree?
    if (grandparent == NULL) {
      break;
    }
    // parent is on left...
    if (parent == grandparent->getLeft()) {
      Node* uncle = grandparent->getRight();
      // if the uncle is red...
      if (uncle != NULL && uncle->getColor() == 0) {
	// swap grandparent & parent colors, move up a layer
        parent->setColor(1);
        uncle->setColor(1);
        grandparent->setColor(0);
        added = grandparent;
      }
      // uncle is black...
      else {
	// toward case
        if (added == parent->getRight()) {
          rotateLeft(actualRoot, parent);
          added = parent;
          parent = added->getParent();
        }
        // away case; rotate right, recolor parent/gp
        rotateRight(actualRoot, grandparent);
        parent->setColor(1);
        grandparent->setColor(0);
        added = parent;
	return; // no need to escalate
      }
    }
    // parent is on right.,.
    else {
      Node* uncle = grandparent->getLeft();
      // uncle is red...
      if (uncle != NULL && uncle->getColor() == 0) {
	// swap colors with grandparent, move up a layer
        parent->setColor(1);
        uncle->setColor(1);
        grandparent->setColor(0);
        added = grandparent;
      }
      // uncle is black...
      else {
        if (added == parent->getLeft()) {
          // toward case
          rotateRight(actualRoot, parent);
          added = parent;
          parent = added->getParent();
        }
        // away case: rotate left, recolor parent/gp
        rotateLeft(actualRoot, grandparent);
        parent->setColor(1);
        grandparent->setColor(0);
        added = parent;
	return; // no need to escalate
      }
    }
  }
  actualRoot->setColor(1); // root is always black
}

void printTree(Node* root, int depth) {
  if (root == NULL) {
    return;
  }
  // recursing right
  if (root->getRight() != NULL) {
    printTree(root->getRight(), depth+1);
  }
  for (int i = 0; i < depth; i++) {
    cout << '\t';
  }
  cout << root->getValue() << " " << root->getColor() << endl;
  // recursing left
  if (root->getLeft() != NULL) {
    printTree(root->getLeft(), depth+1);
  }
}

void rotateLeft(Node* &actualRoot, Node* &u) {
  Node* v = u->getRight();
  //cout << v->getValue() << endl;
  u->setRight(v->getLeft());
  if (v->getLeft() != NULL) {
    v->getLeft()->setParent(u);
  }
  v->setParent(u->getParent());
  // if the rotation involves the root...
  if (u->getParent() == NULL) {
    //cout << "new root" << endl;
    actualRoot = v;
  }
  // updating parent child relationships
  else if (u == u->getParent()->getLeft()) {
    u->getParent()->setLeft(v);
  }
  else {
    u->getParent()->setRight(v);
  }
  v->setLeft(u);
  u->setParent(v);
}

void rotateRight(Node* &actualRoot, Node* &u) {
  Node* v = u->getLeft();
  u->setLeft(v->getRight());
  if (v->getRight() != NULL) {
    v->getRight()->setParent(u);
  }
  v->setParent(u->getParent());
  // if the rotation involves the root...
  if (u->getParent() == NULL) {
    cout << "new root" << endl;
    actualRoot = v;
  }
  // updating parent child relationships
  else if (u == u->getParent()->getLeft()) {
    u->getParent()->setLeft(v);
  }
  else {
    u->getParent()->setRight(v);
  }
  v->setRight(u);
  u->setParent(v);
  //printTree(v, 0);
}
