LIBS=-llwperf -leiger -lsqlite3 -lpapi

all: null eiger csv sstmac

null: test.cpp
	$(CXX) -o test_null test.cpp $(CXXFLAGS) $(LDFLAGS) $(LIBS)

eiger: test.cpp
	$(CXX) -o test_eiger test.cpp $(CXXFLAGS) $(LDFLAGS) $(LIBS) -DLWPERF_USE_EIGER

csv: test.cpp
	$(CXX) -o test_csv test.cpp $(CXXFLAGS) $(LDFLAGS) $(LIBS) -DLWPERF_USE_CSV 

sstmac: test.cpp
	$(CXX) -o test_sstmac test.cpp $(CXXFLAGS) $(LDFLAGS) $(LIBS) -DLWPERF_USE_SSTMAC

clean:
	@rm -rf test_null test_eiger test_csv test_sstmac site1.csv database
