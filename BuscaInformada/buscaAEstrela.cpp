// Trabalho de Inteligência Artificial
// Busca A Estrela (A*)
// Quebra-cabeça e Labirinto

#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <map>

typedef std::vector<std::vector<int>> vvi;
typedef std::pair<int,int> pii;
typedef long long int lli;

struct State {
    // Informações de um determinado estado
    lli id; // Momento em que o estado foi descoberto (0 = estado inicial)
    lli father; // Estado que descobriu ele
    int heuristic; // Valor heurística
    int cost; // Quanto custou para chegar nesse estado
    int row; // Linha que se encontra o elemento INICIALMENTE a se 'movimentar' na matriz
    int column; // Coluna que se encontra o elemento INICIALMENTE a se 'movimentar' na matriz
    int depth; // Profundidade do estado
    vvi matrix; // Matriz para guardar o estado
    
    // Construtor
    State(lli id, lli father, int heuristic, int cost, int row, int column, int depth, int N, vvi matrix)
        : id(id), father(father), heuristic(heuristic), cost(cost), row(row), column(column), depth(depth) {
            this->matrix.resize(N,std::vector<int>(N));
            this->matrix = matrix;
    }

    // É usado para ordenação
    bool operator < (const State &other) const {
        return (cost > other.cost);
    }
};

std::map<int,pii> solutionOpc1; // Solução quebra-cabeça
std::set<vvi> visitedStates; // Estados que já visitei
std::vector<State> states; // guardará todos os estados que foram descobertos. Na posição 0 fica o estado inicial; posição 1 fica o primeiro estado descoberto e assim sucessivamente...
int N, OPC, userHeuristic; // tamanho da matriz, jogo selecionado para busca e tipo de heurística escolhida pelo usuário
int rowEnd = -1, columnEnd = -1; // Pos. final labirinto

int ansCost = -1;
int ansId = -1;

lli searchA(State state) {
    std::priority_queue<State> Queue; // Fila para a busca A*
    Queue.push(state);
    visitedStates.insert(state.matrix); // Estado inicial visitado
    
    // Auxiliar para percorrer a matriz
    int dx[] = {-1,1,0,0};
    int dy[] = {0,0,-1,1};
    
    // Busca A*
    while(!Queue.empty()) {
        State current = Queue.top();
        Queue.pop();

        if(current.heuristic == 0) {
            if(ansCost == -1) {
                ansCost = current.cost;
                ansId = current.id;
            } else {
                if(current.cost < ansCost) {
                    ansCost = current.cost;
                    ansId = current.id;
                }
            }
            return ansId;
            continue;
        }

        for(int k = 0; k < 4; k++) {
            int x = current.row+dx[k];
            int y = current.column+dy[k];

            // Verifico se x e y são posições válidas
            if(x < 0 or x >= N or y < 0 or y >= N) continue;

            // Jogo do labirinto não pode ir em posições 0's
            if(OPC == 2 and current.matrix[x][y] == 0) continue;

            vvi newState(N,std::vector<int>(N));

            bool solution = true;
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

                    bool error = false; // indica se a posicao está errada

                    // Calcula heurística desse estado
                    if(OPC == 1) { // Quebra-cabeça
                        if(i == N/2 and j == N/2) {
                            if(newState[i][j] != 0) {
                                error = true;
                            }
                        } else { 
                            if(newState[i][j] != count) {
                                error = true;
                            }
                            count++;
                        }
                        if(error) {
                            if(userHeuristic == 1) { // Peças na posição errada
                                heuristic++;
                            } else { // Soma das distâncias até a posição correta
                                int e = newState[i][j];
                                heuristic += std::abs(i-solutionOpc1[e].first) + std::abs(j-solutionOpc1[e].second);
                            }
                        }
                    } 
                }
            }
            
            // Heuristica do labirinto
            if(OPC == 2) {
                heuristic = std::abs(rowEnd-x) + std::abs(columnEnd-y);
            }

            int cost = 1+current.cost+heuristic;

            // Estado já foi visitado
            if(visitedStates.find(newState) != visitedStates.end()) continue;

            visitedStates.insert(newState);
            State New(states.size(),current.id,heuristic,cost,x,y,current.depth+1,N,newState);
            states.push_back(New);
            Queue.push(New);
        }
    }
    return ansId;
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
        
        int heuristic = 0;
        int count = 1;

        // Calcula heurística do estado inicial
        if(OPC == 1) { // Quebra-cabeça
            for(int i = 0; i < N; i++) {
                for(int j = 0; j < N; j++) {
                    bool error = false; // indica se tem erro
                    if(i == N/2 and j == N/2) {
                        if(matrix[i][j] != 0)
                            error = true;
                    } else { 
                        if(matrix[i][j] != count)
                            error = true;
                        count++;
                    }

                    if(error) {
                        if(userHeuristic == 1) { // Peças na posição errada
                            heuristic++;
                        } else { // Soma das distâncias até a posição correta
                            int e = matrix[i][j];
                            int dist = std::abs(i-solutionOpc1[e].first) + std::abs(j-solutionOpc1[e].second);
                            heuristic += dist;
                        }
                    }
                }
            }
        } else { // Labirinto
            heuristic = std::abs(rowEnd-row) + std::abs(columnEnd-column);
        }

        // Adicionando meu estado inicial no meu vetor de estados
        states.push_back(State(0,0,heuristic,0,row,column,0,N,matrix));

        lli ans = searchA(*states.begin());
        
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