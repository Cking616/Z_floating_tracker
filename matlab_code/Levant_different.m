% matlab中文只能识别gbk编码的，故这个文件为gbk编码
% 关闭其他窗口,
clear all;
close all;

% 时钟周期取为0.5ms, 此处单位为秒,故为0.0005s
period_time=0.0005;

% 用于中间过程
y_1 = 0; dy_1 = 0;

% 总时间取十秒
for k=1:1:20000
	cur_time = k * period_time;
	time(k) = cur_time;

	% 定义真实信号和噪声信号
	u(k) = sin(k * period_time);
	du(k) = cos(k * period_time);
	noise(k) = 0.5 * sign(rands(1));

	% 混合噪声
	if mod(k, 100) == 1
		yd(k) = u(k) + noise(k);
	else
		yd(k) = u(k);
	end
	yd(k) = yd(k) + 0.15 * rands(1);

    % 定义系数
    alfa = 1.5; nmna = 6;

	% 按公式进行计算
	y(k) = y_1 + period_time * (dy_1 - nmna * sqrt(abs(y_1 - yd(k)))) * sign(y_1 - yd(k));

	dy(k) = dy_1 - period_time * alfa * sign(y_1 - yd(k));

	y_1 = y(k); dy_1 = dy(k);
end

% 滤波信号与真实信号对比图
figure(1);
plot(time, u, 'r',time, y, 'k:', 'linewidth', 2);
legend('sin(t)', 'y');

% 滤波信号微分
figure(2);
plot(time, du, 'r', time, dy, 'k:', 'linewidth', 2);
legend('cos(t)', 'dy');