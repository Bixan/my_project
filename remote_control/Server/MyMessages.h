#pragma once
// �Զ�����Ϣ
#define WM_CREATE_SOCKETE_RROR WM_USER + 1	// ����SOCKETʧ��
#define WM_BIND_SOCKET_ERROR WM_USER + 2	// ��SOCKETʧ��
#define WM_LISTEN_SOCKETE_RROR WM_USER + 3	// ����SOCKETʧ��
#define WM_CLIENT_ACCEPT_SUCCEED WM_USER + 4	// �ͻ������ӳɹ�
#define WM_CLIENT_ACCEPT_FAIL WM_USER + 5	// �ͻ�������ʧ��
#define WM_CLIENT_QUIT WM_USER + 6	// �ͻ����˳�
#define WM_GET_CMD_DATA WM_USER + 7	// ��ȡcommand��Ӧ����
#define WM_RECV_DATA_ERROR WM_USER + 8	// ��������ʧ��
#define WM_SEND_DATA_ERROR WM_USER + 9	// ��������ʧ��
#define WM_DRIVE_INFO WM_USER + 10 // ��������Ϣ
#define WM_GET_FOLDER_NEXT_DATA	WM_USER + 11 // ��ǰ�ļ��е���һ������
#define WM_GET_FOLDER_PREV_DATA	WM_USER + 12 // ��ǰ�ļ��е���һ������
#define WM_HAVE_NEXT_FILE WM_USER + 13 // ����һ���ļ�
#define WM_HAVE_PREV_FILE WM_USER + 14 // ����һ���ļ�
#define WM_IS_FOLDER WM_USER + 15 // ��ǰ��������ļ���
#define WM_IS_FILE WM_USER + 16 // ��ǰ��������ļ�
#define WM_GET_FILE_SIZE WM_USER + 17 // ��ȡ�ļ���С
#define WM_GET_FILE_DATA WM_USER + 18 // ��ȡ�ļ�����
#define WM_DOWN_FILE_DATA_SUCC WM_USER + 19 // �����ļ��ɹ�
#define WM_FILE_UPLOAD_SUCC	WM_USER + 20 // �ļ��ϴ��ɹ�
#define WM_GET_FILE_NEXT_DATA WM_USER + 21 // �ļ���һ������
#define WM_CLIENT_SCREEN_DATA WM_USER + 22 // �ͻ�����Ļ����