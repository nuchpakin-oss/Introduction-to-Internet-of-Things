# Introduction-to-Internet-of-Things

# Introduction to Internet of Things (IoT)

โปรเจกต์นี้เป็นระบบ IoT สำหรับตรวจวัดสภาพแวดล้อมภายในห้องแบบเรียลไทม์ โดยใช้บอร์ด **ESP32** เชื่อมต่อกับเซนเซอร์ต่าง ๆ เพื่อเก็บข้อมูลและตรวจจับการเคลื่อนไหว จากนั้นสามารถนำข้อมูลไปแสดงผลหรือประยุกต์ใช้กับระบบ Smart Home ได้

## Features

* ตรวจวัดอุณหภูมิและความชื้นด้วยเซนเซอร์ DHT
* ตรวจจับการเคลื่อนไหวด้วย PIR Motion Sensor
* ตรวจจับความเข้มแสงด้วย LDR Sensor
* ใช้ RGB LED แสดงสถานะของระบบ
* ประมวลผลและควบคุมด้วย ESP32
* รองรับการพัฒนาต่อยอดสู่ระบบ IoT ผ่าน Wi-Fi

## Components Used

* ESP32 DevKit V1
* DHT11/DHT22 Sensor
* PIR Motion Sensor
* LDR Sensor
* RGB LED
* Resistors
* Breadboard & Jumper Wires

## System Overview

ระบบจะทำการอ่านค่าจากเซนเซอร์ต่าง ๆ เช่น อุณหภูมิ ความชื้น แสง และการเคลื่อนไหว แล้วส่งข้อมูลเข้าสู่ ESP32 เพื่อประมวลผล โดยสามารถแสดงสถานะผ่าน RGB LED หรือส่งข้อมูลไปยังแพลตฟอร์ม IoT สำหรับการติดตามแบบออนไลน์ได้

## Applications

* Smart Room Monitoring
* Smart Home System
* ระบบแจ้งเตือนการเคลื่อนไหว
* ระบบตรวจสอบสภาพแวดล้อมภายในอาคาร

## Circuit Diagram

ตัวอย่างการเชื่อมต่ออุปกรณ์ในโปรเจกต์

<img width="920" height="767" alt="image" src="https://github.com/user-attachments/assets/90ff706d-ebfc-40a1-95d5-31b4d3a3b5fd" />

<img width="1917" height="962" alt="NodeRED_Dashboard" src="https://github.com/user-attachments/assets/c55d580f-2cac-4a64-9ef9-ba7a2d512271" />

<img width="1246" height="587" alt="InfluxDB" src="https://github.com/user-attachments/assets/2d539afc-3e10-431b-8019-b2048ab3aebc" />

<img width="1917" height="971" alt="Blynk_Dashboard" src="https://github.com/user-attachments/assets/0116b848-6463-4426-a9d8-8401ea0b6aa1" />

<img width="870" height="1882" alt="Blynk_Dashboard_mobile" src="https://github.com/user-attachments/assets/6da75bd6-e953-40ff-832c-02b854a01051" />

<img width="870" height="1882" alt="Telegram_Alert" src="https://github.com/user-attachments/assets/2864e62f-df16-4804-b658-a93d2000aa5d" />

## Future Improvements

* เพิ่มการแสดงผลผ่าน Web Dashboard
* บันทึกข้อมูลลง Cloud Database
* แจ้งเตือนผ่าน LINE Notify หรือ Telegram
* ควบคุมอุปกรณ์ผ่าน Mobile Application

## Author

Developed for learning and experimentation in Internet of Things (IoT).
