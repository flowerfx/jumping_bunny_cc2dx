#include "CoreAPI.h"
namespace FXSocialService
{
	void InitService()
	{
		App42API::Initialize(API_KEY, SECRET_KEY);
	}
}