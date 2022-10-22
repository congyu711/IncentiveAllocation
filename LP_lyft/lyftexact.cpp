// for p>=2 ...
// with k-level
// k-level use y=ax-b, hence a=-c and b=-v
#include <bits/stdc++.h>
#include "../k-level/klevel.cc"
using namespace std;

class obj
{
public:
    int v, c, idx;
    obj(int _v, int _c, int _idx) : v(_v), c(_c), idx(_idx) {}
    obj() : v(0), c(0), idx(-1) {}
};
class bkpoint
{
public:
    int lobj, robj;
    double lambda, rdiff;
    bkpoint(int _l, int _r, double _lambda, double _rd) : lobj(_l), robj(_r), lambda(_lambda), rdiff(_rd) {}
};
int main()
{
    long long n, m, C;
    const long long p = 3;
    ifstream fin("data.in");
    // ofstream fout("lyft.out");
    fin >> n >> m >> C;
    vector<vector<obj>> objs(n, vector<obj>(m)), data(n, vector<obj>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            fin >> data[i][j].v;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
        {
            fin >> data[i][j].c;
            data[i][j].idx = j;
        }
    objs = data;


    
    // compute the LP dual opt
    // 1. maintain breakpoints for each set of objs
    // can be done better?
    vector<vector<bkpoint>> bkpts(n);
    for (int k = 0; k < n; k++)
    {
        double _l;
        // _l=(double)objs[k][0].v/(double)objs[k][0].c;
        // if(objs[k][0].c!=0)    bkpts[k].push_back(bkpoint(0,-1,_l));
        for (int i = 0; i < objs[k].size() - 1; i++)
        {
            _l = (double)(objs[k][i].v - objs[k][i + 1].v) / (double)(objs[k][i].c - objs[k][i + 1].c);
            if (_l >= 0)
                bkpts[k].push_back(bkpoint(i + 1, i, _l, -objs[k][i].c));
        }
        bkpts[k].push_back(bkpoint(-1, objs[k].size() - 1, 0.0, -objs[k].end()[-1].c));
        // for(auto e:bkpts[k])    cout<<e.lambda<<' '<<e.lobj<<' '<<e.robj<<' '<<e.rdiff<<endl;
        // cout<<endl;
    }
    // pair<int,int> --> first=line idx of vector<vector<bkpts>>; second=col number of vector<bkpts>
    auto cmp = [&](pair<int, int> a, pair<int, int> b)
    {
        if(bkpts[a.first][a.second].lambda==bkpts[b.first][b.second].lambda)
            return objs[a.first][bkpts[a.first][a.second].robj].c<objs[b.first][bkpts[b.first][b.second].robj].c;
        return bkpts[a.first][a.second].lambda<bkpts[b.first][b.second].lambda; };
    priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(cmp)> pq(cmp);
    double CurrentSlope = C;
    vector<int> idxs(n, 0);
    for (int i = 0; i < n; i++)
        pq.push(make_pair(i, 0));
    while (pq.size())
    {
        auto T = pq.top();
        pq.pop();
        if (T.second != bkpts[T.first].size() - 1)
        {
            pq.push(make_pair(T.first, T.second + 1));
            CurrentSlope -= bkpts[T.first][T.second].rdiff;
            CurrentSlope += bkpts[T.first][T.second + 1].rdiff;
            if (CurrentSlope < 0)
                break;
            idxs[T.first]++;
        }
    }
    cout << "currentsolpe " << CurrentSlope << endl;
    // if(pq.size())   L=bkpts[pq.top().first][pq.top().second].lambda;
    // else L=0;
    double opt = 0.0, cost = 0.0;
    for (int i = 0; i < n; i++)
    {
        auto e = objs[i][bkpts[i][idxs[i]].robj];
        // cout<<bkpts[i][idxs[i]].robj<<' '<<e.v<<' '<<e.c<<endl;
        // opt+=e.v-e.c*L;
        opt += e.v;
        cost += e.c;
        // fout<<e.c<<' '<<e.v<<endl;
        // fout<<endl;
    }
    cout << "opt: " << opt << endl;
    cout << "cost: " << cost << endl;
}