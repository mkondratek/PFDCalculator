//
// Created by mkondratek on 31.10.17.
//

#ifndef PFDCALCULATOR_COEFF_H
#define PFDCALCULATOR_COEFF_H

#include <iostream>
#include <sstream>
#include <set>
#include <map>

#define DEBUG_MODE false

/** declaration begin **/
template<typename Number>
class Coeff;

template<typename Number>
std::ostream& operator<<(std::ostream& stream, Coeff<Number> const& coeff);
template<typename Number>
std::istream& operator>>(std::istream& stream, Coeff<Number>& coeff);
template<typename Number>
Coeff<Number> operator+(Coeff<Number> const& a, Coeff<Number> const& b);
template<typename Number>
Coeff<Number> operator-(Coeff<Number> const& a, Coeff<Number> const& b);
template<typename Number>
Coeff<Number> operator*(Coeff<Number> const& a, Coeff<Number> const& b);
//template<typename Number>
//Coeff<Number> operator/(Coeff<Number> const& a, Coeff<Number> const& b);
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
        explicit SetOfC(Coeff<Number> const& coeff);
        SetOfC(std::initializer_list<Coeff<Number>> il);
        SetOfC& add(Coeff<Number> const& coeff);
        std::set<Coeff<Number>>& asSet() const;
    };

    /* DISPLAY PARAMS */
    enum { FULL = 1, TIGHT = 2, WIDE = 4, BIN_MINUS = 8, PAR_OMIT = 16 };

private:

    /* GENERAL MEMBERS */
    mutable Number m_multiplier;
    String         m_variable;
    SetOfC         m_coeffs;

    static int display_t;

    Coeff<Number>& fix(); //removes 0s

public:
    static Number toNum(String str);

    static Coeff<Number> parse(String str);
    static Coeff<Number> getRandom(unsigned long int seed = 1);

    bool operator<(Coeff<Number> const& other) const;
    bool operator==(Coeff<Number> const& other) const;

    Coeff<Number>(Number const& multiplier, String const& variable, SetOfC const& coeffs) noexcept;
    Coeff<Number>(Number const& multiplier, String const& variable) noexcept;
    Coeff<Number>(Number const& multiplier, SetOfC const& coeffs) noexcept;
    Coeff<Number>(String const& variable, SetOfC const& coeffs) noexcept;

    explicit Coeff<Number>(Number const& multiplier) noexcept;
    explicit Coeff<Number>(String const& variable) noexcept;
    explicit Coeff<Number>(SetOfC const& coeffs) noexcept;

    Coeff<Number>() noexcept; // with 0 multiplier !!!

    Coeff<Number>(Coeff<Number> const& coeff) noexcept;
    Coeff<Number>(Coeff<Number>&& coeff) noexcept;

    Coeff<Number>& operator=(Coeff<Number> const& coeff) noexcept;
    Coeff<Number>& operator=(Coeff<Number>&& coeff) noexcept;

    friend Coeff<Number> operator+<Number>(Coeff<Number> const& a, Coeff<Number> const& b);
    friend Coeff<Number> operator-<Number>(Coeff<Number> const& a, Coeff<Number> const& b);
    friend Coeff<Number> operator*<Number>(Coeff<Number> const& a, Coeff<Number> const& b);
//    friend Coeff<Number> operator/<Number>(Coeff<Number> const& a, Coeff<Number> const& b);

    Coeff<Number>& operator+=(Coeff<Number> const& a);
    Coeff<Number>& operator-=(Coeff<Number> const& a);
    Coeff<Number>& operator*=(Coeff<Number> const& a);
//    Coeff<Number>& operator/=(Coeff<Number> const& a);

    Coeff<Number> operator+() const;
    Coeff<Number> operator-() const;

    Coeff<Number>& add(Coeff<Number> const& coeff); //adds to m_coeffs

    Coeff<Number>& putOut(Number const& num); //divides multipliers in set and multiplies multiplier by num
    Coeff<Number>& putIn(Number const& num); //divides multiplier and multiplies multipliers in set by num

    Coeff<Number>& apply(String const& variable, Number const& value);
    Coeff<Number>& apply(MapOfV const& values);

    bool ready() const; //true if contains no variable

    Number calculate() const;
    Number calculate(MapOfV const& values) const;

    Number getMulti() const;
    String getVar() const;
    SetOfC getCoeffs() const;

    SetOfV getAllVars() const;

    String toString(int type = display_t) const;

    friend std::ostream& operator<<<Number>(std::ostream& stream, Coeff<Number> const& coeff);
    friend std::istream& operator>><Number>(std::istream& stream, Coeff<Number>& coeff);
};

#include "Coeff.tpp"

#endif //PFDCALCULATOR_COEFF_H
