#ifndef GAZEBO_CUSTOM_SENSOR_PRELOADER_EXAMPLECUSTOMSENSOR_H
#define GAZEBO_CUSTOM_SENSOR_PRELOADER_EXAMPLECUSTOMSENSOR_H

#include <gazebo/sensors/Sensor.hh>

// It is very important that your custom sensor class is defined inside the
// gazebo::sensors namespace.

namespace gazebo
{
namespace sensors
{

class ExampleCustomSensor : public Sensor
{
  public: ExampleCustomSensor();
  public: ~ExampleCustomSensor() override;
  public: void Load(const std::string &_worldName) override;
  public: void Init() override;
};

}
}

#endif //GAZEBO_CUSTOM_SENSOR_PRELOADER_EXAMPLECUSTOMSENSOR_H
