#define _CRT_SECURE_NO_WARNINGS

#include	<winsock2.h> /*���� ��ſ� �ʿ��� �Լ����� ����Ǿ��ִ� ������� */
#include	<stdio.h>
#include    <conio.h>

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")	/* ������ ���� �Լ��� ���ǵǾ��ִ�. */
#define BUFFER_SIZE 100  /* 100 ����Ʈ */

char exchange(char* fromClient, char* toClient);

int main() {

	printf("[[[ Server ]]]\n");

	/* WSAStartup �Լ��� �̿��ؼ� ������ ������ ���ǵǾ� �ִ� WS2_32.DLL ������ �ʱ�ȭ�մϴ�.
	   �ʱ�ȭ�� �����ϸ� 0�� �����մϴ�.
	   WS2_32.dll ������ �޸𸮿� ���ε��Ѵ�.
	  ����:
		int WSAStartup ( WORD wVersionRequested, LPWSADATA lpWSAData);
		WORD ������ ������ ����
		LPWSADATA ������ ������ ������ ���� ������ ������ ������
	*/

	WSADATA wsdata;	// ������ ���Ͽ� ���� ������ ����Ǵ� ������
	//MAKEWORK(2, 2) �� ������ ���� 2.2 ������ ����Ѵٴ� ���̴�.
	if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) {
		/*WS_32.DLL �� �ʱ�ȭ �ϴµ� ���� ���� ��� �����޽����� ����ϰ�
		���α׷��� �����մϴ�. */
		printf("WS2_32.DLL �� �ʱ�ȭ �ϴµ� �����߽��ϴ�.\n");
		return 0;
	}
	printf("1. WS2_32.DLL �� �ʱ�ȭ �ϴµ� ���� �߽��ϴ�.\n");

	/*socket �Լ��� �̿��ؼ� serverSocket�� �����մϴ�.
	  serverSocket�� ������ Ŭ���̾�Ʈ�� �����ߴ��� �����ϰ� Ŭ���̾�Ʈ��
	  ������ ��쿡 Ŭ���̾�Ʈ�� �����͸� �ְ� �޴� ���� �����ϴ� ��Ʈ��, �� socket�� �����մϴ�.
	  ���� :
	  SOCKET socket (int af, int type, int protocol)
		  int ������ �������� ü�踦 �����մϴ�.
			  ���ͳ� �ּ�ü���� TCP,UDP �� ��� PF_INET ���� �����մϴ�.
		  int ���� Ÿ���� �����մϴ�. TCP�� SOCK_STREAM �̰� UDP�� SOCK_DGRAM�̴�.
		  int ������ ���������� �����մϴ�. 0 �� �ü���� �ڵ����� ���� Ÿ�Կ� �´� ���������� ��������


		  (����) �������� ü��(Protocol Family)  ����
			PF_INET   IPv4���ͳ� ��������
			PF_INET6  IPv6���ͳ� ��������
			PF_LOCAL  LOCAL ����� ���� UNIX ��������
			PF_PACKET Low level socket�� ���� �������̽�
			PF_IPX    IPX �뺧 ��������

	*/

	/*������ �ڵ� (���� ��ũ����)�� serverSocket�� ����*/
	SOCKET serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	/*ZeroMemory �Լ��� �ش� �޸𸮸� 0���� �ʱ�ȭ �����ִ� �Լ��Դϴ�.
	 serverAddress �迭�� 0���� �ʱ�ȭ ���׽��ϴ�. */
	SOCKADDR_IN serverAddress;
	ZeroMemory(&serverAddress, sizeof(serverAddress));
	/*serverAddress�� ���������� �ּ������� �����ϴ� ����ü �Դϴ�.
	  serverAddress�� �ʿ��� �ּ� ������ ���� �մϴ�.*/
	serverAddress.sin_family = AF_INET; /*������ �ּ� ü�踦 ���� �մϴ�.
										  TCP/IP ���������� ��� ���ͳ� ����̹Ƿ� AF_INET �� �����մϴ�.
		(����)�ּ�ü��(Address Family) ����
		   AF_INET  IPv4���ͳ� ��������, ���������� 2�� ������
		   AF_INET6  IPv6���ͳ� ��������, ���������� 6���� ������
		   AF_LOCAL  LOCAL ����� ���� UNIX ��������*/

	serverAddress.sin_port = htons(10000); /*���� ������ ��Ʈ�� �����մϴ�.  */
	serverAddress.sin_addr.s_addr = htonl(ADDR_ANY);/*������ IP�� �����մϴ�.*/
	/*ADDR_ANY�� ��� Ŭ���̾�Ʈ�κ����� ������ �޾Ƶ����� �ǹ� (Connection will be accepted on any Internet interface on a system having multiple IP addresses)*/
	/*��, �ڵ����� ������ ��밡���� ��� ��ī���� IP�ּҸ� ����϶�� �ǹ�*/
	/*���� ���, ������ 2���� ��ī�尡 ��ġ�Ǿ� �ְ� ������ ip �ּҰ� 192.168.63.1, 192.168.63.2 �� ������ ��*/
	/*�ܺο��� 192.168.63.1�� �����͸� ������ 192.168.63.2�� �����͸� ������ ��� ó���ϵ��� ��*/

	/* htons, htonl
   CPU(����/CISC, Sparc/RISC) ���� �� ����Ʈ �̻��� ���� �����ϴ� ��� (��, ȣ��Ʈ ����Ʈ ����)�� �ٸ�
   16���� �� 0x1234�� ���� ������ ������ 0x34, 0x12 ������ �����ϴ� Little-Endian (����/CISC)�� ����ϴ� �ý����� �ִ°� �ϸ�,
   �ݴ�� ū ������ ������ 0x12, 0x34������ �����ϴ� Big_Endian(Sparc/RISC)�� ����ϴ� �ý����� ����
   ��Ʈ��ũ ����Ʈ ������ 2 ����Ʈ �̻��� ū ���ڿ� ���� � ����Ʈ���� ���������� ���� ������ �򿣵�� ����� �����
   ���� �ٸ� �ý��� ���� ȥ�� �߻��� ������. ����, ���� �迭 CPU������ htons, htonl�� �Ἥ ��Ʋ��Ƽ��->�򿣵��, �򿣵��->��Ʋ���������
   ó���� �־�� ��
   port��ȣ�� short:2����Ʈ
   ip�ּҴ� long: 4����Ʈ*/

   /*bind �Լ��� serverAddress �� ������ ���������� �ּ������� �̿��ؼ�
	 serverSocket�� IP �� PORT�� �ο��մϴ�.  serverSocket�� IP �� port�� �ο��ϴµ�
   �����ϸ� 0��, �����ϸ� SOCKET_ERROR �� �����մϴ�.

	 ����:
	  bind(SOCKET s, const struct sockaddr FAR* name, int namelen)
		  SOCKET IP�� PORT�� �ο��� ������ �ڵ�
		  struct ������ IP�� PORT�� ���� ������ �����ϰ� �ִ� SOCKADDR_IN ����ü�� ������
		  int    SOCKADDR_IN ����ü�� ����
   */
	if (bind(serverSocket, (SOCKADDR*)&serverAddress,
		sizeof(serverAddress)) == SOCKET_ERROR)
	{
		printf("serverSocket�� IP �� PORT �� �ο� �ϴµ� �����߽��ϴ�!!!\n");
		return 0;
	}
	printf("2. serverSocket �� IP �� Port �� �ο� �ϴµ� ���� �߽��ϴ�.\n");

	/*listen �Լ��� Ŭ���̾�Ʈ�� serverSocket�� �ο��� IP �� PORT�� �����ߴ�����
	  �����ϴ� �Լ� �Դϴ�. Ŭ���̾�Ʈ�� �����Ҷ����� �� ���α׷��� ���⼭ Ŭ���̾�Ʈ�� ��ٸ��� �ֽ��ϴ�.

	  ����:
	   listen(SOCKET s, int backlog)
		   SOCKET Ŭ���̾�Ʈ�� ������ ��ٸ��� ���� ��ü
		   int ���ÿ� ���Ӱ����� Ŭ���̾�Ʈ�� ��; SOMAXCONN �� ��� ���� ���Ӱ����� Ŭ���̾�Ʈ�� ���� ���������� ���� �߽��ϴ�.
		 */
	printf("3. Ŭ���̾�Ʈ�� ������ ��ٸ��ϴ�.\n"); /*Ŭ���̾�Ʈ�κ��� ���ӹ��� �غ� ����*/


	listen(serverSocket, SOMAXCONN);
	/*Ŭ���̾�Ʈ�� ������ �����ϸ� ���α׷��� �̺κ����� �����ɴϴ�. */

	SOCKET socket;

	/* accept �Լ��� Ŭ���̾�Ʈ�� ������ ����ϰ� Ŭ���̾�Ʈ�� �����͸� �ְ� �޴� ���� �����ϴ�
	  socket�� �����մϴ�.

	  ����:
	   accept(SOCKET s, struct sockaddr FAR* addr, int FAR* addrlen)
		   SOCKET ������ ����� ���� ������ �ڵ�
		   struct Ŭ���̾�Ʈ�� �ּ� ������ ������ SOCKADDR_IN ����ü�� �ּ�; Ŭ���̾�Ʈ�� �ּ� ������ ���� �� �ʿ䰡 ���� ���� NULL�� ����
		   int Ŭ���̾�Ʈ�� �ּ� ������ ������ SOCKADDR_IN ����ü�� ����; Ŭ���̾�Ʈ�� �ּ� ������ ���� �� �ʿ䰡 ���� ���� NULL�� ����
		 */
	if ((socket = accept(serverSocket, NULL, NULL)) == INVALID_SOCKET)
	{
		printf("Ŭ���̾�Ʈ�� �����͸� �ְ� �޴� socket�� ���� �� �� �����ϴ�.");
		return 0;
	}
	printf("!! Ŭ���̾�Ʈ�� �����߽��ϴ�. �޼����� ��ٸ��� ��......\n\n");
	char fromClient[BUFFER_SIZE + 1]; //Ŭ���̾�Ʈ�� ���� ���޵� ������ ������ �迭

	/* recv �Լ��� Ŭ���̾�Ʈ���� send �Լ��� �̿��ؼ� ������ ������ �����͸� �о���Դϴ�.

	  ����:
	   recv(SOCKET s, char FAR* addr, int len, int flags)
		   SOCKET Ŭ���̾�Ʈ�� �����͸� �����ؼ� �������� ������ ������ �ڵ�
		   char   Ŭ���̾�Ʈ�� �����͸� ������ �迭
		   int    �о���� �������� ũ��
		   int    �����͸� �о���� ���� �ɼ����� ��κ� 0���� ����
		 */

		 /*Ŭ���̾�Ʈ�� ������ �����͸� �о� �鿩�� fromClient �� ���� �մϴ�.*/
	recv(socket, fromClient, BUFFER_SIZE, 0);
	/*fromClient �� ����� ������ ����մϴ�.*/
	printf("Ŭ���̾�Ʈ�� ���� ���޹��� ������: %s\n\n", fromClient);
	char c;
	char filename[100];
	int index = 0;
	
	for (int i = 0; fromClient[i] != '\0' && fromClient[i] != ' '; ++i) {
		filename[index++] = fromClient[i];
	}
	filename[index] = '\0';

	char output[100];
	int start = 0;
	int shiftValue = 3; // �й� ���ڸ�
	
	
	for (int i = index+1; fromClient[i] != '\0'; i++) {
		output[start++] = fromClient[i] + shiftValue;
	}
	output[start] = '\0';

	FILE* fp = fopen(filename, "w");
	fprintf(fp, "%s", output);
	fclose(fp);


	/*toClient �迭�� ����� ������ Ŭ���̾�Ʈ�� �����մϴ�.*/

	/* send �Լ��� Ŭ���̾�Ʈ�� �����͸� �����ϴ� �Լ��Դϴ�.

	  ����:
	   send((SOCKET s, const char FAR* buf, int len, int flags)
		   SOCKET Ŭ���̾�Ʈ�� �����͸� �����ؼ� �������� ������ ������ �ڵ�
		   char   Ŭ���̾�Ʈ�� ���� �����͸� �����ϰ� �ִ� �迭
		   int    ���� �������� ũ��
		   int    �����͸� ���� ���� �ɼ����� ��κ� 0���� ���� )
		 */
	send(socket, output, strlen(output) + 1, 0);
	printf("Ŭ���̾�Ʈ�� ������ �޼��� : %s\n", output);

	_getch();

	/*Ŭ���̾�Ʈ���� ������ �����մϴ�.*/
	closesocket(socket);
	printf("Ŭ���̾�Ʈ���� ������ ���� �߽��ϴ�.\n");
	/*WS_32.DLL ����� ���� �մϴ�.*/
	WSACleanup();	// ws2_32.dll ������ �޸𸮿��� �����Ѵ�.
	printf("WS2_32.DLL �� ����� �����մϴ�.\n");
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
			toClient[i] = fromClient[i] - 32;	/* �ҹ���'a'= 97, �빮��'A'=65 */
	}
	return num;
}

