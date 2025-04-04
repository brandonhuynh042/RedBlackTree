#include <iostream>
#include "Node.h"
#include <cstring>
#include <fstream>
using namespace std;

Node* add(Node* root, int addInput, Node* root2, Node* &actualRoot);
void printTree(Node* root, int depth);
void rotateLeft(Node* &root, Node* &u);
void rotateRight(Node* &root, Node* &u);
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
	  root->setColor(1);
	}
      }
    }
    else if (strcmp(input, "PRINT") == 0) {
      printTree(root, 0);
    }
  } while (quit != true);
  return 0;
}


Node* add(Node* root, int addInput, Node* root2, Node* &actualRoot) {
  if (!root) {
    Node* newNode = new Node(addInput);
    newNode->setColor(0);
    newNode->setParent(root2);
    return newNode;
  }
  // recurse left if it's smaller
  else if (root->getValue() > addInput) {
    root->setLeft(add(root->getLeft(), addInput, root, actualRoot));
    // if two consecutive nodes are red
    if (root->getLeft()->getColor() == 0 && root->getColor() == 0) {
      // is the uncle red?
      cout << "two reds" << endl;
      if (root->getRight() && root->getRight()->getColor() == 0) {
	cout << "red uncle" << endl;
	// if so, grandparent becomes red, parent and uncle become black.
	if (root->getParent()) {
	  root->getParent()->setColor(0);
	}
	root->setColor(1);
	root->getRight()->setColor(1);
      }
      else {
	cout << "black uncle" << endl;
	if (root->getLeft() && root->getLeft()->getValue() == addInput) {
	  cout << "away" << endl;
	  if (root2->getParent() == NULL) {
	    rotateRight(actualRoot, root2);
	  }
	  else {
	    cout << "no root parent" << endl;
	    rotateRight(actualRoot, root2);
	  }
	  root->setColor(0);
	  if (root->getParent() != NULL) {
	    root->getParent()->setColor(1);
	  }
	}
	else {
	  // toward
	}
      }
    }
  }
  // recurse right if it's bigger
  else {
    root->setRight(add(root->getRight(), addInput, root, actualRoot));
    // two consecutive are red
    if (root->getRight()->getColor() == 0 && root->getColor() == 0) {
      // is the uncle red?
      cout << "two reds" << endl;
      if (root->getLeft() && root->getLeft()->getColor() == 0) {
	cout << "red uncle" << endl;
	if (root->getParent() != NULL) {
	  root->getParent()->setColor(0);
	}
	root->setColor(1);
	root->getLeft()->setColor(1);
      }
    }
    else {
      cout << "black uncle" << endl;
      if (root->getRight()->getRight() && root->getRight()->getRight()->getValue() == addInput) {
	cout << "skibs 2" << endl;
	if (root2->getParent() == NULL) {
	  rotateLeft(actualRoot, root2);
	}
	else {
	  cout << "no root parent" << endl;
	  cout << root2->getValue() << " " << root->getValue() << endl;
	  rotateLeft(actualRoot, root2);
	}
	root->setColor(0);
	if (root->getParent() != NULL) {
	  root->getParent()->setColor(1);
	}
      }
      else {

      }
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

void rotateLeft(Node* &root, Node* &u) {
  Node* v = u->getRight();
  cout << v->getValue() << endl;
  u->setRight(v->getLeft());
  if (v->getLeft() != NULL) {
    v->getLeft()->setParent(u);
  }
  v->setParent(u->getParent());
  if (u->getParent() == NULL) {
    cout << "new root" << endl;
    root = v;
  }
  else if (u == u->getParent()->getLeft()) {
    u->getParent()->setLeft(v);
  }
  else {
    u->getParent()->setRight(v);
  }
  v->setLeft(u);
  u->setParent(v);
}

void rotateRight(Node* &root, Node* &u) {
  Node* v = u->getLeft();
  cout << v->getValue() << endl;
  u->setLeft(v->getRight());
  if (v->getRight() != NULL) {
    v->getRight()->setParent(u);
  }
  v->setParent(u->getParent());
  if (u->getParent() == NULL) {
    cout << "new root" << endl;
    root = v;
  }
  else if (u == u->getParent()->getLeft()) {
    u->getParent()->setLeft(v);
  }
  else {
    u->getParent()->setRight(v);
  }
  v->setRight(u);
  u->setParent(v);
  printTree(v, 0);
}
