#pragma once
// 自定义消息
#define WM_CREATE_SOCKETE_RROR WM_USER + 1	// 创建SOCKET失败
#define WM_BIND_SOCKET_ERROR WM_USER + 2	// 绑定SOCKET失败
#define WM_LISTEN_SOCKETE_RROR WM_USER + 3	// 监听SOCKET失败
#define WM_CLIENT_ACCEPT_SUCCEED WM_USER + 4	// 客户端连接成功
#define WM_CLIENT_ACCEPT_FAIL WM_USER + 5	// 客户端连接失败
#define WM_CLIENT_QUIT WM_USER + 6	// 客户端退出
#define WM_GET_CMD_DATA WM_USER + 7	// 获取command对应数据
#define WM_RECV_DATA_ERROR WM_USER + 8	// 接受数据失败
#define WM_SEND_DATA_ERROR WM_USER + 9	// 发送数据失败
#define WM_DRIVE_INFO WM_USER + 10 // 驱动器信息
#define WM_GET_FOLDER_NEXT_DATA	WM_USER + 11 // 当前文件夹的下一层数据
#define WM_GET_FOLDER_PREV_DATA	WM_USER + 12 // 当前文件夹的上一层数据
#define WM_HAVE_NEXT_FILE WM_USER + 13 // 有下一层文件
#define WM_HAVE_PREV_FILE WM_USER + 14 // 有上一层文件
#define WM_IS_FOLDER WM_USER + 15 // 当前点击的是文件夹
#define WM_IS_FILE WM_USER + 16 // 当前点击的是文件
#define WM_GET_FILE_SIZE WM_USER + 17 // 获取文件大小
#define WM_GET_FILE_DATA WM_USER + 18 // 获取文件数据
#define WM_DOWN_FILE_DATA_SUCC WM_USER + 19 // 下载文件成功
#define WM_FILE_UPLOAD_SUCC	WM_USER + 20 // 文件上传成功
#define WM_GET_FILE_NEXT_DATA WM_USER + 21 // 文件下一条数据
#define WM_CLIENT_SCREEN_DATA WM_USER + 22 // 客户端屏幕数据