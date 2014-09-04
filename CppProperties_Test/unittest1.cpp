#include "stdafx.h"
#include "CppUnitTest.h"
#include "../CppProperties/Property.h"

#include <type_traits>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CppProperties_Test
{	
    class ComplexType
    {
        double sum_of_values() const { return b + a; }
    public:
        int a = 0;
        float b = 0;

        ComplexType() {}
        ComplexType(int a, float b) : a(a), b(b) {}
        ComplexType operator+=(int delta)
        {
            a += delta;
            b += delta;
            return *this;
        }
        ComplexType const operator+()
        {
            a = 55;
            b = 22;
            return *this;
        }

        ComplexType const operator-()
        {
            a = 44;
            b = 33;
            return *this;
        }

        bool operator==(ComplexType const& other) const
        {
            return (a == other.a) && (b == other.b);
        }

        bool operator!=(ComplexType const& other) const
        {
            return !(this->operator==(other));
        }

        bool operator<(ComplexType const& other) const
        {
            return this->sum_of_values() < other.sum_of_values();
        }

        bool operator>(ComplexType const& other) const
        {
            return this->sum_of_values() > other.sum_of_values();
        }

        bool operator>=(ComplexType const& other) const
        {
            return this->operator>(other)||this->operator==(other);
        }

        bool operator<=(ComplexType const& other) const
        {
            return this->operator<(other)||this->operator==(other);
        }

        operator bool() const
        {
            return !((a==0) && (b==0));
        }

        bool operator !() const
        {
            return !(bool) (*this);
        }

        bool operator&&(ComplexType const& other) const
        {
            return ((bool) *this) && ((bool) other);
        }
        bool operator||(ComplexType const& other) const
        {
            return ((bool) *this) || ((bool) other);
        }


    };

	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(ConversionOperatorT_ReturnsTValueAsExpected)
		{
            int value = 4;
            auto p = make_property_doubler<int>(value);

            Assert::AreEqual(8, p.toT());
		}

        TEST_METHOD(ConversionOperatorT_ReturnsTByConstValue)
        {
            // No need to test this, as cast to a non reference type is always a prvalue (not modifiable).
        }


        TEST_METHOD(CopyAssignment_ModifiesPropertyValueAsExpected)
        {
            int value = 4;
            auto p = make_property(value);
            p = 5;

            Assert::AreEqual(5, p.toT());
        }

        TEST_METHOD(CopyAssignment_ModifiesStoredValueAsExpected)
        {
            int value = 1;
            auto p = make_property_doubler<double>(value);
            p = 9;

            Assert::AreEqual(4, value);
        }

        TEST_METHOD(CopyAssignment_ReturnsNewValueByConstValue)
        {
            ComplexType _value;
            auto p = make_property(_value);
            Assert::IsTrue(is_const<decltype(p = ComplexType())>::value);
            Assert::IsFalse(is_reference<decltype(p = ComplexType())>::value);
        }

        TEST_METHOD(AdditionOperator_ReturnsExpectedValue)
        {
            double value = 3;
            auto p = make_property_doubler<double>(value);
            Assert::AreEqual(7.2, p + 1.2);
        }

        TEST_METHOD(AdditionOperator_ReturnsNewValueByConstValue)
        {
            ComplexType _value;
            auto p = make_property(_value);
            Assert::IsTrue(is_const<decltype(p + 2)>::value);
            Assert::IsFalse(is_reference<decltype(p + 2)>::value);
        }

        TEST_METHOD(SubtractionOperator_ReturnsExpectedValue)
        {
            double value = 3;
            auto p = make_property_doubler<double>(value);
            Assert::AreEqual(4.8, p - 1.2);
        }

        TEST_METHOD(SubtractionOperator_ReturnsNewValueByConstValue)
        {
            ComplexType _value;
            auto p = make_property(_value);
            Assert::IsTrue(is_const<decltype(p - 2)>::value);
            Assert::IsFalse(is_reference<decltype(p - 2)>::value);
        }

        TEST_METHOD(UnaryPlusOperator_ReturnsUnaryPlusOfPropertyValueAsDefinedByPropertyType)
        {
            double _stored = 3;
            auto p = Property<ComplexType>(
                []() {
                return ComplexType(2, 3);
            },
                [&](ComplexType const& value) {
                _stored = value.a + value.b;
            });
            p = ComplexType(1, 1);
            Assert::AreEqual(55, (+p).a);
            Assert::AreEqual(22.0f, (+p).b);
        }

        TEST_METHOD(UnaryMinusOperator_ReturnsUnaryPlusOfPropertyValueAsDefinedByPropertyType)
        {
            double _stored = 3;
            auto p = Property<ComplexType>(
                []() {
                return ComplexType(2, 3);
            },
                [&](ComplexType const& value) {
                _stored = value.a + value.b;
            });
            Assert::AreEqual(44, (-p).a);
            Assert::AreEqual(33.0f, (-p).b);
        }

        TEST_METHOD(MultiplicationOperator_ReturnsExpectedValue)
        {
            double value = 2.5;
            auto p = make_property_doubler<double>(value);
            Assert::AreEqual(6.0, p * 1.2);
        }

        TEST_METHOD(MultiplicationOperator_ReturnsNewValueByConstValue)
        {
            ComplexType _value;
            auto p = make_property(_value);
            Assert::IsTrue(is_const<decltype(p * 2)>::value);
            Assert::IsFalse(is_reference<decltype(p * 2)>::value);
        }

        TEST_METHOD(DivisionOperator_ReturnsExpectedValue)
        {
            double value = 3;
            auto p = make_property_doubler<double>(value);
            Assert::AreEqual(5.0, p / 1.2);
        }

        TEST_METHOD(DivisionOperator_ReturnsNewValueByConstValue)
        {
            ComplexType _value;
            auto p = make_property(_value);
            Assert::IsTrue(is_const<decltype(p / 2)>::value);
            Assert::IsFalse(is_reference<decltype(p / 2)>::value);
        }

        TEST_METHOD(ModuloOperator_ReturnsExpectedValue)
        {
            int value = 5;
            auto p = make_property_doubler<int>(value);
            Assert::AreEqual(1, p % 3);
        }

        TEST_METHOD(ModuloOperator_ReturnsNewValueByConstValue)
        {
            ComplexType _value;
            auto p = make_property(_value);
            Assert::IsTrue(is_const<decltype(p % 2)>::value);
            Assert::IsFalse(is_reference<decltype(p % 2)>::value);
        }

        TEST_METHOD(PostfixIncrement_IncrementsPropertyByOne_ReturnsInitialValue)
        {
            double _value = 2.5;
            auto p = make_property_doubler<int>(_value);

            auto return_val = p++;

            Assert::AreEqual(5, return_val);
            Assert::AreEqual(6, p.toT());
        }

        TEST_METHOD(PostfixIncrement_ModifiesStoredValueAsExpected)
        {
            double _value = 2.5;
            auto p = make_property_doubler<int>(_value);

            auto return_val = p++;

            Assert::AreEqual(3.0, _value); 
        }

        TEST_METHOD(PrefixIncrement_IncrementsPropertyByOne_ReturnsNewValue)
        {
            double value = 3;
            auto p = make_property(value);
            auto return_val = ++p;
            Assert::AreEqual(4.0, return_val);
            Assert::AreEqual(4.0, p.toT());
        }

        TEST_METHOD(PrefixIncrement_ModifiesStoredValueAsExpected)
        {
            double _value = 2.5;
            auto p = make_property_doubler<int>(_value);

            auto return_val = ++p;

            Assert::AreEqual(3.0, _value);  
        }

        TEST_METHOD(PostfixDecrement_DecrementsByOne_ReturnsInitialValue)
        {
            double value = 2;
            auto p = make_property(value);
            auto return_val = p--;
            Assert::AreEqual(2.0, return_val);
            Assert::AreEqual(1.0, (double) p);
        }

        TEST_METHOD(PostfixDecrement_ModifiesStoredValueAsExpected)
        {
            double _value = 2.5;
            auto p = make_property_doubler<int>(_value);

            auto return_val = p--;

            Assert::AreEqual(2.0, _value);  
        }

        TEST_METHOD(PrefixDecrement_DecrementsByOne_ReturnsNewValue)
        {
            double value = 3;
            auto p = make_property(value);
            auto return_val = --p;
            Assert::AreEqual(2.0, (double) p);
            Assert::AreEqual(2.0, return_val);
        }

        TEST_METHOD(PrefixDecrement_ModifiesStoredValueAsExpected)
        {
            double _value = 2.5;
            auto p = make_property_doubler<int>(_value);

            auto return_val = --p;

            Assert::AreEqual(2.0, _value);  
        }

        // -- Comparison/Relational Operators -----------------------------------------------------------

        TEST_METHOD(EqualToOperator_ReturnsValueAsDefinedByPropType)
        {
            auto p = Property<ComplexType>(
                [] {
                return ComplexType(2, 3);
            });
            Assert::IsTrue(p == ComplexType(2,3));
            Assert::IsFalse(p == ComplexType(3, 3));
        }

        TEST_METHOD(NotEqualToOperator_ReturnsValueAsDefinedByPropType)
        {
            auto p = Property<ComplexType>(
                [] {
                return ComplexType(2, 3);
            });
            Assert::IsFalse(p != ComplexType(2, 3));
            Assert::IsTrue(p != ComplexType(3, 3));
        }

        TEST_METHOD(LessThanOperator_ReturnsValueAsDefinedByPropType)
        {
            auto p = Property<ComplexType>(
                [] {
                return ComplexType(2, 3);
            });
            Assert::IsTrue(p < ComplexType(3, 3));
            Assert::IsFalse(p < ComplexType(2, 3));
        }

        TEST_METHOD(GreaterThanOperator_ReturnsValueAsDefinedByPropType)
        {
            auto p = Property<ComplexType>(
                [] {
                return ComplexType(2, 3);
            });
            Assert::IsFalse(p > ComplexType(3, 3));
            Assert::IsFalse(p > ComplexType(2, 3));
            Assert::IsTrue(p > ComplexType(0, 4));
        }

        TEST_METHOD(LessThanOrEqualToOperator_ReturnsValueAsDefinedByPropType)
        {
            auto p = Property<ComplexType>(
                [] {
                return ComplexType(2, 3);
            });
            Assert::IsTrue(p <= ComplexType(3, 3));
            Assert::IsTrue(p <= ComplexType(2, 3));
            Assert::IsFalse(p <= ComplexType(0, 5));
            Assert::IsFalse(p <= ComplexType(0, 4));
            Assert::IsFalse(p <= ComplexType(2, 2));
        }

        TEST_METHOD(GreaterThanOrEqualToOperator_ReturnsValueAsDefinedByPropType)
        {
            auto p = Property<ComplexType>(
                [] {
                return ComplexType(2, 3);
            });
            Assert::IsFalse(p >= ComplexType(3, 3));
            Assert::IsTrue(p >= ComplexType(2, 3));
            Assert::IsFalse(p >= ComplexType(0, 5));
            Assert::IsTrue(p >= ComplexType(0, 4));
            Assert::IsTrue(p >= ComplexType(2, 2));
        }


        // -- Logical Operators -----------------------------------------------------------------------------
        TEST_METHOD(NegationOperator_ReturnsValueDefinedByPropType)
        {
            ComplexType ct0s(0, 0);
            ComplexType ct5s(5, 5);

            auto p0s = make_property(ct0s);
            auto p5s = make_property(ct5s);

            Assert::IsTrue(!p0s);
            Assert::IsFalse(!p5s);
        }

        TEST_METHOD(AndOperator_ReturnsValueDefinedByPropType)
        {
            ComplexType ct0s(0, 0);
            ComplexType ct5s(5, 5);

            Assert::IsFalse(make_property(ct0s) && make_property(ct5s));
            Assert::IsFalse(make_property(ct0s) && make_property(ct0s));
            Assert::IsTrue(make_property(ct5s) && make_property(ct5s));
        }

        TEST_METHOD(OrOperator_ReturnsValueDefinedByPropType)
        {
            ComplexType ct0s(0, 0);
            ComplexType ct5s(5, 5);

            Assert::IsTrue(make_property(ct0s) || make_property(ct5s));
            Assert::IsFalse(make_property(ct0s) || make_property(ct0s));
            Assert::IsTrue(make_property(ct5s) || make_property(ct5s));
        }



        TEST_METHOD(SubtractOperator_SubtractsByValueProvided)
        {
            double value = 3;
            auto p = make_property(value);
            p = p - 1;
            Assert::AreEqual(2.0, (double) p);
            p = 0.1 - p;
            Assert::AreEqual(-1.9, (double) p);
        }
         
        TEST_METHOD(AdditionAssignment_ModifiesPropertyValueAsExpected)
        {
            double value = 3;
            auto p = make_property(value);
            p += 2;
            Assert::AreEqual(5.0, (double) p);
        }

        TEST_METHOD(AdditionAssignment_ModifiesStoredValueAsExpected)
        {
            double _value = 2.5;
            auto p = make_property_doubler<int>(_value);

            auto return_val = p += 2;

            Assert::AreEqual(3.5, _value); 
        }

        TEST_METHOD(AdditionAssignment_ReturnsNewValueByConstValue)
        {
            ComplexType _value;
            auto p = make_property(_value);
            Assert::IsTrue(is_const<decltype(p += 2)>::value);
            Assert::IsFalse(is_reference<decltype(p += 2)>::value);
        }


        TEST_METHOD(SubtractionAssignment_ModifiesPropertyValueAsExpected)
        {
            double value = 3;
            auto p = make_property_doubler<char>(value);
            p -= 2;
            Assert::AreEqual((char)4, p.toT());
        }

        TEST_METHOD(SubtractionAssignment_ModifiesStoredValueAsExpected)
        {
            double _value = 2.5;
            auto p = make_property_doubler<int>(_value);

            auto return_val = p -= 2;

            Assert::AreEqual(1.5, _value);
        }

        TEST_METHOD(SubtractionAssignment_ReturnsNewValueByConstValue)
        {
            ComplexType _value;
            auto p = make_property(_value);
            Assert::IsTrue(is_const<decltype(p -= 2)>::value);
            Assert::IsFalse(is_reference<decltype(p -= 2)>::value);
        }

        TEST_METHOD(MultiplicationAssignment_ModifiesPropertyValueAsExpected)
        {
            double value = 3;
            auto p = make_property(value);
            p *= 2;
            Assert::AreEqual(6.0, (double) p);
        }

        TEST_METHOD(MultiplicationAssignment_ModifiesStoredValueAsExpected)
        {
            double _value = 2.5;
            auto p = make_property_doubler<int>(_value);

            auto return_val = p *= 2;

            Assert::AreEqual(5.0, _value);
        }

        TEST_METHOD(MultiplicationAssignment_ReturnsNewValueByConstValue)
        {
            ComplexType _value;
            auto p = make_property(_value);
            Assert::IsTrue(is_const<decltype(p *= 2)>::value);
            Assert::IsFalse(is_reference<decltype(p *= 2)>::value);
        }

        TEST_METHOD(DivisionAssignment_ModifiesPropertyValueAsExpected)
        {
            double value = 3;
            auto p = make_property(value);
            p /= 2;
            Assert::AreEqual(1.5, (double) p);
        }

        TEST_METHOD(DivisionAssignment_ModifiesStoredValueAsExpected)
        {
            double _value = 2.5;
            auto p = make_property_doubler<int>(_value);

            auto return_val = p /= 2;

            Assert::AreEqual(1.0, _value);
        }

        TEST_METHOD(DivisionAssignment_ReturnsNewValueByConstValue)
        {
            ComplexType _value;
            auto p = make_property(_value);
            Assert::IsTrue(is_const<decltype(p /= 2)>::value);
            Assert::IsFalse(is_reference<decltype(p /= 2)>::value);
        }

        // ADD EQUALTIY COMAPRISON OPERATOR HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        TEST_METHOD(NotEqualTo)
        {
            double value = 4;
            auto p = make_property(value);
            Assert::IsTrue(p != 3.0);
        }

        TEST_METHOD(GreaterThan)
        {
            double value = 4;
            auto p = make_property(value);
            Assert::IsTrue(p > 3.0);
            Assert::IsFalse(p > 5.0);
        }

        TEST_METHOD(LessThan)
        {
            double value = 4;
            auto p = make_property(value);
            Assert::IsFalse(p < 3.0);
            Assert::IsTrue(p < 5.0);
        }

        TEST_METHOD(GreaterThanOrEqualTo)
        {
            double value = 2;
            auto p = make_property_doubler<int>(value);
            Assert::IsFalse(p >= 5.0);
            Assert::IsTrue(p >= 4.0);
            Assert::IsTrue(p >= 3.9);
        }
        
        TEST_METHOD(LessThanOrEqualTo)
        {
            double value = 4;
            auto p = make_property(value);
            Assert::IsTrue (p <= 5.0);
            Assert::IsTrue(p <= 4.0);
            Assert::IsFalse(p <= 3.9);
        }

        template<typename T, typename TStored>
        auto make_property_doubler(TStored& stored_value)
        {
            return Property<T>(
                [&]() 
            {
                return (T) (stored_value*2); 
            }, 
                [&](T value) 
            {
                stored_value = ((TStored) value)/2; 
            });
        }

	};
}