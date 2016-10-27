/******************************************************************************/
/* TSmin Test Suite Minimization                                              */
/* Author: Peter Schrammel                                                    */
/* Version: 0.2                                                               */
/*                                                                            */
/* Example application                                                        */
/******************************************************************************/

#include <cstdlib>
#include "tsmin.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

/*
usage:
testmin [-best <n>]

reads input from stdin

input format:
<test goal identifier>*\n //goals to cover, must goals (for -best <n>)
(<test case identifier><space><test case cost>(<space><test goal identifier>)*\n)*

writes output to stdout

output format:
(<test case identifier>\n)* //empty if must-goals cannot be covered

*/

int main(int argc, char* argv[]) {
  if (argc!=1 && argc!=3) {
    std::cerr << "usage: ./testmin [-best <n>] < <inputfile>" << std::endl;  
    return -1;
  }
  bool do_best_n = false;
  unsigned best_n = 0;
  if (argc==3) {
    do_best_n = true;
    best_n = atoi(argv[2]);
  }
  tsmin_strstrt::testgoalst testgoals;
  testgoals.clear();
  if(std::cin.good()) {
    std::string line;
    getline (std::cin,line);
    std::istringstream iss(line);
    while (iss) {
      std::string goal;
      iss >> goal;
      if(goal!="") testgoals.insert(goal);
    }  
  }
  tsmin_strstrt::testsuite_desct tsdesc;
  while (std::cin.good()) {
    std::string line;
    getline (std::cin,line);
    tsmin_strstrt::testcase_desct tcdesc;
    tcdesc.goals.clear();
    std::istringstream iss(line);
    std::string id;
    std::string cost;
    if (iss) iss >> id;
    else  {
      std::cerr << "ERROR: test case identifier expected" << std::endl;  
      return -1;
    }
    if (id=="") continue;
    tcdesc.id = id;
    if (iss) iss >> cost;
    else  {
      std::cerr << "ERROR: test case cost expected" << std::endl;  
      return -1;
    }
    tcdesc.cost=atoi(cost.c_str()); 

    while (iss)
    {
      std::string goal;
      iss >> goal;
      if(goal!="") tcdesc.add_goal(goal);
    }  
    if(tcdesc.size()!=0) tsdesc.add_testcase(tcdesc);
    else std::cerr << "WARNING: Empty test case " << id << std::endl;  
  }
 
  // std::cerr << "*** Minimize test suite *** " << std::endl;
  clock_t time_start = clock();  

  tsmin_strstrt::testsuitet ts_out;
  if(do_best_n) {
    if(testgoals.empty()) tsdesc.best_n_min_cost(ts_out,best_n);
    else tsdesc.best_n_min_cost(ts_out,best_n,testgoals); // with must-goals
  }
  else {
    if(!testgoals.empty()) tsdesc.minimize_min_cost(ts_out,testgoals);
    else return -1;
  }

  tsdesc.print_testsuite(std::cout,ts_out,false);
  std::cerr << "time: " << ((double)(clock() - time_start) / CLOCKS_PER_SEC) << std::endl;
  std::cerr << "size = " << ts_out.size() << std::endl;
  std::cerr << "covered goals = " << tsdesc.number_of_covered_goals(ts_out) << std::endl;
  if(!do_best_n && !testgoals.empty()) std::cerr << "coverage = " << tsdesc.get_coverage(ts_out,testgoals) << std::endl;
  std::cerr << "cost = " << tsdesc.get_cost(ts_out) << std::endl;
  
  return 0;
}
