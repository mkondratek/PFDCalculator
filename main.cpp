//
// Created by mkondratek on 29.10.17.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

/*
 * todo list
 * SOLEVer system of linear equation
 * roots finder (?)
 * fast polynomial multiplication
 * set tostring type in cff and poly
 * complex (?)
 */

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);

    srand(time(NULL));

    return RUN_ALL_TESTS();
}