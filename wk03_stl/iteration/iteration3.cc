/**
 * iteration3.cc
 *
 * Print beginning half of a list
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include "iteration3.hh"
using namespace std;


void printHalf(const list<int>& lst)
{
    unsigned int mid = lst.size() / 2;
    std::list<int>::const_iterator iter = lst.begin();

    while (mid != 0) {
        std::cout << *iter << " ";
        --mid;
        ++iter;
    }
    std::cout << std::endl;
}
