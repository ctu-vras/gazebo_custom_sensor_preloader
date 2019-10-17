#include <gazebo_custom_sensor_preloader/CustomSensorPreloader.h>

#include <pluginlib/class_loader.h>
#include <gazebo/sensors/Sensor.hh>

#include <boost/filesystem.hpp>

#include <gazebo/sensors/SensorManager.hh>

// ignition-common is find_packaged by gazebo
#include <ignition/common/StringUtils.hh>

namespace fs = boost::filesystem;

namespace gazebo
{

gazebo::CustomSensorPreloader::CustomSensorPreloader() = default;
CustomSensorPreloader::~CustomSensorPreloader() = default;

void CustomSensorPreloader::Init()
{
  pluginlib::ClassLoader<gazebo::sensors::Sensor> loader(
      "gazebo_custom_sensor_preloader", "gazebo::sensors::Sensor");

  const auto names = loader.getDeclaredClasses();
  for (const auto& name : names)
  {
    std::string type;
    if (ignition::common::StartsWith(name, "sensors/"))
    {
      const auto parts = ignition::common::Split(name, '/');
      if (parts.size() != 2 || parts[1].empty())
      {
        gzerr << "CustomSensorPreloader: Wrong 'name' attribute of custom "
              << "sensor. It should have the form 'sensors/sensor_type', got "
              << name << std::endl;
        continue;
      }
      type = parts[1];
    }
    else
    {
      if (name.find('/') != std::string::npos)
      {
        gzerr << "CustomSensorPreloader: Wrong 'name' attribute of custom "
              << "sensor. It should have the form 'sensors/sensor_type', got "
              << name << std::endl;
        continue;
      }
      else
      {
        gzwarn << "CustomSensorPreloader: Attribute 'name' of custom sensor '"
               << name << "' is missing the 'sensors/' prefix. This may lead "
               << "to name collisions. Consider adding the prefix."
               << std::endl;
        type = name;
      }
    }

    const auto classType = loader.getClassType(name);
    const auto classParts = ignition::common::Split(classType, ':');

    if (classParts.size() != 5)
    {
      gzerr << "CustomSensorPreloader: Attribute 'type' of custom sensor "
            << name << " should have the form 'gazebo::sensors::ClassName', "
            << "got '" << classType << "' instead." << std::endl;
      continue;
    }

    const auto classname = classParts.back();
    const auto fullname = loader.getClassLibraryPath(name);

    if (fullname.empty())
    {
      gzerr << "CustomSensorPreloader: Could not find path to the library of "
            << "custom sensor '" << name << "'." << std::endl;
      continue;
    }
    else if (!fs::exists(fullname))
    {
      gzerr << "CustomSensorPreloader: Library '" << fullname << "' of custom "
            << "sensor '" << name << "' does not exists." << std::endl;
      continue;
    }

    this->ProcessCustomSensor(type, classname, fullname);
  }

  for (const auto& libraryDir : this->libraryDirs)
  {
    gzmsg << "CustomSensorPreloader: Adding GAZEBO_PLUGIN_PATH " << libraryDir
          << std::endl;
    gazebo::common::SystemPaths::Instance()->AddPluginPaths(libraryDir);
  }
}

void CustomSensorPreloader::ProcessCustomSensor(const std::string& _type,
                                                const std::string& _classname, const std::string& _fullname)
{

  const auto path = fs::path(_fullname);
  auto filename = path.filename().string();
  auto libraryDir = path.parent_path();

  this->libraryDirs.insert(libraryDir.string());

  // this implementation is inspired by gazebo::common::Plugin<T>::Create()

#ifdef __APPLE__
  {
    // replace .so with .dylib
    size_t soSuffix = filename.rfind(".so");
    if (soSuffix != std::string::npos)
    {
      const std::string macSuffix(".dylib");
      filename.replace(soSuffix, macSuffix.length(), macSuffix);
    }
  }
#elif _WIN32
  {
    // replace .so with .dll
    size_t soSuffix = filename.rfind(".so");
    if (soSuffix != std::string::npos)
    {
      const std::string winSuffix(".dll");
      filename.replace(soSuffix, winSuffix.length(), winSuffix);
    }
    size_t libPrefix = filename.find("lib");
    if (libPrefix == 0)
    {
      // remove the lib prefix
      filename.erase(0, 3);
    }
  }
#endif  // ifdef __APPLE__

  const auto fullname = (libraryDir / fs::path(filename)).string();

  void *dlHandle = dlopen(fullname.c_str(), RTLD_LAZY|RTLD_GLOBAL);
  if (!dlHandle)
  {
    gzerr << "CustomSensorPreloader: Failed to load custom sensor library "
          << fullname << ": " << dlerror() << std::endl;
    return;
  }

  // The GZ_REGISTER_STATIC_SENSOR macro generates function RegisterClassName()
  typedef void (*registerFuncType)();
  const auto registerFuncName = "Register" + _classname;
  const auto registerFunc = (registerFuncType) dlsym(dlHandle, registerFuncName.c_str());

  if (!registerFunc)
  {
    gzerr << "CustomSensorPreloader: Failed to resolve registration function "
          << registerFuncName << ": " << dlerror() << std::endl
          << "Did you call GZ_REGISTER_STATIC_SENSOR(\"" << _type << "\", "
          << _classname << ") inside your sensor source file?" << std::endl;
    return;
  }

  // Call the registration function
  (*registerFunc)();

  std::vector<std::string> types;
  sensors::SensorManager::Instance()->GetSensorTypes(types);

  if (std::find(types.begin(), types.end(), _type) == types.end())
  {
    gzwarn << "CustomSensorPreloader: Custom sensor " << _classname
           << " from library " << fullname
           << " was preloaded, but it did not register a sensor of type "
           << _type << std::endl;
  }
  else
  {
    gzmsg << "CustomSensorPreloader: Preloaded custom sensor " << _classname
          << " from library " << fullname << std::endl;
  }
}

void CustomSensorPreloader::Load(int _argc, char **_argv)
{
  // Nothing to do here, everything happens in Init()
}


void CustomSensorPreloader::Reset()
{
  // nothing to do here, everything happens in Init()
}

}

GZ_REGISTER_SYSTEM_PLUGIN(gazebo::CustomSensorPreloader)