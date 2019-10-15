sudo apt-get install inotify-tools;
clear;
while inotifywait -e modify ./src/main.c; do clear && bash ./scripts/build.sh && ./program; done