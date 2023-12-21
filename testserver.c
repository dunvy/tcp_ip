// 파이썬이랑 연결중


#include <stdio.h> //입출력
#include <stdlib.h> //문자열 변환, 의사 난수 생성
#include <unistd.h> //표준 심볼 상수 및 자료형
#include <string.h> //문자열 상수
#include <arpa/inet.h> //주소변환
#include <sys/socket.h> //소켓 연결
#include <netinet/in.h> //IPv4 전용 기능
#include <pthread.h> //쓰레드 사용
#include <locale.h>
#define BUF_SIZE 1024                   // 버퍼 사이즈
#define MAX_CLNT 256


// 필요한 함수
void error_handling(char* message);         // 오류처리
void* handle_clnt(void* arg);               // C 클라이언트에서 파일 받아음


// 임계영역 구성할 전역변수
// C 클라이언트
int clnt_cnt = 0;               // 현재 연결된 클라이언트 수
int clnt_socks[MAX_CLNT];       // 클라이언트 소켓 디스크립터 저장

pthread_mutex_t mutx;                   // 뮤텍스 생성을 위한 변수 선언


// C 클라이언트 스레드 구조체
// typedef struct THREAD{
//     int socket;
//     char* ip;
// }THREAD;


int main(int argc, char* argv[]){
    int serv_sock, clnt_sock;                   // 서버 소켓, 클라이언트 소켓
    struct sockaddr_in serv_adr, clnt_adr;      // 서버 소켓 주소, 클라이언트 소켓 주소
    int clnt_adr_sz;                            // 클라이언트 소켓 주소 정보의 크기
    pthread_t t_id;                             // 쓰레드 생성에 사용될 쓰레드 변수

    if (argc!=2){
        printf("Usage: %s <port>\n", argv[0]);      // argv[0]: 포트번호
        exit(1);                                    // 비정상적인 종료
    }

    // 서버 소켓 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");

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


    FILE* fp = fopen("seoule13.csv", "r");
    // FILE* fp2 = fopen("seoule13_2.csv", "w");
    char buf[1024];                         // 클라이언트로부터 수신한 메시지를 저장할 문자열 배열
    char* buf_send[1024];
    char buf_f[500000];
    
    // ===========================================================
    // 클라이언트 연결 수락
    
    clnt_adr_sz = sizeof(clnt_adr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);


    while (!feof(fp))
        {
            while (!feof(fp)) // fp 파일의 끝이 아니라면 반복
            {
                fgets(buf, sizeof(buf), fp);//받는다
                char *ptr = strtok(buf,",");//자른다 쉼표기준으로
                for (int i = 0; i < 4; i++) // 문자열 15번 반복
                {
                    buf_send[i] = ptr;
                    if (i==0 || i==2 || i==3) // 문자열내의 0,2,3 번 뽑는다
                    {
                        printf("%s,",buf_send[i]); 
                        send(clnt_sock, buf_send[i], strlen(buf_send[i]), 0); // 출력한것들 보낸다 한 줄씩
                        send(clnt_sock,",",1,0);
                    }
                    ptr = strtok(NULL,","); // 다음 문자열 자르고 포인터 반환
                }
                send(clnt_sock,"\n",1,0);
                printf("\n");
                break;
            }
        }


    // while (!feof(fp))
    //     {   
    //         while (!feof(fp)) // fp 파일의 끝이 아니라면 반복
    //         {
    //             fgets(buf, sizeof(buf), fp);//받는다
    //             char *ptr = strtok(buf,",");//자른다 쉼표기준으로
    //             for (int i = 0; i < 4; i++) // 문자열 15번 반복
    //             {
    //                 buf_send[i] = ptr;
    //                 if (i==0 || i==2 || i==3) // 문자열내의 0,2,3 번 뽑는다
    //                 {
    //                     // printf("%s,",buf_send[i]); 
    //                     strcat(buf_f, buf_send[i]);
    //                     // strcat(buf_f, ",");
    //                     // send(clnt_sock, buf_send[i], strlen(buf_send[i]), 0); // 출력한것들 보낸다 한 줄씩
    //                     // send(clnt_sock,",",1,0);
    //                 }
    //                 ptr = strtok(NULL,","); // 다음 문자열 자르고 포인터 반환
    //             }
    //             // printf("\n"); 
    //             break;
    //         }
    //     }
    // printf("%s,",buf_f);
    // send(clnt_sock,buf_f,strlen(buf_f),0);
    
    // while(!feof(fp)){
    //     fgets(buf, sizeof(buf), fp);
    //     send(clnt_sock, )
    // }


    fclose(fp);
    close(serv_sock);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
