//-----------------------------------------------------------------------------
//
//              windows �ļ�ϵͳ���Ͷ����ͷ�ļ�
//
//-----------------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include <shlobj.h>
//#pragma comment(lib, "shell32.lib")
#include <iostream>
#include <string>
#include <vector>
using namespace std;

//��ǰ����exe[0ȫ·��,1exeȫ��,2·��,3·��\,4exe��]
enum EXETYPE { PATHALL = 0, PATHEXE, PATHBF, PATHBFA, PATHEXENAME };

//�ļ�������ʽ[0�ݹ�ȫ��,1�ݹ��ļ���,2�ݹ��ļ�,3��ǰ�ļ���,4��ǰ�ļ�]
enum DIRTYPE { DALL = 0, DALLDIR, DALLFILE, DCURRDIR, DCURRFILE };

//�ļ�(��)������ָ��
typedef void(*FileProc)(string& pathfile);


// ��ȱ���Ŀ¼
//BOOL TraverseDirBF(const string& strDir)
//{
//    deque<string> dequePath;
//    string strTemp = strDir;
//    int iFileCount(0),iDirCount(0);
//    ofstream ofs("out.txt");
//    while (TRUE)
//    {
//        if (!PathFileExists(strTemp.c_str()))
//            return FALSE;
//        string strDirFind = strTemp + string("\\*");
//        WIN32_FIND_DATA ffd = { 0 };
//        HANDLE hFind = FindFirstFile(strDirFind.c_str(), &ffd);
//        if (INVALID_HANDLE_VALUE == hFind)
//        {
//            if (dequePath.empty())
//                break;
//            else
//            {
//                strTemp = dequePath.front();
//                dequePath.pop_front();
//                continue;
//            }
//        }
//        // List all the files in the directory with some info about them.
//        do
//        {
//            if ((0 == strcmp(ffd.cFileName, ".")) || (0 == strcmp(ffd.cFileName, "..")))
//                continue;
//            string strFilePathName = strTemp + "\\" + string(ffd.cFileName);
//            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//            {
//                //_tprintf(TEXT("  %s   <DIR>\n"), strFilePathName.c_str());
//                string str111 = strFilePathName + "\n";
//                ofs.write(str111.c_str(), str111.size());
//                dequePath.push_back(strFilePathName);
//                ++iDirCount;
//            } else {
//                //_tprintf(TEXT("  %s   <FILE>\n"), strFilePathName.c_str());
//                string str111 = strFilePathName + "\n";
//                ofs.write(str111.c_str(), str111.size());
//                ++iFileCount;
//            }
//        } while (FindNextFile(hFind, &ffd) != 0);
//        DWORD dwError = GetLastError();
//        if (dwError != ERROR_NO_MORE_FILES)
//        {
//            FindClose(hFind);
//            return FALSE;
//        }
//
//        FindClose(hFind);
//        if (dequePath.empty())
//            break;
//        else
//        {
//            strTemp = dequePath.front();
//            dequePath.pop_front();
//        }
//    }
//    ofs.close();
//    return TRUE;
//}

/*
BOOL GetDesktopPath(char* pszDesktopPath)
{
LPITEMIDLIST  ppidl = NULL;
if (SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &ppidl) == S_OK)
{
BOOL flag = SHGetPathFromIDListA(ppidl, pszDesktopPath);
CoTaskMemFree(ppidl);
return flag;
}
return FALSE;
}

//�õ�������������·��
BOOL GetIEQuickLaunchPath(char *pszIEQueickLaunchPath)
{
LPITEMIDLIST  ppidl;

if (SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &ppidl) == S_OK)
{
BOOL flag = SHGetPathFromIDListA(ppidl, pszIEQueickLaunchPath);
strcat(pszIEQueickLaunchPath, "\\Microsoft\\Internet Explorer\\Quick Launch");
CoTaskMemFree(ppidl);
return flag;
}

return FALSE;
}

//�õ� ��ʼ->������ ��·��
BOOL GetProgramsPath(char *pszProgramsPath)
{
LPITEMIDLIST  ppidl;
if (SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &ppidl) == S_OK)
{
BOOL flag = SHGetPathFromIDListA(ppidl, pszProgramsPath);
CoTaskMemFree(ppidl);
return flag;
}
return FALSE;
}


BOOL ShortLink(LPCSTR lpszWorkDir, LPCSTR lpszDirName, int iShowCmd )
{
WCHAR wsz[MAX_PATH];   //����Unicode�ַ���
IShellLinkW* pLink;     //IShellLink����ָ��
IPersistFile* ppf;      //IPersisFil����ָ��
//����IShellLink����
HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
if (FAILED(hr))
return FALSE;
//��IShellLink�����л�ȡIPersistFile�ӿ�
hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
if (FAILED(hr))
{
pLink->Release();
return FALSE;
}
//����ȫ·��
MultiByteToWideChar(CP_ACP, 0, lpszWorkDir, -1, wsz, MAX_PATH);
if (lpszWorkDir != NULL)
pLink->SetPath(wsz);
//��ݼ�
WORD wHotkey = 0;
if (wHotkey != 0)
pLink->SetHotkey(wHotkey);
//��ע
LPCSTR lpszDescription = "��ע";
MultiByteToWideChar(CP_ACP, 0, lpszDescription, -1, wsz, MAX_PATH);
if (lpszDescription != NULL)
pLink->SetDescription(wsz);
//��ʾ��ʽ
pLink->SetShowCmd(iShowCmd);
//�޸���չ�� lpszDirName
//char szBuffer[MAX_PATH];
//sprintf(szBuffer, "%s", lpszDirName);
//int nLen = strlen(szBuffer);
//szBuffer[nLen - 3] = 'l';
//szBuffer[nLen - 2] = 'n';
//szBuffer[nLen - 1] = 'k';
MultiByteToWideChar(CP_ACP, 0, lpszDirName, -1, wsz, MAX_PATH);
//�����ļ�
hr = ppf->Save(wsz, TRUE);
ppf->Release();
pLink->Release();
return SUCCEEDED(hr);
}

BOOL CreateFileShortcut(LPCSTR lpszFileName, LPCSTR lpszLnkFileDir, LPCSTR lpszLnkFileName,
LPCSTR lpszWorkDir, WORD wHotkey, LPCSTR lpszDescription, int iShowCmd)
{
WCHAR wsz[MAX_PATH];        //����Unicode�ַ���
IShellLink* pLink;          //IShellLink����ָ��
IPersistFile* ppf;          //IPersisFil����ָ��
//����IShellLink����

// �������ʵ�COM������
HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,IID_IShellLink, reinterpret_cast<LPVOID*>(&pLink));
//HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
if (FAILED(hr))
return FALSE;
//��IShellLink�����л�ȡIPersistFile�ӿ�
hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
if (FAILED(hr))
{
pLink->Release();
return FALSE;
}
//Ŀ��
//MultiByteToWideChar(CP_ACP, 0, lpszFileName, -1, wsz, MAX_PATH);
//pLink->SetPath(lpszFileName == NULL ? NULL : wsz);
//����Ŀ¼
MultiByteToWideChar(CP_ACP, 0, lpszWorkDir, -1, wsz, MAX_PATH);
if (lpszWorkDir != NULL)
pLink->SetPath(wsz);
//��ݼ�
if (wHotkey != 0)
pLink->SetHotkey(wHotkey);
//��ע
MultiByteToWideChar(CP_ACP, 0, lpszDescription, -1, wsz, MAX_PATH);
if (lpszDescription != NULL)
pLink->SetDescription(wsz);
//��ʾ��ʽ
pLink->SetShowCmd(iShowCmd);
//�����ļ�
WCHAR* wsz2 = TEXT("c:/TEST.lnk");
hr = ppf->Save(wsz2, TRUE);
ppf->Release();
pLink->Release();
return SUCCEEDED(hr);
}

*/

//���Ա����ļ���
//vector<string> AllDir;
//string path = "c:/";
//PathFormat(path);
//GetDirFiles(path, AllDir,4);
//for (vector<string>::const_iterator it = AllDir.begin();
//    it != AllDir.end();++it)
//    LogFun(it->c_str());

//����ɾ���ļ���
//if (DelPathDir("D:/SYSDOC/down/sd"))
//    MessageBoxA(NULL, "DelPathDir", "BOX", MB_OK);
//if (SmartFile("D:/SYSDOC/down/sd"))
//    MessageBoxA(NULL, "SmartFile", "BOX", MB_OK);
//if (SmartDir("D:/SYSDOC/down/sd"))
//    MessageBoxA(NULL, "SmartDir", "BOX", MB_OK);

//������
/*
bool UpPackExeRes(char* f_strFileName, unsigned short  f_wResID, char* f_strFileType)
{
//��exe��Ч dll��ȡ�������
//************************************************************************
//*  ����˵�����ͷ���Դ��ĳ���͵��ļ�
//* ��    �������ļ�������ԴID����Դ����
//* �� �� ֵ���ɹ�����TRUE�����򷵻�FALSE
//************************************************************************
// ��Դ��С
DWORD   dwWrite = 0;
// �����ļ�
HANDLE  hFile = CreateFileA(f_strFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
if (hFile == INVALID_HANDLE_VALUE)    return FALSE;
// ������Դ�ļ��С�������Դ���ڴ桢�õ���Դ��С
HRSRC   hrsc = FindResourceA(NULL, MAKEINTRESOURCEA(f_wResID), f_strFileType);
HGLOBAL hG = LoadResource(NULL, hrsc);
DWORD   dwSize = SizeofResource(NULL, hrsc);
// д���ļ�
WriteFile(hFile, hG, dwSize, &dwWrite, NULL);
CloseHandle(hFile);
return TRUE;
}

bool UpPackDllRes(char* f_dllname, char* f_strFileName, WORD f_wResID, char* f_strFileType)
{
//�ж��ļ����ھͲ����ͷ�
if (!_access(f_strFileName, 0))  return false;
//dll���
HINSTANCE m_DLLhandle;
//����dll
m_DLLhandle = ::LoadLibraryA(f_dllname);
// ��Դ��С
DWORD   dwWrite = 0;
// �����ļ�
HANDLE  hFile = CreateFileA(f_strFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
if (hFile == INVALID_HANDLE_VALUE)    return FALSE;
// ������Դ�ļ��С�������Դ���ڴ桢�õ���Դ��С
HRSRC   hrsc = FindResourceA(m_DLLhandle, MAKEINTRESOURCEA(f_wResID), f_strFileType);
HGLOBAL hG = LoadResource(m_DLLhandle, hrsc);
DWORD   dwSize = SizeofResource(m_DLLhandle, hrsc);
// д���ļ�
WriteFile(hFile, hG, dwSize, &dwWrite, NULL);
CloseHandle(hFile);
//�ͷ���Դ
FreeLibrary(m_DLLhandle);
return false;
}

bool BitmapWriteFile(char* filename, int width, int height, unsigned char* bitmapData)
{
//WriteBitmapFile
//����bitmapData�ģ�RGB�����ݣ�����bitmap
//filename��Ҫ���浽����Ӳ�̵��ļ���������·����
//���BITMAPFILEHEADER
BITMAPFILEHEADER bitmapFileHeader;
memset(&bitmapFileHeader, 0, sizeof(BITMAPFILEHEADER));
bitmapFileHeader.bfSize = sizeof(BITMAPFILEHEADER);
bitmapFileHeader.bfType = 0x4d42;   //BM
bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
//���BITMAPINFOHEADER
BITMAPINFOHEADER bitmapInfoHeader;
memset(&bitmapInfoHeader, 0, sizeof(BITMAPINFOHEADER));
bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
bitmapInfoHeader.biWidth = width;
bitmapInfoHeader.biHeight = height;
bitmapInfoHeader.biPlanes = 1;
bitmapInfoHeader.biBitCount = 24;
bitmapInfoHeader.biCompression = BI_RGB;
bitmapInfoHeader.biSizeImage = width * abs(height) * 3;
//////////////////////////////////////////////////////////////////////////
FILE* filePtr;          //����Ҫ�����bitmap�ļ���
unsigned char tempRGB;  //��ʱɫ��
size_t imageIdx;
//����R��B������λ��,bitmap���ļ����õ���BGR,�ڴ����RGB
for (imageIdx = 0; imageIdx < bitmapInfoHeader.biSizeImage; imageIdx += 3)
{
tempRGB = bitmapData[imageIdx];
bitmapData[imageIdx] = bitmapData[imageIdx + 2];
bitmapData[imageIdx + 2] = tempRGB;
}
filePtr = fopen(filename, "wb");
if (NULL == filePtr)
return false;
fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
fwrite(bitmapData, bitmapInfoHeader.biSizeImage, 1, filePtr);
fclose(filePtr);
return true;
}

bool CopyBinFile(const char* f_src, const char* f_out)
{
if (f_out == NULL)
return false;
if (f_src == NULL)
return false;
bool bRet = true;
std::ofstream fout(f_out, std::ios::binary | std::ios::app);
std::ifstream fin(f_src, std::ios::binary);  //��Ҫ����ļ�
if (fin.bad())
bRet = false;
else
{
while (!fin.eof())
{
char szBuf[256] = { 0 };
fin.read(szBuf, sizeof(char) * 256);
if (fout.bad())
{
bRet = false;
break;
}
fout.write(szBuf, sizeof(char) * 256);
}
}
fin.close();
fout.close();
return bRet;
}

void Md5File(char* f_infile, char* f_outfile, char* f_pwd, bool f_flag)
{
FILE* fp1, *fp2;
register char ch;
int j = 0, j0 = 0;
fp1 = fopen(f_infile, "r");      //��Ҫ����/���ܵ��ļ�
if (fp1 == NULL)
return;
fp2 = fopen(f_outfile, "w");
if (fp2 == NULL)
return;
//--------------------------------------------------
while (f_pwd[++j0]);
ch = fgetc(fp1);
while (!feof(fp1))
{
if (j0 > 7) j0 = 0;
if (f_flag)
ch += f_pwd[j0++];      //����
else
ch -= f_pwd[j0++];      //����
fputc(ch, fp2);
ch = fgetc(fp1);
}
//--------------------------------------------------
fclose(fp1); //�ر�Դ�ļ�
fclose(fp2); //�ر�Ŀ���ļ�
}

bool WriteBinFile(const char* Filename)
{
std::ofstream fout("a.dat", std::ios::binary);
int nNum = 20;
std::string str("Helloworld");
fout.write((char*)&nNum, sizeof(int));
fout.write(str.c_str(), sizeof(char) * (str.size()));
fout.close();
return true;
//д�ı��ļ���Ƚϼ򵥣����£�
std::ofstream fout2("b.dat");
int nNum1 = 20;
std::string str1("Hello, world");
fout2 << nNum1 << "," << str1 << std::endl;
fout2.close();
//���������ļ�
//��ȡ�������ļ�����ʹ��ifstream �������У��ļ��Ĵ�ģʽһ��Ҫ�� binary���������Ĳ��� binary, �ļ�����ASCII��ʽ�򿪡�
std::ifstream fin1("a.dat", std::ios::binary);
int nNum2;
char szBuf[256] = { 0 };
fin1.read((char*)&nNum2, sizeof(int));
fin1.read(szBuf, sizeof(char) * 256);
//std::cout << "int = " << nNum << std::endl;
//std::cout << "str = " << szBuf << std::endl;
fin1.close();
//����ȡ�ı��ļ���Ƚϼ򵥣�
std::ifstream fin2("b.dat");
int nNum3;
char szBuf2[256] = { 0 };
fin2 >> nNum3 >> szBuf2;
//std::cout << "int = " << nNum << std::endl;
//std::cout << "str = " << szBuf << std::endl;
fin2.close();
//  �ļ��Ĵ�ģʽ
//�ļ�����ʱ���������ʾָ����ģʽ���ļ����ཫʹ��Ĭ��ֵ��
//��<fstream> �ж��������´�ģʽ���ļ�����:
//ios::app // �Ӻ������
//ios::ate // �򿪲��ҵ��ļ�β
//ios::binary // ������ģʽI/O�����ı�ģʽ���)
//ios::in // ֻ����
//ios::out // д��
//ios::trunc // ���ļ���Ϊ 0 ����
//����ʹ��λ������ OR �����Щ��־������
//ofstream logFile("log.dat", ios::binary | ios::app);
}

void binWriteFile(const char* Filename)
{
//������ʵ����������ļ���д����
//std::ofstream fout(Filename, std::ios::binary);
//int nNum = 20;
//std::string str("Hello, world");
//fout.write((char*)&nNum, sizeof(int));
//fout.write(str.c_str(), sizeof(char) * (str.size()));
//fout.close();
//д��������ļ�
std::fstream iofile(Filename, ios::in | ios::out | ios::binary); //�����Ƹ�д
if (iofile)
{
const char* txt = "test";
iofile.seekp(198, ios::beg);      //��λλ��
//iofile.read((char *)&stud1[i/2],sizeof(stud1[0]));    //������
for (int i = 0; i < 2; i++)
iofile.write(txt, sizeof(txt));  //д��ֵ
iofile.seekp(598, ios::beg);      //��λλ��
for (int i = 0; i < 3; i++)
iofile.write(txt, sizeof(txt));  //д��ֵ
iofile.close();
}
}


*/

