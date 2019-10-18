class Node

/**
 * Método construtor. Inicia o tableau com um nó cuja expressão 
 * é expression e cujo valor-verdade é truthValue. 
 * 
 * Exemplo: Node tableau = Node("((~P) v P)", false)
 */
Node(string expression, bool truthValue)

/**
 * Insere um nó com expressão expression e valor-verdade truthValue em todas as folhas alcançáveis
 * a partir deste nó, e retorna todos os nós adicionados por essa operação.
 * A outra versão permite inserir dois nós.
 * 
 * Exemplo: vector<Node*> insertedNodes = node.InsertFront("(~P)", false)
 */
vector<Node *> insertFront(string expression, bool truthValue)
vector<Node *> insertFront(string expression1, bool truthValue1, string expression2, bool truthValue2)

/**
 * Bifurca a árvore e insere dois nós em cada ramo, um com expressão expression1 e valor-verdade truthValue1,
 * e outro com expressão expression2 e valor-verdade truthValue2, em todas as folhas alcançáveis
 * a partir deste nó, e retorna todos os nós adicionados por essa operação.
 * 
 * Exemplo: vector<Node*> insertedNodes = node.InsertSides("P", false, "Q", true)
 */
vector<Node *> insertSides(string expression1, bool truthValue1, string expression2, bool truthValue2)

/**
 * Retorna todos os nós alcançáveis a partir deste nó em que se pode aplicar uma regra. 
 * Para retornar todos os nós da árvore em que se pode aplicar uma regra, o nó chamado deve ser a raiz.
 * 
 * Exemplo: vector<Node*> appliableNodes = tableau.getAppliableNodes()
 */
vector<Node *> getAppliableNodes()

/**
 * Marca o nó como sido aplicado uma regra. Ele não aparecerá mais em chamadas de getAppliableNodes().
 * 
 * Exemplo: node.markApplied()
 */
void markApplied()

/**
 * Checa se a subárvore a partir deste nó está fechada.
 * Para saber se o tableau inteiro está fechado, o nó chamado deve ser a raiz.
 * 
 * Exemplo: bool isClosed = tableau.isClosed()
 */
bool isClosed()

/**
 * Checa se o nó contradiz algum outro do mesmo ramo.
 * 
 * Exemplo: bool isContradictory = node.checkContradiction()
 */
bool checkContradiction()

/**
 * Marca o nó como contraditório. Seu ramo será fechado e ignorado agora em diante.
 * 
 * Exemplo: node.markContradiction()
 */
void markContradiction()

/**
 * Retorna a proposição do nó.
 * 
 * Exemplo: string expression = node.getExpression()
 */
string getExpression()

/**
 * Retorna o valor-verdade do nó.
 * 
 * Exemplo: bool truthValue = node.getTruthValue()
 */
bool getTruthValue()

/**
 * Imprime a árvore a partir do nó atual.
 * Para imprimir toda a árvore, o nó chamado deve ser a raiz.
 * 
 * Exemplo: tableau.printTree()
 */
void printTree()