自控原理工程实践
======
# 简介
自动化的理论有很多，但是这些理论往往和具体的实际应用相距太远，制作这系列文档的目的，是想如何从零开始结合控制原理来做一个可用于实际场景的产品出来。

***声明：不保证代码的可靠性，工业场景使用有风险***

![z_floating_tracker][1]

激光切割中的Z浮随动装置(Z-Floating-Tracker)，是一通过电容传感器测得反馈距离d来保证运动过程中焦距f始终保持稳定高度的闭环控制器。又由于电容作为反馈元件特性较差（如电容噪声较大、有时延），使用环境覆盖面广、条件恶劣（如按材质有钢材、铝材等，按形状有管材、平板等，又有高功率环境下高温条件等），这些都造成了反馈回路极不稳定。且激光焦距的响应快慢直接影响在切出的工件的优劣，且而随着行业发展，行业对技术要求越来越高，这就要求控制算法同时保证高速、稳定、精度高，因此对于设计者来说是一个很大挑战。
在资源受限、时间受限、必须考虑成本的前提下，必然会遇到很多问题，这里记录遇到的问题的总结与当时的解决方案。
# 目录
* [Z浮隨動裝置介紹][introduction]
*  嵌入式C编程风格
*  代码架构设计
    * ...
    * ...
*  [控制算法Matlab仿真][reconsitution3]
# 資料
* [自控入门：《现代控制系统》][2]
* [自控入门：Feedback Control Of Dynamic Systems][3]
* [先进PID控制及Matlab仿真][4]
* [机器人控制系统及Matlab仿真][5]
* [知乎大神的控制自控系统专栏][6]
* [如何使用自动控制原理][7]
* [控制系统带宽的理解][8]
* [对卡尔曼滤波的理解][9]
* [十大滤波算法C语言][10]

  [introduction]: ./docs/Introduction.md
  [problem1]: ./docs/Problem1.md
  [problem2]: ./docs/Problem2.md
  [problem3]: ./docs/Problem3.md
  [problem4]: ./docs/Problem4.md
  [problem5]: ./docs/Problem5.md
  [problem6]: ./docs/Problem6.md
  [reconsitution1]: ./docs/Reconsitution1.md
  [reconsitution2]: ./docs/Reconsitution2.md
  [reconsitution3]: ./matlab_code/Z_floating_tracker(capacity).m


  [1]: ./img/laser.png
  [2]: http://www.xuexi111.com/book/jishu/322.html
  [3]: https://www.researchgate.net/publication/225075468_Feedback_Control_Of_Dynamic_Systems
  [4]: https://pan.baidu.com/s/1rIxk0
  [5]: http://down.51cto.com/zt/5272
  [6]: https://zhuanlan.zhihu.com/control
  [7]: https://www.zhihu.com/question/34329121
  [8]: https://www.zhihu.com/question/40756707?sort=created
  [9]: https://www.zhihu.com/question/23971601
  [10]: http://blog.csdn.net/qincode/article/details/50727016
