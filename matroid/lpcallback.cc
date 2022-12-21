#include <bits/stdc++.h>
#include "laminar.cc"
#include <gurobi_c++.h>
using namespace std;
int n; // number of items
int B; // budget

vector<int> v, c; // v&c.
vector<vector<int>> vecs;
mt19937 gen(19260817);
laminar* l;

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
                // vector<int> idx;
                double sumx=0;
                GRBLinExpr expr=0;
                vector<double> xval(xs->size());
                for(int i=0;i<xval.size();i++)
                {
                    xval[i]=getSolution(xs->at(i));
                }
                auto vio=l->isIndependent(xval,l->root);
                if(vio!=nullptr)
                {
                    for(int i=vio->l;i<vio->r;i++)
                    {
                        expr+=xs->at(i);
                    }
                    addLazy(expr<=vio->cap);
                }
                // for(int i=0;i<xs->size();i++)
                // {
                //     auto c=xval[i];
                //     if(c>0)
                //     {
                //         idx.push_back(i);
                //         sumx+=c;
                //         expr+=xs->at(i);
                //     }
                // }
                // auto r=getrank(idx);
                // if(sumx>r)
                // {
                //     addLazy(expr<=r);
                // }
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
int main()
{
    n = 200000;
    B = 155000000;
    l = new laminar(n);
    // initvectors(n, n / 2);
    for (int i = 0; i < n; i++)
    {
        v.push_back(gen() % 20 + 10);
        c.push_back(gen() % 200000 + 10);
    }
    cout<<"finish init\n";
    try
    {
        // Create an environment
        GRBEnv env = GRBEnv(true);
        // env.set("LogFile", "lp.log");
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
        cout<<boolalpha<<"feasible? "<<(l->isIndependent(sol,l->root)==nullptr)<<endl;
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