rm -f pacnode
g++ -O2 -I. pacnode.cpp message.cpp util.cpp -o pacnode -lssl -lcrypto
