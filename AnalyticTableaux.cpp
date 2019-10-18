#include "AnalyticTableaux.h"

#include <iostream>

/**
 * Método construtor. Inicia o tableau com um nó cuja expressão 
 * é expression e cujo valor-verdade é truthValue. 
 * 
 * Exemplo: Node tableau = Node("((~P) v P)", false)
*/
Node::Node(string expression, bool truthValue)
{
    this->expression = expression;
    this->truthValue = truthValue;
    this->isAppliable = regex_match(expression, regex("\\(.*\\)"));
    this->isContradictory = false;
    this->leftChild = nullptr;
    this->rightChild = nullptr;
    this->frontChild = nullptr;
    this->parent = nullptr;
}

/**
 * Bifurca a árvore e insere dois nós em cada ramo, um com expressão expression1 e valor-verdade truthValue1,
 * e outro com expressão expression2 e valor-verdade truthValue2, em todas as folhas alcançáveis
 * a partir deste nó, e retorna todos os nós adicionados por essa operação.
 * 
 * Exemplo: vector<Node*> insertedNodes = node.InsertSides("P", false, "Q", true)
*/
vector<Node *> Node::insertSides(string expression1, bool truthValue1, string expression2, bool truthValue2)
{
    vector<Node *> insertedNodes;
    vector<Node *> reachableLeafs = getReachableLeafs();
    for (Node *leaf : reachableLeafs)
    {
        Node *leftChild = new Node(expression1, truthValue1);
        Node *rightChild = new Node(expression2, truthValue2);
        insertNode(leaf, leftChild, rightChild);
        insertedNodes.push_back(leftChild);
        insertedNodes.push_back(rightChild);
    }
    return insertedNodes;
}

/**
 * Insere um nó com expressão expression e valor-verdade truthValue em todas as folhas alcançáveis
 * a partir deste nó, e retorna todos os nós adicionados por essa operação.
 * A outra versão permite inserir dois nós.
 * 
 * Exemplo: vector<Node*> insertedNodes = node.InsertFront("(~P)", false)
*/
vector<Node *> Node::insertFront(string expression, bool truthValue)
{
    vector<Node*> insertedNodes;
    vector<Node *> reachableLeafs = getReachableLeafs();
    for (Node *leaf : reachableLeafs)
    {
        Node *child = new Node(expression, truthValue);
        insertNode(leaf, child);
        insertedNodes.push_back(child);
    }
    return insertedNodes;
}

/**
 * Insere um nó com expressão expression e valor-verdade truthValue em todas as folhas alcançáveis
 * a partir deste nó, e retorna todos os nós adicionados por essa operação.
 * A outra versão permite inserir dois nós.
 * 
 * Exemplo: vector<Node*> insertedNodes = node.InsertFront("(~P)", false)
*/
vector<Node *> Node::insertFront(string expression1, bool truthValue1, string expression2, bool truthValue2)
{
    vector<Node *> insertedNodes;
    vector<Node *> reachableLeafs = getReachableLeafs();
    for (Node *leaf : reachableLeafs)
    {
        Node *child1 = new Node(expression1, truthValue1);
        Node *child2 = new Node(expression2, truthValue2);
        insertNode(leaf, child1);
        insertNode(child1, child2);
        insertedNodes.push_back(child1);
        insertedNodes.push_back(child2);
    }
    return insertedNodes;
}

void Node::insertNode(Node *parent, Node *frontChild)
{
    parent->frontChild = frontChild;
    frontChild->parent = parent;
}

void Node::insertNode(Node *parent, Node *leftChild, Node *rightChild)
{
    parent->leftChild = leftChild;
    parent->rightChild = rightChild;
    leftChild->parent = rightChild->parent = parent;
}

vector<Node *> Node::getReachableLeafs()
{
    vector<Node *> allNodes = traverse(new vector<Node *>);
    vector<Node *> reachableLeafs;
    for (Node *node : allNodes)
    {
        if (node->isLeaf())
            reachableLeafs.push_back(node);
    }
    return reachableLeafs;
}

bool Node::isLeaf()
{
    return leftChild == nullptr && rightChild == nullptr && frontChild == nullptr && !isContradictory;
}

/**
 * Retorna todos os nós alcançáveis a partir deste nó em que se pode aplicar uma regra. 
 * Para retornar todos os nós da árvore em que se pode aplicar uma regra, o nó chamado deve ser a raiz.
 * 
 * Exemplo: vector<Node*> appliableNodes = tableau.getAppliableNodes()
*/
vector<Node *> Node::getAppliableNodes()
{
    vector<Node *> allNodes = traverse(new vector<Node *>);
    vector<Node *> appliableNodes;
    for (Node *node : allNodes)
    {
        if (node->isAppliable)
            appliableNodes.push_back(node);
    }
    return appliableNodes;
}

vector<Node *> Node::traverse(vector<Node *> *nodes)
{
    nodes->push_back(this);
    if (frontChild != nullptr)
        frontChild->traverse(nodes);
    else if (leftChild != nullptr && rightChild != nullptr)
    {
        leftChild->traverse(nodes);
        rightChild->traverse(nodes);
    }
    return *nodes;
}

/**
 * Marca o nó como sido aplicado uma regra. Ele não aparecerá mais em chamadas de getAppliableNodes().
 * 
 * Exemplo: node.markApplied()
*/
void Node::markApplied()
{
    isAppliable = false;
}

/**
 * Checa se a subárvore a partir deste nó está fechada.
 * Para saber se o tableau inteiro está fechado, o nó chamado deve ser a raiz.
 * 
 * Exemplo: bool isClosed = tableau.isClosed()
*/
bool Node::isClosed()
{
    return getReachableLeafs().empty();
}

/**
 * Checa se o nó contradiz algum outro do mesmo ramo.
 * 
 * Exemplo: bool isContradictory = node.checkContradiction()
*/
bool Node::checkContradiction()
{
    for (Node *it = this->parent; it != nullptr; it = it->parent)
    {
        if ((it->expression == this->expression && it->truthValue == !this->truthValue) || it->isContradictory)
            return true;
    }
    return false;
}

/**
 * Marca o nó como contraditório. Seu ramo será fechado e ignorado agora em diante.
 * 
 * Exemplo: node.markContradiction()
*/
void Node::markContradiction()
{
    isContradictory = true;
}

/**
 * Imprime a árvore a partir do nó atual.
 * Para imprimir toda a árvore, o nó chamado deve ser a raiz.
 * 
 * Exemplo: tableau.printTree()
*/
void Node::printTree()
{
    printTree(0);
}

void Node::printTree(int level)
{
    for (int i = 0; i < level; i++)
        cout << '\t';

    cout << "v(" << this->expression << ") = " << this->truthValue << (isContradictory ? " X " : "") << endl;
    if (frontChild != nullptr)
    {
        frontChild->printTree(level + 1);
    }
    else if (leftChild != nullptr && rightChild != nullptr)
    {
        leftChild->printTree(level + 1);
        rightChild->printTree(level + 1);
    }
}

/**
 * Retorna a proposição do nó.
 * 
 * Exemplo: string expression = node.getExpression()
*/
string Node::getExpression()
{
    return expression;
}

/**
 * Retorna o valor-verdade do nó.
 * 
 * Exemplo: bool truthValue = node.getTruthValue()
*/
bool Node::getTruthValue()
{
    return truthValue;
}