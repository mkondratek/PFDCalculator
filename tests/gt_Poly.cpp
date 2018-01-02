//
// Created by mkondratek on 03.11.17.
//

#ifndef PFDCALCULATOR_GT_COEFF_H
#define PFDCALCULATOR_GT_COEFF_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../Poly.h"

using testing::Eq;

namespace {
    class gt_Poly : public testing::Test {
    public:
        using Pi = Poly<int>;
        using Pf = Poly<float>;
    };
}

TEST_F(gt_Poly, cstr_default) {
    Pi a;

    ASSERT_EQ(a.toString(), "0");
}

TEST_F(gt_Poly, cstr_number) {
    Pi a(0);
    Pi b(1);
    Pi c(-1);

    ASSERT_EQ(a.toString(), "0");
    ASSERT_EQ(b.toString(), "1");
    ASSERT_EQ(c.toString(), "-1");
}

TEST_F(gt_Poly, cstr_number_exp_0) {
    Pi a(1, 0);
    Pi b(2, 0);
    Pi c(10, 2);
    Pi d(-1, 3);

    ASSERT_EQ(a.toString(), "1");
    ASSERT_EQ(b.toString(), "2");
    ASSERT_EQ(c.toString(), "10x^2");
    ASSERT_EQ(d.toString(), "-x^3");
}

TEST_F(gt_Poly, cstr_number_exp_1) {
    Pi a(0, 0);
    Pi b(0, 1);
    Pi c(1, 1);
    Pi d(-2, 1);

    ASSERT_EQ(a.toString(), "0");
    ASSERT_EQ(b.toString(), "0");
    ASSERT_EQ(c.toString(), "x");
    ASSERT_EQ(d.toString(), "-2x");
}

TEST_F(gt_Poly, cstr_number_exp_2) {
    Pi a(1, 0);
    Pi b(2, 0);
    Pi c(10, 2);
    Pi d(-1, 3);

    ASSERT_EQ(a.toString(), "1");
    ASSERT_EQ(b.toString(), "2");
    ASSERT_EQ(c.toString(), "10x^2");
    ASSERT_EQ(d.toString(), "-x^3");
}

TEST_F(gt_Poly, cstr_number_exp_3) {
    Pi a(0, 0);
    Pi b(0, 1);

    ASSERT_EQ(a.toString(), "0");
    ASSERT_EQ(b.toString(), "0");
}

TEST_F(gt_Poly, cstr_cff) {
    Pi a(Pi::Cff(0));
    Pi b(Pi::Cff(4, "b"));
    Pi c(Pi::Cff(-4, "a"));
    Pi d(Pi::Cff::parse("3a(3c+g)+c(3d+a)"));

    ASSERT_EQ(a.toString(), "0");
    ASSERT_EQ(b.toString(), "4b");
    ASSERT_EQ(c.toString(), "-4a");
    ASSERT_EQ(d.toString(), "(3a(3c+g)+c(a+3d))");
}

TEST_F(gt_Poly, cstr_cff_exp_0) {
    Pi a(Pi::Cff(0), 10);
    Pi b(Pi::Cff(4, "b"), 3);
    Pi c(Pi::Cff(-4, "a"), 0);
    Pi d(Pi::Cff::parse("a(a+2)"), 2);

    ASSERT_EQ(a.toString(), "0");
    ASSERT_EQ(b.toString(), "4bx^3");
    ASSERT_EQ(c.toString(), "-4a");
    ASSERT_EQ(d.toString(), "a(2+a)x^2");
}

TEST_F(gt_Poly, cstr_cff_exp_1) {
    Pi a(Pi::Cff(0), 10);
    Pi b(Pi::Cff(4, "b"), 3);
    Pi c(Pi::Cff(-4, "a"), 0);
    Pi d(Pi::Cff::parse("a(a+2)"), 2);

    ASSERT_EQ(a.toString(), "0");
    ASSERT_EQ(b.toString(), "4bx^3");
    ASSERT_EQ(c.toString(), "-4a");
    ASSERT_EQ(d.toString(), "a(2+a)x^2");
}

TEST_F(gt_Poly, cstr_roots_0) {
    Pi a(Pi::Numvec{10});
    Pi b(Pi::Numvec{-7});
    Pi c(Pi::Numvec{0, 0, 1, 1, 1, 2, -2});
    Pi d(Pi::Numvec{});

    ASSERT_EQ(a.toString(), "x-10");
    ASSERT_EQ(b.toString(), "x+7");
    ASSERT_EQ(c.toString(), "x^7-3x^6-x^5+11x^4-12x^3+4x^2");
    ASSERT_EQ(d.toString(), "0");
}


TEST_F(gt_Poly, cstr_roots_1) {
    Pi a(Pi::Numvec{0});
    Pi b(Pi::Numvec{0, 0});
    Pi c(Pi::Numvec{0, 0, 1});
    Pi d(Pi::Numvec{-1, 1});

    ASSERT_EQ(a.toString(), "x");
    ASSERT_EQ(b.toString(), "x^2");
    ASSERT_EQ(c.toString(), "x^3-x^2");
    ASSERT_EQ(d.toString(), "x^2-1");
}

TEST_F(gt_Poly, cstr_cp) {
    Pi a(a);
    Pi b(a);

    ASSERT_EQ(a.toString(), "0");
    ASSERT_EQ(b.toString(), "0");
}

TEST_F(gt_Poly, cstr_mv) {
    Pi a(std::move(a));
    Pi b;
    Pi c(std::move(b));

    ASSERT_EQ(a.toString(), "0");
    ASSERT_EQ(c.toString(), "0");
}

TEST_F(gt_Poly, assign_cp) {
    Pi a = a;
    Pi b;
    b = a;

    ASSERT_EQ(a.toString(), "0");
    ASSERT_EQ(b.toString(), "0");
}

TEST_F(gt_Poly, assign_mv) {
    Pi a = a;
    Pi b;
    Pi c;
    c = std::move(b);

    ASSERT_EQ(a.toString(), "0");
    ASSERT_EQ(c.toString(), "0");
}

TEST_F(gt_Poly, op_plus_0) {
    Pi a(0);
    for (int i = 0; i < 10; ++i) {
        a += Pi(1, i);
    }

    for (int i = 0; i < 6; ++i) {
        a -= Pi(1, i);
    }

    Pi b(0);
    for (int i = 0; i < 10; ++i) {
        b += Pi(i*i, 2*i);
    }

    for (int i = 0; i < 6; ++i) {
        b -= Pi(1 - i, i);
    }

    ASSERT_EQ(a.toString(), "x^9+x^8+x^7+x^6");
    ASSERT_EQ(b.toString(), "81x^18+64x^16+49x^14+36x^12+25x^10+16x^8+9x^6+4x^5+7x^4+2x^3+2x^2-1");
}

TEST_F(gt_Poly, op_plus_1) {
    Pi a(0);
    for (int i = 0; i < 10; ++i) {
        a += Pi(Pi::Cff("a"), i);
    }

    for (int i = 0; i < 6; ++i) {
        a -= Pi(Pi::Cff("b"), i);
    }

    Pi b(Pi::Cff("c"));
//    for (int i = 0; i < 10; ++i) {
//        b += Pi(Pi::Cff(i*i, "a"), 2*i);
//    }
//
//    for (int i = 0; i < 6; ++i) {
//        b -= Pi(Pi::Cff(-i, "b"), i);
//    }

    ASSERT_EQ(a.toString(), "x^9+x^8+x^7+x^6");
    ASSERT_EQ(b.toString(), "81x^18+64x^16+49x^14+36x^12+25x^10+16x^8+9x^6+4x^5+7x^4+2x^3+2x^2-1");
}

//TEST_F(gt_Poly, op_mul_0) {
//    Pi a(1);
//
//    for (int i = 0; i < 5; ++i) {
//        a *= Pi(Pi::Numvec{-1, 1});
//    }
//
//    ASSERT_EQ(a.toString(), "x^10-5x^8+10x^6-10x^4+5x^2-1");
//}
//
//TEST_F(gt_Poly, op_mul_1) {
//    Pi a(1);
//
//    for (int i = 0; i < 5; ++i) {
//        a *= Pi(Pi::Cffvec{Pi::Cff::parse("-a"), Pi::Cff::parse("b")});
//    }
//
//    ASSERT_EQ(a.toString(), "x^10-5x^8+10x^6-10x^4+5x^2-1");
//}

#endif //PFDCALCULATOR_GT_COEFF_H
