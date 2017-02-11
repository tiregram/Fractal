
#include <CL/cl.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <fstream>
#include <iostream>
#include <string>

#include "Image.hpp"

using namespace std;

string readAllFile(ifstream &in) {
  stringstream sstr;
  sstr << in.rdbuf();
  return sstr.str();
}

int main(int argc, char *argv[]) {

  std::vector<cl::Platform> all_platforms;
  cl::Platform::get(&all_platforms);

  if (all_platforms.size() == 0) {
    std::cout << " No platforms found. Check OpenCL installation!\n";
    exit(1);
  }

  int i = 0;

  for (auto one_platform : all_platforms) {
    std::cout << i++ << ":" << one_platform.getInfo<CL_PLATFORM_NAME>() << "\n";
    std::vector<cl::Device> all_devices;
    one_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);

    if (all_devices.size() == 0) {
      std::cout << " No devices found. Check OpenCL installation!\n";
      continue;
    }

    int id = 0;
    for (auto one_device : all_devices) {
      std::cout << "\t" << id << ":" << one_device.getInfo<CL_DEVICE_NAME>()
                << "\n";
    }
  }

  std::string plat_dev;

  std::cout << "select platforms.device"
            << "\n";

  // std::cin >> plat_dev;
  plat_dev = "0.0";
  std::vector<std::string> all_select_parsing;

  boost::split(all_select_parsing, plat_dev, boost::is_any_of("."));
  int x_p;
  int x_d;
  try {
    x_p = boost::lexical_cast<int>(all_select_parsing[0]);
    x_d = boost::lexical_cast<int>(all_select_parsing[1]);
  } catch (boost::bad_lexical_cast const &) {
    std::cout << "Error: input string was not valid" << std::endl;
  }

  std::vector<cl::Device> all_devices;
  all_platforms[x_p].getDevices(CL_DEVICE_TYPE_ALL, &all_devices);

  cl::Device default_device = all_devices[x_d];

  std::cout << " PL:" << all_platforms[x_p].getInfo<CL_PLATFORM_NAME>()
            << " DV:" << default_device.getInfo<CL_DEVICE_NAME>() << "\n";

  cl::Context context({default_device});

  cl::Program::Sources sources;

  // kernel calculates for each element C=A+B
  ifstream fie("ocl_src/frac.c");
  std::string kernel_code = readAllFile(fie);

  sources.push_back({kernel_code.c_str(), kernel_code.length()});

  cl::Program program(context, sources);
  if (program.build({default_device}) != CL_SUCCESS) {
    std::cout << " Error building: "
              << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)
              << "\n";
    exit(1);
  }

  int size = atoi(argv[1]);

  // create buffers on the device
  cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(float) * 2);
  cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(int) * 1);
  cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(int) * size * size);

  float A[] = {atof(argv[2]), atof(argv[3])};
  int B[] = {size};

  cl::CommandQueue queue(context, default_device);

  queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(float) * 2, A);
  queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(int) * 1, B);

  cl::Kernel kernel_add = cl::Kernel(program, "frac");

  kernel_add.setArg(0, buffer_A);
  kernel_add.setArg(1, buffer_B);
  kernel_add.setArg(2, buffer_C);

  queue.enqueueNDRangeKernel(kernel_add, cl::NullRange,
                             cl::NDRange(size * size), cl::NullRange);
  queue.finish();

  int *C = new int[size * size];
  queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(int) * size * size, C);
  std::cout << "end calcul" << std::endl;

  export_image(size, size, std::string(argv[4]), C);

  delete[] C;

  return 0;
}
