#include "KPQ.cc"
using namespace std;
#include <chrono>
// std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
// std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
// std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;

/// @brief this function computes (top) k-level of a set of lines
/// @param k k for k-level
/// @param lines ptr to the set of lines
/// @return a vector, each element is a double-int pair,
///         represents the x coordinate of the k-level's breakpoints
///         and which line is added to the k-level.
vector<pair<double, int>> klevel(int k, vector<line> *lines)
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    vector<pair<double, int>> res;
    kineticPriorityQueue<double, less<double>> upper(k, lines);
    kineticPriorityQueue<double, greater<double>> lower(lines->size()-k, lines);
    double t = -RANGE_MAX;
    // find k-level for the initial t and init upper&lower
    // can be done in O(n)
    // TODO: O(n) initialization
    vector<int> idxs(lines->size());
    for (int i = 0; i < idxs.size(); i++)
        idxs[i] = i;
    sort(idxs.begin(), idxs.end(), [&](int a, int b)
         { return (*lines)[a].gety(t) > (*lines)[b].gety(t); });
    for(int i=0;i<k;i++)
        upper._insert(idxs[i]);
    for(int i=k;i<idxs.size();i++)
        lower._insert(idxs[i]);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "init time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    auto pushback=[&]()->void{if(res.empty()||res.back().second!=upper.top) res.push_back(make_pair(upper.t,upper.top));};
    pushback();
    while (1)
    {
        double t0=getx((*lines)[upper.top],(*lines)[lower.top]);
        if(t0<t)    t0=RANGE_MAX;
        t=min({upper.nextT,lower.nextT,t0});
        if(t==RANGE_MAX)    break;
        if(t==t0)
        {
            int uppermin=upper.top, lowermax=lower.top;

            upper._insert(lowermax);
            while(upper.t<t0)   upper._advance();
            upper._delete(uppermin);
            while(upper.nextT<t0)   upper._advance();
            lower._insert(uppermin);
            while(lower.t<t0)   lower._advance();
            lower._delete(lowermax);
            while(lower.nextT<t0)   lower._advance();
            pushback();
            if(min(upper.nextT,lower.nextT)==RANGE_MAX) break;
            while(upper.top==lowermax&&lower.top==uppermin)
            {
                if(upper.nextT<lower.nextT) upper._advance();
                else lower._advance();
                if(min(upper.nextT,lower.nextT)==RANGE_MAX) return res;
            }
            if(upper.top!=lowermax) pushback();
        }
        else if(t==upper.nextT) {upper._advance();pushback();}
        else lower._advance();
    }
    return res;
}
#define __TEST_KLEVEL__
#ifdef __TEST_KLEVEL__
int main()
{
    ifstream fin("/home/congyu/IncentiveAllocation/k-level/data.in");
    ofstream fout("klevel.out");
    fin.ignore(numeric_limits<streamsize>::max(),'\n');
    int n;
    fin>>n;
    vector<int> res;
    // int k=n*0.2;
    double a,b;
    for(int i=0;i<n;i++)
    {
        fin>>a>>b;
        lines.push_back(line(a,b));
    }
    auto ans=klevel(20,&lines);
    for(auto e:ans)
    {
        fout<<e.second<<' '<<lines[e.second].a<<' '<<lines[e.second].b<<'\n';
    }
}
#endif