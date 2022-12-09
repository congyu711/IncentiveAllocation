#include <bits/stdc++.h>
#include <eigen3/Eigen/Dense>
#include <gurobi_c++.h>
using namespace std;
using namespace Eigen;
int n;  // number of items
int B;  // budget

vector<int> v,c;    // v&c.
vector<vector<int>> vecs;
mt19937 gen(time(nullptr));

/////////////////////////////////////////////////////////
//////////////submodular function////////////////////////
void initvectors(int nrow,int ncol) // each row is a vector
{
    vecs.resize(nrow,vector<int>(ncol));
    for(auto &v:vecs)
    {
        for(auto &e:v)
        {
            e=gen()%32-16;
        }
    }
}
int getrank(const vector<int>& idx)
{
    MatrixXd m(vecs[0].size(),idx.size());
    for(int i=0;i<idx.size();i++)
    {
        for(int j=0;j<vecs[0].size();j++)
        {
            m(j,i)=vecs[idx[i]][j];
        }
    }
    FullPivLU<MatrixXd> lu_decomp(m);
    return (int)lu_decomp.rank();
}
//////////////submodular function end////////////////////
/////////////////////////////////////////////////////////

int main()
{
    n=20;B=155;
    initvectors(n,n/2);
    for(int i=0;i<n;i++)
    {
        v.push_back(gen()%20+10);
        c.push_back(gen()%20+10);
    }
    try
    {
        // Create an environment
        GRBEnv env = GRBEnv(true);
        // env.set("LogFile", "lp.log");
        env.start();
        // Create an empty model
        GRBModel model = GRBModel(env);
        vector<GRBVar> xs;
        for(int i=0;i<n;i++)
        {
            xs.push_back(model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS));
        }
        GRBLinExpr expr=0,obj=0;
        for(int i=1;i<(1<<n);i++)
        {
            vector<int> idx;
            GRBLinExpr submodular=0;
            bitset<200> bs(i);  // !!!
            for(int j=0;j<n;j++)
            {
                if(bs[j]==1)    {submodular+=xs[j];idx.push_back(j);}
            }
            model.addConstr(submodular<=getrank(idx));
        }
        // GRBLinExpr lin=0;
        // for(int i=0;i<n;i++)    lin+=xs[i];
        // model.addConstr(lin<=6);
        for(int i=0;i<n;i++)    {expr+=c[i]*xs[i];obj+=v[i]*xs[i];}
        model.addConstr(expr<=B);
        model.setObjective(obj,GRB_MAXIMIZE);
        model.optimize();
        cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
        double _B=0;
        vector<int> fracvar;
        for(int i=0;i<n;i++)
        {
            _B+=xs[i].get(GRB_DoubleAttr_X)*c[i];
            if(xs[i].get(GRB_DoubleAttr_X)!=0&&xs[i].get(GRB_DoubleAttr_X)!=1.0)
            {
                fracvar.push_back(i);
            }
        }
        if(fracvar.size()>2)
        {
            ofstream ferr("CE.out",ios::app);
            ferr<<n<<'\n';
            for(auto e:vecs)
            {
                for(auto ee:e)  ferr<<ee<<' ';
                ferr<<'\n';
            }
            for(auto e:v)   ferr<<e<<' ';ferr<<'\n';
            for(auto e:c)   ferr<<e<<' ';ferr<<'\n';
            ferr<<B<<endl;
            return 1;
        }
        cout<<"fracvar: "<<fracvar.size()<<'\n';
        for(auto e:fracvar)
        {
            cout<<'('<<e<<"): "<<xs[e].get(GRB_DoubleAttr_X)<<' ';
            cout<<v[e]<<", "<<c[e]<<'\n';
        }
        cout<<"B/_B: "<<B<<' '<<_B<<'\n';
    }
    catch (GRBException e)
    {
        cerr << e.getErrorCode() << '\n';
        cerr << e.getMessage() << '\n';
    }
}