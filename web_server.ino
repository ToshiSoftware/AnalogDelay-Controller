// callback from Arduino web-server lib

#include "web_server.h"
#include "myUtil.h"

char htmlbody[32768];

// create html message
void CreateIndexHtml(void)
{
  char tmpstr[256];
      strcpy(htmlbody, index_html1);
      for(int i=0;i<NUM_PRESET;i++)
      {
        sprintf(tmpstr, "document.getElementById('e%d').value = %d;\n", i, presetParams[i].effect);
        strcat(htmlbody, tmpstr);
        sprintf(tmpstr, "document.getElementById('f%d').value = %d;\n", i, presetParams[i].feedback);
        strcat(htmlbody, tmpstr);
        sprintf(tmpstr, "document.getElementById('t%d').value = %d;\n\n", i, presetParams[i].time);
        strcat(htmlbody, tmpstr);
      }
      strcat(htmlbody, index_html3);
      if(debug_output_to_html){
        strcat(htmlbody, "<br />\n");
        strcat(htmlbody, debug_message);
        strcat(htmlbody, "<br />\n");
      }
      strcat(htmlbody, index_html4);
}

// callback when root url accessed
//#define HELLO_MESSAGE "Hello from esp32"
void handleRoot(void) {
  char uriStr[256];
  // exit
  if(appVars.available_wifi==true)
  {
    httpServer.uri().toCharArray(uriStr, 255);
    if(strcmp(uriStr, "/")==0){
      // send index.html
      CreateIndexHtml();
      httpServer.send(200, "text/html", htmlbody);
    }
  }
}


void handleSetVol(void) {
  int changeCount=0;
  char uriStr[256];

  // check uri
  httpServer.uri().toCharArray(uriStr, 255);
  if(strcmp(uriStr, "/preset")==0){
    if(appVars.available_wifi==true)
    {
      for (uint8_t i = 0; i < httpServer.args(); i++) {
        //  message += " " + httpServer.argName(i) + ": " + httpServer.arg(i) + "\n";
        char name[32]; 
        httpServer.argName(i).toCharArray(name, 31);
        if(strlen(name)==2){
          // check if the 1st letter is e/f/t
          if(name[0]=='e' || name[0]=='f' || name[0]=='t'){
            // check if the 2nd letter is between 0 to 9
            if(name[1]>='0' && name[1]<='9'){
              int index = name[1]-'0';
              if(index>=0 && index<=9)
              {
                int value = httpServer.arg(i).toInt();
                if( value>=0 && value<=99){
                  if(name[0]=='e') presetParams[index].effect = value;
                  else if(name[0]=='f') presetParams[index].feedback = value;
                  else if(name[0]=='t') presetParams[index].time = value;
                  changeCount++;
                }
              }
            }
          }
        }
      }
      
      // verify preset value
      CheckAndCorrectPreset();

      // send index html
      CreateIndexHtml();
      httpServer.send(200, "text/html", htmlbody);

      // if any preset is changed
      if(changeCount){
        // update html
        appVars.changed_via_wifi = true;

        // save to eeprom
        save_eeprom();
        // LCD
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Presets replaced");
        lcd.setCursor(0,1);
        lcd.print("via WiFi.");
        delay(3000);
      }
    }
  }
  else{
      CreateIndexHtml();
      httpServer.send(200, "text/html", htmlbody);
  }
}

/*
// callback when other uri accessed
void handleSetVol(void) {
  // exit
  if(appVars.available_wifi==true)
  {
    //
    String message = "Receive params..\n\n";
    message += "URI: ";
    message += httpServer.uri();
    message += "\nMethod: ";
    message += (httpServer.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += httpServer.args();
    message += "\n";
    for (uint8_t i = 0; i < httpServer.args(); i++) {
      message += " " + httpServer.argName(i) + ": " + httpServer.arg(i) + "\n";
    }
    httpServer.send(404, "text/plain", message);

    // LCD
    String lcdMessage = "";
    //lcd.clear();
    EraceLCD(0,0,8);
    lcd.setCursor(0, 0);
    //for (uint8_t i = 0; i < httpServer.args(); i++) {
    for (uint8_t i = 0; i < 2; i++) {
      //lcdMessage += httpServer.argName(i);
      //lcdMessage += "=";
      char tmp[128];
      httpServer.argName(i).toCharArray(tmp, 127);
      if(strcmp(tmp, "sn")!=0){
        lcdMessage += httpServer.arg(i);
        lcdMessage += ",";
      }
    }
    lcd.print(lcdMessage);
  }
}
*/