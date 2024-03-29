#ifndef GAZEBO_CUSTOM_SENSOR_PRELOADER_CUSTOMSENSORPRELOADER_H
#define GAZEBO_CUSTOM_SENSOR_PRELOADER_CUSTOMSENSORPRELOADER_H

#include <gazebo/common/Plugin.hh>

#include <memory>
#include <thread>
#include <tuple>
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
   * \brief We need to register the sensors a little bit later, so we defer the
   * calls to a separate thread.
   */
  protected: virtual void DeferredPreloadSensors();

  /// \brief Type of the RegisterCustomSensor function generated by GZ_REGISTER_STATIC_SENSOR.
  typedef void (*registerFuncType)();

  /**
   * \brief Store one custom sensor registration function for deferred loading.
   *
   * \param[in] _type Type of the sensor used in SDF to refer to it.
   * \param[in] _classname Name of the sensor's implementation class (without namespace).
   * \param[in] _fullname Absolute path to the dynamic library with the sensor.
   */
  protected: virtual void ProcessCustomSensor(const std::string& _type,
                                              const std::string& _classname,
                                              const std::string& _fullname);

  /// \brief This thread performs the deferred preloading.
  protected: std::unique_ptr<std::thread> deferredLoadThread;

  /// \brief Registration function, type, classname, fullname.
  typedef std::tuple<registerFuncType, std::string, std::string, std::string> registerTuple;
  /// \brief A set of sensors waiting for deferred registration.
  protected: std::unordered_set<std::shared_ptr<registerTuple> > sensorsToRegister;
};

}

#endif //GAZEBO_CUSTOM_SENSOR_PRELOADER_CUSTOMSENSORPRELOADER_H
