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
int main()
{
    n = 20000;
    B = 15500000;
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
    auto cmp=[&](int a,int b){if(geta(a)==geta(b)) return c[a]>c[b];return geta(a)>geta(b);};
    auto cmpr=[&](int a,int b){if(geta(a)==geta(b)) return c[a]<c[b];return geta(a)>geta(b);};

    double _l=0,_r=ub_v,sumc=0;
    int cnt=0;
    while (true)
    {
        double mid=(_l+_r)/2.0;
        fraction gl=fareyseq(mid-(int)mid,ub_v,ub_c);
        gl.first+=gl.second*(int)mid;
        lambda=f2d(gl);

        sort(index.begin(),index.end(),cmp);
        vector<int> fs,idx(n,0);
        for(int i=0;i<n;i++)
        {
            if(geta(index[i])<0)  break;
            idx[index[i]]=1;
            // fs[i]=1+i+l->getrank(idx,l->root);
            fs.push_back(1+i+l->getrank(idx,l->root));
        }
        sumc=(geta(index[0])>0?fs[0]*c[index[0]]:0);
        for(int i=1;i<fs.size();i++) 
            sumc+=(fs[i]-fs[i-1])*c[index[i]];
        if(sumc>=B)
        {
            fs.clear();idx.clear();
            idx.resize(n);
            sort(index.begin(),index.end(),cmpr);
            opt=B*lambda;
            for(int i=0;i<n;i++)
            {
                auto tmp=geta(index[i]);
                if(tmp<=0)  break;
                idx[index[i]]=1;
                fs.push_back(1+i+l->getrank(idx,l->root));
                // fs[i]=1+i+l->getrank(idx,l->root);
                if(i>0) opt+=(fs[i]-fs[i-1])*tmp;
                else opt+=fs[0]*geta(index[0]);
            }
            double sumc2=(geta(index[0])>0?fs[0]*c[index[0]]:0);
            for(int i=1;i<fs.size();i++)    sumc2+=(fs[i]-fs[i-1])*c[index[i]];
            if(sumc2<=B)
            {
                cout<<opt<<endl;
                cout<<lambda<<endl;
                cout<<sumc<<' '<<sumc2<<endl;
                break;
            }
            else    _l=lambda;
        }
        else    _r=lambda;

        // // binary search part
        // lambda=mid;
        // sort(index.begin(),index.end(),cmp);
        // // vector<int> fs(n,0),idx(n,0);
        // fs.clear();idx.clear();
        // fs.resize(n,0);idx.resize(n,0);
        // for(int i=0;i<n;i++)
        // {
        //     if(tmp<0)  break;
        //     idx[index[i]]=1;
        //     fs[i]=1+i+l->getrank(idx,l->root);
        // }
        // sumc=(geta(index[0])>0?fs[0]*c[index[0]]:0);
        // for(int i=1;i<n;i++)    sumc+=max(0,fs[i]-fs[i-1])*c[index[i]];
        // if(sumc<B)  _r=mid;
        // else _l=mid;
        cnt++;
    }
    cout<<"iteration: "<<cnt<<endl;
}