////
//// Created by mkondratek on 03.11.17.
////
//
//#ifndef PFDCALCULATOR_GT_COEFF_H
//#define PFDCALCULATOR_GT_COEFF_H
//
//#include <gtest/gtest.h>
//#include <gmock/gmock.h>
//#include "../Poly.h"
//
//using testing::Eq;
//
//namespace {
//    class gt_Poly : public testing::Test {
//    public:
//        using Pi = Poly<int>;
//        using Pf = Poly<float>;
//    };
//}
//
//TEST_F(gt_Poly, cstr_default) {
//    Pi a;
//
//    ASSERT_EQ(a.toString(), "1");
//}
//
//TEST_F(gt_Poly, cstr_number) {
//    Pi a(0);
//    Pi b(1);
//    Pi c(-1);
//
//    ASSERT_EQ(a.toString(), "0");
//    ASSERT_EQ(a.toString(), "1");
//    ASSERT_EQ(a.toString(), "-1");
//}
//
//TEST_F(gt_Poly, cstr_number_exp_0) {
//    Pi a(1, 0);
//    Pi b(2, 0);
//    Pi c(10, 2);
//    Pi d(-1, 3);
//
//    ASSERT_EQ(a.toString(), "1");
//    ASSERT_EQ(b.toString(), "2");
//    ASSERT_EQ(c.toString(), "10x^2");
//    ASSERT_EQ(d.toString(), "-x^3");
//}
//
//TEST_F(gt_Poly, cstr_number_exp_1) {
//    Pi a(0, 0);
//    Pi b(0, 1);
//
//    ASSERT_EQ(a.toString(), "0");
//    ASSERT_EQ(b.toString(), "0");
//}
//
//TEST_F(gt_Poly, cstr_number_exp_2) {
//    Pi a(1, 0);
//    Pi b(2, 0);
//    Pi c(10, 2);
//    Pi d(-1, 3);
//
//    ASSERT_EQ(a.toString(), "1");
//    ASSERT_EQ(b.toString(), "2");
//    ASSERT_EQ(c.toString(), "10x^2");
//    ASSERT_EQ(d.toString(), "-x^3");
//}
//
//TEST_F(gt_Poly, cstr_number_exp_3) {
//    Pi a(0, 0);
//    Pi b(0, 1);
//
//    ASSERT_EQ(a.toString(), "0");
//    ASSERT_EQ(b.toString(), "1");
//}
//
//TEST_F(gt_Poly, cstr_cff) {
//    Pi a(Pi::Cff(0));
//    Pi b(Pi::Cff(4, "b"));
//    Pi c(Pi::Cff(-4, "a"));
//    Pi d(Pi::Cff::parse("3a(3c+g)+c(3d+a)"));
//
//    ASSERT_EQ(a.toString(), "0");
//    ASSERT_EQ(d.toString(), "4b");
//    ASSERT_EQ(c.toString(), "-4a");
//    ASSERT_EQ(d.toString(), "3a(3c+g)+c(3d+a)");
//}
//
//TEST_F(gt_Poly, cstr_cff_exp_0) {
//    Pi a(Pi::Cff(0), 10);
//    Pi b(Pi::Cff(4, "b"), 3);
//    Pi c(Pi::Cff(-4, "a"), 0);
//    Pi d(Pi::Cff::parse("a(a+2)"), 2);
//
//    ASSERT_EQ(a.toString(), "0");
//    ASSERT_EQ(d.toString(), "4bx^3");
//    ASSERT_EQ(c.toString(), "-4a");
//    ASSERT_EQ(d.toString(), "a(a+2)x^2");
//}
//
//TEST_F(gt_Poly, cstr_cff_exp_1) {
//    Pi a(Pi::Cff(0), 10);
//    Pi b(Pi::Cff(4, "b"), 3);
//    Pi c(Pi::Cff(-4, "a"), 0);
//    Pi d(Pi::Cff::parse("a(a+2)"), 2);
//
//    ASSERT_EQ(a.toString(), "0");
//    ASSERT_EQ(d.toString(), "4bx^3");
//    ASSERT_EQ(c.toString(), "-4a");
//    ASSERT_EQ(d.toString(), "a(a+2)x^2");
//}
//
//TEST_F(gt_Poly, cstr_roots_1) {
//    Pi a(Pi::Numvec{});
//}
//
//TEST_F(gt_Poly, cstr_roots_0) {
//
//}
//
//#endif //PFDCALCULATOR_GT_COEFF_H
