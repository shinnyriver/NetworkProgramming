#include <stdio.h>
#include <string.h>
#include <errno.h>

// 매크로 정의
#define net_errmsg(no) printf("%s=%s\n", #no, strerror(no))

int main() {
    // 네트워크 관련 일반적인 에러 번호 출력
    net_errmsg(EACCES);
    net_errmsg(EADDRINUSE);
    net_errmsg(EADDRNOTAVAIL);
    net_errmsg(EAFNOSUPPORT);
    net_errmsg(EALREADY);
    net_errmsg(EBADF);
    net_errmsg(ECONNABORTED);
    net_errmsg(ECONNREFUSED);
    net_errmsg(ECONNRESET);
    net_errmsg(EDESTADDRREQ);
    net_errmsg(EHOSTUNREACH);
    net_errmsg(EINPROGRESS);
    net_errmsg(EISCONN);
    net_errmsg(ENETDOWN);
    net_errmsg(ENETRESET);
    net_errmsg(ENETUNREACH);
    net_errmsg(ENOBUFS);
    net_errmsg(ENOPROTOOPT);
    net_errmsg(ENOTCONN);
    net_errmsg(ENOTSOCK);
    net_errmsg(EOPNOTSUPP);
    net_errmsg(ETIMEDOUT);
    net_errmsg(EWOULDBLOCK);
    net_errmsg(EPROTO);
    net_errmsg(EHOSTDOWN);
    net_errmsg(ENODATA);

    return 0;
}
