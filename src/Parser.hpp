#pragma once

#include <CL/cl.hpp>
#include <map>
#include <string>
#include <vector>

#include <fstream>
#include <iostream>

class OpenCLWrapper {

private:
  cl::Buffer buffer_A;
  cl::Buffer buffer_B;
  cl::Buffer buffer_C;

  int context;

  std::vector<std::pair<cl::Platform, std::vector<cl::Device>>>
      list_plat_list_dev;

  std::vector<cl::Device> select_device;

  int max_x, max_y;

public:
  OpenCLWrapper();

  void getInfo();
  void print_plat_dev();
  bool selectByString(std::string select);
  void selectByDefault();
  void selectAll();

  void run(int h, int w, float jp_x, foat jp_y, float x, float y) {

    float A[] = {jp_x, jp_y};
    int B[] = {w, h};

    cl::CommandQueue queue(context, this->select_device);

    queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(float) * 2, A);
    queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(int) * 1, B);

    cl::Kernel kernel_add = cl::Kernel(program, "frac");

    kernel_add.setArg(0, buffer_A);
    kernel_add.setArg(1, buffer_B);
    kernel_add.setArg(2, buffer_C);

    queue.enqueueNDRangeKernel(kernel_add, cl::NullRange, cl::NDRange(h * w),
                               cl::NullRange);
    queue.finish();

    int *C = new int[h * w];
    queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(int) * h * w, C);

    std::cout << "end calcul" << std::endl;
  }

  void loadSource(std::string name_file_src, int max_x, int max_y) {

    this->max_x = max_x;
    this->max_y = max_y;

    std::ifstream file_src(name_file_src);
    std::string kernel_code = this->readAllFile(file_src);

    sources.push_back({kernel_code.c_str(), kernel_code.length()});

    cl::Program program(context, sources);

    if (program.build({default_device}) != CL_SUCCESS) {
      std::cout << " Error building: "
                << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(this->select_device)
                << "\n";
      exit(1);
    }

    this->buffer_A(context, CL_MEM_READ_WRITE, sizeof(float) * 2);
    this->buffer_B(context, CL_MEM_READ_WRITE, sizeof(int) * 1);
    this->buffer_C(context, CL_MEM_READ_WRITE, sizeof(int) * max_x * max_y);
  }

  Protected : bool reloadPlatformFound();

  bool Compile();

private:
  std::string readAllFile(std::ifstream &in) {
    std::stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
  }
};
