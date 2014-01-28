/* 
 * File:   testutils.cc
 * Author: ibgmike
 *
 * Created on Jan 26, 2014, 11:37:42 AM
 */

#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

void test1() {
    std::cout << "testutils test 1" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% testutils" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (testutils)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (testutils)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

