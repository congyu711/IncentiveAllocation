#include <bits/stdc++.h>
#include "laminar.cc"

using namespace std;
int n; // number of items
int B; // budget

vector<int> v, c; // v&c.
vector<vector<int>> vecs;
mt19937 gen(19260817);
laminar* l;

using ll = long long;
using fraction=pair<ll,ll>;
inline double f2d(fraction a){return 1.0*a.first/a.second;}
// find the closest integer fraction to r \in [0,1]
// the ranges of numerator and denominator are n and m, resp
// https://stackoverflow.com/questions/4385580/finding-the-closest-integer-fraction-to-a-given-random-real-between-0-1-given
fraction fareyseq(double r,ll n,ll m)
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
fraction gl;
bool cmp(int a,int b)
{
    double _lambda=f2d(gl);
    if((v[a]-_lambda*c[a])==(v[b]-_lambda*c[b]))    return c[a]<c[b];
    return (v[a]-_lambda*c[a])>(v[b]-_lambda*c[b]);
}
int main()
{
    n = 200;
    B = 155000;
    l = new laminar(n);
    // initvectors(n, n / 2);
    int ub_v=0,ub_c=0;
    for (int i = 0; i < n; i++)
    {
        v.push_back(gen() % 20 + 10);
        c.push_back(gen() % 200000 + 10);
        ub_v=max(ub_v,v.back());
        ub_c=max(ub_c,c.back());
    }
    cout<<"finish init\n";


    vector<int> index;
    for(int i=0;i<n;i++) index.push_back(i);
    double opt=1e9,lambda=0;
    auto geta=[&](int i){return v[i]-lambda*c[i];};
    auto cmp=[&](int a,int b){if(geta(a)==geta(b)) return c[a]<c[b];return geta(a)>geta(b);};
    for(int k1=0;k1<n;k1++)
    {
        for(int k2=k1;k2<n;k2++)
        {
            if(k1==k2)  lambda=1.0*v[k1]/c[k1];
            else if(c[k1]==c[k2])    continue;
            else lambda=(v[k1]-v[k2])*1.0/(c[k1]-c[k2])*1.0;
            if(lambda<0)    continue;
            sort(index.begin(),index.end(),cmp);
            if(geta(index[0])<0)    continue;
            vector<int> fs(n,0),idx(n,0);
            idx[index[0]]=1;
            fs[0]=l->getrank(idx,l->root)+1;
            double topt=geta(index[0])*fs[0]+B*lambda;

            for(int i=1;i<n;i++)
            {
                if(geta(index[i])<0)    break;
                idx[index[i]]=1;
                fs[i]=1+i+l->getrank(idx,l->root);
                topt+=(fs[i]-fs[i-1])*geta(index[i]);
            }
            opt=min(opt,topt);
        }
    }
    cout<<opt<<endl;
    // double l=0,r=ub_v;
    // while (true)
    // {
    //     double mid=(l+r)/2.0;
    //     gl=fareyseq(mid-(int)mid,(int)ub_v,(int)ub_c);
    //     gl.first+=gl.second*(int)mid;
    //     // check guessed lambda gl
    //     vector<int> idx;
    //     for(int i=0;i<n;i++)    idx.push_back(i);
    //     sort(idx.begin(),idx.end(),cmp);

    // }
    
}