/* SpwfInterface NetworkSocketAPI Example Program
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "SpwfInterface.h"
#include "TCPSocket.h"



//------------------------------------
// Hyperterminal configuration
// 9600 bauds, 8-bit data, no parity
//------------------------------------

Serial pc(USBTX, USBRX);
DigitalOut myled(LED1);

/*************************************
//FRDM-K64: D9->UART1_TX, D7->UART1_RX
Pin connections:
    FRDM      IDW01M1
   ------    ---------
    +3v3 <--> +3v3
    GND  <--> GND
    D9   <--> D8
    D7   <--> D2

SpwfSAInterface spwf(D9, D7, false);
*************************************/
/*************************************
//LPCXpresso11U68: D9->UART1_TX, D7->UART1_RX
Pin connections:
    LPC      IDW01M1
   ------    ---------
    +3v3 <--> +3v3
    GND  <--> GND
    A1   <--> D8
    A2   <--> D2

SpwfSAInterface spwf(A1, A2, false);
*************************************/

//NUCLEO: D8->UART1_TX (PA_9), D2->UART1_RX (PA_10)
SpwfSAInterface spwf(D8, D2, false);

int main() {
    int err;    
    char * ssid = "STM";
    char * seckey = "STMDemo";  
    
    pc.printf("\r\nX-NUCLEO-IDW01M1 mbed Application\r\n");     
    pc.printf("\r\nconnecting to AP\r\n");
            
    if(spwf.connect(ssid, seckey, NSAPI_SECURITY_WPA2)) {      
        pc.printf("\r\nnow connected\r\n");
    } else {
        pc.printf("\r\nerror connecting to AP.\r\n");
        return -1;
    }   

    const char *ip = spwf.get_ip_address();
    const char *mac = spwf.get_mac_address();
    
    pc.printf("\r\nIP Address is: %s\r\n", (ip) ? ip : "No IP");
    pc.printf("\r\nMAC Address is: %s\r\n", (mac) ? mac : "No MAC");    
    
    SocketAddress addr(&spwf, "st.com");   
    pc.printf("\r\nst.com resolved to: %s\r\n", addr.get_ip_address());    

    pc.printf("\r\nconnecting to http://4.ifcfg.me\r\n");
    
    TCPSocket socket(&spwf);
    err = socket.connect("4.ifcfg.me", 23);
    if(err!=0) 
    {
      pc.printf("\r\nCould not connect to Socket, err = %d!!\r\n", err); 
      return -1;
    } else pc.printf("\r\nconnected to host server\r\n"); 
    
    char buffer[100];
    int count = 0;
    pc.printf("\r\nReceiving Data\r\n"); 
    count = socket.recv(buffer, sizeof buffer);
    
    if(count > 0)
    {
        buffer [count]='\0';
        printf("%s\r\n", buffer);  
    }
    else pc.printf("\r\nData not received\r\n");

    pc.printf("\r\nClosing Socket\r\n");
    socket.close();
    pc.printf("\r\nUnsecure Socket Test complete.\r\n");
    printf ("Socket closed\n\r");
    spwf.disconnect();
    printf ("WIFI disconnected, exiting ...\n\r");

    while(1) { 
      wait(1);
      myled = !myled;
    }
}
