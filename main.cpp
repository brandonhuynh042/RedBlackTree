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
void fixTreeDel(Node* &actualRoot, Node* deleted, Node* parent);
void printTree(Node* root, int depth);
void rotateLeft(Node* &actualRoot, Node* &u);
void rotateRight(Node* &actualRoot, Node* &u);
void searchTree(Node* root, Node* &target, int searchInput, bool &inTree);
void remove(Node* parent, int delInput, Node* &root, Node* &replacement, Node* &reParent);

int main() {
  bool quit = false;
  char input[30];
  Node* root = NULL;
  do {
    // what do they want to do?
    cout << "You can: \'ADD\' \'SEARCH\' \'DELETE\' \'PRINT\' \'QUIT\'" << endl;
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
    else if (strcmp(input, "SEARCH") == 0) {
      cout << "What number are you looking for?" << endl;
      int searchInput;
      cin >> searchInput;
      bool inTree;
      Node* holder;
      searchTree(root, holder, searchInput, inTree);
      if (inTree) {
	cout << searchInput << " is in the tree." << endl;
      }
      else {
	cout << searchInput << " is not in the tree." << endl;
      }
    }
    else if (strcmp(input, "DELETE") == 0) {
      cout << "What number do you want to delete?" << endl;
      int deleteInput;
      cin >> deleteInput;
      Node* toDelete;
      bool inTree;
      Node* replacement;
      Node* reParent;
      searchTree(root, toDelete, deleteInput, inTree);
      if (inTree == false) {
	cout << "Number not found..." << endl;
      }
      else {
	cout << toDelete->getValue() << " "  << toDelete->getColor() << endl;
	if (toDelete->getColor() == 1) {
	remove(root, deleteInput, root, replacement, reParent);
	cout << "fixing!" << endl;
	fixTreeDel(root, replacement, reParent);
	}
	else {
	  remove(root, deleteInput, root, replacement, reParent);
	}
      }
    }
    else if (strcmp(input, "PRINT") == 0) {
      printTree(root, 0);
    }
  } while (quit != true);
  return 0;
}

void searchTree(Node* root, Node* &target, int searchInput, bool &inTree) {
  // not in the tree.
  if (root == NULL) {
    target = NULL;
    inTree = false;
    return;
  }
  // found it.
  if (root->getValue() == searchInput) {
    target = root;
    inTree = true;
    return;
  }
  // determine if it's bigger or smaller, move down tree accordingly
  if (searchInput < root->getValue()) {
    searchTree(root->getLeft(), target, searchInput, inTree);
  }
  else {
    searchTree(root->getRight(), target, searchInput, inTree);
  }
  return;
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
  while (added != actualRoot && added->getParent()->getColor() == 0) {
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
	break; // no need to escalate
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
	break; // no need to escalate
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
  if (root->getParent()) {
    cout << root->getValue() << " " << root->getColor() << " p: " << root->getParent()->getValue() << endl;
  }
  else {
    cout << root->getValue() << " " << root->getColor() << endl;
  }
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


void remove(Node* parent, int delInput, Node* &root, Node* &replacement, Node* &reParent) {
  // empty tree
  if (root == NULL) {
    cout << "Empty tree!" << endl;
    return;
  }
  // the value to delete is the root
  if (parent->getValue() == delInput) {
    // no children, delete the root
    if (parent->getRight() == NULL && parent->getLeft() == NULL) {
      replacement = NULL;
      reParent = parent;
      delete root;
      root = NULL;
      return;
    }
    // only left child, right child becomes new root
    if (parent->getRight() == NULL) {
      Node* temp = root;
      replacement = parent->getLeft();
      reParent = parent;
      root = parent->getLeft();
      root->setParent(NULL);
      delete temp;
      return;
    }
    // only right child, left child becomes new root
    if (parent->getLeft() == NULL) {
      Node* temp = root;
      replacement = parent->getRight();
      reParent = parent;
      root = parent->getRight();
      root->setParent(NULL);
      delete temp;
      return;
    }
    // two children, find the successor
    else {
      Node* temp = parent->getRight();
      // right child doesn't have a left to search through so it just becomes the new root
      if (temp->getLeft() == NULL) {
	root->setValue(temp->getValue());
	replacement = root;
	reParent = parent;
	root->setRight(temp->getRight());
        if (root->getRight()) {
	  root->getRight()->setParent(root);
	}
        delete temp;
	return;
      }
      // otherwise, traverse until the leftmost child of the right child
    Node* pTemp = temp;
    temp = temp->getLeft();
    while (temp->getLeft() != NULL) {
      pTemp = temp;
      temp = temp->getLeft();
    }
    // this becomes the new root, delete the leaf
    root->setValue(temp->getValue());
    replacement = root;
    reParent = parent;
    if (temp->getRight() != NULL) {
      pTemp->setLeft(temp->getRight());
    }
    else {
      pTemp->setLeft(NULL);
    }
    delete temp;
    return;

    }
  }
  // checking left child
  if (parent->getLeft() != NULL) {
    // left child is value to delete
  if (parent->getLeft()->getValue() == delInput) {
    // no children, remove the link to parent and free memory
    if (parent->getLeft()->getLeft() == NULL && parent->getLeft()->getRight() == NULL) {
      Node* temp = parent->getLeft();
      replacement = NULL;
      reParent = parent;
      parent->setLeft(NULL);
      delete temp;
      return;
    }
    // only right child, that replaces it's parent
    if (parent->getLeft()->getLeft() == NULL) {
      Node* temp = parent->getLeft();
      replacement = temp->getRight();
      reParent = temp;
      parent->setLeft(temp->getRight());
      parent->getLeft()->setParent(parent);
      delete temp;
      return;
    }
    // only left child, that replaces it's parent
    else if (parent->getLeft()->getRight() == NULL) {
      Node* temp = parent->getLeft();
      replacement = temp->getLeft();
      reParent = temp;
      parent->setLeft(temp->getLeft());
      parent->getLeft()->setParent(parent);
      delete temp;
      return;
    }
    // two children, begin looking for successor
    Node* temp = parent->getLeft()->getRight();
    // if the right child doesn't have a left child, it's value goes to the parent and the child gets deleted.
    if (temp->getLeft() == NULL) {
      parent->getLeft()->setValue(temp->getValue());
      replacement = parent->getLeft();
      reParent = parent;
      if (temp->getRight() != NULL) {
        parent->getLeft()->setRight(temp->getRight());
	parent->getLeft()->getRight()->setParent(parent->getLeft());
      }
      else {
        parent->getLeft()->setRight(NULL);
      }
      delete temp;
      return;
    }
    // otherwise, traverse through until the leftmost child of the right child is found
    Node* pTemp = temp;
    temp = temp->getLeft();
    while (temp->getLeft() != NULL) {
      pTemp = temp;
      temp = temp->getLeft();
    }
    // replace the place to delete's value with the successor, delete the former successor
    parent->getLeft()->setValue(temp->getValue());
    replacement = parent->getLeft();
    reParent = parent;
    if (temp->getRight() != NULL) {
      pTemp->setLeft(temp->getRight());
      pTemp->getLeft()->setParent(pTemp);
    }
    else {
      pTemp->setLeft(NULL);
    }
    delete temp;
    return;
  }
  }
  // same as the chunk above, but searching through the right child instead.
  if (parent->getRight() != NULL) {
  if (parent->getRight()->getValue() == delInput) {
    if (parent->getRight()->getLeft() == NULL && parent->getRight()->getRight() == NULL) {
      Node* temp = parent->getRight();
      replacement = NULL;
      reParent = parent;
      parent->setRight(NULL);
      delete temp;
      return;
    }
    if (parent->getRight()->getLeft() == NULL) {
      Node* temp = parent->getRight();
      replacement = temp->getRight();
      reParent = temp;
      parent->setRight(temp->getRight());
      parent->getRight()->setParent(parent);
      delete temp;
      return;
    }
    else if (parent->getRight()->getRight() == NULL) {
      Node* temp = parent->getRight();
      replacement = temp->getLeft();
      reParent = temp;
      parent->setRight(temp->getLeft());
      parent->getRight()->setParent(parent);
      delete temp;
      return;
    }
    Node* temp = parent->getRight()->getRight();
    if (temp->getLeft() == NULL) {
      parent->getRight()->setValue(temp->getValue());
      replacement = parent->getRight();
      reParent = parent;
      if (temp->getRight() != NULL) {
	parent->getRight()->setRight(temp->getRight());
	parent->getRight()->getRight()->setParent(parent->getRight());
      }
      else {
	parent->getRight()->setRight(NULL);
      }
      delete temp;
      return;
    }
    Node* pTemp = temp;
    temp = temp->getLeft();
    while (temp->getLeft() != NULL) {
      pTemp = temp;
      temp = temp->getLeft();
    }
    parent->getRight()->setValue(temp->getValue());
    replacement = parent->getRight();
    reParent = parent;
    if (temp->getRight() != NULL) {
      pTemp->setLeft(temp->getRight());
      pTemp->getLeft()->setParent(pTemp);
    }
    else {
      pTemp->setLeft(NULL);
    }

    delete temp;
    return;
  }
  }
  if (parent->getLeft() == NULL && parent->getRight() == NULL) {
    cout << "Value not found!" << endl;
    return;
  }
  // recurse left if it's smaller
  if (parent->getValue() > delInput) {
    remove(parent->getLeft(), delInput, root, replacement, reParent);
  }
  // recurse right if it's bigger
  else {
    remove(parent->getRight(), delInput, root, replacement, reParent);
  }
}

void fixTreeDel(Node* &actualRoot, Node* deleted, Node* parent) {
  while (deleted != actualRoot && (deleted == NULL || deleted->getColor() == 1)) {
    // replacement is a left child
    if (deleted == parent->getLeft()) {
      Node* sibling = parent->getRight();

      // red sibling
      if (sibling != NULL && sibling->getColor() == 0) {
        sibling->setColor(1);
        parent->setColor(0);
        rotateLeft(actualRoot, parent);
        sibling = parent->getRight();
      }

      // black sibling, black/null children
      if ((sibling == NULL) ||
          ((sibling->getLeft() == NULL || sibling->getLeft()->getColor() == 1) &&
           (sibling->getRight() == NULL || sibling->getRight()->getColor() == 1))) {
        if (sibling != NULL) {
          sibling->setColor(0);
        }
        deleted = parent;
        parent = deleted->getParent(); // move up the tree
      }
      else {
        // sibling's right is black/null, left red
        if (sibling->getRight() == NULL || sibling->getRight()->getColor() == 1) {
          if (sibling->getLeft() != NULL) {
            sibling->getLeft()->setColor(1);
          }
          sibling->setColor(0);
          rotateRight(actualRoot, sibling);
          sibling = parent->getRight();
        }

        // sibling's right is red
        sibling->setColor(parent->getColor());
        parent->setColor(1);
        if (sibling->getRight() != NULL) {
          sibling->getRight()->setColor(1);
        }
        rotateLeft(actualRoot, parent);
        deleted = actualRoot;
      }
    }
    // replacement is a right child
    else {
      Node* sibling = parent->getLeft();

      // red sibling
      if (sibling != NULL && sibling->getColor() == 0) {
        sibling->setColor(1);
        parent->setColor(0);
        rotateRight(actualRoot, parent);
        sibling = parent->getLeft();
      }

      // black sibling, black/null children
      if ((sibling == NULL) ||
          ((sibling->getLeft() == NULL || sibling->getLeft()->getColor() == 1) &&
           (sibling->getRight() == NULL || sibling->getRight()->getColor() == 1))) {
        if (sibling != NULL) {
          sibling->setColor(0);
        }
        deleted = parent;
        parent = deleted->getParent(); // move up the tree
      }
      else {
        // sibling's left is black/null, right red
        if (sibling->getLeft() == NULL || sibling->getLeft()->getColor() == 1) {
          if (sibling->getRight() != NULL) {
            sibling->getRight()->setColor(1);
          }
          sibling->setColor(0);
          rotateLeft(actualRoot, sibling);
          sibling = parent->getLeft();
        }

        // sibling's left is red
        sibling->setColor(parent->getColor());
        parent->setColor(1);
        if (sibling->getLeft() != NULL) {
          sibling->getLeft()->setColor(1);
        }
        rotateRight(actualRoot, parent);
        deleted = actualRoot;
      }
    }
  }

  if (deleted != NULL) {
    deleted->setColor(1);
  }
}
