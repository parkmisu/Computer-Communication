#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

DWORD WINAPI SendThread( LPVOID lpData );

char ID[10];

int main()
{
    //소켓 라이브러리 초기화
    WSADATA wSAData;
    int token = WSAStartup( WINSOCK_VERSION, &wSAData );
    char PORT[5], IP[15];

    //소켓 생성
    SOCKET socket_client = socket( AF_INET, SOCK_STREAM, 0 );

    printf("사용할 IP 주소 : ");
    scanf("%s", &IP);

    printf("사용할 포트 번호 : ");
    scanf("%s", &PORT);

    //소켓 주소 정보
    SOCKADDR_IN servAddr={0};
    servAddr.sin_family=AF_INET;
    servAddr.sin_addr.s_addr=inet_addr(IP);
    servAddr.sin_port=htons( atoi(PORT) ); //포트

    //소켓 접속
    if( connect( socket_client, (struct sockaddr *) &servAddr, sizeof(servAddr) == SOCKET_ERROR))
    {
        closesocket( socket_client );
        printf("서버에 접속할 수 없습니다.");
        return SOCKET_ERROR;
    }

    CreateThread( NULL, 0, SendThread, (LPVOID)socket_client, 0, NULL );
    //상태 출력
    DWORD dwTmp;
    char text[1024] = "서버에 접속되었습니다. \n";
    WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), text, strlen(text), &dwTmp, NULL);

    //전송 & 수신 루프
    while ( 1 )
    {
        char buffer[1024]={0};
        int len = recv( socket_client, buffer, sizeof(buffer), 0 );
        if( len <= 0 )
        {
            printf("접속 종료.");
            break;
        }

        //메시지 출력
        WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, strlen(buffer), &dwTmp, NULL);
    }
    //소켓 해제
    closesocket( socket_client );
    return 0;
}

DWORD WINAPI SendThread( LPVOID lpData )
{
    SOCKET socket_client = (SOCKET)lpData;
    while( 1 )
    {
        printf("전송할 내용 : ");
        DWORD dwTmp;
        char text[1024]={0};
        ReadFile(GetStdHandle(STD_INPUT_HANDLE), text, 1024, &dwTmp, NULL);
        send( socket_client, text, strlen(text), 0 );
    }
    return 0;
}