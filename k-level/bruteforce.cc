#include <bits/stdc++.h>
using namespace std;

class line
{
    // y=ax-b
public:
    int a,b;
    line(int _a, int _b):a(_a),b(_b){}
};
class breakpoint
{
public:
    double x;
    line *l1,*l2;
    breakpoint(line &_l1,line &_l2)
    {
        x=(double)(_l1.b-_l2.b)/(_l1.a-_l2.a);
        l1=&_l1;
        l2=&_l2;
    }
    bool operator<(const breakpoint &b)
    {
        return x<b.x;
    }
};
int main()
{
    ifstream fin("data.in");
    fin.ignore(numeric_limits<streamsize>::max(),'\n');
    int n;
    fin>>n;
    int k=1;
    vector<line> lines;
    int a,b;
    for(int i=0;i<n;i++)
    {
        fin>>a>>b;
        lines.push_back(line(a,b));
    }
    vector<breakpoint> points;
    for(int i=0;i<n;i++)
    {
        for(int j=i+1;j<n;j++)
        {
            points.push_back(breakpoint(lines[i],lines[j]));
        }
    }
    vector<vector<line>> res(points.size()-1);
    sort(points.begin(),points.end());
    for(int i=0;i<points.size()-1;i++)
    {
        double x=(points[i].x+points[i+1].x)/2.0;
        sort(lines.begin(),lines.end(),[&](line &l1,line &l2)->bool{return l1.a*x-l1.b>l2.a*x-l2.b;});
        for(int j=0;j<k;j++)
        {
            res[i].push_back(lines[j]);
        }
    }
}