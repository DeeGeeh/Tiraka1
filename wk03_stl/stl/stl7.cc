#include <iterator>
#include <vector>
#include <algorithm>

#include "test.hh" // NOT_FOUND constant

using namespace std;


/**
 * @brief Find the median value of a given vector, whose elements are in random
 *        order. Return NOT_FOUND if the size of the vector is zero.
 *
 * @param v unsorted vector
 * @return int calculated median of parameter vector
 */
int findMedian(std::vector<int>& v)
{
    if (v.empty()) {
        return NOT_FOUND;
    }

    sort(v.begin(), v.end());
    int size = v.size();

    // CASE: Odd size
    if (size % 2 != 0) {
        return v.at(size/2);
    }

    int mid1 = v.at(size/2);
    int mid2 = v.at((size/2) - 1);
    return (mid1 + mid2) / 2;

}

