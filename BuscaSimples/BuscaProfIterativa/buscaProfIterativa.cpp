// Trabalho de Inteligência Artificial
// Busca em Profundidade Iterativa
// Quebra-cabeça e Labirinto

#include <iostream>
#include <vector>
#include <set>
#include <queue>

typedef std::vector<std::vector<int>> vvi;
typedef long long int lli;

struct State {
    // Informações de um determinado estado
    lli id; // Momento em que o estado foi descoberto (0 = estado inicial)
    lli father; // Estado que descobriu ele
    int row; // Linha que se encontra o elemento INICIALMENTE a se 'movimentar' na matriz
    int column; // Coluna que se encontra o elemento INICIALMENTE a se 'movimentar' na matriz
    int depth; // Profundidade do estado
    vvi matrix; // Matriz para guardar o estado
    bool isSolution; // Estado solução ou não
    
    // Construtor
    State(lli id, lli father, int row, int column, int depth, int N, vvi matrix, bool isSolution)
        : id(id), father(father), row(row), column(column), depth(depth), isSolution(isSolution) {
            this->matrix.resize(N,std::vector<int>(N));
            this->matrix = matrix;
    }
};

std::set<vvi> visitedStates; // Estados que já visitei
std::vector<State> states; // guardará todos os estados que foram descobertos. Na posição 0 fica o estado inicial; posição 1 fica o primeiro estado descoberto e assim sucessivamente...
int N, OPC; // tamanho da matriz e jogo selecionado para busca
int rowEnd = -1, columnEnd = -1; // Pos. final labirinto

// Auxiliar para percorrer a matriz
int dx[] = {-1,1,0,0};
int dy[] = {0,0,-1,1};
lli idAns = -1;

// Aumentar profundida da busca em x
const int x = 5;

// Havia mais estados para serem testados
bool recursion = false;

lli dfs(State state, int limit) {
    // Encontrei uma solução
    if(idAns != -1 and state.depth > idAns) return idAns;

    visitedStates.insert(state.matrix); // visitado

    if(state.isSolution) {
        if(recursion) recursion = false;

        if(idAns == -1) {
            return idAns = state.id;
        } else {
            return idAns = std::min(state.id,idAns);
        }
    }

    for(int k = 0; k < 4; k++) {
        int x = state.row+dx[k];
        int y = state.column+dy[k];

        // Verifico se x e y são posições válidas
        if(x < 0 or x >= N or y < 0 or y >= N) continue;

        // Jogo do labirinto não pode ir em posições 0's
        if(OPC == 2 and state.matrix[x][y] == 0) continue;

        vvi newState(N,std::vector<int>(N));

        bool solution = true;
        int count = 1;

        // Novo estado
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                if(i == x and j == y) {
                newState[i][j] = state.matrix[state.row][state.column];    
            } else if(i == state.row and j == state.column) {
                    newState[i][j] = state.matrix[x][y];
                } else {
                newState[i][j] = state.matrix[i][j];                    
                }

                // Confere se é estado solução
                if(solution) {
                    if(OPC == 1) { // Quebra-cabeça
                        if(i == N/2 and j == N/2) {
                            if(newState[i][j] != 0)
                                solution = false;
                        } else { 
                            if(newState[i][j] != count)
                                solution = false;
                            count++;
                        }
                    } else { // Labirinto
                        if(x != rowEnd or y != columnEnd) {
                            solution = false;
                        }
                    }
                }
            }
        }

        // Estado já foi visitado
        if(visitedStates.find(newState) != visitedStates.end()) continue;

        // Está além do limite permitido
        if(state.depth+1 > limit) {
            if(idAns == -1) {
                recursion = true;
            }
            return idAns;
        }

        State New(states.size(),state.id,x,y,state.depth+1,N,newState,solution);
        states.push_back(New);
        dfs(New,limit);
    }
    return idAns;
}

int main() {
    // Apenas deixa o código um pouco mais rápido
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    // Enquanto houver dados no arquivo de entrada para ler, o código continuará rodando
    while(std::cin >> OPC >> N) {

        vvi matrix(N,std::vector<int>(N));
        int row,column;
        bool solution = true;
        int count = 1;

        // Leitura da matriz (Estado inicial)
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                
                std::cin >> matrix[i][j];

                // Posicoes iniciais dos jogos
                if((OPC == 1 and matrix[i][j] == 0) or (OPC == 2 and matrix[i][j] == 2)) {
                    row = i;
                    column = j;
                } else if(OPC == 2 and matrix[i][j] == 3) {
                    // Posicao final do labirinto
                    rowEnd = i;
                    columnEnd = j;
                }

                // Confere se é estado solução
                if(solution) {
                    if(OPC == 1) { // Quebra-cabeça
                        if(i == N/2 and j == N/2) {
                            if(matrix[i][j] != 0)
                                solution = false;
                        } else { 
                            if(matrix[i][j] != count)
                                solution = false;
                            count++;
                        }
                    } else { // Labirinto
                        solution = false;
                    }
                }
            }
        }

        lli ans = -1;
        int limit = 0; // limite da profundidade

        // Se não achar uma solução até a profundidade limit e houver estados para ser visitados com profundidade maior, então a função dfs irá ser chamada
        do {
            // Reseta para uma possível nova busca
            visitedStates.clear();
            states.clear();
            recursion = false;

            // Adicionando meu estado inicial no meu vetor de estados
            states.push_back(State(0,0,row,column,0,N,matrix,solution));

            ans = dfs(*states.begin(),limit);
            
            limit += x;
        } while(recursion);

        if(ans == -1) {
            std::cout << ">> Nao encontrei!" << std::endl;
        } else {
            std::cout << ">> Solucao encontrada!" << std::endl;
            std::vector<int> stepsSolution;
            bool aux = false;
            int depth = states[ans].depth; // Profundidade da solução

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
            std::cout << ">> Profundidade: " << depth << std::endl;
            std::cout << ">> Custo da solucao: " << stepsSolution.size() << std::endl;
        }

        // Libera a memória
        visitedStates.clear();
        states.clear();

    }
}