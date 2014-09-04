// CppProperties.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Property.h"

using namespace std;

/* So we need two types of properties, 
   - one which proxies a reference (as current)
   - one which returns by copy (for when the value is calculated on the fly and there is no value to reference
   (except from the setter??) - could we return a reference to a property member variable that calls the setter when modified?
   (still need to think about what happens for "Prop<Complex> prop; prop().complex_prop_member = ..." when the complex type cannot be referenced) */

class TimePeriod
{
    double milliseconds = 56345.7;
public:
    Property<double> seconds;

    TimePeriod()
        : seconds([&]() { return this->milliseconds / 1000; }, [](double v) {})
    {
    }

};

int _tmain(int argc, _TCHAR* argv[])
{
    TimePeriod tp;

    cout << tp.seconds << endl;
	return 0;
}


