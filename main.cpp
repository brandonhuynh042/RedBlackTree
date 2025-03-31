#include <iostream>
#include "Node.h"
#include <cstring>
#include <fstream>
using namespace std;

Node* add(Node* root, int addInput, Node* parent, Node* root2);
void printTree(Node* root, int depth);
void rotateLeft(Node* &root, Node* parent, Node* u, Node* v);
void rotateRight(Node* &root, Node* parent, Node* u, Node* v);
int main() {
  bool quit = false;
  char input[30];
  Node* root = NULL;
  do {
     cout << "You can: \'ADD\' \'PRINT\' \'QUIT\'" << endl;
    cin >> input;
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
	root = add(root, addInput, NULL, root);
	root->setColor(1);
      }
      else if (strcmp(choiceInput, "FILE") == 0) {
	// read in file, add one number at a time
	ifstream sequence("numbers.txt");
	int number;
	while (sequence >> number) {
	  root = add(root, number, NULL, root);
	}
	root->setColor(1);
      }
    }
    else if (strcmp(input, "PRINT") == 0) {
      printTree(root, 0);
    }
  } while (quit != true);
  return 0;
}


Node* add(Node* root, int addInput, Node* parent, Node* root2) {
  if (!root) {
    Node* newNode = new Node(addInput);
    newNode->setColor(0);
    newNode->setParent(parent);
    return newNode;
  }
  // recurse left if it's smaller
  else if (root->getValue() > addInput) {
    root->setLeft(add(root->getLeft(), addInput, root, root2));
    // if two consecutive nodes are red
    if (root->getLeft()->getColor() == 0 && root->getColor() == 0) {
      // is the uncle red?
      if (root->getRight() && root->getRight()->getColor() == 0) {
	// if so, grandparent becomes red, parent and uncle become black.
	root->setColor(0);
	root->getLeft()->setColor(1);
	root->getRight()->setColor(1);
      }
      else {
	if (root->getLeft()->getLeft() && root->getLeft()->getLeft()->getValue() == addInput) {
	  // away
	}
	else {
	  // toward
	}
      }
    }
  }
  // recurse right if it's bigger
  else {
    root->setRight(add(root->getRight(), addInput, root, root2));
    // two consecutive are red
    if (root->getRight()->getColor() == 0 && root->getColor() == 0) {
      // is the uncle red?
      if (root->getLeft() && root->getLeft()->getColor() == 0) {
	root->setColor(0);
	root->getLeft()->setColor(1);
	root->getRight()->setColor(1);
      }
    }
    else {
      //uncle is black, rotation needed...
    }
  }
  return root;
}

void printTree(Node* root, int depth) {
  if (root->getRight() != NULL) {
    printTree(root->getRight(), depth+1);
  }
  for (int i = 0; i < depth; i++) {
    cout << '\t';
  }
  cout << root->getValue() << " " << root->getColor() << endl;
  if (root->getLeft() != NULL) {
    printTree(root->getLeft(), depth+1);
  }
}

void rotateLeft(Node* &root, Node* parent, Node* u, Node* v) {
  v = u->getRight();
  u->setRight(v->getLeft());
  if (parent == root) {
    root = v;
  }
  else if (parent->getLeft() == u) {
    parent->setLeft(v);
  }
  else {
    parent->setRight(v);
  }
  v->setLeft(u);
}

void rotateRight(Node* &root, Node* parent, Node* u, Node* v) {
  v = u->getLeft();
  u->setLeft(v->getRight());
  if (parent == root) {
    root = v;
  }
  else if (parent->getLeft() == u) {
    parent->setLeft(v);
  }
  else {
    parent->setRight(v);
  }
  v->setRight(u);
}
