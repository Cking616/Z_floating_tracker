Z浮随动装置介绍
======
# 目录
分四部分介绍：
* 总览
* 测量系统
* 被控对象
* 控制系统
* 工程实践
 
> 说明：技术经过多年的发展，各方面内容都已经积累下丰富的资料，所以前三章的内容采用图加网络可获取的资源链接的形式，使得读者能通过图简单了解，通过链接下的资料可以较为深刻的了解，这里重点来说工程实践中所考虑的因素。

# 总览
控制系统的输出源单一，就是模拟量的速度指令(为电压量)，被控对象伺服驱动器为速度控制模式，根据这个模拟量控制电机运行的速度。但是控制系统的输入源有两种情况
## 位置控制模式
![][pulse_control]

伺服驱动器位置模式示意
![][position_model]

## 电容控制模式
![][capacity_tracker]
## 资料
* [论文：激光切割机高度调节系统的设计][controller1]
# 测量系统
* 电容测量示意图

![][capacitive_transducer]

* 电容距离转换公式

![][capacity2height]

* 电容距离函数图

![][capacity2height2]
* 资料
    * [论文：激光切割电容式Z浮微小电容检测系统的研究][capacity_measure1]
    * [论文：用边缘电容测量表面光洁度][capacity_measure2]
# 被控对象
![][speed_model]

* 资料
    * [安川伺服驱动器资料文档][motor1]
    * [施耐德Lexium 23伺服驱动器文档][motor2]
# 控制系统
## 参考系统
* 振镜控制系统
    
    振镜控制属于带宽(不理解带宽概念的看首页下面的链接)最高的系统，而且实现全部采用响应速度最快的模拟电路实现，我们的控制系统可以在振镜系统上做裁剪设计即可。

![][galvanometer]
* PD加前馈系统
    
    需要响应速度快稳定性高伺服系统一般设计上使用PD加前馈来设计，这是一个标准PD加前馈系统:

![][Pd_controller]

> 注:图中yd表示目标位置，y表示实际位置，dyd和d2yd分别表示yd的一阶导数和二阶导数
## Z浮系统

> 注:这里只说跟电容模式下的系统模型，位置控制模式参考伺服驱动器即可

![][laser]

设从电容传感器返回的距离量为E，设定的高度为d，Z轴编码器值为ye则有:
~~~ 
    实际位置: y = ye
    目标位置: yd = 平板位置 - 设定高度 = (ye + E) - d
    误差为: e = y - yd = d - E
    取为PD+前馈控制模式时，其中k1,k2,f1,f2均为常数:
        u = k1 * e - k2 * de + f1 * dyd + f2 * d2yd
          = k1 * (d - E) - k2 * dE + f1 * (dye + dE) + f2 * d2(ye + E)
    可取k2 = f1 则有:
        u = k1 * (d - E) - f1 * dye + f2 * d2(ye + E)
~~~
    
* 资料
  * [论文：高速振镜伺服驱动的PID加前馈控制][controller2]
  * [论文：直线电机伺服系统的复合前馈PID控制][controller3]
# 工程实践

# 链接
  [返回首页][main_page]
  
  [capacity_measure1]: http://www.docin.com/p-776342960.html
  [capacity_measure2]: http://www.ixueshu.com/document/a64bfffc7bcb5a7e318947a18e7f9386.html#pdfpreview
  [controller1]: http://xueshu.baidu.com/s?wd=paperuri:%28bc93f3433a45560e716c78a6c15744a7%29&filter=sc_long_sign&tn=SE_xueshusource_2kduw22v&sc_vurl=http://www.doc88.com/p-2691543418116.html&ie=utf-8&sc_us=610292654239538222
  [controller2]: https://wenku.baidu.com/view/3217674f482fb4daa58d4bbf.html
  [controller3]:https://wenku.baidu.com/view/3af1bcfa770bf78a652954d2.html
  [motor1]:http://www.yaskawa.com.cn/database/default.aspx?sel=2_72339069014638592&sid=1_5&zl=144678138029277184
  [motor2]:https://www.schneider-electric.cn/zh/product-range-download/2265-lexium-23#tabs-top
  [main_page]: ../ReadMe.md


  [pulse_control]: ../img/Introduction/pulse_control.png
  [capacity_tracker]: ../img/Introduction/capacity_tracker.png
  [position_model]: ../img/Introduction/position_model.png
  [speed_model]: ../img/Introduction/speed_model.png
  [capacitive_transducer]: ../img/Introduction/capacitive_transducer.png
  [capacity2height]: ../img/Introduction/capacity2height.png
  [capacity2height2]: ../img/Introduction/capacity2height2.png
  [galvanometer]: ../img/Introduction/galvanometer.png
  [Pd_controller]: ../img/Introduction/Pd_controller.png
  [laser]: ../img/laser.png

