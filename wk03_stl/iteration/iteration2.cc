/**
 * iteration2.cc
 *
 * Print every second item of a list starting from the first item
 */

/**
 * DO NOT ADD ANY INCLUDES!!
 */

#include "iteration2.hh"
using namespace std;


void printEverySecond(const list<int>& lst)
{
    std::list<int>::const_iterator iter = lst.begin();
    int count = 0;

    while (iter != lst.end()) {
        if (count % 2 == 0) {
            std::cout << *iter << " ";
        }
        ++count;
        ++iter;
    }
    std::cout << std::endl;
}
