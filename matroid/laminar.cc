// generate laminar matroid.
#include<bits/stdc++.h>

using namespace std;
int mtseed;
mt19937 gen;
struct node
{
    int l,r;    // constains [l,r)
    vector<node*>   ch;
    vector<int> p;  // range of each child;
    vector<int> psum;
    int cap;
    int cnt;    // a counter for rank computing.
    node(int _l,int _r): l(_l),r(_r){}
    node(int _l,int _r,int _c): l(_l),r(_r),cap(_c){}
};

// https://stackoverflow.com/questions/2161406/how-do-i-generate-a-uniform-random-integer-partition
// too slow. expected running time O(n^1.75)
// change `sum==n` to `sum is close to n`
vector<int> __partition(int n)
{
    const double pi =  acos(-1.0);
    double x=exp(-pi/sqrt(6*n));
    while (true)
    {
        vector<int> res;
        int sum=0;
        for(int i=1;i<=n;i++)
        {
            geometric_distribution<int> gd(1-pow(x,i));
            auto c=gd(gen);
            res.push_back(c);
            sum+=c*i;
        }
        if(sum<=n&&(n-sum)<0.25*n)
        {
            vector<int> ans;
            for(int i=0;i<res.size();i++)
                while(res[i]--) ans.push_back(i+1);
            if(n!=sum)  ans.push_back(n-sum);
            shuffle(ans.begin(),ans.end(),gen);
            return ans;
        }
    }
    
}

class laminar
{
public:
    int n;  // size of the groundset. elements are 0,..,n-1;
    node *root;
    node *build(int l,int r)
    {
        auto h=new node(l,r);
        if(r<l+1)  return nullptr;
        // partition. p0,p1,...,pn
        h->p=__partition(r-l);
        // not going to use the last part;
        int _l=l;
        h->psum.resize(h->p.size()-1);
        for(int i=0;i<h->p.size()-1;i++)
        {
            if(i==0)    h->psum[i]=h->p[i];
            else h->psum[i]=h->psum[i-1]+h->p[i];
            h->ch.push_back(build(l,l+h->p[i]));
            l+=h->p[i];
            // if(h->ch.back()==nullptr)   h->ch.pop_back();
        }
        return h;
    }
    laminar(int _n)
    {
        n=_n;
        root=build(0,n);
        cap(root);
    }
    int cap(node *h)
    {
        int ub=0;
        if(h->ch.size()!=0) ub+=h->p.back();
        else ub=1;
        // cout<<"----------\n";
        // cout<<"l&r "<<h->l<<' '<<h->r-1<<endl;
        // cout<<"ub: "<<ub<<endl;
        // for(int i=0;i<h->p.size()-1;i++)    cout<<h->p[i]<<' ';cout<<"\n---------\n";
        for(int i=0;i<h->ch.size();i++) ub+=cap(h->ch[i]);
        h->cap=ub-(rand()%10==0?1:0);
        return h->cap;
    }
    // return the violated node in the tree of laminar family.
    node* isIndependent(const vector<double> &xs, node* h)
    {
        double sumx=0;
        for(int i=h->l;i<h->r;i++)
        {
            sumx+=xs[i];
        }
        if(sumx>h->cap) return h;
        for(int i=0;i<h->ch.size();i++)
        {
            auto res=isIndependent(xs,h->ch[i]);
            if(res!=nullptr)    return res;
        }
        return nullptr;
    }

    // get the rank of I={i|xs[i]!=0}
    // rank = |I|+getrank(...)
    int getrank(const vector<int> &xs, node* h)
    {
        int del=0,sumx=0;
        for(auto e:h->ch)
        {
            del+=getrank(xs,e);
        }
        for(int i=h->l;i<h->r;i++)
        {
            if(xs[i]>1e-6)  sumx++;
        }
        return del+min(0,h->cap-sumx-del);
    }
    void cntclear(node *h)
    {
        h->cnt=0;
        for(auto e:h->ch)   cntclear(e);
    }
    // returns the rank change after adding a new item idx
    int deltarank(int idx, node*h)
    {
        if(idx==41)
        {
            int c=0;
            c++;
        }
        if(h->cnt==h->cap)   return 0;
        auto c=upper_bound(h->psum.begin(),h->psum.end(),idx-h->l)-h->psum.begin();
        if(c==h->psum.size())   {h->cnt++;return 1;}
        int res=deltarank(idx,h->ch[c]);
        if(res==0) return 0;
        h->cnt++;
        return 1;
    }
};
