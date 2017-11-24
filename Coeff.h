//
// Created by mkondratek on 31.10.17.
//

#ifndef PFDCALCULATOR_COEFF_H
#define PFDCALCULATOR_COEFF_H

#include <ostream>
#include <sstream>
#include <set>
#include <map>

/** declaration begin **/
template<typename Number>
class Coeff;

template<typename Number>
std::ostream &operator<<(std::ostream &stream, Coeff<Number> const &coeff);
template<typename Number>
Coeff<Number> operator+(Coeff<Number> const &a, Coeff<Number> const &b);
template<typename Number>
Coeff<Number> operator-(Coeff<Number> const &a, Coeff<Number> const &b);
//template<typename Number>
//Coeff<Number> operator*(Coeff<Number> const &a, Coeff<Number> const &b);
//template<typename Number>
//Coeff<Number> operator/(Coeff<Number> const &a, Coeff<Number> const &b);

/**  declaration end  **/

template<typename Number>
class Coeff {
public:
    using String   = std::string;
    using SetOfV   = std::set<String>;
    using SetOfC   = std::set<Coeff<Number>>;
    using Iterator = typename SetOfC::iterator;

    static const String NONVAR;

private:
    mutable Number m_multiplier;
    String m_variable;
    SetOfC m_coeffs;

public:

    Coeff<Number> &fix(); //removes 0s

    bool operator<(Coeff<Number> const &other) const;

    bool operator==(Coeff<Number> const &other) const;

    Coeff<Number> &addCoeff(Coeff const &coeff); //adds to m_coeffs

    Coeff<Number>(Number const &multiplier, String const &variable, SetOfC const &coeffs) noexcept;

    Coeff<Number>(Number const &multiplier, String const &variable) noexcept;

    Coeff<Number>(Number const &multiplier, SetOfC const &coeffs) noexcept;

    Coeff<Number>(String const &variable, SetOfC const &coeffs) noexcept;

    explicit Coeff<Number>(Number const &multiplier) noexcept;

    explicit Coeff<Number>(String const &variable) noexcept;

    explicit Coeff<Number>(SetOfC const &coeffs) noexcept;

    Coeff<Number>() noexcept;

    Coeff<Number>(Coeff<Number> const &coeff) noexcept;

    Coeff<Number>(Coeff<Number> &&coeff) noexcept;

    Coeff<Number> &operator=(Coeff<Number> const &coeff) noexcept;

    Coeff<Number> &operator=(Coeff<Number> &&coeff) noexcept;

    friend Coeff<Number> operator+<Number>(Coeff<Number> const &a, Coeff<Number> const &b);

    friend Coeff<Number> operator-<Number>(Coeff<Number> const &a, Coeff<Number> const &b);

//    template<typename N>
//    friend Coeff<N> operator*(Coeff<N> const &a, Coeff<N> const &b);

//    template<typename N>
//    friend Coeff<N> operator/(Coeff<N> const &a, Coeff<N> const &b);

    Coeff<Number> &operator+=(Coeff<Number> const &a);

    Coeff<Number> &operator-=(Coeff<Number> const &a);

//    Coeff<Number> &operator*=(Coeff<Number> const &a);

//    Coeff<Number> &operator/=(Coeff<Number> const &a);

    Coeff<Number> operator+();

    Coeff<Number> operator-();

    Coeff<Number> &apply(String variable, Number value);

    Coeff<Number> &apply(std::map<String, Number> values);

    Coeff<Number> &calculate(std::map<String, Number> values) const;

    Coeff<Number> &putOut(Number num); //divides multipliers in set and multiplies multiplier by num

    Coeff<Number> &putIn(Number num); //divides multiplier and multiplies multipliers in set by num

    Number getMultiplier() const;

    String getVariable() const;

    SetOfC getCoeffs() const;

    SetOfV getAllVariables() const;

    String toString() const;

    friend std::ostream &operator<<<Number>(std::ostream &stream, Coeff<Number> const &coeff);
};

template<typename Number>
Coeff<Number> operator+(Coeff<Number> const &a, Coeff<Number> const &b) {
    std::cout << "operator+\n";
    Coeff<Number> result;

    if (a.m_variable == b.m_variable && a.m_multiplier == b.m_multiplier
        && !a.m_coeffs.empty() && !b.m_coeffs.empty()) {

        std::cout << "= =\n";
        Coeff<Number> tmp_a = a;
        tmp_a.putOut(tmp_a.m_multiplier);

        result = b;
        for (const auto &c : tmp_a.m_coeffs) { result.addCoeff(c); }

    } else if (a.m_variable == b.m_variable
               && !a.m_coeffs.empty() && !b.m_coeffs.empty()) {

        std::cout << "= !ept\n";
        Coeff<Number> tmp_a = a;
        Coeff<Number> tmp_b = b;
        tmp_a.putIn(tmp_a.m_multiplier);
        tmp_b.putIn(tmp_b.m_multiplier);

        std::cout << tmp_a << " | " << tmp_b << "\n";

        for (const auto &c : tmp_a.m_coeffs) { result.addCoeff(c); }
        for (const auto &c : tmp_b.m_coeffs) { result.addCoeff(c); }

        result.m_variable = tmp_a.m_variable;

    } else if (a.m_variable == b.m_variable
               && a.m_coeffs.empty() && b.m_coeffs.empty()) {
        std::cout << "= ept\n";
        result = a;
        result.m_multiplier += b.m_multiplier;
    } else {
        std::cout << "e\n";
        result.addCoeff(a).addCoeff(b);
    }

    std::cout << a << " + " << b << " = " << result << std::endl;
    return result.fix();
}

template<typename Number>
Coeff<Number> operator-(Coeff<Number> const &a, Coeff<Number> const &b) {
    Coeff<Number> tmp(b);
    tmp.m_multiplier *= Number(-1);
    return a + tmp;
}

template<typename Number>
Coeff<Number> operator*(Coeff<Number> const &a, Coeff<Number> const &b) {
    return Coeff<Number>();
}

template<typename Number>
Coeff<Number> operator/(const Coeff<Number> &a, const Coeff<Number> &b) {
    return Coeff<Number>();
}

template<typename Number>
std::ostream &operator<<(std::ostream &stream, Coeff<Number> const &coeff) {
    stream << coeff.toString();
    return stream;
}

#endif //PFDCALCULATOR_COEFF_H
