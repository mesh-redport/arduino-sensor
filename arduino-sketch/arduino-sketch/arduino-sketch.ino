#include "SPI.h"
#include "Ethernet.h"
#include "SHT2x.h"
#include "Wire.h"

byte mac[] = {   0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,100); // Cambiar IP a una en el rango del Router <----- <------- <--------
EthernetServer server(80);  // Arrancamos el servidor en el puerto estandard 80

int UVOUT = A0; 
int REF_3V3 = A1; 


void setup() 
{
  Wire.begin();
  Serial.begin(9600);
  while (!Serial) ; // Retraso para el Leonardo

  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("Servidor Web en la direccion: ");
  Serial.println(Ethernet.localIP());
  pinMode(12, OUTPUT);

  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);

  Serial.println("ML8511 example");
}



void loop() 
{
  EthernetClient client = server.available();  // Buscamos entrada de clientes
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;  // Las peticiones HTTP finalizan con linea en blanco
    while (client.connected()) 
    { if (client.available()) 
    digitalWrite(13, LOW);    // apaga el LED

        {  char c = client.read();
        digitalWrite(13, LOW);    // apaga el LED
           Serial.write(c);    // Esto no es necesario, pero copiamos todo a la consola
           if (c == '\n' && currentLineIsBlank)  //
            {      
              client.println("HTTP/1.1 200 OK");  // Enviar una respuesta tipica
              client.println("Content-Type: text/html");
              client.println("Connection: close");  
              client.println();
    
              float h = SHT2x.GetHumidity();                // Leer el sensor
              float t = SHT2x.GetTemperature();
              Serial.println(t);
              Serial.println(h);

              digitalWrite(13, HIGH);   // enciende el LED
              delay(100);              // espera un segundo
              digitalWrite(13, LOW);    // apaga el LED
              delay(100);              // espera un segundo

              int uvLevel = averageAnalogRead(UVOUT);
              int refLevel = averageAnalogRead(REF_3V3);
            
              //Usa el pin 3.3V para mejorar la lectura del sensor
              float outputVoltage = 3.3 / refLevel * uvLevel;
            
              float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Convierte el voltaje a unidad de intenisdad UV

              
              client.print("Temperatura:");
              client.print(t);      // Aqui mandamus la temperatura
              client.print(" ");
              client.print("Humedad:");
              client.print(h);      // Aqui mandamus la humedad
              client.print(" ");
              client.print("UV:");
              client.print(uvIntensity);

              // Descomentar para leer los valores de sensores por monitor serial
              //Serial.print("output: ");
              //Serial.print(refLevel);
            
              //Serial.print("ML8511 output: ");
              //Serial.print(uvLevel);
            
              //Serial.print(" / ML8511 voltage: ");
              //Serial.print(outputVoltage);
            
              //Serial.print(" / UV Intensity (mW/cm^2): ");
              //client.print("Radiación UV:");
              //client.print(uvIntensity);
            
              //Serial.println();


  
              break;
            }
        if (c == '\n') 
           currentLineIsBlank = true;          // nueva linea
        else if (c != '\r') 
            currentLineIsBlank = false;
     }
    }
    //delay(10);    // Para asegurarnos de que los datos se envia
    client.stop();    // Cerramos la conexion
    Serial.println("client disonnected");
  }
}

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 

  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;

  return(runningValue);  
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
