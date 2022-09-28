#include"trivialKPQ.cc"
#include<boost/heap/priority_queue.hpp>
using namespace std;

template<class type,class cmp>
class kineticPriorityQueue: public trivialKPQ<type,cmp>
{
public:
    // priority_queue<pair<type,int>> pq;  // this is not completely correct.
    // boost::heap::priority_queue<pair<type,int>,boost::heap::compare<less<pair<type,int> >>> pq;
    trivialKPQ<type,cmp> Q;
    vector<trivialKPQ<type,cmp>*> subKPQs;
    unordered_map<int,int> mp;  // l is in subkpq No.mp[l]
    int r;  // r=ceil(log n)
    bool _maintain();
    bool _advance();
    void _insert(int);
    void _delete(int);
    kineticPriorityQueue(int sz){
        r=ceil(log((double)sz));
        subKPQs.resize(r+1,nullptr);
        trivialKPQ<type,cmp>(-9999);
        if(sz<50)
        {
            for(int i=0;i<r;i++)
            {
                subKPQs[i]=new trivialKPQ<type,cmp>;
            }
        }
        else
        {
            for(int i=0;i<r;i++)
            {
                subKPQs[i]=new kineticPriorityQueue<type,cmp>(ceil(1.0*sz/r));
            }
        }
    }
};

template<class type,class cmp>
bool kineticPriorityQueue<type,cmp>::_maintain()
{
    this->top=Q.top;
    type nxt=1e9;
    int minp=-1;
    for(int i=0;i<r;i++)
    {
        if(subKPQs[i]!=nullptr)
        {
            if(nxt>subKPQs[i]->nextT)
            {
                nxt=subKPQs[i]->nextT;
                minp=i;
            }
        }
    }
    this->nextT=min(Q.nextT,nxt);
    // this->nextT=min(Q.nextT,pq.top().first);
    return true;
}
template<class type,class cmp>
bool kineticPriorityQueue<type,cmp>::_advance()
{
    this->t=this->nextT;
    // if(pq.empty())  return false;
    // auto p=pq.top().second;
    type nxt=1e9;
    int p=-1;
    for(int i=0;i<r;i++)
    {
        if(subKPQs[i]!=nullptr&&subKPQs[i]->fin==false)
        {
            if(nxt>subKPQs[i]->nextT)
            {
                nxt=subKPQs[i]->nextT;
                p=i;
            }
        }
    }
    if(p==-1)
    {
        cerr<<"/have no subkpqs or all subkpqs finished\n";
        this->fin=true;
        return false;
    }
    if(subKPQs[p]!=nullptr&&this->t==subKPQs[p]->nextT)
    {
        auto pmin=subKPQs[p]->top;
        subKPQs[p]->_advance();
        auto nextpmin=subKPQs[p]->top;
        if(pmin!=nextpmin)
        {
            Q._delete(pmin);
            Q._insert(nextpmin);
        }
    }
    else if(this->t==Q.nextT)
        Q._advance();
    return _maintain();
}
template<class type,class cmp>
void kineticPriorityQueue<type,cmp>::_insert(int l)
{
    this->S.insert(l);
    // find smallest |P.set|, this can be done faster
    // but O(r) still doesn't affect total complexity
    int p=-1,maxpsize=0x07fffffff;
    for(int i=0;i<r;i++)
    {
        if(subKPQs[i]==nullptr)
        {
            p=i,maxpsize=0;
            break;
        }
        if(maxpsize>subKPQs[i]->S.size())
        {
            maxpsize=subKPQs[i]->S.size();
            p=i;
        }
    }
    mp.insert(make_pair(l,p));
    Q._delete(subKPQs[p]->top);
    subKPQs[p]->_insert(l);
    Q._insert(subKPQs[p]->top);
    _maintain();
}
template<class type,class cmp>
void kineticPriorityQueue<type,cmp>::_delete(int l)
{
    this->S.erase(l);
    auto P=subKPQs[mp[l]];
    Q._delete(P->top);
    P->_delete(l);
    Q._insert(P->top);
    _maintain();
}
int main()
{
    ifstream fin("data.in");
    fin.ignore(numeric_limits<streamsize>::max(),'\n');
    int n;
    fin>>n;
    // int k=n*0.2;
    int a,b;
    kineticPriorityQueue<double,less<double>> kpq(20);
    for(int i=0;i<n;i++)
    {
        fin>>a>>b;
        lines.push_back(line(a,b));
        kpq._insert(i);
    }
    cout<<kpq.t<<' '<<kpq.nextT<<' '<<kpq.top<<endl;
    while (kpq.fin==false)
    {
        kpq._advance();
        cout<<kpq.t<<' '<<kpq.nextT<<' '<<kpq.top<<endl;
    }
    
}