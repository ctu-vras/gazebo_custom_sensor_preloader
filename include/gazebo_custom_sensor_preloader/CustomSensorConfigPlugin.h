#ifndef GAZEBO_CUSTOM_SENSOR_PRELOADER_CUSTOMSENSORCONFIGPLUGIN_H
#define GAZEBO_CUSTOM_SENSOR_PRELOADER_CUSTOMSENSORCONFIGPLUGIN_H

#include <gazebo/common/Plugin.hh>

namespace gazebo
{

class CustomSensorConfigPlugin : public SensorPlugin
{
  public: void Load(sensors::SensorPtr _sensor, sdf::ElementPtr _sdf) override;
};

}

#endif //GAZEBO_CUSTOM_SENSOR_PRELOADER_CUSTOMSENSORCONFIGPLUGIN_H
