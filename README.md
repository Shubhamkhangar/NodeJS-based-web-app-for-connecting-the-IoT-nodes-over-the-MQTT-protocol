# NodeJS-based-web-app-for-connecting-the-IoT-nodes-over-the-MQTT-protocol

#Abstract
The IoT-Based Control and Monitoring System is a comprehensive project that integrates NodeMCU, Node.js, and HTML to create a robust Internet of Things (IoT) application. The project focuses on remotely controlling and monitoring through a user-friendly web interface with the lightweight MQTT protocol from anywhere in the world with the help of the URL address of the web application.
The NodeMCU microcontroller is employed to manage the LM35 for temperature readings as well as control three LEDs for the demonstration of the actuator control operation. The device connects to a Wi-Fi network and communicates with an MQTT broker for seamless data exchange.
On the software side, the Node.js server, powered by the Express framework, acts as an intermediary between the NodeMCU and the web interface. It subscribes to MQTT topics, receives real-time updates from the NdeMCU, and broadcasts this information to connected clients via a webpage application.
The web interface, crafted using HTML and the Pug template engine, provides users with an intuitive dashboard. Actuator unit states and sensor data are dynamically displayed, and users can interact with the system by pressing virtual buttons on the webpage. The webpage refreshes periodically to reflect the most recent data.
The communication flow ensures a continuous exchange of information, enabling users to remotely control the actuator and monitor environmental conditions in real time. The system's modular design allows for scalability and potential integration with additional IoT devices.
This project serves as a practical demonstration of IoT concepts, showcasing the seamless integration of hardware (NodeMCU) and software (Node.js and HTML) components to create an accessible and responsive IoT application.

* Hosting the Node.js app on the render web hosting services









