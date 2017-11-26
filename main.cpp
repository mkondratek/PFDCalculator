//
// Created by mkondratek on 29.10.17.
//

#include <gtest/gtest.h>
#include <gmock/gmock.h>

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);

    srand(time(NULL));

    return RUN_ALL_TESTS();
}