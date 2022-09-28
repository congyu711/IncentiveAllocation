#include<bits/stdc++.h>
using namespace std;
template<typename T>
class b
{
public:
    vector<int> c;
    virtual void foo();
    
};
template<typename T>
void b<T>::foo()
{
    cout<<"old\n";
}


template<typename T>
class a : public b<T>
{
public:
    void foo();
};
template<typename T>
void a<T>::foo()
{
    b<T>::c.push_back(1);
    this->c.push_back(1);
    cout<<"yes\n";
}

int main()
{
    b<int>* p=new(a<int>);
    p->foo();
    cout<<p->c.size()<<endl;
    for(int i=1;i<=100;i++)
    {
        cout<<i<<' '<<ceil(log((double)i))<<endl;
    }
}