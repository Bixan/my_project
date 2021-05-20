#pragma once

// Э��ͷ��Ϣ
typedef struct tagPacketHead
{
public:
	tagPacketHead()
	{
		m_nCmd = -1;
		m_nLen = 0;
	}
	int m_nCmd;	// ����
	int m_nLen;	// ����
}PACKETHEAD, *PPACKETHEAD;


// ��Ļ
typedef struct tagScreenInfo
{
public:
	tagScreenInfo()
	{
		m_nWndWidth = 0;
		m_nWndHeight = 0;
		m_SrcSize = 0;
		m_CompressedSize = 0;
	}

	int m_nWndWidth;	// ��Ļ���
	int m_nWndHeight;	// ��Ļ�߶�
	unsigned long m_SrcSize;	// ԭ��Ļ���ݵĴ�С
	unsigned long m_CompressedSize;	//ѹ������Ļ���ݴ�С
}SCREENINFO, *PSCREENINFO;


// �������
enum Packet
{
	PK_START_CMD,	// ����cmd
	PK_INPUT_CMD_ORDER,	// �����cmd����
	PK_OUTPUT_CMD_DATA,	// ͨ��cmd��������Ķ�Ӧ������Ϣ
	PK_QUIT_CMD,	// �˳�cmd

	PK_GET_DES_DRIVE,	// ��ȡĿ�������ϵĴ�������
	PK_GET_FOLDER_NEXT_DATA,	// ��ȡ��ǰ�ļ��е���һ������
	PK_GET_FOLDER_PREV_DATA,	// ��ȡ��ǰ�ļ��е���һ������
	PK_HAVE_PREV_FOLDER,	// ����һ���ļ���
	PK_NO_PREV_FOLDER,	// û����һ���ļ���
	PK_HAVE_NEXT_FOLDER,	// ����һ���ļ���
	PK_NO_NEXT_FOLDER,	// û����һ���ļ���
	PK_DOWN_IS_FILE,	// ���ص��Ƿ����ļ�
	PK_IS_FOLDER,	// ��ǰ��������ļ���
	PK_IS_FILE,	// ��ǰ��������ļ�
	PK_START_DOWN_FILE,	// ��ʼ�����ļ�
	PK_GET_FILE_SIZE,	// ��ȡ�ļ���С
	PK_GET_FILE_DATA,	// ��ȡ�ļ�����
	PK_DOWN_FILE_SUCC,	// �����ļ��ɹ�
	PK_GET_FILE_NEXT_DATA,	// ��ȡ�ļ���һ������
	PK_UP_FILE_DATA,	//�ϴ��ļ�����

	// �ͻ�����Ļ
	PK_CLIENT_SCREEN_DATA,	// ��Ļ����
	PK_QUIT_CLIENT_SCREEN,	// �˳��ͻ�����Ļ
	PK_MOUSE_BUTTON_DOWN,	// ����������
	PK_MOUSE_BUTTON_DBLCLK,	// ������˫��
	PK_KEYBOARD_INPUT,	// ��������
	PK_HEART_BEAT,	//������
};
