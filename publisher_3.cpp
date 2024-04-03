

#include <mosquitto.h>
#include <iostream>
#include <json/json.h>
#include <cstring>
#include <ctime>


void publish_message(const std::string& topic, const std::string& message) {
    //  spdlog::info("Publishing to {}: {}", topic, message);

    Json::Value root;
    root["message"] = message;
    root["timestamp"] = static_cast<unsigned int>(std::time(nullptr));
    Json::StreamWriterBuilder builder;
    std::string payload = Json::writeString(builder, root);

    struct mosquitto *mosq = mosquitto_new(nullptr, true, nullptr);
    if (mosq) {
        if (mosquitto_connect(mosq, "localhost", 1883, 60) == 0) {
            mosquitto_publish(mosq, nullptr, topic.c_str(), payload.size(), payload.data(), 0, false);
            mosquitto_disconnect(mosq);
        } else {
            //  spdlog::error("Could not connect to broker");
        }
        mosquitto_destroy(mosq);
    } else {
        //  spdlog::error("Could not create Mosquitto instance");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <message>" << std::endl;
        return 1;
    }

    const char* topic = "room/sensor3";
    std::string message = argv[1];

    publish_message("room/sensor3", message); 

    mosquitto_lib_cleanup();
    return 0;
}




