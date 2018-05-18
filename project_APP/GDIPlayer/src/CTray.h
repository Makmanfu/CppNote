

#ifndef CTRAY_H
#define CTRAY_H

//���̿ؼ���
class CTray
{
#define WM_SHOWTASK WM_USER+1
    //ע��������Ϣ
    //static UINT const WM_TASKBAR_CREATED;
#define WM_TASKBAR_CREATED RegisterWindowMessageA("TaskbarCreated")
    //if ( uMsg == WM_TASKBAR_CREATED )//ϵͳExplorer��������ʱ�����¼�������
    //    MP.myTrayClass.ShowTray ( true );
    //break;
public:
    CTray();
    CTray(HWND hWnd, size_t IconID, HMENU hMenu);
    virtual ~CTray();
private:
    NOTIFYICONDATAA nid;        //��С���������õ��ļ�¼ͼ����Ϣ
    HWND hDlgWnd;               //���ھ��
    size_t TaskIconID;          //����ID
    HMENU hTrayMenu;            //�˵�
public:
    //���̳�ʼ��
    void SetMenu(HMENU hMenu);
    //����ͼ��
    void SetIcon(size_t IconID);

    //��ʾ����
    void ShowTray(bool _state);
    //��������ʾ����
    void ShowTask(HWND hWnd, WPARAM IconID, LPARAM iMouseMsg);
};


#endif
