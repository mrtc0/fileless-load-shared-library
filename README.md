# fileless-load-shared-library

---

[ネットワーク経由で取得した共有ライブラリをファイルを作成せずに読み込む](https://blog.ssrf.in/post/fileless-load-shared-library/)

```shell
$ make
$ mv hello.so /tmp
$ cd /tmp
$ python -m http.server

$ ./main
[+] Get Shared Object from http://127.0.0.1:8000/hello.so
[+] Written memfd
[+] Load Start
[+] Hello!
```
