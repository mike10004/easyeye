/* 
 * File:   main_outputs_test.cc
 * Author: mchaberski
 *
 * Created on Jul 16, 2013, 2:07:47 PM
 */

#include <cstdlib>
#include <iostream>
#include <easyeye/common/easyeye_utils.h>
#include <easyeye_extract_main.h>
#include <unistd.h>

using namespace std;
using easyeye::extract::Extract;
using namespace easyeye;

void testExtractWithDiagnostics() {
    cout << "easyeye_extract_main_test testExtractWithDiagnostics" << endl;
    const char* diags_dir = "/tmp/easyeye_extract_tests_diagnostics";
    string diag_file;
    diag_file.append(diags_dir);
    diag_file.append("/");
    diag_file.append("gallery_022-1-segmentation.png");
    unlink(diag_file.c_str());
    char* argv[] = {
        (char*) "./easyeye-extract",
        (char*) "--diagnostics",
        (char*) diags_dir,  // - = stdout
        (char*) "../testdata/images/gallery_022.bmp"
    };
    int argc = sizeof(argv) / sizeof(char*);
    cout << argc << " arguments to main" << endl;
    Extract program;
    int rv = program.Main(argc, argv);
    cout << "main returned " << rv << endl;
    bool failed = false;
    if (rv != 0) {
        cout << "%TEST_FAILED% time=0 testname=testExtractWithDiagnostics (main_outputs) message=failed with nonzero error code " << rv << endl;
        failed = true;
    } 
    if (!IOUtils::IsDirectory(diags_dir)) {
        cout << "%TEST_FAILED% time=0 testname=testExtractWithDiagnostics (main_outputs) message=diagnostics directory does not exist" << endl;
        failed = true;
    }
    if (!IOUtils::IsFile(diag_file)) {
        cout << "%TEST_FAILED% time=0 testname=testExtractWithDiagnostics (main_outputs) message=diagnostics file does not exist: " << diag_file << endl;
        failed = true;
    }
    else {
        cout << "easyeye_extract_main test succeeded" << endl;
    }
}


void testExtractWithCsvOutput() {
    cout << "easyeye_extract_main_test testExtractWithCsvOutput" << endl;
    char* argv[] = {
        (char*) "./easyeye-extract",
        (char*) "--csv",
        (char*) "-",  // - = stdout
        (char*) "../testdata/images/gallery_022.bmp"
    };
    int argc = sizeof(argv) / sizeof(char*);
    cout << argc << " arguments to main" << endl;
    Extract program;
    int rv = program.Main(argc, argv);
    cout << "main returned " << rv << endl;
    if (rv != 0) {
        cout << "%TEST_FAILED% time=0 testname=testExtractWithCsvOutput (main_outputs) message=failed with nonzero error code " << rv << endl;
    } else {
        cout << "easyeye_extract_main test succeeded" << endl;
    }
}

int main(int argc, char** argv) {
    cout << "%SUITE_STARTING% main_test" << endl;
    cout << "%SUITE_STARTED%" << endl;

    cout << "%TEST_STARTED% testExtractWithCsvOutput (main_outputs)" << endl;
    testExtractWithCsvOutput();
    cout << "%TEST_FINISHED% time=0 testExtractAndDoNotWrite (main_outputs)" << endl;

    cout << "%TEST_STARTED% testExtractWithDiagnostics (main_outputs)" << endl;
    testExtractWithDiagnostics();
    cout << "%TEST_FINISHED% time=0 testExtractWithDiagnostics (main_outputs)" << endl;

//    cout << "%TEST_STARTED% testExtractAndWriteNullWithCsvStdout (easyeye_extract_main_test)" << endl;
//    testExtractAndWriteNull();
//    cout << "%TEST_FINISHED% time=0 testExtractAndWriteNullWithCsvStdout (easyeye_extract_main_test)" << endl;
//
//    cout << "%TEST_STARTED% testExtractAndWrite (easyeye_extract_main_test)" << endl;
//    testExtractAndWrite();
//    cout << "%TEST_FINISHED% time=0 testExtractAndWrite (easyeye_extract_main_test)" << endl;
//
//    cout << "%TEST_STARTED% testExtractAndWriteUnwritable (easyeye_extract_main_test)" << endl;
//    testExtractAndWriteUnwritable();
//    cout << "%TEST_FINISHED% time=0 testExtractAndWriteUnwritable (easyeye_extract_main_test)" << endl;

    cout << "%SUITE_FINISHED% time=0" << endl;

    return (EXIT_SUCCESS);
}
