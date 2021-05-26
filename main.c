#include "simpletools.h"
#include "abdrive360.h"
#include "wifi.h"
#include "ping.h"

int event, id, handle;
int wsId, wsHandle = 0;
int navChar, buttonCmd, buttonCmdPrev; 
int val, speedVal, speedLeft, speedRight;
int pingDis;

int main()
{
  wifi_start(31, 30, 115200, WX_ALL_COM);

  wsId = wifi_listen(WS, "/ws/a");
  high(27);
  while(1)
  {
    wifi_poll(&event, &id, &handle); 
    high(26);
    if(event == 'W')
    {
      if(id == wsId)
      {
        wsHandle = handle;
        print("wsHandle = %d\n\n", wsHandle);
      }        
    }
    else if(event == 'D')
    {
      wifi_scan(WS, handle, "%c%d", &buttonCmd, &val);
      if(buttonCmd == 'v')
      {
        buttonCmd = buttonCmdPrev;
        speedVal = val;
      }        
      else 
      {
        buttonCmdPrev = buttonCmd;
      }        
    }       

    if(wsHandle != 0)
    {
      pingDis = ping_inches(10);
      wifi_print(WS, wsHandle, "{\"ping\": %01d, \"currentButton\": %01d, \"currentSpeed\": %01d}", pingDis, buttonCmdPrev, speedVal);
      //      wifi_print(WS, wsHandle, "%01b%01b", irLeft, irRight);
    }  

    if(buttonCmd != 0)
    {
      switch(buttonCmd)
      {
        case 'F': 
          speedLeft = speedVal;
          speedRight = speedVal;
          break;
        case 'L':
          speedLeft = -speedVal;
          speedRight = speedVal;
          break;
        case 'R':
          speedLeft = speedVal;
          speedRight = -speedVal;
          break;
        case 'B':
          speedLeft = -speedVal;
          speedRight = -speedVal;
          break;
        case 'S':
          speedLeft = 0;
          speedRight = 0;
          break;
      } 

      drive_speed(speedLeft, speedRight);
      if (buttonCmd == "L") {
        buttonCmd = 0;
        pause(1000);
        drive_speed(0,0);
      } else if (buttonCmd == "R") {
        buttonCmd = 0;
        pause(1000);
        drive_speed(0,0);
      } else {
        buttonCmd = 0;
      }
    }            
    
    low(26);
    pause(50);
  }    
}
