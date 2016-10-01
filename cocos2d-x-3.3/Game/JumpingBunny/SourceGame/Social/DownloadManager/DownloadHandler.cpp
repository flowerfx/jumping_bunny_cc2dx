#include "DownloadHandler.h"
#include "Utility/FileManager.h"
namespace FXSocialService
{
	extern std::mutex mtx;
	DownloadHandler::DownloadHandler()
	{
		p_url = "";
		p_CurrentStatus = D_NONE;
	}
	DownloadHandler::~DownloadHandler()
	{

	}
	void DownloadHandler::downLoadObject(std::string url, std::string nameSave)
	{
		HttpRequest* request = new HttpRequest();
		request->setUrl(url.c_str());
		request->setRequestType(HttpRequest::Type::GET);
		request->setResponseCallback(CC_CALLBACK_2(DownloadHandler::onObjectDownLoaded,this));
		request->setTag(nameSave.c_str());
		HttpClient::getInstance()->send(request);
		p_CurrentStatus = D_DOWNLOADING;
		request->release();
	}
	void DownloadHandler::onObjectDownLoaded(HttpClient* pSender, HttpResponse* pResponse)
	{
		mtx.lock();
		HttpResponse* response = pResponse;

		if (!response)
		{
			p_CurrentStatus = D_FAIL;
			cocos2d::log("No Response");
			return;
		}
		int statusCode = (int)response->getResponseCode();

		char statusString[64] = {};
		sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
		cocos2d::log("response code: %d", statusCode);

		if (!response->isSucceed())
		{
			p_CurrentStatus = D_FAIL;
			cocos2d::log("response failed");
			cocos2d::log("error buffer: %s", response->getErrorBuffer());
			return;
		}
		std::vector<char>*buffer = response->getResponseData();

		CCImage * img = new CCImage();
		img->initWithImageData((unsigned char *)&(buffer->front()), buffer->size());

		// Save object file to device.
		std::string writablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
		writablePath.append(response->getHttpRequest()->getTag());
		img->saveToFile(writablePath.c_str());
		//
		p_CurrentStatus = D_SUCCESS;
		mtx.unlock();
	}
}