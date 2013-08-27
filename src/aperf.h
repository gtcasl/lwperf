#ifndef aperf_h
#define aperf_h
#include "csvformatter.h"
#include <map>
#include <cassert>

#ifndef LocationElements_h_seen
enum Location {
// An enum that must be tailored (along with getlog) to the code being instrumented.
// We want to define a single pile of instrumentation that
// gets installed in many different source variants of a code where the stuff here is common.
// Also want it to be autogenerated and to be fast to dispatch during runs, so no string 
// compares. A makefile rule keeps LocationElements.h current.
	X, // dummy for example
#ifndef PERF_DISABLE
#include "LocationElements.h"
#endif
};
#endif


/** A singleton performance logging class so that measurement 
does not pollute application argument lists and headers.
This approach seems to be the industry practice, cf. tau.

The specific perf counters and timers collected are tailored
in csvformatter.h and .cpp start/stop functions and in 
DEFAULT_PERFCTRS definition.
*/
class Perf {
public: /** USER interface; wrap these calls in PERFDECL to allow Perf suppression
	 at build time by passing -DPERF_DISABLE to compilers
	 init, fileOptions, mpiArgs, finalize may only be called in order, though
	spread across the application.
	 */
	static void init();
	/** 
	* @param prefix: typically a host or architecture name for the data file set.
	* @param suffix: typically .log (.dat, .out, .txt, etc) for data file set
	*/
	static void fileOptions(std::string prefix, std::string suffix);
        /** Several string option inputs; none can include no whitespace.
        * @param host: can be a name or an architecture.
        * @param tools: compiler set.
        * @param application: name of the application
        * @param database: name of database in mysql.
        * @param prefix: used for the data file set.
        * @param suffix: typically .log (.dat, .out, .txt, etc) for data file set
        * @bug no way to specify eiger database user, pw, hostname; hard coded.
        */
        static void stringOptions(std::string host, std::string tools, std::string application, std::string database, std::string prefix, std::string suffix);

	static void mpiArgs(int rank, int size) ;
	static void finalize();
	// The rest of the perf user interface is the
	// PERFDECL, PERFLOG, PERFSTOP, and optional PERFSTART at bottom.

private:
	std::map<enum Location, csvformatter *> log;
	std::string prefix;
	std::string suffix;
	bool append;
	int mpirank;
	int mpisize;
	bool mpiused;

	void initX(csvformatter *cf) {}
#ifndef PERF_DISABLE
// A makefile rule keeps CSVInitFuncs.h current.
#include "CSVInitFuncs.h"
#endif

	Perf(bool append) : append(append), mpirank(0), mpisize(1), mpiused(false) {
//std::cout << "Perf:" << mpirank << "/" << mpisize <<std::endl;
	}
	~Perf() {
		for (std::map<enum Location, csvformatter *>::iterator i = log.begin();
			 i != log.end(); i++) {
			delete i->second;
		}
	}

	std::string makeFileName(std::string & filename);

	csvformatter *getLog(enum Location l, std::string filename, bool screen) {
		csvformatter *cf = log[l];
		if (!cf) {
			std::string fname = makeFileName(filename);
//std::cout << "aPerf getLog:" << mpirank << "/" << mpisize <<std::endl;
			csvformatter * ncf = new csvformatter(fname,screen, mpirank, mpisize,append);
			switch (l) {
			case X: initX(ncf); break;
#ifndef PERF_DISABLE
// A makefile rule keeps InitSwitchElements.h current.
#include "InitSwitchElements.h"
#endif
			default:
				assert(0 == "unexpected Location given to PERFLOG");
				break;
			}
			if (!append) { ncf->writeheaders(); }
			log[l] = ncf;
		}
		cf = log[l];
		return cf;
	}

// internals
public:
	// macro use only
	static csvformatter *Log(enum Location l, std::string filename, bool screen);
};

#ifdef PERF_DISABLE
// turn it all off
#define PERF ErRoR
#define PERFDECL(...)
#define PERFLOG(X, Y, ...)
#define PERFSTART(X)
#define PERFSTOP(X, ...) 
#define DP(v)
#define IP(v)

#else // no PERF_DISABLE defined
#ifndef PERF
#define PERF Perf
// what is its internal logger class
#define PERFFORMATTER csvformatter

// make stuff that goes away with -DPERF_DISABLE exist
#define PERFDECL(...) __VA_ARGS__

#ifdef _LWPERF_SCREEN
#define _USE_LS true
#else
#define _USE_LS false
#endif

/** This macro is able to handle overlapping timers by different names. 
 It defines a local variable logX. X is a Location enum member, a timer name.
 The variable are used only by the include file generator,
 E.g. PERFLOG(site1,IP(param1))
 @param X name of timer location; must be unique in application and follow
the rules of C enum members.
  @param ... IP(name) or DP(name) for int/double log data names .
*/
#define PERFLOG(X, ...) \
  csvformatter * log##X = Perf::Log(lwperf_##X, #X, _USE_LS); log##X->start()

/** Compute perf counters and record values. USER values should already be computed.
 VARARGS list should be same as to PERFLOG, e.g. PERFSTOP(site1,IP(param1_value),...)
but here the values are given to IP()/DP() rather than their names.
*/
#define PERFSTOP(X, ...) \
   { log##X->stop();  csvformatter *tmpcf = log##X;  __VA_ARGS__ ; } log##X->nextrow()

/** Reset reference time point and other perf counter initial values
 not normally needed separately after PERFLOG, but C++ scoping rules
 may make it handy to have in more complex cases.
*/
#define PERFSTART(X) \
  log##X->start()

#define DP(v) tmpcf->put(v)
#define IP(v) tmpcf->put(v)

#endif // PERF

#endif // PERF_DISABLE

#endif // aperf_h