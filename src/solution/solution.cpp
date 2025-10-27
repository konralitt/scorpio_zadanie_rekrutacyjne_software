#include "tester.hpp"

// You can remove or add any includes you need
#include <chrono>
#include <iostream>
#include <thread>
#include <cmath>

int solver(std::shared_ptr<backend_interface::Tester> tester, bool preempt) {
  // Short example you can remove it
  std::cout << (preempt ? "Preempt" : "Queue") << '\n';
  auto motor1 = tester->get_motor_1();
  auto motor2 = tester->get_motor_2();
  auto commands = tester->get_commands();
  motor1->add_data_callback([](const uint16_t& data) {
    std::cout << "Motor 1 data: " << static_cast<int>(data) << "\n";
  });
  motor2->add_data_callback([](const uint16_t& data) {
    std::cout << "Motor 2 data: " << static_cast<int>(data) << "\n";
  });
  commands->add_data_callback([](const Point& point) {
    std::cout << "Command point: (" << point.x << ", " << point.y << ", " << point.z << ")\n";
    double obrotWokolOsiPionowej = atan2(point.x, point.z);
    double obrotWokolOsiPoziomej = atan2(point.y, sqrt((point.x* point.x + point.z* point.z)));
    std::cout << "Katy:(" << obrotWokolOsiPionowej << ", " << obrotWokolOsiPoziomej << ")\n";
  });
  std::this_thread::sleep_for(std::chrono::seconds(15));
  
  return 0;
}
