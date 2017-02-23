#include "Parser.hpp"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

#include <functional>

OpenCLWrapper::OpenCLWrapper() { this->reloadPlatformFound(); }

bool operator==(cl::Device &a, const cl::Device &b) {
  return a.getInfo<CL_DEVICE_NAME>() == b.getInfo<CL_DEVICE_NAME>();
}

void OpenCLWrapper::selectByDefault() { this->selectByString("0.0"); }

void OpenCLWrapper::selectAll() {

  for (auto a : this->list_plat_list_dev) {
    for (auto d : a.second)
      this->select_device.push_back(d);
  }
}

bool OpenCLWrapper::reloadPlatformFound() {

  std::vector<cl::Platform> all_platforms;
  cl::Platform::get(&all_platforms);

  if (all_platforms.size() == 0) {
    std::cout << " No platforms found. Check OpenCL installation!\n";
    exit(1);
  }

  for (auto one_platform : all_platforms) {

    std::vector<cl::Device> all_devices;
    one_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);

    if (all_devices.size() == 0) {
      std::cout << " No devices found. Check OpenCL installation!\n";
      continue;
    }

    this->list_plat_list_dev.push_back(
        std::make_pair(one_platform, all_devices));
  }

  return true;
}

void OpenCLWrapper::print_plat_dev() {

  int ip = 0;

  for (auto p : this->list_plat_list_dev) {
    std::cout << "| " << ip++ << ":" << p.first.getInfo<CL_PLATFORM_NAME>()
              << "\n";

    int id = 0;
    for (auto d : p.second) {

      if (std::find_if(this->select_device.begin(), this->select_device.end(),
                       [d](const cl::Device &dec) {
                         return d.getInfo<CL_DEVICE_NAME>() ==
                                dec.getInfo<CL_DEVICE_NAME>();
                       }) != this->select_device.end())
        std::cout << "*";

      std::cout << "L> " << ip << "," << id++ << ":"
                << d.getInfo<CL_DEVICE_NAME>() << "\n";
    }
  }
}

bool OpenCLWrapper::selectByString(std::string select) {

  std::vector<cl::Platform> all_platforms;
  cl::Platform::get(&all_platforms);

  auto found = select.find("*");
  if (found != std::string::npos) {
    this->selectAll();
    return true;
  }

  std::vector<std::string> all_select_parsing_plat_dev;
  boost::split(all_select_parsing_plat_dev, select, boost::is_any_of(" "));

  for (auto one_plat_dev : all_select_parsing_plat_dev) {

    unsigned int ip;
    unsigned int id;
    bool allDeviceOfThisPlatformGlobal = false;

    try {
      std::vector<std::string> plat_dev_string;
      boost::split(plat_dev_string, one_plat_dev, boost::is_any_of("."));

      if (plat_dev_string.size() != 2) {
        std::cout << "error with string is not platform.device : "
                  << one_plat_dev << "\n";
      }

      ip = boost::lexical_cast<unsigned int>(plat_dev_string[0]);
      if (plat_dev_string[1] == "*")
        id = boost::lexical_cast<unsigned int>(plat_dev_string[1]);
      else
        allDeviceOfThisPlatformGlobal = false;
    } catch (boost::bad_lexical_cast const &) {
      std::cout << "Error: input string was not valid" << std::endl;
      continue;
    }

    if (ip < 0) {
      std::cout << "Platfom number is less than 0"
                << "\n";
      continue;
    }

    if (ip >= all_platforms.size()) {
      std::cout << "Platfom number is more important than platform available ("
                << all_platforms.size() << ")"
                << "\n";
      continue;
    }

    auto dev_list = this->list_plat_list_dev[ip].second;
    if (allDeviceOfThisPlatformGlobal) {
      
      return;
    }
    if (id < 0) {
      std::cout << "Device number is less than 0"
                << "\n";
      continue;
    }

    if (id >= dev_list.size()) {
      std::cout << "Device number is more important than Device available "
                   "for "
                << all_platforms[ip].getInfo<CL_PLATFORM_NAME>()
                << " include [|0," << dev_list.size() << "|]"
                << "\n";
      continue;
    }

    this->select_device.push_back(dev_list[id]);
  }
}
