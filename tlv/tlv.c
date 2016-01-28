int tlv_encode(tlv_t *tlv, char *buffer)
{
    int tt = (-1);
    tlv_t *cur = tlv;
    int pos = 0;
    unsigned short l = 0;
 
    while(cur){
        tt = _tt_T(cur->t);
        switch(tt){
        case eint32:
            if(buffer){
                __memcpy(buffer+pos+0, &cur->t, 2);
                l = 4; __memcpy(buffer+pos+2, &l, 2);
                __memcpy(buffer+pos+4, &cur->v.int32Value, 4);
            }
            pos += (2+2+4);
            break;
        case eint16:
            if(buffer){
                __memcpy(buffer+pos+0, &cur->t, 2);
                l = 2;__memcpy(buffer+pos+2, &l, 2);
                __memcpy(buffer+pos+4, &cur->v.int16Value, 4);
            }      
            pos += (2+2+2);
            break;
        case eint8:
            if(buffer){
                __memcpy(buffer+pos+0, &cur->t, 2);
                l = 1;__memcpy(buffer+pos+2, &l, 2);
                __memcpy(buffer+pos+4, &cur->v.int8Value, 1);
            }       
            pos += (2+2+1);
            break;
        case estr:
            l = (unsigned short)(cur->v.strValue?(__strlen(cur->v.strValue)+1):0);
            if(buffer){
                __memcpy(buffer+pos+0, &cur->t, 2);
                __memcpy(buffer+pos+2, &l, 2);
                __memcpy(buffer+pos+4, cur->v.strValue, l);
            }
            pos += (2+2+l);
            break;
        case ebytes:
            if(buffer){
                __memcpy(buffer+pos+0, &cur->t, 2);
                __memcpy(buffer+pos+2, &cur->v.bytesValue.l, 2);
                __memcpy(buffer+pos+4, cur->v.bytesValue.b, cur->v.bytesValue.l);
            }
            pos += (2+2+cur->v.bytesValue.l);
            break;
        case etlv:
            l=tlv_encode(cur->v.tlv, buffer?(buffer+pos+4):NULL);
            if(buffer){
                __memcpy(buffer+pos+0, &cur->t, 2);                
                __memcpy(buffer+pos+2, &l, 2);
            }
            pos += (2+2+l);
            break;
        default:            
            break;
        }
        cur = cur->_next;
    }
    return pos;
}
 
static int _hung(tlv_t **head, unsigned short t, unsigned short l, char *buffer)
{
    tlv_t *cur=NULL;
     
    cur = tlv_create(t,l,buffer);
    if(!cur){
        return 0;
    }
    tlv_add(head, cur);
    return l;
}
 
 
int tlv_decode(tlv_t **head, char *buffer, int len)
{
    int tt = (-1);
    int pos=0;
    unsigned short t=0,l=0;
    int stop = 0;
     
    while(!stop&&len-pos>=4){
        __memcpy(&t, buffer+pos+0, 2);
        __memcpy(&l, buffer+pos+2, 2);
        if(len-pos<l){
            break;
        }
        tt = _tt_T(t);
        switch(tt){
        case eint32:
        case eint16:
        case eint8:
        case estr:
        case ebytes:
        case etlv:
            if(l!=_hung(head,t,l,buffer+pos+4)){
                stop=1;
            }
            break;
        default:
            break;
        }
        pos+=2+2+l;
    }
 
    return pos;
}
 
tlv_t* tlv_create(unsigned short t, unsigned short l, char *v)
{
    tlv_t *cur = NULL;
     
    cur = (tlv_t*)__malloc(sizeof(tlv_t));
    if(!cur) {
        return NULL;
    }
    __memset(cur, 0x00, sizeof(tlv_t));
     
    cur->t = t;
    if(tlv_update(cur,l,v)){
        __free(cur);
        return NULL;
    }
 
    return cur;
}
 
void tlv_destroy(tlv_t *head)
{
    tlv_t *tlv = head;
    tlv_t *temp = NULL;
         
    while(tlv){
        int tt = _tt_T(tlv->t);
        temp = tlv->_next;
        if(etlv==tt){
            if(tlv->v.tlv){
                tlv_destroy(tlv->v.tlv);
                tlv->v.tlv=NULL;
            }
        }
        else if(estr==tt){
            if(tlv->v.strValue){
                __free(tlv->v.strValue);
                tlv->v.strValue = NULL;
            }
        }
        else if(ebytes==tt){
            if(tlv->v.bytesValue.b){
                __free(tlv->v.bytesValue.b);
                tlv->v.bytesValue.b = NULL;
            }
        }        
        __free(tlv);
        tlv = temp;
    }    
}
 
int tlv_gets(tlv_t *owner, unsigned short t, tlv_t* arr[], int n)
{
    tlv_t *cur = owner;
    int i = 0;
    int tt = 0;
     
    while(cur&&i<n){
        tt=_tt_T(cur->t);
        if(cur->t==t){
            arr[i++] = cur;
        }
        if(etlv==tt){
            i+=tlv_gets(cur->v.tlv,t,arr+i,n-i);
        }
        cur = cur->_next;
    }
    return i;
}
 
void tlv_add(tlv_t **phead, tlv_t *tlv)
{
    tlv_t *tail = NULL;
 
    if(!(*phead)){
        *phead=tlv;
    }
    else{
        tail=(*phead);
        while(tail->_next){
            tail=tail->_next;
        }
        tail->_next=tlv;
    }
}
 
void tlv_rem(tlv_t **phead, tlv_t *cur)
{
    tlv_t *prev = NULL;
 
    if((*phead)==cur){
        *phead=cur->_next;
    }
    else{
        prev = *phead;
        while(prev&&prev->_next!=cur){
            prev = prev->_next;
        }
        if(prev){
            prev->_next=cur->_next;
        }
    }
    cur->_next=NULL;
}
 
int tlv_update(tlv_t *cur, unsigned short l, char *v)
{
    int tt = _tt_T(cur->t);
    int err = 0;
         
    switch(tt){
    case eint32:
        __memcpy(&cur->v.int32Value, v, 4);
        break;
    case eint16:
        __memcpy(&cur->v.int16Value, v, 2);
        break;
    case eint8:
        __memcpy(&cur->v.int8Value, v, 1);
        break;
    case estr:
        if(cur->v.strValue){
            __free(cur->v.strValue);
            cur->v.strValue=NULL;
        }
        if(l>0){
            cur->v.strValue = __malloc(l);
            if(!cur->v.strValue){
                err=1;
                break;
            }            
            __memset(cur->v.strValue,0x00,l);
            __memcpy(cur->v.strValue,v,l);
        }
        break;
    case ebytes:
        if(cur->v.bytesValue.b){
            __free(cur->v.bytesValue.b);
            cur->v.bytesValue.b=NULL;
        }
        cur->v.bytesValue.l = l;
        if(l>0){
            cur->v.bytesValue.b = __malloc(l);
            if(!cur->v.bytesValue.b){
                err=1;
                break;
            }            
            __memset(cur->v.bytesValue.b,0x00,l);
            __memcpy(cur->v.bytesValue.b,v,l);
        }
        break;
    case etlv:
        if (cur->v.tlv){
            tlv_destroy(cur->v.tlv);
            cur->v.tlv = NULL;
        }
        if(l!=tlv_decode(&cur->v.tlv, v,l)){
            err=1;
        }
        break;
    default:
        err=1;
        break;
    }
 
    return err;
}
 
 
#ifdef _TDEBUG
void tlv_print(int level, tlv_t *tlv)
{
    tlv_t *cur = tlv;
     
    while(cur){
        int i = level;
        while(i-->0){
            __printf("\t");
        }
        __printf("(%d,%d)\t", _id_T(cur->t), _tt_T(cur->t));
        switch(_tt_T(cur->t)){
        case eint32:
            __printf("%d\t%d\r\n", 4, cur->v.int32Value);
            break;
        case eint16:
            __printf("%d\t%d\r\n", 2, cur->v.int16Value);
            break;
        case eint8:
            __printf("%d\t%d\r\n", 1, cur->v.int8Value);
            break;
        case estr:
            if(cur->v.strValue){
                __printf("%d\t%s\r\n", __strlen(cur->v.strValue)+1, cur->v.strValue);
            }else{
                __printf("%d\t\r\n", 0);
            }
            break;
        case ebytes:
            __printf("%d\t", cur->v.bytesValue.l);
            for(i=0;i<cur->v.bytesValue.l;i++){
                __printf("\\%x", cur->v.bytesValue.b[i]);
            }
            __printf("\r\n");
            break;
        case etlv:
            __printf("%d\t\r\n", tlv_encode(cur->v.tlv, NULL));
            tlv_print(level+1,cur->v.tlv);
            break;
        default:
            break;
        }
        cur = cur->_next;
    }
}
#endif
