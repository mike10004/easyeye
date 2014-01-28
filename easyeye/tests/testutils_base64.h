/* 
 * File:   testutils.h
 * Author: ibgmike
 *
 * Created on January 26, 2014, 11:37 AM
 */

#ifndef TESTUTILS_H
#define	TESTUTILS_H

#include <cstring>
#include <iostream>
#include <string>

class TestUtils
{
public:

    static bool IsEqual(const std::string& tag, int* a, int* b, const size_t len) 
    {
        std::cout << " testing equality of arrays of length " << len << std::endl;
        int cmp  = memcmp(a, b, sizeof(int) * len);
        int firstWrong = -1, numWrong = 0;
        if (cmp  != 0) {
            std::cerr << tag << " values not equal" << std::endl;
            for (int i = 0; i < len; i++) {
                if (a[i] != b[i]) {
                    if (firstWrong == -1) {
                        std::cerr << "...starting at index " << i << std::endl;
                        firstWrong = i;
                    }
                    numWrong++;
                }
            }
        }
        if (numWrong > 0) {
            std::cerr << "..." << numWrong << " of " << len << " values are different" << std::endl;
        }
        return numWrong == 0;
    }
        
private:
    TestUtils() {}
};


#endif	/* TESTUTILS_H */

