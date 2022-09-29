#include<bits/stdc++.h>
#include<boost/heap/binomial_heap.hpp>
using namespace std;
template<class T>
class biheap=boost::heap::binomial_heap<pair<T,int>,boost::heap::compare<std::greater<pair<T,int>>>> ;

int main()
{
    vector<boost::heap::binomial_heap<pair<double,int>,boost::heap::compare<std::greater<pair<double,int>>>>::handle_type> vec(10);
    for(int i=0;i<10;i++)
    {
        vec[i]=h.push(make_pair(i+1.23,i));
    }
    // auto p=h.push(make_pair(91.99,123));
    cout<<h.top().first<<endl;
    h.update(vec[9],make_pair(80.01,111));
    cout<<h.top().first<<endl;
    // h.decrease(vec[9],make_pair(4.0,0));
}