# ChipKIT project
#### Authors: War Ahmed & Miro Bakovic

##### Project Description
We've developed a digital temperature and a game that will run on a ChipKIT Uno32 (which is a basic microcontroller) together with a ChipKIT Basic I/O.

The a digital temperature works by fetching data from the ChipKIT temperature sensor and display the temperature in Celsius. Depending on the weather the thermometer will display one of two animation either a sun if its hot otherwise a snowflake. The temperature is fetched by communicating with the I/O shield temperature sensor TCN75A over I2C. The sensor itself was addressed using the simple protocol described in the data sheet. The software retrieves both the most signiﬁcant byte and the least signiﬁcant byte. However, it uses only the MSB since the decimal numbers, represented by the LSB, really aren’t vital. The MSB is converted from a signed 8 bit integer to a char array of three elements using a simple routine written by us. The resulting string is then displayed on the OLED using the font array from the lab material.

The OLED implementation where we displayed the information to the user was done by always updating the entire display, we used horizontal and page addressing mode in combination with the commands to select speciﬁc columns and pages to write to. This is useful when software needs only to update the 3 byte temperature. The temperature and animations are updated once every second with the help of a timer and its interrupt. The icons used in the animations were created with the help of pen, paper and understanding of how the OLED displays individual pixels.

The game we implemented was supposed to relate to cold weather. We created a snowman ﬁghting game. It consists of dodging snowballs until you’re close enough to jab the snowman with your sword, which ends the game. The speed with which the snowballs travel increase as one gets closer to the snowman, so as to create some diﬃculty levels. The entire game is backed by a 128 * 4 memory array of unsigned 8 bit integers. The content of the memory array changes in pace with the game. The display is then update using the memory array. Controls in this game consist of buttons 4, 3 and 2 of the I/O shield. Sideways movement is handled by buttons 3 and 4, if button 2 is pressed in combination with button 3 or 4 the player moves up or down. Horizontal movement is increased or decreased by 5 pixels, while the vertical position changes by a full page for each button press. The player jabs with the sword by sliding switch 1 upwards. The position of the player is updated 10 times per second, while the snowman mirrors the players moment after a delay of half a second.

In order to run the code you will need ofcourse a ChipKIT Uno32 together with the ChipKIT Basic I/O. Then plug in the chipKIT to your computer with a USB-cable that is included with the chipKIT then compile and run the code on MCB32 tool chain or the MPLAB X IDE. For information on see this [page](https://kth.instructure.com/courses/2535/pages/chipkit-slash-mcb32-toolchain-faq).

_This project was developed during our third semester (2016) for the course, [IS1500](https://www.kth.se/student/kurser/kurs/IS1500?l=en) Computer Organization and Components._
