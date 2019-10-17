#ifndef GAZEBO_CUSTOM_SENSOR_PRELOADER_CUSTOMSENSORPRELOADER_H
#define GAZEBO_CUSTOM_SENSOR_PRELOADER_CUSTOMSENSORPRELOADER_H

#include <gazebo/common/Plugin.hh>

#include <unordered_set>

namespace gazebo
{

/**
 * \brief Gazebo system plugin which allows using custom sensors.
 */
class CustomSensorPreloader : public SystemPlugin
{
  public: CustomSensorPreloader();
  public: ~CustomSensorPreloader() override;
  public: void Load(int _argc, char **_argv) override;
  public: void Init() override;
  public: void Reset() override;

  /**
   * \brief Preload one custom sensor and add its library directory to
   * this->libraryDirs.
   *
   * \param[in] _type Type of the sensor used in SDF to refer to it.
   * \param[in] _classname Name of the sensor's implementation class (without namespace).
   * \param[in] _fullname Absolute path to the dynamic library with the sensor.
   */
  protected: virtual void ProcessCustomSensor(const std::string& _type,
                                              const std::string& _classname,
                                              const std::string& _fullname);

  /// \brief Set of directories which should be added to Gazebo plugins path.
  protected: std::unordered_set<std::string> libraryDirs;
};

}

#endif //GAZEBO_CUSTOM_SENSOR_PRELOADER_CUSTOMSENSORPRELOADER_H
