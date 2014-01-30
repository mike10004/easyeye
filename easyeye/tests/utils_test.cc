/* 
 * File:   testutils.cc
 * Author: ibgmike
 *
 * Created on Jan 26, 2014, 11:37:42 AM
 */

#include <cerrno>
#include <cstdlib>
#include <iostream>
#include <string>
#include <uuid/uuid.h>
#include <sstream>
#include <cassert>
#include <cstring>
#include "../src/easyeye/common/easyeye_utils.h"
#include <unistd.h>

using namespace easyeye;
using namespace std;

static void stateFailed(const char* testname, const char* message) 
{
    cout << "%TEST_FAILED% time=0 testname=" << testname << " (utils) message=" << message << endl;
}

string NewUniqueTempName() {
    ostringstream ss;
    ss << "/tmp/tmpdir-";
    uuid_t uuid;
    uuid_generate(uuid);
    char uuid_str[37];
    uuid_unparse_lower(uuid, uuid_str);
    ss << uuid_str;
    return ss.str();
}

string NewUniqueManyPartName() {
    ostringstream ss;
    ss << NewUniqueTempName();
    for (int i = 0; i < 5; i++) {
        ss << '/' << i;
    }
    return ss.str();
}

void testMakeDirs() {
    std::cout << "testutils testMakeDirs" << std::endl;
    
    string dirname = NewUniqueManyPartName();
    
    cout << "unique name: " << dirname << endl;
    int error_code;
    bool exists = IOUtils::MakeDirs(dirname, &error_code);
    cout << "directory now exists? " << exists << endl;
    cout << "error code = " << error_code << endl;
    if (error_code != 0) {
        cout << "error message: " << strerror(error_code) << endl;
    }
    if (!exists) {
        stateFailed("testMakeDirs", "directory does not exist");
    }
    rmdir(dirname.c_str());
    if (IOUtils::IsDirectory(dirname)) {
        cerr << "warning: failed to delete directory" << endl;
    }
}

void testMakeDir() {
    std::cout << "testutils testMakeDir" << std::endl;
    
    string dirname = NewUniqueTempName();
    
    cout << "unique name: " << dirname << endl;
    int error_code;
    bool exists = IOUtils::MakeDirs(dirname, &error_code);
    cout << "directory now exists? " << exists << endl;
    cout << "error code = " << error_code << endl;
    if (error_code != 0) {
        cout << "error message: " << strerror(error_code) << endl;
    }
    if (!exists) {
        stateFailed("testMakeDirs", "directory does not exist");
    }
    rmdir(dirname.c_str());
    if (IOUtils::IsDirectory(dirname)) {
        cerr << "warning: failed to delete directory" << endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% utils" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testMakeDir (testutils)" << std::endl;
    testMakeDir();  
    std::cout << "%TEST_FINISHED% time=0 testMakeDir (testutils)" << std::endl;

    std::cout << "%TEST_STARTED% testMakeDirs (testutils)" << std::endl;
    // testMakeDirs(); // can't delete this directory, so we shouldn't run this test often
    std::cout << "%TEST_FINISHED% time=0 testMakeDirs (testutils)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

