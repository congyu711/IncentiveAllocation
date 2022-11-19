// kinetic priority queue
// trivial version

// https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.44.9042&rep=rep1&type=pdf
// section 2.2

#define RANGE_MAX 1e10
#include<bits/stdc++.h>
using namespace std;

typedef pair<double,double> point;
class line
{
    // y=ax-b
public:
    double a,b;
    line(double _a, double _b):a(_a),b(_b){}
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
    int ntp=-1;
    type nt=RANGE_MAX;
    for(auto e:S)
    {
        if(e==top)  continue;
        auto x=getx((*lines)[top],(*lines)[e]);
        if(x>t&&x<nt)
        {
            nt=x,ntp=e;
        }
    }
    if(ntp!=-1)    nextT=nt,nextTop=ntp;
    else nextT=RANGE_MAX,nextTop=top;
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
    if((*lines)[a].gety(t+1e-8)==(*lines)[b].gety(t+1e-8))
        return cmp()((*lines)[a].a,(*lines)[b].a);
    return cmp()((*lines)[a].gety(t+1e-8),(*lines)[b].gety(t+1e-8));
}
template<class type,class cmp>
void trivialKPQ<type,cmp>::_insert(int l)
{
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
    S.erase(l);
    nextT=RANGE_MAX;
    // change top
    if(top==l)
    {
        if(S.empty())
        {
            return;
        }
        top=*S.begin();
        for(auto e:S)
        {
            if(cmp()((*lines)[e].gety(t+1e-8),(*lines)[top].gety(t+1e-8)))
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