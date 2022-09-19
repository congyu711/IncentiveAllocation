/**
 * @file lp.cpp
 * @author congyu
 * @brief this is LP for the problem in 
 * https://eng.lyft.com/how-to-solve-a-linear-optimization-problem-on-incentive-allocation-5a8fb5d04db1
 * @version 0.1
 * @date 2022-08-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "gurobi_c++.h"
#include<bits/stdc++.h>
using namespace std;

int main()
{
    int n,m,C;
    ifstream fin("data.in");
    fin>>n>>m>>C;
    vector<vector<int>> v(n,vector<int>(m,0)),c(n,vector<int>(m,0));
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            fin>>v[i][j];
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            fin>>c[i][j];
    try
    {
        // Create an environment
        GRBEnv env = GRBEnv(true);
        env.set("LogFile", "lp1.log");
        env.start();
        // Create an empty model
        GRBModel model = GRBModel(env);
        vector<vector<GRBVar>> xs(n);
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<m;j++)
            {
                xs[i].push_back(model.addVar(0.0, 1.0, 0.0, GRB_CONTINUOUS));
            }
        }
        GRBLinExpr expr=0,obj=0;
        for(int i=0;i<n;i++)
        {
            GRBLinExpr lin=0;
            for(int j=0;j<m;j++)
            {
                lin+=xs[i][j];
                expr+=c[i][j]*xs[i][j];
                obj+=v[i][j]*xs[i][j];
            }
            model.addConstr(lin<=3);
        }
        model.addConstr(expr<=C);
        model.setObjective(obj,GRB_MAXIMIZE);
        model.optimize();
        cout << "Obj: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
        // double ans=0.0,cost=0.0;
        // ofstream fout("lp.out");
        // for(int i=0;i<n;i++)
        // {
        //     vector<int> non0;
        //     for(int j=0;j<m;j++)
        //     {
        //         if(xs[i][j].get(GRB_DoubleAttr_X)!=0)
        //         {    
        //             non0.push_back(j);
        //             fout<<c[i][j]<<' '<<v[i][j]<<'\n';
        //         }
        //     }
        //     fout<<endl;
        //     if(non0.size()==1)
        //     {
        //         if(xs[i][non0[0]].get(GRB_DoubleAttr_X)!=1)    continue;
        //         ans+=v[i][non0[0]];
        //         cost+=c[i][non0[0]];
        //     }
        //     else if(non0.size()==2)
        //     {
        //         // always use the small one
        //         if(c[i][non0[0]]>c[i][non0[1]])   swap(non0[0],non0[1]);
        //         ans+=v[i][non0[0]];
        //         cost+=c[i][non0[0]];
        //     }
        // }
        // cout<<"ans: "<<ans<<"\ncost: "<<cost<<endl;
    }
    catch (GRBException e)
    {
        cerr << e.getErrorCode() << '\n';
        cerr << e.getMessage() << '\n';
    }
}