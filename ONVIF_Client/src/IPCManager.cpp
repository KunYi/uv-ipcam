#include "IPCManager.h"
#include <thread>
#include <algorithm>
#include <memory>

static bool ipc_init = false;


IPCManager::IPCManager()
{
    onvifClient_ = std::unique_ptr<OnvifClient>(new OnvifClient());
}

IPCManager::~IPCManager()
{
    onvifClient_.reset();
}

/**
 * @brief 初始化IPC设备，判断IPC设备是否具备相关能力
 *
 * @return true
 * @return false
 */
bool IPCManager::initIPC()
{
    if (ipc_init)
    {
        std::cout << "ipc has initiated sunccess." << std::endl;
        return true;
    }
    bool ret = false;
    if (onvifClient_.get() == NULL)
    {
        std::cout << "onvifClient_ is NULL." << std::endl;
        return ret;
    }

    // 1. 搜索终端局域网内的IPC设备
    std::cout << "------------------------------IPC diagnose begin------------------------------" << std::endl;
    std::cout << "1. 搜索终端局域网内的IPC设备" << std::endl;
    std::vector<std::string> ipList = onvifClient_.get()->SearchIpc();
	std::cout << "search ipc, ip size:" << ipList.size() << std::endl;
	for (uint16_t i=0; i<ipList.size(); i++)
	{
		std::cout << "ipc, ip" << i << ":" << ipList[i] << std::endl;
	}

    // 2. 初始化终端外接的IPC设备
    std::cout << "2. 初始化终端外接的IPC设备" << std::endl;
    if (std::find(ipList.begin(), ipList.end(), ip) == ipList.end())
    {
        std::cout << "ipc " << ip << " is not connect to your device, please check it." << std::endl;
        return ret;
    }
	ret = onvifClient_.get()->Init(this->ip, this->username, this->password);
    if (!ret)
    {
        std::cout << "init ipc " << this->ip << " fail." << std::endl;
        return ret;
    }
    std::cout << "init ipc " << this->ip << " success." << std::endl;

    // 3. 获取IPC设备的相关信息
    std::cout << "3. 获取IPC设备的相关信息" << std::endl;
	std::string  Manufacturer;
	std::string  SerialNumber;
	std::string  Model;
	ret = onvifClient_.get()->GetDeviceInfo(Manufacturer, SerialNumber, Model);
    if (!ret)
    {
        std::cout << "ipc " << ip << " get device info fail." << std::endl;
        return ret;
    }
    std::cout << "IPC Manufacturer:" << Manufacturer << ", SerialNumber:" << SerialNumber << ", Model:" << Model << std::endl;

    // 4.获取设备的能力，如果无法获取到Media和PTZ的URI，则说明这个设备无法支持PTZ控制,使用这两个URL可以进行流和PTZ操作
    std::cout << "4. 获取设备的能力，如果无法获取到Media和PTZ的URI，则说明这个设备无法支持PTZ控制,使用这两个URL可以进行流和PTZ操作" << std::endl;
    std::cout << "IPC Capabilities:" << std::endl;
    ret = onvifClient_.get()->Capabilities();
    if (!ret)
    {
        return ret;
    }

    // 5. IPC设备一般包含多个Media Profile Token，分为主码流、子码流、第三码流
    std::cout << "5. IPC设备一般包含多个Media Profile Token，分为主码流、子码流、第三码流" << std::endl;
    std::cout << "IPC Media Profile:" << std::endl;
	ret = onvifClient_.get()->GetMediaProfile();
    if (!ret)
    {
        return ret;
    }

    ipc_init = true;
    std::cout << "------------------------------IPC diagnose end and success------------------------------" << std::endl;
    return ret;
}

/**
 * @brief 获取IPC设备的RTSP拉流地址
 *
 * @param ip IPC设备的IP地址
 * @param username IPC设备的用户名
 * @param password IPC设备的密码
 * @return vector<std::string> RTSP拉流地址列表
 */
vector<std::string> IPCManager::get_ipc_rtsp_url(const char* ip, const char* username, const char* password)
{
    std::vector<std::string> m_rtspUrlList;
    if (onvifClient_.get() == NULL)
    {
        std::cout << "onvifClient_ is NULL." << std::endl;
        return m_rtspUrlList;
    }
    this->ip = ip;
    this->username = username;
    this->password = password;

    bool ret = initIPC();
    if (!ret)
    {
        return m_rtspUrlList;
    }

	m_rtspUrlList = onvifClient_.get()->GetStreamUri();
    if (0 == m_rtspUrlList.size())
    {
        std::cout << "no rtsp url, please check it." << std::endl;
        return m_rtspUrlList;
    }
    return m_rtspUrlList;
}


/**
 * @brief IPC摄像头变焦放大
 *
 * @param ip IPC设备的IP地址
 * @param username IPC设备的用户名
 * @param password IPC设备的密码
 * @param speed 执行变焦的速度
 * @return true 执行成功
 * @return false
 */
bool IPCManager::ipc_zoom_in(const char* ip, const char* username, const char* password, float speed)
{
    std::cout << "ptz zoom in begin." << std::endl;
    bool ret = false;
    if (onvifClient_.get() == NULL)
    {
        std::cout << "onvifClient_ is NULL." << std::endl;
        return ret;
    }
    this->ip = ip;
    this->username = username;
    this->password = password;

    ret = initIPC();
    if (!ret)
    {
        return ret;
    }

    ret = onvifClient_.get()->PtzControl("zoom_in", speed);
    std::cout << "ptz zoom in end." << std::endl;
    if (!ret)
    {
        std::cout << "IPC " << ip << " zoom in fail." << std::endl;
        return ret;
    }
    std::cout << "IPC " << ip << " zoom in success." << std::endl;
    // 云台执行完一次指令需要在停止后才能继续执行下一次指令
    ret = onvifClient_.get()->PtzStop();
    if (!ret)
    {
        std::cout << "IPC " << ip << " stop fail after zoom in." << std::endl;
    }
    return true;
}

/**
 * @brief IPC摄像头变焦缩小
 *
 * @param ip IPC设备的IP地址
 * @param username IPC设备的用户名
 * @param password IPC设备的密码
 * @param speed 执行变焦的速度
 * @return true 执行成功
 * @return false
 */
bool IPCManager::ipc_zoom_out(const char* ip, const char* username, const char* password, float speed)
{
    std::cout << "ptz zoom out begin." << std::endl;
    bool ret = false;
    if (onvifClient_.get() == NULL)
    {
        std::cout << "onvifClient_ is NULL." << std::endl;
        return ret;
    }
    this->ip = ip;
    this->username = username;
    this->password = password;

    ret = initIPC();
    if (!ret)
    {
        return ret;
    }

    ret = onvifClient_.get()->PtzControl("zoom_out", speed);
    std::cout << "ptz zoom out end." << std::endl;
    if (!ret)
    {
        std::cout << "IPC " << ip << " zoom out fail." << std::endl;
        return ret;
    }
    std::cout << "IPC " << ip << " zoom out success." << std::endl;
    // 云台执行完一次指令需要在停止后才能继续执行下一次指令
    ret = onvifClient_.get()->PtzStop();
    if (!ret)
    {
        std::cout << "IPC " << ip << " stop fail after zoom out." << std::endl;
    }
    return true;
}

/**
 * @brief PTZ stop, 在停止失败后可多次调用该接口尝试是否能够stop成功
 *
 * @return true
 * @return false
 */
bool IPCManager::ipc_ptz_stop()
{
    std::cout << "ptz stop begin." << std::endl;
    bool ret = false;
    if (onvifClient_.get() == NULL)
    {
        std::cout << "onvifClient_ is NULL." << std::endl;
        return ret;
    }
    ret = onvifClient_.get()->PtzStop();
    std::cout << "ptz stop end." << std::endl;
    if (!ret)
    {
        std::cout << "IPC " << ip << " stop fail." << std::endl;
        return ret;
    }
    std::cout << "IPC " << ip << " stop success." << std::endl;
    return ret;
}
