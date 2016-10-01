#ifndef _DOWNLOAD_HANDLER_H_
#define _DOWNLOAD_HANDLER_H_

#include "../App42/CoreAPI.h"
#include "network/HttpClient.h"
using namespace cocos2d;
using namespace network;
namespace FXSocialService
{
	enum STATUS_DOWNLOAD
	{
		D_NONE = 0,
		D_DOWNLOADING,
		D_FAIL,
		D_SUCCESS
	};
	class DownloadHandler
	{
	private:
		std::string p_url;
		STATUS_DOWNLOAD p_CurrentStatus;
	public:
		DownloadHandler();
		~DownloadHandler();
		void downLoadObject(std::string url, std::string nameSave);
		void onObjectDownLoaded(HttpClient* pSender, HttpResponse* pResponse);

		STATUS_DOWNLOAD GetCurrentStatus() { return p_CurrentStatus; }
		void ResetStatus() { p_CurrentStatus = D_NONE; }
	};
}

#endif//_DOWNLOAD_HANDLER_H_