/* 
 * File:   options_test.cc
 * Author: ibgmike
 *
 * Created on Jan 29, 2014, 3:20:30 PM
 */

#include <iostream>
#include "../src/easyeye_compare_main.h"
#include <easyeye/common/easyeye_utils.h>
#include <string>
#include <vector>

using namespace easyeye::compare;
using namespace std;

void assertEquals(int expected, int actual, const string& testname, const string& message)
{
    if (expected != actual) {
        cout << "%TEST_FAILED% time=0 testname=" << testname << " (options_test) message=" 
                << message << "; expected " << expected << " != " << actual << " (actual)" << endl;
    } else {
        cout << testname << ": assertEquals(" << expected << ", " << actual << ") ok" << endl;
    }
}

void testHelp() {
    cout << "options_test test help" << endl;
    
    Compare program;
    vector<string> args;
    char* argv[] = {(char*)"easyeye-compare", (char*)"--help"};
    easyeye::Vectors::AddAll(argv, 2, args);
    
    int rv = program.Main(args);
    assertEquals(easyeye::program::kExitSuccess, rv, "testHelp", "wrong exit code");
}

void testVersion() {
    cout << "options_test test version" << endl;
    Compare program;
    vector<string> args;
    char* argv[] = {(char*)"easyeye-compare", (char*)"--version"};
    easyeye::Vectors::AddAll(argv, 2, args);
    
    int rv = program.Main(args);
    assertEquals(easyeye::program::kExitSuccess, rv, "testVersion", "wrong exit code");
}

void testNoArgs() {
    cout << "options_test test noArgs" << endl;
    Compare program;
    vector<string> args;
    char* argv[] = {(char*)"easyeye-compare"};
    easyeye::Vectors::AddAll(argv, 1, args);
    
    int rv = program.Main(args);
    assertEquals(easyeye::program::kErrorUsage, rv, "testNoArgs", "wrong exit code");
}

int main(int argc, char** argv) {
    cout << "%SUITE_STARTING% options_test" << endl;
    cout << "%SUITE_STARTED%" << endl;

    cout << "%TEST_STARTED% test1 (options_test)" << endl;
    testHelp();
    cout << "%TEST_FINISHED% time=0 test1 (options_test)" << endl;

    cout << "%TEST_STARTED% testVersion (options_test)\n" << endl;
    testVersion();
    cout << "%TEST_FINISHED% time=0 testVersion (options_test)" << endl;

    cout << "%TEST_STARTED% testNoArgs (options_test)" << endl;
    testNoArgs();
    cout << "%TEST_FINISHED% time=0 testNoArgs (options_test)" << endl;

    cout << "%SUITE_FINISHED% time=0" << endl;

    return (EXIT_SUCCESS);
}

