#define _CRT_SECURE_NO_WARNINGS

#include	<winsock2.h> /*소켓 통신에 필요한 함수들이 선언되어있는 헤더파일 */
#include	<stdio.h>
#include    <conio.h>

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")	/* 윈도우 소켓 함수가 정의되어있다. */
#define BUFFER_SIZE 100  /* 100 바이트 */

char exchange(char* fromClient, char* toClient);

int main() {

	printf("[[[ Server ]]]\n");

	/* WSAStartup 함수를 이용해서 윈도우 소켓이 정의되어 있는 WS2_32.DLL 파일을 초기화합니다.
	   초기화가 성공하면 0을 리턴합니다.
	   WS2_32.dll 파일을 메모리에 업로드한다.
	  형식:
		int WSAStartup ( WORD wVersionRequested, LPWSADATA lpWSAData);
		WORD 윈도우 소켓의 버젼
		LPWSADATA 윈도우 소켓의 구현에 관한 정보를 저장할 포인터
	*/

	WSADATA wsdata;	// 윈도우 소켓에 대한 정보가 저장되는 포인터
	//MAKEWORK(2, 2) 는 윈도우 소켓 2.2 버전을 사용한다는 뜻이다.
	if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) {
		/*WS_32.DLL 을 초기화 하는데 실패 했을 경우 에러메시지를 출력하고
		프로그램을 종료합니다. */
		printf("WS2_32.DLL 을 초기화 하는데 실패했습니다.\n");
		return 0;
	}
	printf("1. WS2_32.DLL 을 초기화 하는데 성공 했습니다.\n");

	/*socket 함수를 이용해서 serverSocket을 생성합니다.
	  serverSocket의 역할은 클라이언트가 접속했는지 감시하고 클라이언트가
	  접속한 경우에 클라이언트와 데이터를 주고 받는 일을 전담하는 스트림, 즉 socket를 생성합니다.
	  형식 :
	  SOCKET socket (int af, int type, int protocol)
		  int 소켓의 프로토콜 체계를 설정합니다.
			  인터넷 주소체계의 TCP,UDP 의 경우 PF_INET 으로 설정합니다.
		  int 소켓 타입을 설정합니다. TCP는 SOCK_STREAM 이고 UDP는 SOCK_DGRAM이다.
		  int 소켓의 프로토콜을 설정합니다. 0 은 운영체제가 자동으로 소켓 타입에 맞는 프로토콜을 선택해줌


		  (참고) 프로토콜 체계(Protocol Family)  정의
			PF_INET   IPv4인터넷 프로토콜
			PF_INET6  IPv6인터넷 프로토콜
			PF_LOCAL  LOCAL 통신을 위한 UNIX 프로토콜
			PF_PACKET Low level socket을 위한 인터페이스
			PF_IPX    IPX 노벨 프로토콜

	*/

	/*소켓의 핸들 (파일 디스크립터)를 serverSocket에 대입*/
	SOCKET serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	/*ZeroMemory 함수는 해당 메모리를 0으로 초기화 시켜주는 함수입니다.
	 serverAddress 배열을 0으로 초기화 시켰습니다. */
	SOCKADDR_IN serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	/*serverAddress는 서버소켓의 주소정보를 저장하는 구조체 입니다.
	  serverAddress에 필요한 주소 정보를 저장 합니다.*/
	serverAddress.sin_family = AF_INET; /*소켓의 주소 체계를 설정 합니다.
										  TCP/IP 프로토콜의 경우 인터넷 기반이므로 AF_INET 을 대입합니다.
		(참고)주소체계(Address Family) 정의
		   AF_INET  IPv4인터넷 프로토콜, 내부적으로 2로 설정됨
		   AF_INET6  IPv6인터넷 프로토콜, 내부적으로 6으로 설정됨
		   AF_LOCAL  LOCAL 통신을 위한 UNIX 프로토콜*/

	serverAddress.sin_port = htons(10000); /*서버 소켓의 포트를 설정합니다.  */
	serverAddress.sin_addr.s_addr = htonl(ADDR_ANY);/*서버의 IP를 설정합니다.*/
	/*ADDR_ANY는 모든 클라이언트로부터의 접속을 받아들임을 의미 (Connection will be accepted on any Internet interface on a system having multiple IP addresses)*/
	/*즉, 자동으로 서버에 사용가능한 모든 랜카드의 IP주소를 사용하라는 의미*/
	/*예를 들어, 서버에 2개의 랜카드가 설치되어 있고 각각의 ip 주소가 192.168.63.1, 192.168.63.2 라 가정할 때*/
	/*외부에서 192.168.63.1로 데이터를 보내나 192.168.63.2로 데이터를 보내나 모두 처리하도록 함*/

	/* htons, htonl
   CPU(인텔/CISC, Sparc/RISC) 마다 두 바이트 이상의 값을 저장하는 방식 (즉, 호스트 바이트 순서)이 다름
   16진수 값 0x1234을 작은 단위의 값부터 0x34, 0x12 순서로 저장하는 Little-Endian (인텔/CISC)을 사용하는 시스템이 있는가 하면,
   반대로 큰 단위의 값부터 0x12, 0x34식으로 저장하는 Big_Endian(Sparc/RISC)을 사용하는 시스템이 있음
   네트워크 바이트 순서는 2 바이트 이상의 큰 숫자에 대해 어떤 바이트부터 전송할지에 대한 순서로 빅엔디안 방식을 사용함
   서로 다른 시스템 간에 혼란 발생을 방지함. 따라서, 인텔 계열 CPU에서는 htons, htonl를 써서 리틀엔티안->빅엔디안, 빅엔디안->리틀엔디안으로
   처리해 주어야 함
   port번호는 short:2바이트
   ip주소는 long: 4바이트*/

   /*bind 함수는 serverAddress 에 대입한 서버소켓의 주소정보를 이용해서
	 serverSocket에 IP 와 PORT를 부여합니다.  serverSocket에 IP 와 port를 부여하는데
   성공하면 0을, 실패하면 SOCKET_ERROR 를 리턴합니다.

	 형식:
	  bind(SOCKET s, const struct sockaddr FAR* name, int namelen)
		  SOCKET IP와 PORT를 부여할 소켓의 핸들
		  struct 소켓의 IP와 PORT에 관한 정보를 저장하고 있는 SOCKADDR_IN 구조체의 포인터
		  int    SOCKADDR_IN 구조체의 길이
   */
	if (bind(serverSocket, (SOCKADDR*)&serverAddress,
		sizeof(serverAddress)) == SOCKET_ERROR)
	{
		printf("serverSocket에 IP 와 PORT 를 부여 하는데 실패했습니다!!!\n");
		return 0;
	}
	printf("2. serverSocket 에 IP 와 Port 를 부여 하는데 성공 했습니다.\n");

	/*listen 함수는 클라이언트가 serverSocket에 부여한 IP 와 PORT로 접속했는지를
	  감시하는 함수 입니다. 클라이언트가 접속할때까지 이 프로그램은 여기서 클라이언트를 기다리고 있습니다.

	  형식:
	   listen(SOCKET s, int backlog)
		   SOCKET 클라이언트의 접속을 기다리는 소켓 객체
		   int 동시에 접속가능한 클라이언트의 수; SOMAXCONN 의 경우 동시 접속가능한 클라이언트의 수를 무제한으로 설정 했습니다.
		 */
	printf("3. 클라이언트의 접속을 기다립니다.\n"); /*클라이언트로부터 접속받을 준비가 끝남*/


	listen(serverSocket, SOMAXCONN);
	/*클라이언트가 서버에 접속하면 프로그램은 이부분으로 내려옵니다. */

	SOCKET socket;

	/* accept 함수는 클라이언트의 접속을 허락하고 클라이언트와 데이터를 주고 받는 일을 전담하는
	  socket을 리턴합니다.

	  형식:
	   accept(SOCKET s, struct sockaddr FAR* addr, int FAR* addrlen)
		   SOCKET 접속을 허락할 서버 소켓의 핸들
		   struct 클라이언트의 주소 정보를 저장할 SOCKADDR_IN 구조체의 주소; 클라이언트의 주소 정보를 굳이 알 필요가 없을 때는 NULL로 설정
		   int 클라이언트의 주소 정보를 저장할 SOCKADDR_IN 구조체의 길이; 클라이언트의 주소 정보를 굳이 알 필요가 없을 때는 NULL로 설정
		 */
	if ((socket = accept(serverSocket, NULL, NULL)) == INVALID_SOCKET)
	{
		printf("클라이언트와 데이터를 주고 받는 socket을 생성 할 수 없습니다.");
		return 0;
	}
	printf("!! 클라이언트가 접속했습니다. 메세지를 기다리는 중......\n\n");
	char fromClient[BUFFER_SIZE + 1]; //클라이언트로 부터 전달된 내용을 저장할 배열

	/* recv 함수는 클라이언트에서 send 함수를 이용해서 서버로 전달한 데이터를 읽어들입니다.

	  형식:
	   recv(SOCKET s, char FAR* addr, int len, int flags)
		   SOCKET 클라이언트의 데이터를 전담해서 가져오고 보내는 소켓의 핸들
		   char   클라이언트의 데이터를 저장할 배열
		   int    읽어들일 데이터의 크기
		   int    데이터를 읽어들일 때의 옵셥으로 대부분 0으로 설정
		 */

		 /*클라이언트가 전달한 데이터를 읽어 들여서 fromClient 에 저장 합니다.*/
	recv(socket, fromClient, BUFFER_SIZE, 0);
	/*fromClient 에 저장된 내용을 출력합니다.*/
	printf("클라이언트로 부터 전달받은 데이터: %s\n\n", fromClient);
	char c;
	char filename[100];
	int index = 0;
	
	for (int i = 0; fromClient[i] != '\0' && fromClient[i] != ' '; ++i) {
		filename[index++] = fromClient[i];
	}
	filename[index] = '\0';

	char output[100];
	int start = 0;
	int shiftValue = 3; // 학번 끝자리
	
	
	for (int i = index+1; fromClient[i] != '\0'; i++) {
		output[start++] = fromClient[i] + shiftValue;
	}
	output[start] = '\0';

	FILE* fp = fopen(filename, "w");
	fprintf(fp, "%s", output);
	fclose(fp);


	/*toClient 배열에 저장된 내용을 클라이언트로 전달합니다.*/

	/* send 함수는 클라이언트로 데이터를 전송하는 함수입니다.

	  형식:
	   send((SOCKET s, const char FAR* buf, int len, int flags)
		   SOCKET 클라이언트의 데이터를 전담해서 가져오고 보내는 소켓의 핸들
		   char   클라이언트로 보낼 데이터를 저장하고 있는 배열
		   int    보낼 데이터의 크기
		   int    데이터를 보낼 때의 옵셥으로 대부분 0으로 설정 )
		 */
	send(socket, output, strlen(output) + 1, 0);
	printf("클라이언트로 전달한 메세지 : %s\n", output);

	_getch();

	/*클라이언트와의 연결을 종료합니다.*/
	closesocket(socket);
	printf("클라이언트와의 접속을 종료 했습니다.\n");
	/*WS_32.DLL 사용을 종료 합니다.*/
	WSACleanup();	// ws2_32.dll 파일을 메모리에서 삭제한다.
	printf("WS2_32.DLL 의 사용을 종료합니다.\n");
	return 1;
}

char exchange(char* fromClient, char* toClient) {
	int i;
	char num = 0;

	for (i = 0; i < 100; i++) {
		toClient[i] = fromClient[i];
		if (fromClient[i] == '\0')
			break;
		num++;
	}
	for (i = 0; i < num; i++) {
		if (fromClient[i] >= 97)
			toClient[i] = fromClient[i] - 32;	/* 소문자'a'= 97, 대문자'A'=65 */
	}
	return num;
}

