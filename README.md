# TinyGlo+
[TinyGlo+](https://www.kickstarter.com/projects/mrjohnk/tinyglo-0) is a custom lighting solution for use in numerous projects. 
Leave TinyGlo+ in your project or combine many of them for a dramatic custom lighting feature. TinyGlo+ is Arduino compatible, 
which simplifies programming it for your application. Let your mind go wild with three programmable full color LEDs; 
your imagination is the only limit. 

TinyGlo+ builds upon the success of the [original TinyGlo project](https://www.kickstarter.com/projects/1034145369/tinyglo), 
which put on a firefly like light show. 

Getting Started
--
The TinyGlo+ comes pre-loaded with seven modes. Once connected to power your device will run this code. To cycle though the modes press the onboard button. 

To get started writing your own code, make sure you have the [Arduinio IDE](http://arduino.cc/download) up and running. 
Next, you'll want to install the Digispark Pro board from Digistump. This is done by following install instructions on the [Digistump Wiki](https://digistump.com/wiki/digispark/tutorials/connectingpro). 

Lastly, make sure you have the NeoPixel or FastLED library installed (see Libraries for more)
 
Once the board and libraries have been installed you'll be able to start writing your own code from the Arduino IDE! You may want to start by checking out the code found in the [Demo.ino](Demo.ino) file.

Please note, while TinyGlo+ is compatible with the Digispark Pro. It is in no way supported, authorized, or made by Digistump.

#### Libraries
To control the LEDs the use of a library is requied. The demo code uses the [Adafruit NeoPixel Library](https://github.com/adafruit/Adafruit_NeoPixel). How to use this library can be found on the [NeoPixel Guide](https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library) on the Adafuit Learning System.

Alternativly, you may also use the [FastLED library](https://github.com/FastLED/FastLED) library. While the FastLED team does not officially support this chipset with it does work a [small modification](https://digistump.com/board/index.php/topic,1944.msg9056.html#msg9056). 

To install a library download it in zip format then select Sketch > Included Library > Add ZIP Library. 

You can also manually install libraries by copying an unziped copy to your `libraries` folder. Typically this is found in the following locations:
`My Documents/Arduino/libraires` [windows] 
`Documents/Arduino/libraries` [Mac and Linux] 

If you need help with installing a library check out the [installation guide](https://www.arduino.cc/en/Guide/Libraries) on the Arduino site. 

Pinout
---
(![TinyGlo+ Pinout](https://ksr-ugc.imgix.net/assets/004/937/344/ad3340cfb9fb9c92a294b45c243afd04_original.jpg?v=1447867260&w=680&fit=max&auto=format&q=92&s=c7834817c749eeeb63051a1d35844c78)


Acknowledgements
---

[Arduino:](https://arduino.cc) For years has enabled millions of makers to work with microprocessors in a friendly way. From beginners to experts, their efforts make learning and using electronics much easier.  

[Digistump:](https://digitsump.com) Pioneered the use of the ATtiny167 microprocessor for use with Arduino.

[Adafruit:](https://adafruit.com) Creator of the open source [NeoPixel Arduino library] () allowing TinyGlo+ and countless other projects to use the WS2812B programmable LED. Adafruit takes great strides in providing quality products that are open source to share their knowledge with the maker community.

[Micronucleus V2.02:](https://github.com/micronucleus/micronucleus) Numerous contributors to the Micronucleus bootloader software enables inexpensive Atmel microprocessors to use an emulated USB port to upload user programs. It is revolutionary in enabling VUSB functionality.

[Atmel:](http://atmel.com) Provides a wide variety of electronic components, such as the ATtiny167 microprocessor used in the TinyGlo+ project. A "system on a chip" design that incorporates many features into a tiny comprehensive package that is the heart and brain of the project.
