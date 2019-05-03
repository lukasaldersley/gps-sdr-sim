echo run this as root
apt install git -y
mkdir /gnsssim
git clone github.com/lukasaldersley/gps-sdr-sim /gnsssim
chown -R gps /gnsssim
chgrp -R gps /gnsssim
gcc /gnsssim/gpssim.c -lm -O3 -o /gnsssim/gps-sdr-sim -DUSER_MOTION_SIZE=6000
gcc /gnsssim/gpssim.c -lm -O3 .o /gnsssim/gps-sdr-sim-live -DLIVE -DUSER_MOTION_SIZE=12000
gcc /gnsssim/hackrf-live-streaming.c -o /gnsssim/hackrf-live-streaming
echo "[Unit]\nDescription=GNSS Simulator service\nAfter=network.target\nStartLimitIntervalSec=0\n\n[Service]\nType=simple\nRestart=always\nRestartSec=1\nUser=gnss\nExecStart=/gnsssim/hackrf-live-streaming\n\n[Install]\nWantedBy=multi-user.target">GPS_Simulator.service
#systemctl start GPS_Simulator
#systemctl enable GPS_Simulator
#rm $0
