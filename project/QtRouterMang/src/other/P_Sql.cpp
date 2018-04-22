
#include "stdafx.h"
#include "P_Sql.h"

std::string SetTaskOK(string MOID, string taskID)
{
    return "UPDATE E_Tasklist SET MOID=\"" + MOID + "\" where taskID=" + taskID;
}

std::string BeforeTK(string vid)
{
    return "SELECT * FROM E_Tasklist WHERE vidstate=0 and proceid=(SELECT min(proceid) \
           FROM E_Tasklist WHERE vidstate=0 and vid=\"" + vid + "\") and vid=\"" + vid + "\"";
}

std::string BeforeOneTK(string vid)
{
    return "SELECT top 1 taskID,vidstate FROM E_Tasklist WHERE vidstate=0 and proceid\
           =(SELECT min(proceid) FROM E_Tasklist WHERE vidstate=0 and vid=\"" + vid + "\") and vid=\"" + vid + "\"";
}

std::string GetTaskID(string moid)
{
    return "SELECT taskid FROM E_Tasklist WHERE vidstate=1 and moid=\"" + moid + "\"";
}

std::string OKtaskList(string TaskID, string Wstate)
{
    return "UPDATE E_Tasklist SET vidstate=\"" + Wstate + "\" where TaskID=" + TaskID;
}

std::string SetworkState(string moid, string workstate)
{
    return "UPDATE E_REG SET mworkstate=\"" + workstate + "\" where moid=\"" + moid + "\"";
}

std::string GetE_Task_ID(string vid)
{
    return "SELECT TASKID FROM E_Task WHERE VID=\"" + vid + "\"";
}
