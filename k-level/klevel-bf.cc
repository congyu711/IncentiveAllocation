#include "KPQ.cc"
using namespace std;
const int k=20;
#define __TEST_KLEVEL__
#ifdef __TEST_KLEVEL__
int main()
{
    ifstream fin("/home/congyu/IncentiveAllocation/k-level/data.in");
    fin.ignore(numeric_limits<streamsize>::max(),'\n');
    int n;
    fin>>n;
    vector<int> res;
    // int k=n*0.2;
    int a,b;
    for(int i=0;i<n;i++)
    {
        fin>>a>>b;
        lines.push_back(line(a,b));
    }
    vector<pair<double,int>> ans;
    
    for(auto e:ans)
    {
        cout<<e.first<<' '<<e.second<<endl;
    }
}
#endif