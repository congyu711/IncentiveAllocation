// kinetic priority queue
// this is the divide-and-conquer version

// https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.44.9042&rep=rep1&type=pdf
// section 2.2

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
    void _maintain();
    void _advance();
    void _insert(int);
    void _delete(int);
    kineticPriorityQueue(int sz):trivialKPQ<type,cmp>(){
        r=ceil(log((double)sz));
        subKPQs.resize(r,nullptr);
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
                subKPQs[i]=new kineticPriorityQueue<type,cmp>(ceil(1.0*sz/r));
            }
        }
    }
};

template<class type,class cmp>
void kineticPriorityQueue<type,cmp>::_maintain()
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
}
template<class type,class cmp>
void kineticPriorityQueue<type,cmp>::_advance()
{
    this->t=this->nextT;
    if(this->t>=RANGE_MAX)
    {
        this->fin=true;
        return;
    }
    // if(pq.empty())  return false;
    // auto p=pq.top().second;
    type nxt=1e9;
    int p=-1;
    for(int i=0;i<r;i++)
    {
        if(subKPQs[i]!=nullptr&&subKPQs[i]->S.size())
        {
            if(nxt>subKPQs[i]->nextT)
            {
                nxt=subKPQs[i]->nextT;
                p=i;
            }
        }
    }
    if(p==-1)
        cerr<<"error!???\n";
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
    _maintain();
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
    if(subKPQs[p]->S.size())
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