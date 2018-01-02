//
// Created by mkondratek on 27.12.17.
//

template<typename Number>
const typename Coeff<Number>::String Coeff<Number>::NONVAR = "";

template <typename Number>
Coeff<Number>::SetOfC::SetOfC() {}

template <typename Number>
Coeff<Number>::SetOfC::SetOfC(Coeff<Number> const& coeff) {
    add(coeff);
}

template <typename Number>
Coeff<Number>::SetOfC::SetOfC(std::initializer_list<Coeff<Number>> il) {
    for (auto & c : il) {
        add(c);
    }
}

template <typename Number>
typename Coeff<Number>::SetOfC &Coeff<Number>::SetOfC::add(Coeff<Number> const& coeff) {
    if (DEBUG_MODE) {
        std::cout << "add ( " << coeff << " )\n";
    }

    if (coeff.m_multiplier != Number(0) || m_coeffs.empty()) {
        if (m_coeffs.size() == 1 && m_coeffs.begin()->m_multiplier == Number(0)) {
            m_coeffs.clear();
        }

        auto it = m_coeffs.find(coeff);
        if (it == m_coeffs.end()) {
            if (DEBUG_MODE) {
                std::cout << "  not here\n";
            }

            m_coeffs.insert(coeff);
        } else {
            Coeff<Number> tmp(Number(1));

            if (DEBUG_MODE) {
                std::cout << "  here ";
            }

            tmp = *it + coeff;

            if (DEBUG_MODE) {
                std::cout << "  " << *it << " -> " << coeff << "\n";
            }

            m_coeffs.erase(it);
            add(tmp);
        }
    }

    return *this;
}

template<typename Number>
std::set<Coeff<Number>>& Coeff<Number>::SetOfC::asSet() const {
    return m_coeffs;
}

template<typename Number>
int Coeff<Number>::display_t = TIGHT;

template<typename Number>
Coeff<Number>& Coeff<Number>::fix() {
    /** remove coeffs with 0 multiplier **/

    if (DEBUG_MODE) {
        std::cout << "fixing ( " << *this << " )";
    }

    if (m_multiplier == Number(0)) {
        if (DEBUG_MODE) {
            std::cout << " -> mul = 0 ";
        }

        m_coeffs.asSet().clear();
        m_variable = NONVAR;
    } else {
        if (DEBUG_MODE) {
            std::cout << " -> els ";
        }

        for (auto it = m_coeffs.asSet().begin(); it != m_coeffs.asSet().end() && m_coeffs.asSet().size() > 1;) {
            if (DEBUG_MODE) {
                std::cout << "\n    child = " << *it;
            }

            if (it->m_multiplier == Number(0)) {
                it = m_coeffs.asSet().erase(it);
            } else { ++it; }
        }

        if (DEBUG_MODE) {
            std::cout << "\n      | " << *this << std::endl;
        }
    }

    /** connect single child with parent if at least one have no variable **/
    if (m_coeffs.asSet().size() == 1) {
        if (DEBUG_MODE) {
            std::cout << "  single ";
        }

        if (m_coeffs.asSet().begin()->m_multiplier == Number(0)) {
            m_coeffs.asSet().clear();
            m_variable = NONVAR;
            m_multiplier = 0;

        } else if (m_variable == NONVAR || m_coeffs.asSet().begin()->m_variable == NONVAR) {
            Coeff tmp = *m_coeffs.asSet().begin();
            m_multiplier *= tmp.m_multiplier;
            m_coeffs = tmp.m_coeffs;
            m_variable = (m_variable == NONVAR ? tmp.m_variable : m_variable);

        } else if (m_coeffs.asSet().begin()->m_variable < m_variable) {
            Coeff<Number> tmp_cff(m_variable, m_coeffs.asSet().begin()->m_coeffs);
            m_multiplier *= m_coeffs.asSet().begin()->m_multiplier;
            m_variable = m_coeffs.asSet().begin()->m_variable;
            m_coeffs.asSet().clear();
            m_coeffs.asSet().insert(tmp_cff);
        }
    }

    if (DEBUG_MODE) {
        std::cout << "  result: " << *this << std::endl;
    }

    return *this;
}

template<typename Number>
Coeff<Number> Coeff<Number>::parse(String str) {
    const String split_points = "-+";

    if (DEBUG_MODE) {
        std::cout << "parse( \"" << str << "\"";
    }

    if (str == "") {
        str = "0";
    }

    bool needsParentheses = false;
    int deep = 0;

    for (unsigned long int i = 0; i < str.length(); ++i) {
        deep += (str[i] == '(');
        deep -= (str[i] == ')');
        if (deep == 0 && split_points.find(str[i]) != String::npos && i != 0) {
            needsParentheses = true;
            break;
        }
    }

    if (needsParentheses) {
        str = "(" + str + ")";
    }

    if (DEBUG_MODE) {
        std::cout << " -> \"" << str << "\" )\n";
    }

    Coeff<Number> result;

    auto simpleParse = [](String s) -> Coeff<Number> {
        if (DEBUG_MODE) {
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

        if (DEBUG_MODE) {
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

    unsigned long int u_beg = 0;
    unsigned long int u_end = 0;

    u_end = str.find('(');

    u_end = (u_end == String::npos ? str.length() : u_end);

    result = simpleParse(str.substr(0, u_end));

    if (DEBUG_MODE) {
        std::cout << "  '(' at " << u_end << "/" << str.length() << std::endl;
        std::cout << "  result: " << result << std::endl;
    }

    u_beg = u_end + 1;
    for (unsigned long int i = u_beg; i < str.length() - 1; ++i) {
        u_end = i;

        deep = 0;
        while (u_end < str.length() && (deep != 0 || str[u_end] != ')') &&
               ((split_points.find(str[u_end]) == String::npos && deep == 0)
                || deep > 0 || u_beg == u_end)) {
            deep += (str[u_end] == '(');
            deep -= (str[u_end] == ')');

            u_end++;
        }

        if (DEBUG_MODE) {
            std::cout << "  beg: " << u_beg << " end: " << u_end << std::endl;
        }

        if (str[u_beg] == '-') {
            if (u_beg - 1 < 0 || str[u_beg - 1] == '(') {  //unary -

                if (DEBUG_MODE) {
                    std::cout << "  unary-\n";
                }

                Coeff<Number> tmp = parse(str.substr(u_beg, u_end - u_beg));
                result.add(tmp);
            } else { //binary -

                if (DEBUG_MODE) {
                    std::cout << "  binary-\n";
                }

                Coeff<Number> tmp = parse(str.substr(u_beg, u_end - u_beg));
                result.add(tmp);
            }
        } else if (str[u_beg] == '+') {
            if (u_beg - 1 < 0 || str[u_beg - 1] == '(') {  //unary +

                if (DEBUG_MODE) {
                    std::cout << "  unary+\n";
                }

                Coeff<Number> tmp = parse(str.substr(u_beg, u_end - u_beg));
                result.add(tmp);
            } else { //binary +

                if (DEBUG_MODE) {
                    std::cout << "  binary+\n";
                }

                Coeff<Number> tmp = parse(str.substr(u_beg, u_end - u_beg));
                result.add(tmp);
            }
        } else {
            Coeff<Number> tmp = parse(str.substr(u_beg, u_end - u_beg));
            result.add(tmp);
        }

        i = u_end;
        u_beg = i;
    }

    result.fix();

    return result;
}

template <typename Number>
Coeff<Number> Coeff<Number>::getRandom(unsigned long int seed) {
    String var = " ";

    var[0] = (rand() % 4 + 'a');

    Coeff<Number> result((Number(rand() % 300 + 100) / Number(100))
                         * (rand() % 2 == 0 ? Number(-1) : Number(1)), var);

    if (seed > 0) {
        unsigned long children = rand() % (seed / 2 + 2);

        while (children--) {
            if ((rand() % 10) < 7) {
                result.add(getRandom(seed / 3 + children));
            }
        }
    }

    if (result.toString().length() < seed / 2) {
        result = getRandom(seed);
    }

    return result;
}

template<typename Number>
bool Coeff<Number>::operator<(Coeff<Number> const& other) const {
    return m_variable < other.m_variable;
}

template<typename Number>
bool Coeff<Number>::operator==(Coeff<Number> const& other) const {
    return !(*this < other || other < *this);
}

template<typename Number>
Coeff<Number>::Coeff(const Number& multiplier,
                     const Coeff::String& variable,
                     const Coeff<Number>::SetOfC& coeffs) noexcept
        : m_multiplier(multiplier), m_variable(variable), m_coeffs(coeffs) { fix(); }

template<typename Number>
Coeff<Number>::Coeff(const Number& multiplier,
                     const Coeff::String& variable) noexcept
        : m_multiplier(multiplier), m_variable(variable) { fix(); }

template<typename Number>
Coeff<Number>::Coeff(const Number& multiplier,
                     const Coeff<Number>::SetOfC& coeffs) noexcept
        : m_multiplier(multiplier), m_variable(NONVAR), m_coeffs(coeffs) { fix(); }

template<typename Number>
Coeff<Number>::Coeff(const Coeff::String& variable,
                     const Coeff<Number>::SetOfC& coeffs) noexcept
        : m_multiplier(Number(1)), m_variable(variable), m_coeffs(coeffs) { fix(); }

template<typename Number>
Coeff<Number>::Coeff(const Number& multiplier) noexcept
        : m_multiplier(multiplier), m_variable(NONVAR) {}

template<typename Number>
Coeff<Number>::Coeff(const Coeff::String& variable) noexcept
        : m_multiplier(Number(1)), m_variable(variable) {}

template<typename Number>
Coeff<Number>::Coeff(const Coeff<Number>::SetOfC& coeffs) noexcept
        : m_multiplier(Number(1)), m_variable(NONVAR), m_coeffs(coeffs) { fix(); }

template<typename Number>
Coeff<Number>::Coeff() noexcept : m_multiplier(Number(0)), m_variable(NONVAR) {}

template<typename Number>
Coeff<Number>::Coeff(const Coeff<Number>& coeff) noexcept {
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
Coeff<Number>::Coeff(Coeff<Number>&& coeff) noexcept {
    if (&coeff == this) {
        m_multiplier = Number(1);
        m_variable = NONVAR;
    } else {
        m_multiplier = std::move(coeff.m_multiplier);
        m_variable = coeff.m_variable;
        m_coeffs = coeff.m_coeffs;
    }
}

template<typename Number>
Coeff<Number>& Coeff<Number>::operator=(const Coeff<Number>& coeff) noexcept {
    if (&coeff != this) {
        m_multiplier = coeff.m_multiplier;
        m_variable = coeff.m_variable;
        m_coeffs = coeff.m_coeffs;
    }
    return *this;
}

template<typename Number>
Coeff<Number>& Coeff<Number>::operator=(Coeff<Number>&& coeff) noexcept {
    if (&coeff != this) {
        m_multiplier = std::move(coeff.m_multiplier);
        m_variable = std::move(coeff.m_variable);
        m_coeffs = std::move(coeff.m_coeffs);
    }
    return *this;;
}

template<typename Number>
Coeff<Number> operator+(Coeff<Number> const& a, Coeff<Number> const& b) {

    Coeff<Number> x = a;
    Coeff<Number> y = b;

    if (DEBUG_MODE) {
        std::cout << "operator+ ( " << x << " | " << y << " )";
    }

    Coeff<Number> result(Number(1));

    if (x.ready() && x.calculate() == Number(0)) {
        if (DEBUG_MODE) {
            std::cout << " -> x == 0\n";
        }
        result = y;
    } else if (y.ready() && y.calculate() == Number(0)) {
        if (DEBUG_MODE) {
            std::cout << " -> y == 0\n";
        }
        result = x;
    } else {

        if (x.m_coeffs.asSet().empty() && !y.m_coeffs.asSet().empty()) {
            x.m_coeffs.asSet().insert(Coeff<Number>(1));
        } else if (!x.m_coeffs.asSet().empty() && y.m_coeffs.asSet().empty()) {
            y.m_coeffs.asSet().insert(Coeff<Number>(1));
        }

        /* make coeffs with nonempty set having same multiplier */
        if (x.m_multiplier != y.m_multiplier && !x.m_coeffs.asSet().empty()) {
            if (x.m_multiplier != Number(0)) { x.putIn(x.m_multiplier); }
            if (y.m_multiplier != Number(0)) { y.putIn(y.m_multiplier); }
        }

        if (x.m_variable == y.m_variable && !x.m_coeffs.asSet().empty()) {

            if (DEBUG_MODE) {
                std::cout << " -> = !ept\n";
                std::cout << "  " << x << " | " << y << "\n";
            }

            result = y;

            for (const auto &c : x.m_coeffs.asSet()) { result.add(c); }

        } else if (x.m_variable == y.m_variable && x.m_coeffs.asSet().empty()) {

            if (DEBUG_MODE) {
                std::cout << " -> = ept\n";
            }

            result = x;
            result.m_multiplier += y.m_multiplier;
        } else {

            if (DEBUG_MODE) {
                std::cout << " -> els\n";
            }

            result.add(x).add(y).fix();
        }
    }

    if (DEBUG_MODE) {
        std::cout << "  " << x << " + " << y << " = " << result << std::endl;
    }

    return result;
}

template<typename Number>
Coeff<Number> operator-(Coeff<Number> const& a, Coeff<Number> const& b) {
    if (DEBUG_MODE) {
        std::cout << "operator- ( " << a << " | " << b << ")\n";
    }

    return a + (-b);
}

template<typename Number>
Coeff<Number> operator*(Coeff<Number> const& a, Coeff<Number> const& b) {
    Coeff<Number> result(Number(1));

    Coeff<Number> x = a;
    Coeff<Number> y = b;

    if (DEBUG_MODE) {
        std::cout << "operator* ( " << x << " | " << y  << " )";
    }

    if (y < x) {
        std::swap(x, y);

        if (DEBUG_MODE) {
            std::cout << " swaping";
        }
    }

    if (x.m_multiplier == Number(0) || y.m_multiplier == Number(0)) {

        if (DEBUG_MODE) {
            std::cout << " -> 0 | 0\n";
        }

        result.m_multiplier = 0; //returns 0
    } else if (x.m_variable == Coeff<Number>::NONVAR && x.m_coeffs.asSet().empty()) {

        if (DEBUG_MODE) {
            std::cout << " -> 1 * y\n";
        }

        result = y;
        result.m_multiplier *= x.m_multiplier;
    } else if (y.m_variable == Coeff<Number>::NONVAR && y.m_coeffs.asSet().empty()) {

        if (DEBUG_MODE) {
            std::cout << " -> x * 1\n";
        }

        result = x;
        result.m_multiplier *= y.m_multiplier;
    } else if (x.m_coeffs.asSet().empty() && !y.m_coeffs.asSet().empty()) {

        if (DEBUG_MODE) {
            std::cout << " -> x ept\n";
        }

        result = x;
        result.add(y);
        result.putOut(y.m_multiplier);
    } else if (!x.m_coeffs.asSet().empty() && y.m_coeffs.asSet().empty()) {

        if (DEBUG_MODE) {
            std::cout << " -> y ept\n";
        }

        result = y;
        result.add(x);
        result.putOut(x.m_multiplier);
    } else if (x.m_coeffs.asSet().empty() && y.m_coeffs.asSet().empty()) {

        if (DEBUG_MODE) {
            std::cout << " -> both ept\n";
        }

        result = x;
        result.add(y);
        result.putOut(y.m_multiplier);

    } else {

        if (DEBUG_MODE) {
            std::cout << " -> both !ept\n";
        }

        result.m_multiplier = x.m_multiplier * y.m_multiplier;
        result.m_variable = x.m_variable;
        Coeff<Number> inner(y.m_variable);

        for (auto & c_a : x.m_coeffs.asSet()) {
            for (auto & c_b : y.m_coeffs.asSet()) {
                inner.add(c_a * c_b);
                if (DEBUG_MODE) {
                    std::cout << "  then " << inner << "    ";
                }
            }
        }

        if (DEBUG_MODE) {
            std::cout << std::endl;
        }

        result.add(inner);
    }

    result.fix();

    if (DEBUG_MODE) {
        std::cout << "  " << x << " * " << y << " = " << result << std::endl;
    }

    return result;
}

//template<typename Number>
//Coeff<Number> operator/(const Coeff<Number>& a, const Coeff<Number>& b) {
//    return Coeff<Number>();
//}

template<typename Number>
Coeff<Number>& Coeff<Number>::operator+=(Coeff<Number> const& a) {
    *this = *this + a;
    return *this;
}

template<typename Number>
Coeff<Number>& Coeff<Number>::operator-=(Coeff<Number> const& a) {
    *this = *this - a;
    return *this;
}

template <typename Number>
Coeff<Number>& Coeff<Number>::operator*=(Coeff<Number> const& a) {
    *this = *this * a;
    return *this;
}

//template <typename Number>
//Coeff<Number>& Coeff<Number>::operator/=(const Coeff<Number>& a) {
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
Coeff<Number>& Coeff<Number>::add(Coeff<Number> const& coeff) {
    if (DEBUG_MODE) {
        std::cout << "add ( " << coeff << " ) to ( " << *this << " )\n";
    }

    m_coeffs.add(coeff);

    return *this;
}

template<typename Number>
Coeff<Number>& Coeff<Number>::putOut(Number const& num) {
    if (!m_coeffs.asSet().empty()) {
        for (auto &c : m_coeffs.asSet()) {
            c.m_multiplier /= num;
        }
        m_multiplier *= num;
    }

    return *this;
}

template<typename Number>
Coeff<Number>& Coeff<Number>::putIn(Number const& num) {
    if (!m_coeffs.asSet().empty()) {
        for (auto &c : m_coeffs.asSet()) {
            c.m_multiplier *= num;
        }
        m_multiplier /= num;
    }

    return *this;
}

template<typename Number>
Coeff<Number>& Coeff<Number>::apply(Coeff<Number>::String const& variable, Number const& value) {
    if (DEBUG_MODE) {
        std::cout << "apply " << variable << " = " << value << " for " << *this << "\n";
    }

    if (m_variable == variable) {
        m_multiplier *= value;
        m_variable = NONVAR;
    }

    Coeff<Number> tmp_cff;
    for (auto c : m_coeffs.asSet()) {
        c.apply(variable, value);
        tmp_cff.add(c);
    }

    if (!m_coeffs.asSet().empty() && tmp_cff.m_coeffs.asSet().empty()) {
        //m_coeffs sumed to 0 after apply
        m_multiplier = Number(0);
        m_variable = NONVAR;
    }

    m_coeffs = tmp_cff.m_coeffs;

    if (DEBUG_MODE) {
        std::cout << *this << " -> ";
    }

    fix();

    if (DEBUG_MODE) {
        std::cout << *this << "\n";
    }

    return *this;
}

template<typename Number>
Coeff<Number>& Coeff<Number>::apply(MapOfV const& values) {
    for (auto const& v : values) {
        this->apply(v.first, v.second);
    }
    return *this;
}

template <typename Number>
bool Coeff<Number>::ready() const {
    return getAllVars().empty();
}

template<typename Number>
Number Coeff<Number>::calculate(MapOfV const& values) const {
    Coeff<Number> tmp(*this);
    tmp.apply(values);

    SetOfV v_s = tmp.getAllVars();
    if (!v_s.empty()) {
        String error_msg = "calculate error: Following variables have no values ";
        for (const auto &v : v_s) {
            error_msg.append("\'" + (v + (v == *v_s.rbegin() ? "'" : "', ")));
        }
        error_msg.append("\n");
        throw std::logic_error(error_msg);
    }

    return tmp.m_multiplier;
}

template <typename Number>
Number Coeff<Number>::calculate() const {
    return calculate(MapOfV());
}

template<typename Number>
Number Coeff<Number>::getMulti() const {
    return m_multiplier;
}

template<typename Number>
typename Coeff<Number>::String Coeff<Number>::getVar() const {
    return m_variable;
}

template<typename Number>
typename Coeff<Number>::SetOfC Coeff<Number>::getCoeffs() const {
    return m_coeffs;
}

template<typename Number>
typename Coeff<Number>::SetOfV Coeff<Number>::getAllVars() const {
    SetOfV out;

    if (!m_variable.empty()) {
        out.insert(m_variable);
    }

    for (const auto &c : m_coeffs.asSet()) {
        SetOfV tmp = c.getAllVars();
        out.insert(tmp.begin(), tmp.end());
    }

    return out;
}

template<typename Number>
typename Coeff<Number>::String Coeff<Number>::toString(int type) const {
    String out = NONVAR;

    if (m_multiplier != Number(0) || (type & FULL)) {
        std::stringstream ss;
        if ((m_variable != NONVAR || !m_coeffs.asSet().empty()) && !(type & FULL)) {
            if (m_multiplier == Number(-1)) {
                out.append("-");
                if ((type & BIN_MINUS)) {
                    out.append(" ");
                }
            }
            else if (m_multiplier == Number(1) && !(type & FULL)) {}
            else {
                if (m_multiplier < Number(0) && (type & BIN_MINUS)) {
                    ss << "- " << (m_multiplier * Number(-1));
                } else {
                    ss << m_multiplier;
                }
            }
        } else {
            if (m_multiplier < Number(0) && (type & BIN_MINUS)) {
                ss << "- " << (m_multiplier * Number(-1));
            } else {
                ss << m_multiplier;
            }
        }
        out.append(ss.str()).append(m_variable);

        if (!m_coeffs.asSet().empty()) {
            out.append("(");
            if (type & WIDE) {
                out.append(" ");
            }

            for (auto &c : m_coeffs.asSet()) {
                out.append((c == *m_coeffs.asSet().begin() || c.m_multiplier <= Number(0) ?
                          NONVAR : ((type & WIDE) ? "+ " : "+")) +
                         c.toString(type + ((c == *m_coeffs.asSet().begin() ||
                                 !(type & WIDE)) ? 0 : BIN_MINUS)));

                if (type & WIDE) {
                    out.append(" ");
                }
            }
            out.append(")");
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

template<typename Number>
std::ostream& operator<<(std::ostream &stream, Coeff<Number> const& coeff) {
    stream << coeff.toString();
    return stream;
}

template<typename Number>
std::istream& operator>>(std::istream &stream, Coeff<Number>& coeff) {
    typename Coeff<Number>::String str;

    stream >> str;
    coeff = Coeff<Number>::parse(str);

    return stream;
}