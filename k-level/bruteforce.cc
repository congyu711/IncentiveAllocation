// upper envelope(convex hull)
#include <bits/stdc++.h>
using namespace std;

class obj
{
public:
    double v, c, idx;
    obj(double _v, double _c, int _idx) : v(_v), c(_c), idx(_idx) {}
    obj() : v(0.), c(0.), idx(-1) {}
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
        {fin >> objs[0][j].v>>objs[0][j].c;objs[0][j].idx=j;}
    // for (int j = 0; j < m; j++)
    // {
    //     fin >> data[0][j].c;
    //     data[0][j].idx = j;
    // }
    // objs = data;
    // preprocessing
    // for every group, draw every object(value,cost) on a 2D plane,
    // keep those objects lying on the bottom-right convex hull and delete others
    auto crossproduct = [&](obj st1, obj ed1, obj st2, obj ed2) -> double
    { return (ed1.v - st1.v) * (ed2.c - st2.c) -
             (ed1.c - st1.c) * (ed2.v - st2.v); };
    auto CH_Andrew = [&](vector<obj> &points) -> vector<obj>
    {
        vector<obj> res;
        // res.push_back(obj(0, 0, -1));
        sort(points.begin(), points.end(), [&](obj a, obj b) -> bool
             {
            if (fabs(a.v - b.v) < 1e-9)
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
        while(objs[0].end()[-1].v==objs[0].end()[-2].v)    objs[0].pop_back();
        // while (objs[k].back().v < objs[k].end()[-2].v)
        //     objs[k].pop_back();
        ofstream fout("bf.out");
        for(auto e:objs[k]) fout<<e.v<<' '<<e.c <<'\n';
    }
}