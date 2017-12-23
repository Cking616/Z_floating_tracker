% matlab����ֻ��ʶ��gbk����ģ�������ļ�Ϊgbk����
% �ر���������,
clear all;
close all;

% ʱ������ȡΪ0.5ms, �˴���λΪ��,��Ϊ0.0005s
period_time=0.0005;

% �����м����
y_1 = 0; dy_1 = 0;

% ��ʱ��ȡʮ��
for k=1:1:20000
	cur_time = k * period_time;
	time(k) = cur_time;

	% ������ʵ�źź������ź�
	u(k) = sin(k * period_time);
	du(k) = cos(k * period_time);
	noise(k) = 0.5 * sign(rands(1));

	% �������
	if mod(k, 100) == 1
		yd(k) = u(k) + noise(k);
	else
		yd(k) = u(k);
	end
	yd(k) = yd(k) + 0.15 * rands(1);

    % ����ϵ��
    alfa = 1.5; nmna = 6;

	% ����ʽ���м���
	y(k) = y_1 + period_time * (dy_1 - nmna * sqrt(abs(y_1 - yd(k)))) * sign(y_1 - yd(k));

	dy(k) = dy_1 - period_time * alfa * sign(y_1 - yd(k));

	y_1 = y(k); dy_1 = dy(k);
end

% �˲��ź�����ʵ�źŶԱ�ͼ
figure(1);
plot(time, u, 'r',time, y, 'k:', 'linewidth', 2);
legend('sin(t)', 'y');

% �˲��ź�΢��
figure(2);
plot(time, du, 'r', time, dy, 'k:', 'linewidth', 2);
legend('cos(t)', 'dy');