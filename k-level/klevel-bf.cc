#include "KPQ.cc"
using namespace std;
const int k=20;
const double eps=1e-6;
#define __TEST_KLEVEL__
#ifdef __TEST_KLEVEL__
int main()
{
    ifstream fin("/home/congyu/IncentiveAllocation/k-level/data.in");
    ofstream fout("klevelbf.out");
    fin.ignore(numeric_limits<streamsize>::max(),'\n');
    int n;
    fin>>n;
    vector<int> res;
    // int k=n*0.2;
    double a,b;
    for(int i=0;i<n;i++)
    {
        fin>>a>>b;
        lines.push_back(line(a,b));
    }
    double t=-1e10;
    vector<int> idxs(n);
    for(int i=0;i<n;i++)    idxs[i]=i;
    sort(idxs.begin(),idxs.end(),[&](int a,int b){return lines[a].gety(t+eps)>lines[b].gety(t+eps);});
    fout<<idxs[k-1]<<' '<<lines[idxs[k-1]].a<<' '<<lines[idxs[k-1]].b<<endl;
    int top=idxs[k-1];
    while (1)
    {
        double tt=1e10; int nxt=-1;
        for(int i=0;i<n;i++)
        {
            if(i==top)   continue;
            double x=getx(lines[i],lines[top]);
            if(x>t&&x<tt)
            {
                nxt=i;
                tt=x;
            }
        }
        if(nxt==-1) break;
        fout<<nxt<<' '<<lines[nxt].a<<' '<<lines[nxt].b<<'\n';
        top=nxt;
        t=tt;
    }
}
#endif