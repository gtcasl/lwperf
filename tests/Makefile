INC=-I/home/eric/.local/include/lwperf -g
LIB=-L/home/eric/.local//lib -llwperf

all: null eiger csv

null: test.cpp
	$(CXX) -o test_null test.cpp $(INC) $(LIB)

eiger: test.cpp
	$(CXX) -o test_eiger test.cpp $(INC) $(LIB) -D_USE_EIGER

csv: test.cpp
	$(CXX) -o test_csv test.cpp $(INC) $(LIB) -D_USE_CSV

clean:
	@rm -rf test_null test_eiger test_csv cite1.csv database
