#include<bits/stdc++.h>
using namespace std;

int main()
{
    mt19937 gen(time(nullptr));
    ofstream fout("data.in");
    int n,m,C;
    cout<<"n m c\n";
    cin>>n>>m>>C;
    fout<<n<<' '<<m<<' '<<C<<endl;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            fout<<gen()%100<<' ';
        }
        fout<<endl;
    }
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            fout<<gen()%100<<' ';
        }
        fout<<endl;
    }
    
}