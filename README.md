# DoorSystemSolution
The Solution for startup company to build their own door system with low cost and complete freedom.

## Solution Overview
1. First we need to modify the magnitic door locker, typically if no voltage on locker goes unlock, and high goes lock. The locker at least have one trigger, for arduino, the trigger can be controlled with a relay module.
2. Second is the RFID card used for entrance control. Hacking into a card is hard, but if we can choose the RFID standard, write and read cards is easy. Things goes even earier when we found open source arduino libraries for RFID.
3. Third is the communication between clients and servers. If you just want to control one door with one arduino, all the cards can be fixed on arduino, but for more security and complete freedom, you gona need a server. Each door has an arduino to control the locker, connecting to wifi, thus it became a typical client. One server contains all the card information, and authorize which can pass and which cannot. Also, the server is more useful if we want to add attandance statistic function, and more other functions.

## Board
The Main Board is WeMos D1, also can change to other arduino board support wifi.
How to use WeMos D1 in arduino IDE, please follow the guide:
https://www.wemos.cc/tutorial/get-started-arduino.html
WeMos D1(retired)

## RFID
We choose the M1 card which is cheap and easy to get.
The standard is MIFARE Classic 1k, any other MIFARE card is ok even Chinese UID changeable MIFARE cards.
To read the card, we use MFRC-522 breakout board connected with arduino. The library for arduino to read and write:
https://github.com/miguelbalboa/rfid

## Wiring

Connection between WEMOS D1 and RC522 breakout board.
<pre><code>/*
 * PINOUT
 * +--------------------------------+---------------+
 * | WEMOS D1 ESP8266 BOARD         | CONECT TO PIN |
 * +--------------------------------+---------------+
 * | PIN  | FUCTION  | ESP-8266 PIN | RC522 | RELAY |
 * +------+----------+--------------+-------+-------+
 * | 3.3V | POWER    | 3.3V         | 3.3V  |       |
 * +------+----------+--------------+-------+-------+
 * | 5V   | POWER    | 5V           |       | VCC   |
 * +------+----------+--------------+-------+-------+
 * | GND  | GND      | GND          | GND   | GND   |
 * +------+----------+--------------+-------+-------+
 * | D13  | SCK      | GPIO-14      | SCK   |       |
 * +------+----------+--------------+-------+       +
 * | D12  | MISO     | GPIO-12      | MISO  |       |
 * +------+----------+--------------+-------+       +
 * | D11  | MOSI     | GPIO-13      | MOSI  |       |
 * +------+----------+--------------+-------+       +
 * | D10  | SS (SDA) | GPIO-15      | SDA   |       |
 * +------+----------+--------------+-------+       +
 * | D8   | IO       | GPIO-0       | RESET |       |
 * +------+----------+--------------+-------+-------+
 * | D2   | IO       | GPIO-16      |       | IN1   |
 * +------+----------+--------------+-------+-------+
 */
For Door control relay

	relay D2
	buzzer D3

## Server

We tried to find a tool for server, the easier the better. [XAMPP](https://www.apachefriends.org/download.html) is a good choice, and [PHPstudy](http://www.phpstudy.net/a.php/211.html) is light and easy for newbie.

You can choose either tool to run database and server. Just add **attandance.php** into "WWW" or "ROOT" folder to make it accessable.

