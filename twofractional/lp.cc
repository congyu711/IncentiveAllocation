#include <bits/stdc++.h>
#include <eigen3/Eigen/Dense>
#include <gurobi_c++.h>
using namespace std;
using namespace Eigen;
int n; // number of items
int B; // budget

vector<int> v, c; // v&c.
vector<vector<int>> vecs1,vecs2;
mt19937 gen;

/////////////////////////////////////////////////////////
void initvectors(int nrow, int ncol, vector<vector<int>>& vecs) // each row is a vector
{
    vecs.resize(nrow, vector<int>(ncol));
    for (auto &v : vecs)
    {
        for (auto &e : v)
        {
            e = gen() % 32 - 16;
        }
    }
}
int getrank(const vector<int> &idx, vector<vector<int>>& vecs)
{
    MatrixXd m(vecs[0].size(), idx.size());
    for (int i = 0; i < idx.size(); i++)
    {
        for (int j = 0; j < vecs[0].size(); j++)
        {
            m(j, i) = vecs[idx[i]][j];
        }
    }
    FullPivLU<MatrixXd> lu_decomp(m);
    return (int)lu_decomp.rank();
}
/////////////////////////////////////////////////////////
class matroidcallback : public GRBCallback
{
public:
    vector<GRBVar> *xs;
    matroidcallback(vector<GRBVar>* _xs):xs(_xs){}
protected:
    void callback()
    {
        try
        {
            if(where==GRB_CB_MIPSOL)
            {
                vector<int> idx;
                double sumx=0;
                GRBLinExpr expr=0;
                vector<double> xval(xs->size());
                for(int i=0;i<xval.size();i++)
                {
                    xval[i]=getSolution(xs->at(i));
                }
                for(int i=0;i<xs->size();i++)
                {
                    auto c=xval[i];
                    if(c>0)
                    {
                        idx.push_back(i);
                        sumx+=c;
                        expr+=xs->at(i);
                    }
                }
                auto r1=getrank(idx,vecs1);
                auto r2=getrank(idx,vecs2);
                if(sumx>r1)
                {
                    addLazy(expr<=r1);
                }
                if(sumx>r2)
                {
                    addLazy(expr<=r2);
                }
            }
        }
        catch (GRBException e)
        {
            cout << "Error number: " << e.getErrorCode() << endl;
            cout << e.getMessage() << endl;
        }
        catch (...)
        {
            cout << "Error during callback" << endl;
        }
    }
};
int main(int argc, char** argv)
{
    // n = 2000;
    // B = 15500;
    n=atoi(argv[1]);
    B=atoi(argv[2]);
    gen.seed(atoi(argv[3]));

    initvectors(n, n / 2, vecs1);
    initvectors(n, n / 2, vecs2);
    for (int i = 0; i < n; i++)
    {
        v.push_back(gen() % 2000 + 10);
        c.push_back(gen() % 200000 + 10);
    }

    try
    {
        // Create an environment
        GRBEnv env = GRBEnv(true);
        // env.set("LogFile", "lp.log");
        env.set(GRB_IntParam_OutputFlag, 0);
        env.start();
        // Create an empty model
        GRBModel model = GRBModel(env);
        model.set(GRB_IntParam_LazyConstraints, 1);
        vector<GRBVar> xs;
        for (int i = 0; i < n; i++)
        {
            xs.push_back(model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS));
        }
        GRBLinExpr expr = 0, obj = 0;
        matroidcallback cb= matroidcallback(&xs);
        model.setCallback(&cb);
        // for (int i = 1; i < (1 << n); i++)
        // {
        //     vector<int> idx;
        //     GRBLinExpr submodular = 0;
        //     bitset<200> bs(i); // !!!
        //     for (int j = 0; j < n; j++)
        //     {
        //         if (bs[j] == 1)
        //         {
        //             submodular += xs[j];
        //             idx.push_back(j);
        //         }
        //     }
        //     model.addConstr(submodular <= getrank(idx));
        // }
        for (int i = 0; i < n; i++)
        {
            expr += c[i] * xs[i];
            obj += v[i] * xs[i];
        }
        model.addConstr(expr <= B);
        GRBVar rdt=model.addVar(0.0,1.0,0.0,GRB_INTEGER);
        obj-=rdt;
        model.setObjective(obj, GRB_MAXIMIZE);
        model.optimize();
        cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
        double sumx=0,_B=0;
        // vector<int> idxs;
        vector<double> sol(n);
        vector<int> frac;
        for(int i=0;i<n;i++)
        {
            _B+=xs[i].get(GRB_DoubleAttr_X)*c[i];
            sol[i]=xs[i].get(GRB_DoubleAttr_X);
            if(sol[i]!=0 && sol[i]!=1.0)    frac.push_back(i);
            // sumx+=xs[i].get(GRB_DoubleAttr_X);
            // if(xs[i].get(GRB_DoubleAttr_X)>0)   idxs.push_back(i);
        }
        cout<<"_B "<<_B<<endl;
        // cout<<"sumx "<<sumx<<endl;
        // cout<<"rank. "<<getrank(idxs)<<endl;
        cout<<"num of fractional value: "<<frac.size()<<'\n';
        for(auto e:frac)    cout<<sol[e]<<' ';
        cout<<endl;
    }
    catch (GRBException e)
    {
        cerr << e.getErrorCode() << '\n';
        cerr << e.getMessage() << '\n';
    }
}