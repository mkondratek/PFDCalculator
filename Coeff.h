//
// Created by mkondratek on 31.10.17.
//

#ifndef PFDCALCULATOR_COEFF_H
#define PFDCALCULATOR_COEFF_H

#include <iostream>
#include <sstream>
#include <set>
#include <map>

#define DEBUG false

/** declaration begin **/
template<typename Number>
class Coeff;

template<typename Number>
std::ostream &operator<<(std::ostream &stream, Coeff<Number> const &coeff);
template<typename Number>
std::istream &operator>>(std::istream &stream, Coeff<Number> &coeff);
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
    using String = std::string;
    using SetOfV = std::set<String>;
    using SetOfC = std::set<Coeff<Number>>;
    using MapOfV = std::map<String, Number>;

    static const String NONVAR;

private:
    mutable Number m_multiplier;
    String m_variable;
    SetOfC m_coeffs;

    Coeff<Number> &fix(); //removes 0s

public:

    static Coeff<Number> parse(String str);

    static Coeff<Number> getRandom(unsigned long int seed = 1);

    bool operator<(Coeff<Number> const &other) const;

    bool operator==(Coeff<Number> const &other) const;

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

    Coeff<Number> operator+() const;

    Coeff<Number> operator-() const;

    Coeff<Number> &addCoeff(Coeff const &coeff); //adds to m_coeffs

    Coeff<Number> &putOut(Number num); //divides multipliers in set and multiplies multiplier by num

    Coeff<Number> &putIn(Number num); //divides multiplier and multiplies multipliers in set by num

    Coeff<Number> &apply(String variable, Number value);

    Coeff<Number> &apply(MapOfV values);

    Number calculate(MapOfV values) const;

    Number getMultiplier() const;

    String getVariable() const;

    SetOfC getCoeffs() const;

    SetOfV getAllVariables() const;

    String toString() const;

    friend std::ostream &operator<<<Number>(std::ostream &stream, Coeff<Number> const &coeff);
    friend std::istream &operator>><Number>(std::istream &stream, Coeff<Number> &coeff);
};

template<typename Number>
Coeff<Number> operator+(Coeff<Number> const &a, Coeff<Number> const &b) {

    Coeff<Number> x = a;
    Coeff<Number> y = b;

    if (DEBUG) {
        std::cout << "operator+ multi( " << x.getMultiplier()
                  << " | " << y.getMultiplier() << " ) var( "
                  << x.getVariable() << " | " << y.getVariable() << " ) ";
    }

    Coeff<Number> result(Number(1));

    if (x.m_coeffs.empty() && !y.m_coeffs.empty()) {
        x.m_coeffs.insert(Coeff<Number>(1));
    }
    else if (!x.m_coeffs.empty() && y.m_coeffs.empty()) {
        y.m_coeffs.insert(Coeff<Number>(1));
    }

    if (x.m_variable == y.m_variable && x.m_multiplier == y.m_multiplier
        && !x.m_coeffs.empty() && !y.m_coeffs.empty()) {

        if (DEBUG) {
            std::cout << " -> = =\n";
        }

        Coeff<Number> tmp_a = x;
        tmp_a.putOut(tmp_a.m_multiplier);

        result = y;
        for (const auto &c : tmp_a.m_coeffs) { result.addCoeff(c); }

    } else if (x.m_variable == y.m_variable
               && !x.m_coeffs.empty() && !y.m_coeffs.empty()) {

        if (DEBUG) {
            std::cout << " -> = !ept tmp( ";
        }

        Coeff<Number> tmp_a = x;
        Coeff<Number> tmp_b = y;
        tmp_a.putIn(tmp_a.m_multiplier);
        tmp_b.putIn(tmp_b.m_multiplier);

        if (DEBUG) {
            std::cout << tmp_a << " | " << tmp_b << " )\n";
        }

        for (const auto &c : tmp_a.m_coeffs) { result.addCoeff(c); }
        for (const auto &c : tmp_b.m_coeffs) { result.addCoeff(c); }

        result.m_variable = tmp_a.m_variable;

    } else if (x.m_variable == y.m_variable
               && x.m_coeffs.empty() && y.m_coeffs.empty()) {

        if (DEBUG) {
            std::cout << " -> = ept\n";
        }

        result = x;
        result.m_multiplier += y.m_multiplier;
    } else {

        if (DEBUG) {
            std::cout << " -> els\n";
        }

        result.addCoeff(x).addCoeff(y).fix();
    }

    if (DEBUG) {
        std::cout << x << " + " << y << " = " << result << std::endl;
    }

    return result;
}

template<typename Number>
Coeff<Number> operator-(Coeff<Number> const &a, Coeff<Number> const &b) {
    Coeff<Number> tmp(b);
    tmp.m_multiplier *= Number(-1);
    return a + tmp;
}

//template<typename Number>
//Coeff<Number> operator*(Coeff<Number> const &a, Coeff<Number> const &b) {
//    return Coeff<Number>();
//}

//template<typename Number>
//Coeff<Number> operator/(const Coeff<Number> &a, const Coeff<Number> &b) {
//    return Coeff<Number>();
//}

template<typename Number>
std::ostream &operator<<(std::ostream &stream, Coeff<Number> const &coeff) {
    stream << coeff.toString();
    return stream;
}

template<typename Number>
std::istream &operator>>(std::istream &stream, Coeff<Number> &coeff) {
    typename Coeff<Number>::String str;

    stream >> str;
    coeff = Coeff<Number>::parse(str);

    return stream;
}

#endif //PFDCALCULATOR_COEFF_H
