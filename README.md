> C语言是一门非常棒的编程语言，也是我最喜欢的编程语言，在使用它的时候，总是感觉到编程的乐趣，感叹前人设计的精妙。所以有了这个仓库，记录使用c的时候的一些经历。

### tlv解析

感谢来自开源中国的[Tlv-协议的C语言实现](http://www.oschina.net/code/snippet_2372099_48227)

作者[Listening2009](http://my.oschina.net/u/2372099)封装的tlv库我正好使用上了，所以把此项目附上。见`tlv`目录

### simple hash

看[一篇介绍php的博文中的简单hash样例](http://www.php-internals.com/book/?p=chapt03/03-01-01-hashtable),原作者的代码也在[github](https://github.com/reeze/tipi/tree/master/book/sample/chapt03/03-01-01-hashtable)上，我这就拿来一份作为自己的工具。见`simple-hash`目录

### linux process communication

linux进程间通信，可以看看个人博客中的读书笔记。

+ 消息队列，见`linux_process_communication/msgtest.c`文件。在该文件中只是使用了简单地使用了消息队列进行读写，并未涉及进程间的通信:)，以后有时间补上。
+ 共享内存，见`linux_process_communication/shmtest.c`文件。


### 平台检测

程序编译的时候如果有平台限制的话，会使用宏来做一个依赖检测.

```cpp
#ifdef OS_LINUX
#include <sys/epoll.h>
#elif defined(OS_MACOSX)
#include <sys/event.h>
#else
#error "platform unsupported"
#endif
```

### 多线程优雅退出

```c
// In Child Thread
while (!stop){
    //do something
}
// In parent Thread
stop = true; //then child thread will go die
```

### dump uint\_8数组

记得这是使用c中超级常用的一个场景了，比如构造了一个报文，然后想查看下报文内容，报文包一般都是uint\_8类型的字符数组，建立dump函数查看整个包就很常用了。

我在云风的pbc项目中看到dump函数，适合很多地方使用:

```c
static void
dump(uint8_t *buffer, int sz) {
	int i , j;
	for (i=0;i<sz;i++) {
		printf("%02X ",buffer[i]);
		if (i % 16 == 15) {
			for (j = 0 ;j <16 ;j++) {
				char c = buffer[i/16 * 16+j];
				if (c>=32 && c<127) {
					printf("%c",c);
				} else {
					printf(".");
				}
			}
			printf("\n");
		}
	}

	printf("\n");
}
```

结果:

```
0A 06 41 6C 69 63 65 00 10 B9 60 1A 0D 61 6C 69 ..Alice...`..ali
63 65 40 75 6E 6B 6F 77 6E 00 50 B1 A8 03 2A 0A ce@unkown.P...*.
85 FF FF FF 0F B9 60 87 AD 4B 22 0B 0A 09 38 37 ......`..K"...87
36 35 34 33 32 31 00 22 10 0A 0C 31 33 39 30 31 654321."...13901
32 33 34 35 36 37 00 10 00
```

### epoll

最初接触网络编程就是使用epoll。在旅住到[handy](https://github.com/yedf/handy)项目时，看到了一个epoll的好例子，是c++写的，便摘抄下来。

详细见`epoll`目录中代码及文档。在epoll目录中可以看到**socket服务端**的初始化过程。

### reverse list

<img src="img/cloudwu_reverselist.jpg" width="85%">

翻到这个[微博](http://weibo.com/2388714105/DjvJZC1Qc?from=page_1005052388714105_profile&wvr=6&mod=weibotime&type=comment)，花了10分钟左右写了翻转链表的代码。

见`reverse_list`目录

### 大数相乘

博客`http://www.jianshu.com/p/b5af56d676b2`，博主介绍了使用分治实现大数相乘，值得学习。见`big_number_multiply`目录。

### 快速排序

鉴于太基础了，所以加上。见`quick_sort`目录，有相应文档。

### KMP算法

在一个子串中查找另一个子串的第一个位置，暴力算法的时间为N^2，时间有点长。使用KMP算法时间复杂度为`Ｏ(n+m)`。见`kmp`目录。

