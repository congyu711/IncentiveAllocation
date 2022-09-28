// kinetic priority queue
// this is the divide-and-conquer version

// https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.44.9042&rep=rep1&type=pdf
// section 2.2

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
    return (double)(_l1.b-_l2.b)/(_l1.a-_l2.a);
}
class breakpoint
{
public:
    double x;
    line *l1,*l2;
    breakpoint(line &_l1,line &_l2)
    {
        x=getx(_l1,_l2);
        l1=&_l1;
        l2=&_l2;
    }
    bool operator<(const breakpoint &b)
    {
        return x<b.x;
    }
};
// store all lines
vector<line> lines;
template<class type=double,class cmp=less<type>>
class trivialKPQ
{
public:
    bool fin;   // finish--KPQ.advance() label
    unordered_set<int> S;   // store indices of lines
    type t,nextT;
    int top,nextTop;
    // int _top();
    // type _getnextT();
    virtual bool _advance();
    virtual void _insert(int);
    virtual void _delete(int);
    // maintain nextTop and nextT
    virtual bool _maintain();
    trivialKPQ(type _t):t(_t),nextT(-1*_t),top(-1),nextTop(-1),fin(false){}
    trivialKPQ():trivialKPQ(-9999){}
};
template<class type,class cmp>
bool trivialKPQ<type,cmp>::_maintain()
{
    fin=1;
    auto nt=nextT;
    auto ntp=nextTop;
    nextTop=*S.begin();
    nextT=getx(lines[top],lines[nextTop]);
    for(auto e:S)
    {
        auto x=getx(lines[top],lines[e]);
        if(x>t&&x<nextT)
        {
            fin=0;
            nextT=x;
            nextTop=e;
        }
    }
    if(fin)  {nextT=nt,nextTop=ntp;}
    return fin;
}
template<class type,class cmp>
bool trivialKPQ<type,cmp>::_advance()
{
    t=nextT;
    top=nextTop;
    // find nextT and nextTop
    // O(S.size)
    return _maintain();
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
    if(cmp()(lines[l].gety(t),lines[top].gety(t)))
        top=l;
    _maintain();
}
template<class type,class cmp>
void trivialKPQ<type,cmp>::_delete(int l)
{
    S.erase(l);
    // change top
    if(top==l)
    {
        auto y=lines[*S.begin()].gety(t);
        top=*S.begin();
        for(auto e:S)
        {
            if(cmp()(lines[e].gety(t),lines[top].gety(t)))
                top=e;
        }
    }
    _maintain();
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