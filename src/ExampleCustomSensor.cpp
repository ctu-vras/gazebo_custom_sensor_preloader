#include <gazebo_custom_sensor_preloader/ExampleCustomSensor.h>
#include <gazebo/sensors/SensorFactory.hh>

// Do not forget to register your sensor via this block of code.
// The first argument is the Gazebo sensor type, which is how you reference the
// custom sensor in SDF. It should also match the 'name' attribute in XML plugin
// definition (together with the 'sensors/' prefix).
using gazebo::sensors::Sensor;
using gazebo::sensors::SensorFactory;
extern "C"
{
GZ_REGISTER_STATIC_SENSOR("example_custom_sensor", ExampleCustomSensor)
}

void gazebo::sensors::ExampleCustomSensor::Load(const std::string &_worldName)
{
  Sensor::Load(_worldName);

  gzmsg << "Example custom sensor loaded" << std::endl;
}

void gazebo::sensors::ExampleCustomSensor::Init()
{
  Sensor::Init();

  gzmsg << "Example custom sensor initialized" << std::endl;
}

// you can also use other sensor categories
gazebo::sensors::ExampleCustomSensor::ExampleCustomSensor()
    : Sensor(gazebo::sensors::SensorCategory::OTHER)
{
}

gazebo::sensors::ExampleCustomSensor::~ExampleCustomSensor() = default;
