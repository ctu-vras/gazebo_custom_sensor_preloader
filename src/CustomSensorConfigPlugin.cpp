#include <gazebo_custom_sensor_preloader/CustomSensorConfigPlugin.h>

namespace gazebo
{

void CustomSensorConfigPlugin::Load(sensors::SensorPtr _sensor, sdf::ElementPtr _sdf)
{
  // we don't need to do anything here; all this plugin serves for is being a
  // container for configuration options of a custom sensor
}

}

GZ_REGISTER_SENSOR_PLUGIN(gazebo::CustomSensorConfigPlugin)