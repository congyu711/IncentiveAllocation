CCC = g++ 
FLAGS = -O3
GRBPATH = /home/congyu/gurobi952/

lpcallback: lpcallback.cc laminar.cc
	$(CCC) $(FLAGS) lpcallback.cc -o lpcallback -I$(GRBPATH)/linux64/include -L$(GRBPATH)/linux64/lib -lgurobi_c++ -lgurobi95

search_greedy: search_greedy.cc laminar.cc
	$(CCC) $(FLAGS) search_greedy.cc -o search_greedy