//
// Created by mkondratek on 31.10.17.
//

#include <iostream>
#include "Coeff.h"

template<typename Number>
const typename Coeff<Number>::String Coeff<Number>::NONVAR = "";

template<typename Number>
Coeff<Number> &Coeff<Number>::fix() {
    /** remove coeffs with 0 multiplier **/
    if (m_multiplier == Number(0)) {
        m_coeffs.clear();
        m_variable = NONVAR;
    } else {
        Iterator it;
        for (it = m_coeffs.begin(); it != m_coeffs.end() && m_coeffs.size() > 1;) {
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
Coeff<Number> &Coeff<Number>::addCoeff(Coeff<Number> const &coeff) {
    if (coeff.m_multiplier != Number(0)) {
        Iterator it = m_coeffs.find(coeff);
        if (it == m_coeffs.end()) {
            m_coeffs.insert(coeff);
        } else {
            Coeff<Number> tmp = *it + coeff;
            m_coeffs.erase(it);
            this->addCoeff(tmp);
        }
    }
    return *this;
}

//template<typename Number>
//Coeff<Number> &Coeff::pull(Number num) {
//    //todo
//    return <#initializer#>;
//}

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
Coeff<Number>::Coeff() noexcept : m_multiplier(Number(1)), m_variable(NONVAR) {}

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

//todo better look with spaces
template<typename Number>
typename Coeff<Number>::String Coeff<Number>::toString() const {
    String out = NONVAR;

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
