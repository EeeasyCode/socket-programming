#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#include <stdio.h>
#include <winsock2.h>
#include <conio.h>

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")
#define BUFFER_SIZE 100


int main(void) {
	SOCKET s;					// 소켓 디스크립터
	WSADATA wsaData;
	struct sockaddr_in	sin;	// 소켓 구조체

	printf("[[[ Client ]]]\n");

	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0) {
		printf("WSAStartup 실패, 에러 코드 = %d \n", WSAGetLastError());

		return 0;
	}

	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s == INVALID_SOCKET) {
		printf("소켓 생성 실패, 에러코드 : %d \n", WSAGetLastError());
		WSACleanup();  return 0;
	}

	sin.sin_family = AF_INET;						// 주소 체계 설정
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");		// 접속 주소 설정, inet_addr() 함수는 인자로 들어간 string 타입의 데이터를 빅 엔디안 형식의 정수값으로 반환
	sin.sin_port = htons(10000);				// 포트 번호 설정

		/* connect 함수는 클라이언트를 서버에 접속하게 하는 함수 입니다.
		   서버에 접속 성공시 0, 실패시 SOCKET_ERROR를 리턴합니다.

	  형식:
	   connect(SOCKET s, const char sockaddr FAR* name, int namelen)
		   SOCKET 서버와 메시지를 주고 받을 소켓
		   struct 서버의 주소 정보
		   int 서버 주소 정보의 길이
		 */


	if (connect(s, (struct sockaddr*)&sin, sizeof(sin)) != 0) {
		printf("접속 실패, 에러 코드 =  %u \n", WSAGetLastError());
		closesocket(s);  WSACleanup();  return 0;
	}

	printf("127.0.0.1의 10000번 포트에 접속을 성공하였습니다.\n\n");

	char toServer[BUFFER_SIZE];
	printf("서버로 보낼 메세지 : ");
	gets_s(toServer);	// 키보드로 부터 문장을 입력 받아 toServer 배열에 넣는다.
	send(s, toServer, strlen(toServer) + 1, 0);	// s라는 소켓에 toServer 배열의 내용을 strlen(toServer)+1 길이만큼 보낸다.

	char len[1];	// 서버로 부터 받게 될 메세지의 길이를 대입할 배열이다.
	char fromServer[BUFFER_SIZE + 1];
	recv(s, len, sizeof(char), 0);
	recv(s, fromServer, len[0], 0);
	printf("\n서버로 부터 받은 메세지 : ");
	for (int j = 0; j < len[0]; j++) {
		printf("%c", fromServer[j]);
	}
	printf("\n서버로 부터 받은 메세지 길이: %d\n", len[0]);

	_getch();

	printf("\n\n서버와의 접속을 종료 했습니다.\n");

	if (closesocket(s) != 0) {
		printf("소켓 제거 실패, 에러 코드 = %u \n", WSAGetLastError());
		WSACleanup();  return 0;
	}

	if (WSACleanup() != 0) { //WS2_32.DLL을 메모리에서 삭제; 성공적으로 삭제시 0, 실패시 SOCKET_ERROR을 리턴
		printf("WSACleanup 실패, 에러 코드 = %u \n", WSAGetLastError());
		return 0;
	}
}
