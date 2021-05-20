#pragma once

// 协议头信息
typedef struct tagPacketHead
{
public:
	tagPacketHead()
	{
		m_nCmd = -1;
		m_nLen = 0;
	}
	int m_nCmd;	// 命令
	int m_nLen;	// 长度
}PACKETHEAD, *PPACKETHEAD;


// 屏幕
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

	int m_nWndWidth;	// 屏幕宽度
	int m_nWndHeight;	// 屏幕高度
	unsigned long m_SrcSize;	// 原屏幕数据的大小
	unsigned long m_CompressedSize;	//压缩后屏幕数据大小
}SCREENINFO, *PSCREENINFO;


// 命令参数
enum Packet
{
	PK_START_CMD,	// 启动cmd
	PK_INPUT_CMD_ORDER,	// 输入的cmd命令
	PK_OUTPUT_CMD_DATA,	// 通过cmd命令输入的对应数据信息
	PK_QUIT_CMD,	// 退出cmd

	PK_GET_DES_DRIVE,	// 获取目标计算机上的磁盘驱动
	PK_GET_FOLDER_NEXT_DATA,	// 获取当前文件夹的下一层数据
	PK_GET_FOLDER_PREV_DATA,	// 获取当前文件夹的上一层数据
	PK_HAVE_PREV_FOLDER,	// 有上一层文件夹
	PK_NO_PREV_FOLDER,	// 没有上一层文件夹
	PK_HAVE_NEXT_FOLDER,	// 有下一层文件夹
	PK_NO_NEXT_FOLDER,	// 没有下一层文件夹
	PK_DOWN_IS_FILE,	// 下载的是否是文件
	PK_IS_FOLDER,	// 当前点击的是文件夹
	PK_IS_FILE,	// 当前点击的是文件
	PK_START_DOWN_FILE,	// 开始下载文件
	PK_GET_FILE_SIZE,	// 获取文件大小
	PK_GET_FILE_DATA,	// 获取文件数据
	PK_DOWN_FILE_SUCC,	// 下载文件成功
	PK_GET_FILE_NEXT_DATA,	// 获取文件下一条数据
	PK_UP_FILE_DATA,	//上传文件数据

	// 客户端屏幕
	PK_CLIENT_SCREEN_DATA,	// 屏幕数据
	PK_QUIT_CLIENT_SCREEN,	// 退出客户端屏幕
	PK_MOUSE_BUTTON_DOWN,	// 鼠标左键单击
	PK_MOUSE_BUTTON_DBLCLK,	// 鼠标左键双击
	PK_KEYBOARD_INPUT,	// 键盘输入
	PK_HEART_BEAT,	//心跳包
};
