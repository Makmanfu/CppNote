//*************************************************************************
//
//                          С���߼���
//
//
//*************************************************************************
#ifndef MINITOOLS_HPP
#define MINITOOLS_HPP
#include "WinTool.h"
#include <iostream>
using namespace std;

//��������
class SignFlagexe
{
public:
    static int main(int argc, char** argv)
    {
        if (argc > 1)
        {
            WinTool::Instance()->SignFlagEXE(argv[argc - 1]);
            cout << argv[argc - 1] << "  ok!" << endl;
        }
        return 0;
    };
};








#endif
