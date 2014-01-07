//
//  TPCurl.h
//  WebGame
//
//  Created by 何遵祖 on 13-11-1.
//
//

#ifndef __WebGame__TPCurl__
#define __WebGame__TPCurl__

#include "cocos2d.h"
#include <curl/curl.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#include "Sealed.h"

using namespace cocos2d;

enum ConnectState{
    waiting = 0,//等待链接状态
    connecting = 1,//正在链接状态
    disConnected = 2//网络断开状态
};

class TPCurl : Class_Is_Sealed(TPCurl) , public CCObject{
public:
    TPCurl();
    ~TPCurl();
    
    /*产生链接,参数是链接地址*/
    static TPCurl* connect(const std::string url);
    /*发送信息*/
    void send(const std::string msg);
    /*接收消息*/
    void receive();
    static size_t writtenJsonData(void *ptr,size_t size,size_t nmenb,void *userData);
    /*是否为连接状态*/
    inline bool stateIsConnecting(){
        /*没有连接*/
        if(this->getConnectState() != connecting) {return false;}
        /*已经连接*/
        else {return true;}
    };
private:
    CC_SYNTHESIZE(CURL*, _curl, CURL);
    CC_SYNTHESIZE(std::string, _url, URL);
    CC_SYNTHESIZE(ConnectState, _state, ConnectState);
};

#endif /* defined(__WebGame__TPCurl__) */
