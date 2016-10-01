//
//  DisplayView_IOS.h
//  JumpingBunny_IOS
//
//  Created by qchien on 4/28/15.
//
//

#ifndef JumpingBunny_IOS_DisplayView_IOS_h
#define JumpingBunny_IOS_DisplayView_IOS_h
#include "wchar.h"

const char* CCUnicodeToUtf8(const wchar_t* pwszStr);
const wchar_t* CCUtf8ToUnicode(const char * pszUtf8Str, unsigned len/* = -1*/);

#endif

