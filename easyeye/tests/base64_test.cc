/* 
 * File:   base64_test.cc
 * Author: mchaberski
 *
 * Created on Jul 18, 2013, 5:48:45 PM
 */

#include <string>
#include <iostream>
#include "../src/easyeye/common/base64.h"
#include "../src/easyeye/common/easyeye_utils.h"
#include "../src/easyeye/common/easyeye_serial.h"
#include "testutils_base64.h"
#include "BitPackingTest.h"

using namespace std;
using namespace easyeye;

void maybeFail(int expected, unsigned char actual)
{
    std::cout << "check: expected " << expected << " got " << int(actual) << std::endl;
    if (expected != int(actual)) {
        std::cout << "%TEST_FAILED% time=0 testname=testToBytes_int (base64_test) message=incorrect output " << int(actual) << "(expected " << expected << ")" << std::endl;
    }
}

void testToBytes_int() {
    std::cout << "base64_test testToBytes_int" << std::endl;
    int n0[] = {0, 0, 0, 0, 0, 0, 0, 0};
    int n8[] = {0, 0, 0, 1, 0, 0, 0, 0};
    int n16[] = {0, 0, 0, 0, 1, 0, 0, 0};
    int n45[] = {1, 0, 1, 1, 0, 1, 0, 0};
    int n128[] = {0, 0, 0, 0, 0, 0, 0, 1};
    int n255[] = {1, 1, 1, 1, 1, 1, 1, 1};
    
    unsigned char dst[1];
    BitPacking::Pack(n0, 8, dst);
    maybeFail(0, dst[0]);
    BitPacking::Pack(n8, 8, dst);
    maybeFail(8, dst[0]);
    BitPacking::Pack(n16, 8, dst);
    maybeFail(16, dst[0]);
    BitPacking::Pack(n45, 8, dst);
    maybeFail(45, dst[0]);
    BitPacking::Pack(n128, 8, dst);
    maybeFail(128, dst[0]);
    BitPacking::Pack(n255, 8, dst);
    maybeFail(255, dst[0]);
    
    
}

void printBytes(unsigned char* inbytes, const int nbytes) 
{
    for (int i = 0; i < nbytes; i++) {
        std::cout << int(inbytes[i]) << ' ';
    }
}

void testEncodeIntArray() {
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "base64_test testEncodeIntArray" << std::endl;
////    std::cout << "%TEST_FAILED% time=0 testname=testEncodeIntArray (base64_test) message=error message sample" << std::endl;
//    int nums[] = {
//        0, 0, 0, 0, 0, 0, 0, 0,
//        1, 0, 0, 0, 0, 0, 0, 0,
//        0, 1, 0, 0, 0, 0, 0, 0,
//        1, 1, 0, 0, 0, 0, 0, 0,
//        0, 0, 1, 0, 0, 0, 0, 0,
//        1, 0, 1, 0, 0, 0, 0, 0,
//        1, 1, 1, 0, 0, 0, 0, 0,
//        0, 0, 0, 1, 0, 0, 0, 0
//    };
//    const int nnums = 64;
//    for (int i = 0; i < nnums; i++) {
//        std::cout << (nums[i] == 0 ? '0' : '1');
//    }
//    std::cout << std::endl;
//    
//    unsigned char* inbytes = (unsigned char*) malloc(sizeof(unsigned char) * (nnums / 8));
//    BitPacking::Pack(nums, nnums, inbytes);
//    int nbytes = nnums / 8;
//    printBytes(inbytes, nbytes);
//    std::string encoded = Base64::Encode(inbytes, nbytes);
//    std::cout << std::endl;
//    std::cout << "in base64 = " << encoded << std::endl;
//    
//    free(inbytes);
//    
//    unsigned char* outbytes = (unsigned char*) malloc(sizeof(unsigned char) * (nnums / 8));
//    std::string decoded = Base64::DecodeToString(encoded);
//    Base64::ToBytes(decoded, outbytes);
//    printBytes(outbytes, nbytes);
//    std::cout << std::endl;
//    free(outbytes);
    
}

void testBitPackingWithBase64()
{
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
    
    Base64 codec;
    string b64encoded = codec.Encode(bytes, num_bytes);
    
    string byteStrDecodedFromB64 = codec.DecodeToString(b64encoded);
    
    unsigned char* bytesDecodedFromB64 = (unsigned char*) byteStrDecodedFromB64.c_str();
    
    int num_bytes_from_B64 = byteStrDecodedFromB64.length();
    easyeye::BitPacking::Unpack(bytesDecodedFromB64, num_bytes_from_B64, decoded_ints);
    
    bool decode_success = TestUtils::IsEqual("ints", ints, decoded_ints, num_ints);
    
    if (!decode_success) {
        std::cout << "%TEST_FAILED% time=0 testname=testBitPackingWithBase64 (base64_test) message=no decode success" << std::endl;        
    }
    
    cout << "============================= PACKING TEST" << endl;
    
    int input[] = { 
    0, 1, 0, 0, 0, 0, 1, 0, 
    1, 1, 1, 1, 1, 1, 1, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 
    1, 0, 0, 0, 0, 0, 1, 1, 
    1, 1, 0, 0, 1, 1, 1, 1, 
    1, 0, 1, 0, 0, 0, 0, 0, 
    1, 1, 1, 0, 1, 1, 1, 0, 
    0, 1, 0, 0, 1, 0, 0, 0};
    string input_encoded("Qn8QwfMFdxIYwqaYZLFIwCgMtadPUMm2lNBWjSmtwtcNbcfl9pqW/T7OBup4/XTvyNr6TJOjj3yAQXZYxNs5eDV0P/8708yJEDtjrdBxmYVtOhZKfPL2wfdMnxkujLZLZxmxbfReCxvrww8REcS77PXEtmntLl56+fvAxVjjj/ZqcFKkvW2jsVeb12MIpJqrAINY/PCHa9K4tOtS6MbGKOQOGYD+a1ey7GPEVZ/07YXS7OpPKvuHMG8iMyzz/6FP");
//TRACE Encoding::Serialize irisMask [0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0...]
//TRACE Encoding::Serialize irisMask = FIgDax/aPU/DGuOiLvlHnEXxmwaDYyALivYJ51Q7IuT0rxDZHB4seFTBB+f9B2UG9QQB6ZtDpXcoe3EgbndnKUngiY51kmu0knWdimlKNyM/fVUa77+k35ow6gqvoPSFzy+rrwAEwX7lllynWwm6dHz/JOKb9oHCfE37scMdN7wwF6fFpUMwyrc8QrwUecpKsvdAYtt7Tv6MhFbaylFT4XY3kA+bdq0kxTzxZDMDKsLnNmlDR6c0OTtd18GMxzDG...
//TRACE Encoding::Deserialize irisTemplate = Qn8QwfMFdxIYwqaYZLFIwCgMtadPUMm2lNBWjSmtwtcNbcfl9pqW/T7OBup4/XTvyNr6TJOjj3yAQXZYxNs5eDV0P/8708yJEDtjrdBxmYVtOhZKfPL2wfdMnxkujLZLZxmxbfReCxvrww8REcS77PXEtmntLl56+fvAxVjjj/ZqcFKkvW2jsVeb12MIpJqrAINY/PCHa9K4tOtS6MbGKOQOGYD+a1ey7GPEVZ/07YXS7OpPKvuHMG8iMyzz/6FP...
//TRACE Encoding::Deserialize irisTemplate [0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0...]
//TRACE Encoding::Deserialize irisMask = FIgDax/aPU/DGuOiLvlHnEXxmwaDYyALivYJ51Q7IuT0rxDZHB4seFTBB+f9B2UG9QQB6ZtDpXcoe3EgbndnKUngiY51kmu0knWdimlKNyM/fVUa77+k35ow6gqvoPSFzy+rrwAEwX7lllynWwm6dHz/JOKb9oHCfE37scMdN7wwF6fFpUMwyrc8QrwUecpKsvdAYtt7Tv6MhFbaylFT4XY3kA+bdq0kxTzxZDMDKsLnNmlDR6c0OTtd18GMxzDG...
//TRACE Encoding::Deserialize irisMask [0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0
    for (int i = 0; i < 64; i++) {
        cout << input[i] << ' ';
    }
    cout << endl;
    unsigned char input_as_bytes[8];
    BitPacking::Pack(input, 64, input_as_bytes);
    string input_as_base64 = codec.Encode(input_as_bytes, 8);
    cout << "input_encoded = " << input_encoded << endl;
    cout << "input_as_base64 = " << input_as_base64 << endl;
    unsigned char* input_as_base64_packed = (unsigned char*) codec.DecodeToString(input_as_base64).c_str();
    int input_as_base64_unpacked[64];
    BitPacking::Unpack(input_as_base64_packed, 8, input_as_base64_unpacked);
    for (int i = 0; i < 64; i++) {
        cout << input_as_base64_unpacked[i] << ' ';
    }
    cout << endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% base64_test" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testToBytes_int (base64_test)" << std::endl;
    testToBytes_int();
    std::cout << "%TEST_FINISHED% time=0 test1 (base64_test)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (base64_test)\n" << std::endl;
    testEncodeIntArray();
    std::cout << "%TEST_FINISHED% time=0 test2 (base64_test)" << std::endl;

    std::cout << "%TEST_STARTED% testBitPackingWithBase64 (base64_test)\n" << std::endl;
    testBitPackingWithBase64();
    std::cout << "%TEST_FINISHED% time=0 testBitPackingWithBase64 (base64_test)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

