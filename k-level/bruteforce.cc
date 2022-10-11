/**
 * @file lyftmethod.cpp
 * @author congyu
 * @brief this is another method for the problem in
 * https://eng.lyft.com/how-to-solve-a-linear-optimization-problem-on-incentive-allocation-5a8fb5d04db1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <bits/stdc++.h>
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
    double lambda,rdiff;
    bkpoint(int _l,int _r,double _lambda,double _rd):lobj(_l),robj(_r),lambda(_lambda),rdiff(_rd){}
};
int main()
{
    long long n, m, C;
    ifstream fin("data.in");
    fin.ignore(numeric_limits<streamsize>::max(),'\n');
    // ofstream fout("lyft.out");
    fin >> m;
    vector<vector<obj>> objs(1, vector<obj>(m)), data(1, vector<obj>(m));
    for (int j = 0; j < m; j++)
        {fin >> data[0][j].v>>data[0][j].c;data[0][j].idx=j;}
    // for (int j = 0; j < m; j++)
    // {
    //     fin >> data[0][j].c;
    //     data[0][j].idx = j;
    // }
    objs = data;
    // preprocessing
    // for every group, draw every object(value,cost) on a 2D plane,
    // keep those objects lying on the bottom-right convex hull and delete others
    auto crossproduct = [&](obj st1, obj ed1, obj st2, obj ed2) -> long long
    { return (ed1.v - st1.v) * (ed2.c - st2.c) -
             (ed1.c - st1.c) * (ed2.v - st2.v); };
    auto CH_Andrew = [&](vector<obj> &points) -> vector<obj>
    {
        vector<obj> res;
        // res.push_back(obj(0, 0, -1));
        sort(points.begin(), points.end(), [&](obj a, obj b) -> bool
             {
            if (fabs(a.v - b.v) < 1e-6)
                return a.c < b.c;
            return a.v < b.v; });
        for (int i = points.size()-1; i >=0 ; i--)
        {
            while (res.size() >= 2 && crossproduct(res.end()[-2], points[i], res.end()[-2],
                                                   res.end()[-1]) >= 0)
                res.pop_back();
            res.push_back(points[i]);
        }
        return res;
    };
    for (int k = 0; k < 1; k++)
    {
        objs[k] = CH_Andrew(objs[k]);
        // while (objs[k].back().v < objs[k].end()[-2].v)
        //     objs[k].pop_back();
        ofstream fout("bf.out");
        for(auto e:objs[k]) fout<<e.v<<' '<<e.c <<'\n';
    }
    // compute the LP dual opt
    // 1. maintain breakpoints for each set of objs
    // can be done better?
    // vector<vector<bkpoint>> bkpts(n);
    // for (int k = 0; k < n; k++)
    // {
    //     double _l;
    //     // _l=(double)objs[k][0].v/(double)objs[k][0].c;
    //     // if(objs[k][0].c!=0)    bkpts[k].push_back(bkpoint(0,-1,_l));
    //     for (int i = 0; i < objs[k].size() - 1; i++)
    //     {
    //         _l=(double)(objs[k][i].v-objs[k][i+1].v)/(double)(objs[k][i].c-objs[k][i+1].c);
    //         if(_l>=0)    bkpts[k].push_back(bkpoint(i+1,i,_l,-objs[k][i].c));
    //     }
    //     bkpts[k].push_back(bkpoint(-1,objs[k].size()-1,0.0,-objs[k].end()[-1].c));
    //     // for(auto e:bkpts[k])    cout<<e.lambda<<' '<<e.lobj<<' '<<e.robj<<' '<<e.rdiff<<endl;
    //     // cout<<endl;
    // }
    // // pair<int,int> --> first=line idx of vector<vector<bkpts>>; second=col number of vector<bkpts> 
    // auto cmp=[&](pair<int,int> a,pair<int,int> b){
    //     if(bkpts[a.first][a.second].lambda==bkpts[b.first][b.second].lambda)
    //         return objs[a.first][bkpts[a.first][a.second].robj].c<objs[b.first][bkpts[b.first][b.second].robj].c;
    //     return bkpts[a.first][a.second].lambda<bkpts[b.first][b.second].lambda;};
    // priority_queue<pair<int,int>,vector<pair<int,int> >,decltype(cmp)> pq(cmp);
    // double CurrentSlope=C;
    // vector<int> idxs(n,0);
    // for(int i=0;i<n;i++)
    //     pq.push(make_pair(i,0));
    // while(pq.size())
    // {
    //     auto T=pq.top();
    //     pq.pop();
    //     if(T.second!=bkpts[T.first].size()-1)
    //     {
    //         pq.push(make_pair(T.first,T.second+1));
    //         CurrentSlope-=bkpts[T.first][T.second].rdiff;
    //         CurrentSlope+=bkpts[T.first][T.second+1].rdiff;
    //         if(CurrentSlope<0) break;
    //         idxs[T.first]++;
    //     }
    // }
    // cout<<"currentsolpe "<<CurrentSlope<<endl;
    // // if(pq.size())   L=bkpts[pq.top().first][pq.top().second].lambda;
    // // else L=0;
    // double opt=0.0,cost=0.0;
    // for(int i=0;i<n;i++)
    // {
    //     auto e=objs[i][bkpts[i][idxs[i]].robj];
    //     // cout<<bkpts[i][idxs[i]].robj<<' '<<e.v<<' '<<e.c<<endl;
    //     // opt+=e.v-e.c*L;
    //     opt+=e.v;
    //     cost+=e.c;
    //     fout<<e.c<<' '<<e.v<<endl;
    //     fout<<endl;
    // }
    // cout<<"opt: "<<opt<<endl;
    // cout<<"cost: "<<cost<<endl;
}