#ifndef TLV_H
#define TLV_H
 
#include "stdlib.h"
#include "memory.h"
#include "string.h"
#include "stdio.h"
 
#define _TDEBUG
 
#define __malloc malloc
#define __free   free
#define __memset memset
#define __memcpy memcpy
#ifdef _TDEBUG
#define __printf printf
#endif
#define __strlen strlen
 
 
#define eint32 (1)
#define eint16 (2)
#define eint8  (3)
#define ebytes (4)
#define estr   (5)
#define etlv   (6)
 
 
#define _T(c,t)  ((unsigned short)(((unsigned char)(c))|((unsigned short)((unsigned char)(t)))<<8))
#define _tt_T(q) ((unsigned char)(((unsigned short)(q)>>8)&0xFF))
#define _id_T(q) ((unsigned char)(q))
 
 
typedef struct _tlv_s{
    struct _tlv_s *_next;
    unsigned short t;
    union _uv_u{
        int  int32Value;
        short int16Value;
        char int8Value;
        char *strValue;
        struct {
            unsigned short l;
            unsigned char *b;
        }bytesValue;
        struct _tlv_s *tlv;
    }v;
}tlv_t;
 
 
int tlv_encode(tlv_t *tlv, char *buffer);
int tlv_decode(tlv_t **head, char *buffer, int len);
tlv_t* tlv_create(unsigned short t, unsigned short l, char *uv);
void tlv_destroy(tlv_t *tlv);
int tlv_gets(tlv_t *head, unsigned short t, tlv_t* arr[], int n);
void tlv_add(tlv_t **phead, tlv_t *tlv);
void tlv_rem(tlv_t **phead, tlv_t *cur);
int tlv_update(tlv_t *tlv, unsigned short l, char *v);
 
#ifdef _TDEBUG
void tlv_print(int level, tlv_t *cur);
#endif
 
static __inline
tlv_t* tlv_get(tlv_t *head, unsigned short t)
{
    tlv_t *__arr[1] = {0};
    tlv_gets(head, t, __arr, 1);
    return __arr[0];
}
 
static __inline
tlv_t* tlv_createstr(unsigned short t, char *str)
{
    return tlv_create(t, (unsigned short)(str?(__strlen(str)+1):0), str);
}
 
static __inline
tlv_t* tlv_createint32(unsigned short t, int int32Value)
{
    return tlv_create(t,4,(char*)&int32Value);
}
 
static __inline
tlv_t* tlv_createint16(unsigned short t, unsigned short int16Value)
{
    return tlv_create(t,2,(char*)&int16Value);
}
 
static __inline
tlv_t* tlv_createint8(unsigned short t, unsigned char int8Value)
{
    return tlv_create(t,1,(char*)&int8Value);
}
 
static __inline
int tlv_updatestr(tlv_t *tlv, char *str)
{
    return tlv_update(tlv, (unsigned short)(str?(__strlen(str)+1):0), str);
}
 
static __inline
int tlv_updateint32(tlv_t *tlv, int int32Value)
{
    return tlv_update(tlv, 4, (char*)&int32Value);
}
 
static __inline
int tlv_updateint16(tlv_t *tlv, unsigned short int16Value)
{
    return tlv_update(tlv, 2, (char*)&int16Value);
}
 
static __inline
int tlv_updateint8(tlv_t *tlv, unsigned char int8Value)
{
    return tlv_update(tlv, 1, (char*)&int8Value);
}
 
#ifdef _TDEBUG
static __inline
void tlv_printbuffer(char *buffer, int len)
{
    tlv_t *head=NULL;
    tlv_decode(&head,buffer,len);
    tlv_print(0, head);
    tlv_destroy(head);
}
#endif
 
 
#endif
