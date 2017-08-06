#if !defined(__UTILITY_H__)

#define internal static

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef long long int u64;

internal void die_with_user_msg(char *message, const char *detail);
internal void die_with_sys_msg(const char *message);

#define __UTILITY_H__
#endif // __UTILITY_H__
