//
// Created by mkondratek on 31.10.17.
//

#include <iostream>
#include <type_traits>
#include <vector>
#include "Coeff.h"

template<typename Number>
const typename Coeff<Number>::String Coeff<Number>::NONVAR = "";

template<typename Number>
Coeff<Number> Coeff<Number>::parse(Coeff::String str) {
    const String split_points = "-+";

    if (DEBUG) {
        std::cout << "parse( \"" << str << "\"";
    }

    if (str == "") {
        str = "0";
    }

    bool needsParentesis = false;
    int deep = 0;

    for (unsigned long int i = 0; i < str.length(); ++i) {
        deep += (str[i] == '(');
        deep -= (str[i] == ')');
        if (deep == 0 && split_points.find(str[i]) != String::npos && i != 0) {
            needsParentesis = true;
            break;
        }
    }

    if (needsParentesis) {
        str = "(" + str + ")";
    }

    if (DEBUG) {
        std::cout << " -> \"" << str << "\" )\n";
    }

    Coeff<Number> result;

    auto simpleParse = [](String s) -> Coeff<Number> {
        if (DEBUG) {
            std::cout << "simpleParse( \"" << s << "\"";
        }

        if (s.length() > 0 && s[0] == '+') { // cut +
            s = s.substr(1);
        }

        if (s.length() == 0) {
            s = "1";
        } else if (s.length() == 1) {
            if (s[0] == '-') {
                s = "-1";
            } else if ('0' > s[0] || '9' < s[0]) {
                s = "1" + s;
            }

        } else {
            if (s[0] == '-' && ('0' > s[1] || '9' < s[1])) {
                s[0] = '1';
                s = "-" + s;
            }
        }

        unsigned long int i = 0;
        i += (s[0] == '-');
        while (('0' <= s[i] && s[i] <= '9') || s[i] == '.') { i++; }

        if (i == 0) { //there is no number
            s = "1" + s;
            i++;
        }

        if (DEBUG) {
            std::cout << " -> \"" << s << "\" )\n";
        }

        if (std::is_same<Number, int>::value) {
            return Coeff<Number>(std::stoi(s.substr(0, i)), s.substr(i));
        } else if (std::is_same<Number, long int>::value) {
            return Coeff<Number>(std::stol(s.substr(0, i)), s.substr(i));
        } else if (std::is_same<Number, long long int>::value) {
            return Coeff<Number>(std::stoll(s.substr(0, i)), s.substr(i));
        } else if (std::is_same<Number, float>::value) {
            return Coeff<Number>(std::stof(s.substr(0, i)), s.substr(i));
        } else if (std::is_same<Number, double>::value) {
            return Coeff<Number>(std::stod(s.substr(0, i)), s.substr(i));
        } else if (std::is_same<Number, long double>::value) {
            return Coeff<Number>(std::stold(s.substr(0, i)), s.substr(i));
        } else {
            return Coeff<Number>(1, s);
        }
    };

    unsigned long int __beg = 0;
    unsigned long int __end = 0;

    __end = str.find('(');

    __end = (__end == String::npos ? str.length() : __end);

    result = simpleParse(str.substr(0, __end));

    if (DEBUG) {
        std::cout << "( at " << __end << "/" << str.length() << std::endl;
        std::cout << "result: " << result << std::endl;
    }

    __beg = __end + 1;
    for (unsigned long int i = __beg; i < str.length() - 1; ++i) {
        __end = i;

        deep = 0;
        while (__end < str.length() && (deep != 0 || str[__end] != ')') &&
               ((split_points.find(str[__end]) == String::npos && deep == 0)
                || deep > 0 || __beg == __end)) {
            deep += (str[__end] == '(');
            deep -= (str[__end] == ')');

            __end++;
        }

        if (DEBUG) {
            std::cout << "beg: " << __beg << " end: " << __end << std::endl;
        }

        if (str[__beg] == '-') {
            if (__beg - 1 < 0 || str[__beg - 1] == '(') {  //unary -

                if (DEBUG) {
                    std::cout << "unary-\n";
                }

                Coeff<Number> tmp = parse(str.substr(__beg, __end - __beg));
                result.addCoeff(tmp);
            } else { //binary -

                if (DEBUG) {
                    std::cout << "binary-\n";
                }

                Coeff<Number> tmp = parse(str.substr(__beg, __end - __beg));
                result.addCoeff(tmp);
            }
        } else if (str[__beg] == '+') {
            if (__beg - 1 < 0 || str[__beg - 1] == '(') {  //unary +

                if (DEBUG) {
                    std::cout << "unary+\n";
                }

                Coeff<Number> tmp = parse(str.substr(__beg, __end - __beg));
                result.addCoeff(tmp);
            } else { //binary +

                if (DEBUG) {
                    std::cout << "binary+\n";
                }

                Coeff<Number> tmp = parse(str.substr(__beg, __end - __beg));
                result.addCoeff(tmp);
            }
        } else {
            Coeff<Number> tmp = parse(str.substr(__beg, __end - __beg));
            result.addCoeff(tmp);
        }

        i = __end;
        __beg = i;
    }

    result.fix();

    return result;
}

template <typename Number>
Coeff<Number> Coeff<Number>::getRandom(unsigned long int seed) {
    String var = " ";

    var[0] = (rand() % 4 + 'a');

    Coeff<Number> result(Number(rand() % 300 + 100) / Number(100), var);

    if (seed > 0) {
        unsigned long children = rand() % (seed / 2 + 2);

        while (children--) {
            if ((rand() % 10) < 7) {
                result.addCoeff(getRandom(seed / 3 + children));
            }
        }
    }

    if (result.toString().length() < seed / 2) {
        result = getRandom(seed);
    }

    return result;
}

template<typename Number>
Coeff<Number> &Coeff<Number>::fix() {
    /** remove coeffs with 0 multiplier **/
    if (m_multiplier == Number(0)) {
        m_coeffs.clear();
        m_variable = NONVAR;
    } else {
        for (auto it = m_coeffs.begin(); it != m_coeffs.end() && m_coeffs.size() > 1;) {
            if (it->m_multiplier == Number(0)) {
                it = m_coeffs.erase(it);
            } else { ++it; }
        }
    }

    /** connect single child with parent if at least one have no variable **/
    if (m_coeffs.size() == 1 &&
        (m_variable == NONVAR || m_coeffs.begin()->m_variable == NONVAR)) {
        Coeff tmp = *m_coeffs.begin();
        m_multiplier *= tmp.m_multiplier;
        m_coeffs = tmp.m_coeffs;
        m_variable = (m_variable == NONVAR ? tmp.m_variable : m_variable);
    }

    return *this;
}

template<typename Number>
bool Coeff<Number>::operator<(Coeff<Number> const &other) const {
    return m_variable < other.m_variable;
}

template<typename Number>
bool Coeff<Number>::operator==(Coeff<Number> const &other) const {
    return !(*this < other || other < *this);
}

template<typename Number>
Coeff<Number>::Coeff(const Number &multiplier,
                     const Coeff::String &variable,
                     const Coeff::SetOfC &coeffs) noexcept
        : m_multiplier(multiplier), m_variable(variable), m_coeffs(coeffs) { fix(); }

template<typename Number>
Coeff<Number>::Coeff(const Number &multiplier,
                     const Coeff::String &variable) noexcept
        : m_multiplier(multiplier), m_variable(variable) { fix(); }

template<typename Number>
Coeff<Number>::Coeff(const Number &multiplier,
                     const Coeff::SetOfC &coeffs) noexcept
        : m_multiplier(multiplier), m_variable(NONVAR), m_coeffs(coeffs) { fix(); }

template<typename Number>
Coeff<Number>::Coeff(const Coeff<Number>::String &variable,
                     const Coeff::SetOfC &coeffs) noexcept
        : m_multiplier(Number(1)), m_variable(variable), m_coeffs(coeffs) { fix(); }

template<typename Number>
Coeff<Number>::Coeff(const Number &multiplier) noexcept
        : m_multiplier(multiplier), m_variable(NONVAR) {}

template<typename Number>
Coeff<Number>::Coeff(const Coeff::String &variable) noexcept
        : m_multiplier(Number(1)), m_variable(variable) {}

template<typename Number>
Coeff<Number>::Coeff(const Coeff::SetOfC &coeffs) noexcept
        : m_multiplier(Number(1)), m_variable(NONVAR), m_coeffs(coeffs) { fix(); }

template<typename Number>
Coeff<Number>::Coeff() noexcept : m_multiplier(Number(0)), m_variable(NONVAR) {}

template<typename Number>
Coeff<Number>::Coeff(const Coeff<Number> &coeff) noexcept {
    if (&coeff == this) {
        m_multiplier = Number(1);
        m_variable = NONVAR;
    } else {
        m_multiplier = coeff.m_multiplier;
        m_variable = coeff.m_variable;
        m_coeffs = coeff.m_coeffs;
    }
}

template<typename Number>
Coeff<Number>::Coeff(Coeff<Number> &&coeff) noexcept {
    if (&coeff == this) {
        m_multiplier = Number(1);
        m_variable = NONVAR;
    } else {
        m_multiplier = coeff.m_multiplier;
        m_variable = coeff.m_variable;
        m_coeffs = coeff.m_coeffs;
    }
}

template<typename Number>
Coeff<Number> &Coeff<Number>::operator=(const Coeff<Number> &coeff) noexcept {
    if (&coeff != this) {
        m_multiplier = coeff.m_multiplier;
        m_variable = coeff.m_variable;
        m_coeffs = coeff.m_coeffs;
    }
    return *this;
}

template<typename Number>
Coeff<Number> &Coeff<Number>::operator=(Coeff<Number> &&coeff) noexcept {
    if (&coeff != this) {
        m_multiplier = std::move(coeff.m_multiplier);
        m_variable = std::move(coeff.m_variable);
        m_coeffs = std::move(coeff.m_coeffs);
    }
    return *this;;
}

template<typename Number>
Coeff<Number> &Coeff<Number>::operator+=(Coeff<Number> const &a) {
    *this = *this + a;
    return *this;
}

template<typename Number>
Coeff<Number> &Coeff<Number>::operator-=(Coeff<Number> const &a) {
    *this = *this - a;
    return *this;
}

//template <typename Number>
//Coeff<Number> &Coeff<Number>::operator*=(Coeff<Number> const &a) {
//    return Coeff<Number>(*this);
//}

//template <typename Number>
//Coeff<Number> &Coeff<Number>::operator/=(const Coeff<Number> &a) {
//    return Coeff<Number>(*this);
//}

template<typename Number>
Coeff<Number> Coeff<Number>::operator+() const {
    return Coeff<Number>(*this);
}

template<typename Number>
Coeff<Number> Coeff<Number>::operator-() const {
    Coeff<Number> negated(*this);
    negated.m_multiplier *= Number(-1);
    return negated;
}

template<typename Number>
Coeff<Number> &Coeff<Number>::addCoeff(Coeff<Number> const &coeff) {
    if (DEBUG) {
        std::cout << *this << " addCoeff " << coeff << "\n";
    }

    if (coeff.m_multiplier != Number(0)) {
        auto it = m_coeffs.find(coeff);
        if (it == m_coeffs.end()) {
            m_coeffs.insert(coeff);
        } else {
            Coeff<Number> tmp0(Number(1));
            Coeff<Number> tmp1(Number(1));

            if (DEBUG) {
                std::cout << " > " << *it << " | " << coeff << "\n";
            }

            tmp0 = *it + coeff;

            m_coeffs.erase(it);
            this->addCoeff(tmp0);
        }
    }
    return *this;
}

template<typename Number>
Coeff<Number> &Coeff<Number>::putOut(Number num) {
    for (auto &c : m_coeffs) {
        c.m_multiplier /= num;
    }
    m_multiplier *= num;

    return *this;
}

template<typename Number>
Coeff<Number> &Coeff<Number>::putIn(Number num) {
    for (auto &c : m_coeffs) {
        c.m_multiplier *= num;
    }
    m_multiplier /= num;

    return *this;
}

template<typename Number>
Coeff<Number> &Coeff<Number>::apply(Coeff<Number>::String variable, Number value) {
    if (DEBUG) {
        std::cout << "apply " << variable << " = " << value << " for " << *this << "\n";
    }

    if (m_variable == variable) {
        m_multiplier *= value;
        m_variable = NONVAR;
    }

    Coeff<Number> tmp_cff;
    for (auto c : m_coeffs) {
        c.apply(variable, value);
        tmp_cff.addCoeff(c);
    }

    if (!m_coeffs.empty() && tmp_cff.m_coeffs.empty()) {
        //m_coeffs sumed to 0 after apply
        m_multiplier = Number(0);
        m_variable = NONVAR;
    }

    m_coeffs = tmp_cff.m_coeffs;

    if (DEBUG) {
        std::cout << *this << " -> ";
    }

    fix();

    if (DEBUG) {
        std::cout << *this << "\n";
    }

    return *this;
}

template<typename Number>
Coeff<Number> &Coeff<Number>::apply(MapOfV values) {
    for (auto v : values) {
        this->apply(v.first, v.second);
    }
    return *this;
}

template<typename Number>
Number Coeff<Number>::calculate(MapOfV values) const {
    Coeff<Number> tmp(*this);
    tmp.apply(values);
    return tmp.m_multiplier;
}

template<typename Number>
Number Coeff<Number>::getMultiplier() const {
    return m_multiplier;
}

template<typename Number>
typename Coeff<Number>::String Coeff<Number>::getVariable() const {
    return m_variable;
}

template<typename Number>
typename Coeff<Number>::SetOfC Coeff<Number>::getCoeffs() const {
    return m_coeffs;
}

template<typename Number>
typename Coeff<Number>::SetOfV Coeff<Number>::getAllVariables() const {
    SetOfV out;

    if (!m_variable.empty()) {
        out.insert(m_variable);
    }

    for (const auto &c : m_coeffs) {
        SetOfV tmp = c.getAllVariables();
        out.insert(tmp.begin(), tmp.end());
    }

    return out;
}

template<typename Number>
typename Coeff<Number>::String Coeff<Number>::toString() const {
    String out = NONVAR;
    //todo better look with spaces

    if (m_multiplier != Number(0)) {
        std::stringstream ss;
        if (m_variable != NONVAR || !m_coeffs.empty()) {
            if (m_multiplier == Number(-1)) { out += "-"; }
            else if (m_multiplier == Number(1)) {}
            else {
                ss << m_multiplier;
            }
        } else {
            ss << m_multiplier;
        }
        out += ss.str();

        out += m_variable;

        if (!m_coeffs.empty()) {
            { out += "("; }
            for (auto &c : m_coeffs) {
                out += (c == *m_coeffs.begin() || c.m_multiplier <= Number(0) ? NONVAR : "+") + c.toString();
            }
            { out += ")"; }
        }
    }

    if (out == NONVAR) { out = "0"; }
    else {
        while (out.size() > 1 && out[0] == out[1] && out[0] == '0') {
            out = out.substr(1);
        }
    }

    return out;
}

/** explicit instantiations **/
template
class Coeff<int>;

template
class Coeff<long int>;

template
class Coeff<long long int>;

template
class Coeff<float>;

template
class Coeff<double>;

template
class Coeff<long double>;
