/*
  modificacion alpha, ~200hz, escritura en: cout and file
*/
#include <iostream>
#include <boost/asio.hpp>
#include "vmu931/commands.hpp"
#include "vmu931/sensor.hpp"
#include "vmu931/types.hpp"
#include <boost/date_time.hpp>
#include <fstream>


  boost::posix_time::ptime now = boost::date_time::not_a_date_time;
  boost::posix_time::ptime last = boost::date_time::not_a_date_time;


int main()
{
    std::ofstream myfile;
    myfile.open("Data.txt");

    last = boost::posix_time::microsec_clock::universal_time();
    boost::asio::io_service io_service;
    boost::asio::serial_port serial_port(io_service, "/dev/ttyACM0");
    //boost::asio::serial_port serial_port(io_service, "/dev/tty.usbmodem931901443E");

    vmu931::Sensor sensor(std::move(serial_port));

    sensor.register_sink([&myfile](vmu931::Accelerometers accel) {
        myfile <<" " <<accel.x <<" " <<accel.y <<" " <<accel.z;
        std::cout <<" accelX=" <<accel.x <<" " <<accel.y <<" " <<accel.z;
    });

    sensor.register_sink([&myfile](vmu931::Gyroscopes gyro) {
        myfile <<" " <<gyro.x <<" " <<gyro.y <<" " <<gyro.z;
        std::cout <<" gyroX=" <<gyro.x <<" " <<gyro.y <<" " <<gyro.z;
    });

    sensor.register_sink([](vmu931::Magnetometers magneto) {
        std::cout << "magneto: x=" << magneto.x << " y=" << magneto.y << " z=" << magneto.z <<"\n";
    });

    sensor.register_sink([&myfile](vmu931::EulerAngles euler) {
        myfile <<" " <<euler.x <<" " <<euler.y <<" " <<euler.z;
        std::cout <<" eulX=" <<euler.x <<" " <<euler.y <<" " <<euler.z;
    });

    sensor.register_sink([&myfile](vmu931::Quaternions quat) {
      now = boost::posix_time::microsec_clock::universal_time();
      auto diff = now-last;
      last = now;
        myfile <<now <<" " <<diff.total_microseconds();
        std::cout <<now <<" " <<diff.total_microseconds();
        myfile <<" " <<quat.w <<" " <<quat.x <<" " <<quat.y <<" " <<quat.z;
        std::cout <<" quatW=" <<quat.w <<" " <<quat.x <<" " <<quat.y <<" " <<quat.z;
    });

    sensor.register_sink([&myfile](vmu931::Heading h) {
        myfile <<" " <<h.heading <<std::endl;
        std::cout <<" head=" <<h.heading <<std::endl;
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
        vmu931::commands::Accelerometers,
        vmu931::commands::Gyroscopes,
        //vmu931::commands::Magnetometers,
        vmu931::commands::EulerAngles,
        vmu931::commands::Quaternions,
        vmu931::commands::Heading,
        //vmu931::commands::Status
      });
    });

    std::cout << "Start reading VMU931 sensor stream...\n";
    myfile <<"UNITS: Diftime[microsec] /*/ accel[g] /*/ gyro[dps] /*/ euler[degres] /*/ heading[degres]" <<std::endl;
    std::cout <<"UNITS: Diftime[microsec] /*/ accel[g] /*/ gyro[dps] /*/ euler[degres] /*/ heading[degres]" <<std::endl;

    myfile <<"Date Time " <<"Diftime[microsec] " <<"/quatW " <<"quatX " <<"quatY " <<"quatZ " <<"/accelX " <<"accelY " <<"accelZ "
      <<"/gyroX " <<"gyroY " <<"gyroZ " <<"/eulerX "  <<"eulerY " <<"eulerZ " <<"/heading" <<std::endl;
    std::cout <<"Date Time " <<"Diftime[microsec] " <<"/quatW " <<"quatX " <<"quatY " <<"quatZ " <<"/accelX " <<"accelY " <<"accelZ "
      <<"/gyroX " <<"gyroY " <<"gyroZ " <<"/eulerX "  <<"eulerY " <<"eulerZ " <<"/heading" <<std::endl;

    io_service.run();

  return 0;
}
