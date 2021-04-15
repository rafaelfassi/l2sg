#ifndef TESTSOLVERLOGIC_H
#define TESTSOLVERLOGIC_H

#include <string>

class TestSolverLogic
{
public:
    TestSolverLogic();
    bool execTest();

private:
    bool testUnit(const std::string &unitName, int level, int *in, int *out = 0);
    void printMsg(const std::string &msg);
    void printMsg(const std::string &testName, const std::string &msg);
};

#endif // TESTSOLVERLOGIC_H
