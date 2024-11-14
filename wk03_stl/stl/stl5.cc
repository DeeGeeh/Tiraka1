#include <iterator>
#include <vector>
#include <algorithm>

#include "test.hh"

using namespace std;


/**
 * @brief Arrange vector in three subsequent sections:
 *        - those divisible by three (asc order)
 *        - those whose reminder is 1 (asc order)
 *        - those whose reminder is 2 (asc order)
 * @param v vector to be sorted
 * @return int EXIT_SUCCESS if everything went OK, EXIT_FAILURE otherwise
 */
int sortMod3(std::vector<int>& v)
{
    try {
        auto comp = [v] (int a, int b) {
            if (a % 3 != b % 3) {
                return (a % 3) < (b % 3);
            }
            return a < b;
        };
        stable_sort(v.begin(), v.end(), comp);

    } catch (exception) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

