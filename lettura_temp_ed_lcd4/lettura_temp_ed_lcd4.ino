#include <SPI.h>
#include <Ethernet.h>

#include <DHT.h>
#include <LiquidCrystal.h>
#define DHTPIN 6
#define relaytemp 9
#define relaypomp 8
#define relayled 7

int analogInPin = A1;
int sensorValue = 0;

// 8 è il pin di Arduino a cui collego il sensore di temperatura
#define DHTTYPE DHT11
// dht11 è il tipo di sensore che uso
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(A5, A4, 5, A3, 3, 2);
// connessione display (pin)


byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0xE8, 0xEF };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "172.30.40.1";    // name address for Google (using DNS)
char server2[] = "172.30.40.21";

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(172, 30, 200, 6);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {

  Serial.begin(9600);
  pinMode(relaytemp, OUTPUT);
  pinMode(relaypomp, OUTPUT);
  pinMode(relayled, OUTPUT);
 pinMode( analogInPin, INPUT);
  // start the Ethernet connection:
  Serial.println("connecting...");
  if (Ethernet.begin(mac) == 0) {
    // Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  Serial.println("connected");
  // imposto il tipo di display (colonne, righe)



  lcd.begin(16, 2);

  lcd.println("Temperatura");
  lcd.setCursor(0, 1);
  lcd.print("Umidita'");
  lcd.setCursor(1, 0);



}
void loop() {

  int t = dht.readTemperature();
  Serial.println("Temperatura:");  Serial.println(t);
  int h = dht.readHumidity();
  Serial.println("Umidità:"); Serial.println(h);
  // posiziono il cursore alla colonna 12 e riga 0
  lcd.setCursor(14, 0);
  lcd.print(t);
  lcd.setCursor(14, 1);
  lcd.print(h);

  //fotoresitore
  sensorValue = analogRead(analogInPin);            
 
  Serial.print("sensor = " );
  Serial.println(sensorValue);      

  if(sensorValue <= 50)
  {
   digitalWrite(relayled, LOW); 
    
  }
else
{
  digitalWrite(relayled, HIGH);
  }
 
  delay(2000);

  
  //if temperatura <,>
  send(t, h);

  char s0 = getStato();
  char s1 = getStato1();
  //char s2 = getStato2();
  Serial.print("Stato0: ");
  Serial.println(s0);
  Serial.print("Stato1: ");
  Serial.println(s1);
  //Serial.print("Stato2: ");
  //Serial.println(s2);

  updateRelayTemp(s0, t);
  updateRelayPomp(s1, h);
  //  updateRelayLed(s2, t);
}

void send(int t, int h)
{
  Serial.println("sending temperature");

  if (client.connect(server, 8089)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.print("GET /ethernet/data.php?temperature=");
    client.print(t);
    client.print("&humidity=");
    client.print(h);
    client.println(" HTTP/1.1");

    client.println("Host: 172.22.20.68");
    client.println("Connection: close");
    client.println();
    client.flush();

  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }

  while (client.connected())
  {
    // if there are incoming bytes available
    // from the server, read them and print them:
    if (client.available()) {
      char c = client.read();
      Serial.print(c);
      
    }


  }

  Serial.println();
  Serial.println("disconnecting.");
  client.stop();
}

char getStato()
{
  char statoFinale = '?';
  Serial.println("getting state");
  if (client.connect(server2, 8089)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.print("GET /stato");
    client.println(" HTTP/1.1");

    client.println("Host: 172.30.40.21");
    client.println("Connection: close");
    client.println();
    client.flush();

  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }

  int newlines = 0;
  while (client.connected())
  {
    // if there are incoming bytes available
    // from the server, read them and print them:

    if (client.available()) {
      char c = client.read();
      if (c == '\n')
      {
        newlines++;
        //Serial.print(newlines);
      }
      if (newlines == 7 && (c == 'f' || c == 't' || c == 'a'))
      {
        statoFinale = c;
        break;
      }
      //Serial.print(c);
    }
  }

  Serial.println();
  Serial.println("disconnecting.");
  client.stop();


  return statoFinale;
}

//stato della luce

char getStato1()
{
  char statoFinale;
  Serial.println("getting state1");
  if (client.connect(server2, 8089)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.print("GET /stato1");
    client.println(" HTTP/1.1");

    client.println("Host: 172.30.40.21");
    client.println("Connection: close");
    client.println();
    client.flush();

  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }

  int newlines = 0;
  while (client.connected())
  {
    // if there are incoming bytes available
    // from the server, read them and print them:

    if (client.available()) {
      char c = client.read();
      if (c == '\n')
      {
        newlines++;
        //Serial.print(newlines);
      }
      if (newlines == 7 && (c == 'f' || c == 't' || c == 'a'))
      {
        statoFinale = c;
        break;
      }
    }

  }
  //Serial.print(c);


  Serial.println();
  Serial.println("disconnecting.");
  client.stop();


  return statoFinale;
}

//
//
//bool getStato2()
//{
//  bool stato2;
//  Serial.println("getting state2");
//  if (client.connect(server2, 8089)) {
//    Serial.println("connected");
//    // Make a HTTP request:
//    client.print("GET /stato2");
//    client.println(" HTTP/1.1");
//
//    client.println("Host: 172.30.40.21");
//    client.println("Connection: close");
//    client.println();
//    client.flush();
//
//  } else {
//    // if you didn't get a connection to the server:
//    Serial.println("connection failed");
//  }
//
//  int newlines = 0;
//  while (client.connected())
//  {
//    // if there are incoming bytes available
//    // from the server, read them and print them:
//
//    if (client.available()) {
//      char c = client.read();
//      if (c == '\n')
//      {
//        newlines++;
//        //Serial.print(newlines);
//      }
//      if (newlines == 7)
//      {
//        if (c == 't')
//        {
//          stato2 = true; break;
//        } else if (c == 'f')
//        {
//
//          stato2 = false; break;
//        }
//
//      }
//      //Serial.print(c);
//    }
//  }
//
//  //Serial.println();
//  Serial.println("disconnecting.");
//  client.stop();
//
//
//  return stato2;
//
//}

void updateRelayTemp(char stato, int temperature) {
  if (stato == 'f') {
    digitalWrite(relaytemp, HIGH);
  } else if (stato == 't') {
    digitalWrite(relaytemp, LOW);
  } else if (stato == 'a') {
    if (temperature >= 24)
    {
      digitalWrite(relaytemp, LOW);
    }
    else
    {
      digitalWrite(relaytemp, HIGH);
    }
  } else {
    Serial.println("Stato ILLEGALE, sono scemo o cosa?");
  }
}

void updateRelayPomp(char stato, int humidity) {
  if (stato == 'f') {
    digitalWrite(relaypomp, HIGH);
  } else if (stato == 't') {
    digitalWrite(relaypomp, LOW  );
  } else if (stato == 'a') {
    if (humidity >= 50)
    {
      digitalWrite(relaypomp, LOW);
    }
    else
    {
      digitalWrite(relaypomp, HIGH);
    }
  } else {
    Serial.println("Stato ILLEGALE, sono scemo o cosa?");
  }
}


//void updateRelayLed(char stato, int led) {
//  if (stato == 'f') {
//    digitalWrite(relayled, HIGH);
//  } else if (stato == 't') {
//    digitalWrite(relayled, LOW);
//  } else if (stato == 'a') {
//    if (temperature >= 24)
//    {
//      digitalWrite(relaytemp, LOW);
//    }
//    else
//    {
//      digitalWrite(relaytemp, HIGH);
//    }
//  } else {
//    Serial.println("Stato ILLEGALE, sono scemo o cosa?");
//  }
//}



