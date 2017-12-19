物理层设计
======
# 目录
* 反馈输入模块
* DAC模块
* 物理量转换
# 反馈输入模块
反馈输入模块分为电容检测的处理，和从电容到距离的转换模式。
## 电容检测
### 调频法测电容
> 调频法是最想当然的电容测量方法，自己设计电路输出测量频率值到MCU的寄存器，然后MCU定时读取.
* 测量电路示意图 

    ![][capacity_measure_img1]

* 电容频率转换关系

    ![][capacity_measure_img2]
    
* 资料
    * [电容式位移传感器][capacity_measure1]
    
> 调频法较为成熟，且由于工控环境中的技术普遍迭代速度较慢，目前还为很多公司采用。
### 电容测量模块

* 测量电路示意图 

    ![][capacity_measure_img3]

* 转换关系

    ![][capacity_measure_img4]

* 成熟的测量芯片模块
    > 德国acam公司专利的PICOCAP测量原理则给电容测量提供了革命性的突破。在2011年推出
了最新的带有内部DSP单片机的单芯片电容测量方案PCAP01, 这个芯片会使电容测量提高到
一个前所未有的水平。 

    ![][capacity_measure_img5]

* 资料
    * [PCAP01芯片测量原理][capacity_measure2]
    * [PCAP01芯片用户手册][capacity_measure3]
    * [论文：PCAP01与STM8的激光头电容测距传感器设计][capacity_measure4]
    
* 优势
    > 在产品化方面的优势在于不用什么模块都用自己从头来制作；从解决实际问题角度来讲，切割过程由于高功率会导致温度变化很大，
    可以很好的利用PCAP01解决电容测量时因温度导致的问题,将很多环境影响因素缩小。
    
## 电容距离转换
> 电容距离转换是一个非线性关系，且由于环境不同其参数，故为了使得这些环境下能通用只能采用查表法转距离

查表法要确定查表计量的最小单元，由上面给出的[论文][capacity_measure4]，采用PCAP01芯片使用文中作者给定的方法可以达到：
在2.5毫米以内的位置，测量误差低于0.5微米，故取最小计量单元为1 μm较为合适。

这里只提查表法，具体查表法需要注意的问题和算法实现在[后文][chapter5]。
# DAC模块
DAC模块是将数字量转为电压量输出的元件，现今已高度成熟，芯片和原理和参考[此文][DAC2]。

DAC模块需要注意的是其位数和输出电压范围。

输出电压范围可以通过放缩、加偏置等进行调节，如果电机要能满足正反转，需要有正向电压
和反向电压存在，正向最大输出电压对应正向最大速度，方向对应方向最大速度，如使得电压输出范围为-10 ~ +10V（很多伺服驱动器的输入均以此范围，故以此为例）

如施耐德驱动器电压速度转换图

![][DAC_img1]

DAC模块的位数决定其输出精度，如精度为八位时，数字输出0x00~0xFF对应最低电压到最高电压值。
常见DAC模块选型可见[此处][DAC1]。
# 物理量转换
## 外部物理量
* 电机
    ![][motor1]
    
    实际中电机通常单位为r/min(转每分)，且在中国大部分额定转速都是3000r/min。
* 丝杆

    ![][screw]
    
    丝杆一般最重要的是螺距，单位是mm/r，一般的规格是5mm/r、10mm/r等。

* 速度
    > 速度 = 丝杆螺距 * 电机转速 (mm/min)
 
* 伺服
    > 伺服要定义一个 速度 = a * 电压 的关系，这里 a 的单位为 r/(min * V)，称之为速度系数。
## 内部物理量
* 时间
    > 时间单位取法为了方便运算取控制周期的较小倍数较好，目前控制周期是100μs级的数，取ms较为合适。
* 距离
    > 距离单位按电容转换精度取为μm。
* 速度
    > 由上速度单位为μm/ms。
* 数字输出
    > 输出电压 V = 最大电压 V * (数字输出 / (2 ^ 传感器位数 / 2) )
## 输出转换
    设：
    丝杆螺距为 len mm/r， 速度系数为 sp r/(min * V)， 最大电压为 maxv V， 传感器位数为 dig 位
    , 计算速度为 calu μm/ms， 实际速度为 relu (mm / min) 。
    
    有条件：
    数字输出 = b * calu
    输出电压 V = 最大电压 V * (数字输出 / (2 ^ 传感器位数 / 2) )
    实际速度 = 输出电压 * 速度系数 * 螺距
    
    求b,使得 计算速度 calu(μm/ms) = 实际输出速度 relu(mm/min)
    
使用符号演算 
~~~
relu = sp * maxv * (b * calu / ( 2 ^ dig / 2))  * len 
b = relu * (2 ^ dig / 2) / (sp * maxv * len * calu)
b = (2 ^ dig / 2 / (sp * maxv * len)) * (relu / calu)
~~~
    由于 b 的目标是使得 relu(μm/ms) = calu(mm/min) ，代入值约化单位即可求得转换系数b。


  [chapter5]: Chapter5.md

  [capacity_measure_img1]: ../img/Chapter1/capacity_mesure1.png
  [capacity_measure_img2]: ../img/Chapter1/capacity_mesure2.png
  [capacity_measure_img3]: ../img/Chapter1/capacity_mesure3.png
  [capacity_measure_img4]: ../img/Chapter1/capacity_mesure4.png
  [capacity_measure_img5]: ../img/Chapter1/capacity_mesure5.png
  [DAC_img1]: ../img/Chapter1/DAC1.png
  [motor1]: ../img/Chapter1/motor1.png
  [screw]: ../img/Chapter1/screw1.jpg
    
  [DAC1]: https://wenku.baidu.com/view/41430572f01dc281e53af0bb.html
  [DAC2]: http://www.doc88.com/p-7905996394673.html
  
  [capacity_measure1]: https://wenku.baidu.com/view/ed0c8632a32d7375a4178034.html
  [capacity_measure2]: http://blog.csdn.net/yunxianpiaoyu/article/details/9167235
  [capacity_measure3]: https://wenku.baidu.com/view/5cb65dc98762caaedd33d48b.html
  [capacity_measure4]: http://xueshu.baidu.com/s?wd=paperuri%3A%28c347731cbb8812598b75bdb899821db7%29&filter=sc_long_sign&tn=SE_xueshusource_2kduw22v&sc_vurl=http%3A%2F%2Fwww.doc88.com%2Fp-6721537117865.html&ie=utf-8&sc_us=2871659838306416451
