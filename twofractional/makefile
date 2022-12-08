CCC = g++ 
FLAGS = -O3
GRBPATH = /home/congyu/gurobi952/

lp: lp.cc
	$(CCC) $(FLAGS) lp.cc -o lp -I$(GRBPATH)/linux64/include -L$(GRBPATH)/linux64/lib -lgurobi_c++ -lgurobi95