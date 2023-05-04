#include <iostream>
#include <vector>
#include <queue>
#include <set>
using namespace std;

const int MAXN = 10;

typedef pair<int,int> pii;
typedef pair<pair<int,int>,pair<int,int>> ppp;

int N;
int dx[] = {-1,1,0,0};
int dy[] = {0,0,-1,1};
pii BEGIN, END;

typedef struct Tree {
    int matrix[MAXN][MAXN];
    vector<Tree> node;
    Tree(int mtx[][MAXN]) {
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                matrix[i][j] = mtx[i][j];
            }
        }
    }
}Tree;

Tree *tree = nullptr;
set<ppp> st;

Tree* insertBT(Tree* tree, int matrix[][MAXN]) {
    if(!tree) // árvore vazia
        return new Tree(matrix); // crio um novo nó

    tree->node.push_back(matrix);

    return tree;
}

void solve(int i, int j) {
    queue<pii> Queue;
    Queue.push({i,j});
    Tree *aux = tree;
    while(!Queue.empty()) {
        i = Queue.front().first;
        j = Queue.front().second;
        Queue.pop();
        for(int k = 0; k < 4; k++) {
            int x = dx[k]+i;
            int y = dy[k]+j;
            if(x < 0 or x >= N or y < 0 or y >= N) continue;
            ppp current = {{i,j},{x,y}}; 
            if(st.find(current) != st.end()) continue;
            st.insert(current);
            Queue.push({x,y});
            int mtx[MAXN][MAXN];
            for(int line = 0; line < N; line++) {
                for(int column = 0; column < N; column++) {
                    if(line == i and column == j) {
                        mtx[i][j] = 0;
                    } else {
                        mtx[i][j] = aux->matrix[i][j];
                    }
                }
            }
            aux = insertBT(aux,mtx);
            aux = NULL;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0);
    cin >> N;
    int matrix[MAXN][MAXN];
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            cin >> matrix[i][j];
            if(matrix[i][j] == 2) {
                BEGIN.first = i;
                BEGIN.second = j;
            } else if(matrix[i][j] == 3){
                END.first = i;
                END.second = j;
            }
        }
    }

    tree = insertBT(tree,matrix);
    
    auto aux = tree;
    while(aux != NULL) {
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < N; j++) {
                cout << aux->matrix[i][j] << " ";
            }
            cout << endl;
        }
        aux = NULL;
    }
    solve(BEGIN.first, BEGIN.second);   
}