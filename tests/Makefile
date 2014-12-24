INC=-I/home/eric/.local/include/lwperf -g
LIB=-L/home/eric/.local/lib -llwperf -leiger -lsqlite3

all: null eiger csv sstmac

null: test.cpp
	$(CXX) -o test_null test.cpp $(INC) $(LIB)

eiger: test.cpp
	$(CXX) -o test_eiger test.cpp $(INC) $(LIB) -DLWPERF_USE_EIGER

csv: test.cpp
	$(CXX) -o test_csv test.cpp $(INC) $(LIB) -DLWPERF_USE_CSV 

sstmac: test.cpp
	$(CXX) -o test_sstmac test.cpp $(INC) $(LIB) -DLWPERF_USE_SSTMAC

clean:
	@rm -rf test_null test_eiger test_csv test_sstmac site1.csv database
