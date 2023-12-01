# IF Receiver Control System Project

## Overview

The IF Receiver Control System is designed for the precise control and manipulation of an Intermediate Frequency (IF) receiver. Combining hardware and software components, this project enables users to dynamically adjust input attenuation and phase angles, optimizing the performance of the IF receiver.

## Key Components

### Printed Circuit Board (PCB)

The heart of the system lies in the meticulously designed Printed Circuit Board (PCB), created using Eagle CAD. The PCB serves as the interface between the control system and the IF receiver, housing the RF control, power distribution, and interfacing components.

### Firmware Development

The intelligence behind the control system is embedded in the firmware, developed using MPLAB for PIC microcontrollers. This firmware ensures precise execution of user commands, facilitating seamless communication with the IF receiver.

### Graphical User Interface (GUI)

Users interact with the system through an intuitive Graphical User Interface (GUI) developed using Python Tkinter. The GUI provides a centralized control hub, offering real-time feedback on the system's status and enabling users to make dynamic adjustments.

### RF Components

The IF receiver features an RF section incorporating digital attenuators and analog phase shifters. These components play a crucial role in fine-tuning the RF signals according to user requirements.

## Functionalities

- **Dynamic Adjustment:** Users can dynamically adjust input attenuation and phase angles, tailoring the IF receiver's performance to specific needs.
  
- **Real-time Feedback:** The GUI provides real-time feedback, enhancing user understanding and control.

- **Modular Design:** The project follows a modular design philosophy, allowing for scalability and future enhancements.

## Technological Stack

- **Hardware:** PCB designed in Eagle CAD, RF components for digital attenuators and analog phase shifters.
  
- **Firmware:** Developed using MPLAB for PIC microcontrollers.
  
- **User Interface:** GUI created with Python Tkinter.
  
- **Communication:** PySerial library for reliable serial communication.

## Conclusion

The IF Receiver Control System showcases the synergy of hardware and software technologies. With its intuitive GUI, precise adjustments, and real-time feedback, this project stands as a testament to the engineering excellence achieved in optimizing IF receiver performance.
