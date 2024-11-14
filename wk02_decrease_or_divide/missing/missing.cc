#include <cstdlib>
#include <iostream>
#include "test.hh"

int searchSmallestMissingIteration(int* arr, int size){
    if (size < 0) {
        return NO_VALUE_MISSING;
    }

    int temp = arr[0];
    for (int i = 1; i < size; ++i) {
        if (temp + 1 != arr[i]) {
            return temp + 1;
        }
        temp = arr[i];
    }

    return NO_VALUE_MISSING;
}

/*
int main(int argc, char const *argv[])
{
    int arr[10] = {4, 5, 6, 7, 9, 11, 12, 13, 14, 15};
    
    std::cout << searchSmallestMissingIteration(arr, 10) << '\n';

    return 0;
}
*/