// kinetic priority queue
// this is the divide-and-conquer version

// https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.44.9042&rep=rep1&type=pdf
// section 2.2
#include"trivialKPQ.cc"
#include<boost/heap/binomial_heap.hpp>
using namespace std;
template<class type,class cmp, class binomialHeap=boost::heap::binomial_heap<pair<type,int>,boost::heap::compare<std::greater<pair<type,int>>>>>
class kineticPriorityQueue: public trivialKPQ<type,cmp>
{
public:
    binomialHeap pq;
    vector<typename binomialHeap::handle_type> pq_handlers;
    trivialKPQ<type,cmp> Q;
    vector<trivialKPQ<type,cmp>*> subKPQs;
    unordered_map<int,int> mp;  // l is in subkpq No.mp[l]
    int r;  // r=ceil(log n)
    void _maintain();
    void _advance();
    void _insert(int);
    void _delete(int);
    kineticPriorityQueue(int sz, vector<line>* p){
        this->lines=p;
        r=ceil(log((double)sz));
        subKPQs.resize(r,nullptr);
        for(int i=0;i<r;i++)    pq_handlers.push_back(pq.push(make_pair(RANGE_MAX,i)));
        Q=trivialKPQ<type,cmp>(p);
        if(sz<50)
        {
            for(int i=0;i<r;i++)
            {
                subKPQs[i]=new trivialKPQ<type,cmp>(p);
            }
        }
        else
        {
            for(int i=0;i<r;i++)
            {
                subKPQs[i]=new kineticPriorityQueue<type,cmp,binomialHeap>(ceil(1.0*sz/r),p);
            }
        }
    }
};

template<class type,class cmp, class binomialHeap>
void kineticPriorityQueue<type,cmp,binomialHeap>::_maintain()
{
    this->top=Q.top;
    type nxt=pq.top().first;
    // if(min(Q.nextT,nxt)==this->nextT)   this->nextT=RANGE_MAX,this->nextTop=-1;
    this->nextT=min(Q.nextT,nxt);
}
template<class type,class cmp, class binomialHeap>
void kineticPriorityQueue<type,cmp,binomialHeap>::_advance()
{
    this->t=this->nextT;
    type nxt=pq.top().first;
    int p=pq.top().second;
    if(this->t==subKPQs[p]->nextT)
    {
        auto pmin=subKPQs[p]->top;
        subKPQs[p]->_advance();
        pq.update(pq_handlers[p],make_pair(subKPQs[p]->nextT,p));
        auto nextpmin=subKPQs[p]->top;
        if(pmin!=nextpmin)
        {
            Q.t=this->t;
            Q._maintain();
            Q._delete(pmin);
            Q._insert(nextpmin);
        }
    }
    else if(this->t==Q.nextT)
        Q._advance();
    _maintain();
}
template<class type,class cmp, class binomialHeap>
void kineticPriorityQueue<type,cmp,binomialHeap>::_insert(int l)
{
    this->S.insert(l);
    // find smallest |P.set|, this can be done faster
    // but O(r) still doesn't affect total complexity
    int p=-1,maxpsize=0x07fffffff;
    for(int i=0;i<r;i++)
    {
        if(maxpsize>subKPQs[i]->S.size())
        {
            maxpsize=subKPQs[i]->S.size();
            p=i;
        }
    }
    mp.insert(make_pair(l,p));
    if(subKPQs[p]->S.size())
        Q._delete(subKPQs[p]->top);
    subKPQs[p]->_insert(l);
    pq.update(pq_handlers[p],make_pair(subKPQs[p]->nextT,p));
    Q._insert(subKPQs[p]->top);
    _maintain();
}
template<class type,class cmp, class binomialHeap>
void kineticPriorityQueue<type,cmp,binomialHeap>::_delete(int l)
{
    this->S.erase(l);
    auto P=subKPQs[mp[l]];
    Q._delete(P->top);
    P->_delete(l);
    pq.update(pq_handlers[mp[l]],make_pair(P->nextT,mp[l]));
    mp.erase(l);
    Q._insert(P->top);
    _maintain();
}
#define __TEST__
#ifdef __TEST__
int main()
{
    ifstream fin("/home/congyu/IncentiveAllocation/k-level/data.in");
    fin.ignore(numeric_limits<streamsize>::max(),'\n');
    int n;
    fin>>n;
    vector<int> res;
    // int k=n*0.2;
    int a,b;
    kineticPriorityQueue<double,less<double>> kpq(n,&lines);
    for(int i=0;i<n;i++)
    {
        fin>>a>>b;
        lines.push_back(line(a,b));
        kpq._insert(i);
    }
    res.push_back(kpq.top);
    auto pre_t=-1;
    while (kpq.t!=RANGE_MAX)
    {
        pre_t=kpq.t;
        kpq._advance();
        // if(kpq.t>=RANGE_MAX)  break;
        res.push_back(kpq.top);
    }
    auto it=unique(res.begin(),res.end());
    res.erase(it,res.end());
    for(auto e:res) cout<<lines[e].a<<' '<<lines[e].b<<'\n';
}
#endif