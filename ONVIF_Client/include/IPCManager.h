using namespace std;

#ifndef __IPCMANAGER_H__
#define __IPCMANAGER_H__


class IPCManager
{
public:
    IPCManager();
    virtual ~IPCManager();

public:
    /**
     * @brief 获取IPC设备的RTSP拉流地址
     * 
     * @param ip IPC设备的IP地址
     * @param username IPC设备的用户名
     * @param password IPC设备的密码
     * @return vector<std::string> RTSP拉流地址列表
     */
    vector<std::string>     get_ipc_rtsp_url(const char* ip, const char* username, const char* password);

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
    bool                    ipc_zoom_in(const char* ip, const char* username, const char* password, float speed);

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
    bool                    ipc_zoom_out(const char* ip, const char* username, const char* password, float speed);

    /**
     * @brief PTZ stop, 在停止失败后可多次调用该接口尝试是否能够stop成功
     * 
     * @return true 
     * @return false 
     */
    bool                    ipc_ptz_stop();

private:
    bool initIPC();
    std::unique_ptr<OnvifClient> onvifClient_;
    const char* ip;
    const char* username;
    const char* password;

};


#endif

