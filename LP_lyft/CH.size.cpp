#include <bits/stdc++.h>
using namespace std;
const double eps = 1e-6;

class point
{
public:
    double x, y;
    point(double a, double b) : x(a), y(b) {}
    point() : x(0), y(0) {}
};

class _vector
{
public:
    double x, y;
    _vector(point st, point ed)
    {
        x = ed.x - st.x;
        y = ed.y - st.y;
    }
    _vector()
    {
        x = 0, y = 0;
    }
};
double crossproduct(const _vector &a, const _vector &b)
{
    return a.x * b.y - a.y * b.x;
}
class polygon
{
public:
    vector<point> pts;
};
point base = point(0, 0);
double crossproduct(const point &a, const point &b)
{
    return (a.x - base.x) * (b.y - base.y) - (a.y - base.y) * (b.x - base.x);
}
bool cmp1(const point &a, const point &b)
{
    if (fabs(crossproduct(a, b)) < eps)
        return a.x < b.x;
    else
        return crossproduct(a, b) > 0;
}
bool cmp2(const point &a, const point &b)
{
    if (fabs(a.x - b.x) < eps)
        return a.y < b.y;
    return a.x < b.x;
}
vector<point> convexhull;

void Andrew(const polygon &p)
{
    auto points = p.pts;
    sort(points.begin(), points.end(), cmp2);
    vector<point> ch1, ch2;
    for (int i = 0; i < points.size(); i++)
    {
        if (ch1.size())
            base = *(ch1.end() - 1);
        while (ch1.size() >= 2 &&
               crossproduct(_vector(ch1[ch1.size() - 2], points[i]),
                            _vector(ch1[ch1.size() - 2], ch1[ch1.size() - 1])) >= 0)
            ch1.pop_back();
        ch1.push_back(points[i]);
    }
    for (int i = points.size() - 2; i >= 0; i--)
    {
        if (ch2.size())
            base = *(ch2.end() - 1);
        while (ch2.size() >= 2 &&
               crossproduct(_vector(ch2[ch2.size() - 2], points[i]),
                            _vector(ch2[ch2.size() - 2], ch2[ch2.size() - 1])) >= 0)
            ch2.pop_back();
        ch2.push_back(points[i]);
    }
    for (auto e : ch1)
        convexhull.push_back(e);
    for (auto e : ch2)
        convexhull.push_back(e);
    convexhull.pop_back();
}

int main()
{
    polygon p;
    mt19937 gen(time(nullptr));
    int n=gen()%int(1e5), sz=1e5;
    for (int i = 1; i <= n; i++)
    {
        p.pts.push_back(point(gen()%sz, gen()%sz));
    }
    Andrew(p);
    cout<<"N: "<<n<<"  |CH|:"<<convexhull.size()<<endl;
}