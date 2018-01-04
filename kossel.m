clf;
armcnt = 3;
Rarms = 152; % (152-(31)-(22.6));
Rpret = 218;
h = 300;

% s = serial('COM5', 'baudrate', 9600);
% fopen(s);


% Polozenie koncowki drukujacej
xc = 0;
yc = 0;
zc = 0;
drawRadius = 30; % max 90

for i = 1:3
	xbase(i) = Rarms * sin((2*pi*i)/armcnt);
	ybase(i) = Rarms * cos((2*pi*i)/armcnt);
end

for i = 1:3
	dx = xbase(i) - xc;
	dy = ybase(1) - yc;
	z(i) = zc + sqrt(Rpret*Rpret - dx*dx - dy*dy);
	arms(i) = line([xbase(i), xc], [ybase(i), yc], [z(i), zc]);
	set(arms(i), 'Color', [.7, 0, 0]);
end

% z(i) - pozycje kolumn

colLines = zeros(3);
bottomLines = zeros(3);
topLines = zeros(3);

bottomLines(1) = line([xbase(1), xbase(2)], [ybase(1), ybase(2)], [0, 0]);
bottomLines(1) = line([xbase(2), xbase(3)], [ybase(2), ybase(3)], [0, 0]);
bottomLines(1) = line([xbase(3), xbase(1)], [ybase(3), ybase(1)], [0, 0]);

topLines(1) = line([xbase(1), xbase(2)], [ybase(1), ybase(2)], [h, h]);
topLines(1) = line([xbase(2), xbase(3)], [ybase(2), ybase(3)], [h, h]);
topLines(1) = line([xbase(3), xbase(1)], [ybase(3), ybase(1)], [h, h]);

% Kolumny
for i = 1:3
	colLines(i) = line([xbase(i), xbase(i)], [ybase(i), ybase(i)], [0, h]);
end

view(125, 10);

% Kossel: 0..25k, 25k is bottom

dont = 0;
routex = [];
routey = [];
routez = [];
for t = 1:2:600
	xc = drawRadius * sin(2*pi/60 *t);
	yc = drawRadius * cos(2*pi/60 *t);
	zc = t/600 * h/4; % h/4 + sin(2*pi/120 * t);
	routex(end+1) = xc;
	routey(end+1) = yc;
	routez(end+1) = zc;
	for i = 1:3
		dx = xbase(i) - xc;
		dy = ybase(i) - yc;
		z(i) = zc + sqrt(Rpret*Rpret - dx*dx - dy*dy);
		if(z(i) > h)
			dont = 1;
		end
	end
	if(~dont)
		for i = 1:3
			set(arms(i), 'XData', [xbase(i), xc], 'YData', [ybase(i), yc], 'ZData', [z(i), zc]);
			toSend(i) = round((z(i)/h) * 25000);
		end
	end
	str = sprintf('x %d y %d z %d\n', toSend(1), toSend(2), toSend(3));
% 	fprintf('%s', str);
	fprintf(s, str);
	hold on;
	plot3(routex, routey, routez);
	hold off;
	drawnow();
	pause(1/60);
end

% fclose(s);