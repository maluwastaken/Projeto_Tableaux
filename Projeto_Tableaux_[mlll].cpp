#include <iostream>
#include "AnalyticTableaux.cpp"
#include <string>
#include <sstream>
#include<fstream>

/*Maria Luisa Lima - mlll*/

using namespace std;

vector<Node*> applyRule(Node* node) {
    vector<Node*> insertedNodes;
     string tmp = node->getExpression();
     //qdo o op principal eh o nao
    if(tmp[1]=='~'){
      //copia a expressao sem os parenteses externos e o nao
      string str = tmp.substr(2, tmp.length()-3);
      //inverte o valor verdade
      insertedNodes = node->insertFront(str, !(node->getTruthValue()));
    }
    else{//se nao for o nao, tem q procurar o op principal e colocar o valor-verdade de acordo com a tabela verdade
      int esq=0,dir=0;string sEsq, sDir;
      for(int i=1; i<tmp.length()-1; i++){
        if(tmp[i]=='(') esq++;
        if(tmp[i]==')') dir++;
        if(esq == dir) {
          if(tmp[i] == '>'){
            sEsq = tmp.substr(1, i-2);
            sDir = tmp.substr( i+2, tmp.length()-i-3);
            if(node->getTruthValue()==0)
              insertedNodes = node->insertFront(sEsq, true, sDir, false);
            else
              insertedNodes = node->insertSides(sEsq, false, sDir, true);
            break;
          }
          else if (tmp[i] == 'v'){
            sEsq = tmp.substr(1, i-2);
            sDir = tmp.substr( i+2, tmp.length()-i-3);
            if(node->getTruthValue()==0)
              insertedNodes = node->insertFront(sEsq, false, sDir, false);
            else
              insertedNodes = node->insertSides(sEsq, true, sDir, true);
            break;
          }
          else if(tmp[i] == '&'){
            sEsq = tmp.substr(1, i-2);
            sDir = tmp.substr( i+2, tmp.length()-i-3);
            if(node->getTruthValue()==1)
              insertedNodes = node->insertFront(sEsq, true, sDir, true);
            else
              insertedNodes = node->insertSides(sEsq, false, sDir, false);
            break;
          }
        }
      }
    }
    node->markApplied();
    return insertedNodes;
}

//checa contradicao em todo o tableau. se achar, marca ela;
void checkContradictions(vector<Node*> insertedNodes) {
    for (int i = 0; i < insertedNodes.size(); i++) {
        if(insertedNodes[i]->checkContradiction())
            insertedNodes[i]->markContradiction();
    }
}

bool isAlfa(Node *node){
//determina se uma dada expressao eh alfa baseada em seu valor verdade e operador principal
  string tmp = node->getExpression();
  int esq=0,dir=0;
  if(tmp[1] == '~') return true;
  else{
    for(int i=1; i<tmp.length()-1; i++){
      if(tmp[i]=='(') esq++;
      if(tmp[i]==')') dir++;
      if(esq == dir) {
        if(tmp[i] == '>' && node->getTruthValue() == 0)
          return true;
        else if(tmp[i] == '>' && node->getTruthValue() == 1)
          return false;
        else if(tmp[i] == 'v' && node->getTruthValue() == 0)
          return true;
        else if(tmp[i] == 'v' && node->getTruthValue() == 1)
          return false;
        else if(tmp[i] == '&' && node->getTruthValue() == 1)
          return true;
        else if(tmp[i] == '&' && node->getTruthValue() == 0)
          return false;
      }
    }
  }
  return false;
}

int main(){
    int buffEsq, buffDir, i, count=0,n, buffVir;
    string pergunta, expr, buff, problema, buffCons, temp;
    ifstream myFileA; ofstream myFileOut;

    //abrindo os arquivos de entrada e saida
    myFileA.open("Entrada.in");
    myFileOut.open("Saida.out");
    myFileA >> n;
    getline(myFileA, pergunta);
    Node tableau = Node(pergunta, false);

    for(int k=0; k<n; k++){
      getline (myFileA, pergunta);
      //se o problema for consequencia logica, oq tiver antes do 'e' eh a conclusao, q eh colocada no tableu com valor verdade 0
      if(pergunta.find("consequencia logica")!= -1){
        problema = "consequencia logica";
        buffDir = pergunta.find_first_of('e', 0);
        buffCons = pergunta.substr(0, buffDir-1);
        tableau = Node(buffCons, false);
      }
      else{//se n for consequencia logica, sera inserido no tableau oq tiver ate o ultimo parenteses, com valor verdade a ser posto de acordo com o problema
        buffDir = pergunta.find_last_of(')', pergunta.length());
        expr = pergunta.substr(0, buffDir+1);

        if(pergunta.find("tautologia")!= -1){
          problema = "tautologia";
          tableau = Node(expr, false);
        }
        else if(pergunta.find("refutavel")!= -1){
          problema = "refutavel";
          tableau = Node(expr, false);
        }
        else if(pergunta.find("satisfativel")!= -1 && pergunta.find("insatisfativel") == -1){
          problema = "satisfativel";
          tableau = Node(expr, true);
        }
        else if(pergunta.find("insatisfativel")!= -1){
          problema = "insatisfativel";
          tableau = Node(expr, true);
        }
      }
      vector<Node*> appliableNodes;
      /*
      se for consequencia logica, eh preciso pegar todas as premissas que estao entre chaves e possivelmente separadas por virgulas
      (quando tem mais de uma). Logo, eh preciso procurar de pedaco em pedaco (entre a chave esquerda e virgula, entre virgula e virgula ou
      entre virgula e chave direita), adicionando a expressao que for encontrada ao tableau, com valor verdade 1.
      Enquanto isso, deve-se checar se ha uma contradicao entre as proprias premissas e/ou conclusao.
      */
      if(problema == "consequencia logica"){
        //determina o espaco onde as premissas estao localizadas
        buffEsq = pergunta.find_first_of('{', 0);
        buffDir = pergunta.find_last_of('}', pergunta.length());
        do{
          //percorre o espaco das premissas, buscando-as individualmente
          //estas sao adicionadas ao tableau e a cada adicao eh checado se ha contradicao
          buffVir = pergunta.find_first_of(',', buffEsq);
          if(buffVir == -1){
            temp = pergunta.substr(buffEsq+1, buffDir-buffEsq-1);
            appliableNodes = tableau.insertFront(temp, true);
          }
          else{
            temp = pergunta.substr(buffEsq+1, buffVir-(buffEsq+1));
            appliableNodes = tableau.insertFront(temp, true);
          }
          buffEsq = buffVir+1;
          checkContradictions(appliableNodes);
        }while(buffVir != -1 && !tableau.checkContradiction());
        //as premissas acabam quando nao houver mais virgulas (a premissa apos a ultima virgula ja deve ter sido inserida)
      }
      /*
      Agora deve-se procurar os nos onde eh possivel aplicar regras e aplica-las, checando por contradicoes
      */
      appliableNodes = tableau.getAppliableNodes();
      while(!tableau.isClosed()  && !appliableNodes.empty()){
        //percorre o vetor procurando regras alfa pra aplicar
        //se encontrar, aplica. Se n, pula pra o prox
        //se n tiver alfa, volta ao inicio do tableau, procurando a primeira regra beta disponivel a ser apliada
        for(int i = 0; i < appliableNodes.size(); i++) {
          if(isAlfa(appliableNodes[i]) == true){
            //enquanto houver regras alfa a serem aplicadas, estas serao
            //a cada nova insercao de regra no tableau, o loop volta ao inicio das regras ainda a serem aplicadas
            //de forma que, caso uma nova regra alfa seja inserida, ela ainda tera prioridade sobre as betas inseridas antes dela
            vector<Node*> insertedNodes = applyRule(appliableNodes[i]);
            checkContradictions(insertedNodes);
            appliableNodes = tableau.getAppliableNodes();
            i=-1;
            if(appliableNodes.empty()) break;
          }
        }//quando nao houver mais regras alfa, volta-se ao inicio do tableau e aplica-se a primeira regra beta
        appliableNodes = tableau.getAppliableNodes();
        if(!appliableNodes.empty()){
          vector<Node*> insertedNodes = applyRule(appliableNodes[0]);
        checkContradictions(insertedNodes);
        }appliableNodes = tableau.getAppliableNodes();
      }

      //printa a resposta final do problema perguntado, de acordo com o tableau gerado
      if(problema == "tautologia"){
        if(tableau.isClosed())
          myFileOut << "Problema #"<< k+1 << endl << "Sim, e tautologia." << endl;
        else
          myFileOut << "Problema #"<< k+1 << endl << "Nao, nao e tautologia." << endl;
      }
      if(problema == "satisfativel"){
        if(!tableau.isClosed())
          myFileOut << "Problema #"<< k+1 << endl << "Sim, e satisfativel." << endl;
        else
          myFileOut << "Problema #"<< k+1 << endl << "Nao, nao e satisfativel." << endl;
      }
      if(problema == "insatisfativel"){
        if(tableau.isClosed())
          myFileOut << "Problema #"<< k+1 << endl << "Sim, e insatisfativel." << endl;
        else
          myFileOut << "Problema #"<< k+1 << endl << "Nao, nao e insatisfativel." << endl;
      }
      if(problema == "refutavel"){
        if(!tableau.isClosed())
          myFileOut << "Problema #"<< k+1 << endl << "Sim, e refutavel." << endl;
        else
          myFileOut << "Problema #"<< k+1 << endl << "Nao, nao e refutavel." << endl;
      }
      if(problema == "consequencia logica"){
        if(tableau.isClosed())
          myFileOut << "Problema #"<< k+1 << endl << "Sim, e consequencia logica." << endl;
        else
          myFileOut << "Problema #"<< k+1 << endl << "Nao, nao e consequencia logica." << endl;
      }
      myFileOut << endl;

  //tableau.printTree();
    }
    return 0;
}
