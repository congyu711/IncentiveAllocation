#include<bits/stdc++.h>
using namespace std;

int main(int argc, char **argv)
{
    mt19937 gen(time(nullptr));
    if(argc<2)
    {
        cerr<<"./gen `NumberofLines`\n";
    }
    const int n = atoi(argv[1]);
    ofstream fout("data.in");
    fout<<"/* format: a b ==> y=ax-b */\n";
    fout<<n<<'\n';
    for(int i=0;i<n;i++)
    {
        fout<<gen()%20<<' '<<gen()%500<<'\n';
    }
}