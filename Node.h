#ifndef NODE_H
#define NODE_H

class Node {
private:
    bool leafNode;
    unsigned int attribute;
    unsigned int count;
    std::vector<unsigned int> childrenLabels;
    std::vector<Node *> children;

public:
    Node();
    ~Node();

    void setAttribute(unsigned int attribute);
    unsigned int getAttribute();
    void insertChild(Node* node, unsigned int label);
    std::vector<Node *> getChildren();
    std::vector<unsigned int> getChildrenLabels();
    void setCount(unsigned int count);
    unsigned int getCount();
    void setLeaf();
    bool isLeaf();
};

Node::Node(){
    this->leafNode = false;
}

Node::~Node(){
    for(unsigned int i=0; i<children.size(); i++)
        delete children[i];
}

void Node::setAttribute(unsigned int attribute){
    this->attribute = attribute;
}

unsigned int Node::getAttribute(){
    return this->attribute;
}

void Node::insertChild(Node* node, unsigned int label){
    this->children.push_back(node);
    this->childrenLabels.push_back(label);
}

std::vector<Node *> Node::getChildren(){
    return this->children;
}

std::vector<unsigned int> Node::getChildrenLabels(){
    return this->childrenLabels;
}

void Node::setCount(unsigned int count){
    this->count = count;
}

unsigned int Node::getCount(){
    return this->count;
}

void Node::setLeaf(){
    this->leafNode = true;
}

bool Node::isLeaf(){
    return this->leafNode;
}

#endif