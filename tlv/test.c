#include "tlv.h"
 
 
#define CATARRAY  _T(1,etlv)
#define CAT       _T(2,etlv)
#define CAT_NAME  _T(3,estr)
#define CAT_AGE   _T(4,eint32)
#define CAT_COLOR _T(5,eint8)
 
#define _N (32)
 
int main(int argc, const char * argv[])
{
    tlv_t *cats = NULL;
    tlv_t *_out = NULL;
    tlv_t *tmp = NULL;
    int len = 0;
    char *buffer = NULL;
    tlv_t *___arr[_N] = {0};
    int n = 0;
    int i = 0;
         
    printf("BUILD:\r\n");
    cats = tlv_create(CATARRAY, 0, NULL);    
    tmp = tlv_create(CAT, 0, NULL);
    tlv_add(&tmp->v.tlv, tlv_createstr(CAT_NAME, "BIngGO1"));
    tlv_add(&tmp->v.tlv, tlv_createint32(CAT_AGE, 8));
    tlv_add(&tmp->v.tlv, tlv_createint8(CAT_COLOR, 1));
    tlv_add(&cats->v.tlv, tmp);
     
    tmp = tlv_create(CAT, 0, NULL);
    tlv_add(&tmp->v.tlv, tlv_createstr(CAT_NAME, "BIngGO2"));
    tlv_add(&tmp->v.tlv, tlv_createint32(CAT_AGE, 5));
    tlv_add(&tmp->v.tlv, tlv_createint8(CAT_COLOR, 3));
    tlv_add(&cats->v.tlv, tmp);
 
    #ifdef _TDEBUG
    tlv_print(0, cats);
    #endif
 
    printf("ENCODE:\r\n");
    len = tlv_encode(cats, NULL);
    buffer = (char*)malloc(len);
    memset(buffer, 0x00, len);
    len = tlv_encode(cats, buffer);
     
    while(i<len){
        printf("\\%x", buffer[i]);
        i++;
    }
    printf("\r\n");
    #ifdef _TDEBUG
    tlv_printbuffer(buffer, len);
    #endif
     
    printf("DECODE:\r\n");
    tlv_decode(&_out, buffer, len);
    #ifdef _TDEBUG
    tlv_print(0, _out);
    #endif
    tlv_destroy(_out);
    free(buffer);
 
    printf("GET:\r\n");
    n = tlv_gets(cats,CAT,___arr,_N);
    for(i=0;i<n;i++){
        tlv_t *cur = ___arr[i]->v.tlv;
        printf("%d {", i);
        while(cur){
            if(cur->t==CAT_NAME){
                printf("NAME=%s;", cur->v.strValue?cur->v.strValue:"");
            }else if(cur->t==CAT_AGE){
                printf("AGE=%d;", cur->v.int32Value);
            }else if(cur->t==CAT_COLOR){
                printf("COLOR=%d;", cur->v.int8Value);
            }
            cur = cur->_next;
        }
        printf("}\r\n");
    }
 
    printf("UPDATE:\r\n");
    n = tlv_gets(cats,CAT_NAME,___arr,_N);
    if(___arr[1]){
        tlv_updatestr(___arr[1], "MyNewName");
    }
 
    #ifdef _TDEBUG
    tlv_print(0, cats);
    #endif
         
    tlv_destroy(cats);
     
    return 0;
}
