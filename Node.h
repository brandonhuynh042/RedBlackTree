#ifndef NODE_H
#define NODE_H
#include <iostream>
using namespace std;

class Node {
 public:
  Node(int newValue);
  ~Node();
  void setLeft(Node* newLeft);
  void setRight(Node* newRight);
  void setValue(int newValue);
  void setColor(int newColor);
  Node* getLeft();
  Node* getRight();
  int getValue();
  int getColor();
  void setParent(Node* newParent);
  Node* getParent();
 protected:
  int value;
  int color;
  Node* left;
  Node* right;
  Node* parent;
};
#endif
