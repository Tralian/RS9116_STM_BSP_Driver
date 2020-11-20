# Initial page

## Why I choose Redpine RSI9116

Recently. I'm study low power consumption module for WI-FI application, I using Texas Instruments CC3220 MCU develop my project at the beginning. Although CC3220 have great performed at power consumption and easy using platform\(have a lot  Free RTOS example in AWS\).but this MCU can't change Main clock source\(just fix at 80MHz\), It will be okay with some project, but it will be deadly problem with project using battery .At the same time I found Redpine RSI9116\(maybe recent Silicon labs acquisition it\),Have better  WI-FI power consumption than CC3220, And can connect it just thought AT command. It's the gospel for me

## Why write library by myself

Redpine have abundant example coed with WIFI host, but most of that are is for sapis communicate, and Redpine library is coding base on  Free RTOS ,In my application, just using bare metal is enough\(less code size and better power consumption \).so I decide write a library myself 

## Why Opensource 

I'm working at a Start-upCompany in Taiwan ,I'm very new at HW and FW design, but need lead  whole project\(actually only me in the team at beginning\), you can image that how hopeless and helpless at that period

 Luckly, I met my mentor  Zhih-Tai, Liu\(He also have a [Electro-Hedgehog](https://www.tasker.com.tw/workroom/3KWQr) in Tasker \)last year , He teach me a lot at HW designed and FW structure. We often excitedly discussing some designed over and over again 

 so I decide pass this passion to other one who struggle with problem but no have abundant  source 

In the end , thank my partner chase hu, you are so support no only in HW and ME but also in  design ideas

