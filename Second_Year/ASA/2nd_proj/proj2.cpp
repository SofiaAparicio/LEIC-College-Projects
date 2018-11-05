#include <cstdio>
#include <iostream>
#include <limits.h>
#include <set>
#include <vector>

using namespace std;

typedef pair<int, int> set_item; //set_item.first: altura de um vertice com indice set_item.second no vector _graph
typedef pair<int, int> edge;//edge.first: vertice de destno (v), edge.secund: peso (w)


class Graph{
  private:
    vector< vector<edge> > _graph;
    int _vertexNum, _numFiliais, _min, _minCost;
    vector<int> _totalCost;
    vector<int> _filiais;
    vector<int> _height;


  public:
    Graph(int vertexNum, int filiais) {
      _vertexNum = vertexNum;
      _numFiliais = filiais;
      _graph = vector< vector<edge> >(_vertexNum);
      _height = vector<int>(_vertexNum,0);
      _min = -1;
      _minCost = INT_MAX/2;
      _filiais = vector<int>(_numFiliais);
      _totalCost = vector<int>(_vertexNum, 0);
    }

    void addFilial(int i, int filial){
      _filiais[i] = filial-1;
    }

    void addGraphEdge(int u, int v, int w){
      _graph[u-1].push_back(edge(v-1,w));
    }

    void Bellman_Ford(){
      for(int i = 0; i < _vertexNum-1; i++){
        for(unsigned int j = 0; j < _graph[i].size() ; j++){
          if(_height[i] + _graph[i][j].second < _height[_graph[i][j].first]){
            _height[_graph[i][j].first] = _height[i] + _graph[i][j].second;
          }
        }
      }

      for(int x = 0; x < _vertexNum; x++){
        for(unsigned int y = 0; y < _graph[x].size() ; y++ ){
          _graph[x][y].second = _graph[x][y].second + _height[x] -_height[ _graph[x][y].first];
        }
      }
    }

    vector<int> Dijkstra(int filial){
      vector<int> dist = vector<int>(_vertexNum, INT_MAX/2);
      set<set_item> graph_set;

      graph_set.insert(set_item(0,filial));

      while(!graph_set.empty()){
        set_item first = *graph_set.begin();
        graph_set.erase(graph_set.begin());

        int d = first.first, iv = first.second;

        dist[iv] = d;

        for(unsigned int i = 0; i < _graph[iv].size(); i++){
          int v_adj = _graph[iv][i].first, cost = _graph[iv][i].second;
          if(dist[v_adj] > dist[iv] + cost){
              if(dist[v_adj] != INT_MAX/2)
                graph_set.erase(set_item(dist[v_adj], v_adj));
              graph_set.insert(set_item(dist[iv] + cost, v_adj));
              dist[v_adj] = dist[iv] + cost;
          }
        }
      }
      return dist;
    }

    void johnson(){
      vector<int> dij;
      Bellman_Ford();
      for(int i = 0; i < _numFiliais; i++){
          dij = Dijkstra(_filiais[i]);
          for(int j = 0; j < _vertexNum; j++){
            if(dij[j] != INT_MAX/2 && _totalCost[j] != INT_MAX/2){
              _totalCost[j] +=  dij[j] + _height[j] - _height[_filiais[i]];
            }
            else{
              _totalCost[j] = INT_MAX/2;
            }
          }
      }
      for(int j = 0; j < _vertexNum; j++){
        if(_totalCost[j] < _minCost){
          _minCost = _totalCost[j];
          _min = j;
        }
      }
      if(_min == -1){
        printf("N\n");
      }
      else{
        printf("%d %d\n", _min+1, _minCost);
        for(int i = 0; i < _numFiliais; i++){
            dij = Dijkstra(_filiais[i]);
            printf("%d ", dij[_min] + _height[_min] - _height[_filiais[i]]);
        }
        printf("\n");
      }
    }
};

int main(){
  int nada, u, v, w, filial, numLocalidades, numFiliais, numLigacoes;
  nada = scanf("%d %d %d", &numLocalidades, &numFiliais, &numLigacoes);
  Graph g(numLocalidades, numFiliais);
  for(int i=0; i< numFiliais; i++){
    nada = scanf("%d ", &filial);
    g.addFilial(i, filial);
  }
  for(int j=0; j < numLigacoes; j++){
    nada = scanf("%d %d %d", &u, &v, &w);
    nada++;
    g.addGraphEdge(u, v, w);
  }
  g.johnson();

}
