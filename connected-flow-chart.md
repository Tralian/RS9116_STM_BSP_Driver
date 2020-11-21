# Tutorial : RS9116 Connect to AWS using MQTT \(AT Command Library implement by STM\)

## Develop Flow Chart

![ Implement Flow chart](.gitbook/assets/image.png)

#### This is the flow chart implement  RS9116 Using RS9116 EVM and STM32 Nucleo board

##  1.RS9116 EVM Firmware update

Need  update  EVM FW  , it can reference silicon lab opensource document.

 [AN1290: RS9116W Firmware Update Application Note](https://www.silabs.com/documents/login/application-notes/an1290-rs9116w-firmware-update-application-note.pdf)

## 2.Load AWS Certificate using python script

Silicon provide simple Python 2.7 script to upload AWS certificate to RS9116 module. but little complex.

so my work mate rewrite it ,you also can find python script in my project

## _3.STM board setting and Import library_

### 1.Create STM project 

In my library have using DMA UART peripheral .need config first\( also can just modify form my example , just slight change in GPIO port and DMA channel I though\)

### 2.Import bsp\_rf/bsp\_uart library to STM project

### 3.Config MQTT setting in bsp\_rf.c

```c
const char * SSID="Yours WIFI";
const char * SSID_password="Yours WIFI password";
const char * AWS_endpoint="--Yours AWS endpoint----.iot.----.amazonaws.com";
const char * MQTT_Server_Port="8883";
const char * Device_ID="Yours Device ID";
const char * MQTT_username="username";
const char * MQTT_password="password";
const char * MQTT_client_port="5503";
const char * MQTT_keep_alive_interval="80";
const char * MQTT_En_clean_session="1";
const char * MQTT_En_keep_alive_interval="1";
```

## 4.Application Test

Unfortunately, In RS9116 EVM Board don't have TX/RX pinout\(can't bypass form USB virtual comport driver like cp2302 \), I try connect GPIO\_8 \(RSI9116 TX\),GPIO\_9 \(RSI9116 RX\) by jumper . and USB port just plug in Power port . But still have some bug and unstable\(If have better solutionplease tell me\)

![RS9116 EVM Sch ](.gitbook/assets/image%20%282%29.png)

In the end I wrote a python script, just simple exchange data between two virtual comport ,and monitor by CMD\( you also can find this script form my git project\),Block diagram is below

![Serial data exchange Block diagram ](.gitbook/assets/image%20%283%29.png)

So using this python script, In Hardware setting   just connect STM and RSI 9116 EVM  in USB port .After Execute python script , chose you comport ,then you can start your project

![python script screen shot](.gitbook/assets/image%20%284%29.png)

  


