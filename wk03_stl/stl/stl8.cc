#include <iterator>
#include <vector>
#include <algorithm>

#include "test.hh"

using namespace std;


/**
 * @brief Remove from vector v all elements with value less than the limit
 * @param v a vector whose values are to be erased if less than limit
 * @param limit a threshold whose lower values are removed
 * @return int EXIT_SUCCESS if everything went OK, EXIT_FAILURE otherwise
 */
int removeLessThan(std::vector<int>& v, int limit)
{
    try {
        v.erase(std::remove_if(v.begin(), v.end(), [limit](int a) {return a < limit;}),
                v.end());
    } catch (const exception& e) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

