#include <iostream>
#include <queue>
#include <set>
#include <time.h>
#include <iomanip>
#define _ ios_base::sync_with_stdio(0); cin.tie(0);
using namespace std;

const int SUM = 5;

typedef pair<int,int> pii;
typedef vector<vector<int>> vvi;

// Struct para guardar a posição da matriz de um nó e a altura desse nó
typedef struct Node {
    int row;
    int column;
    int height;
    vvi matrix;
    Node() {}
    Node(int row, int column, int height, vvi matrix) : row(row), column(column), height(height), matrix(matrix) {}
}Node;

Node BEGIN; // posição inicial na matriz
Node aux; // irá auxiliar na função solve

pii END; // posição final para o personagem no labirinto

// auxilia para percorrer a matriz
int dx[] = {-1,1,0,0}; 
int dy[] = {0,0,-1,1};

int heuristic; // tipo de heuristica escolhida pelo usuário para o quebra cabeça
int OPC; // 1 -> Jogo quebra-cabeça ; 2 -> Jogo do labirinto
long long int totalNodes = 0;

set<vvi> checkedStates; // guardo os estados já visitados

/* =================================================================
/   DEIXEI ESSAS VARIÁVEIS GLOBAIS PARA FACILITAR COM AS FUNÇÕES!  / 
/              FICARIAM MUITOS PARÂMETROS PARA PASSAR              /
/ =============================================================== */ 

bool isSolution(int i, int j, vvi mtx, int N) {
    // Verifica se cheguei na solução do problema
    if(OPC == 1) { // Quebra cabeça
        if(mtx[N/2][N/2] != 0) {
            return false;
        }
        else {
            bool ok = true;
            int count = 1;
            for(int i = 0; i < N and ok; i++) {
                for(int j = 0; j < N and ok; j++) {
                    if(i == N/2 and j == N/2) continue;
                    if(mtx[i][j] != count) {
                        ok = false;
                    }
                    count++;
                }
            }
            return ok;
        }
    } else { // Labirinto
        return (i == END.first and j == END.second);
    }
}

bool FIND = false; // irá dizer se uma solução foi encontrada
bool recursion = false; // irá dizer se é preciso chamar a recursão para uma busca mais profunda caso a solução ainda não tenha sido encontrada

bool solve(Node node, int N, int maxHeight) {

    if(FIND) return FIND; // caso já achei minha solução, então não deixo minha recursão continuar..
    
    // Aqui será feito a busca em profundiade para os problemas

    checkedStates.insert(node.matrix); // Estados que já foram visitados

    int i,j,h;
    vvi matrix = node.matrix; // matriz representando o estado atual
    h = node.height;

    // posições que personagem está no labirinto ou espaço vazio no quebra-cabeça
    i = node.row;
    j = node.column;

    // Printa o estado que estou
        
    cout << ">> Estado: " << ++totalNodes << endl;
        
    for(int l = 0; l < N; l++) {
        for(int r = 0; r < N; r++) {
            cout << matrix[l][r] << " ";
        }
        cout << endl;
    }
    cout << endl;
        
    // =====================================================

    // Verifica se cheguei na solução
    if(isSolution(i,j,matrix,N)) {
        aux = node;
        FIND = true;
        return FIND;
    }

    // Faço a busca pelos possíveis próximos estados
    for(int k = 0; k < 4; k++) {
        int x = i+dx[k];
        int y = j+dy[k];
        
        // Verifico se são posições válidas para a matriz
        if(x < 0 or x >= N or y < 0 or y >= N) continue;

        // Se for labirinto, então não deixo visitar posições que são 0 na matriz
        if(OPC == 2 and !matrix[x][y]) continue;

        vvi newState(N,vector<int>(N)); // um possível estado que ainda não foi visitado
            
        // monto minha matriz/estado
        for(int l = 0; l < N; l++) {
            for(int r = 0; r < N; r++) {
                if(l == x and r == y) {
                    newState[l][r] = matrix[i][j];    
                } else if(l == i and r == j) {
                    newState[l][r] = matrix[x][y];
                }else {
                    newState[l][r] = matrix[l][r];                    
                }
            }
        }

        // verifico se já visitei esse estado
        if(checkedStates.find(newState) != checkedStates.end()) continue;
        if(h+1 > maxHeight) {
            recursion = true;
            continue;
        }

        Node newNode(x,y,h+1,newState);
        solve(newNode,N,maxHeight);
    }
    return FIND; // não encontrou solução
}

int main() { _
    clock_t start, end; // calcular o tempo de execução do código
    start = clock(); // marca o início
    int N; // N = matriz NxN
    int t = 0;
    bool flag = false;
    while(cin >> OPC >> N >> heuristic) {
        if(flag) cout << endl << "=============================================" << endl << endl;
        flag = true;
        cout << "Teste " << ++t << ": " << endl;
        totalNodes = 0;
        vvi matrix(N,vector<int>(N)); // estado inicial

        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                // Lendo a matriz inicial
                cin >> matrix[i][j];
                if(OPC == 1) { // Quebra-cabeça
                    if(matrix[i][j] == 0) { // Posição vaga no quebra-cabça
                        BEGIN.row = i;
                        BEGIN.column = j;
                    }
                } else { // Labirinto
                    if(matrix[i][j] == 2) { // Posição inicial do personagem
                        BEGIN.row = i;
                        BEGIN.column = j;
                    } else if(matrix[i][j] == 3) { // Posição que deseja chegar
                        END.first = i;
                        END.second = j;
                    }
                }
            }
        }
        BEGIN.matrix = matrix;
        BEGIN.height = 0;
        int maxHeight = 0;
        Node INIT = BEGIN;
        FIND = false;
        do{
            if(maxHeight) {
                cout << ">> Profundidade max (" << maxHeight-SUM << ") atingida!" << endl;
                cout << ">> Atualizando profundidade..." << endl << endl;
                totalNodes = 0;
            }
            recursion = false;
            checkedStates.clear();
            solve(INIT,N,maxHeight); // irá dizer se encontrei ou não a solução
            maxHeight += SUM;
        } while(recursion and !FIND);

        end = clock();

        if(FIND) {
            cout << ">> Encontrei!" << endl;
            cout << ">> Profundidade da meta: " << aux.height << endl;
        } else {
            cout << ">> Nao encontrei!" << endl;
        }
        cout << ">> Total de nos: " << totalNodes << endl;

        double time_taken = double(end-start) / double(CLOCKS_PER_SEC);

        cout << fixed << setprecision(8);
        cout << ">> Tempo de execucao: " << time_taken << " segundos." << endl;
        
        matrix.clear();
        checkedStates.clear();
    } 
    return 0;
}