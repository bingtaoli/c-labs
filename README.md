一些工具或是奇技淫巧

### tlv

感谢来自开源中国的[Tlv-协议的C语言实现](http://www.oschina.net/code/snippet_2372099_48227)

作者[Listening2009](http://my.oschina.net/u/2372099)封装的tlv库我正好使用上了，所以把此项目附上。

见`tlv`目录

### simple hash

看[一篇介绍php的博文中的简单hash样例](http://www.php-internals.com/book/?p=chapt03/03-01-01-hashtable),原作者的代码也在[github](https://github.com/reeze/tipi/tree/master/book/sample/chapt03/03-01-01-hashtable)上，我这就拿来一份作为自己的工具.

见`simple-hash`目录

### reverse list

<img src="img/cloudwu_reverselist.jpg" width="85%">

翻到这个[微博](http://weibo.com/2388714105/DjvJZC1Qc?from=page_1005052388714105_profile&wvr=6&mod=weibotime&type=comment)，花了10分钟左右写了翻转链表的代码。

见`reverse_list`目录

### linux process communication

linux进程间通信，可以看看个人博客中的读书笔记。

+ 消息队列，见`linux_process_communication/msgtest.c`文件。在该文件中只是使用了简单地使用了消息队列进行读写，并未涉及进程间的通信:)，以后有时间补上。
+ 共享内存，见`linux_process_communication/shmtest.c`文件。