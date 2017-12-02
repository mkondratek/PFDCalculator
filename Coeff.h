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
template<typename Number>
Coeff<Number> operator*(Coeff<Number> const &a, Coeff<Number> const &b);
//template<typename Number>
//Coeff<Number> operator/(Coeff<Number> const &a, Coeff<Number> const &b);

/**  declaration end  **/

template<typename Number>
class Coeff {
public:
    using String = std::string;
    using SetOfV = std::set<String>;
    using MapOfV = std::map<String, Number>;

    static const String NONVAR;

    class SetOfC {
        mutable std::set<Coeff<Number>> m_coeffs;

    public:
        SetOfC();

        explicit SetOfC(Coeff<Number> const &coeff);

        SetOfC(std::initializer_list<Coeff<Number>> il);

        SetOfC &add(Coeff const &coeff); //adds to m_coeffs

        std::set<Coeff<Number>> &asSet() const;
    };


    /* DISPLAY PARAMS */
    static const int FULL;
    static const int TIGHT;
    static const int WIDE;
private:
    /* DISPLAY PARAM */
    static const int BIN_MINUS;

    /* GENERAL MEMBERS */
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

    template<typename N>
    friend Coeff<N> operator*(Coeff<N> const &a, Coeff<N> const &b);

//    template<typename N>
//    friend Coeff<N> operator/(Coeff<N> const &a, Coeff<N> const &b);

    Coeff<Number> &operator+=(Coeff<Number> const &a);

    Coeff<Number> &operator-=(Coeff<Number> const &a);

    Coeff<Number> &operator*=(Coeff<Number> const &a);

//    Coeff<Number> &operator/=(Coeff<Number> const &a);

    Coeff<Number> operator+() const;

    Coeff<Number> operator-() const;

    Coeff<Number> &add(Coeff<Number> const &coeff); //adds to m_coeffs

    Coeff<Number> &putOut(Number num); //divides multipliers in set and multiplies multiplier by num

    Coeff<Number> &putIn(Number num); //divides multiplier and multiplies multipliers in set by num

    Coeff<Number> &apply(String variable, Number value);

    Coeff<Number> &apply(MapOfV values);

    Number calculate(MapOfV values) const;

    Number getMulti() const;

    String getVar() const;

    SetOfC getCoeffs() const;

    SetOfV getAllVars() const;

    String toString(int type = TIGHT) const;

    friend std::ostream &operator<<<Number>(std::ostream &stream, Coeff<Number> const &coeff);

    friend std::istream &operator>><Number>(std::istream &stream, Coeff<Number> &coeff);
};

template<typename Number>
Coeff<Number> operator+(Coeff<Number> const &a, Coeff<Number> const &b) {

    Coeff<Number> x = a;
    Coeff<Number> y = b;

    if (DEBUG) {
        std::cout << "operator+ ( " << x << " | " << y << " )";
    }

    Coeff<Number> result(Number(1));

    if (x.m_coeffs.asSet().empty() && !y.m_coeffs.asSet().empty()) {
        x.m_coeffs.asSet().insert(Coeff<Number>(1));
    }
    else if (!x.m_coeffs.asSet().empty() && y.m_coeffs.asSet().empty()) {
        y.m_coeffs.asSet().insert(Coeff<Number>(1));
    }

    /* make coeffs with nonempty set having same multiplier */
    if (x.m_multiplier != y.m_multiplier && !x.m_coeffs.asSet().empty()) {
        x.putIn(x.m_multiplier);
        y.putIn(y.m_multiplier);
    }

    if (x.m_variable == y.m_variable && !x.m_coeffs.asSet().empty()) {

        if (DEBUG) {
            std::cout << " -> = !ept\n";
            std::cout << "  " << x << " | " << y << "\n";
        }

        result = y;

        for (const auto &c : x.m_coeffs.asSet()) { result.add(c); }

    } else if (x.m_variable == y.m_variable && x.m_coeffs.asSet().empty()) {

        if (DEBUG) {
            std::cout << " -> = ept\n";
        }

        result = x;
        result.m_multiplier += y.m_multiplier;
    }
    else {

        if (DEBUG) {
            std::cout << " -> els\n";
        }

        result.add(x).add(y).fix();
    }

    if (DEBUG) {
        std::cout << "  " << x << " + " << y << " = " << result << std::endl;
    }

    return result;
}

template<typename Number>
Coeff<Number> operator-(Coeff<Number> const &a, Coeff<Number> const &b) {
    if (DEBUG) {
        std::cout << "operator- ( " << a << " | " << b << ")\n";
    }

    Coeff<Number> tmp(b);
    tmp.m_multiplier *= Number(-1);
    return a + tmp;
}

template<typename Number>
Coeff<Number> operator*(Coeff<Number> const &a, Coeff<Number> const &b) {
    Coeff<Number> result(Number(1));

    Coeff<Number> x = a;
    Coeff<Number> y = b;

    if (DEBUG) {
        std::cout << "operator* ( " << x << " | " << y  << " )";
    }

    if (y < x) {
        std::swap(x, y);

        if (DEBUG) {
            std::cout << " swaping";
        }
    }

    if (x.m_multiplier == Number(0) || y.m_multiplier == Number(0)) {

        if (DEBUG) {
            std::cout << " -> 0 | 0\n";
        }

        result.m_multiplier = 0; //returns 0
    } else if (x.m_variable == Coeff<Number>::NONVAR && x.m_coeffs.asSet().empty()) {

        if (DEBUG) {
            std::cout << " -> 1 * y\n";
        }

        result = y;
        result.m_multiplier *= x.m_multiplier;
    } else if (y.m_variable == Coeff<Number>::NONVAR && y.m_coeffs.asSet().empty()) {

        if (DEBUG) {
            std::cout << " -> x * 1\n";
        }

        result = x;
        result.m_multiplier *= y.m_multiplier;
    } else if (x.m_coeffs.asSet().empty() && !y.m_coeffs.asSet().empty()) {

        if (DEBUG) {
            std::cout << " -> x ept\n";
        }

        result = x;
        result.add(y);
        result.putOut(y.m_multiplier);
    } else if (!x.m_coeffs.asSet().empty() && y.m_coeffs.asSet().empty()) {

        if (DEBUG) {
            std::cout << " -> y ept\n";
        }

        result = y;
        result.add(x);
        result.putOut(x.m_multiplier);
    } else if (x.m_coeffs.asSet().empty() && y.m_coeffs.asSet().empty()) {

        if (DEBUG) {
            std::cout << " -> both ept\n";
        }

        result = x;
        result.add(y);
        result.putOut(y.m_multiplier);

    } else {

        if (DEBUG) {
            std::cout << " -> both !ept\n";
        }

        result.m_multiplier = x.m_multiplier * y.m_multiplier;
        result.m_variable = x.m_variable;
        Coeff<Number> inner(y.m_variable);

        for (auto & c_a : x.m_coeffs.asSet()) {
            for (auto & c_b : y.m_coeffs.asSet()) {
                inner.add(c_a * c_b);
                if (DEBUG) {
                    std::cout << "  then " << inner << "    ";
                }
            }
        }

        if (DEBUG) {
            std::cout << std::endl;
        }

        result.add(inner);
    }

    result.fix();

    if (DEBUG) {
        std::cout << "  " << x << " * " << y << " = " << result << std::endl;
    }

    return result;
}

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
