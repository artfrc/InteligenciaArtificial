#include <iostream>
#include <queue>
#include <set>
#define _ ios_base::sync_with_stdio(0); cin.tie(0);
using namespace std;

typedef pair<int,int> pii;
typedef vector<vector<int>> vvi;
typedef queue<vector<vector<int>>> qvvi;

// auxilia para percorrer a matriz
int dx[] = {-1,1,0,0}; 
int dy[] = {0,0,-1,1};


int OPC; // 1 -> Jogo quebra-cabeça ; 2 -> Jogo do labirinto
int totalNodes = 0;

pii BEGIN; // posição que personagem começa no labirinto ou vazio quebra-cabeça

pii END; // posição final para o personagem no labirinto

set<vvi> checkedStates; // guardo os estados já visitados

/* =================================================================
/   DEIXEI ESSAS VARIÁVEIS GLOBAIS PARA FACILITAR COM AS FUNÇÕES!
/   FICARIAM MUITOS PARÂMETROS PARA PASSAR
/ ================================================================= */ 

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

bool solve(int i, int j, qvvi& qMatrix, int N) {
    // Aqui será feito a busca em largura para os problemas
    queue<pii> Queue;
    Queue.push({i,j}); // Guardo a posição i e j
    
    while(!Queue.empty()) {
        vvi matrix = qMatrix.front(); // matriz representando o estado atual
        qMatrix.pop();

        // posições que personagem está no labirinto ou espaço vazio no quebra-cabeça
        i = Queue.front().first;
        j = Queue.front().second;
        Queue.pop();


        // Printa o estado que estou
        
        cout << ">> Estado: " << totalNodes++ << endl;
        
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
            return true;
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

            checkedStates.insert(newState); // registro que esse novo estado foi visitado
            qMatrix.push(newState); // coloco ele na fila da busca
            Queue.push({x,y}); // posições do meu novo estado que se encontra o personagem no labirinto ou posição vazia no quebra-cabeça
        }
    }
    return false; // não encontrou solução
}

int main() { _
    int N; // N = matriz NxN
    while(cin >> OPC >> N) {
        totalNodes = 0;
        if(OPC == 1) {
            cout << "*** QUEBRA CABEÇA ***" << endl;
        } else {
            cout << "*** LABIRINTO ***" << endl;
        }

        vvi matrix(N,vector<int>(N)); // estado inicial
        qvvi qMatrix; // fila de estados para a busca

        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                // Lendo a matriz inicial
                cin >> matrix[i][j];
                if(OPC == 1) { // Quebra-cabeça
                    if(matrix[i][j] == 0) { // Posição vaga no quebra-cabça
                        BEGIN.first = i;
                        BEGIN.second = j;
                    }
                } else { // Labirinto
                    if(matrix[i][j] == 2) { // Posição inicial do personagem
                        BEGIN.first = i;
                        BEGIN.second = j;
                    } else if(matrix[i][j] == 3) { // Posição que deseja chegar
                        END.first = i;
                        END.second = j;
                    }
                }
            }
        }
       
        qMatrix.push(matrix); // Inserindo minha matriz inicial na fila
       
        if(solve(BEGIN.first,BEGIN.second,qMatrix,N)) {
            cout << ">> Encontrei!" << endl;
        } else {
            cout << ">> Nao encontrei!" << endl;
        }
        cout << "Total de nos: " << totalNodes << endl;
        
        matrix.clear();
        checkedStates.clear();
    } 
    return 0;
}