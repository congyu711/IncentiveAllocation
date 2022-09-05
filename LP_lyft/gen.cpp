#include<bits/stdc++.h>
using namespace std;
const int size = 1e4;
int main(int argc, char **argv)
{
    mt19937 gen(time(nullptr));
    ofstream fout("data.in");
    long long n,m,C;
    if(argc==1)
    {
        cin>>n>>m>>C;
    }
    else
    {
        n=atoi(argv[1]);
        m=atoi(argv[2]);
        C=n*size*(0.2*gen()/gen.max());
    }
    fout<<n<<' '<<m<<' '<<C<<endl;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            fout<<1+gen()%size<<' ';
        }
        fout<<endl;
    }
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            fout<<1+gen()%size<<' ';
        }
        fout<<endl;
    }
    
}