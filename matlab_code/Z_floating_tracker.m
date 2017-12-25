% matlab中文只能识别gbk编码的，故这个文件为gbk编码
% 关闭其他窗口,
clear all;
close all;

% 时钟周期取为为0.5ms, 此处单位为秒,故为0.0005s
period_time=0.0005;

% 被控系统仿真,简单处理为一个速度惯性环节和一个速度积分环节
% 被控对象取成 Y = 1 / ((T * s + 1) * s), 下面T取0.1
servo_sys=tf(1, [0.1,1,0]);
% 受控对象Z变换
dservo_sys=c2d(servo_sys,period_time,'z');
[servo_num,servo_den]=tfdata(dservo_sys,'v');

% 前馈滤波常数,为一阶互补滤波[y(n) = a * x + b * y(n - 1), b = 1 - a]
feedfoword_fliter_a = 475 / 1024;feedfoword_fliter_b = 549 / 1024;

% u为输出速度,u_1为计算过程中的u(k),u_2为计算过程中的u(k-1),u_3为计算过程中的u(k-2)
u_1=0.0;u_2=0.0;u_3=0.0;

% y为输出位置,yd为目标位置,下标1,2,3为过程中计算量
yd_1=0;yd_2=0;yd_3=0;
y_1=0;y_2=0;y_3=0;

% v为前馈速度,下标意义同上
v_1=0;v_2=0;v_3=0;

% kp,kvff,kaff初值,kp单位Hz,kvff无量纲,kaff单位s
kp_0=200 * 1000 / 2048;kvff=1;kaff=0;

% 位置限制,单位0.01mm
limit_position = 30;
% 当前误差,单位0.01mm
cur_e = 0;
kp=kp_0;
% 最大加加速度,最大加速度(m/s^2),最大速度(m/s)
limit_Tc = 5;limit_acc = 15;limit_u = 5;
% 接收的最大前馈速度(m/s),接受的最大前馈加速度(m/s^2)
limit_v = 5;limit_facc = 0.1;

% 计算时的输入向量,x(1)为e,单位为(m)
% x(2)为前馈速度,单位为(m/s),x(3)为前馈加速度单位为(m/s^2)
x=[0,0,0]';

% 总仿真时间为 20000 * period_time = 10s
for k=1:1:20000
    time(k)=k*period_time;

    if time(k) == 10
        yd(k) = 0;
    else       
        % 目标yd构造，小于2s时，以2m/s^2匀加速运动
        if mod(time(k),10) < 2
            yd(k)=(time(k)) * (time(k));
        % 2~3s时，以4m/s匀速运动
        elseif mod(time(k),10) < 3
            yd(k)= time(k) * 4 - 4;
        % 3~5s时，以2m/s^2匀减速运动
        elseif mod(time(k),10) < 5
            yd(k)=12 - (5 - time(k)) * (5 - time(k));
        % 5~7s时，以2m/s^2方向匀加速运动
        elseif mod(time(k),10) < 7
            yd(k)= 12 - (time(k) - 5) * (time(k) - 5);
        % 7~8s时，以4m/s匀速运动
        elseif mod(time(k),10) < 8
            yd(k)= 8 - (time(k) - 7) * 4;
        % 8~10s时，以2m/s匀减速运动到速度为0
        else
            yd(k)= (10 - time(k)) * (10 - time(k));
        end
    end

    % kaff自适应调整，暂时只是分离了加速和减速情况
    if v_1 > 0
        if x(3) > 0
            kaff = 2.1;
        else
            kaff = 1.7;
        end
    else
        if x(3) > 0
            kaff = 2.1;
        else
            kaff = 1.7;
        end
    end

    % kp自适应调整,sech函数调节最好,硬件限制下换成多项式调节
    if cur_e > 0
        if cur_e > limit_position
            kp = 1.55 * kp_0;
        else
        	% kp = (0.05 + 1.5 * (1 - sech(cur_e))) * kp_0;
            kp = (1.5 * cur_e / limit_position + 0.05) * kp_0;
        end
    else
        if cur_e < -limit_position
            kp = 1.55 * kp_0;
        else
        	% kp = (0.05 + 1.5 * (1 - sech(-cur_e ))) * kp_0;
            kp = (1.5 * (-cur_e) / limit_position + 0.05) * kp_0;
        end
    end

    % 速度计算，速度滤波避免反复出现加速度过大的剧烈变化
    u_1=kp*x(1)+kvff*x(2)+kaff*x(3);
    u_1= u_1 * 924 / 1024 + u_2 * 50 / 1024 + u_3 * 50 / 1024;
    u(k)=u_1;

    % 速度限制
    if u(k)>=limit_u
        u(k)=limit_u;
    end
    if u(k)<=-limit_u;
        u(k)=-limit_u;
    end

    % 加加速度限制
    acc(k) = (u(k) - u_2) / period_time;
    if k > 1
        if acc(k) -  acc(k - 1) > limit_Tc
            acc(k) =  acc(k - 1) + limit_Tc;
        end

        if acc(k) -  acc(k - 1) < -limit_Tc
            acc(k) =  acc(k - 1) - limit_Tc;
        end
    else
        if acc(k) > limit_Tc
            acc(k) = limit_Tc;
        end

        if acc(k) < -limit_Tc
            acc(k) = - limit_Tc;
        end
    end

    % 加速度限制
    if acc(k)>=limit_acc
        acc(k)=limit_acc;
    end
    if acc(k)<=-limit_acc
        acc(k)=-limit_acc;
    end

    u(k) = u_2 + period_time * acc(k);
    u_1 = u(k);

    % 由速度输出转为位置
    y(k)=-servo_den(2)*y_1-servo_den(3)*y_2+servo_num(2)*u_1+servo_num(3)*u_2;
    error(k)=yd(k)-y(k);

    yd_3=yd_2;yd_2=yd_1;yd_1=yd(k);
    y_3=y_2;y_2=y_1;y_1=y(k);
    u_3=u_2;u_2=u_1;
    v_1 = (yd_1-yd_2) / period_time;

    % 前馈速度限制
    if v_1 > limit_v
        v_1=limit_v;
    end

    if v_1 < -limit_v
        v_1=-limit_v;
    end

    % 前馈速度滤波
    v_1 = feedfoword_fliter_b * v_2 + feedfoword_fliter_a * v_1;

    x(1)=error(k);
    x(2)= v_1;
    x(3)=(v_1 - v_2)/ period_time;
    % 前馈加速度限制
    if x(3) > limit_facc
        x(3) = limit_facc;
    end
    if x(3) < -limit_facc
        x(3) = -limit_facc;
    end
    v_2 = v_1;

    % 转换e单位到0.01mm，方便查看
    error_100(k) = error(k) * 100 * 1000;
    cur_e = error_100(k);
end

% 目标yd和实际y图,x轴为时间单位s,y单位为m
figure(1);
plot(time,yd,'r',time,y,'k:','linewidth',2);
legend('Ideal position signal','Position tracking');

% 位置误差e图,横轴为时间单位,纵轴单位为0.01mm
figure(2);
plot(time,error_100,'r','linewidth',2);
legend('error position');

% 输出速度图,单位为m/s
figure(3);
plot(time,u,'r','linewidth',2);
legend('speed');

% 输出加速度图,单位为m/s^2
figure(4);
plot(time,acc,'r','linewidth',2);
legend('acceleration');
