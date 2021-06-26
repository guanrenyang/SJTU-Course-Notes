#include <vector>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;
//Please put this source code in the same directory with SCC.in
//And do NOT change the file name.
/*
This function computes the number of Strongly Connected Components in a graph
Args:
    n: The number of nodes in the graph. The nodes are indexed as 0~n-1
    edge: The edges in the graph. For each element (a,b) in edge, it means
          there is a directed edge from a to b
          Notice that there may exists multiple edge and self-loop
Return:
    The number of strongly connected components in the graph.
*/
int Count=0;
struct SEtimeNode// record the id of each node and the order of them doing "postvisit"
{
public:
    int ID=0;
    int End=0;
    SEtimeNode(int iD=0,int end=0)
    {
        ID=iD;
        End=end;
    }
    bool operator< ( const SEtimeNode & b) const
    {
        return this->End<b.End;
    }
};

// recursion functions.
// explore_traverse is DFS the graph with each edge truning direction
// explore is DFS the initial graph
void explore_traverse(int i,int n,const vector<pair<int,int>> & edge, vector<bool> & visited,priority_queue<SEtimeNode> &SEtime);
void explore(int i,int n,const vector<pair<int,int>> & edge, vector<bool> & visited);
int SCC(int n, vector<pair<int,int>>& edge) {

    vector<bool> visited(n,0);
    priority_queue<SEtimeNode> SEtime;

    for(int i=0;i<n;i++)
    {
        if(!visited[i])
            explore_traverse(i,n,edge,visited,SEtime);
    }

    visited.clear();
    visited.resize(n,0);
    int sccCount=0;
    while(!SEtime.empty())
    {
        SEtimeNode tmp=SEtime.top();
        SEtime.pop();

        if(!visited[tmp.ID])
        {
            explore(tmp.ID,n,edge,visited);
            sccCount++;
        }
    }
    return sccCount;
}

void explore(int i,int n,const vector<pair<int,int>> & edge, vector<bool> & visited)
{
    visited[i]=true;

    for(int j=0;j<n;j++)
    {
        if(edge[j].first==i&&!visited[edge[j].second])// change the direction
            explore(edge[j].second,n,edge,visited);
    }
}
void explore_traverse(int i,int n,const vector<pair<int,int>> & edge, vector<bool> & visited,priority_queue<SEtimeNode> &SEtime)
{
    visited[i]=true;
    // previsit
    Count++;

    for(int j=0;j<n;j++)
    {
        if(edge[j].second==i&&!visited[edge[j].first])// change the direction
            explore_traverse(edge[j].first,n,edge,visited,SEtime);
    }

    // postvisit
    Count++;
    SEtimeNode tmp(i,Count);
    SEtime.push(tmp);

}
//Please do NOT modify anything in main(). Thanks!
int main()
{
    int m,n;
    vector<pair<int,int>> edge;
    ifstream fin;
    ofstream fout;
    fin.open("SCC.in");
    cout<<fin.is_open()<<endl;
    fin>>n>>m;
    cout<<n<<" "<<m<<endl;
    int tmp1,tmp2;
    for(int i=0;i<m;i++)
    {
        fin>>tmp1>>tmp2;
        edge.push_back(pair<int,int>(tmp1,tmp2));
    }
    fin.close();
    int ans=SCC(n,edge);
    fout.open("SCC.out");
    fout<<ans<<'\n';
    fout.close();
    return 0;
}
