# Network_programming

某大学の授業「ネットワーク」の写経と課題をこなした記録。

## サーバ

### コンパイル

```sh
[vagrant@localhost vagrant]$ gcc -o CServer -Imysocklib CServer.c -Lmysocklib -lmysock
```

### 起動

```sh
[vagrant@localhost vagrant]$ ./CServer
```

## クライアント

### コンパイル
```sh
[vagrant@localhost vagrant]$ gcc -o CSclient CSclient.c
```

### 接続

```sh
[vagrant@localhost vagrant]$ ./CSclient  -p 10007
```