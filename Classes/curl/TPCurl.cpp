//
//  TPCurl.cpp
//  WebGame
//
//  Created by 何遵祖 on 13-11-1.
//
//

#include "TPCurl.h"

TPCurl::TPCurl(){
    _state = waiting;
    _url = "";
}

TPCurl::~TPCurl(){
}

TPCurl* TPCurl::connect(const std::string url){
    TPCurl *tCurl = new TPCurl();
    
    /*初始化curl*/
    CURL *curl = curl_easy_init();

    tCurl->setCURL(curl);
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL,url.c_str());
        
        CURLcode res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            /*设置链接地址*/
            tCurl->setURL(url);
            /*添加到释放池中*/
            tCurl->autorelease();
            /*设置state*/
            char *ct;
            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE,&ct);
            /*控制台输出相关信息*/
            if (ct) CCLOG("TPCURL:Content-Type:%s",ct);
            CCLOG("TPCURL:Connect Success URL : %s",url.c_str());
        }else{
            CCLOG("Fail to Connect Server:INFO:%s\n",curl_easy_strerror(res));
        }
    }else{
        /*初始化错误返回空的curl对象*/
        CCLOG("ERROR : fail to Init curl");
        curl_easy_cleanup(curl);
        CC_SAFE_DELETE(tCurl);
        tCurl = NULL;
        return tCurl;
    }
    
    curl_easy_cleanup(curl);
    return tCurl;
}

void TPCurl::send(const std::string msg){
    CURL *curl = curl_easy_init();
    const std::string url = this->getURL();
    curl_easy_setopt(curl, CURLOPT_URL,url.c_str());
    
    const char *my_data = "name=any_data";
    curl_easy_setopt(curl,CURLOPT_POST,1);
    curl_easy_setopt(curl,CURLOPT_POSTFIELDS,my_data);
    
    /*获取post状态*/
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) CCLOG("Error send data : info : %s\n",curl_easy_strerror(res));
    
    curl_easy_cleanup(curl);
}

void TPCurl::receive(){
    CURL *curl = curl_easy_init();
    const std::string url = this->getURL();
    curl_easy_setopt(curl, CURLOPT_URL,url.c_str());
    curl_easy_setopt(curl, CURLOPT_VERBOSE,1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,TPCurl::writtenJsonData);

    /*获取receive状态*/
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) CCLOG("Error reveive data : info : %s\n",curl_easy_strerror(res));

    curl_easy_cleanup(curl);
}

size_t TPCurl::writtenJsonData(void *ptr, size_t size, size_t nmenb, void *userData){
    CCLOG("(char*)receive data:%s",(char*)ptr);
    
    return size * nmenb;
}
