#include <iostream>
#include "Node.h"
using namespace std;

Node::Node(int newValue) {
  value = newValue;
  left = NULL;
  right = NULL;
  parent = NULL;
}

Node::~Node() {
  left = NULL;
  right = NULL;
  parent = NULL;
}

void Node::setValue(int newValue) {
  value = newValue;
}

int Node::getValue() {
  return value;
}

void Node::setLeft(Node* newLeft) {
  left = newLeft;
}

void Node::setRight(Node* newRight) {
  right = newRight;
}

Node* Node::getLeft() {
  return left;
}

Node* Node::getRight() {
  return right;
}

void Node::setColor(int newColor) {
  color = newColor;
}

int Node::getColor() {
  return color;
}

void Node::setParent(Node* newParent) {
  parent = newParent;
}

Node* Node::getParent() {
  return parent;
}
