#include <iostream>
#define _ ios_base::sync_with_stdio(0); cin.tie(0);
using namespace std;

typedef pair<int,int> pii;
typedef struct Node {
    int **mtx;
}Node;   

void destroyMtx(int **matrix, int rows) {
    // Função para desalocar toda a memória alocada para matriz
    for(int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
}

void solve() {
    // Aqui será feito a busca em largura para os problemas
}

int main() { _
    int N, OPC; // OPC = Qual jogo está fazendo o teste ; N = matriz NxN
    cin >> OPC >> N; 
    
    int **matrix = new int*[N];
    pii BEGIN, END;

    for(int i = 0; i < N; i++) {
        matrix[i] = new int[N];
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
    solve();
    destroyMtx(matrix,N);
    return 0;
}