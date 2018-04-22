
#include "stdafx.h"
#include "BoostConfig.h"
#include "Boost_timer.h"
#include <iostream>
using namespace std;
//***********************编译date_time的方法*********************************
#define BOOST_DATE_TIME_SOURCE
#define BOOST_DATE_TIME_NO_LIB
#include <libs/date_time/src/gregorian/greg_names.hpp>
#include <libs/date_time/src/gregorian/date_generators.cpp>
#include <libs/date_time/src/gregorian/greg_month.cpp>
#include <libs/date_time/src/gregorian/greg_weekday.cpp>
#include <libs/date_time/src/gregorian/gregorian_types.cpp>
//*******************************************************************************
#include <boost/timer.hpp>
#include <boost/progress.hpp>
using namespace boost;
using namespace boost::gregorian;


void boost_Timer_Demo(void)
{
    //timer方法计算时间
    boost::timer t_time;
    Sleep(200);
    cout << "计算过了多少时间(s):  " << t_time.elapsed() << endl;
    //---------------------------------------------------------------------------------------------------
    //progress_timer方法 在过程析构时自动计算[警告: 只适合在 return前运行的算法用最合理]
    boost::progress_timer t_time2;         //stringstream out; t_time2(out);
    Sleep(200);
    //---------------------------------------------------------------------------------------------------
    //精度计算到10位小数 [return前用最合理]
    new_progress_timer<10> t_time3;
    Sleep(200);
    //---------------------------------------------------------------------------------------------------
    return ;
}

void boost_Progress_Demo(int f_num)
{
    //设置长度
    boost::progress_display pd(100);
    for (int i = 0; i < 100; i++)   //遍历到总长度
    {
        Sleep(f_num);
        //这个中间插入变化代码
        ++pd;           //进度条变化
    }
    return ;
}















/*
    File          : CHMOD.C   (Change file attributes)
    Author        : Thomas P. Devitt
                Pseudonym Software - Data: 818-796-4197

    Updated on    : July 7, 1987
    Include files : stdio.h
                ctype.h
                dos.h
                dos.h
                process.h
                string.h

    Portibility   : PC/MS-DOS only
    Defined in    : UNIX (Details differ)
    Synopsis      : CHMOD [-flags] filename
*/

/*  #include <stdio.h>
    #include <ctype.h>
    #include <dos.h>
    #include <dir.h>
    #include <process.h>
    #include <string.h>

    #define ANY    22     */
/*
                                     Hidden  ::=   2
                                     System  ::=   4
                                     Subdir  ::=  16
*/
//#define CHMOD  0x4301
//#define GETMOD 0x4300

/*
    void help(void);
    void uchmod(char *fname, int att);
    void ugetmod(char *fname, int att);

    struct ffblk dta;                      // Disk xfer area

    char *documentation[] = {
    "CHMOD (c) Pseudonym Software 1987",
    "          Thomas P. Devitt",
    "",
    "CHMOD will change or report on the current attributes of files and",
    "directories.  The syntax for CHMOD is as follows:",
    "",
    "To change file attributes:",
    "",
    "    CHMOD -<flags> filename  - or -",
    "    CHMOD filename /<flags>",
    "",
    "    where flags equal:      n   Normal",
    "                            r   Read Only",
    "                            h   Hidden",
    "                            s   System",
    "                            a   Archive (modified flag)",
    "",
    "    Any flag not explicitly set will be cleared.",
    "",
    "To view current attributes:",
    "",
    "    CHMOD filename",
    "",
    "The file name may include DOS wildcard characters.",
    "",
    "Output may be redirected by using the DOS redirecton symbols \">\" and \"|\".",
    0 };

    main(int argc, char *argv[])
    {
        char *filename, *parm;
        char path[MAXPATH], drive[MAXDRIVE], dir[MAXDIR], name[MAXFILE],
             ext[MAXEXT], newname[MAXPATH];
        char c;
        int  attr = -1;              // if -1, inquiry only
        int  done = 0;

        if (argc == 1) help();       // no parms

        while (--argc)
        {
           parm = *++argv;
           switch (*parm)
           {
              case  '?':  help();

              case  '/':
              case  '-':  ++parm;
                          if (attr == -1)  // clear attribute
                             attr = 0;
                          while ((c = tolower(*parm++)) != '\0')
                          {
                             switch (c)
                             {
                                case  'r' :  attr |= FA_RDONLY;
                                             break;
                                case  'h' :  attr |= FA_HIDDEN;
                                             break;
                                case  's' :  attr |= FA_SYSTEM;
                                             break;
                                case  'a' :  attr |= FA_ARCH;
                                             break;
                                case  'n' :  attr = 0;
                             }
                          }
                          break;

              default  :  filename = parm;   // if no switch char
           }
        }

        if (attr > -1)
        {
           fnsplit(filename, drive, dir, name, ext);   // parse filename
           fnmerge(path, drive, dir, NULL, NULL);      // build path
        }

        done = findfirst(filename, &dta, ANY);       // find filename
        if (done)
        {
           // error if file not found

           fprintf(stderr,"\nChmod: Cannot find %s.\n",filename);
           exit(1);
        }
        else                               // file was found
           do
           {
              if (*dta.ff_name != '.')    // ignore dir pointers
              {
                 if (attr >= 0)                       // change attribute
                 {
                    strcpy(newname,path);                // build filespec
                    strcat(newname,dta.ff_name);
                    uchmod(newname,attr);
                 }
                 else                                 // display attribute
                    ugetmod(dta.ff_name,dta.ff_attrib);
              }
              done = findnext(&dta);                  // find next file
           }
           while (!done);

           if (attr < 0)
              printf("\n");        // print cr/lf if displaying attributes

           exit(0);                // report evertything a-ok
    }

    void uchmod(char *fname, int att)
    {
    union      REGS    regs;
    struct     SREGS   sregs;

    sregs.ds = _DS;
    regs.x.ax = CHMOD;
    regs.x.cx = att;
    regs.x.dx =(unsigned)fname;
    intdosx(&regs,&regs,&sregs);
    }

    void ugetmod(char *fname, int att)     // get attr and print
    {
    char               attrbuff[5], *abuffp;

    abuffp = attrbuff;                          // pointer to attr text buff
    if ( !(att & 7) )                           // parse attribute
       abuffp++ = 'N';
    if (att & 1)
       abuffp++ = 'R';
    if (att & 2)
       abuffp++ = 'H';
    if (att & 4)
       abuffp++ = 'S';
    if (att & 32)
       abuffp++ = 'A';
    abuffp = '\0';                             // terminate string
    if (att & 16)
      printf("\n\\%-14s%s",fname,attrbuff);    // prefix dir with '\'
    else
      printf("\n%-15s%s",fname,attrbuff);      // print normal file
    }

    void help(void)                           // print doc text and exit
    {
    char **dp;
    for (dp = documentation; *dp; dp++)
      printf("%s\n",*dp);
    exit(0);
    }


*/