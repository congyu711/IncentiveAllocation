CCC = g++ 
FLAGS = -O3
GRBPATH = /home/congyu/gurobi952/

all: lp gen lyft

lp: lp.cpp 
	$(CCC) $(FLAGS) lp.cpp -o lp -I$(GRBPATH)/linux64/include -L$(GRBPATH)/linux64/lib -lgurobi_c++ -lgurobi95

clean: 
	rm lp gen lyft

gen: gen.cpp
	$(CCC) gen.cpp -o gen $(FLAGS)

lyft:
	$(CCC) lyft.cpp -o lyft $(FLAGS)