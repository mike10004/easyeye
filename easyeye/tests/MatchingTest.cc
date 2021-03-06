/*
 * File:   MatchingTest.cc
 * Author: ibgmike
 *
 * Created on Jan 26, 2014, 4:13:00 PM
 */

#include <iostream>
#include <fstream>
#include "../src/easyeye/common/mylog.h"
#include "../src/easyeye/encode/easyeye_encode.h"
#include "../src/easyeye/match/easyeye_match.h"
#include "MatchingTest.h"
#include "testdata.h"

using namespace easyeye;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(MatchingTest);

MatchingTest::MatchingTest() {
}

MatchingTest::~MatchingTest() {
}

void MatchingTest::setUp() {
}

void MatchingTest::tearDown() {
}

void MatchingTest::testComputeScoresBasic() {
    mylog::Logs::GetLogger().set_level(mylog::ALL);
    const int nencodings = NUM_SAMPLES;
    Encoding encodings[nencodings];
    for (int i = 0; i < nencodings;i++) {
        string json = Files::Read(serialized_encodings[i]);
        cerr << serialized_encodings[i] << " opened: " << json.length() << " chars" << endl;
        serial::Deserialize(json, encodings[i]);
    }
    
    Matcher matcher;
    for (int i = 0; i < nencodings; i++) {
        for (int j = i + 1; j < nencodings; j++) {
            Encoding& g = encodings[i];
            Encoding& p = encodings[j];
            MatchInfo match_info;
            double score = matcher.ComputeScore(g, p, match_info);
            CPPUNIT_ASSERT(match_info.flag == MatchInfo::CLEAN);
            cerr << score << '\t' << serialized_encodings[i] << '\t' << serialized_encodings[j] << endl;
        }
    }
}

