// Trabalho de Inteligência Artificial
// Busca Simulated Annealing
// Quebra-cabeça e Labirinto

#include <iostream>
#include <vector>
#include <set>
#include <map>

typedef std::vector<std::vector<int>> vvi;
typedef std::pair<int,int> pii;
typedef long long int lli;

// Maximo de vezes que irá gerar estados aleatórios
const int  MAXTEST = 100000;

struct State {
    // Informações de um determinado estado
    lli id; // Momento em que o estado foi descoberto (0 = estado inicial)
    lli father; // Estado que descobriu ele
    int heuristic; // Valor heurística
    int row; // Linha que se encontra o elemento INICIALMENTE a se 'movimentar' na matriz
    int column; // Coluna que se encontra o elemento INICIALMENTE a se 'movimentar' na matriz
    vvi matrix; // Matriz para guardar o estado
    
    // Construtor
    State(lli id, lli father, int heuristic, int row, int column, int N, vvi matrix)
        : id(id), father(father), heuristic(heuristic), row(row), column(column) {
            this->matrix.resize(N,std::vector<int>(N));
            this->matrix = matrix;
    }
};

std::map<int,pii> solutionOpc1; // Solução quebra-cabeça
std::set<vvi> visitedStates; // Estados que já visitei
std::vector<State> states; // guardará todos os estados que foram descobertos. Na posição 0 fica o estado inicial; posição 1 fica o primeiro estado descoberto e assim sucessivamente...
int N, OPC, userHeuristic; // tamanho da matriz, jogo selecionado para busca e tipo de heurística escolhida pelo usuário
int rowEnd = -1, columnEnd = -1; // Pos. final labirinto

bool isSolution(vvi matrix, int i, int j) {
    if(OPC == 1) { // Quebra-cabeça
        int count = 1;
        for(int r = 0; r < N; r++) {
            for(int c = 0; c < N; c++) {
                if(r == N/2 and c == N/2 and matrix[r][c] != 0) {
                    return false;
                } else {
                    if(matrix[r][c] != count) return false;
                    count++;
                }
            }
        }
        return true;
    } else { // Labirinto
        return (i == rowEnd and j == columnEnd);
    }
}

int calculateHeuristic(vvi matrix, int row, int column) {
    int heuristic;
    int count = 1;
    // Calcula heurística do estado inicial
        if(OPC == 1) { // Quebra-cabeça
            for(int i = 0; i < N; i++) {
                for(int j = 0; j < N; j++) {
                    bool correct = true; // indica se tem erro
                    if(i == N/2 and j == N/2) {
                        if(matrix[i][j] != 0)
                            correct = false;
                    } else { 
                        if(matrix[i][j] != count)
                            correct = false;
                        count++;
                    }

                    if(correct) {
                        if(userHeuristic == 1) { // Peças na posição correta
                            heuristic++;
                        } else { // Soma das distâncias até a posição correta
                            int e = matrix[i][j];
                            int dist = std::abs(i-solutionOpc1[e].first) + std::abs(j-solutionOpc1[e].second);
                            heuristic += dist;
                        }
                    }
                }
            }
            if(N == 3) {
                heuristic = 50-heuristic;
            } else {
                heuristic = 100-heuristic;
            }
        } else { // Labirinto
            heuristic = std::abs(rowEnd-row) + std::abs(columnEnd-column);
        }
        return heuristic;
}

lli simulatedAnnealing(State state) {
    visitedStates.insert(state.matrix); // Estado inicial visitado
    
    // Auxiliar para percorrer a matriz
    int dx[] = {-1,1,0,0};
    int dy[] = {0,0,-1,1};

    State current = state;
    State next = current;
    bool ok;
    int currentHeuristic = state.heuristic;
    std::vector<State> nextStates;

    // Simulated Annealing
    do {
        ok = false;
        current = next;
        for(int k = 0; k < 4; k++) {
            int x = current.row+dx[k];
            int y = current.column+dy[k];

            // Verifico se x e y são posições válidas
            if(x < 0 or x >= N or y < 0 or y >= N) continue;

            // Jogo do labirinto não pode ir em posições 0's
            if(OPC == 2 and current.matrix[x][y] == 0) continue;

            vvi newState(N,std::vector<int>(N));

            int count = 1;
            int heuristic = 0;

            // Novo estado
            for(int i = 0; i < N; i++) {
                for(int j = 0; j < N; j++) {
                    if(i == x and j == y) {
                        newState[i][j] = current.matrix[current.row][current.column];    
                    } else if(i == current.row and j == current.column) {
                        newState[i][j] = current.matrix[x][y];
                    } else {
                        newState[i][j] = current.matrix[i][j];                    
                    }

                    bool correct = true; // indica se a posicao está errada

                    // Calcula heurística desse estado
                    if(OPC == 1) { // Quebra-cabeça
                        if(i == N/2 and j == N/2) {
                            if(newState[i][j] != 0) {
                                correct = false;
                            }
                        } else { 
                            if(newState[i][j] != count) {
                                correct = false;
                            }
                            count++;
                        }
                        if(correct) {
                            if(userHeuristic == 1) { // Peças na posição correta
                                heuristic++;
                            } else { // Soma das distâncias até a posição correta
                                int e = newState[i][j];
                                heuristic += std::abs(i-solutionOpc1[e].first) + std::abs(j-solutionOpc1[e].second);
                            }
                        }
                    } 
                }
            }

            if(OPC == 1) {
                if(N == 3) {
                    heuristic = 50-heuristic;
                } else {
                    heuristic = 100-heuristic;
                }
            }
                
            // Heuristica do labirinto
            if(OPC == 2) {
                heuristic = std::abs(rowEnd-x) + std::abs(columnEnd-y);
            }

            // Estado já foi visitado
            if(visitedStates.find(newState) != visitedStates.end()) continue;

            visitedStates.insert(newState);
            State New(states.size(),current.id,heuristic,x,y,N,newState);
            states.push_back(New);
            if(New.heuristic < currentHeuristic) {
                currentHeuristic = New.heuristic;
                nextStates.push_back(next);
                next = New;
                ok = true;
            } else {
                nextStates.push_back(New);
            }
        }
    } while(ok); 
    return current.id;
}

int main() {
    // Apenas deixa o código um pouco mais rápido
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    // Enquanto houver dados no arquivo de entrada para ler, o código continuará rodando
    while(std::cin >> OPC >> N) {

        // Somente no quebra-cabeça que tem duas heurísticas para escolher
        if(OPC == 1) {
            std::cin >> userHeuristic;
        }

        vvi matrix(N,std::vector<int>(N));
        int row,column;
        int countSolutionOpc1 = 1;

        // Leitura da matriz (Estado inicial)
        // Monto também a solução do quebra-cabeça caso for o escolhido
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                
                std::cin >> matrix[i][j];

                // Guardando a solução do quebra-cabeça
                if(OPC == 1) {
                    if(i == N/2 and j == N/2) {
                        solutionOpc1[0] = {i,j};
                    } else {
                        solutionOpc1[countSolutionOpc1] = {i,j};
                        countSolutionOpc1++;
                    }
                }

                // Posicoes iniciais dos jogos
                if((OPC == 1 and matrix[i][j] == 0) or (OPC == 2 and matrix[i][j] == 2)) {
                    row = i;
                    column = j;
                } else if(OPC == 2 and matrix[i][j] == 3) {
                    // Posicao final do labirinto
                    rowEnd = i;
                    columnEnd = j;
                }
            }
        }
        
        int heuristic = calculateHeuristic(matrix,row,column);
        int count = 1;

        // Adicionando meu estado inicial no meu vetor de estados
        states.push_back(State(0,0,heuristic,row,column,N,matrix));

        lli ans = simulatedAnnealing(states.back());
        
        std::cout << ">> Solucao encontrada:" << std::endl;
        std::vector<int> stepsSolution;
        bool aux = false;

        // Marcando o caminho encontrado para solução   
        while(ans != 0) {
            stepsSolution.push_back(ans);
            ans = states[ans].father;
        }
        stepsSolution.push_back(0);
            
        // Printando o percurso em ordem
        for(auto it = stepsSolution.rbegin(); it != stepsSolution.rend(); it++) {
            std::cout << std::endl;
            std::cout << "Estado: " << *it << std::endl;
            for(int i = 0; i < N; i++) {
                bool aux = false;
                for(int j = 0; j < N; j++) {
                    if(aux) std::cout << " ";
                    aux = true;
                    std::cout << states[*it].matrix[i][j];
                }
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;

        // Resultados finais
        std::cout << ">> Numero de nos visitados: " << visitedStates.size() << std::endl;
        std::cout << ">> Custo da solucao: " << stepsSolution.size() << std::endl;
    }

    // Libera a memória
    visitedStates.clear();
    states.clear();
}