/*
  modificacion alpha
*/
#include <iostream>
#include <boost/asio.hpp>
#include "vmu931/commands.hpp"
#include "vmu931/sensor.hpp"
#include "vmu931/types.hpp"
#include <boost/date_time.hpp>


boost::posix_time::ptime now = boost::date_time::not_a_date_time;
boost::posix_time::ptime last = boost::date_time::not_a_date_time;


int main()
{
    last = boost::posix_time::microsec_clock::universal_time();
    boost::asio::io_service io_service;
    //boost::asio::serial_port serial_port(io_service, "/dev/ttyACM0");
    boost::asio::serial_port serial_port(io_service, "/dev/tty.usbmodem931901443E");


    vmu931::Sensor sensor(std::move(serial_port));
    sensor.register_sink([](vmu931::Accelerometers accel) {
        std::cout << "accel: x=" << accel.x << " y=" << accel.y << " z=" << accel.z <<"\n";
    });
    sensor.register_sink([](vmu931::Gyroscopes gyro) {
        std::cout << "gyro: x=" << gyro.x << " y=" << gyro.y << " z=" << gyro.z <<"\n";
    });
    sensor.register_sink([](vmu931::Magnetometers magneto) {
        std::cout << "magneto: x=" << magneto.x << " y=" << magneto.y << " z=" << magneto.z <<"\n";
    });
    sensor.register_sink([](vmu931::EulerAngles euler) {
      now = boost::posix_time::microsec_clock::universal_time();
      //std::cout<<now-last<<std::endl;
      if(now-last >= boost::posix_time::milliseconds(100.0)) {
        std::cout <<now <<std::endl;
        //std::cout << "euler: x=" << euler.x << " y=" << euler.y << " z=" << euler.z <<"\n";
        last = now;
      }
    });
    sensor.register_sink([](vmu931::Quaternions quat) {
        std::cout << "quat: w=" << quat.w << " x=" << quat.x << " y=" << quat.y << " z=" << quat.z <<"\n";
    });
    sensor.register_sink([](vmu931::Heading h) {
      now = boost::posix_time::microsec_clock::universal_time();
      std::cout <<now <<std::endl;
        std::cout << "heading: " << h.heading << "\n";
    });
    sensor.register_sink([](std::string s) {
        std::cout << "message: " << s << "\n";
    });
    sensor.register_sink([](vmu931::Status status) {
        std::cout << "VMU931 status:\n"
            << " - streams: " << status.streaming() << "\n"
            << " - gyroscopes: " << status.resolution_gyroscopes() << " dps\n"
            << " - accelerometers: " << status.resolution_accelerometers() << " g\n"
            << " - output rate: " << status.output_rate() << " Hz\n";
    });

    io_service.post([&sensor]() {
      sensor.set_streams({
        //vmu931::commands::Accelerometers,
        //vmu931::commands::Gyroscopes,
        //vmu931::commands::Magnetometers,
        vmu931::commands::EulerAngles,
        //vmu931::commands::Quaternions,
        //vmu931::commands::Heading,
        //vmu931::commands::Status
      });
    });

    std::cout << "Start reading VMU931 sensor stream...\n";
    io_service.run();
    /*while(true) {
      auto start = boost::posix_time::microsec_clock::universal_time();
      io_service.poll_one();
      auto stop = boost::posix_time::microsec_clock::universal_time();
      auto diff = stop - start;
      auto to_sleep = boost::posix_time::seconds(1.0) - diff;
      usleep(to_sleep.total_microseconds());
    }*/
    return 0;
}
