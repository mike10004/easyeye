/* 
 * File:   ezimatch_test.cc
 * Author: mchaberski
 *
 * Created on Jul 17, 2013, 1:52:09 PM
 */

#include <stdlib.h>
#include <iostream>

void test1() {
    std::cout << "ezimatch_test test 1" << std::endl;
}

void test2() {
    std::cout << "ezimatch_test test 2" << std::endl;
//    std::cout << "%TEST_FAILED% time=0 testname=test2 (ezimatch_test) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% ezimatch_test" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (ezimatch_test)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (ezimatch_test)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (ezimatch_test)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (ezimatch_test)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

