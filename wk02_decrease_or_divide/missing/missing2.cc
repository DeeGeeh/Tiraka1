#include <cstdlib>
#include <iostream>
#include "test.hh"

int searchSmallestMissing(int* A, int left, int right)
{
    if (left > right) {
        return NO_VALUE_MISSING;
    }

    if (left == right) {
        if (A[left] != A[0] + left) {
            return A[0] + left;
        } else {
            return NO_VALUE_MISSING;
        }
    }

    int mid = (left + right) / 2;
    if (A[mid] != A[0] + mid) {
        return searchSmallestMissing(A, left, mid);
    } else {
        return searchSmallestMissing(A, mid + 1, right);
    }
}

/*
int main(int argc, char const *argv[])
{
    int arr[10] = {2, 4, 5, 6, 7, 8, 9, 10, 11, 13};

    std::cout << searchSmallestMissing(arr, 0, 9) << std::endl;

    return 0;
}
*/