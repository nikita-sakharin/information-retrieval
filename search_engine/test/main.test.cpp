#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestResult.h>
#include <cppunit/ui/text/TextTestRunner.h>

using namespace std;

class main_test : public CppUnit::TestCase {
public:
    main_test(string name) : CppUnit::TestCase(name) {
    }

    void runTest() override {
        string s = "";
        cerr << "line 13\n";
        CPPUNIT_ASSERT_MESSAGE("empty string must have zero size", s.size() != 0);
    }
/*
    void runTest() override {
        CPPUNIT_ASSERT(Complex (10, 1) == Complex (10, 1));
        CPPUNIT_ASSERT(!(Complex (1, 1) == Complex (2, 2)));
    }
*/
};

int main() {
    // CppUnit::TextTestRunner runner;
    CppUnit::TestSuite suite;
    suite.addTest(new main_test("this is simple test"));
    CppUnit::TestResult result;
    suite.run(&result);
    cerr << "result = " << result << endl;
    cerr << "line 23" << endl;

    return 0;
}
