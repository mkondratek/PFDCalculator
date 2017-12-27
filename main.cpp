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
 * keep roots sorted
 * set tostring type in cff and
 */

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);

    srand(time(NULL));

    return RUN_ALL_TESTS();
}