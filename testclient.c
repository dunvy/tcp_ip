#include <stdio.h> //입출력
#include <stdlib.h> //문자열 변환, 의사 난수 생성
#include <unistd.h> //표준 심볼 상수 및 자료형
#include <string.h> //문자열 상수
#include <arpa/inet.h> //주소변환
#include <sys/socket.h> //소켓 연결
#include <netinet/in.h> //IPv4 전용 기능
#include <pthread.h> //쓰레드 사용

void error_handling(char* message); 

int main(int argc, char* argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    pthread_t snd_thread, rcv_thread;
    void* thread_return;


    int str_len;
    char message[30];

    if (argc!=3){
        printf("Usage: %s <port>\n", argv[0]);      
        exit(1);                                    
    }

    // 소켓 생성 후, 서버에 연결
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock== -1)
        error_handling("socket() error");

    // 서버 주소 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // FILE* fp2 = fopen("seoul.csv", "w");
    // if (fp2 == NULL){
    //     printf("파일오픈 실패! \n");
    //     return -1;
    // }

    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");

    // read(sock, message, sizeof(message)-1);    
    // puts(message);

    // while((str_len = read(sock, message, sizeof(message))) != 0){            // 파일의 끝에 도달한 경우 0이 아닌 값 반환
    //     message[str_len] = '\0';
    //     puts(message);
    // }

    // while((str_len = read(sock, message, sizeof(message))) != 0){            // 파일의 끝에 도달한 경우 0이 아닌 값 반환
    //     message[str_len] = '\0';
    //     fputs(message, fp2);
    //     printf("%s", message);
    // }

    while((str_len = read(sock, message, sizeof(message))) != 0){            // 파일의 끝에 도달한 경우 0이 아닌 값 반환
        message[str_len] = '\0';
        printf("%s", message);
    }
	
    close(sock);
    return 0;
}

void error_handling(char* message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
