CCC = g++ 
FLAGS = -O3
GRBPATH = /home/congyu/gurobi952/

trivial: trivial.cc
	$(CCC) $(FLAGS) trivial.cc -o trivial -I$(GRBPATH)/linux64/include -L$(GRBPATH)/linux64/lib -lgurobi_c++ -lgurobi95

callback: callback.cc
	$(CCC) $(FLAGS) callback.cc -o callback -I$(GRBPATH)/linux64/include -L$(GRBPATH)/linux64/lib -lgurobi_c++ -lgurobi95
