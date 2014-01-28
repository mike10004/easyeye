/*
 * File:   BitPackingTest.cc
 * Author: mchaberski
 *
 * Created on Jan 25, 2014, 8:42:13 PM
 */

#include "../src/easyeye/common/base64.h"
#include "BitPackingTest.h"
#include <string>
#include "../src/easyeye/common/easyeye_utils.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "testutils_bitpacking.h"

using namespace easyeye;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(BitPackingTest);

BitPackingTest::BitPackingTest() {
}

BitPackingTest::~BitPackingTest() {
}

void BitPackingTest::setUp() {
}

void BitPackingTest::tearDown() {
}





void BitPackingTest::testBitPackingStatic() {
    
    int ints[] = {
            0, 1, 0, 0, 0, 0, 1, 0, 
            0, 1, 0, 0, 0, 1, 0, 1, 
            0, 1, 0, 0, 1, 0, 0, 0, 
            1, 0, 0, 0, 0, 1, 0, 0, 
            1, 0, 0, 0, 1, 0, 0, 1, 
            0, 0, 1, 0, 1, 0, 0, 1, 
            0, 0, 1, 0, 1, 1, 1, 1, 
            1, 1, 1, 1, 1, 1, 0, 0, 
            1, 0, 1, 0, 1, 0, 1, 0, 
            0, 1, 0, 0, 1, 0, 0, 0, 
            0, 1, 0, 0, 0, 0, 1, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 0, 
            0, 0, 0, 0, 0, 0, 0, 1, 
            0, 1, 1, 0, 0, 1, 0, 1, 
            0, 0, 0, 0, 1, 0, 1, 1
        };
    const size_t num_ints = 16 * 8;
    const size_t num_bytes = num_ints / 8;    
    unsigned char bytes[num_bytes];
    int decoded_ints[num_bytes * 8];
    
    easyeye::BitPacking::Pack(ints, num_ints, bytes);
    easyeye::BitPacking::Unpack(bytes, num_bytes, decoded_ints);
    
    bool decode_success = TestUtils::IsEqual("ints", ints, decoded_ints, num_ints);
    
    CPPUNIT_ASSERT(decode_success);
    
}

void BitPackingTest::testBitPackingGenerated() {
    /* initialize random seed: */
    srand (time(NULL));
    int num_trials_per_len = 100;
    int len_min = 0, len_max = 9600 * 2, len_step = 8 * 100; // step must be divisible by 8
    
    for (int len = len_min; len <= len_max; len += len_step) {
        cerr << "generated array of length " << len << endl;
        for (int trial = 0; trial < num_trials_per_len; trial++) {
            CPPUNIT_ASSERT(len % 8 == 0);
            int* ints = new int[len];//(int*) malloc(sizeof(int) * len);
            int byte_len = len / 8;
            unsigned char* bytes = new unsigned char[byte_len];//(unsigned char*) malloc(sizeof(unsigned char) * num_bytes);
            int* decoded_ints = new int[len];//(int*) malloc(sizeof(int) * len);
            CPPUNIT_ASSERT(ints != NULL && decoded_ints != NULL && bytes != NULL);
            for (int i = 0; i < len; i++) {
                ints[i] = rand() % 2;
            }
            easyeye::BitPacking::Pack(ints, len, bytes);
            easyeye::BitPacking::Unpack(bytes, byte_len, decoded_ints);

            bool decode_success = TestUtils::IsEqual("ints", ints, decoded_ints, len);

            delete ints;
            delete bytes;
            delete decoded_ints;
            CPPUNIT_ASSERT(decode_success);
        }
        cerr << "all arrays of length " << len << " decoded successfully" << endl;
    }
}

