#include <QApplication>
#include <QTextEdit>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <mosquitto.h>
#include <json/json.h>
#include <iostream>
#include <QTextCursor>


class Subscriber : public QTextEdit {
public:
    Subscriber(QWidget *parent = nullptr) : QTextEdit(parent) {
        setReadOnly(true);
        qRegisterMetaType<QTextCursor>("QTextCursor");

        // The database
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("./mqtt_messages.db");

        if (!db.open()) {
            std::cerr << "Failed to open database: " << db.lastError().text().toStdString() << std::endl;
            return;
        }

        // Create the messages table if it does not exist
        QSqlQuery query;
        if (!query.exec("CREATE TABLE IF NOT EXISTS messages ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                        "topic TEXT, "
                        "payload TEXT, "
                        "received_at DATETIME DEFAULT CURRENT_TIMESTAMP)")) {
            std::cerr << "Failed to create table: " << query.lastError().text().toStdString() << std::endl;
            return;
        }

        // Mosquitto
        mosquitto_lib_init();
        mosq = mosquitto_new(nullptr, true, this);
        if (!mosq) {
            std::cerr << "Failed to create Mosquitto instance." << std::endl;
            return;
        }

        mosquitto_message_callback_set(mosq, [](struct mosquitto*, void* obj, const struct mosquitto_message* msg) {
            auto* self = static_cast<Subscriber*>(obj);
            std::string payload(static_cast<const char*>(msg->payload), msg->payloadlen);

            // Attempt to parse the message as JSON
            Json::Reader reader;
            Json::Value root;
            if (reader.parse(payload, root)) {
                // Successfully parsed as JSON
                std::string messageContent = root.toStyledString();  // Convert the JSON object to a styled string
                self->append(QString::fromStdString("[" + std::string(msg->topic) + "]: " + messageContent));

                // Insert the message into the database
                QSqlQuery query;
                query.prepare("INSERT INTO messages (topic, payload) VALUES (:topic, :payload)");
                query.bindValue(":topic", QString::fromStdString(msg->topic));
                query.bindValue(":payload", QString::fromStdString(messageContent));
                if (!query.exec()) {
                    std::cerr << "Failed to insert message into database: " << query.lastError().text().toStdString() << std::endl;
                }
            } else {
                // Log error if parsing fails
                std::cerr << "Failed to parse message as JSON: " << payload << std::endl;
            }
        });

        // Connect to the MQTT broker
        if (mosquitto_connect_async(mosq, "localhost", 1883, 60) != 0) {
            std::cerr << "Could not connect to MQTT broker." << std::endl;
        } else {
            // Subscribe to topics as needed
            mosquitto_subscribe(mosq, nullptr, "room/sensor1", 0);
            mosquitto_subscribe(mosq, nullptr, "room/sensor2", 0);
            mosquitto_subscribe(mosq, nullptr, "room/sensor3", 0);
            mosquitto_loop_start(mosq);
        }
    }

    ~Subscriber() {
        if (mosq) {
            mosquitto_loop_stop(mosq, true);
            mosquitto_disconnect(mosq);
            mosquitto_destroy(mosq);
        }
        mosquitto_lib_cleanup();
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection);
    }

private:
    mosquitto* mosq;
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    Subscriber subscriber;
    subscriber.setWindowTitle("MQTT Subscriber GUI");
    subscriber.show();

    return app.exec();
}


