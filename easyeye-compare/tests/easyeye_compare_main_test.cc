/* 
 * File:   main_test.cc
 * Author: mchaberski
 *
 * Created on Jul 16, 2013, 4:06:43 PM
 */

#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <easyeye/common/mylog.h>
#include "../src/easyeye_compare_main.h"
#include <vector>
#include <string>
#include <sstream>
#include <easyeye/common/easyeye_types.h>
#include <easyeye/common/easyeye_utils.h>

using namespace easyeye::compare;
using namespace std;

static const char* serialized_encodings[] = {
    "../testdata/encodings/gallery_132-encoding.json",
    "../testdata/encodings/gallery_022-encoding.json",
    "../testdata/encodings/gallery_014-encoding.json",
    "../testdata/encodings/gallery_103-encoding.json",
    "../testdata/encodings/gallery_140-encoding.json",
    "../testdata/encodings/probes_013-encoding.json",
    "../testdata/encodings/probes_016-encoding.json",
    "../testdata/encodings/probes_010-encoding.json",
    "../testdata/encodings/probes_006-encoding.json"
};

void testeasyeye_compare_main_multi() {
    vector<string> args;
    args.push_back("easyeye-compare");
    const int nencodings = 9;
    for (int i = 0; i < nencodings; i++) {
        args.push_back(serialized_encodings[i]);
    }
    Compare program;
    int rv = program.Main(args);
    if (rv != 0) {
        std::cout << "%TEST_FAILED% time=0 testname=testeasyeye_compare_main_multi (easyeye_compare_main_Test) message=exit status " << rv << ' ' << easyeye::Result::DescribeStatus(rv) << endl;
    } else {
        std::cout << "testeasyeye_compare_main_multi completed successfully (exit status 0)" << std::endl;
    }
}


void testeasyeye_compare_main() {
//    mylog::SetLogLevel(mylog::ALL);
    vector<string> args;
    args.push_back("easyeye-compare");
    args.push_back(serialized_encodings[0]);
    args.push_back(serialized_encodings[1]);
    const int argc =args.size();
    for (int i = 0; i < argc; i++) {
        std::cout << "    arg[" << i << "] = " << args[i] << std::endl;
    }
    Compare program;
    int rv = program.Main(args);
    if (rv != 0) {
        cout << "%TEST_FAILED% time=0 testname=testeasyeye_compare_main (easyeye_compare_main_Test) message=exit status " << rv << ' ' << easyeye::program::Program::DescribeCode(rv) << endl;
    } else {
        cout << "testeasyeye_compare_main completed successfully (exit status 0)" << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% main_test" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testeasyeye_compare_main (main_test)" << std::endl;
    cerr << "[start log for testeasyeye_compare_main]" << endl;
    testeasyeye_compare_main();
    cerr << "[end log for testeasyeye_compare_main]" << endl;
    std::cout << "%TEST_FINISHED% time=0 testeasyeye_compare_main (main_test)" << std::endl;

    std::cout << "%TEST_STARTED% testeasyeye_compare_main_multi (main_test)" << std::endl;
    cerr << "[start log for testeasyeye_compare_main_multi]" << endl;
    testeasyeye_compare_main_multi();
    cerr << "[end log for testeasyeye_compare_main_multi]" << endl;
    std::cout << "%TEST_FINISHED% time=0 testeasyeye_compare_main_multi (main_test)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

