// 棋思通讯规约
#ifndef __QS_PROTOCOL_H__
#define __QS_PROTOCOL_H__

#include <stdint.h>
#include <string.h>
#include "function.h"
#include "goType.h"

// 错误码定义
// 没有找到报文头
#define CMD_DECODE_OK                1
// 没有找到报文头
#define CMD_ERROR_NO_HEAD            -1
// 校验码错误
#define CMD_ERROR_CHECK_CODE         -2
// 数据长度错误
#define CMD_ERROR_DATA_LEN           -3
// 报文还没有收全错误
#define CMD_ERROR_DATA_NOT_FULL      -4
// 没有足够缓存区错误
#define CMD_ERROR_BUF_NOT_ENOUGH     -5
// 报文长度小于最小长度错误
#define CMD_ERROR_PACK_LEN_TOO_SHORT -6
// ID不匹配错误
#define CMD_ERROR_ID_NOT_MATCH       -7
// 命令码错误
#define CMD_ERROR_CMD                -10


// 校验码计算长度
#define CMD_CRC_CALC_LEN      12
// 报文最小长度
#define CMD_MIN_PACKET_LEN    (CMD_CRC_CALC_LEN + 2)
// ID长度
#define CMD_ID_LEN            6
// crc初始值
#define CMD_CRC_INIT_CODE     0xFFFF
// 最大数据长度
#define CMD_MAX_DATA_LEN      1000

// 当前规约版本
#define CURR_PTL_VERSION 0x00000000

// 命令编码
// 读取版本
#define CMD_READ_VERSION      0x01
// 读取有无棋子
#define CMD_READ_HAVE_CHESS   0x02
// 读取黑白棋子
#define CMD_READ_BW_CHESS     0x03
// 读取所有棋子
#define CMD_READ_ALL_CHESS    0x04
// 发送所有棋子
#define CMD_SEND_ALL_CHESS    0x05
// 服务器悔棋
#define CMD_HOST_UNDO         0x06
// 棋盘悔棋
#define CMD_BOARD_UNDO        0x07
// 电脑落子
#define CMD_COMPUTER_MOVE     0x08
// 人落子
#define CMD_HUMAN_MOVE        0x09
// 服务器指令
#define CMD_HOST_INDICATE     0x0A
// 棋盘指令
#define CMD_BOARD_INDICATE    0x0B
// 写所有LED
#define CMD_WRITE_ALL_LED     0x10
// 写单个LED
#define CMD_WRITE_SINGLE_LED  0x11
// 开始校准
#define CMD_START_CAL         0x20
// 读取校准数据
#define CMD_READ_CAL_DATA     0x21
// 心跳
#define CMD_HEART             0x30
// 读取棋盘状态
#define CMD_READ_STATE        0x31
// 语音音量
#define CMD_VOLUME            0x32
// 空闲休眠时间(s)
#define CMD_IDLE_TIME         0x33
// 亮度
#define CMD_LIGHTENESS        0x34
// 模式
#define CMD_MODE              0x35
// 提子延迟时间(ms)
#define CMD_TAKE_DELAY        0x36
// 后台服务
#define CMD_WEB_SERVICE       0x37
// 下载程序
#define CMD_DOWNLOAD_APP      0x40
// 播放语音
#define CMD_PLAY_AUDIO        0x50

// 开始记谱
#define CMD_START_RECORD      0x60
// 上送棋谱数据
#define CMD_UPLOAD_RECORD_DATA 0x61
// 读取棋谱数据
#define CMD_READ_RECORD_DATA  0x62
// 读取棋谱编号
#define CMD_READ_RECORD_IDS   0x63
// 写入棋谱数据
#define CMD_WRITE_RECORD_DATA 0x64
// 清除所有棋谱
#define CMD_CLEAR_ALL_RECORD  0x65

// 开始对弈
#define CMD_START_PLAY        0x68
// 开始复盘
#define CMD_START_REPLAY      0x69
// 开始习题
#define CMD_START_EXERCISES   0x6A 


// 指令
#define CMD_INDICATE_NONE           0x00   // 无指令
#define CMD_INDICATE_CLOCK_KEY1     0x01   // 棋钟按键1
#define CMD_INDICATE_CLOCK_KEY2     0x02   // 棋钟按键2
#define CMD_INDICATE_HINT           0x03   // 支招/错误提示
#define CMD_INDICATE_PASS           0x04   // 停一手
#define CMD_INDICATE_RESIGN         0x05   // 认输
#define CMD_INDICATE_ESTIMATE       0x06   // 形势判断

// host-->dev 命令码的最高位为1
// dev-->host 命令码的最高位为0

// 棋思通讯规约
class CQsPtl
{
public:
// 校准类型
typedef enum
{
	ctNoType  = 0,  // 无类型
	ctEmptyDA,      // 无棋子时的DA输出值
	ctEmptyAD,      // 无棋子时的AD采样值
	ctWhiteAD,      // 白棋子时的AD采样值
	ctBlackAD,      // 黑棋子时的AD采样值
}ECalType;

// 规约值
typedef struct
{
	// 设备ID
	uint8_t id[CMD_ID_LEN];
	// 报文编号
	uint8_t number;
	// 命令码
	uint8_t cmd;
	union
	{
		uint8_t ok;             // 是否正确
		uint8_t haveChess[46];  // 是否有子
		uint8_t blackChess[46]; // 是否黑子
		uint8_t allChess[95];   // 所有棋子
		uint16_t undoStep;      // 悔棋步数
		uint8_t indicate;       // 指令
		// 版本号
		struct
		{
			uint16_t audioV;    // 音频版本号
			uint16_t ptlV;      // 规约版本号
			uint32_t hardV;     // 硬件版本号
			uint32_t softV;     // 软件版本号
		}version;
		// 心跳
		struct
		{
			uint32_t time;      // 当前时间
			uint8_t alarm;      // 告警
			uint8_t reserve[3]; // 保留
			//uint8_t hostIp[4];  // 主机IP地址
		}heart;
		// 棋盘状态
		struct
		{
			float batV;         // 电池电压
			float temp;         // 温度
		}state;
		// 单个led值
		struct
		{
			uint8_t x;      // x轴
			uint8_t y;      // y轴
			uint8_t value;  // LED点亮时长，以10ms为单位
			uint8_t white;  // 白棋指示灯
			uint8_t black;  // 黑棋指示灯
			uint8_t beep;   // 蜂鸣器叫声时长，以10ms为单位
		}singleLed;
		// 所有led值
		uint8_t allLed[50];
		// 落子
		struct
		{
			uint8_t color;   // 颜色
			uint16_t number; // 手数
			uint8_t x;       // x坐标
			uint8_t y;       // y坐标
		}move;
		struct
		{
			uint16_t start;      // 开始位置
			uint16_t cnt;        // 个数
			union
			{
				SGoCoord coords[25]; // 坐标
				uint8_t ret;     // 回复值，返回落子是否正确
			};
		}moves;
		// 校准值
		struct
		{
			ECalType type;  // 校准类型
			int32_t index;  // 第几包
			uint16_t value[19];
		}cal;
		// 下载程序
		struct
		{
			int32_t address;  // 数据地址(address=-1时表示下载结束)
			int32_t length;   // 数据长度(address=-1时表示下载数据的总长度)
			int32_t checkCode;// 校验码(address=-1时表示下载数据的校验码)
			union
			{
				uint8_t d[1];     // 数据内容; 
				uint8_t d38[38];  // 数据内容; 短数据，用于USB
				uint8_t d512[512];// 数据内容; 长数据，用于网络
			}data;
		}downloadApp;
		// 播放语音
		struct
		{
			uint32_t audioIndex;// 语音编号
			char audioFile[32]; // 语音文件
		}playAudio;
		// 记谱
		struct
		{
			uint8_t handicap;
		}startRecord;
		// 棋谱数据
		struct
		{
			uint16_t id;
			uint16_t cnt;
			SGoCoord data[GO_RECORD_COORD_CNT];
		}recordData;
		// 棋谱ID
		struct
		{
			uint16_t cnt;
			uint16_t data[100];
		}recordIds;
		// 语音音量
		uint8_t volume;
		// 空闲休眠时间(s)
		uint8_t idleTime;
		// 亮度
		uint8_t lighteness;
		// 模式
		struct
		{
			uint8_t mode;              // 模式：0--正常模式；1--比赛模式
			uint8_t showHintLed;       // 显示提示LED：0--不显示；1--显示
			uint8_t boardSound;        // 落子声音：0--无声音；1--蜂鸣器；11及以上--喇叭声音
			uint8_t keyMove;          // 使用棋钟按键确认落子 0--落子马上记谱；1--用棋钟按键确认后记谱
			//uint8_t reserve;           // 保留
		}mode;
		// 提子延迟时间(ms)
		uint16_t takeDelay;
		// 后台服务软件IP地址、端口、连接类型
		struct
		{
			char ipAddr[32];           // ip地址
			uint32_t port;             // 端口
			uint8_t linkKind;          // 类型
			uint8_t reserve[3];        // 保留
		}webService;
		// 开始对弈
		struct
		{
			uint8_t handicap;          // 让子数
			uint8_t selfColor;         // 我方颜色，如我方颜色是无，表示是人人对弈
		}startPlay;
		// 开始复盘/打谱
		struct
		{
			uint16_t mode;             // 模式，0：直接提示落子位置，1--10：猜棋次数，超过后直接显示落子位置
			uint16_t start;            // 当前包棋谱起始手数
			uint16_t cnt;              // 当前包棋谱总手数
			union
			{
				// 棋谱数据
				SGoCoord d1[1];
				SGoCoord d20[20];
				SGoCoord dall[GO_RECORD_COORD_CNT];
				uint8_t ret;           // 返回值
			}data;
		}startReplay;

	};
	
	// 设置指令
	void SetIndicate(uint8_t indicate)
	{
			haveChess[sizeof(haveChess) - 1] &= 0x80;
			haveChess[sizeof(haveChess) - 1] |= indicate;
	}
	// 得到指令
	uint8_t GetIndicate(void)
	{
			return haveChess[sizeof(haveChess) - 1] & 0x0F;
	}
}SPtlValue;

private:
	// 是否是Host端
	bool m_isHost;
	// 接收数据空间
	uint8_t *m_recvData;
	// 接收数据空间大小
	int m_recvDataSize;
	// 接收数据长度的字段
	uint16_t m_recvDataLenField;
	// 接收数据长度
	uint16_t m_recvDataLen;
	// 接收的命令码
	uint8_t m_recvCmd;
	// 设备的ID
	uint8_t m_id[CMD_ID_LEN];
	// 检查校验码
	bool m_checkCC;
	// 检查ID
	bool m_checkId;
public:
	// 构造函数
	CQsPtl()
	{
		m_isHost = false;
		m_recvData = 0;
		m_recvDataSize = 0;
		m_recvDataLenField = 0;
		m_recvDataLen = 0;
		m_recvCmd = 0;
		m_checkCC = true;
		m_checkId = true;
	};
	// 析构函数
	virtual ~CQsPtl()
	{
	};
	// 设置参数
	void SetParamter(bool isHost, char id[CMD_ID_LEN], void *recvData, int recvDataSize)
	{
		m_isHost = isHost;
		if(id)
		{
			memcpy(m_id, id, CMD_ID_LEN);
		}
		m_recvData = (uint8_t*)recvData;
		m_recvDataSize = recvDataSize;
		m_recvCmd = 0;
		m_recvDataLen = 0;
	};
	// 编码
	int encode(void *buf, int bufSize, SPtlValue *value)
	{
		uint8_t *p = (uint8_t *)buf;
		p[0] = 0xC6;
		p[1] = 0xE5;
		p += 2;
		//memcpy(p, value->id, sizeof(value->id));
		memcpy(p, m_id, sizeof(m_id));
		p += sizeof(m_id);
		p[0] = value->number;
		p += sizeof(value->number);
		p[0] = value->cmd;
		if(m_isHost)
		{
			p[0] |= 0x80;
		}
		p += sizeof(value->cmd);
		uint8_t *pDataLen = p;
		uint16_t dataLenField = 0;
		p += sizeof(dataLenField);
		switch(value->cmd)
		{
#if !defined(QSBOOT)
		case CMD_HEART:            // 心跳
			u32toa(p, value->heart.time, true);
			p += 4;
			//memcpy(p, &value->heart.hostIp, sizeof(value->heart.hostIp));
			p[0] = value->heart.alarm;
			dataLenField = sizeof(value->heart);
			break;
		case CMD_READ_STATE:       // 读取棋盘状态
			memcpy(p, &value->state, sizeof(value->state));
			dataLenField = sizeof(value->state);
			break;
		case CMD_VOLUME:           // 语音音量
			dataLenField = sizeof(value->volume);
			*p = value->volume;
			break;
		case CMD_IDLE_TIME:        // 空闲休眠时间(s)
			dataLenField = sizeof(value->idleTime);
			*p = value->idleTime;
			break;
		case CMD_LIGHTENESS:       // 亮度
			dataLenField = sizeof(value->lighteness);
			*p = value->lighteness;
			break;
		case CMD_MODE:             // 模式
			dataLenField = sizeof(value->mode);
			p[0] = value->mode.mode;
			p[1] = value->mode.showHintLed;
			p[2] = value->mode.boardSound;
			p[3] = value->mode.keyMove;
			break;
		case CMD_TAKE_DELAY:       // 提子延迟时间(ms)
			dataLenField = sizeof(value->takeDelay);
			u16toa(p, value->takeDelay, true);
			break;
		case CMD_WEB_SERVICE:      // 后台服务
			dataLenField = sizeof(value->webService);
			memcpy(p, value->webService.ipAddr, sizeof(value->webService.ipAddr));
			p += sizeof(value->webService.ipAddr);
			u32toa(p, value->webService.port, true);
			p += sizeof(value->webService.port);
			*p = value->webService.linkKind;
			break;
		case CMD_READ_VERSION:     // 读取版本
			if(m_isHost)
			{
				dataLenField = 0;
			}else
			{
				dataLenField = sizeof(value->version);
				u16toa(p, value->version.audioV, true);
				p += sizeof(value->version.audioV);
				u16toa(p, value->version.ptlV, true);
				p += sizeof(value->version.ptlV);
				u32toa(p, value->version.hardV, true);
				p += sizeof(value->version.hardV);
				u32toa(p, value->version.softV, true);
			}
			break;
		case CMD_WRITE_ALL_LED:    // 写所有LED
			if(m_isHost)
			{
				dataLenField = sizeof(value->allLed);
				memcpy(p, value->allLed, dataLenField);
			}else
			{
				p[0] = value->ok;
				dataLenField = 1;
			}
			break;
		case CMD_WRITE_SINGLE_LED: // 写单个LED
			if(m_isHost)
			{
				p[0] = value->singleLed.x;
				p[1] = value->singleLed.y;
				p[2] = value->singleLed.value;
				p[3] = value->singleLed.white;
				p[4] = value->singleLed.black;
				p[5] = value->singleLed.beep;
				dataLenField = 6;
			}else
			{
				p[0] = value->ok;
				dataLenField = 1;
			}
			break;
		case CMD_READ_HAVE_CHESS:  // 读取有无棋子
			if(m_isHost)
			{
				dataLenField = 0;
			}else
			{
				dataLenField = sizeof(value->haveChess);
				if(bufSize < CMD_MIN_PACKET_LEN + dataLenField)
				{
					return CMD_ERROR_BUF_NOT_ENOUGH;
				}
				memcpy(p, value->haveChess, dataLenField);
			}
			break;
		case CMD_READ_BW_CHESS:    // 读取黑白棋子
			if(m_isHost)
			{
				dataLenField = 0;
			}else
			{
				dataLenField = sizeof(value->blackChess);
				if(bufSize < CMD_MIN_PACKET_LEN + dataLenField)
				{
					return CMD_ERROR_BUF_NOT_ENOUGH;
				}
				memcpy(p, value->blackChess, dataLenField);
			}
			break;
		case CMD_READ_ALL_CHESS:   // 读取所有棋子
		case CMD_SEND_ALL_CHESS:   // 发送所有棋子
			if(m_isHost)
			{
				dataLenField = 0;
			}else
			{
				dataLenField = sizeof(value->allChess);
				if(bufSize < CMD_MIN_PACKET_LEN + dataLenField)
				{
					return CMD_ERROR_BUF_NOT_ENOUGH;
				}
				memcpy(p, value->allChess, dataLenField);
			}
			break;
		case CMD_HOST_UNDO:        // 服务器悔棋
			dataLenField = sizeof(value->undoStep);
			u16toa(p, value->undoStep, true);
			break;
		case CMD_BOARD_UNDO:       // 棋盘悔棋
			break;
		case CMD_COMPUTER_MOVE:    // 电脑落子
		case CMD_HUMAN_MOVE:       // 人落子
			if((CMD_COMPUTER_MOVE == value->cmd && m_isHost) || (CMD_HUMAN_MOVE == value->cmd && !m_isHost))
			{
				if(value->moves.cnt > countof(value->moves.coords))
				{
					return CMD_ERROR_BUF_NOT_ENOUGH;
				}
				u16toa(p, value->moves.start, true);
				p += sizeof(value->moves.start);
				u16toa(p, value->moves.cnt, true);
				p += sizeof(value->moves.cnt);
				int cnt = value->moves.cnt * sizeof(value->moves.coords[0]);
				memcpy(p, value->moves.coords, cnt);
				dataLenField = 4 + cnt;
			}else
			{
				u16toa(p, value->moves.start, true);
				p += sizeof(value->moves.start);
				u16toa(p, value->moves.cnt, true);
				p += sizeof(value->moves.cnt);
				*p = value->moves.ret;
				dataLenField = 5;
			}
			break;
		case CMD_HOST_INDICATE:        // 服务器指令
			if(m_isHost)
			{
				dataLenField = sizeof(value->indicate);
				p[0] = value->indicate;
			}
			break;
		case CMD_BOARD_INDICATE:
			if(!m_isHost)
			{
				dataLenField = sizeof(value->indicate);
				p[0] = value->indicate;
			}
			break;
		case CMD_START_CAL:        // 开始校准
		case CMD_READ_CAL_DATA:    // 读取校准数据
			if(m_isHost)
			{
				dataLenField = 0;
			}else
			{
				dataLenField = sizeof(value->cal.value);
			}
			*p = value->cal.type;
			p += 1;
			*p = value->cal.index;
			p += 1;
			if(bufSize < CMD_MIN_PACKET_LEN + dataLenField + 2)
			{
				return CMD_ERROR_BUF_NOT_ENOUGH;
			}
			memcpy(p, value->cal.value, dataLenField);
			dataLenField += 2;
			break;
		case CMD_PLAY_AUDIO:       // 播放语音
			if(m_isHost)
			{
				dataLenField = sizeof(value->playAudio);
				u32toa(p, value->playAudio.audioIndex, true);
				p += 4;
				memcpy(p, value->playAudio.audioFile, sizeof(value->playAudio.audioFile));
			}else
			{
				dataLenField = 0;
			}
			break;
		case CMD_START_RECORD:      // 开始记谱
			dataLenField = sizeof(value->startRecord);
			*p = value->startRecord.handicap;
			break;
		case CMD_UPLOAD_RECORD_DATA:    // 上送棋谱信息
		case CMD_READ_RECORD_DATA:      // 读取棋谱信息
			u16toa(p, value->recordData.id, true);
			p += sizeof(value->recordData.id);
			dataLenField = sizeof(value->recordData.id);
			if(m_isHost)
			{
			}else
			{
				int recordLen = 0;
				//if(CMD_UPLOAD_RECORD_DATA == value->cmd)
				{
					recordLen = value->recordData.cnt * sizeof(value->recordData.data[0]);
					memcpy(p, value->recordData.data, recordLen);
				}
				dataLenField += recordLen;
			}
			break;
		case CMD_READ_RECORD_IDS:       // 读取棋谱编号
			if(m_isHost)
			{
				dataLenField = 0;
			}else
			{
				dataLenField = value->recordIds.cnt * sizeof(value->recordIds.data[0]);
				for(int i = 0; i < value->recordIds.cnt; i++)
				{
					u16toa(p, value->recordIds.data[i], true);
					p += sizeof(value->recordIds.data[i]);
				}
				//memcpy(p, value->recordIds.data, dataLenField);
			}
			break;
		case CMD_WRITE_RECORD_DATA:     // 写入棋谱数据
			if(m_isHost)
			{
				dataLenField = sizeof(value->recordData.id);
				u16toa(p, value->recordData.id, true);
				p += dataLenField;
				int recordLen = value->recordData.cnt * sizeof(value->recordData.data[0]);
				memcpy(p, value->recordData.data, recordLen);
				dataLenField += recordLen;
			}else
			{
				dataLenField = 1;
				*p = value->ok;
			}
			break;
		case CMD_CLEAR_ALL_RECORD:      // 清除所有棋谱
			if(m_isHost)
			{
				dataLenField = 0;
			}
			break;
#endif
		case CMD_DOWNLOAD_APP:           // 下载程序
			u32toa(p, value->downloadApp.address, true);
			p += 4;
			u32toa(p, value->downloadApp.length, true);
			p += 4;
			u32toa(p, value->downloadApp.checkCode, true);
			p += 4;
			dataLenField = 12;
			if(m_isHost)
			{
				if(value->downloadApp.address >= 0)
				{
					if(value->downloadApp.length > sizeof(value->downloadApp.data))
					{
						return CMD_ERROR_DATA_LEN;
					}
					memcpy(p, value->downloadApp.data.d, value->downloadApp.length);
					dataLenField += value->downloadApp.length;
				}
			}
			break;
		case CMD_START_PLAY:             // 开始对弈
			p[2] = value->startPlay.selfColor;
			p[1] = value->startPlay.handicap;
			dataLenField = sizeof(value->startPlay);
			break;
		case CMD_START_REPLAY:           // 开始复盘
			u16toa(p, value->startReplay.mode, true);
			p += sizeof(value->startReplay.mode);
			u16toa(p, value->startReplay.start, true);
			p += sizeof(value->startReplay.start);
			u16toa(p, value->startReplay.cnt, true);
			p += sizeof(value->startReplay.cnt);
			dataLenField = 6;
			if(m_isHost)
			{
				int recordLen = value->startReplay.cnt * sizeof(value->startReplay.data.dall[0]);
				memcpy(p, value->startReplay.data.dall, recordLen);
				dataLenField += recordLen;
			}else
			{
				*p = value->startReplay.data.ret;
				dataLenField += sizeof(value->startReplay.data.ret);
			}
			break;
		case CMD_START_EXERCISES:        // 开始习题
			break;
		}
		// 长度
		u16toa(pDataLen, dataLenField, true);
		// 校验码
		uint8_t *pCrc = pDataLen + 2 + dataLenField;
		uint16_t crc = calc_crc16_ccitt(CMD_CRC_INIT_CODE, buf, dataLenField + CMD_CRC_CALC_LEN);
		u16toa(pCrc, crc, true);
		// 返回报文长度
		return dataLenField + CMD_MIN_PACKET_LEN;
	};
	
	// 解码
	int decode(void *buf, int bufSize, SPtlValue *value)
	{
		// 不是首包，需要把报文数据连接起来
		if(m_recvDataLenField > 0)
		{
			// 判断数据是否超出缓存区大小
			if(CMD_CRC_CALC_LEN + m_recvDataLen + bufSize > m_recvDataSize)
			{
				m_recvDataLenField = 0;
				return CMD_ERROR_BUF_NOT_ENOUGH;
			}
			// 复制数据到缓存区
			memcpy(m_recvData + CMD_CRC_CALC_LEN + m_recvDataLen, buf, bufSize);
			m_recvDataLen += bufSize;
		}

		// 是首包数据
		if(0 == m_recvDataLenField)
		{
			if(bufSize >= CMD_MIN_PACKET_LEN)
			{
				uint8_t *p = (uint8_t *)buf;
				bool find = false;
				for(int i = 0; i < bufSize; i++)
				{
					if(0xC6 == p[0] && 0xE5 == p[1])
					{
						find = true;
						break;
					}
					p++;
				}
				if(!find)
				{
					return CMD_ERROR_NO_HEAD;
				}
				p += 2;
				// id，000000是超级id
				if(m_checkId)
				{	
					// 自己是否是超级id
					bool selfIsSuperId = 0 == memcmp(m_id, "000000", sizeof(m_id));
					// 联络方是否是超级id
					bool liaisonIsSuperId = 0 == memcmp(p, "000000", sizeof(m_id));
					// id是否相同
					bool commonIdEqu = 0 == memcmp(m_id, p, sizeof(m_id));
					if(!selfIsSuperId && !liaisonIsSuperId && !commonIdEqu)
					{
						return CMD_ERROR_ID_NOT_MATCH;
					}
				}
				//memcpy(m_id, p, sizeof(m_id));
				p += sizeof(m_id);
				// 报文编号
				value->number = p[0];
				p += sizeof(value->number);
				// 命令码
				m_recvCmd = p[0];
				// 判断命令码是否合法
				if(m_isHost && (m_recvCmd >= 0x80))
				{
					m_recvDataLenField = 0;
					return CMD_ERROR_CMD;
				}
				if(!m_isHost && (m_recvCmd < 0x80))
				{
					m_recvDataLenField = 0;
					return CMD_ERROR_CMD;
				}
				p += sizeof(value->cmd);
				// 总长度
				m_recvDataLenField = atou16(p, true);
				if(m_recvDataLenField + CMD_MIN_PACKET_LEN > m_recvDataSize)
				{
					m_recvDataLenField = 0;
					return CMD_ERROR_BUF_NOT_ENOUGH;
				}
				if(m_recvDataLenField > CMD_MAX_DATA_LEN)
				{
					return CMD_ERROR_DATA_LEN;
				}
				p += sizeof(m_recvDataLenField);
				// 复制数据
				uint16_t dataLen = bufSize - CMD_CRC_CALC_LEN;
				if(dataLen > m_recvDataSize)
				{
					dataLen = m_recvDataSize;
				}
				memcpy(m_recvData, buf, bufSize);
				m_recvDataLen = dataLen;
			}else
			{
				return CMD_ERROR_PACK_LEN_TOO_SHORT;
			}
		}

		// 接收到的数据长度，大于等于长度字段的值，表示数据接收内容完成，可以解析数据
		if(m_recvDataLen >= m_recvDataLenField)// && m_recvDataLenField > 0)
		{
			// 检查校验码
			if(m_checkCC)
			{
				int crcLen = m_recvDataLenField + CMD_CRC_CALC_LEN;
				uint8_t *pCrc = m_recvData + crcLen;
				uint16_t crc1 = calc_crc16_ccitt(CMD_CRC_INIT_CODE, m_recvData, crcLen);
				uint16_t crc2 = atou16(pCrc, true);
				if(crc1 != crc2)
				{
					m_recvDataLenField = 0;
					m_recvDataLen = 0;
					return CMD_ERROR_CHECK_CODE;
				}
			}
			value->cmd = m_recvCmd & 0x7F;
			memcpy(value->id, m_id, sizeof(m_id));
			uint8_t *p = m_recvData + CMD_CRC_CALC_LEN;
			switch(value->cmd)
			{
#if !defined(QSBOOT)
			case CMD_HEART:
				value->heart.time = atou32(p, true);
				p += 4;
				value->heart.alarm = p[0];
				//memcpy(&value->heart.hostIp, p, sizeof(value->heart.hostIp));
				break;
			case CMD_READ_STATE:
				memcpy(&value->state, p, sizeof(value->state));
				break;
			case CMD_VOLUME:           // 语音音量
				value->volume = *p;
				break;
			case CMD_IDLE_TIME:        // 空闲休眠时间(s)
				value->idleTime = *p;
				break;
			case CMD_LIGHTENESS:       // 亮度
				value->lighteness = *p;
				break;
			case CMD_MODE:             // 模式
				value->mode.mode        = p[0];
				value->mode.showHintLed = p[1];
				value->mode.boardSound  = p[2];
				value->mode.keyMove    = p[3];
				break;
			case CMD_TAKE_DELAY:       // 提子延迟时间(ms)
				value->takeDelay = atou16(p, true);
				break;
			case CMD_WEB_SERVICE:      // 后台服务
				memcpy(value->webService.ipAddr, p, sizeof(value->webService.ipAddr));
				p += sizeof(value->webService.ipAddr);
				value->webService.port = atou32(p, true);
				p += sizeof(value->webService.port);
				value->webService.linkKind = *p;
				break;
			case CMD_READ_VERSION:
				value->version.audioV = atou16(p, true);
				p += sizeof(value->version.audioV);
				value->version.ptlV = atou32(p, true);
				p += sizeof(value->version.ptlV);
				value->version.hardV = atou32(p, true);
				p += sizeof(value->version.hardV);
				value->version.softV = atou32(p, true);
				break;
			case CMD_WRITE_ALL_LED:
				if(m_isHost)
				{
				}else
				{
					memcpy(value->allLed, p, sizeof(value->allLed));
				}
				break;
			case CMD_WRITE_SINGLE_LED:
				if(m_isHost)
				{
				}else
				{
					value->singleLed.x     = p[0];
					value->singleLed.y     = p[1];
					value->singleLed.value = p[2];
					value->singleLed.white = p[3];
					value->singleLed.black = p[4];
					value->singleLed.beep  = p[5];
				}
				break;
			case CMD_READ_HAVE_CHESS:
				if(m_isHost)
				{
					memcpy(value->haveChess, p, sizeof(value->haveChess));
				}else
				{
				}
				break;
			case CMD_READ_BW_CHESS:
				if(m_isHost)
				{
					memcpy(value->blackChess, p, sizeof(value->blackChess));
				}else
				{
				}
				break;
			case CMD_READ_ALL_CHESS:
			case CMD_SEND_ALL_CHESS:
				if(m_isHost)
				{
					memcpy(value->allChess, p, sizeof(value->allChess));
				}else
				{
				}
				break;
			case CMD_HOST_UNDO:        // 悔棋
				value->undoStep = atou16(p, true);
				break;
			case CMD_BOARD_UNDO:       // 棋盘悔棋
				break;
			case CMD_COMPUTER_MOVE:    // 电脑落子
			case CMD_HUMAN_MOVE:       // 人落子
				if((CMD_COMPUTER_MOVE == value->cmd && !m_isHost) || (CMD_HUMAN_MOVE == value->cmd && m_isHost))
				{
					value->moves.start = atou16(p, true);
					p += sizeof(value->moves.start);
					value->moves.cnt = atou16(p, true);
					p += sizeof(value->moves.cnt);
					if(value->moves.cnt > countof(value->moves.coords))
					{
						return CMD_ERROR_BUF_NOT_ENOUGH;
					}
					memcpy(value->moves.coords, p, value->moves.cnt * sizeof(value->moves.coords[0]));
					//dataLen = sizeof(value->moves);
				}else
				{
					value->moves.start = atou16(p, true);
					p += sizeof(value->moves.start);
					value->moves.cnt = atou16(p, true);
					p += sizeof(value->moves.cnt);
					value->moves.ret = *p;
				}
				break;
			case CMD_HOST_INDICATE:        // 服务器指令
				if(!m_isHost)
				{
					value->indicate = p[0];
				}
				break;
			case CMD_BOARD_INDICATE:
				if(m_isHost)
				{
					value->indicate = p[0];
				}
				break;
			case CMD_START_CAL:
			case CMD_READ_CAL_DATA:
				value->cal.type = (ECalType)*p;
				p += 1;
				value->cal.index = *p;
				p += 1;
				if(m_isHost)
				{
					memcpy(value->cal.value, p, sizeof(value->cal.value));
				}else
				{
				}
				break;
			case CMD_PLAY_AUDIO:
				if(!m_isHost)
				{
					value->playAudio.audioIndex = atou32(p, true);
					p += 4;
					memcpy(value->playAudio.audioFile, p, sizeof(value->playAudio.audioFile));
				}else
				{
				}
				break;
			case CMD_START_RECORD:
				if(m_isHost)
				{
				}else
				{
					value->startRecord.handicap = *p;
				}
				break;
			case CMD_UPLOAD_RECORD_DATA:
			case CMD_READ_RECORD_DATA:
				value->recordData.id = atou16(p, true);
				p += sizeof(value->recordData.id);
				if(m_isHost)
				{
					value->recordData.cnt = (m_recvDataLenField - sizeof(value->recordData.id)) / sizeof(value->recordData.data[0]);
					value->recordData.cnt = min(value->recordData.cnt, countof(value->recordData.data));
					p += sizeof(value->recordData.cnt);
					memcpy(value->recordData.data, p, value->recordData.cnt * sizeof(value->recordData.data[0]));
				}else
				{
				}
				break;
			case CMD_READ_RECORD_IDS:       // 读取棋谱编号
				if(m_isHost)
				{
					value->recordIds.cnt = atou16(p, true);
					value->recordIds.cnt = min(value->recordIds.cnt, countof(value->recordIds.data));
					p += 2;
					for(int i = 0; i < value->recordIds.cnt; i++)
					{
						value->recordIds.data[i] = atou16(p, true);
						p += sizeof(value->recordIds.data[i]);
					}
					//memcpy(value->recordIds.data, p, value->recordIds.cnt * sizeof(value->recordIds.data[0]));
				}else
				{
				}
				break;
			case CMD_WRITE_RECORD_DATA:     // 写入棋谱数据
				if(m_isHost)
				{
					value->ok = *p;
				}else
				{
					if(m_recvDataLenField >= sizeof(value->recordData.id))
					{
						value->recordData.id = atou16(p, true);
						value->recordData.cnt = (m_recvDataLenField - sizeof(value->recordData.id)) / sizeof(value->recordData.data[0]);
						p += sizeof(value->recordData.id);
						int recordLen = value->recordData.cnt * sizeof(value->recordData.data[0]);
						memcpy(value->recordData.data, p, recordLen);
					}else
					{
						return CMD_ERROR_DATA_LEN;
					}
				}
				break;
			case CMD_CLEAR_ALL_RECORD:      // 清除所有棋谱
				break;
#endif
			case CMD_DOWNLOAD_APP:
				//if(m_isHost)
				//{
				//
				//}else
				//{
					value->downloadApp.address = atou32(p, true);
					if(!m_isHost)
					{
						p += 4;
						value->downloadApp.length = atou32(p, true);
						p += 4;
						value->downloadApp.checkCode = atou32(p, true);
						if(value->downloadApp.address < 0)
						{
							
						}else
						{
							if(value->downloadApp.length > sizeof(value->downloadApp.data))
							{
								return CMD_ERROR_DATA_LEN;
							}
							p += 4;
							memcpy(value->downloadApp.data.d, p, value->downloadApp.length);
						}
					}
				//}
				break;
			case CMD_START_PLAY:             // 开始对弈
				value->startPlay.selfColor = p[0];
				value->startPlay.handicap = p[1];
				break;
			case CMD_START_REPLAY:           // 开始复盘
				value->startReplay.mode = atou16(p, true);
				p += sizeof(value->startReplay.mode);
				value->startReplay.start = atou16(p, true);
				p += sizeof(value->startReplay.start);
				value->startReplay.cnt = atou16(p, true);
				p += sizeof(value->startReplay.cnt);
				if(m_isHost)
				{
					value->startReplay.data.ret = *p;
				}else
				{
					int recordLen = value->startReplay.cnt * sizeof(value->startReplay.data.dall[0]);
					if(recordLen > sizeof(value->startReplay.data))
					{
						return CMD_ERROR_DATA_LEN;
					}
					memcpy(value->startReplay.data.dall, p, recordLen);
				}
				break;
			case CMD_START_EXERCISES:        // 开始习题
				break;
			default:
				return CMD_ERROR_CMD;
			}
			m_recvDataLen = 0;
			m_recvDataLenField = 0;
			return CMD_DECODE_OK;
		}else
		{
			return CMD_ERROR_DATA_NOT_FULL;
		}
	};
};

#endif
