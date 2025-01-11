# A highly customized electronic calendar developed based on Unity and ESP32 DROOM 32D – Joy Timer


## SDV_Group_Assessignment – Final Report


---
```
Jinming Xia
ID： 24214074
Zhiyu Cao
ID： 24067861
Yue Zhu
ID： 21130194
Hongbing Qiu
ID： 24065411
```


Word Count: 2060 words

GitHub URL: https://github.com/mk20661/SDV_Group_Assessignment

---

### I. Introduction

Effective schedule management is essential for students and educators to maintain a
balance between workloads and rest. Research shows that a well-structured schedule
allows individuals to allocate their time efficiently, prioritize tasks, and strike a better
balance between professional and personal responsibilities. For educators, this helps
to deliver lessons more smoothly and to enhance interaction with students (Trueman
& Hartley, 1996). In general, traditional tools such as static calendars often fail to
provide intuitive and engaging results for monitoring academic terms and vacations.
Therefore, Joy Timer was developed as an innovative tool that integrates visual and
interactive components. By combining physical devices with digital dashboards, it
provides a more clear and understandable way to support the academic and vacation
plans of stakeholders, helping them achieve a better balance between academic
responsibilities and personal schedules. This report will explore the design,
development, and results of the project. It covers data sources and structures,
hardware design, and the development of Unity-based digital dashboards. What's
more, it reflects the challenges encountered during the development process and the
solutions implemented. At the end of the report, we will summarize our reflections on
the project and some possible improvements in the future.


---
### II. Design and development process

#### 2.1 Data source and structure

A combination of external APIs and customized JSON files is used in this project,
aiming to obtain key data to support real-time monitoring of holiday and assessment
deadlines for various courses in this module. The specific operations are as follows:
First, by calling the UK Bank Holiday API provided by the UK government, our
group obtained real-time holiday countdown information in the UK to ensure that
users can know the upcoming holidays in time. At the same time, considering that the
holiday information in China changes every year, we updated and wrote a customized
JSON file according to the latest holiday document released by the Chinese
government, which can provide users with accurate information about the countdown
to Chinese holidays. In addition, the project also involves the integration of UCL
course module information, by compiling a specific JSON file to track and display the
DDL of each course, to help students manage their own course schedule. This
comprehensive approach to data acquisition optimizes the real-time and accuracy of
the information flow, enabling the project to better serve a wide range of user groups.

The detailed JSON structures used for UK bank holidays, Chinese holidays, and
course deadlines can be found in Appendices A, B, and C respectively.


---
#### 2.2 Design of hardware equipment

This project selected ESP32 WROOM 32D as the central controller, its multi-core
processor and high clock speed give superior computing performance, while the larger
memory capacity can support more complex program logic and higher number of
variables processing. In addition, the ESP32 WROOM 32D has low power
consumption in idle mode, making it ideal for scenarios requiring portable power
supply or long-term deployment. Its low price and excellent price-performance ratio
are also highly compatible with the project's low-cost development goals (Espressif
Systems, 2023).

The servo motor model used in this project is MG90S. With a size of 22.8×12.2×28.
mm and a weight of 13.4 g, it is ideal for portable device development. Especially for
compact design and limited space, MG90S is the most suitable choice. It is not only
cheap and easy to develop, but also compatible with ESP32 WROOM 32D, supports
PWM signal control, and has the advantages of fast response speed and wide control
range, which is very suitable for the driving pointer mechanism in this project.

The main device to output information on the physical device is the dial driven by the
MG90S pointer, which is used to countdown the date of the festival; an LED strip for
recording the remaining weeks of each semester and reading week; and the SSD
OLED displays the name information of the next festival. These devices are
characterized by low power consumption and high ease of use while being
inexpensive to facilitate mass production. The basic circuit connection design is
shown in Figure 1.


![Figure 1](https://github.com/XLunaXX07/SDV-report/blob/main/Circuit%20Diagram%20of%20the%20Joy%20Timer.PNG?raw=true)

*Figure 1 : Circuit Diagram of the Joy Timer*




        
Furthermore, the interaction between the hardware components and the overall system
workflow is illustrated in Figure 2.


![Figure 2](https://github.com/XLunaXX07/SDV-report/blob/main/System%20Logic%20Flowchart.png?raw=true)

*Figure 2 : System Logic Flowchart*




        
When it comes to Joy Timer's shell design, it has gone through a series of iterations
(see Appendices D and E). Initially, the shell was designed for basic display
requirements, however, this initial design presented too much information on the
interface, making it difficult for users to intuitively access key details. Sketches from
this early design stage, illustrating the challenges and initial concepts, are included in
Appendix D. Subsequent versions of the improvements focused on simple principles,
focusing on reducing redundancy and optimizing the user interface. The course
display was removed from the physical model and moved to the AR interface, making
the physical design more streamlined and focused. The final design, inspired by
devices such as record players and interchangeable lens digital cameras, features a
modular design that allows the housing to be split into two distinct parts and easily
assembled without glue (see Figure 3). This modularity not only enhances the
aesthetic and functional appeal of the product, but also enables users to customize
colors and materials, increasing versatility and commercial viability. As the design is
completed, the focus shifts to fine modeling to ensure that the internal structure of the
prototype can be adapted to the actual dimensions of the hardware. Detailed technical
drawings from the precision modeling process can be found in Appendices F and G.


![Figure 3](https://github.com/XLunaXX07/SDV-report/blob/main/Final%20Shell%20Design%20with%20Customizable%20Colors%20and%20Materials%20for%20Joy%20Timer.png?raw=true)

*Figure 3 : Final Shell Design with Customizable Colors and Materials for Joy Timer*




        
Ultimately, the main body of the prototype shell of the final product is 3D printed
with PLA material, and smooth surfaces and geometric structures are mostly used.
The splice structure and friction connection between the parts are used. The dial uses
acrylic for laser cutting and a UV printer to achieve the artistic effect of a transparent
clock surface. The Visual Trigger uses plywood as material, laser engraving is used to
achieve an intaglio effect, and the whole is presented in a minimalist modern style
(see Figure 4 ).


![Figure 4](https://github.com/XLunaXX07/SDV-report/blob/main/Prototype%20Appearance%20of%20the%20Joy%20Timer.png?raw=true)

*Figure 4 : Prototype Appearance of the Joy Timer*



---
#### 2.3 Development of digital dashboards

The UI design of the dashboard in the virtual appliance was developed entirely in
Unity, and it is a complement to the physical appliance, making the content we want
to show in the virtual appliance even more complete. The layout of the interface is
carefully designed with features such as live API update times, live countdown of UK
and Chinese holidays, module deadlines and task summaries, and ring charts
indicating the percentage of the semester remaining. By combining the metal and
wood grain textures of the device model with the blue-purple theme of the dashboard
itself, our team created an interactive interface that felt futuristic and technological
(see Figure 5 ).


![Figure 5](https://github.com/XLunaXX07/SDV-report/blob/main/UI%20design%20for%20the%20dashboard%20in%20digital%20device.png?raw=true)

*Figure 5 : UI design for the dashboard in digital device*




        
The AR interaction functionality was subsequently integrated into the virtual device,
leveraging image tracking for model generation and enabling the movement, scaling,
and rotation of digital models. Additional features included holiday video playback
with interactive effects and slideshow presentations for course-related content. All
interactions were controlled through custom C# scripts implemented in Unity (see
Figure 6 ).

![Figure 6](https://github.com/XLunaXX07/SDV-report/blob/main/Example%20of%20Unity%20Scripts%20for%20AR%20Interaction%20and%20Functionality.png?raw=true)
*Figure 6 : Example of Unity Scripts for AR Interaction and Functionality*


---
#### 2.4 Data flow and system architecture

In this project, data is obtained from several sources, including the UK GOV API
(which gets UK bank holiday data) and locally stored ChinsesHoliday.json and
DDL.json files. The physical and virtual devices send requests to the server, which
parses the data through a Python script and extracts the required fields (such as
vacation name, date, task deadline), sending it back to the requesting device (see
Figure 7 ). In addition, the server will recognize the request and send the
corresponding data back. In the physical device, this data displays the holiday in the
OLED, controlling the servo motor to point to the remaining days as well as the
number of weeks remaining in the current semester. In the virtual appliance, different
content blocks in the dashboard send different requests, filling them with the returned
data.


![Figure 7](https://github.com/XLunaXX07/SDV-report/blob/main/Data%20Flow%20and%20Processing%20Architecture.png?raw=true)

*Figure 7 : Data Flow and Processing Architecture*



---
### III. Technical challenges and solutions

One of the challenges is related to data retrieval. When using the UK Bank Holiday
API, the physical device consistently fails to parse JSON data. During debugging, it
was discovered that the API transmitted all the data fields in the JSON file but did not
filter them. To solve this problem, our team set up a server on the Raspberry Pi. The
server first receives API data, processes it to extract information about the next bank
holiday, and then forwards it to the physical device. Similarly, the virtual appliance
retrieves the processed data via C# scripts.

Another problem is the inability to move virtual models on virtual devices. The
project is designed to display virtual models using image objects and use Lean Touch
for moving, scaling, and rotating operations. However, during the actual operation, it
was observed that the Lean Touch command did not work, the virtual model could not
move, and was vibrating all the time. After a series of troubleshooting steps, it was
found that the root cause was the constant activation of the image target, which
continuously regenerated the virtual model. To solve this problem, we disabled the
image target script after identifying the image and generating the virtual model. This
ensures that Lean Touch scripts work properly.


---
### IV. Reflection

The Joy Timer project successfully integrates physical devices with digital devices,
showing users data visualization and device interactivity. Among them, the physical
device combines a servo motor to drive the pointer on the dashboard to display the
countdown days of the holiday, coupled with an LED light bar and an SSD
OLED display to supplement the visual information. In the virtual device, the
interaction between the user and the model is realized by AR technology. By solving
a number of technical challenges encountered in the design, Joy Timer achieves a
functional and aesthetic balance, highlighting its future scalability and potential for a
wider range of applications.

However, the project still has some limitations. Firstly, the pointer used for days
countdown has the problem of inaccurate positioning in the stability test, which is
mainly due to the performance limitation of the MG90S steering gear and the
interference of environmental factors. The Angle control accuracy of the MG90S is
about 1°, which is difficult to meet in application scenarios requiring high accuracy,
such as mechanical watches or precision instruments (Tower Pro, n.d.). In addition, in
the combination process of the pointer and the dial, it is inevitably affected by
friction, gravity and inertia, which further aggravates the accumulation of errors. At
the code level, the project realizes through optimization that the steering gear is reset
to 0 degree before using MG90S to drive the pointer each time, and the physical
design ensures that the pointer can accurately point to the digit 0 when the steering
gear is reset. However, despite this, there are still errors. The optimisation currently
envisaged is to use more accurate and torque-efficient steering gear, such as the
Futaba BLS153 and the Savox SH-0255MG. The Futaba BLS153, as a brushless
motor, has a longer service life and uses a high-precision gear set with a positioning
error usually less than 0.1 degrees, even taking into account load, backlash and
external interference, the actual error is only between 0.1 and 0.2 degrees
(ServoDatabase, 2024). The Savox SH-0255MG, on the other hand, uses a brushed
motor while ensuring accuracy, which is relatively inexpensive and cost-effective,
helping to reduce the overall cost of the finished product (Savox Shop, n.d.).

Secondly, consider replacing the currently used ESP32 WROOM 32D controller with
the MKR WiFi 1010. Although the ESP32 WROOM 32D has advantages in
performance and price, it falls short in terms of ease of use, security, and size. As an
official controller launched by Arduino, the MKR WiFi 1010 is fully integrated into
the Arduino ecosystem, allowing it to work more effectively with the Arduino IDE,
significantly simplifying the development process and improving development
efficiency. Additionally, the MKR WiFi 1010 is equipped with the ECC508A
encryption chip, which provides higher data transmission security in IoT applications,
meeting the needs of future scenarios that may involve sensitive data (Arduino, 2025).
Meanwhile, the MKR WiFi 1010 is slightly smaller in size compared to the ESP
WROOM 32D, offering more possibilities for layout optimization in device design,
especially in scenarios with strict requirements for portability and space utilization.

In summary, by optimizing the selection of steering gear and controller, the
performance of the equipment can be significantly improved. These improvements
will effectively improve the accuracy, stability and ease of use of the device, while
providing a solid technical foundation for Joy Timer to achieve more complex
functions in a wider range of application scenarios.

---

### Reference

Espressif Systems, 2023. ESP32-WROOM-32 Datasheet. Version 3.4. Available at:
https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf [Accessed 7 Jan. 2025].

Arduino, 2025. Arduino MKR WiFi 1010 Product Reference Manual. SKU:
ABX00023. Available at: https://docs.arduino.cc/resources/datasheets/ABX00023-datasheet.pdf [Accessed 7 Jan. 2025].

Tower Pro, n.d. MG90S Servo Datasheet. Available at:https://www.electronicoscaldas.com/datasheet/MG90S_TowerPro.pdf [Accessed 7 Jan. 2025].

Trueman, M. & Hartley, J. (1996) ‘A comparison between the time-management
skills and academic performance of mature and traditional-entry university students’,
Higher Education, 32(2), pp. 199–215. Available at:https://doi.org/10.1007/BF00138396 [Accessed: 1 January 2025].

ServoDatabase, 2024. Futaba BLS153 - Brushless Mini Air Servo. Available at:
https://servodatabase.com/servo/futaba/bls153 [Accessed 7 Jan. 2025].

Savox Shop, n.d. Savox SH-0255MG Servo. Available at:https://www.savox-shop.com/en/savox-sh-0255mgservo.html [Accessed 7 Jan. 2025].



---

### Appendix

![Appendix A](https://github.com/XLunaXX07/SDV-report/blob/main/Appendix%20A.png?raw=true)

*Appendix A: JSON Snippet Example for UK Bank Holidays*
    

This JSON structure stores the details of each UK bank holiday, including the name of
the holiday (for example "Christmas"), the date, and so on.

title：Holiday name, for example, "Christmas Day".

date： Date of the holiday.

notes：Notes, if there are no notes, it remains an empty string.

bunting：A Boolean value (true or false) indicating whether decorative flags and
banners are customary to be displayed on holidays. When set to true, it denotes
traditional decorating practices for major celebrations.



---
![Appendix B](https://github.com/XLunaXX07/SDV-report/blob/main/Appendix%20B.png?raw=true)

*Appendix B: JSON Snippet Example e for Chinese Holidays*
    

The data is keyed by date and stores information about holidays.

date：The date of the holiday.

name：The name of the holiday, such as "Spring Festival".

isOffDay： A Boolean value that indicates whether the day is an official holiday. If
true, it denotes that the day is a government-sanctioned non-working day across the
country, such as during national festivals or statutory holidays. If false, it signifies that
it is a regular working day with no official holiday status.

---


![Appendix C](https://github.com/XLunaXX07/SDV-report/blob/main/Appendix%20C.png?raw=true)

*Appendix C: JSON Snippet Example for Course Task Deadlines*
    

This section stores detailed deadline information for assessment tasks across various
courses within this module, organized by course category:

courses：An array of courses, where each course includes the following fields:

name：The name of the course, e.g., "Connected Environment".

code：The course code, e.g., "CASA0014".

deadlines：An array of tasks, with each task containing:

title：The name of the task, e.g., "Blog Post".

type：The type of task, e.g., "Assessment".

date：The deadline date for the task.

time：The time by which the task must be submitted.

---

![Appendix D](https://github.com/XLunaXX07/SDV-report/blob/main/Appendix%20D.JPG?raw=true)

*Appendix D: Early stage design sketches for the shell, illustrating initial ideas for displaying
semester week countdown and holiday countdown*
    

---

![Appendix E](https://github.com/XLunaXX07/SDV-report/blob/main/Appendix%20E.JPG?raw=true)

*Appendix E: Variations of SketchUp models created during Stage 2 of the prototype
development, focusing on simplicity and modularity*
    

---

![Appendix F](https://github.com/XLunaXX07/SDV-report/blob/main/Appendix%20F.png?raw=true)

*Appendix F: Detailed Technical Drawings from the Precision Modeling Process (a)*
    

---

![Appendix G](https://github.com/XLunaXX07/SDV-report/blob/main/Appendix%20G.png?raw=true)

*Appendix G: Detailed Technical Drawings from the Precision Modeling Process (b)*
    

---
