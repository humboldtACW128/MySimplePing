#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <iphlpapi.h>
#include <icmpapi.h>
#include <string>
#include <iostream>
#include <climits>

//Must link to the Iphlpapi.lib file
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,PSTR lpCmdLine, int nCmdShow)
{
    HANDLE hICMPFile;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[32] = "Data Buffer";
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;

    hICMPFile = IcmpCreateFile();
    if(hICMPFile == INVALID_HANDLE_VALUE)
    {
        // cout << "Error: IcmpCreateFile" << endl;
        printf("Error: IcmpCreatefile returned error code: %ld\n", GetLastError());
        return 1;
    }
    else {
        string ip = "";
        while(ip != "exit") {
            auto data = SendData;
            printf("You may enter \"exit\" to quit the program.\n");
            printf("Enter IP address: ");
            cin >> ip;
            if(ip == "exit") {
                break;
            }
            cout << "Sending ICMP echo request to " << ip << endl;
            ipaddr = inet_addr(ip.c_str());
            printf("IcmpCreateFile returned handle: %ld\n", hICMPFile);
            ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(data);
            ReplyBuffer = (VOID*) malloc(ReplySize);
            if(ReplyBuffer == NULL) {
                printf("\tUnable to allocate memory for reply buffer.\n");
                return 1;
            }
            IcmpSendEcho(hICMPFile,ipaddr,SendData,sizeof(data),NULL,ReplyBuffer,ReplySize,1000);
            PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
            if(pEchoReply->Status == 0) {
                printf("Received ICMP echo reply with status:%ld\n", pEchoReply->Status);
            }
            else {
                if(pEchoReply->Status == 11010) {
                    printf("Received ICMP timeout with status: %ld\n", pEchoReply->Status);
                }
                else {
                    printf("Recieved unchecked ICMP status with: %ld\n",pEchoReply->Status);
                }
            }
        }
        IcmpCloseHandle(hICMPFile);
    }
    printf("Press any key to continue...\n");
    cin.ignore(1);
    cin.get();
    return 0;
}