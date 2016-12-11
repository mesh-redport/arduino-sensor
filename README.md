# arduino-sensor
Programa para módulo Arduino de REdPORT, este módulo tiene sensores de temperatura, humedad y UV, los valores de los sensores son obtenidos mediante el servicio web [REdPORT] (http://redport.cl/). Para armar tu módulo de sensores, primero debes tener los siguientes componentes:

1. [Arduino UNO](http://www.maxelectronica.cl/inicio/25-arduino-uno-r3-cable-usb-5-cables-m-m-5-cables-h-h.html)
2. [Shield Ethernet] (http://www.maxelectronica.cl/inicio/4-arduino-shield-ethernet-w5100.html?search_query=ethernet&results=8)
3. [Sensor Temperatura y Humedad SHT-21] (http://www.maxelectronica.cl/sensores/162-sensor-de-temperatura-y-humedad-sht-21.html)
4. [Sensor UV ML8511] (http://www.maxelectronica.cl/luz-color/167-modulo-ml8511-sensor-de-luz-y-radiacion-ultravioleta-uv-a-uv-b.html?search_query=UV&results=1)

</br>
# Conexión 

* Debes encajar el shield de Ethernet al Arduino UNO, luego debes conectar los cables que aparecen en la foto, el cable USB del Arduino al computador (para subir el código) y el cable de red (RJ45) al router que estés configurando. 
![Conexión Shield Ehternet](http://microfun.es/img/web5.jpg "Conexión Shield Ehternet")

* En la siguiente imagen aparece el esquema de conexión de los sensores, debes tener especial atención en los voltajes, porque estos sensores trabajan a 3.3V, si los conectas a 5V podrías dañarlos. Puedes añadir un led al PIN 13 de Arduino para hacer más fácil la configuración, aunque por defecto todas las placas Arduino traen un LED interno conectado al PIN 13.
![Conexiones](https://github.com/mesh-redport/arduino-sensor/blob/master/img/conexiones.png?raw=true "Conexiones")

# Código

* En este paso subiremos el código de Arduino, para esto debemos [instalar](https://www.arduino.cc/en/Guide/Libraries) las siguientes librerías:
</br>

1. [SPI] (https://www.arduino.cc/en/Reference/SPI)
2. [Ethernet] (https://www.arduino.cc/en/Reference/Ethernet)
3. [SHT2] (https://github.com/adafruit/DHT-sensor-library)
4. [Wire] (https://www.arduino.cc/en/Reference/Wire)

En el código de Arduino debes determinar la IP que usará el Arduino dentro de la Red que estés configurando, por defecto nosotros dejamos la IP 192.168.1.100, pero puedes usar la IP que necesites, en este código tambien puedes determinar la MAC que usará el dispositivo.

```
IPAddress ip(192,168,1,100);
```
Una vez que cambies la IP dentro del dominio de tu router, debes subir compilar el código y luego subirlo a la placa. Puedes revisar este procedimiento desde [acá] (https://www.arduino.cc/en/Main/Howto)

Genera un archivo .txt llamado seed, con los datos que posteriormente guarda en la base de datos.
https://github.com/mesh-redport/redport/blob/master/visor.php

El código que llama a desde home.php a los datos de los indicadores es el siguiente:
<div class="indicators">

    <?php
    $shots = mysql_query("SELECT id,tipo,valor,fecha FROM mesh_redport.sensores ORDER BY id DESC LIMIT 3;") or die(mysql_error());
    while($row = mysql_fetch_assoc($shots)) { ?>
    <div class="indicator">
      <?php
      if($row["tipo"] == "Temperatura"){
        echo "<div class='rpicon-temperature'></div>";
      }else if($row["tipo"] == "UV"){
        echo "<div class='rpicon-uv'></div>";
      }else if($row["tipo"] == "Humedad"){
        echo "<div class='rpicon-humedity'></div>";
      }
      if($row["tipo"] == "Temperatura"){
        echo "<p class='text_indicator'>".round($row['valor'])."°C</p>";
      }else if($row["tipo"] == "UV"){
        if(round($row["valor"]) <= 2){
          echo "<p class='text_indicator' style='color:#41ab4b;'>BAJO</p>";
        }else if(round($row["valor"]) <= 5){
          echo "<p class='text_indicator' style='color:orange;'>MEDIO</p>";
        }else if(round($row["valor"]) <= 7){
          echo "<p class='text_indicator' style='color:orangered;'>ALTO</p>";
        }else if(round($row["valor"]) <= 10){
          echo "<p class='text_indicator' style='color:#f71138;'>MUY ALTO</p>";
        }else if(round($row["valor"]) >= 11){
          echo "<p class='text_indicator' style='color:#7a58dc;'>EXTREMO</p>";
        }
      }else if($row["tipo"] == "Humedad"){
        echo "<p class='text_indicator'>".round($row['valor'])."%</p>";
      }
      if($row["tipo"] == "UV"){
        //echo "<span class='text_uv'>uv</span>";
        echo "<div class='bar_background'></div>";
        echo "<div class='bar_uv' style='width: ".(abs($row['valor']*100)+10)."%;'></div>";
      }else if($row["tipo"] == "Temperatura"){
        echo "<div class='bar_background'></div>";
        echo "<div class='bar_t' style='width: ".(abs($row['valor'])/50*75)."%;'></div>";
      }else if($row["tipo"] == "Humedad"){
        echo "<div class='bar_background'></div>";
        echo "<div class='bar_h' style='width: ".(abs($row['valor'])/100*75)."%;'></div>";
      }
      ?>                   
