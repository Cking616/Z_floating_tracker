嵌入式编程风格
======
# 前言
网络上已经流传了大量经验丰富工程师写的相关准则，
如[编写优质嵌入式C程序][c_code_style1]、[嵌入式C语言优化小技巧][c_code_style2]，助人参考查阅。
又同时有大量写的相当优秀的短小精悍的开源代码与其分析说明文档，如[lua语言核心代码][lua1]、[lua源代码欣赏][lua2]，
供人学习瞻仰。

笔者又非在这一领域深耕多年之人，本来没有能力写这方面的大道。不过细细想来优秀参照确实能让人在深入理解后如同醍醐灌顶，
但是单薄而枯燥的条规却使人望而却步，没有饱满感受的事物实在难以让人产生认同感，故而作者想来在此文档中，写下一些自身在
实践过程中关于编码的体会，作为对那些条规的释义补充。

# 工具
工欲善事，必先利其器；一个人要做好一件事情处决于两个方面：内因、外因，内因是沉积性的，随人的阅历、分析表达能力、专业
知识的增加而增加的，随着时间变化甚至经常只会出现倒退不会增加。
*  笔者使用的工具

    ![][IDE1]
   
    ![][IDE2]
   
   [IDE1]: ../img/Chapter3/IDE1.png
   [IDE2]: ../img/Chapter3/IDE2.png
   
   [lua1]: http://www.lua.org/download.html
   [lua2]: http://download.csdn.net/download/monkey07118124/9700822
   [code_style1]: https://zhuanlan.zhihu.com/p/21270222
   [code_style2]: http://blog.csdn.net/rexuefengye/article/details/47029339
   [c_code_style1]: http://blog.csdn.net/zhzht19861011/article/details/45508029
   [c_code_style2]: http://blog.csdn.net/u013467442/article/details/47071171
   [jetbrains]: https://www.jetbrains.com/
   [myvimsc]: https://github.com/Cking616/MyVimsc
   [vim1]: https://www.jianshu.com/p/4188326dfba6
