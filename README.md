# samplemqttapp

## Building and running app

  * Clone the repo
    ```bash
      cd samplemqttapp
    ```
  * Setup neccesary dependecny and build
    ```bash
      ./setup.sh
      ./run.sh
    ```
    The ** `setup.sh`** script do the following
        1. Installs Mosquitto (MQTT Broker and Client library) along with its development headers.
        2. Installs the JsonCpp library for JSON handling.
        3. Optionally installs Qt5 and its development headers for GUI applications
        4. Enables the Mosquitto broker to start at boot.

    The ** `run.sh`** script do the following
        1. Remove the existing `build` directory.
        2. Create a new `build` directory.
        3. Change the current directory to `build`.
        4. Run CMake to configure the build.
        5. Build the project using `make`.
        

  * Run the publisher 1 or 2
    ```bash
      publisher_1 <MESSAGE>
      publisher_1 <MESSAGE>
    ``` 
  * Run the subscriber to receive the publised messages
  ```bash
    subscribergui_1
  ``` 

# samplemqttapp2
