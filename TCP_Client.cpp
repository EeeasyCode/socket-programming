#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>
#include <conio.h>
#include <string.h>
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")
#define BUFFER_SIZE 100


int main(void) {
	SOCKET s;					// ���� ��ũ����
	WSADATA wsaData;
	struct sockaddr_in	sin;	// ���� ����ü

	printf("[[[ Client ]]]\n");

	if (WSAStartup(WINSOCK_VERSION, &wsaData) != 0) {
		printf("WSAStartup ����, ���� �ڵ� = %d \n", WSAGetLastError());

		return 0;
	}

	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s == INVALID_SOCKET) {
		printf("���� ���� ����, �����ڵ� : %d \n", WSAGetLastError());
		WSACleanup();  return 0;
	}

	sin.sin_family = AF_INET;						// �ּ� ü�� ����
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");		// ���� �ּ� ����, inet_addr() �Լ��� ���ڷ� �� string Ÿ���� �����͸� �� ����� ������ ���������� ��ȯ
	sin.sin_port = htons(10000);				// ��Ʈ ��ȣ ����

		/* connect �Լ��� Ŭ���̾�Ʈ�� ������ �����ϰ� �ϴ� �Լ� �Դϴ�.
		   ������ ���� ������ 0, ���н� SOCKET_ERROR�� �����մϴ�.

	  ����:
	   connect(SOCKET s, const char sockaddr FAR* name, int namelen)
		   SOCKET ������ �޽����� �ְ� ���� ����
		   struct ������ �ּ� ����
		   int ���� �ּ� ������ ����
		 */


	if (connect(s, (struct sockaddr*)&sin, sizeof(sin)) != 0) {
		printf("���� ����, ���� �ڵ� =  %u \n", WSAGetLastError());
		closesocket(s);  WSACleanup();  return 0;
	}

	printf("127.0.0.1�� 10000�� ��Ʈ�� ������ �����Ͽ����ϴ�.\n\n");

	char toServer[BUFFER_SIZE];
	char textfile[40];
	char filetext[BUFFER_SIZE] = "";
	char s2[10] = ".txt";
	char space[] = " ";

	printf("open�� ���ϸ� : ");
	gets_s(textfile, 40);	// Ű����� ���� ������ �Է� �޾� toServer �迭�� �ִ´�.
	strcat(textfile, s2);
	FILE* fp; // ����  
	fp = fopen(textfile, "rb");
	if (fp == NULL) {
		printf(textfile);
		printf("Cannot open this file. \n");
		return 1;
	}
	strcat(textfile, space);
	fscanf(fp, "%s", toServer);
	strcat(filetext, textfile);
	strcat(filetext, toServer);
	fclose(fp);

	send(s, filetext, strlen(filetext) + 1, 0);	// s��� ���Ͽ� toServer �迭�� ������ strlen(toServer)+1 ���̸�ŭ ������.
	
	char len[1];	// ������ ���� �ް� �� �޼����� ���̸� ������ �迭�̴�.
	char fromServer[BUFFER_SIZE + 1];
	recv(s, fromServer, BUFFER_SIZE, 0);
	printf("\n������ ���� ���� �޼��� : %s\n\n", fromServer);
	
	_getch();

	printf("\n\n�������� ������ ���� �߽��ϴ�.\n");

	if (closesocket(s) != 0) {
		printf("���� ���� ����, ���� �ڵ� = %u \n", WSAGetLastError());
		WSACleanup();  return 0;
	}

	if (WSACleanup() != 0) { //WS2_32.DLL�� �޸𸮿��� ����; ���������� ������ 0, ���н� SOCKET_ERROR�� ����
		printf("WSACleanup ����, ���� �ڵ� = %u \n", WSAGetLastError());
		return 0;
	}
}
