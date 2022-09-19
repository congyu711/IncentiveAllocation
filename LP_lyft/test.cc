// for p>=2 ...
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
    double lambda, rdiff;
    bkpoint(int _l, int _r, double _lambda, double _rd) : lobj(_l), robj(_r), lambda(_lambda), rdiff(_rd) {}
};
int main()
{
    // long long n, m, C;
    // const long long p = 5;
    // ifstream fin("data.in");
    // // ofstream fout("lyft.out");
    // fin >> n >> m >> C;
    // vector<vector<obj>> objs(n, vector<obj>(m)), data(n, vector<obj>(m));
    // for (int i = 0; i < n; i++)
    //     for (int j = 0; j < m; j++)
    //         fin >> data[i][j].v;
    // for (int i = 0; i < n; i++)
    //     for (int j = 0; j < m; j++)
    //     {
    //         fin >> data[i][j].c;
    //         data[i][j].idx = j;
    //     }
    // objs = data;
    vector<obj> vec;
    for(int i=0;i<10;i++)
    {
        vec.push_back(obj(rand()%20,rand()%20,i));
    }
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
            if (fabs(a.c - b.c) < 1e-6)
                return a.v < b.v;
            return a.c < b.c; });
        for (int i = 0; i < points.size(); i++)
        {
            while (res.size() >= 2 && crossproduct(res.end()[-2], points[i], res.end()[-2],
                                                   res.end()[-1]) >= 0)
                res.pop_back();
            res.push_back(points[i]);
        }
        int _sz=res.size();
        for (int i = points.size() - 2; i >= 0; i--)
        {
            while (res.size()>=_sz+2 && crossproduct(res.end()[-2], points[i], res.end()[-2],
                                                   res.end()[-1]) >= 0)
                res.pop_back();
            res.push_back(points[i]);
        }
        res.pop_back();
        return res;
    };
    vector<obj> CH=CH_Andrew(vec);
    int idx=20;
    for(int k=1;k<4;k++)
    {
        auto _CH=CH;
        CH.clear();
        for(auto e:_CH)
        {
            for()
        }
    }
}