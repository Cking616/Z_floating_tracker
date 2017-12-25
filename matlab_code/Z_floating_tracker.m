% matlab����ֻ��ʶ��gbk����ģ�������ļ�Ϊgbk����
% �ر���������,
clear all;
close all;

% ʱ������ȡΪΪ0.5ms, �˴���λΪ��,��Ϊ0.0005s
period_time=0.0005;

% ����ϵͳ����,�򵥴���Ϊһ���ٶȹ��Ի��ں�һ���ٶȻ��ֻ���
% ���ض���ȡ�� Y = 1 / ((T * s + 1) * s), ����Tȡ0.1
servo_sys=tf(1, [0.1,1,0]);
% �ܿض���Z�任
dservo_sys=c2d(servo_sys,period_time,'z');
[servo_num,servo_den]=tfdata(dservo_sys,'v');

% ǰ���˲�����,Ϊһ�׻����˲�[y(n) = a * x + b * y(n - 1), b = 1 - a]
feedfoword_fliter_a = 475 / 1024;feedfoword_fliter_b = 549 / 1024;

% uΪ����ٶ�,u_1Ϊ��������е�u(k),u_2Ϊ��������е�u(k-1),u_3Ϊ��������е�u(k-2)
u_1=0.0;u_2=0.0;u_3=0.0;

% yΪ���λ��,ydΪĿ��λ��,�±�1,2,3Ϊ�����м�����
yd_1=0;yd_2=0;yd_3=0;
y_1=0;y_2=0;y_3=0;

% vΪǰ���ٶ�,�±�����ͬ��
v_1=0;v_2=0;v_3=0;

% kp,kvff,kaff��ֵ,kp��λHz,kvff������,kaff��λs
kp_0=200 * 1000 / 2048;kvff=1;kaff=0;

% λ������,��λ0.01mm
limit_position = 30;
% ��ǰ���,��λ0.01mm
cur_e = 0;
kp=kp_0;
% ���Ӽ��ٶ�,�����ٶ�(m/s^2),����ٶ�(m/s)
limit_Tc = 5;limit_acc = 15;limit_u = 5;
% ���յ����ǰ���ٶ�(m/s),���ܵ����ǰ�����ٶ�(m/s^2)
limit_v = 5;limit_facc = 0.1;

% ����ʱ����������,x(1)Ϊe,��λΪ(m)
% x(2)Ϊǰ���ٶ�,��λΪ(m/s),x(3)Ϊǰ�����ٶȵ�λΪ(m/s^2)
x=[0,0,0]';

% �ܷ���ʱ��Ϊ 20000 * period_time = 10s
for k=1:1:20000
    time(k)=k*period_time;

    if time(k) == 10
        yd(k) = 0;
    else       
        % Ŀ��yd���죬С��2sʱ����2m/s^2�ȼ����˶�
        if mod(time(k),10) < 2
            yd(k)=(time(k)) * (time(k));
        % 2~3sʱ����4m/s�����˶�
        elseif mod(time(k),10) < 3
            yd(k)= time(k) * 4 - 4;
        % 3~5sʱ����2m/s^2�ȼ����˶�
        elseif mod(time(k),10) < 5
            yd(k)=12 - (5 - time(k)) * (5 - time(k));
        % 5~7sʱ����2m/s^2�����ȼ����˶�
        elseif mod(time(k),10) < 7
            yd(k)= 12 - (time(k) - 5) * (time(k) - 5);
        % 7~8sʱ����4m/s�����˶�
        elseif mod(time(k),10) < 8
            yd(k)= 8 - (time(k) - 7) * 4;
        % 8~10sʱ����2m/s�ȼ����˶����ٶ�Ϊ0
        else
            yd(k)= (10 - time(k)) * (10 - time(k));
        end
    end

    % kaff����Ӧ��������ʱֻ�Ƿ����˼��ٺͼ������
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

    % kp����Ӧ����,sech�����������,Ӳ�������»��ɶ���ʽ����
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

    % �ٶȼ��㣬�ٶ��˲����ⷴ�����ּ��ٶȹ���ľ��ұ仯
    u_1=kp*x(1)+kvff*x(2)+kaff*x(3);
    u_1= u_1 * 924 / 1024 + u_2 * 50 / 1024 + u_3 * 50 / 1024;
    u(k)=u_1;

    % �ٶ�����
    if u(k)>=limit_u
        u(k)=limit_u;
    end
    if u(k)<=-limit_u;
        u(k)=-limit_u;
    end

    % �Ӽ��ٶ�����
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

    % ���ٶ�����
    if acc(k)>=limit_acc
        acc(k)=limit_acc;
    end
    if acc(k)<=-limit_acc
        acc(k)=-limit_acc;
    end

    u(k) = u_2 + period_time * acc(k);
    u_1 = u(k);

    % ���ٶ����תΪλ��
    y(k)=-servo_den(2)*y_1-servo_den(3)*y_2+servo_num(2)*u_1+servo_num(3)*u_2;
    error(k)=yd(k)-y(k);

    yd_3=yd_2;yd_2=yd_1;yd_1=yd(k);
    y_3=y_2;y_2=y_1;y_1=y(k);
    u_3=u_2;u_2=u_1;
    v_1 = (yd_1-yd_2) / period_time;

    % ǰ���ٶ�����
    if v_1 > limit_v
        v_1=limit_v;
    end

    if v_1 < -limit_v
        v_1=-limit_v;
    end

    % ǰ���ٶ��˲�
    v_1 = feedfoword_fliter_b * v_2 + feedfoword_fliter_a * v_1;

    x(1)=error(k);
    x(2)= v_1;
    x(3)=(v_1 - v_2)/ period_time;
    % ǰ�����ٶ�����
    if x(3) > limit_facc
        x(3) = limit_facc;
    end
    if x(3) < -limit_facc
        x(3) = -limit_facc;
    end
    v_2 = v_1;

    % ת��e��λ��0.01mm������鿴
    error_100(k) = error(k) * 100 * 1000;
    cur_e = error_100(k);
end

% Ŀ��yd��ʵ��yͼ,x��Ϊʱ�䵥λs,y��λΪm
figure(1);
plot(time,yd,'r',time,y,'k:','linewidth',2);
legend('Ideal position signal','Position tracking');

% λ�����eͼ,����Ϊʱ�䵥λ,���ᵥλΪ0.01mm
figure(2);
plot(time,error_100,'r','linewidth',2);
legend('error position');

% ����ٶ�ͼ,��λΪm/s
figure(3);
plot(time,u,'r','linewidth',2);
legend('speed');

% ������ٶ�ͼ,��λΪm/s^2
figure(4);
plot(time,acc,'r','linewidth',2);
legend('acceleration');
