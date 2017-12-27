////
//// Created by mkondratek on 26.11.17.
////
//
//#ifndef PFDCALCULATOR_POLY_H
//#define PFDCALCULATOR_POLY_H
//
//#include <vector>
//#include "Coeff.h"
//
//
///** declaration begin **/
//template<typename Number>
//class Poly;
//
//template<typename Number>
//std::ostream &operator<<(std::ostream &stream, Poly<Number> const &Poly);
//template<typename Number>
//std::istream &operator>>(std::istream &stream, Poly<Number> &Poly);
//template<typename Number>
//Poly<Number> operator+(Poly<Number> const &a, Poly<Number> const &b);
//template<typename Number>
//Poly<Number> operator-(Poly<Number> const &a, Poly<Number> const &b);
//template<typename Number>
//Poly<Number> operator*(Poly<Number> const &a, Poly<Number> const &b);
////template<typename Number>
////Poly<Number> operator/(Poly<Number> const &a, Poly<Number> const &b);
///**  declaration end  **/
//
//template <typename Number>
//class Poly {
//
//public:
//
//    using String = std::string;
//    using Cff    = Coeff<Number>;
//    using Numvec = std::vector<Number>;
//    using Cffvec = std::vector<Cff>;
//
//    /* DISPLAY PARAMS */
//    enum { ASC = 32, DESC = 64, ROOTS = 128, EXT = 256 };
//
//private:
//
//    Cffvec m_monomials;
//    Numvec m_roots;
//
//    bool m_roots_valid;
//    Poly<Number> &updateRoots();
//
//public:
//
//    static Poly parse(String str);
//
//    Poly() noexcept;
//    Poly(Number number, unsigned int exp) noexcept ;
//    Poly(Cff const& cff, unsigned int exp) noexcept ;
//
//    explicit Poly(Number const& number) noexcept;
//    explicit Poly(Cff const& cff) noexcept ;
//    explicit Poly(Numvec const& roots) noexcept ;
//
//    Poly<Number>(Poly<Number> const& poly) noexcept;
//    Poly<Number>(Poly<Number>&& poly) noexcept;
//
//    Poly<Number> &operator=(Poly<Number> const& poly) noexcept;
//    Poly<Number> &operator=(Poly<Number>&& poly) noexcept;
//
//    friend Poly<Number> operator+<Number>(Poly<Number> const &a, Poly<Number> const &b);
//    friend Poly<Number> operator-<Number>(Poly<Number> const &a, Poly<Number> const &b);
//    friend Poly<Number> operator*<Number>(Poly<Number> const &a, Poly<Number> const &b);
//    friend Poly<Number> operator/<Number>(Poly<Number> const &a, Poly<Number> const &b);
//
//    Poly<Number> &operator+=(Poly<Number> const &a);
//    Poly<Number> &operator-=(Poly<Number> const &a);
//    Poly<Number> &operator*=(Poly<Number> const &a);
////    Poly<Number> &operator/=(Poly<Number> const &a);
//
//    Poly<Number> operator+() const;
//    Poly<Number> operator-() const;
//
//    Cffvec getCoeffs() const;
//    Numvec getRoots() const;
//
//    Poly &apply(Cff::String variable, Number value);
//    Poly &apply(Cff::MapOfV values);
//
//    Number calculate(Number number) const;
//    Number calculate(Number number, Cff::MapOfV values) const;
//
//    Cff getCoeff(unsigned int exp) const;
//
//    unsigned int getDegree() const;
//
//    String toString(int type = (Cff::TIGHT | DESC)) const;
//
//    friend std::ostream &operator<< <Number>(std::ostream &stream, Poly<Number> const &poly);
//
//    friend std::istream &operator>> <Number>(std::istream &stream, Poly<Number> &poly);
//};
//
//template <typename Number>
//Poly<Number> operator+(Poly<Number> const &a, Poly<Number> const &b) {
//    Poly<Number> result(a);
//    result.m_monomials.resize(std::max(a.getDegree(), b.getDegree()), Cff(Number(0)));
//    for (unsigned int i = 0; i < b.getDegree(); ++i) {
//        result.m_monomials[i] += b.m_monomials[i];
//    }
//
//    return result;
//}
//
//template <typename Number>
//Poly<Number> operator-(Poly<Number> const &a, Poly<Number> const &b) {
//    return a + (-b);
//}
//
//template <typename Number>
//Poly<Number> operator*(Poly<Number> const &a, Poly<Number> const &b) {
//    Poly<Number> result;
//    result.m_monomials.resize(a.getDegree() * b.getDegree(), Cff(Number(0)));
//    for (int i = 0; i < a.getDegree(); ++i) {
//        for (int j = 0; j < b.getDegree(); ++j) {
//            result.m_monomials[i * j] = a.m_monomials[i] * b.m_monomials[j];
//        }
//    }
//
//    return result;
//}
//
////template <typename Number>
////Poly<Number> operator/(Poly<Number> const &a, Poly<Number> const &b) {
////
////}
//
//template <typename Number>
//std::ostream &operator<<(std::ostream &stream, Poly<Number> const &poly) {
//    stream << poly.toString();
//    return stream;
//}
//
//template <typename Number>
//std::istream &operator>>(std::istream &stream, Poly<Number> const &poly) {
//    Poly<Number>::String str;
//    stream >> str;
//    poly = Poly<Number>::parse(str);
//    return stream;
//}
//
//
//#endif //PFDCALCULATOR_POLY_H
