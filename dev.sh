sudo apt-get install inotify-tools;
clear;
bash ./build.sh && ./program;
while inotifywait -e modify ./src/main.c; do clear && bash ./build.sh && ./program; done