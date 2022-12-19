CCC = g++ 
FLAGS = -O3
GRBPATH = /home/congyu/gurobi952/

lpcallback: lpcallback.cc
	$(CCC) $(FLAGS) lpcallback.cc -o lpcallback -I$(GRBPATH)/linux64/include -L$(GRBPATH)/linux64/lib -lgurobi_c++ -lgurobi95
