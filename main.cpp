// Remember this, or most things won't work
#define __AVR_ATmega328P__
// Include all Arduino
#include <Arduino.h>
// ----------------- CUT HERE IF USING ARDUINO IDE -----------------------------

enum Status {INIT, WAIT_COMMAND, PARSE_COMMAND};

const unsigned long LOG_DELAY = 1000;
const String COMMAND_HELP = "\n\rHELP\n\r\n\rCommand list:\n\r\n\rhelp: prints help\n\rled start: starts led blinking\n\rled stop: stop led blinking\n\r";

Status gCommandLineStatus = INIT;
String gCommand = "";
unsigned long gLastStamp = millis();
int gLed = LOW;
bool gLedBlinkActive = true;

void loop_blink();
void loop_commandLine();
void parseCommand(String command);

void setup()
{
    Serial.begin(9600);
    while (!Serial) {}
    
    pinMode(LED_BUILTIN, OUTPUT);
    
    Serial.println("setup(): OK");
}

void loop()
{
    loop_commandLine();
    loop_blink();
}

void loop_blink()
{
    if (!gLedBlinkActive)
        return;
    
    if ((millis() - gLastStamp) >= LOG_DELAY)
    {
        gLed = gLed == LOW ? HIGH : LOW;
        digitalWrite(LED_BUILTIN, gLed);
        
        gLastStamp = millis();
    }
}

void loop_commandLine()
{
    switch (gCommandLineStatus)
    {
        case INIT:
        {
            Serial.print('>');
            Serial.print(' ');
            gCommandLineStatus = WAIT_COMMAND;
            break;
        }
        
        case WAIT_COMMAND:
        {
            while (Serial.available() > 0)
            {
                char mych = (char) Serial.read();
                // Echo the char
                Serial.print(mych);
                if (mych == '\r')
                {
                    Serial.println();
                    gCommandLineStatus = PARSE_COMMAND;
                }
                else
                    gCommand += mych;
            }
            break;
        }
        
        case PARSE_COMMAND:
        {
            parseCommand(gCommand);
            gCommand = "";
            gCommandLineStatus = INIT;
            break;
        }
    }
}

void parseCommand(String command)
{
    if (command.equals("help") || command.equals("h") || command.equals("?"))
    {
        Serial.println(COMMAND_HELP);
    }
    else if (String("led stop").equals(command))
    {
        gLedBlinkActive = false;
        digitalWrite(LED_BUILTIN, LOW);
    }
    else if (String("led start").equals(command))
    {
        gLedBlinkActive = true;
    }
    else if (command.equals(""))
    {
        
    }
    else if (command.equals(""))
    {
        
    }
}
