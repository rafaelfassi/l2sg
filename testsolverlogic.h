#ifndef TESTSOLVERLOGIC_H
#define TESTSOLVERLOGIC_H

#include <QString>

class TestSolverLogic
{
public:
    TestSolverLogic();
    bool execTest();

private:
    bool testUnit(const QString &unitName, int level, int *in, int *out = 0);
    void printRes(const QString &errorMsg = QString());
};

#endif // TESTSOLVERLOGIC_H
