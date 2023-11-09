#include "RedBlackTree.h"
#include <iostream>
#include <vector>
#include <set>
#include <iomanip>
#include <random>
#include <algorithm>
#include <cstddef>
using namespace std;

namespace {
  void fail(const string& str) {
    cerr << endl;
    cerr << "Error: " << str << endl;
    cerr << "Run this program in gdb and backtrace for more details." << endl;
    abort();
  }
  
  const size_t kNumRounds  = 10;   // Rounds of testing
  const int    kMinValue   = 0;
  const int    kMaxValue   = 1000;
  const int    kNumInserts = (kMaxValue - kMinValue) * 10;
}

int main() {
  mt19937 gen;
  uniform_int_distribution<int> dist(kMinValue, kMaxValue);
  
  cout << "These automated tests will hit your red/black tree with random" << endl;
  cout << "sequences of operations and confirm that they match the expected" << endl;
  cout << "behavior." << endl;
  cout << endl;
  for (size_t round = 1; round <= kNumRounds; round++) {
    cout << "Round " << round << " / " << kNumRounds << "... " << flush;
    
    RedBlackTree t;
    
    /* Reference implementation; is sorted. */
    vector<int> ref;
    ref.reserve(kMaxValue - kMinValue);
   
    for (int i = 0; i < kNumInserts; i++) {
      int toAdd = dist(gen);
      
      /* Add to the reference list if not yet there. */
      bool expected = false;
      auto itr = lower_bound(ref.begin(), ref.end(), toAdd);
      if (itr == ref.end() || *itr != toAdd) {
        ref.insert(itr, toAdd);
        expected = true;
      }
      
      /* Confirm the insertion behaves as expected. */
      if (t.insert(toAdd) != expected) {
        fail("Insert operation did not behave as expected.");
      }
      
      /* Confirm the right values are there. */
      for (int value = kMinValue; value < kMaxValue; value++) {
        if (t.contains(value) != binary_search(ref.begin(), ref.end(), value)) {
          fail("Contains operation did not behave as expected.");
        }
      }
      
      /* Confirm rank works. */
      for (int value = kMinValue; value < kMaxValue; value++) {
        if (t.rankOf(value) != size_t(lower_bound(ref.begin(), ref.end(), value) - ref.begin())) {
          fail("rankOf operation did not behave as expected.");
        }
      }
      
      /* Confirm select works on valid indices. */
      for (size_t i = 0; i < ref.size(); i++) {
        if (t.select(i) != ref[i]) {
          fail("select operation did not behave as expected.");
        }
      }
    }
    
    /* Just once, try doing an out-of-bounds select. */
    try {
      (void) t.select(ref.size());
      fail("select operation did not behave as expected.");
    } catch (const runtime_error &) {
      // All is well!
    } catch (...) {
      fail("select operation did not behave as expected.");
    }
    
    cout << "done!" << endl;
  }
  
  cout << "All tests passed!" << endl;
}
