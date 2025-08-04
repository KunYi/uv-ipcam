//============================================================================
// Name        : OnvifDemo.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include "IPCManager.h"
using namespace std;

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavdevice/avdevice.h"
#include "libavformat/avformat.h"
#include "libavfilter/avfilter.h"
#include "libavutil/avutil.h"
#include "libswscale/swscale.h"
#include "libavutil/pixdesc.h"
#include "libavutil/log.h"
}

const char* ip = "192.168.1.108";
const char* username = "admin";
const char* password = "pl123456";

bool is_sps_pps_data(uint8_t *data) {
    switch (data[0] & 0x1f) {
        case 7: // SPS
        case 8: // PPS
        case 5:
            return true;
        case 1:
            return false;
        default:
            return false;
    }
}

bool is_key_frame(uint8_t *data, int size) {
    int last = 0;
    for (int i = 2; i <= size; ++i) {
        if (i == size) {
            if (last) {
                if (is_sps_pps_data(data + last)) {
                    return true;
                }
            }
        } else if (0x00 == data[i - 2] && 0x00 == data[i - 1] && 0x01 == data[i]) {
            if (last) {
                size = i - last - 3;
                if (data[i - 3]) ++size;
                if (is_sps_pps_data(data + last)) {
                    return true;
                }
            }
            last = i + 1;
        }
    }
    return false;
}

void get_ipc_rtsp_stream(const char *rtsp_url)
{
	printf("*****************get ipc rtsp stream***********************\n");
    unsigned int    i;
    int             ret;
    int             video_st_index = -1;
    int             audio_st_index = -1;
    AVFormatContext *ifmt_ctx = NULL;
    AVDictionary *options = NULL;
    AVPacket        pkt;
    AVStream        *st = NULL;
    char            errbuf[64];
    bool            is_keyframe = false;

    //av_log_set_level(AV_LOG_DEBUG);

    avformat_network_init();                                                    // Initialization of network components
    av_dict_set(&options, "buffer_size", "4096000", 0); //设置缓存大小
    av_dict_set(&options, "reorder_queue_size", "2000", 0);
    av_dict_set(&options, "rtsp_transport", "tcp", 0);  //以tcp的方式打开
    av_dict_set(&options, "stimeout", "5000000", 0);    //设置超时断开链接时间，单位us,   5s
    av_dict_set(&options, "max_delay", "500000", 0);    //设置最大时延
    av_dict_set(&options, "mux_delay", "1", 0);  //强制使用tcp，udp在1080p下会丢包导致花屏
    ifmt_ctx = avformat_alloc_context(); //用来申请AVFormatContext类型变量并初始化默认参数,申请的空间

    if ((ret = avformat_open_input(&ifmt_ctx, rtsp_url, NULL, &options)) < 0) {            // Open the input file for reading.
        printf("Could not open input file '%s' (error '%s')\n", rtsp_url, av_make_error_string(errbuf, sizeof(errbuf), ret));
        goto EXIT;
    }

    // 此处的参数不应该设置&options，否则会出现Segmentation fault，原因不详
    if ((ret = avformat_find_stream_info(ifmt_ctx, NULL)) < 0) {                // Get information on the input file (number of streams etc.).
        printf("Could not open find stream info (error '%s')\n", av_make_error_string(errbuf, sizeof(errbuf), ret));
        goto EXIT;
    }

    av_dict_free(&options);

    for (i = 0; i < ifmt_ctx->nb_streams; i++) {                                // dump information
        av_dump_format(ifmt_ctx, i, rtsp_url, 0);
    }

    for (i = 0; i < ifmt_ctx->nb_streams; i++) {                                // find video stream index
        st = ifmt_ctx->streams[i];
        switch(st->codecpar->codec_type) {
        case AVMEDIA_TYPE_AUDIO: audio_st_index = i; break;
        case AVMEDIA_TYPE_VIDEO: video_st_index = i; break;
        default: break;
        }
    }
    if (-1 == video_st_index) {
        printf("No H.264 video stream in the input file\n");
        goto EXIT;
    }

    av_init_packet(&pkt);                                                       // initialize packet.
    pkt.data = NULL;
    pkt.size = 0;

    while (1)
    {
        do {
            ret = av_read_frame(ifmt_ctx, &pkt);                                // read frames
        } while (ret == AVERROR(EAGAIN));

        if (ret < 0) {
            printf("Could not read frame (error '%s')\n", av_make_error_string(errbuf, sizeof(errbuf), ret));
            break;
        }

        if (pkt.stream_index == video_st_index) {                               // video frame
            printf("Video Packet size = %d\n", pkt.size);
            is_keyframe = is_key_frame(pkt.data, pkt.size);
            printf("is_keyframe=%d\n", is_keyframe);
        } else if(pkt.stream_index == audio_st_index) {                         // audio frame
            printf("Audio Packet size = %d\n", pkt.size);
        } else {
            printf("Unknow Packet size = %d\n", pkt.size);
        }

        av_packet_unref(&pkt);
    }

EXIT:

    if (NULL != ifmt_ctx) {
        avformat_close_input(&ifmt_ctx);
        ifmt_ctx = NULL;
    }
}

int main()
{
	IPCManager mIPCManager;
	std::vector<std::string> m_rtspUrlList = mIPCManager.get_ipc_rtsp_url(ip, username, password);
	std::cout << "m_rtspUrlList size:" << m_rtspUrlList.size() << std::endl;

	mIPCManager.ipc_zoom_in(ip, username, password, 5);

	mIPCManager.ipc_zoom_out(ip, username, password, 5);

	// 这里获取主码流的RTSP数据
	if (m_rtspUrlList.size() > 0)
	{
		get_ipc_rtsp_stream(m_rtspUrlList[0].c_str());
	}

	return 0;
}

