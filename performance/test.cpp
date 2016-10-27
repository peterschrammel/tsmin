#include <cstdlib>
#include "../src/example/tsmin.h"
#include <ctime>


int random(int max) {
  return rand() % max;
}

void generate_random_testsuite(tsmin_intt& tsmin, int n_testcases, int n_testgoals, int max_goals_per_testcase) {
  for(int tcid=0;tcid<n_testcases;tcid++) { 
    tsmin_intt::testcaset tc;
    tc.id = tcid;
    int n_goals = random(max_goals_per_testcase);
    for(int i=0;i<n_goals;i++) {
      int g = random(n_testgoals);
      tc.add_goal(g);
    }
    tc.cost=tc.size();
    if(tc.size()>0) tsmin.add_testcase(tc); else tcid--;
  }
}

void do_test(int n_testcases, int n_goals, int n_max_goals, int n_best, bool print) {
  tsmin_intt tsmin;
  generate_random_testsuite(tsmin,n_testcases,n_goals,n_max_goals);
  std::cout << "Input test suite: " << std::endl;
  if(print) tsmin.print_testsuite(std::cout,tsmin.get_input_testsuite(),true);
  tsmin.set_n_total_goals(n_goals);
  std::cout << "size = " << n_testcases << std::endl;
  std::cout << "covered goals = " << tsmin.compute_n_covered_goals() << std::endl;
  std::cout << "coverage = " << tsmin.get_input_coverage() << std::endl;
  std::cout << "cost = " << tsmin.compute_input_cost() << std::endl;

  std::cout << "*** Best " << n_best << " test suite *** " << std::endl;
  clock_t time_start = clock();  
  tsmin.best_n(n_best);
  std::cout << "time: " << ((double)(clock() - time_start) / CLOCKS_PER_SEC) << std::endl;

  std::cout << "Output test suite: " << std::endl;
  tsmin_intt::testsuitet tso = tsmin.get_output_testsuite();
  if(print) tsmin.print_testsuite(std::cout,tso,true);
  std::cout << "size = " << tso.size() << std::endl;
  std::cout << "covered goals = " << tsmin.get_output_n_covered_goals() << std::endl;
  std::cout << "coverage = " << tsmin.get_output_coverage() << std::endl;
  std::cout << "cost = " << tsmin.get_output_cost() << std::endl;

  std::cout << "*** Minimize test suite *** " << std::endl;
  time_start = clock();  
  tsmin.minimize();
  std::cout << "time: " << ((double)(clock() - time_start) / CLOCKS_PER_SEC) << std::endl;

  std::cout << "Output test suite: " << std::endl;
  tso = tsmin.get_output_testsuite();
  if(print) tsmin.print_testsuite(std::cout,tso,true);
  std::cout << "size = " << tso.size() << std::endl;
  std::cout << "covered goals = " << tsmin.get_output_n_covered_goals() << std::endl;
  std::cout << "coverage = " << tsmin.get_output_coverage() << std::endl;
  std::cout << "cost = " << tsmin.get_output_cost() << std::endl;
}


int main(int argc, char* argv[]) {
  srand((unsigned)time(NULL));

  //  do_test(10000,100000,100,100,false);
  do_test(1000,10000,100,100,false);
}
