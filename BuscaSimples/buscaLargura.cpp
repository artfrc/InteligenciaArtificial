#include <iostream>
#include <queue>
#include <set>
#define _ ios_base::sync_with_stdio(0); cin.tie(0);
using namespace std;

typedef pair<int,int> pii;
typedef pair<pii,int> ppi;
typedef pair<pii,pii> ppp;
typedef vector<vector<int>> vvi;
typedef queue<vector<vector<int>>> qvvi;

int dx[] = {-1,1,0,0};
int dy[] = {0,0,-1,1};
int OPC; // 1 -> Jogo quebra-cabeça ; 2 -> Jogo do labirinto
int totalNodes = 0;
int flag = 0; // Irá ajudar marcar a posição que irei inserir meu novo estado no vetor de matrizes
pii BEGIN, END;
set<vvi> checkedStates;

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
                    // cout << "count = " << count << " mtx[i][j] = " << mtx[i][j] << endl; 
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
    // Guardo a posição i e j que estou e também guardo o index (idxArr) do vetor de Matrizes que devo guardar meu próximo estado
    Queue.push({i,j});
    int state = 0;
    while(!Queue.empty()) {
        vvi matrix = qMatrix.front(); // matriz representando o estado atual
        qMatrix.pop();

        // posições que estou na matriz atual
        i = Queue.front().first;
        j = Queue.front().second;
        Queue.pop();

        // cout << ">> Posicao: [" << i << "][" << j << "]" << endl;
        cout << ">> Estado: " << state++ << endl;
        for(int l = 0; l < N; l++) {
            for(int r = 0; r < N; r++) {
                cout << matrix[l][r] << " ";
            }
            cout << endl;
        }
        cout << endl;

        totalNodes++;
        if(isSolution(i,j,matrix,N)) {
            return true;
        }
        for(int k = 0; k < 4; k++) {
            int x = i+dx[k];
            int y = j+dy[k];
            if(x < 0 or x >= N or y < 0 or y >= N) continue;
            vvi newState(N,vector<int>(N));
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
            if(checkedStates.find(newState) != checkedStates.end()) continue;
            if(OPC == 2 and !matrix[x][y]) continue;
            // cout << "x = " << x << " y = " << y  << " N = " << N << endl;
            
            // Guardo o meu novo estado
            checkedStates.insert(newState);
            qMatrix.push(newState);
            Queue.push({x,y});
        }
    }
    return false;
}

int main() { _
    int N; // N = matriz NxN
    bool flag = false;
    int t = 0;
    while(cin >> OPC >> N) {
        totalNodes = 0;
        if(flag) cout << "==================================================" << endl;
        flag = true;
        cout << ">> Teste: " << ++t << endl;
        if(OPC == 1) {
            cout << "*** QUEBRA CABEÇA ***" << endl;
        } else {
            cout << "*** LABIRINTO ***" << endl;
        }

        vvi matrix(N,vector<int>(N));
        qvvi qMatrix;

        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                // Lendo a matriz inicial e criando matriz de visitados
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
        qMatrix.push(matrix); // Inserindo minha matriz inicial
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