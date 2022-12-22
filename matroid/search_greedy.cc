#include <bits/stdc++.h>
#include "laminar.cc"
#include <chrono>

using namespace std;
using namespace chrono;
int n; // number of items
int B; // budget

vector<int> v, c; // v&c.
vector<vector<int>> vecs;
mt19937 gen(mtseed);
laminar* l;

using ll = long long;
using fraction=pair<ll,ll>;
inline double f2d(fraction a){return 1.0*a.first/a.second;}
// find the closest integer fraction to r \in [0,1]
// the ranges of numerator and denominator are n and m, resp
// https://stackoverflow.com/questions/4385580/finding-the-closest-integer-fraction-to-a-given-random-real-between-0-1-given
fraction fareyseq(double r,int n,int m)
{
    fraction a(0,1),b(1,1),c;
    while(true)
    {
        c=make_pair(a.first+b.first,a.second+b.second);
        if(c.first>n||c.second>m)
        {
            if(r-f2d(a)<f2d(b)-r)   return a;
            else    return b;
        }
        if(f2d(c)<r)    a=c;
        else b=c;
    }
}
int main(int argc, char** argv)
{
    // n = 200000;
    // B = 155000000;
    if(argc!=4)
    {
        cerr<<"./xxx seed n B\n";
        return 1;
    }
    else
    {
        mtseed=atoi(argv[1]);
        n=atoi(argv[2]);
        B=atoi(argv[3]);
    }
    l = new laminar(n);
    // initvectors(n, n / 2);
    int ub_v=0,ub_c=0;
    for (int i = 0; i < n; i++)
    {
        v.push_back(gen() % 20000 + 10);
        c.push_back(gen() % 200000 + 10);
        ub_v=max(ub_v,v.back());
        ub_c=max(ub_c,c.back());
    }
    // cout<<"finish init\n";

    auto st=system_clock::now();

    vector<int> index;
    for(int i=0;i<n;i++) index.push_back(i);
    double opt=1e9,lambda=0;
    auto geta=[&](int i){return v[i]-lambda*c[i];};
    auto cmp=[&](int a,int b){if(fabs(geta(a)-geta(b))<1e-6) return c[a]>c[b];return geta(a)>geta(b);};
    auto cmpr=[&](int a,int b){if(fabs(geta(a)-geta(b))<1e-6) return c[a]<c[b];return geta(a)>geta(b);};

    double _l=0,_r=ub_v,sumc=0;
    int cnt=0;
    fraction pgl=make_pair(-1,-1);
    while (true)
    {
        double mid=(_l+_r)/2.0;
        fraction gl=fareyseq(mid-(int)mid,ub_v,ub_c);
        gl.first+=gl.second*(int)mid;
        lambda=f2d(gl);

        sort(index.begin(),index.end(),cmp);
        vector<int> fs;
        l->cntclear(l->root);
        for(int i=0;i<n;i++)
        {
            if(geta(index[i])<0)  break;
            fs.push_back(l->deltarank(index[i],l->root));
        }
        sumc=0;
        for(int i=0;i<fs.size();i++)    sumc+=fs[i]*c[index[i]];
        if(sumc>=B||pgl==gl)
        {
            l->cntclear(l->root);
            fs.clear();
            sort(index.begin(),index.end(),cmpr);
            opt=B*lambda;
            for(int i=0;i<n;i++)
            {
                auto tmp=geta(index[i]);
                if(tmp<1e-6)  break;
                fs.push_back(l->deltarank(index[i],l->root));
                opt+=fs[i]*tmp;
            }
            double sumc2=0;
            for(int i=0;i<fs.size();i++)    sumc2+=fs[i]*c[index[i]];
            if(sumc2<=B||pgl==gl)
            {
                cout<<opt<<", ";
                // cout<<lambda<<endl;
                // cout<<sumc<<' '<<sumc2<<endl;
                break;
            }
            else    _l=lambda;
        }
        else    _r=lambda;
        cnt++;
        pgl=gl;
    }
    // cout<<"iteration: "<<cnt<<endl;
    auto ed=system_clock::now();
    cout<<double(duration_cast<microseconds>(ed - st).count())*microseconds::period::num / microseconds::period::den<<endl;
}