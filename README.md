# socket-programming (OS-B)

マルチスレッドTCP echo Server （唐揚げの注文と待ち時間などなど）

## Refferences
W.リチャードスティーヴンズ, UNIXネットワークプログラミング Vol.1
* Github: https://github.com/unpbook/unpv13e.git
* pdf: https://github.com/sqm2050/wiki/blob/master/Books/c%26programme/UNIX%20Network%20Programming%2C%20Volume%201%2C%20Third%20Edition%2C%20The%20Sockets%20Networking%20API.pdf


## Server
* コンパイル

  gcc TCPEchoServer-Thread.c DieWithError.c CreateTCPServerSocket.c AcceptTCPConnection.c HandleTCPClient.c -o TCPEchoServer-Thread

* 実行方法

  ./TCPEchoServer-Thread 5000

## Client
* コンパイル

  gcc TCPEchoClient.c DieWithError.c -o TCPEchoClient

* 実行方法

  ./TCPEchoClient 127.0.0.1 5000

  Please enter the dish you want to order:

  となるので

  Please enter the dish you want to order:karaage

  と入力する