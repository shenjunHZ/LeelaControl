
#include <fcntl.h>
#ifndef WIN_ENV_RUN
#include <termios.h>
#include <unistd.h>
#else
#include <windows.h>
#endif
#include "com/include/ComSerial.hpp"
#include "applications/UserApp.hpp"

namespace
{
    constexpr int OPEN_COM_FAILED = -1;
    constexpr int DATA_BITS_CS7 = 7;
    constexpr int DATA_BITS_CS8 = 8;
    constexpr int DATA_BIT_STOP = 1;
    constexpr int DATA_BITS_STOP = 2;
    constexpr int DATA_RECV_SIZE = 1024;
}

namespace com
{
    ComSerial::ComSerial(const std::string& serial,
        const configurations::comConfigParams& config,
        applications::UserApp& userApp)
        : m_ttyCom{ serial }
        , m_fd{ -1 }
        , m_comConfig{ config }
        , m_userApp{ userApp }
    {

    }
    int ComSerial::openCom()
    {
        if (m_ttyCom.empty())
        {
            std::cout << "error: " << "tty com is empty." << std::endl;
            return -1;
        }
        /*
        O_RDWR   读写模式
        O_NOCTTY 如果路径名指向终端设备，不要把这个设备用作控制终端。
        O_NDELAY 表示不关心DCD信号所处的状态（端口的另一端是否激活或者停止）。
        */
        m_fd = open(m_ttyCom.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
        if (OPEN_COM_FAILED == m_fd)
        {
            std::cout << "error: " << "open tty com failed." << std::endl;
        }
        /*
         获得／设置文件状态标记(cmd=F_GETFL或F_SETFL)
         串口为阻塞状态
        */
        const int result = fcntl(m_fd, F_SETFL, 0);
        if (result < 0)
        {
            std::cout << "error: " << "fcntl set F_SETFL failed." << std::endl;
        }
        /*
        若为终端设备返回1，否则返回0
        */
        if (isatty(STDIN_FILENO) == 0)
        {
            std::cout << "error: " << "standard input is not a terminal device." << std::endl;
        }

        if (initCom() < 0)
        {
            return -1;
        }
        return m_fd;
    }

    int ComSerial::initCom()
    {
        struct termios param;
        /*
        tcgetattr可以初始化一个终端对应的termios结构
        如果这些值其后被修改了，可以通过调用函数tcsetattr来重新配置
        */
        int result = tcgetattr(m_fd, &param);
        if (result != 0)
        {
            std::cout << "error: " << "tcgetattr failed: " << result << std::endl;
            return -1;
        }
        memset(&param, 0, sizeof(param));
        /*
        c_iflag：输入模式
        c_oflag：输出模式
        c_cflag：控制模式
        c_lflag：本地模式
        c_cc[NCCS]：特殊控制模式
        */
        /*
        设置字符大小 c_cflag
        CLOCAL含义为忽略所有调制解调器的状态行，这个目的是为了保证程序不会占用串口
        CREAD代表启用字符接收器，目的是能够从串口中读取输入的数据。
        CS5/6/7/8表示发送或接收字符时使用5/6/7/8比特
        CSTOPB表示每个字符使用两位停止位
        HUPCL表示关闭时挂断调制解调器
        PARENB：启用奇偶校验码的生成和检测功能
        PARODD：只使用奇校验而不使用偶校验
        */
        param.c_cflag |= CLOCAL | CREAD;
        param.c_cflag &= ~CSIZE;
        /*设置停止位*/
        switch (m_comConfig.dataBits)
        {
        case DATA_BITS_CS7:
            param.c_cflag |= CS7;
            break;
        case DATA_BITS_CS8:
            param.c_cflag |= CS8;
            break;
        default:
            std::cout << "error: " << "databits set failed: " << m_comConfig.dataBits << std::endl;
            return -1;
        }

        /*
        设置奇偶校验位 c_iflag
        BRKINT：当在输入行中检测到一个终止状态时，产生一个中断。
        TGNBRK：忽略输入行中的终止状态。
        TCRNL：将接受到的回车符转换为新行符。
        TGNCR：忽略接受到的新行符。
        INLCR：将接受到的新行符转换为回车符。
        IGNPAR：忽略奇偶校检错误的字符。
        INPCK：对接收到的字符执行奇偶校检。
        PARMRK：对奇偶校检错误作出标记。
        ISTRIP：将所有接收的字符裁减为7比特。
        IXOFF：对输入启用软件流控。
        IXON：对输出启用软件流控。
        */
        if ("ODD" == m_comConfig.dataPaity) //奇数 
        {
            param.c_cflag |= PARENB;
            param.c_cflag |= PARODD;
            param.c_iflag |= (INPCK | ISTRIP);
        }
        else if ("EVEN" == m_comConfig.dataPaity) //偶数 
        {
            param.c_iflag |= (INPCK | ISTRIP);
            param.c_cflag |= PARENB;
            param.c_cflag &= ~PARODD;
        }
        else if ("NONE" == m_comConfig.dataPaity)  //无奇偶校验位 
        {
            param.c_cflag &= ~PARENB;
        }
        else
        {
            std::cout << "error: " << "dataPaity set failed: " << m_comConfig.dataPaity.c_str() << std::endl;
            return -1;
        }
        /*设置波特率*/
        switch (m_comConfig.baudRate)
        {
        case 2400:
            cfsetispeed(&param, B2400);
            cfsetospeed(&param, B2400);
            break;
        case 4800:
            cfsetispeed(&param, B4800);
            cfsetospeed(&param, B4800);
            break;
        case 9600:
            cfsetispeed(&param, B9600);
            cfsetospeed(&param, B9600);
            break;
        case 115200:
            cfsetispeed(&param, B115200);
            cfsetospeed(&param, B115200);
            break;
        case 460800:
            cfsetispeed(&param, B460800);
            cfsetospeed(&param, B460800);
            break;
        default:
            cfsetispeed(&param, B9600);
            cfsetospeed(&param, B9600);
            std::cout << "waring: " << "baudRate set default value 9600." << std::endl;
            break;
        }
        /*设置停止位*/
        if (m_comConfig.stopBits == DATA_BIT_STOP)
        {
            param.c_cflag &= ~CSTOPB;
        }
        else if (m_comConfig.stopBits == DATA_BITS_STOP)
        {
            param.c_cflag |= CSTOPB;
        }
        /*
        设置等待时间和最小接收字符 c_cc
        标准模式：
        VEOF:EOF字符
        VEOL:EOF字符
        VERASE:ERASE字符
        VINTR:INTR字符
        VKILL:KILL字符
        VQUIT:QUIT字符
        VSTART:START字符 
        VSTOP:STOP字符

        非标准模式:
        VINTR:INTR字符
        VMIN:MIN值
        VQUIT:QUIT字符
        VSUSP:SUSP字符
        VTIME:TIME值
        VSTART:START字符 
        VSTOP:STOP字符
        */
        param.c_cc[VTIME] = 0;
        param.c_cc[VMIN] = 0;
        /*处理未接收字符*/
        tcflush(m_fd, TCIFLUSH);
        /*
        激活新配置
        参数actions控制修改方式，共有三种修改方式：
        TCSANOW：立刻对值进行修改
        TCSADRAIN：等当前的输出完成后再对值进行修改
        TCSAFLUSH：等当前的输出完成之后，再对值进行修改，但丢弃还未从read调用返回的当前的可用的任何输入
        */
        if ((tcsetattr(m_fd, TCSANOW, &param)) != 0)
        {
            std::cout << "error: " << "com set error" << std::endl;
            return -1;
        }

        return 0;
    }

    int ComSerial::closeCom()
    {
        int result = close(m_fd);
        if (result < 0)
        {
            std::cout << "error: " << "close com failed: " << result << std::endl;
        }
        return result;
    }

    int ComSerial::writeCom(const std::string& data)
    {
        return write(m_fd, data.c_str(), data.size());
    }

    int ComSerial::readCom(std::string& data)
    {
        std::vector<char> recvData;
        recvData.resize(DATA_RECV_SIZE);
        int readSize = read(m_fd, &recvData[0], DATA_RECV_SIZE);
        recvData.resize(readSize);

        data.assign(recvData.begin(), recvData.end());
        return readSize;
    }
} // namespace com