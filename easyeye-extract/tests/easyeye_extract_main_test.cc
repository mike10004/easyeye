/* 
 * File:   main_test.cc
 * Author: mchaberski
 *
 * Created on Jul 16, 2013, 2:07:47 PM
 */

#include <stdlib.h>
#include <iostream>
#include <easyeye/common/easyeye_utils.h>
#include <easyeye_extract_main.h>

using easyeye::extract::Extract;

void testExtractAndWriteUnwritable() {
    std::cout << "easyeye_extract_main_test testExtractAndWriteUnwritable" << std::endl;
    char* argv[] = {
        (char*) "./easyeye-extract",
        (char*) "../testdata/images/gallery_022.bmp",
        (char*) "/tmp" // this is a directory -- we'll fail to open it for writing
    };
    int argc = 3;
    Extract program;
    int rv = program.Main(argc, argv);
    std::cout << "main returned " << rv << std::endl;
    if (rv != easyeye::program::kErrorIO) {
        std::cout << "%TEST_FAILED% time=0 testname=testExtractAndWriteUnwritable (easyeye_extract_main_test) message=failed with nonzero error code " << rv << std::endl;
    } else {
        std::cout << "easyeye_extract_main test succeeded" << std::endl;
    }
}


void testExtractAndWrite() {
    std::cout << "easyeye_extract_main_test testExtractAndWrite" << std::endl;
    char* argv[] = {
        (char*) "./easyeye-extract",
        (char*) "../testdata/images/gallery_022.bmp",
        (char*) "/tmp/easyeye-extract-test-encoding.json"
    };
    int argc = 3;
    Extract program;
    int rv = program.Main(argc, argv);
    std::cout << "main returned " << rv << std::endl;
    bool is_file = easyeye::IOUtils::is_file("/tmp/easyeye-extract-test-encoding.json");
    if (rv != 0 || !is_file) {
        std::cout << "%TEST_FAILED% time=0 testname=testExtractAndWrite (easyeye_extract_main_test) message=failed with nonzero error code " << rv << std::endl;
    } else {
        std::cout << "easyeye_extract_main test succeeded" << std::endl;
    }
}

void testExtractAndWriteNull() {
    std::cout << "easyeye_extract_main_test testExtractAndWriteNull" << std::endl;
    char* argv[] = {
        (char*) "./easyeye-extract",
        (char*) "../testdata/images/gallery_022.bmp",
        (char*) "/dev/null"
    };
    int argc = 3;
    Extract program;
    int rv = program.Main(argc, argv);
    std::cout << "main returned " << rv << std::endl;
    if (rv != 0) {
        std::cout << "%TEST_FAILED% time=0 testname=testExtractAndWriteNull (easyeye_extract_main_test) message=failed with nonzero error code " << rv << std::endl;
    } else {
        std::cout << "easyeye_extract_main test succeeded" << std::endl;
    }
}

void testExtractAndDoNotWrite() {
    std::cout << "easyeye_extract_main_test testExtractAndDoNotWrite" << std::endl;
    char* arg0 = (char*) "./easyeye-extract";
    char* arg1 = (char*) "../testdata/images/gallery_022.bmp";
    char* argv[2];
    argv[0] = arg0;//"./easyeye-extract";
    argv[1] = arg1;//"tests/images/011.bmp";
    int argc = 2;
    Extract program;
    int rv = program.Main(argc, argv);
    std::cout << "main returned " << rv << std::endl;
    if (rv != 0) {
        std::cout << "%TEST_FAILED% time=0 testname=testExtractAndDoNotWrite (easyeye_extract_main_test) message=failed with nonzero error code " << rv << std::endl;
    } else {
        std::cout << "easyeye_extract_main test succeeded" << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% main_test" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testExtractAndDoNotWrite (easyeye_extract_main_test)" << std::endl;
    testExtractAndDoNotWrite();
    std::cout << "%TEST_FINISHED% time=0 testExtractAndDoNotWrite (easyeye_extract_main_test)" << std::endl;

    std::cout << "%TEST_STARTED% testExtractAndWriteNull (easyeye_extract_main_test)" << std::endl;
    testExtractAndWriteNull();
    std::cout << "%TEST_FINISHED% time=0 testExtractAndWriteNull (easyeye_extract_main_test)" << std::endl;

    std::cout << "%TEST_STARTED% testExtractAndWrite (easyeye_extract_main_test)" << std::endl;
    testExtractAndWrite();
    std::cout << "%TEST_FINISHED% time=0 testExtractAndWrite (easyeye_extract_main_test)" << std::endl;

    std::cout << "%TEST_STARTED% testExtractAndWriteUnwritable (easyeye_extract_main_test)" << std::endl;
    testExtractAndWriteUnwritable();
    std::cout << "%TEST_FINISHED% time=0 testExtractAndWriteUnwritable (easyeye_extract_main_test)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}
