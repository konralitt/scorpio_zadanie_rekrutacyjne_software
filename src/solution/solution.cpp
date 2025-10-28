#include "tester.hpp"
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>

#define M_PI 3.14159265358979323846

int solver(std::shared_ptr<backend_interface::Tester> tester, bool preempt) {
    auto motor1 = tester->get_motor_1(); // poziomy
    auto motor2 = tester->get_motor_2(); // pionowy
    auto commands = tester->get_commands();

    std::cout << (preempt ? "Preempt" : "Queue") << '\n';

    // Callback na nowy punkt
    commands->add_data_callback([&](const Point& point) {
        std::cout << "Nowy cel: (" << point.x << ", " << point.y << ", " << point.z << ")\n";

        // Oblicz k¹ty w radianach
        double yaw = atan2(point.z, point.x); // obrót poziomy (Z wokó³ globalnej osi)
        double pitch = atan2(point.y, sqrt(point.x * point.x + point.z * point.z)); // obrót pionowy

        // W stopniach
        double yaw_deg = yaw * 180.0 / M_PI;
        double pitch_deg = pitch * 180.0 / M_PI;

        // Konwersja na jednostki enkodera
        double yaw_enc = yaw_deg * (4096.0 / 360.0);
        double pitch_enc = pitch_deg * (4096.0 / 360.0);

        std::cout << "Docelowe enkodery: yaw=" << yaw_enc << ", pitch=" << pitch_enc << "\n";

        // Prosty ruch: kierunek zale¿ny od znaku
        int8_t yaw_dir = (yaw_enc >= 0) ? 50 : -50;
        int8_t pitch_dir = (pitch_enc >= 0) ? 50 : -50;

        // Przyk³adowy czas ruchu (w realnym kodzie kontrola enkodera)
        motor1->send_data(yaw_dir);
        motor2->send_data(pitch_dir);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        motor1->send_data(0);
        motor2->send_data(0);
        });

    std::this_thread::sleep_for(std::chrono::seconds(15));
    return 0;
}
