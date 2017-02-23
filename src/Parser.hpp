#pragma once

#include <CL/cl.hpp>
#include <map>
#include <string>
#include <vector>



class OpenCLWrapper {

  std::vector<
    std::pair<
      cl::Platform,
      std::vector<cl::Device>
      >
    > list_plat_list_dev;

  std::vector<cl::Device> select_device;

public:
  OpenCLWrapper();

  void getInfo();
  void print_plat_dev();
  bool selectByString(std::string select);
  void selectByDefault();
  void selectAll();

protected:
  bool reloadPlatformFound();
  bool Compile();
};


