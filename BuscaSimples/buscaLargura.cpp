#include <iostream>
#include <queue>
#define _ ios_base::sync_with_stdio(0); cin.tie(0);
using namespace std;

typedef pair<int,int> pii;

int dx[] = {-1,1,0,0};
int dy[] = {0,0,-1,1};
int OPC; // 1 -> Jogo quebra-cabeça ; 2 -> Jogo do labirinto
int totalNodes = 0;
pii BEGIN, END;

void destroyMtx(int **matrix, int rows) {
    // Função para desalocar toda a memória alocada para matriz
    for(int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
}

bool isSolution(int i, int j, int **mtx, int rows) {
    // Verifica se cheguei na solução do problema
    if(OPC == 1) { // Quebra cabeça
        if(mtx[rows-1][rows-1] != 0) return false;
        else {
            bool ok = true;
            int count = 1;
            for(int i = 0; i < rows and ok; i++) {
                for(int j = 0; j < rows and ok; j++) {
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

void solve(int i, int j, int **mtx, int **vis, int rows) {
    // Aqui será feito a busca em largura para os problemas
    queue<pii> Queue;
    Queue.push({i,j});
    vis[i][j] = true;
    while(!Queue.empty()) {
        i = Queue.front().first;
        j = Queue.front().second;
        cout << ">> Posicao: [" << i << "][" << j << "]" << endl;
        Queue.pop();
        totalNodes++;
        if(isSolution(i,j,mtx,rows)) {
            return;
        }
        for(int k = 0; k < 4; k++) {
            int x = i+dx[k];
            int y = j+dy[k];
            if(x < 0 or x >= rows or y < 0 or y >= rows or vis[x][y]) continue;
            if(OPC == 2 and !mtx[x][y]) continue;
            Queue.push({x,y});
            vis[x][y] = true;
        }
    }
}

int main() { _
    int N; // N = matriz NxN
    bool flag = false;
    int t = 0;
    while(cin >> OPC >> N) {
        if(flag) cout << endl;
        flag = true;
        cout << ">> Teste: " << ++t << endl;
        if(OPC == 1) {
            cout << "*** QUEBRA CABEÇA ***" << endl;
        } else {
            cout << "*** LABIRINTO ***" << endl;
        }
        int **matrix = new int*[N];
        int **vis = new int*[N];


        for(int i = 0; i < N; i++) {
            matrix[i] = new int[N];
            vis[i] = new int[N];
            for(int j = 0; j < N; j++) {
                // Lendo a matriz inicial e criando matriz de visitados
                cin >> matrix[i][j];
                vis[i][j] = 0;
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
        solve(BEGIN.first,BEGIN.second,matrix,vis,N);
        if(vis[END.first][END.second]) {
            cout << ">> Encontrei!" << endl;
        } else {
            cout << ">> Nao encontrei!" << endl;
        }
        cout << "Total de nos: " << totalNodes << endl;
        
        destroyMtx(matrix,N);
        destroyMtx(vis,N);
    } 
    return 0;
}