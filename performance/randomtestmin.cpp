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
randomtestmin <input file> <total test goals>

file format:
(((<tg>|fff\(<cost>\))<space>)+\n)*
 */
int main(int argc, char* argv[]) {
  tsmin_strintt tsmin;
  assert(argc==3);
  std::string line;
  std::ifstream infile (argv[1]);
  tsmin.set_n_total_goals(atoi(argv[2]));
  if (!infile.is_open()) {
    std::cout << "Unable to open file";  
    return 1;
  }
  while (infile.good())
    {
    getline (infile,line);
    tsmin_strintt::testcaset tc;
    tc.id = line;
    std::istringstream iss(line);
    std::set<unsigned> tempgoals;
    while (iss)
    {
        std::string goal;
        iss >> goal;
        if(goal=="") break;
        if(goal.at(0)!='f' && goal.at(0)!='s') {
	  tempgoals.insert(atoi(goal.c_str()));
	}
        if(goal.at(0)=='s') {
          tempgoals.clear();
	}
        if(goal.at(0)=='f') {
          tc.cost=atoi(goal.substr(4,goal.size()-5).c_str());
	  for(std::set<unsigned>::iterator it = tempgoals.begin();
	      it!=tempgoals.end(); it++) {
	    tc.add_goal(*it);
	  }
          tempgoals.clear();
	}
    }  
    if(tc.size()!=0) tsmin.add_testcase(tc);
  }
  infile.close();
 
  //  tsmin.print_testsuite(std::cout,tsmin.get_input_testsuite(),true);
  //std::cout << "size = " << tsmin.get_input_testsuite().size() << std::endl;
  tsmin.compute_n_covered_goals();
  tsmin.compute_input_cost();
  std::cout << "*** Minimize test suite *** " << std::endl;
  clock_t time_start = clock();  
  tsmin.minimize();
  std::cout << "time: " << ((double)(clock() - time_start) / CLOCKS_PER_SEC) << std::endl;
  tsmin.print_testsuite(std::cout,tsmin.get_output_testsuite(),true);
  std::cout << "size = " << tsmin.get_output_testsuite().size() << std::endl;
  std::cout << "covered goals = " << tsmin.get_output_n_covered_goals() << std::endl;
  std::cout << "coverage = " << tsmin.get_output_coverage() << std::endl;
  std::cout << "cost = " << tsmin.get_output_cost() << std::endl;

  return 0;
}
