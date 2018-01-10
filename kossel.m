% Before running, be sure to open serial port like this:
% s = serial('COM5', 'baudrate', 57600);
% fopen(s);
% After running, close serial:
% fclose(s)

clf;
armcnt = 3;          % Printer has 3 arms
printerRadius = 152; % Radius (mm) of whole 3D printer - not the printing area!
rodLength = 218;     % Length (mm) of rods connecting motors to printer carriage
h = 300;             % Printer height (mm)

% Init carriage pos
xc = 0;
yc = 0;
zc = 0;

z = zeros(3);           % Stepper positions on columns
colLines = zeros(3);    % Lines used to draw columns
bottomLines = zeros(3); % Lines connecting columns on bottom
topLines = zeros(3);    % Lines connecting columns on top

% Place printer columns evenly on circle & draw them
for i = 1:3
	xbase(i) = printerRadius * sin((2*pi*i)/armcnt);
	ybase(i) = printerRadius * cos((2*pi*i)/armcnt);
	colLines(i) = line([xbase(i), xbase(i)], [ybase(i), ybase(i)], [0, h]);
end

% Draw initial arms - could be really anything
for i = 1:3
	dx = xbase(i) - xc;
	dy = ybase(i) - yc;
	z(i) = zc + sqrt(rodLength*rodLength - dx*dx - dy*dy);
	arms(i) = line([xbase(i), xc], [ybase(i), yc], [z(i), zc]);
	set(arms(i), 'Color', [.7, 0, 0]);
end

% Init top and bottom lines connecting columns
bottomLines(1) = line([xbase(1), xbase(2)], [ybase(1), ybase(2)], [0, 0]);
bottomLines(1) = line([xbase(2), xbase(3)], [ybase(2), ybase(3)], [0, 0]);
bottomLines(1) = line([xbase(3), xbase(1)], [ybase(3), ybase(1)], [0, 0]);
topLines(1) = line([xbase(1), xbase(2)], [ybase(1), ybase(2)], [h, h]);
topLines(1) = line([xbase(2), xbase(3)], [ybase(2), ybase(3)], [h, h]);
topLines(1) = line([xbase(3), xbase(1)], [ybase(3), ybase(1)], [h, h]);

% Initial display angle
view(125, 10);

% Valid stepper pos: 0..25k, 25k is bottom
dont = 0;
routex = []; % History for trajectory - stepper 1
routey = []; % Ditto, stepper 2
routez = []; % Ditto, stepper 3
drawRadius = 30; % Spiral radius (mm), max should be around 90
for t = 1:2:600
	% Generate next point for spiral trajectory
	xc = drawRadius * sin(2*pi/60 *t);
	yc = drawRadius * cos(2*pi/60 *t);

	% I've tried zc = h/4 + sin(2*pi/120 * t); but didn't worked nicely
	zc = t/600 * h/4;

	% Save point in trajectory history
	routex(end+1) = xc;
	routey(end+1) = yc;
	routez(end+1) = zc;

	for i = 1:3
		% Translate trajectory pos to stepper position in mm
		% 0 mm is bottom, h mm is topmost
		dx = xbase(i) - xc;
		dy = ybase(i) - yc;
		z(i) = zc + sqrt(rodLength*rodLength - dx*dx - dy*dy);

		% Don't send point if position exceeds printer range
		if(z(i) > h)
			dont = 1;
		end
	end
	if(~dont)
		for i = 1:3
			% Translate stepper pos from mm to steps - inverse z axis along the way
			set(arms(i), 'XData', [xbase(i), xc], 'YData', [ybase(i), yc], 'ZData', [z(i), zc]);
			toSend(i) = round((z(i)/h) * 25000);
		end
	end
	% Generate string for uC and send it
	str = sprintf('x %d y %d z %d\n', toSend(1), toSend(2), toSend(3));
	% fprintf('%s', str); % DEBUG
	fprintf(s, str);

	% DEBUG - that's some dirty way to draw trajectory
	hold on;
	plot3(routex, routey, routez);
	hold off;
	drawnow();
	pause(1/60); % 60 points per second
end
