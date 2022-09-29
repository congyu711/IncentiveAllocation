// kinetic priority queue
// this is the divide-and-conquer version

// https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.44.9042&rep=rep1&type=pdf
// section 2.2

#include"trivialKPQ.cc"
#include<boost/heap/binomial_heap.hpp>
using namespace std;
// I need to overload operator+ and operator- for std::pair<T,int>
// template<class type>
// inline const pair<type,int> operator+(const pair<type,int> &p1,const pair<type,int> &p2)
// {
//     pair<type,int> res;
//     res.first=p1.first+p2.first;
//     res.second=p1.second+p2.second;
//     return res;
// }
// template<class type>
// inline const pair<type,int> operator-(const pair<type,int> &p1,const pair<type,int> &p2)
// {
//     pair<type,int> res;
//     res.first=p1.first-p2.first;
//     res.second=p1.second-p2.second;
//     return res;
// }
// actually... I don't need this currently.
template<class type,class cmp, class binomialHeap=boost::heap::binomial_heap<pair<type,int>,boost::heap::compare<std::greater<pair<type,int>>>>>
class kineticPriorityQueue: public trivialKPQ<type,cmp>
{
public:
    // priority_queue<pair<type,int>> pq;  // this is not completely correct.
    // typedef typename boost::heap::binomial_heap<pair<type,int>,boost::heap::compare<std::greater<pair<type,int>>>> binomialHeap;
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
    kineticPriorityQueue(int sz):trivialKPQ<type,cmp>(){
        r=ceil(log((double)sz));
        subKPQs.resize(r,nullptr);
        for(int i=0;i<r;i++)    pq_handlers.push_back(pq.push(make_pair(RANGE_MAX,i)));
        Q=trivialKPQ<type,cmp>(-RANGE_MAX);
        if(sz<50)
        {
            for(int i=0;i<r;i++)
            {
                subKPQs[i]=new trivialKPQ<type,cmp>();
            }
        }
        else
        {
            for(int i=0;i<r;i++)
            {
                subKPQs[i]=new kineticPriorityQueue<type,cmp,binomialHeap>(ceil(1.0*sz/r));
            }
        }
    }
};

template<class type,class cmp, class binomialHeap>
void kineticPriorityQueue<type,cmp,binomialHeap>::_maintain()
{
    this->top=Q.top;
    // type nxt=1e9;
    // int minp=-1;
    // for(int i=0;i<r;i++)
    // {
    //     if(subKPQs[i]!=nullptr)
    //     {
    //         if(nxt>subKPQs[i]->nextT)
    //         {
    //             nxt=subKPQs[i]->nextT;
    //             minp=i;
    //         }
    //     }
    // }
    type nxt=pq.top().first;
    this->nextT=min(Q.nextT,nxt);
    // this->nextT=min(Q.nextT,pq.top().first);
}
template<class type,class cmp, class binomialHeap>
void kineticPriorityQueue<type,cmp,binomialHeap>::_advance()
{
    this->t=this->nextT;
    if(this->t>=RANGE_MAX)
    {
        this->fin=true;
        return;
    }
    // type nxt=1e9;
    // int p=-1;
    // for(int i=0;i<r;i++)
    // {
    //     if(subKPQs[i]!=nullptr&&subKPQs[i]->S.size())
    //     {
    //         if(nxt>subKPQs[i]->nextT)
    //         {
    //             nxt=subKPQs[i]->nextT;
    //             p=i;
    //         }
    //     }
    // }
    type nxt=pq.top().first;
    int p=pq.top().second;
    if(p==-1)
        cerr<<"error!???\n";
    if(subKPQs[p]!=nullptr&&this->t==subKPQs[p]->nextT)
    {
        auto pmin=subKPQs[p]->top;
        subKPQs[p]->_advance();
        pq.update(pq_handlers[p],make_pair(subKPQs[p]->nextT,p));
        auto nextpmin=subKPQs[p]->top;
        if(pmin!=nextpmin)
        {
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
int main()
{
    ifstream fin("/home/congyu/mip/k-level/data.in");
    fin.ignore(numeric_limits<streamsize>::max(),'\n');
    int n;
    fin>>n;
    vector<int> res;
    // int k=n*0.2;
    int a,b;
    kineticPriorityQueue<double,less<double>> kpq(n);
    for(int i=0;i<n;i++)
    {
        fin>>a>>b;
        lines.push_back(line(a,b));
        kpq._insert(i);
    }
    res.push_back(kpq.top);

    while (kpq.fin==false)
    {
        kpq._advance();
        // if(kpq.t>=RANGE_MAX)  break;
        res.push_back(kpq.top);
    }
    auto it=unique(res.begin(),res.end());
    res.erase(it,res.end());
    for(auto e:res) cout<<e<<' ';cout<<endl;
}