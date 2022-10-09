// kinetic priority queue
// trivial version

// https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.44.9042&rep=rep1&type=pdf
// section 2.2

#define RANGE_MAX 100000000
#include<bits/stdc++.h>
using namespace std;

typedef pair<int,int> point;
class line
{
    // y=ax-b
public:
    int a,b;
    line(int _a, int _b):a(_a),b(_b){}
    double gety(double x)
    {
        return x*a-b;
    }
};
inline double getx(const line &_l1, const line &_l2)
{
    if(fabs(_l1.a-_l2.a)<1e-5)  return RANGE_MAX;
    return (double)(_l1.b-_l2.b)/(_l1.a-_l2.a);
}

/////////////////////////////////////////////////
// store all lines. KPQ use indices of lines 
// based on this vector.
vector<line> lines;
/////////////////////////////////////////////////

template<class type,class cmp>
class trivialKPQ
{
public:
    vector<line> *lines;
    bool fin;   // finish--KPQ.advance() label
    unordered_set<int> S;   // store indices of lines
    type t,nextT;
    int top,nextTop;
    // int _top();
    // type _getnextT();
    virtual void _advance();
    virtual void _insert(int);
    virtual void _delete(int);
    // maintain nextTop and nextT
    virtual void _maintain();
    bool compare(const int,const int);
    trivialKPQ(type _t):t(_t),nextT(-1*_t),top(-1),nextTop(-1),fin(false){}
    trivialKPQ(vector<line>* p):trivialKPQ(-RANGE_MAX){lines=p;}
    trivialKPQ():trivialKPQ(-RANGE_MAX){lines=nullptr;}
};
template<class type,class cmp>
void trivialKPQ<type,cmp>::_maintain()
{
    fin=1;
    int ntp=-1;
    type nt=RANGE_MAX;
    for(auto e:S)
    {
        if(e==top)  continue;
        auto x=getx((*lines)[top],(*lines)[e]);
        if(x>t&&x<RANGE_MAX)
        {
            fin=0;
            if(nt>x)    nt=x,ntp=e;
        }
    }
    if(nt>t&&nt<RANGE_MAX)  nextT=nt,nextTop=ntp;
    // if(fin==1) then there is no breakpoint right to t;
    if(fin) nextT=RANGE_MAX,nextTop=-1;
}
template<class type,class cmp>
void trivialKPQ<type,cmp>::_advance()
{
    t=nextT;
    top=nextTop;
    // find nextT and nextTop
    // O(S.size)
    _maintain();
}
template<class type,class cmp>
bool trivialKPQ<type,cmp>::compare(const int a,const int b)
{
    if((*lines)[a].gety(t)==(*lines)[b].gety(t))
        return cmp()((*lines)[a].a,(*lines)[b].a);
    return cmp()((*lines)[a].gety(t),(*lines)[b].gety(t));
}
template<class type,class cmp>
void trivialKPQ<type,cmp>::_insert(int l)
{
    fin=0;
    S.insert(l);
    // change top
    if(S.size()<2)
    {
        top=l;
        return;
    }
    if(compare(l,top))
        top=l;
    _maintain();
}
template<class type,class cmp>
void trivialKPQ<type,cmp>::_delete(int l)
{
    fin=0;
    S.erase(l);
    nextT=RANGE_MAX;
    // change top
    if(top==l)
    {
        top=*S.begin();
        for(auto e:S)
        {
            if(cmp()((*lines)[e].gety(t),(*lines)[top].gety(t)))
                top=e;
        }
    }
    if(S.size())    _maintain();
}
// int main()
// {
//     ifstream fin("data.in");
//     fin.ignore(numeric_limits<streamsize>::max(),'\n');
//     int n;
//     fin>>n;
//     int k=n*0.2;
//     int a,b;
//     trivialKPQ<double,greater<double>> kpq(-20);
//     for(int i=0;i<n;i++)
//     {
//         fin>>a>>b;
//         lines.push_back(line(a,b));
//         kpq._insert(i);
//     }
//     while (kpq.fin==false)
//     {
//         kpq._advance();
//         cout<<kpq.t<<' '<<kpq.nextT<<' '<<kpq.top<<endl;
//     }
    
// }