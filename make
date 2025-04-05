rm -f pacnode
g++ -g -O2 -I. pacnode.cpp message.cpp simplelogger.cpp util.cpp -o pacnode -lssl -lcrypto
