#  EDF scheduler based on FreeRTOS
 - Earliest Deadline First scheduler based on FreeRTOS and simulated on ARM7_LPC2129_Keil_RVDS

## Implementation


 - All the modifications were made to tasks.c and FreeRTOSConfig.h files to convert the default FreeRTOS to an EDF scheduler
 ---
 - All the modifications were made under the guard
 

> configUSE_EDF_SCHEDULER

to ensure that all the modifications could be reversed or used again at any time

---
- All the modifications are marked by this comment to be found easily between the FreeRTOS code lines


> /*EDF Related EDF Related EDF Related */


## Application
- Task 1: ""Button_1_Monitor"", {Periodicity: 50, Deadline: 50}  
This task will monitor rising and falling edge on button 1 and send this event to the consumer task (The rising and failling edges are treated as separate events, hence they have separate strings)
---
- Task 2: ""Button_2_Monitor"", {Periodicity: 50, Deadline: 50}  
This task will monitor rising and falling edge on button 2 and send this event to the consumer task
---

- Task 3: ""Periodic_Transmitter"", {Periodicity: 100, Deadline: 100}  
This task will send preiodic string every 100ms to the consumer task
---

- Task 4: ""Uart_Receiver"", {Periodicity: 20, Deadline: 20}  
This is the consumer task which will write on UART any received string from other tasks  
---

-  5th and 6th task to simulate a heavier load:

	- Task 5: ""Load_1_Simulation"", {Periodicity: 10, Deadline: 10}, Execution time: 5ms  
	- Task 6: ""Load_2_Simulation"", {Periodicity: 100, Deadline: 100}, Execution time: 12ms






## Software Used

 1. Keil uVision5 
 2. SimSo for scheduling process simulation
    

## Explaination Video

> **You can watch the explaination video of the application in arabic** [here](https://drive.google.com/file/d/1gdXSZnJTe36nWYcXz5DLYc6Su9fh8fnu/view?usp=sharing) .


## Directories:

+ The **FreeRTOS/Source** directory contains the FreeRTOS source code, and contains
  its own readme file.

+ The **FreeRTOS/Demo** directory contains the demo application for (ARM7_LPC2129_Keil_RVDS) 
+ The **FreeRTOS/Simso** directory contains the six-task-application file for SimSo software
--- 
This project is based on some of the modifications mentioned in the paper "[Implementation and Test of EDF and LLREFSchedulers in FreeRTOS](http://tesi.cab.unipd.it/51896/1/Implementation_and_Test_of_EDF_and_LLREF_Scgheduler_in_FreeRTOS.pdf)" by  [Enrico Carraro](https://www.semanticscholar.org/author/Enrico-Carraro/2057428125)

---




$$
  \int_0^\infty\infty  dt 
$$
