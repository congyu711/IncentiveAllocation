CCC = g++ 
FLAGS = -g -Wall -Wextra 
GRBPATH = /home/congyu/gurobi952/

build: lp.cpp 
	$(CCC) $(FLAGS) lp.cpp -o lp -I$(GRBPATH)/linux64/include -L$(GRBPATH)/linux64/lib -lgurobi_c++ -lgurobi95

clean: 
	rm lp gen

gendata: gen.cpp
	$(CCC) gen.cpp -o gen && ./gen