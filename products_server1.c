#include <stdio.h> //입출력
#include <stdlib.h> //문자열 변환, 의사 난수 생성
#include <unistd.h> //표준 심볼 상수 및 자료형
#include <string.h> //문자열 상수
#include <arpa/inet.h> //주소변환
#include <sys/socket.h> //소켓 연결
#include <netinet/in.h> //IPv4 전용 기능
#include <pthread.h> //쓰레드 사용
#include <locale.h>
#define BUF_SIZE 102400                   // 버퍼 사이즈
// #define PORTNUM 8888                    // port 번호
#define MAX_CLNT 256

void error_handling(char* message);

// 임계영역 구성할 전역변수
int clnt_cnt = 0;               // 현재 연결된 클라이언트 수
int clnt_socks[MAX_CLNT];       // 클라이언트 소켓 디스크립터 저장

pthread_mutex_t mutx;                   // 뮤텍스 생성을 위한 변수 선언

int main(int argc, char* argv[]){
    argc = 2;
    argv[1] = "8889";
    int serv_sock, clnt_sock;                   // 서버 소켓, 클라이언트 소켓
    struct sockaddr_in serv_adr, clnt_adr;      // 서버 소켓 주소, 클라이언트 소켓 주소
    int clnt_adr_sz;                            // 클라이언트 소켓 주소 정보의 크기
    
    pthread_t t_id;                             // 쓰레드 생성에 사용될 쓰레드 변수

    if (argc!=2){
        printf("Usage: %s <port>\n", argv[0]);      // argv[0]: 포트번호
        exit(1);                                    // 비정상적인 종료
    }

    // 뮤텍스 초기화 (뮤텍스 변수 전달)
    // pthread_mutex_init(&mutx, NULL);

    // 서버 소켓 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");

    // 한글 깨져서 넣어줌
    setlocale(LC_ALL, "en_US.UTF-8");

    // 주소 초기화 및 설정
    memset(&serv_adr, 0, sizeof(serv_adr));         // serv_adr 변수의 메모리를 모두 0으로 초기화
    serv_adr.sin_family = AF_INET;                  // IPv4 주소 사용
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);   // 소켓이 모든 네트워크 인터페이스에 들어오는 패킷을 수신
    serv_adr.sin_port = htons(atoi(argv[1]));             // 서버 포트 번호 설정 8888

    // 주소 할당 및 연결요청 대기
    if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))== -1)
        error_handling("bind() error");
    if (listen(serv_sock, 5)==-1)
        error_handling("listen() error");


    // 클라이언트에서 보낸 데이터를 받아서 저장할 파일
    // FILE* fp2 = fopen("product.csv", "w, ccs=UTF-8");
    FILE* fp2 = fopen("seoule13.csv", "w");
    // FILE* fp3 = fopen("product.csv", "at+");

    if (fp2 == NULL){
        printf("파일오픈 실패! \n");
        return -1;
    }


    // ===========================================================
    // 클라이언트 연결 수락
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

    char buf[BUF_SIZE];                         // 클라이언트로부터 수신한 메시지를 저장할 문자열 배열
    int str_len = 0;                            // 클라이언트로부터 수신한 메시지의 길이 저장

    // while((str_len = recv(clnt_sock, buf, sizeof(buf), 0)) > 0){
    //     fwrite(buf, 1, str_len, fp2);
    //     printf("%s", buf);        
    // }

    while((str_len = read(clnt_sock, buf, sizeof(buf))) != 0){            // 파일의 끝에 도달한 경우 0이 아닌 값 반환
        buf[str_len] = '\0';
        fputs(buf, fp2);
        printf("%s", buf);
    }


    // 일단 됨
    // read(clnt_sock, buf, sizeof(buf)-1);    
    // fputs(buf, fp2);

    // // 버퍼에 남아있는 데이터 강제로 출력함 => fclose할 때 자동으로 비워지니까 굳이 필요 없음
    // fflush(fp2);

    fclose(fp2);
    close(serv_sock);
    return 0;
}


void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
