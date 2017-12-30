//
// Created by mkondratek on 29.10.17.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

/*
 * todo list
 * poly of roots vector
 * SOLEVer system of linear equation
 * roots finder (?)
 * throwing exception (and test it)
 * parsing error
 * fast polynomial multiplication
 * set tostring type in cff and poly
 * tests for poly !!!
 */

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);

    srand(time(NULL));

    return RUN_ALL_TESTS();
}