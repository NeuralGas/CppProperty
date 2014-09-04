#pragma once

#include <functional>

using namespace std;

template<typename T, typename TGet = function<T const(void)>, typename TSet = function<void(T const&)>>
class Property
{
    static void empty_setter(T const&) {}

    TGet const getter;
    TSet const setter;
public:
    Property(TGet getter, TSet setter = empty_setter)
        : getter(getter)
        , setter(setter)
    {
    }

    T const toT() const
    {
        return getter();
    }

    operator T () const
    {
        return getter();
    }

    // -- Arithmetic Operators -----------------------------------------------------------------------

    T const operator=(T const& new_value)
    {
        setter(new_value);
        return getter();
    }

    template <typename TDelta>
    T const operator +(TDelta delta) const
    {
        T new_value = getter() + delta;
        // don't need to set as this operation does not modify value
        return new_value;
    }

    template <typename TDelta>
    T const operator -(TDelta delta) const
    {
        T new_value = getter() - delta;
        // don't need to set as this operation does not modify value
        return new_value;
    }

    T const operator +() const
    {
        T t = getter();
        return +t;
    }

    T const operator -() const
    {
        T t = getter();
        return -t;
    }

    template <typename TMult>
    T const operator *(TMult mult) const
    {
        T new_value = getter() * mult;
        // don't need to set as this operation does not modify value
        return new_value;
    }

    template <typename TDiv>
    T const operator /(TDiv div) const
    {
        T new_value = getter() / div;
        // don't need to set as this operation does not modify value
        return new_value;
    }

    template <typename TS>
    T const operator %(TS v) 
    {
        return getter()%v;
    }

    T operator++(int)
    {
        T initial = getter();
        setter(initial+1);
        return initial;
    }
    T operator++()
    {
        return this->operator+=(1);
    }

    T operator--(int)
    {
        T initial = getter();
        setter(initial-1);
        return initial;
    }
    T operator--()
    {
        return this->operator-=(1);
    }

    // -- Comparison/Relational Operators --------------------------------------------------
    template <typename TOther>
    bool operator ==(TOther const& other) const
    {
        return getter() == other;
    }
    template <typename TOther>
    bool operator !=(TOther const& other) const
    {
        return getter() != other;
    }
    template <typename TOther>
    bool operator <(TOther const& other) const
    {
        return getter() < other;
    }
    template <typename TOther>
    bool operator >(TOther const& other) const
    {
        return getter() > other;
    }
    template <typename TOther>
    bool operator <=(TOther const& other) const
    {
        return getter() <= other;
    }
    template <typename TOther>
    bool operator >=(TOther const& other) const
    {
        return getter() >= other;
    }

    // -- Logical Operators--------------------------------------------------------------------------

    bool operator !() const
    {
        return !getter();
    }

    template <typename TOther>
    bool operator &&(TOther const& other) const
    {
        return getter() && other;
    }

    template <typename TOther>
    bool operator ||(TOther const& other) const
    {
        return getter() || other;
    }

    // -- Compound Assignment Operators ------------------------------------------------------------

    template<typename TDelta>
    T const operator+=(TDelta delta)
    {
        T new_value = getter() + delta;
        setter(new_value);
        return new_value;
    }

    template<typename TDelta>
    T const operator-=(TDelta delta)
    {
        return operator+=(-delta);
    }

    template<typename TMult>
    T const operator*=(TMult multiplier)
    {
        T new_value = getter() * multiplier;
        setter(new_value);
        return new_value;
    }

    template<typename TDiv>
    T const operator/=(TDiv div)
    {
        T new_value = getter() / div;
        setter(new_value);
        return new_value;
    }
};

template<typename T>
auto make_property(T& value)
{
    return Property<T>([&]() -> T const { return value; }, [&](T v) { value = v; });
}