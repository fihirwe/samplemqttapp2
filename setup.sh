#!/bin/sh

#install cmake
sudo apt install cmake
sudo apt-get install pkg-config

# Install Mosquitto (MQTT Broker and Client library)
sudo apt-get install -y mosquitto mosquitto-clients libmosquitto-dev

# Install spdlog (Logging library)
# sudo apt-get install -y libspdlog-dev
# sudo apt-get install --only-upgrade libfmt-dev
# sudo apt-get install --only-upgrade libspdlog-dev


# Install JsonCpp (JSON handling library)
sudo apt-get install -y libjsoncpp-dev

# Install Qt5 for GUI applications
sudo apt-get install qt5-qmake qtbase5-dev
sudo apt-get install libqt5widgets5

# Start the Mosquitto broker
sudo systemctl start mosquitto

# Enable Mosquitto broker to start at boot
sudo systemctl enable mosquitto

echo "Installation complete. Mosquitto MQTT broker is running."
echo "Logs for Mosquitto can be found in /var/log/mosquitto/mosquitto.log"
