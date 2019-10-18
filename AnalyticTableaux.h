#ifndef ANALYTICTABLEAUX_LIBRARY_H
#define ANALYTICTABLEAUX_LIBRARY_H

#include <string>
#include <vector>
#include <regex>
using namespace std;

class Node
{

  private:
    string expression;
    bool truthValue;

    Node *parent;
    Node *leftChild;
    Node *rightChild;
    Node *frontChild;

    bool isContradictory;
    bool isAppliable;

    vector<Node *> traverse(vector<Node *> *nodes);
    vector<Node *> getReachableLeafs();
    void insertNode(Node *parent, Node *leftChild, Node *rightChild);
    void insertNode(Node *parent, Node *frontChild);
    void printTree(int level);
    bool isLeaf();

  public:
    Node(string expression, bool truthValue);

    vector<Node *> insertSides(string lExpression, bool lTruthValue, string rExpression, bool rTruthValue);
    vector<Node *> insertFront(string expression, bool truthValue);
    vector<Node *> insertFront(string expression1, bool truthValue1, string expression2, bool truthValue2);

    vector<Node *> getAppliableNodes();
    void markApplied();

    bool isClosed();
    bool checkContradiction();
    void markContradiction();

    string getExpression();
    bool getTruthValue();

    void printTree();
};

#endif