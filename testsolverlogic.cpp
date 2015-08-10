#include "testsolverlogic.h"
#include "grid.h"
#include "solverlogic.h"
#include "solverbruteforce.h"
#include <iostream>

TestSolverLogic::TestSolverLogic()
{

}

bool TestSolverLogic::execTest()
{
    // Facil ***********************************************************************
    {
        int in[9*9] =  {9,1,0,  2,0,8,  0,3,0,
                        0,0,3,  1,0,0,  0,8,5,
                        0,0,0,  0,3,4,  1,0,0,

                        0,7,2,  0,0,3,  4,9,6,
                        0,9,0,  0,0,0,  0,0,2,
                        3,5,4,  0,0,6,  7,0,8,

                        6,3,0,  0,0,0,  0,0,0,
                        0,4,5,  0,7,0,  0,0,1,
                        0,8,1,  0,0,5,  0,0,0};

        int ou[9*9] =  {9,1,7,  2,5,8,  6,3,4,
                        4,6,3,  1,9,7,  2,8,5,
                        5,2,8,  6,3,4,  1,7,9,

                        1,7,2,  5,8,3,  4,9,6,
                        8,9,6,  7,4,1,  3,5,2,
                        3,5,4,  9,2,6,  7,1,8,

                        6,3,9,  8,1,2,  5,4,7,
                        2,4,5,  3,7,9,  8,6,1,
                        7,8,1,  4,6,5,  9,2,3};

        if(!testUnit("0_0000", 0, in, ou))
            return false;
    }

    {
        int in[9*9] =  {7,4,0,  0,0,0,  0,0,0,
                        8,0,0,  0,0,0,  0,0,0,
                        0,0,0,  0,9,0,  0,1,0,

                        0,0,0,  8,0,0,  0,3,0,
                        0,0,0,  4,0,5,  0,0,0,
                        0,0,2,  0,0,0,  6,9,0,

                        0,0,9,  0,3,0,  0,0,0,
                        0,0,0,  0,0,0,  0,0,5,
                        0,0,0,  7,0,0,  0,0,8};

        int ou[9*9] =  {7,4,3,  6,8,1,  2,5,9,
                        8,9,1,  2,5,7,  4,6,3,
                        6,2,5,  3,9,4,  8,1,7,

                        4,1,7,  8,6,9,  5,3,2,
                        9,3,6,  4,2,5,  7,8,1,
                        5,8,2,  1,7,3,  6,9,4,

                        2,7,9,  5,3,8,  1,4,6,
                        1,6,8,  9,4,2,  3,7,5,
                        3,5,4,  7,1,6,  9,2,8};

        if(!testUnit("0_0001", 0, in, ou))
            return false;
    }

    {
        int in[9*9] =  {7,0,0,  3,0,0,  0,0,0,
                        0,0,0,  0,0,0,  8,2,3,
                        0,8,5,  1,4,2,  0,0,0,

                        5,9,0,  6,0,4,  0,0,8,
                        0,0,0,  0,5,0,  7,0,4,
                        0,0,4,  7,1,8,  6,0,9,

                        0,0,3,  0,0,0,  0,0,2,
                        9,0,0,  5,0,0,  4,7,0,
                        0,0,0,  0,9,0,  0,0,6};

        int ou[9*9] =  {7,2,9,  3,8,6,  1,4,5,
                        6,4,1,  9,7,5,  8,2,3,
                        3,8,5,  1,4,2,  9,6,7,

                        5,9,7,  6,3,4,  2,1,8,
                        8,1,6,  2,5,9,  7,3,4,
                        2,3,4,  7,1,8,  6,5,9,

                        4,7,3,  8,6,1,  5,9,2,
                        9,6,8,  5,2,3,  4,7,1,
                        1,5,2,  4,9,7,  3,8,6};

        if(!testUnit("0_0002", 0, in, ou))
            return false;
    }


    // Dificil ***********************************************************************
    {
        // Do video https://www.youtube.com/watch?v=YsHpygKu0lE
        int in[9*9] =  {0,3,0,  9,6,1,  0,8,0,
                        1,6,0,  0,0,8,  0,2,9,
                        9,8,4,  7,0,0,  1,0,0,

                        0,2,0,  3,0,0,  0,0,0,
                        6,1,0,  8,7,5,  0,4,2,
                        0,4,0,  0,0,9,  0,7,0,

                        0,9,1,  0,8,7,  6,0,0,
                        4,7,0,  1,0,0,  0,9,8,
                        0,5,0,  0,9,0,  0,0,0};

        int ou[9*9] =  {7,3,2,  9,6,1,  4,8,5,
                        1,6,5,  4,3,8,  7,2,9,
                        9,8,4,  7,5,2,  1,6,3,

                        8,2,7,  3,4,6,  9,5,1,
                        6,1,9,  8,7,5,  3,4,2,
                        5,4,3,  2,1,9,  8,7,6,

                        2,9,1,  5,8,7,  6,3,4,
                        4,7,6,  1,2,3,  5,9,8,
                        3,5,8,  6,9,4,  2,1,7};

        if(!testUnit("2_0000", 2, in, ou))
            return false;
    }


    printRes("Test OK");
    return true;
}

bool TestSolverLogic::testUnit(const QString &unitName, int level, int *in, int *out)
{
    Grid grid;
    grid.setValues(in);
    grid.fillNotes();

    SolverLogic solverLogic(grid);
    solverLogic.solve();

    if(!grid.isFull())
    {
        printRes(QString("%1 Not Solve").arg(unitName));
        return false;
    }

    if(out)
    {
        if(!grid.compareValues(out))
        {
            printRes(QString("%1 Error In Solution").arg(unitName));
            return false;
        }
    }
    else
    {
        Grid gridBf;
        gridBf.setValues(in);
        SolverBruteForce solverBruteForce(gridBf);
        QList<Grid> solutions = solverBruteForce.solveSolutions(2);

        if(solutions.size() != 1)
        {
            printRes(QString("%1 Is Not Valid").arg(unitName));
            return false;
        }

        if(!grid.compareValues(solutions[0]))
        {
            printRes(QString("%1 Error In Solution").arg(unitName));
            return false;
        }

        printRes(QString("Use this as out solution to %1").arg(unitName));
        for(int i = 0; i < 9; i++)
        {
            for(int j = 0; j < 9; j++)
            {
                std::cout << solutions[0].getValue(i, j) << ",";
                if (j % 3 == 2) std::cout << "  ";
            }
            std::cout << std::endl;
            if (i % 3 == 2) std::cout << std::endl;
        }
    }

    if(solverLogic.getResultLevel() != level)
    {
        printRes(QString("%1 Error In Level").arg(unitName));
        return false;
    }

    return true;
}

void TestSolverLogic::printRes(const QString &errorMsg)
{
    std::cout << std::endl;
    std::cout << "***************************************************************" << std::endl;
    std::cout << errorMsg.toStdString() << std::endl;
    std::cout << "***************************************************************" << std::endl;
    std::cout << std::endl;
}
