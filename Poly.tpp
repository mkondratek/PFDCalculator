////
//// Created by mkondratek on 27.12.17.
////
//
//#include <algorithm>
//#include "Poly.h"
//
//template<typename Number>
//Poly<Number> &Poly::updateRoots() {
//    m_roots.clear();
//    return *this;
//}
//
//template<typename Number>
//Poly Poly::parse(Poly::String str) {
//    return Poly();
//}
//
//template<typename Number>
//Poly::Poly() {
//
//}
//
//template<typename Number>
//Poly::Poly(Number number) {
//
//}
//
//template<typename Number>
//Poly::Poly(Cff cff) {
//
//}
//
//template<typename Number>
//Poly::Poly(Number number, unsigned int exp, bool fill_using_ones) {
//
//}
//
//template<typename Number>
//Poly::Poly(Cff cff, unsigned int exp, bool fill_using_ones) {
//    if (cff.ready() && cff.calculate() == Number(0)) {
//        m_monomials.push_back(Number(0));
//    } else {
//
//    }
//}
//
//template<typename Number>
//Poly::Poly(Poly::Numvec roots) {
//    m_roots = roots;
//    std::sort(m_roots.begin(), m_roots.end());
//}
//
//template<typename Number>
//Poly::Poly(Poly<Number> const &poly) {
//    if (&poly == this) {
//        m_monomials.push_back(Cff(0));
//    } else {
//        m_monomials = poly.m_monomials;
//        m_roots = poly.m_roots;
//    }
//}
//
//template<typename Number>
//Poly::Poly(Poly<Number> &&poly) {
//    if (&poly == this) {
//        m_monomials.push_back(Cff(0));
//    } else {
//        m_monomials = std::move(poly.m_monomials);
//        m_roots = std::move(poly.m_roots);
//    }
//}
//
//template<typename Number>
//Poly<Number> &Poly::operator=(Poly<Number> const &poly) {
//    if (&poly != this) {
//        m_monomials = poly.m_monomials;
//        m_roots = poly.m_roots;
//    }
//    return *this;
//}
//
//template<typename Number>
//Poly<Number> &Poly::operator=(Poly<Number> &&poly) {
//    if (&poly != this) {
//        m_monomials = std::move(poly.m_monomials);
//        m_roots = std::move(poly.m_roots);
//    }
//    return *this;
//}
//
//template<typename Number>
//Poly<Number> &Poly::operator+=(Poly<Number> const &a) {
//    Poly tmp = *this + a;
//    *this = tmp;
//    return *this;
//}
//
//template<typename Number>
//Poly<Number> &Poly::operator-=(Poly<Number> const &a) {
//    Poly tmp = *this - a;
//    *this = tmp;
//    return *this;
//}
//
//template<typename Number>
//Poly<Number> &Poly::operator*=(Poly<Number> const &a) {
//    Poly tmp = *this * a;
//    *this = tmp;
//    return *this;
//}
//
//template<typename Number>
//Poly<Number> Poly::operator+() const {
//    return *this;
//}
//
//template<typename Number>
//Poly<Number> Poly::operator-() const {
//    Poly<Number> tmp(*this);
//    for (auto &c : tmp.m_monomials) {
//        c *= Number(-1);
//    }
//    return tmp;
//}
//
//template<typename Number>
//Poly::Cffvec Poly::getCoeffs() const {
//    return m_monomials;
//}
//
//template<typename Number>
//Poly::Numvec Poly::getRoots() const {
//    return m_roots;
//}
//
//template<typename Number>
//Poly &Poly::apply(Coeff::String variable, Number value) {
//    for (auto &c : m_monomials) {
//        c.apply(variable, value);
//    }
//
//    return *this;
//}
//
//template<typename Number>
//Poly &Poly::apply(Coeff::MapOfV values) {
//    for (auto &v : values) {
//        apply(v.first, v.second);
//    }
//
//    return *this;
//}
//
//template<typename Number>
//Number Poly::calculate(Number number) const {
//    return calculate(number, Cff::MapOfV());
//}
//
//template<typename Number>
//Number Poly::calculate(Number number, Coeff::MapOfV values) const {
//    Poly<Number> tmp(*this);
//
//    Number result(0);
//    for (unsigned int i = tmp.m_monomials.size() - 1; i >= 0; --i) {
//        result *= number;
//        result += tmp.m_monomials[i].calculate(values);
//    }
//
//    return result;
//}
//
//template<typename Number>
//Poly::Cff Poly::getCoeff(unsigned int exp) const {
//    return m_monomials[exp];
//}
//
//template<typename Number>
//unsigned int Poly::getDegree() const {
//    return m_monomials.size() - 1;
//}
//
//template<typename Number>
//Poly::String Poly::toString(int type) const {
//    String str = "";
//
//    if (ROOTS) {
//        unsigned multiplicity = 0;
//
//        for (unsigned int j = 0; j < m_roots.size(); ++j) {
//            unsigned int i = (type & ASC ? 0 : m_roots.size() - j - 1);
//
//            if (i == 0 || m_roots[i] != m_roots[i - 1]) {
//                if (multiplicity > (1 - ((type & Cff::FULL) == Cff::FULL))) {
//                    std::stringstream ss;
//                    ss << (type & Cff::WIDE ? " " : "") << "^"
//                       << (type & Cff::WIDE ? " " : "") << multiplicity << " ";
//
//                    str.append(ss.str());
//                }
//
//                multiplicity = 0;
//                if (m_roots[i] != Number(0)) {
//                    if (type & Cff::WIDE) {
//                        str.append("( x " + std::to_string(m_roots[i]) + " )");
//                    } else {
//                        str.append("(x");
//                        str.append((m_roots[i] > 0 ? "+" : "") + std::to_string(m_roots[i]) + ")");
//                    }
//                } else {
//                    if (type & Cff::FULL) {
//                        if (type & Cff::WIDE) {
//                            str.append("(x)");
//                        } else {
//                            str.append("( x )");
//                        }
//                    } else {
//                        str.append("x");
//                    }
//                }
//            } else {
//                multiplicity++;
//            }
//        }
//    } else {
//        bool first_printed = false;
//        for (unsigned int j = 0; j < getDegree(); ++j) {
//            unsigned int i = (type & ASC ? 0 : getDegree() - i - 1);
//            if (m_monomials[i].getMulti() != Number(0) || (type & EXT)) {
//                if (first_printed && m_monomials[i].getMulti() >= Number(0)) {
//                    str.append(type & Cff::WIDE ? " " : "");
//                    str.append("+");
//                    str.append(type & Cff::WIDE ? " " : "");
//                }
//                str.append(m_monomials[i].toString(type));
//                str.append((type & Cff::WIDE ? " x  ^ " : "x^") + std::to_string(i));
//                first_printed = true;
//            }
//        }
//    }
//
//    return str;
//}
