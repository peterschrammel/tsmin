/******************************************************************************/
/* TSmin Test Suite Minimization                                              */
/* Author: Peter Schrammel                                                    */
/* Version: 0.2                                                               */
/*                                                                            */
/* TSmin template                                                             */
/******************************************************************************/

#ifndef TSMIN_TSMIN_TEMPLATE_H
#define TSMIN_TSMIN_TEMPLATE_H

#include <assert.h>
#include <set>
#include <list>
#include <map>
#include <cmath>
#include <iostream>

template<class TestCaseIdT, class TestGoalIdT, class TestCaseCostT>
class tsmin_templatet {
 public:

  // ********* types *********
  typedef std::set<TestGoalIdT> testgoalst;
  typedef std::set<TestCaseIdT> testsuitet;

  /*****************************************************************************/
  // test case descriptor
  /*****************************************************************************/
  class testcase_desct {
    public:
      TestCaseIdT id;
      testgoalst goals; 
      TestCaseCostT cost;
      unsigned size() { return goals.size(); }

      void add_goal(TestGoalIdT g) { goals.insert(g); }

      // print a test case descriptor
      std::ostream& print(std::ostream& out) {
        out << id << ": cost=" << cost << ", size=" << size() << ", goals=[";
        for(typename testgoalst::const_iterator g = goals.begin();
             g != goals.end(); g++) {
          out << *g << " ";
        }
        out << "]";
        return out;
     }
  };

  /*****************************************************************************/
  // test suite descriptor (main entry point to all functions)
  /*****************************************************************************/
  class testsuite_desct {
  public:

    //  ********* input ********* 

    void add_testcase(testcase_desct tc) { 
      testcases.insert(std::pair<TestCaseIdT,testcase_desct>(tc.id,tc));
      tsi.push_back(tc); 
    }

    //  ********* get info ********* 

    //returns covered goals in test suite
    void get_covered_goals(testgoalst& goals) {
      goals.clear();
      for(typename testsuite_spect::const_iterator it = tsi.begin();
          it != tsi.end(); it++) {
        goals.insert(it->goals.begin(),it->goals.end());  
      } 
    }

    //computes number of covered goals in test suite
    unsigned number_of_covered_goals() {
      testgoalst goals;
      get_covered_goals(goals);
      return goals.size();
    }

    //returns covered goals in given test suite
    void get_covered_goals(testsuitet& ts,testgoalst& goals) {
      goals.clear();
      for(typename testsuitet::const_iterator it = ts.begin();
          it != ts.end(); it++) {
        if(testcases.find(*it)!=testcases.end()) {
          testcase_desct& tc = testcases[*it];
          goals.insert(tc.goals.begin(),tc.goals.end());  
	}
      } 
    }

    //computes number of covered goals in given test suite
    unsigned number_of_covered_goals(testsuitet& ts) {
      testgoalst goals;
      get_covered_goals(ts,goals);
      return goals.size();
    }

    //computes coverage of test suite
    float get_coverage(unsigned total_number_of_testgoals) {
      assert(total_number_of_testgoals>0);
      return (float)number_of_covered_goals()/(float)total_number_of_testgoals;
    }

    //computes coverage of test suite
    float get_coverage(testgoalst total_testgoals) {
      return get_coverage(total_testgoals.size());
    }

    //computes coverage of given test suite
    float get_coverage(testsuitet& ts, unsigned total_number_of_testgoals) {
      assert(total_number_of_testgoals>0);
      return (float)number_of_covered_goals(ts)/(float)total_number_of_testgoals;
    }

    //computes coverage of test suite
    float get_coverage(testsuitet& ts, testgoalst total_testgoals) {
      return get_coverage(ts,total_testgoals.size());
    }

    //computes cost of test suite
    TestCaseCostT get_cost() {
      TestCaseCostT cost = 0;
      for(typename testsuite_spect::const_iterator it = tsi.begin();
          it != tsi.end(); it++) {
        cost += it->cost;
      } 
      return cost;
    }

    //computes cost of given test suite
    TestCaseCostT get_cost(testsuitet& ts) {
      TestCaseCostT cost = 0;
      for(typename testsuitet::const_iterator it = ts.begin();
          it != ts.end(); it++) {
        if(testcases.find(*it)!=testcases.end()) {
          testcase_desct& tc = testcases[*it];
          cost += tc.cost;
	}
      }
      return cost;
    }

    //  ********* printing ********* 

    // print the given test suite
    std::ostream& print_testsuite(std::ostream& out, const testsuitet& ts, 
                                  bool show_desc) {
      if(!show_desc) {
	for(typename testsuitet::const_iterator it = ts.begin();
	    it != ts.end(); it++) {
	  out << *it << std::endl; 
	}
      }
      else {
	out << "{ " << std::endl;
	for(typename testsuitet::const_iterator it = ts.begin();
	    it != ts.end(); it++) {
	  testcase_desct tc = testcases.at(*it);
	  out << "  ";
	  tc.print(out) << std::endl;
	}
	out << "}" << std::endl;
      }
      return out;
    }

    // print the test suite
    std::ostream& print_testsuite(std::ostream& out) {
      return print_testsuite_spec(out,tsi);
    }

    //  ********* test suite minimization ********* 

    //find minimum test suite that covers n goals
    void best_cover(testsuitet& ts_out, unsigned n_goals) {
      //ordering by descending sizes (additionally_covered_goals)
      ts_out.clear();
      basic_greedy(tsi,ts_out,break_best_cover,compare_size,n_goals);
    }

    //find minimum test suite that covers n goals while minimizing cost
    void best_cover_min_cost(testsuitet& ts_out, unsigned n_goals) {
      //ordering by descending benefit (additionally_covered_goals/cost)
      ts_out.clear();
      basic_greedy(tsi,ts_out,break_best_cover,compare_benefit,n_goals);
    }

    //find minimum test suite that achieves the given coverage  
    void best_cover(testsuitet& ts_out, unsigned n_total_goals, double coverage) {
      best_cover(ts_out,ceil(n_total_goals*coverage));
    }

    //find minimum test suite that achieves the given coverage  
    void best_cover(testsuitet& ts_out, testgoalst& total_goals, double coverage) {
      best_cover(ts_out,ceil(total_goals.size()*coverage));
    }

    //find minimum test suite that achieves the given coverage while minimizing cost
    void best_cover_min_cost(testsuitet& ts_out, unsigned n_total_goals, double coverage) {
      best_cover_min_cost(ts_out,ceil(n_total_goals*coverage));
    }

    //find minimum test suite that achieves the given coverage while minimizing cost
    void best_cover_min_cost(testsuitet& ts_out, testgoalst& total_goals, double coverage) {
      best_cover_min_cost(ts_out,ceil(total_goals.size()*coverage));
    }

    //find minimum test suite with equal coverage
    void minimize(testsuitet& ts_out) {
      best_cover(ts_out,number_of_covered_goals());
    }

    //find minimum test suite with equal coverage while minimizing cost
    void minimize_min_cost(testsuitet& ts_out) {
      best_cover_min_cost(ts_out,number_of_covered_goals());
    }

    //find minimum test suite with equal coverage on a subset of test goals
    void minimize(testsuitet& ts_out, testgoalst& goals_subset) {
      testsuite_spect tss; 
      copy_testsuite_spec(tsi,tss);
      restrict_testsuite(tss,goals_subset);
      ts_out.clear();
      basic_greedy(tss,ts_out,break_best_cover,compare_size,number_of_covered_goals());
    }

    //find minimum test suite with equal coverage on a subset of test goals while minimizing cost
    void minimize_min_cost(testsuitet& ts_out, testgoalst& goals_subset) {
      testsuite_spect tss; 
      copy_testsuite_spec(tsi,tss);
      restrict_testsuite(tss,goals_subset);
      ts_out.clear();
      basic_greedy(tss,ts_out,break_best_cover,compare_benefit,number_of_covered_goals());
    }

    //returns a test suite with the n test cases achieving maximum coverage
    void best_n (testsuitet& ts_out, unsigned n) {
      //ordering by descending sizes (additionally_covered_goals)
      ts_out.clear();
      basic_greedy(tsi,ts_out,break_best_n,compare_size,n);
    }

    //returns a test suite with the n test cases achieving maximum coverage while covering all must_goals; the test suite is empty if not all must_goals could be covered
    void best_n (testsuitet& ts_out, unsigned n, testgoalst& must_goals) {
      //ordering by descending sizes (additionally_covered_goals)
      testsuite_spect tss; 
      copy_testsuite_spec(tsi,tss);
      restrict_testsuite(tss,must_goals);
      ts_out.clear();
      basic_greedy(tss,ts_out,break_best_n,compare_size,n);
      if(ts_out.size()<n) { //fill up with best non-must goals
        testsuite_spect tss; 
        copy_testsuite_spec(tsi,tss);
        testgoalst nonmust_goals;
        get_covered_goals(nonmust_goals);
        goals_diff(nonmust_goals,must_goals);
        restrict_testsuite(tss,nonmust_goals);
        basic_greedy(tss,ts_out,break_best_n,compare_size,n);
      }
    }

    //returns a test suite with the n test cases achieving maximum coverage while minimizing cost
    void best_n_min_cost (testsuitet& ts_out, unsigned n) {
      //ordering by descending benefit (additionally_covered_goals/cost)
      basic_greedy(tsi,ts_out,break_best_n,compare_benefit,n);
    }

    //returns a test suite with the n test cases achieving maximum coverage while minimizing cost and covering all must_goals; the test suite is empty if not all must_goals could be covered
    void best_n_min_cost (testsuitet& ts_out, unsigned n, testgoalst must_goals)      {
      //ordering by descending sizes (additionally_covered_goals)
      testsuite_spect tss; 
      copy_testsuite_spec(tsi,tss);
      restrict_testsuite(tss,must_goals);
      ts_out.clear();
      basic_greedy(tss,ts_out,break_best_n,compare_benefit,n);
      if(ts_out.size()<n) { //fill up with best non-must goals
        testsuite_spect tss; 
        copy_testsuite_spec(tsi,tss);
        testgoalst nonmust_goals;
        get_covered_goals(nonmust_goals);
        goals_diff(nonmust_goals,must_goals);
        restrict_testsuite(tss,nonmust_goals);
        basic_greedy(tss,ts_out,break_best_n,compare_benefit,n);
      }    
   }


    //  ********* internal functions ********* 

  protected:
    typedef std::list<testcase_desct> testsuite_spect;
    typedef std::map<TestCaseIdT,testcase_desct> testcasest;
    testsuite_spect tsi; // the input test suite ordered by cost
    testcasest testcases;  // the input test suite for access by test case id

    // print the given test suite spec
    std::ostream& print_testsuite_spec(std::ostream& out, testsuite_spect& tsi) {
      out << "{ " << std::endl;
      for(typename testsuite_spect::const_iterator it = tsi.begin();
	  it != tsi.end(); it++) {
	testcase_desct tc = *it;
	out << "  ";
	tc.print(out) << std::endl;
      }
      out << "}" << std::endl;
      return out;
    }

    // copy a test suite specification
    static void copy_testsuite_spec(const testsuite_spect& from, testsuite_spect& to) {
       to.clear();
       for(typename testsuite_spect::const_iterator it = from.begin();
	  it != from.end(); it++) {
	to.push_back(*it);
      }
    }

    //reduce input test suite to given goals to cover
    static void restrict_testsuite(testsuite_spect& _tsi, testgoalst& goals) {
      testsuite_spect tss; 
      copy_testsuite_spec(_tsi,tss);
      _tsi.clear();  
      for(typename testsuite_spect::const_iterator it = tss.begin();
	  it != tss.end(); it++) {
	testcase_desct tc;
	tc.id = it->id;
	tc.cost = it->cost;
	for(typename testgoalst::const_iterator g = it->goals.begin();
	    g != it->goals.end(); g++) {
	  if(goals.find(*g)!=goals.end()) tc.add_goal(*g);
	}
	_tsi.push_back(tc);  
      } 
    }

    //this one does the main work: basic, non-optimal (!), greedy n^2 algorithm by descending ordering
    template <typename BreakCond, typename Compare>
    void basic_greedy(testsuite_spect& _tsi, testsuitet& tso, 
                        BreakCond break_cond, Compare compare, unsigned n) {
      testgoalst goals;
      goals.clear();
      for(typename testsuitet::const_iterator it = tso.begin();
          it != tso.end(); it++) {
        if(testcases.find(*it)!=testcases.end()) {
          testcase_desct tc = testcases[*it];
          goals.insert(tc.goals.begin(),tc.goals.end());  
	}
      } 
      unsigned no_covered = goals.size();
      testsuite_spect tss;
      copy_testsuite_spec(_tsi,tss);
      while(break_cond(tso.size(),no_covered,n) && !tss.empty()) {
	tss.sort(compare); // descending order of remaining test cases
        #if 0
	   print_testsuite_spec(std::cerr,tss);
        #endif
	testcase_desct tc = *tss.begin(); // pick test case on top
        #if 0
          std::cerr << "picked tc: " << tc.id << " with benefit " << 
            ((float)tc.size()/(float)tc.cost) << std::endl;
        #endif
	if(tc.goals.empty()) break; // break if it does not contribute (and so do the remaining ones)
	no_covered += tc.size();
	tss.erase(tss.begin()); // remove test case on top
	tso.insert(tc.id); // add it to the output suite
	for(typename testsuite_spect::iterator it = tss.begin();
	    it != tss.end(); it++) {
	  if(it->goals.empty()) break; // break if remaining test cases will not contribute anyway
	  goals_diff(it->goals,tc.goals); // remove goals from test case that are covered by the picked one
	}    
      }  
    }

  // compute goal set difference
  static void goals_diff(testgoalst& goals1, testgoalst& goals2) {
    typename testgoalst::iterator it1 = goals1.begin();
    typename testgoalst::iterator it2 = goals2.begin();
    while (it1!=goals1.end() && it2!=goals2.end()) {
      if (*it1<*it2) ++it1;
      else if (*it2<*it1) ++it2;
      else {
        TestGoalIdT g = *it1;
        ++it1; ++it2;
        goals1.erase(g);
      }
    }
  }

  //breaking condition based on output coverage
  static bool break_best_cover(unsigned size, unsigned n_covered, unsigned n) { return n_covered<n; }

  //breaking condition based on output test suite size
  static bool break_best_n(unsigned size, unsigned n_covered, unsigned n) { return size<n; }

  //descending orders
  static bool compare_cost(testcase_desct x,testcase_desct y) { return x.cost>y.cost; }
  static bool compare_size(testcase_desct x,testcase_desct y) { return x.size()>y.size(); }
  static bool compare_benefit(testcase_desct x,testcase_desct y) { 
    if(x.cost!=0 && y.cost!=0) return x.size()/(double)x.cost>y.size()/(double)y.cost; 
    if(x.cost!=0) return true;
    return false;
  }

  }; //end testsuite_desct

};

#endif
