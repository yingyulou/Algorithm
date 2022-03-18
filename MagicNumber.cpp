#include <pprint/pprint>

using namespace std;
using pprint::print;

template <typename __T, typename __L, typename __R>
class __Plus
{
public:

    __Plus(const __L &lhs, const __R &rhs): __lhs(lhs), __rhs(rhs) {}


    __T operator()() const
    {
        return __lhs() + __rhs();
    }


private:

    const __L &__lhs;
    const __R &__rhs;
};


template <typename __T>
class __Scalar
{
public:

    __Scalar(const __T &val): __val(val) {}


    __Scalar<__T> &operator=(const __T &val)
    {
        __val = val;

        return *this;
    }


    __T operator()() const
    {
        return __val;
    }


private:

    __T __val;
};


template <typename __T, typename __E>
class __Expr
{
public:

    __Expr(const __E &expr): __expr(expr) {}


    template <typename __R>
    __Expr<__T, __Plus<__T, __Expr<__T, __E>, __R>> operator+(const __R &rhs)
    {
        return __Expr<__T, __Plus<__T, __Expr<__T, __E>, __R>>(__Plus<__T, __Expr<__T, __E>, __R>(*this, rhs));
    }


    __Expr<__T, __E> &operator=(const __T &val)
    {
        __expr = val;

        return *this;
    }


    __T operator()() const
    {
        return __expr();
    }


private:

    __E __expr;
};


template <typename __T, typename __E>
ostream &operator<<(ostream &os, const __Expr<__T, __E> &val)
{
    return os << val();
}


template <typename T>
using Number = __Expr<T, __Scalar<T>>;


int main()
{
    Number<int> a(2), b(4);

    auto c = a + b;

    print(c);  // 6

    b = 664;

    print(c);  // 666!
}
