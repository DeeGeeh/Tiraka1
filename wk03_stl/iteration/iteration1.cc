/**
 * iteration1.cc
 *
 * Print all items of a list
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include "iteration1.hh"
using namespace std;


void printAllItems(const list<int>& lst)
{
    std::list<int>::const_iterator iter = lst.begin();
    while (iter != lst.end()) {
        std::cout << *iter << " ";
        ++iter;
    }
    std::cout << std::endl;
}
