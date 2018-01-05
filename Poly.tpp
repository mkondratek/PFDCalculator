//
// Created by mkondratek on 27.12.17.
//

#include <algorithm>

template<typename Number>
int Poly<Number>::display_t = int(DESC) | int(Cff::TIGHT);

template<typename Number>
Poly<Number> &Poly<Number>::updateRoots() {
    m_roots.clear();
    m_roots_valid = false;

    return *this;
}

template<typename Number>
Poly<Number> Poly<Number>::parse(String str) {
    Poly result;

    if (str == "") {
        str = "0";
    }

    const String split_points = "-+";

    unsigned long int u_beg = 0;
    unsigned long int u_end = 0;

    u_end = str.find('^');
    u_end = (u_end == String::npos ? str.length() : u_end);

    while (u_beg != str.length()) {
        if (u_end != str.length()) {
            std::cout << "tu\n";
            unsigned long int exp_end = u_end;

            while (exp_end < str.length() && split_points.find(str[exp_end]) == String::npos) {
                ++exp_end;
            }

            result += Poly(Cff::parse(str.substr(u_beg, u_end - u_beg)), Cff::toNum(str.substr(u_end + 1, exp_end - u_end - 1)));
            u_end = exp_end;
        }
        else {
            std::cout << "jednak tu ";
            bool found_x = (str.find('x', u_end) != String::npos);
            u_end = (!found_x ? str.length() : u_end);

            if (found_x && str.find_first_of(split_points, u_beg) != String::npos) {
                u_end = str.find_first_of(split_points, u_beg);
            }
            std::cout << u_beg << " " << u_end << " | ";

            std::cout << "to Cff::parse: " << str.substr(u_beg, u_end - u_beg ) << std::endl;
            result += Poly(Cff::parse(str.substr(u_beg, u_end - u_beg)), found_x);
        }

        u_beg = u_end;
        u_end = str.find('^', u_end);
        u_end = (u_end == String::npos ? str.length() : u_end);
        std::cout << "b: " << u_beg << " e: " << u_end << " / " << str.length() << std::endl;
    }

    result.updateRoots();

    return result;
}

template<typename Number>
Poly<Number>::Poly() noexcept
        : m_monomials(Cffvec{Cff(Number(0))}), m_roots_valid(false) {}

template<typename Number>
Poly<Number>::Poly(Number const& number, unsigned int exp) noexcept {
    if (number != Number(0)) {
        m_monomials.resize(exp + 1, Cff(Number(0)));
        m_monomials[exp] = Cff(number);
        for (unsigned int i = 0; i < exp; ++i) {
            m_roots.push_back(Number(0));
        }
        m_roots_valid = true;
    } else {
        m_monomials.push_back(Cff(Number(0)));
        m_roots_valid = false;
    }
}

template<typename Number>
Poly<Number>::Poly(Poly<Number>::Cff const& cff, unsigned int exp) noexcept {
    if (!cff.ready() || cff.getMulti() != Number(0)) {
        m_monomials.resize(exp + 1, Cff(Number(0)));
        m_monomials[exp] = cff;
        if (cff.ready()) {
            m_roots.push_back(cff.calculate());
            m_roots_valid = true;
        } else {
            m_roots_valid = false;
        }
    } else {
        m_monomials.push_back(Cff(Number(0)));
        m_roots_valid = false;
    }
}

template<typename Number>
Poly<Number>::Poly(Poly<Number>::Numvec const& roots) noexcept {
    Poly tmp(Number((roots.empty() ? 0 : 1)));

    for (auto const& r : roots) {
        tmp *= (Poly(-r) + Poly(Number(1), 1));
    }

    m_roots = roots;
    m_roots_valid = true;
    std::sort(m_roots.begin(), m_roots.end());
    m_monomials = tmp.m_monomials;
}

template<typename Number>
Poly<Number>::Poly(Poly<Number>::Cffvec const& roots) noexcept {
    Poly tmp(Number((roots.empty() ? 0 : 1)));

    for (auto const& r : roots) {
        tmp *= (Poly(-r) + Poly(Number(1), 1));
    }

    m_roots_valid = false;
    m_monomials = tmp.m_monomials;
}

template<typename Number>
Poly<Number>::Poly(Poly<Number> const& poly) noexcept {
    if (&poly == this) {
        m_monomials.push_back(Cff(0));
    } else {
        m_roots_valid = poly.m_roots_valid;
        m_monomials = poly.m_monomials;
        m_roots = poly.m_roots;
    }
}

template<typename Number>
Poly<Number>::Poly(Poly<Number>&& poly) noexcept {
    if (&poly == this) {
        m_monomials.push_back(Cff(0));
    } else {
        m_roots_valid = std::move(poly.m_roots_valid);
        m_monomials = std::move(poly.m_monomials);
        m_roots = std::move(poly.m_roots);
    }
}

template<typename Number>
Poly<Number> &Poly<Number>::operator=(Poly<Number> const &poly) noexcept {
    if (&poly != this) {
        m_roots_valid = poly.m_roots_valid;
        m_monomials = poly.m_monomials;
        m_roots = poly.m_roots;
    }
    return *this;
}

template<typename Number>
Poly<Number> &Poly<Number>::operator=(Poly<Number> &&poly) noexcept {
    if (&poly != this) {
        m_roots_valid = std::move(poly.m_roots_valid);
        m_monomials = std::move(poly.m_monomials);
        m_roots = std::move(poly.m_roots);
    }
    return *this;
}

template <typename Number>
Poly<Number> operator+(Poly<Number> const &a, Poly<Number> const &b) {
    if (DEBUG_MODE) {
        std::cout << "operator+ ( " << a << " | " << b << " )" << std::endl;
    }

    Poly<Number> result(a);
    result.m_monomials.resize(std::max(a.getDegree(), b.getDegree()) + 1, Coeff<Number>(Number(0)));
    for (unsigned int i = 0; i < b.getDegree() + 1; ++i) {
        result.m_monomials[i] += b.m_monomials[i];
    }

    result.updateRoots();

    if (DEBUG_MODE) {
        std::cout << "  -> " << result << std::endl;
    }

    return result;
}

template <typename Number>
Poly<Number> operator-(Poly<Number> const &a, Poly<Number> const &b) {
    return a + (-b);
}

template <typename Number>
Poly<Number> operator*(Poly<Number> const &a, Poly<Number> const &b) {
    if (DEBUG_MODE) {
        std::cout << "operator* ( " << a << " | " << b << " )" << std::endl;
    }

    Poly<Number> result;
    result.m_monomials.resize(a.getDegree() + b.getDegree() + 1, Coeff<Number>(Number(0)));
    for (int i = 0; i < a.getDegree() + 1; ++i) {
        for (int j = 0; j < b.getDegree() + 1; ++j) {
            result.m_monomials[i + j] += a.m_monomials[i] * b.m_monomials[j];
        }
    }

    if (a.m_roots_valid && b.m_roots_valid) {
        result.m_roots = a.m_roots;
        for (auto const& r : b.m_roots) {
            result.m_roots.push_back(r);
        }
        result.m_roots_valid = true;
    } else {
        result.updateRoots();
    }

    std::sort(result.m_roots.begin(), result.m_roots.end());

    if (DEBUG_MODE) {
        std::cout << "  -> " << result << std::endl;
    }

    return result;
}

//template <typename Number>
//Poly<Number> operator/(Poly<Number> const &a, Poly<Number> const &b) {
//
//}

template<typename Number>
Poly<Number> &Poly<Number>::operator+=(Poly<Number> const &a) {
    Poly tmp = *this + a;
    *this = tmp;
    return *this;
}

template<typename Number>
Poly<Number> &Poly<Number>::operator-=(Poly<Number> const &a) {
    Poly tmp = *this - a;
    *this = tmp;
    return *this;
}

template<typename Number>
Poly<Number> &Poly<Number>::operator*=(Poly<Number> const &a) {
    Poly tmp = *this * a;
    *this = tmp;
    return *this;
}

template<typename Number>
Poly<Number> Poly<Number>::operator+() const {
    return *this;
}

template<typename Number>
Poly<Number> Poly<Number>::operator-() const {
    Poly<Number> tmp(*this);
    for (auto &c : tmp.m_monomials) {
        c *= Cff(Number(-1));
    }
    return tmp;
}

template<typename Number>
typename Poly<Number>::Cffvec Poly<Number>::getCoeffs() const {
    return m_monomials;
}

template<typename Number>
typename Poly<Number>::Numvec Poly<Number>::getRoots() const {
    return m_roots;
}

template<typename Number>
Poly<Number> &Poly<Number>::apply(typename Coeff<Number>::String const& variable, Number const& value) {
    for (auto &c : m_monomials) {
        c.apply(variable, value);
    }

    updateRoots();
    return *this;
}

template<typename Number>
Poly<Number> &Poly<Number>::apply(typename Coeff<Number>::MapOfV const& values) {
    for (auto &v : values) {
        apply(v.first, v.second);
    }

    return *this;
}

template<typename Number>
Number Poly<Number>::calculate(Number const& number) const {
    return calculate(number, Poly<Number>::Cff::MapOfV());
}

template<typename Number>
Number Poly<Number>::calculate(Number const& number, typename Poly<Number>::Cff::MapOfV const& values) const {
    Poly<Number> tmp(*this);

    Number result(0);
    for (unsigned int i = tmp.m_monomials.size() - 1; i >= 0; --i) {
        result *= number;
        result += tmp.m_monomials[i].calculate(values);
    }

    return result;
}

template<typename Number>
typename Poly<Number>::Cff Poly<Number>::getCoeff(unsigned int exp) const {
    return (exp < getDegree() ? m_monomials[exp] : Cff(Number(0)));
}

template<typename Number>
unsigned int Poly<Number>::getDegree() const {
    return m_monomials.size() - 1;
}

template<typename Number>
typename Poly<Number>::String Poly<Number>::toString(int type) const {
    String str = "";

    if (type & int(ROOTS)) {
        unsigned int multiplicity = 0;

        for (unsigned int j = 0; j < getDegree() + 1; ++j) {
            unsigned int i = (type & int(ASC) ? 0 : getDegree() - j);

            if (i == 0 || m_roots[i] != m_roots[i - 1]) {
                if (multiplicity > (1 - ((type & int(Cff::FULL)) == int(Cff::FULL)))) {
                    std::stringstream ss;
                    ss << (type & int(Cff::WIDE) ? " " : "") << "^"
                       << (type & int(Cff::WIDE) ? " " : "") << multiplicity << " ";

                    str.append(ss.str());
                }

                multiplicity = 0;
                if (m_roots[i] != Number(0)) {
                    if (type & int(Cff::WIDE)) {
                        str.append("( x " + std::to_string(m_roots[i]) + " )");
                    } else {
                        str.append("(x");
                        str.append((m_roots[i] > 0 ? "+" : "") + std::to_string(m_roots[i]) + ")");
                    }
                } else {
                    if (type & int(Cff::FULL)) {
                        if (type & int(Cff::WIDE)) {
                            str.append("(x)");
                        } else {
                            str.append("( x )");
                        }
                    } else {
                        str.append("x");
                    }
                }
            } else {
                multiplicity++;
            }
        }
    } else {
        bool first_printed = false;
        for (unsigned int j = 0; j < getDegree() + 1; ++j) {
            unsigned int i = (type & int(ASC) ? j : getDegree() - j);
            if (m_monomials[i].getMulti() != Number(0) || (type & int(EXT)) || getDegree() == 0) {
                if (first_printed && m_monomials[i].getMulti() >= Number(0)) {
                    str.append(type & int(Cff::WIDE) ? " " : "");
                    str.append("+");
                    str.append(type & int(Cff::WIDE) ? " " : "");
                }
                String tmp = m_monomials[i].toString(type);
                if (type & int(EXT) || i == 0) {
                    str.append(tmp);
                } else {
                    if (tmp == "1") {
                        //ignore
                    } else if (tmp == "-1") {
                        str.append((type & int(Cff::WIDE) ? " - " : "-"));
                    } else {
                        str.append(m_monomials[i].toString(type));
                    }
                }

                if ((type & int(EXT)) || i > 0) {
                    if (type & int(EXT)) {
                        str.append((type & int(Cff::WIDE) ? " x ^ " : "x^"));
                        str.append(std::to_string(i));
                    } else if (i == 1) {
                        str.append((type & int(Cff::WIDE) ? " x " : "x"));
                    } else {
                        str.append((type & int(Cff::WIDE) ? " x ^ " : "x^"));
                        str.append(std::to_string(i));
                    }
                }


                first_printed = true;
            }
        }
    }

    return str;
}

template <typename Number>
std::ostream &operator<<(std::ostream &stream, Poly<Number> const &poly) {
    stream << poly.toString();
    return stream;
}

template <typename Number>
std::istream &operator>>(std::istream &stream, Poly<Number> const &poly) {
    typename Poly<Number>::String str;
    stream >> str;
    poly = Poly<Number>::parse(str);
    return stream;
}