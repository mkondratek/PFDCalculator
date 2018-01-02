//
// Created by mkondratek on 26.11.17.
//

#ifndef PFDCALCULATOR_POLY_H
#define PFDCALCULATOR_POLY_H

#include <vector>
#include "Coeff.h"

#define DEBUG_MODE true

/** declaration begin **/
template<typename Number>
class Poly;

template<typename Number>
std::ostream& operator<<(std::ostream& stream, Poly<Number> const& Poly);
template<typename Number>
std::istream& operator>>(std::istream& stream, Poly<Number>& Poly);
template<typename Number>
Poly<Number> operator+(Poly<Number> const& a, Poly<Number> const& b);
template<typename Number>
Poly<Number> operator-(Poly<Number> const& a, Poly<Number> const& b);
template<typename Number>
Poly<Number> operator*(Poly<Number> const& a, Poly<Number> const& b);
//template<typename Number>
//Poly<Number> operator/(Poly<Number> const& a, Poly<Number> const& b);
/**  declaration end  **/

template <typename Number>
class Poly {

public:

    using String = std::string;
    using Cff    = Coeff<Number>;
    using Numvec = std::vector<Number>;
    using Cffvec = std::vector<Cff>;

    /* DISPLAY PARAMS */
    enum { ASC = 32, DESC = 64, ROOTS = 128, EXT = 256 };

private:

    Cffvec m_monomials;
    Numvec m_roots;

    static int display_t;

    bool m_roots_valid;
    Poly<Number>& updateRoots();

public:

    static Poly parse(String const& str);

    Poly() noexcept;

    explicit Poly(Number const& number, unsigned int exp = 0) noexcept;
    explicit Poly(Cff const& cff, unsigned int exp = 0) noexcept ;

    explicit Poly(Numvec const& roots) noexcept ;
    explicit Poly(Cffvec const& roots) noexcept ;

    Poly<Number>(Poly<Number> const& poly) noexcept;
    Poly<Number>(Poly<Number>&& poly) noexcept;

    Poly<Number>& operator=(Poly<Number> const& poly) noexcept;
    Poly<Number>& operator=(Poly<Number>&& poly) noexcept;

    friend Poly<Number> operator+<Number>(Poly<Number> const& a, Poly<Number> const& b);
    friend Poly<Number> operator-<Number>(Poly<Number> const& a, Poly<Number> const& b);
    friend Poly<Number> operator*<Number>(Poly<Number> const& a, Poly<Number> const& b);
//    friend Poly<Number> operator/<Number>(Poly<Number> const& a, Poly<Number> const& b);

    Poly<Number>& operator+=(Poly<Number> const& a);
    Poly<Number>& operator-=(Poly<Number> const& a);
    Poly<Number>& operator*=(Poly<Number> const& a);
//    Poly<Number>& operator/=(Poly<Number> const& a);

    Poly<Number> operator+() const;
    Poly<Number> operator-() const;

    Cffvec getCoeffs() const;
    Numvec getRoots() const;

    Poly<Number>& apply(typename Cff::String const& variable, Number const& value);
    Poly<Number>& apply(typename Cff::MapOfV const& values);

    Number calculate(Number const& number) const;
    Number calculate(Number const& number, typename Cff::MapOfV const& values) const;

    Cff getCoeff(unsigned int exp) const;

    unsigned int getDegree() const;

    String toString(int type = display_t) const;

    friend std::ostream& operator<< <Number>(std::ostream& stream, Poly<Number> const& poly);
    friend std::istream& operator>> <Number>(std::istream& stream, Poly<Number>& poly);
};

#include "Poly.tpp"

#endif //PFDCALCULATOR_POLY_H
