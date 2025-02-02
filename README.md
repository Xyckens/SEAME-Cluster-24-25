# **JetPackJoyRide - Cluster Implementation**

## **Introduction**
The JetRacer Instrument Cluster Qt Application project aims to create a real-time speedometer for a JetRacer car. The application will run on a Raspberry Pi 4 and use the Controller Area Network (CAN) protocol to receive speed data from a speed sensor. This project provides hands-on experience in software engineering, including embedded systems, communication protocols, and GUI development with Qt, a framework widely used in the automotive industry.

The successful completion of this project demonstrates the ability to design and implement real-world software solutions and effectively communicate results.

Code documentation can be found here:  [JetRacer Documentation](https://xyckens.github.io/SEAME-Cluster-24-25/)

---

## **Background Information**
This project involves the following key technologies:

- **Raspberry Pi**: A powerful, low-cost AI computer ideal for embedded applications and robotics projects.
- **JetRacer**: A small autonomous vehicle platform based on the Jetson Nano, suitable for testing and experimentation.
- **CAN Protocol**: A robust communication standard for vehicle ECUs (Electronic Control Units).
- **Qt Framework**: A popular tool for creating cross-platform GUIs, especially in automotive applications.

---

## **Goals and Objectives**
### **Main Goal**
To develop a functional instrument cluster for the JetRacer car, displaying real-time speed data from a speed sensor via the CAN bus.

### **Specific Objectives**
- Design and implement a Qt-based graphical user interface (GUI).
- Integrate a speed sensor system with the Raspberry Pi 4 using the CAN bus protocol.
- Display real-time speed data on the instrument cluster GUI.
- Test functionality using realistic driving scenarios.
- Document the project, including system architecture, design, and implementation.

**Bonus Objective:** Display battery level on the instrument cluster.

---

## **Technical Requirements**
### **Hardware**
- **Raspberry Pi 4**: Ideal for handling the computational requirements of real-time GUI and CAN data processing.
- **CAN Bus Interface**: (e.g., MCP2515 or TJA1050) for interfacing with the Raspberry Pi 4.
- **Speed Sensor**: A CAN-compatible speed sensor for data acquisition.
- **OBD-II Cable**: Connects the Raspberry Pi 4 and speed sensor to the vehicle’s CAN bus.
- **Voltage Regulator**: Converts 12V to 5V for powering the Raspberry Pi 4 from the vehicle's electrical system.
- **Display**: LCD/OLED display for GUI visualization.
- **Mounting Hardware**: Brackets or enclosures for securely mounting the display and Raspberry Pi 4 on the JetRacer.
- **JetRacer Vehicle**: A small-scale autonomous car platform with a CAN bus system.

### **Software**
- **Qt Framework**: Open-source version for creating GUI applications.
- **CAN Libraries**: Suitable libraries for CAN communication.

---

## **Software Design**
- **Main Application**: Manages GUI, CAN communication, and speed display.
- **CAN Communication Module**: Interfaces with the speed sensor and processes data.
- **Display Module**: Displays real-time speed data using customizable formats.

---

## **Implementation**
- **Code Development**: Key components written in C++ using Qt.
- **Testing**: Verification of real-time speed display.
- **Deployment**: Application setup and execution on Jetson Nano.

---

## **Testing Plan**
*(To be detailed later)*  
- **Functional Testing**: Verifying data acquisition and display.
- **Usability Testing**: Ensuring user-friendly interface.
- **Performance Testing**: Evaluating system response under various conditions.

## **Submission Checklist**
*(To be updated upon submission)*  
- [ ] Code and configuration files.
- [ ] Algorithm and data structure descriptions.
- [ ] UI screenshots and diagrams.
- [ ] Test plans, cases, and results.
- [ ] Hardware component details.
- [ ] References and additional materials.

