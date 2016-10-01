//
//  DisplayView.m
//  JumpingBunny_IOS
//
//  Created by hz on 4/26/15.
//
//
#import "DisplayView_iOS.h"

#import <Foundation/Foundation.h>


const char* ConvertFroWcharToString(const wchar_t* wstr)
{
   
    NSString* str = [[NSString alloc] initWithBytes:wstr length:(wcslen(wstr)*sizeof(wchar_t)) encoding:NSUTF32LittleEndianStringEncoding];
    const char* ret = [str cStringUsingEncoding:NSUTF8StringEncoding];
    return ret;
    
}

const char* CCUnicodeToUtf8(const wchar_t* pwszStr)
{
    return  ConvertFroWcharToString(pwszStr);
}

const wchar_t* CCUtf8ToUnicode(const char * pszUtf8Str, unsigned len/* = -1*/)
{
    NSString * str = [[NSString alloc] initWithBytes:pszUtf8Str length:len encoding:NSUTF8StringEncoding];
    const wchar_t* ret = (const wchar_t *)[str cStringUsingEncoding:NSUTF32StringEncoding];
    return ret;
}