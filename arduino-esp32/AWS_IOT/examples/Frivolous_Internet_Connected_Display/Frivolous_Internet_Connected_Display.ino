/*
 * For detailed tutorial on building the project visit:
 * https://exploreembedded.com/wiki/Building_a_Frivolous_Internet_Connected_Display_with_ESP32
 * Install the libraries listed in the tutorial before compiling the code.

 * Reference links:

   https://exploreembedded.com/wiki/Secure_IOT_with_AWS_and_Hornbill_ESP32
   https://exploreembedded.com/wiki/AWS_IOT_with_Arduino_ESP32

 * Library Links:
   https://github.com/ExploreEmbedded/Hornbill-Examples/tree/master/arduino-esp32/AWS_IOT/examples
   https://github.com/adafruit/Adafruit_SSD1306
   https://github.com/adafruit/Adafruit-GFX-Library
 */

#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <jsonParser.h>
#include <AWS_IOT.h>


AWS_IOT AWS_CLIENT;
/************************************************************************
                             AWS Configuration
/************************************************************************/
char WIFI_SSID[]="your Wifi SSID";
char WIFI_PASSWORD[]="Wifi Password";
char HOST_ADDRESS[]="AWS host address";
char CLIENT_ID[]= "client id";
char TOPIC_NAME[]= "your thing/topic name";
/***********************************************************************/



/*************************************************************************
                           OLED
 *************************************************************************/
#define OLED_RESET 0xFF
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
/************************************************************************/




/**********************************************************************************************************
                                     32x32 Horizontal Logos
 ***********************************************************************************************************/
const uint8_t logoTwitter PROGMEM []= {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00,
        0x00, 0x00, 0x3F, 0xCE, 0x30, 0x00, 0x7F, 0xFC, 0x38, 0x00, 0xFF, 0xF8, 0x7C, 0x00, 0xFF, 0xFE,
        0x3E, 0x00, 0xFF, 0xFC, 0x3F, 0x80, 0xFF, 0xF8, 0x3F, 0xF0, 0xFF, 0xF8, 0x1F, 0xFF, 0xFF, 0xF8,
        0x2F, 0xFF, 0xFF, 0xF8, 0x3F, 0xFF, 0xFF, 0xF0, 0x3F, 0xFF, 0xFF, 0xF0, 0x3F, 0xFF, 0xFF, 0xF0,
        0x1F, 0xFF, 0xFF, 0xF0, 0x0F, 0xFF, 0xFF, 0xE0, 0x03, 0xFF, 0xFF, 0xE0, 0x0F, 0xFF, 0xFF, 0xC0,
        0x07, 0xFF, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0x80, 0x01, 0xFF, 0xFF, 0x00, 0x00, 0x7F, 0xFE, 0x00,
        0x00, 0xFF, 0xFC, 0x00, 0x07, 0xFF, 0xF8, 0x00, 0x7F, 0xFF, 0xE0, 0x00, 0x1F, 0xFF, 0x80, 0x00,
        0x01, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


const uint8_t logoFacebook PROGMEM [] = {
        0x0F, 0xFF, 0xFF, 0xF0, 0x3F, 0xFF, 0xFF, 0xFC, 0x7F, 0xFF, 0xFF, 0xFE, 0x7F, 0xFF, 0xF8, 0x1E,
        0xFF, 0xFF, 0xF0, 0x1E, 0xFF, 0xFF, 0xE0, 0x1F, 0xFF, 0xFF, 0xC0, 0x1F, 0xFF, 0xFF, 0xC0, 0x1F,
        0xFF, 0xFF, 0xC1, 0xFF, 0xFF, 0xFF, 0xC1, 0xFF, 0xFF, 0xFF, 0xC1, 0xFF, 0xFF, 0xFF, 0x00, 0x1F,
        0xFF, 0xFE, 0x00, 0x1F, 0xFF, 0xFE, 0x00, 0x1F, 0xFF, 0xFE, 0x00, 0x1F, 0xFF, 0xFE, 0x00, 0x1F,
        0xFF, 0xFF, 0xC1, 0xFF, 0xFF, 0xFF, 0xC1, 0xFF, 0xFF, 0xFF, 0xC1, 0xFF, 0xFF, 0xFF, 0xC1, 0xFF,
        0xFF, 0xFF, 0xC1, 0xFF, 0xFF, 0xFF, 0xC1, 0xFF, 0xFF, 0xFF, 0xC1, 0xFF, 0xFF, 0xFF, 0xC1, 0xFF,
        0xFF, 0xFF, 0xC1, 0xFF, 0xFF, 0xFF, 0xC1, 0xFF, 0x7F, 0xFF, 0xC1, 0xFE, 0x7F, 0xFF, 0xC1, 0xFE,
        0x3F, 0xFF, 0xFF, 0xFC, 0x1F, 0xFF, 0xFF, 0xF8, 0x07, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00
};

const uint8_t logoTime PROGMEM [] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x01, 0x00, 0x60, 0x00, 0x04, 0x08, 0x10, 0x00,
        0x08, 0x00, 0x08, 0x00, 0x10, 0x08, 0x04, 0x00, 0x00, 0x08, 0x02, 0x00, 0x20, 0x08, 0x00, 0x00,
        0x00, 0x08, 0x01, 0x00, 0x40, 0x08, 0x01, 0x80, 0x40, 0x08, 0x00, 0x80, 0x40, 0x08, 0x00, 0x80,
        0x42, 0x0F, 0xF2, 0x80, 0x40, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x80, 0x40, 0x00, 0x00, 0x00,
        0x40, 0x00, 0x00, 0x00, 0x20, 0x00, 0x01, 0x00, 0x20, 0x00, 0x03, 0x80, 0x10, 0x08, 0x02, 0x80,
        0x08, 0x00, 0x04, 0x80, 0x04, 0x00, 0x3C, 0x78, 0x02, 0x00, 0x20, 0x08, 0x00, 0x80, 0x10, 0x10,
        0x00, 0x3E, 0x08, 0x20, 0x00, 0x00, 0x08, 0x20, 0x00, 0x00, 0x11, 0x20, 0x00, 0x00, 0x14, 0xE0,
        0x00, 0x00, 0x18, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t logoTemperature PROGMEM [] = {
        0x06, 0x00, 0x0F, 0x00, 0x06, 0x00, 0x1F, 0x80, 0x1F, 0x87, 0xD9, 0x80, 0x1F, 0x80, 0x19, 0x80,
        0x06, 0x00, 0x19, 0x80, 0x06, 0x01, 0xD9, 0x80, 0x00, 0x00, 0x19, 0x80, 0x00, 0x07, 0xF9, 0x80,
        0x00, 0x00, 0x19, 0x80, 0x00, 0x01, 0xD9, 0x80, 0x00, 0x01, 0xDF, 0x80, 0x00, 0x00, 0x1F, 0x80,
        0x00, 0x07, 0xFF, 0x80, 0x00, 0x00, 0x1F, 0x80, 0x00, 0x01, 0xDF, 0x80, 0x00, 0x00, 0x1F, 0x80,
        0x1F, 0x80, 0x1F, 0x80, 0x1F, 0x87, 0xDF, 0x80, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x00, 0x76, 0xE0,
        0x00, 0x00, 0xE6, 0xF0, 0x00, 0x01, 0xDF, 0xB8, 0x00, 0x01, 0xBF, 0xF8, 0x00, 0x01, 0xFF, 0xF8,
        0x00, 0x01, 0xFF, 0xF8, 0x00, 0x01, 0xFF, 0xF8, 0x00, 0x01, 0xBF, 0xD8, 0x00, 0x01, 0xBF, 0xD8,
        0x00, 0x00, 0xDF, 0xB0, 0x00, 0x00, 0xE0, 0x70, 0x00, 0x00, 0x7F, 0xE0, 0x00, 0x00, 0x1F, 0x80
};

const uint8_t logoHornbill PROGMEM [] = {
        0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0x80, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x07, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFE, 0x00, 0x00, 0x00, 0x00,
        0xFC, 0xFC, 0x0F, 0xF0, 0x00, 0xC0, 0x31, 0xC6, 0x78, 0x70, 0x03, 0xE0, 0x01, 0xC0, 0x33, 0xCE,
        0x30, 0x30, 0x01, 0xC0, 0x00, 0xC0, 0x01, 0xC6, 0x30, 0x30, 0x00, 0x00, 0x00, 0xC0, 0x01, 0xC6,
        0x30, 0x31, 0xFD, 0xFF, 0xFC, 0xFE, 0x71, 0xC6, 0x3F, 0xF1, 0x8C, 0xED, 0x8C, 0xE7, 0x31, 0xC6,
        0x30, 0x73, 0x0E, 0xE1, 0x8C, 0xC3, 0x31, 0xC6, 0x30, 0x33, 0x06, 0xE1, 0x8C, 0xC3, 0x31, 0xC6,
        0x30, 0x33, 0x86, 0xE1, 0x8C, 0xC3, 0x31, 0xC6, 0x30, 0x33, 0x8C, 0xE1, 0x8C, 0xC3, 0x31, 0xC6,
        0x78, 0x79, 0xDC, 0xE3, 0x9C, 0xEE, 0x39, 0xCE, 0x00, 0x08, 0x70, 0x00, 0x00, 0x38, 0x00, 0x00,
        0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x7F, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xED, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x01, 0xDF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x07, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x0F, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x1F, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xEF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x3D, 0xE6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3D, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
/*******************************************************************************************************/

typedef enum{
    SERVICE_TWITTER,
    SERVICE_FACEBOOK,
    SERVICE_TIME,
    SERVICE_TEMPERATURE,
    SERVICE_MAX_OPTIONS
};


jsmntok_t token[512];
char rcvdPayload[512];
char msgReceived=0;
int serviceNum = 0;

char supportedServices[SERVICE_MAX_OPTIONS][20]=
{
        "twitter",
        "facebook",
        "time",
        "temperature"
};

typedef enum{
    VALUE_NUM,
    VALUE_FLOAT,
    VALUE_STRING
}valueType_t;

typedef struct{
    char Tag[20];
    char value[20];
    const uint8_t *logo;
}serviceData_t;

serviceData_t service[SERVICE_MAX_OPTIONS]={
        /*serviceTag   value     LogoArray */
        {"tweets",   "12345", logoTwitter},
        {"likes",    "5678",  logoFacebook},
        {"Time",     "9:30",  logoTime},
        {"Temp",     "35",    logoTemperature}
};

void processReceivedMsg();
void wifiInit();
void connectToAWS();

void mySubCallBackHandler (char *topicName, int payloadLen, char *payLoad)
{
    if(msgReceived==0)
    {
        strncpy(rcvdPayload,payLoad,payloadLen);
        rcvdPayload[payloadLen] = 0;
        msgReceived = 1;
    }
}


void setup()   {

    Serial.begin(115200);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

    wifiInit();
    connectToAWS();
}



void loop() {

    display.clearDisplay();
    if(service[serviceNum].logo != 0);     // Display Logo if available
    display.drawBitmap(0,0, service[serviceNum].logo, 32, 32, WHITE); // Display the Logo
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(36,0);
    display.print(service[serviceNum].Tag);
    display.setTextSize(3);
    display.setCursor(32,32);
    display.print(service[serviceNum].value);

    display.setTextSize(1);
    display.setCursor(64-(SERVICE_MAX_OPTIONS*3),56);
    for(int i=0;i<=SERVICE_MAX_OPTIONS;i++)
    {
        if(i==serviceNum)
        {
            int x0,y0,x1,y1,x2,y2;
            x0 = display.getCursorX()+3;
            y0 = display.getCursorY()+3;
            display.fillCircle(x0,y0,3,WHITE);
        }
        else
        {
            display.write('.');
        }
    }

    if(serviceNum<(SERVICE_MAX_OPTIONS-1))  // Ensure the state machine is within the limits
    {
        serviceNum++;                   // Go to next state
    }
    else
    {
        serviceNum = 0;
    }
    display.display();

    if(msgReceived == 1)
    {
        processReceivedMsg();
        msgReceived = 0;
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        hornbillInitMsgs("Wifi Disconnected, Reconnecting after 1sec");
        vTaskDelay(1000/portTICK_PERIOD_MS);
        wifiInit();
        connectToAWS();
    }

    vTaskDelay(2000/portTICK_PERIOD_MS);
}



void hornbillInitMsgs(char *msg)
{
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.drawBitmap(0,0, logoHornbill, 64, 32, WHITE);
    display.setTextSize(1);
    display.setCursor(0,40);
    display.print(msg);
    display.display();
}




void wifiInit()
{
    int status = WL_IDLE_STATUS;
    while (status != WL_CONNECTED)
    {
        sprintf(rcvdPayload,"Reconnicting to SSID:%s",WIFI_SSID);
        hornbillInitMsgs(rcvdPayload);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        // wait 5 seconds for connection:
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }
}


void connectToAWS()
{
    int rc,reconnectCnt=0;
    hornbillInitMsgs("Connected to wifi");

    if(AWS_CLIENT.connect(HOST_ADDRESS,CLIENT_ID)== 0)
    {
        hornbillInitMsgs("Connected to AWS");
        vTaskDelay(1000/portTICK_PERIOD_MS);

        do{
            rc = AWS_CLIENT.subscribe(TOPIC_NAME,mySubCallBackHandler);
            if(rc == 0)
            {
                hornbillInitMsgs("Subscribe Successfull");
                vTaskDelay(1000/portTICK_PERIOD_MS);
            }
            else if(reconnectCnt++>5)
            {
                hornbillInitMsgs("Subscribe Failed, Check the Thing Name and Certificates");
                while(1);
            }
        }while(rc != 0);
    }
    else
    {
        hornbillInitMsgs("AWS connection failed, Check the HOST Address");
        while(1);
    }
}


char ser[20], serTag[20],serVal[20];
void processReceivedMsg()
{
    int i,r,index;
    jsmn_parser_t tokenParser;


    jsmn.Init(&tokenParser);
    r = jsmn.parse(&tokenParser, rcvdPayload, strlen(rcvdPayload), token, sizeof(token)/sizeof(token[0]));
    if (r < 0) {
        return;// HORNBILL_IO_REQUEST_JSON_INVALID;
    }

    /* Assume the top-level element is an object */
    if (r < 1 || token[0].type != JSMN_OBJECT) {
        return;// HORNBILL_IO_REQUEST_JSON_INVALID;
    }

    /* Loop over all keys of the root object */
    for (i = 1; i < r; i++) {
        if (jsmn.equate(rcvdPayload, &token[i], "reported") == 0) {
            /* We may use strndup() to fetch string value */
            return ;
        }else if (jsmn.equate(rcvdPayload, &token[i], "desired") == 0) {
            /* We may use strndup() to fetch string value */
            i++;
        }else if (jsmn.equate(rcvdPayload, &token[i], "service") == 0) {
            /* We may use strndup() to fetch string value */
            sprintf(ser,"%.*s", token[i+1].end-token[i+1].start,rcvdPayload + token[i+1].start);
            i++;
        } else if (jsmn.equate(rcvdPayload, &token[i], "serviceTag") == 0) {
            /* We may want to do strtol() here to get numeric value */
            sprintf(serTag,"%.*s", token[i+1].end-token[i+1].start,rcvdPayload + token[i+1].start);
            i++;
        } else if (jsmn.equate(rcvdPayload, &token[i], "value") == 0) {
            /* We may use strndup() to fetch string value */
            sprintf(serVal,"%.*s", token[i+1].end-token[i+1].start,rcvdPayload + token[i+1].start);
            i++;

            for(index=0;index<SERVICE_MAX_OPTIONS;index++)
            {
                if(strcmp(ser,supportedServices[index])==0)
                {
                    strcpy(service[index].Tag,serTag);
                    strcpy(service[index].value,serVal);
                    break;
                }
            }
        }
        else {
            //  printf("Unexpected key: %.*s\n", token[i].end-token[i].start,rcvdPayload + token[i].start);
        }
    }
}

