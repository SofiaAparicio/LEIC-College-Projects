/**
* @file al067.cpp
*
* @author Ana Sofia Costa n 81105
* @author Joao Pedro Silvestre n 80996
* @date 17 March 2016
*
* @brief Project developed for the course ASA as a solution for the first
project.
*/

#include <cstdio>
#include <list>
#include <vector>
#include <algorithm>
#define NIL -1
using namespace std;


class Vertex{
private:
  /*
  * @param[int] time of discovery, lowest time, parent discoverd through the DFS algorithm
  * @param[bool] _visited by the DFS algorithm, _fund if the vertex was added to
  fundamental list
  */
  int _num, _dis, _low, _parent;
  bool _visited, _fund;

public:
  Vertex(int num){
    _num = num;
    _low = _dis = _parent= NIL;
    _visited = _fund = false;
  }
  int getNum(){return _num;}
  int getLow(){return _low;}
  int getDis(){return _dis;}
  int getParent(){return _parent;}
  bool getVisited(){return _visited;}
  bool getFund(){return _fund;}
  void setDis(int dis){_dis=dis;}
  void setLow(int low){_low=low;}
  void setParent(int parent){_parent=parent;}
  void setVisited(bool visited){_visited=visited;}
  void setFund(bool fund){_fund = fund;}
};

typedef list<Vertex*> adlist;

/**
* @class Graph
*/
class Graph{
  private:
    /**
    * @param[int] number os vertexs, time of the DFS algorithm
    * @param[int] fundamental with the lowest number, fundamental with the max
    number, number of fundamentals found
    * @param[vector<adlist>] 
    */
    int _vertexNum, _time;
    int _minf, _maxf, _nFund;
    vector<adlist> _graph;

  public:
    Graph(int vertexNum) {
      _graph = vector<adlist>(vertexNum);
      _vertexNum = vertexNum;
      _minf = _maxf = NIL;
      _nFund = _time = 0;
      for(int i = 0; i < _vertexNum; i++){
        adlist newList;
        newList.push_front(new Vertex(i+1));
        _graph[i] = newList;
      }
    }


    void addEdge(int p1, int p2){
      _graph[p1-1].push_back(_graph[p2-1].front());
      _graph[p2-1].push_back(_graph[p1-1].front());
    }

    void DFS(){
      if(_graph[0].front()->getVisited() == false)
        FundFinder(0);
      printf("%d\n", _nFund);
      printf("%d %d\n", _minf, _maxf);
    }

    void FundFinder(int position){
      _time++;
      _graph[position].front()->setDis(_time);
      _graph[position].front()->setLow(_time);
      _graph[position].front()->setVisited(true);
      int nChildren = 0; //Number of children in DFS
      list<Vertex*>::iterator i;
      for(i = ++_graph[position].begin(); i != _graph[position].end(); i++){
        Vertex* a = *i;
        Vertex* begin = _graph[position].front();
        if(a -> getVisited() == false){
          nChildren++;
          a->setParent(position);
          FundFinder(a->getNum() - 1);
          begin -> setLow(min(begin->getLow(),a->getLow()));

          if(((begin->getParent()) == NIL && (nChildren > 1))
           || ((begin->getParent() != NIL) && (a->getLow() >= begin->getDis() ))){
             if( begin->getFund() == false){
              begin->setFund(true);

              _nFund++;
              if(_minf == NIL || _maxf == NIL)
                _minf = _maxf = begin->getNum();
              _minf = min(_minf, begin->getNum());
              _maxf = max(_maxf, begin->getNum());
            }
          }
        }
        else if((a->getNum() -1) != begin->getParent())
          begin -> setLow(min(begin->getLow(), a->getDis()));
      }
    }
};

int main(){
  int nada, i, people, bridge,p1,p2;
  nada = scanf("%d %d\n", &people, &bridge);
  Graph g(people);
  for(i=0; i < bridge; i++){
    nada = scanf("%d %d\n", &p1, &p2);
    nada++;
    g.addEdge(p1,p2);
  }
  g.DFS();

}
