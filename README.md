# Tutorials

## Why I choose Redpine RSI9116

Recently. I'm studying in  low power consumption module for WI-FI application, I using Texas Instruments CC3220 MCU develop my project at the beginning. Although CC3220 have great performed at power consumption and easy using platform\(have a lot  Free RTOS resource \).but this MCU can't change Main clock source\(just fix at 80MHz\), It will be okay with some project, but it will be deadly problem with project using battery .At the same time I found Redpine RSI9116\(maybe recent Silicon labs acquisition it\),Have better  WI-FI power consumption than CC3220, And can connect it just thought AT command. It's the gospel for me

## Why write library by myself

Redpine have abundant example coed with WIFI host, but most of that are is for sapis communicate, and Redpine library is coding base on  Free RTOS ,In my application, using bare metal environment is enough\(less code size and better power consumption \).so I decide write a library by myself 

## Why Opensource 

I'm working at a Start-up Company in Taiwan ,I'm very new at HW and FW design, but need lead  whole project\(actually only me in the team at beginning\), you can image that how hopeless and helpless at that period

 Luckly, I met my mentor  Zhih-Tai, Liu last year \(He is a freelancer and also have a  [Electro-Hedgehog](https://www.tasker.com.tw/workroom/3KWQr) in Taiwan Tasker \), He teach me a lot at HW designed and FW structure. We often discussing some designed over and over again excitedly  

 so I decide pass this passion to other one who struggle with problem but no have abundant  resource 

In the end , thank my partner chase hu, you are so support no only in HW and ME but also in  design ideas

## Develop Flow Chart

![ Implement Flow chart](.gitbook/assets/image.png)

#### This is the flow chart implement  RS9116 Using RS9116 EVM and STM32 Nucleo board

##  1.RS9116 EVM Firmware update

When star you project Need  update  EVM FW at first\( Different FW version will effect AT Command parameter\)  , it can reference silicon lab opensource document.

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

### 4.Config MQTT Topic in main c

```c
const char * MQTT_Sub_Topic="topic/Subcribe";
const char * MQTT_Pub_Topic="topic/Publish";
```



### 5.Config MQTT Topic in main c

```c
	BSP_RF_RS9116_Init();
  BSP_RF_RS9116_WIFI_Connect();
  BSP_RF_RS9116_MQTT_Connect();
  
  HAL_Delay(5000);
  BSP_RF_RS9116_MQTT_DisConnect();
```

## 4.Test

Unfortunately, In RS9116 EVM Board don't have TX/RX pinout\(can't bypass form USB virtual comport driver like cp2302 \), I try connect GPIO\_8 \(RSI9116 TX\),GPIO\_9 \(RSI9116 RX\) by jumper . and USB port just plug in Power port . But still have some bug and unstable\(If have better solution please tell me\)

![RS9116 EVM Sch ](.gitbook/assets/image%20%282%29.png)

In the end I wrote a python script, just simple exchange data between two virtual comport ,and monitor by CMD\( you also can find this script form my git project\),Block diagram is below

![Serial data exchange Block diagram ](.gitbook/assets/image%20%283%29.png)

So using this python script, In Hardware setting   just connect STM and RSI 9116 EVM  in USB port .After Execute python script , chose you comport ,then you can start your project

![python script screen shot](.gitbook/assets/image%20%284%29.png)

  


