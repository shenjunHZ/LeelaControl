// ��˼ͨѶ��Լ
#ifndef __QS_PROTOCOL_H__
#define __QS_PROTOCOL_H__

#include <stdint.h>
#include <string.h>
#include "function.h"
#include "goType.h"

// �����붨��
// û���ҵ�����ͷ
#define CMD_DECODE_OK                1
// û���ҵ�����ͷ
#define CMD_ERROR_NO_HEAD            -1
// У�������
#define CMD_ERROR_CHECK_CODE         -2
// ���ݳ��ȴ���
#define CMD_ERROR_DATA_LEN           -3
// ���Ļ�û����ȫ����
#define CMD_ERROR_DATA_NOT_FULL      -4
// û���㹻����������
#define CMD_ERROR_BUF_NOT_ENOUGH     -5
// ���ĳ���С����С���ȴ���
#define CMD_ERROR_PACK_LEN_TOO_SHORT -6
// ID��ƥ�����
#define CMD_ERROR_ID_NOT_MATCH       -7
// ���������
#define CMD_ERROR_CMD                -10


// У������㳤��
#define CMD_CRC_CALC_LEN      12
// ������С����
#define CMD_MIN_PACKET_LEN    (CMD_CRC_CALC_LEN + 2)
// ID����
#define CMD_ID_LEN            6
// crc��ʼֵ
#define CMD_CRC_INIT_CODE     0xFFFF
// ������ݳ���
#define CMD_MAX_DATA_LEN      1000

// ��ǰ��Լ�汾
#define CURR_PTL_VERSION 0x00000000

// �������
// ��ȡ�汾
#define CMD_READ_VERSION      0x01
// ��ȡ��������
#define CMD_READ_HAVE_CHESS   0x02
// ��ȡ�ڰ�����
#define CMD_READ_BW_CHESS     0x03
// ��ȡ��������
#define CMD_READ_ALL_CHESS    0x04
// ������������
#define CMD_SEND_ALL_CHESS    0x05
// ����������
#define CMD_HOST_UNDO         0x06
// ���̻���
#define CMD_BOARD_UNDO        0x07
// ��������
#define CMD_COMPUTER_MOVE     0x08
// ������
#define CMD_HUMAN_MOVE        0x09
// ������ָ��
#define CMD_HOST_INDICATE     0x0A
// ����ָ��
#define CMD_BOARD_INDICATE    0x0B
// д����LED
#define CMD_WRITE_ALL_LED     0x10
// д����LED
#define CMD_WRITE_SINGLE_LED  0x11
// ��ʼУ׼
#define CMD_START_CAL         0x20
// ��ȡУ׼����
#define CMD_READ_CAL_DATA     0x21
// ����
#define CMD_HEART             0x30
// ��ȡ����״̬
#define CMD_READ_STATE        0x31
// ��������
#define CMD_VOLUME            0x32
// ��������ʱ��(s)
#define CMD_IDLE_TIME         0x33
// ����
#define CMD_LIGHTENESS        0x34
// ģʽ
#define CMD_MODE              0x35
// �����ӳ�ʱ��(ms)
#define CMD_TAKE_DELAY        0x36
// ��̨����
#define CMD_WEB_SERVICE       0x37
// ���س���
#define CMD_DOWNLOAD_APP      0x40
// ��������
#define CMD_PLAY_AUDIO        0x50

// ��ʼ����
#define CMD_START_RECORD      0x60
// ������������
#define CMD_UPLOAD_RECORD_DATA 0x61
// ��ȡ��������
#define CMD_READ_RECORD_DATA  0x62
// ��ȡ���ױ��
#define CMD_READ_RECORD_IDS   0x63
// д����������
#define CMD_WRITE_RECORD_DATA 0x64
// �����������
#define CMD_CLEAR_ALL_RECORD  0x65

// ��ʼ����
#define CMD_START_PLAY        0x68
// ��ʼ����
#define CMD_START_REPLAY      0x69
// ��ʼϰ��
#define CMD_START_EXERCISES   0x6A 


// ָ��
#define CMD_INDICATE_NONE           0x00   // ��ָ��
#define CMD_INDICATE_CLOCK_KEY1     0x01   // ���Ӱ���1
#define CMD_INDICATE_CLOCK_KEY2     0x02   // ���Ӱ���2
#define CMD_INDICATE_HINT           0x03   // ֧��/������ʾ
#define CMD_INDICATE_PASS           0x04   // ͣһ��
#define CMD_INDICATE_RESIGN         0x05   // ����
#define CMD_INDICATE_ESTIMATE       0x06   // �����ж�

// host-->dev ����������λΪ1
// dev-->host ����������λΪ0

// ��˼ͨѶ��Լ
class CQsPtl
{
public:
// У׼����
typedef enum
{
	ctNoType  = 0,  // ������
	ctEmptyDA,      // ������ʱ��DA���ֵ
	ctEmptyAD,      // ������ʱ��AD����ֵ
	ctWhiteAD,      // ������ʱ��AD����ֵ
	ctBlackAD,      // ������ʱ��AD����ֵ
}ECalType;

// ��Լֵ
typedef struct
{
	// �豸ID
	uint8_t id[CMD_ID_LEN];
	// ���ı��
	uint8_t number;
	// ������
	uint8_t cmd;
	union
	{
		uint8_t ok;             // �Ƿ���ȷ
		uint8_t haveChess[46];  // �Ƿ�����
		uint8_t blackChess[46]; // �Ƿ����
		uint8_t allChess[95];   // ��������
		uint16_t undoStep;      // ���岽��
		uint8_t indicate;       // ָ��
		// �汾��
		struct
		{
			uint16_t audioV;    // ��Ƶ�汾��
			uint16_t ptlV;      // ��Լ�汾��
			uint32_t hardV;     // Ӳ���汾��
			uint32_t softV;     // ����汾��
		}version;
		// ����
		struct
		{
			uint32_t time;      // ��ǰʱ��
			uint8_t alarm;      // �澯
			uint8_t reserve[3]; // ����
			//uint8_t hostIp[4];  // ����IP��ַ
		}heart;
		// ����״̬
		struct
		{
			float batV;         // ��ص�ѹ
			float temp;         // �¶�
		}state;
		// ����ledֵ
		struct
		{
			uint8_t x;      // x��
			uint8_t y;      // y��
			uint8_t value;  // LED����ʱ������10msΪ��λ
			uint8_t white;  // ����ָʾ��
			uint8_t black;  // ����ָʾ��
			uint8_t beep;   // ����������ʱ������10msΪ��λ
		}singleLed;
		// ����ledֵ
		uint8_t allLed[50];
		// ����
		struct
		{
			uint8_t color;   // ��ɫ
			uint16_t number; // ����
			uint8_t x;       // x����
			uint8_t y;       // y����
		}move;
		struct
		{
			uint16_t start;      // ��ʼλ��
			uint16_t cnt;        // ����
			union
			{
				SGoCoord coords[25]; // ����
				uint8_t ret;     // �ظ�ֵ�����������Ƿ���ȷ
			};
		}moves;
		// У׼ֵ
		struct
		{
			ECalType type;  // У׼����
			int32_t index;  // �ڼ���
			uint16_t value[19];
		}cal;
		// ���س���
		struct
		{
			int32_t address;  // ���ݵ�ַ(address=-1ʱ��ʾ���ؽ���)
			int32_t length;   // ���ݳ���(address=-1ʱ��ʾ�������ݵ��ܳ���)
			int32_t checkCode;// У����(address=-1ʱ��ʾ�������ݵ�У����)
			union
			{
				uint8_t d[1];     // ��������; 
				uint8_t d38[38];  // ��������; �����ݣ�����USB
				uint8_t d512[512];// ��������; �����ݣ���������
			}data;
		}downloadApp;
		// ��������
		struct
		{
			uint32_t audioIndex;// �������
			char audioFile[32]; // �����ļ�
		}playAudio;
		// ����
		struct
		{
			uint8_t handicap;
		}startRecord;
		// ��������
		struct
		{
			uint16_t id;
			uint16_t cnt;
			SGoCoord data[GO_RECORD_COORD_CNT];
		}recordData;
		// ����ID
		struct
		{
			uint16_t cnt;
			uint16_t data[100];
		}recordIds;
		// ��������
		uint8_t volume;
		// ��������ʱ��(s)
		uint8_t idleTime;
		// ����
		uint8_t lighteness;
		// ģʽ
		struct
		{
			uint8_t mode;              // ģʽ��0--����ģʽ��1--����ģʽ
			uint8_t showHintLed;       // ��ʾ��ʾLED��0--����ʾ��1--��ʾ
			uint8_t boardSound;        // ����������0--��������1--��������11������--��������
			uint8_t keyMove;          // ʹ�����Ӱ���ȷ������ 0--�������ϼ��ף�1--�����Ӱ���ȷ�Ϻ����
			//uint8_t reserve;           // ����
		}mode;
		// �����ӳ�ʱ��(ms)
		uint16_t takeDelay;
		// ��̨�������IP��ַ���˿ڡ���������
		struct
		{
			char ipAddr[32];           // ip��ַ
			uint32_t port;             // �˿�
			uint8_t linkKind;          // ����
			uint8_t reserve[3];        // ����
		}webService;
		// ��ʼ����
		struct
		{
			uint8_t handicap;          // ������
			uint8_t selfColor;         // �ҷ���ɫ�����ҷ���ɫ���ޣ���ʾ�����˶���
		}startPlay;
		// ��ʼ����/����
		struct
		{
			uint16_t mode;             // ģʽ��0��ֱ����ʾ����λ�ã�1--10�����������������ֱ����ʾ����λ��
			uint16_t start;            // ��ǰ��������ʼ����
			uint16_t cnt;              // ��ǰ������������
			union
			{
				// ��������
				SGoCoord d1[1];
				SGoCoord d20[20];
				SGoCoord dall[GO_RECORD_COORD_CNT];
				uint8_t ret;           // ����ֵ
			}data;
		}startReplay;

	};
	
	// ����ָ��
	void SetIndicate(uint8_t indicate)
	{
			haveChess[sizeof(haveChess) - 1] &= 0x80;
			haveChess[sizeof(haveChess) - 1] |= indicate;
	}
	// �õ�ָ��
	uint8_t GetIndicate(void)
	{
			return haveChess[sizeof(haveChess) - 1] & 0x0F;
	}
}SPtlValue;

private:
	// �Ƿ���Host��
	bool m_isHost;
	// �������ݿռ�
	uint8_t *m_recvData;
	// �������ݿռ��С
	int m_recvDataSize;
	// �������ݳ��ȵ��ֶ�
	uint16_t m_recvDataLenField;
	// �������ݳ���
	uint16_t m_recvDataLen;
	// ���յ�������
	uint8_t m_recvCmd;
	// �豸��ID
	uint8_t m_id[CMD_ID_LEN];
	// ���У����
	bool m_checkCC;
	// ���ID
	bool m_checkId;
public:
	// ���캯��
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
	// ��������
	virtual ~CQsPtl()
	{
	};
	// ���ò���
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
	// ����
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
		case CMD_HEART:            // ����
			u32toa(p, value->heart.time, true);
			p += 4;
			//memcpy(p, &value->heart.hostIp, sizeof(value->heart.hostIp));
			p[0] = value->heart.alarm;
			dataLenField = sizeof(value->heart);
			break;
		case CMD_READ_STATE:       // ��ȡ����״̬
			memcpy(p, &value->state, sizeof(value->state));
			dataLenField = sizeof(value->state);
			break;
		case CMD_VOLUME:           // ��������
			dataLenField = sizeof(value->volume);
			*p = value->volume;
			break;
		case CMD_IDLE_TIME:        // ��������ʱ��(s)
			dataLenField = sizeof(value->idleTime);
			*p = value->idleTime;
			break;
		case CMD_LIGHTENESS:       // ����
			dataLenField = sizeof(value->lighteness);
			*p = value->lighteness;
			break;
		case CMD_MODE:             // ģʽ
			dataLenField = sizeof(value->mode);
			p[0] = value->mode.mode;
			p[1] = value->mode.showHintLed;
			p[2] = value->mode.boardSound;
			p[3] = value->mode.keyMove;
			break;
		case CMD_TAKE_DELAY:       // �����ӳ�ʱ��(ms)
			dataLenField = sizeof(value->takeDelay);
			u16toa(p, value->takeDelay, true);
			break;
		case CMD_WEB_SERVICE:      // ��̨����
			dataLenField = sizeof(value->webService);
			memcpy(p, value->webService.ipAddr, sizeof(value->webService.ipAddr));
			p += sizeof(value->webService.ipAddr);
			u32toa(p, value->webService.port, true);
			p += sizeof(value->webService.port);
			*p = value->webService.linkKind;
			break;
		case CMD_READ_VERSION:     // ��ȡ�汾
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
		case CMD_WRITE_ALL_LED:    // д����LED
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
		case CMD_WRITE_SINGLE_LED: // д����LED
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
		case CMD_READ_HAVE_CHESS:  // ��ȡ��������
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
		case CMD_READ_BW_CHESS:    // ��ȡ�ڰ�����
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
		case CMD_READ_ALL_CHESS:   // ��ȡ��������
		case CMD_SEND_ALL_CHESS:   // ������������
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
		case CMD_HOST_UNDO:        // ����������
			dataLenField = sizeof(value->undoStep);
			u16toa(p, value->undoStep, true);
			break;
		case CMD_BOARD_UNDO:       // ���̻���
			break;
		case CMD_COMPUTER_MOVE:    // ��������
		case CMD_HUMAN_MOVE:       // ������
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
		case CMD_HOST_INDICATE:        // ������ָ��
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
		case CMD_START_CAL:        // ��ʼУ׼
		case CMD_READ_CAL_DATA:    // ��ȡУ׼����
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
		case CMD_PLAY_AUDIO:       // ��������
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
		case CMD_START_RECORD:      // ��ʼ����
			dataLenField = sizeof(value->startRecord);
			*p = value->startRecord.handicap;
			break;
		case CMD_UPLOAD_RECORD_DATA:    // ����������Ϣ
		case CMD_READ_RECORD_DATA:      // ��ȡ������Ϣ
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
		case CMD_READ_RECORD_IDS:       // ��ȡ���ױ��
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
		case CMD_WRITE_RECORD_DATA:     // д����������
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
		case CMD_CLEAR_ALL_RECORD:      // �����������
			if(m_isHost)
			{
				dataLenField = 0;
			}
			break;
#endif
		case CMD_DOWNLOAD_APP:           // ���س���
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
		case CMD_START_PLAY:             // ��ʼ����
			p[2] = value->startPlay.selfColor;
			p[1] = value->startPlay.handicap;
			dataLenField = sizeof(value->startPlay);
			break;
		case CMD_START_REPLAY:           // ��ʼ����
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
		case CMD_START_EXERCISES:        // ��ʼϰ��
			break;
		}
		// ����
		u16toa(pDataLen, dataLenField, true);
		// У����
		uint8_t *pCrc = pDataLen + 2 + dataLenField;
		uint16_t crc = calc_crc16_ccitt(CMD_CRC_INIT_CODE, buf, dataLenField + CMD_CRC_CALC_LEN);
		u16toa(pCrc, crc, true);
		// ���ر��ĳ���
		return dataLenField + CMD_MIN_PACKET_LEN;
	};
	
	// ����
	int decode(void *buf, int bufSize, SPtlValue *value)
	{
		// �����װ�����Ҫ�ѱ���������������
		if(m_recvDataLenField > 0)
		{
			// �ж������Ƿ񳬳���������С
			if(CMD_CRC_CALC_LEN + m_recvDataLen + bufSize > m_recvDataSize)
			{
				m_recvDataLenField = 0;
				return CMD_ERROR_BUF_NOT_ENOUGH;
			}
			// �������ݵ�������
			memcpy(m_recvData + CMD_CRC_CALC_LEN + m_recvDataLen, buf, bufSize);
			m_recvDataLen += bufSize;
		}

		// ���װ�����
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
				// id��000000�ǳ���id
				if(m_checkId)
				{	
					// �Լ��Ƿ��ǳ���id
					bool selfIsSuperId = 0 == memcmp(m_id, "000000", sizeof(m_id));
					// ���緽�Ƿ��ǳ���id
					bool liaisonIsSuperId = 0 == memcmp(p, "000000", sizeof(m_id));
					// id�Ƿ���ͬ
					bool commonIdEqu = 0 == memcmp(m_id, p, sizeof(m_id));
					if(!selfIsSuperId && !liaisonIsSuperId && !commonIdEqu)
					{
						return CMD_ERROR_ID_NOT_MATCH;
					}
				}
				//memcpy(m_id, p, sizeof(m_id));
				p += sizeof(m_id);
				// ���ı��
				value->number = p[0];
				p += sizeof(value->number);
				// ������
				m_recvCmd = p[0];
				// �ж��������Ƿ�Ϸ�
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
				// �ܳ���
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
				// ��������
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

		// ���յ������ݳ��ȣ����ڵ��ڳ����ֶε�ֵ����ʾ���ݽ���������ɣ����Խ�������
		if(m_recvDataLen >= m_recvDataLenField)// && m_recvDataLenField > 0)
		{
			// ���У����
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
			case CMD_VOLUME:           // ��������
				value->volume = *p;
				break;
			case CMD_IDLE_TIME:        // ��������ʱ��(s)
				value->idleTime = *p;
				break;
			case CMD_LIGHTENESS:       // ����
				value->lighteness = *p;
				break;
			case CMD_MODE:             // ģʽ
				value->mode.mode        = p[0];
				value->mode.showHintLed = p[1];
				value->mode.boardSound  = p[2];
				value->mode.keyMove    = p[3];
				break;
			case CMD_TAKE_DELAY:       // �����ӳ�ʱ��(ms)
				value->takeDelay = atou16(p, true);
				break;
			case CMD_WEB_SERVICE:      // ��̨����
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
			case CMD_HOST_UNDO:        // ����
				value->undoStep = atou16(p, true);
				break;
			case CMD_BOARD_UNDO:       // ���̻���
				break;
			case CMD_COMPUTER_MOVE:    // ��������
			case CMD_HUMAN_MOVE:       // ������
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
			case CMD_HOST_INDICATE:        // ������ָ��
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
			case CMD_READ_RECORD_IDS:       // ��ȡ���ױ��
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
			case CMD_WRITE_RECORD_DATA:     // д����������
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
			case CMD_CLEAR_ALL_RECORD:      // �����������
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
			case CMD_START_PLAY:             // ��ʼ����
				value->startPlay.selfColor = p[0];
				value->startPlay.handicap = p[1];
				break;
			case CMD_START_REPLAY:           // ��ʼ����
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
			case CMD_START_EXERCISES:        // ��ʼϰ��
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
