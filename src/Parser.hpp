#pragma once

#include <CL/cl.hpp>
#include <map>
#include <string>
#include <vector>

#include <fstream>
#include <iostream>
#include <sstream>

class OpenCLWrapper {

private:
  cl::Program program;
  cl::Program::Sources sources;
  cl::Context context;

  cl::Buffer buffer_A;
  cl::Buffer buffer_B;
  cl::Buffer buffer_C;

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

  unsigned int *run(int h, int w, float jp_x, float jp_y, float x, float y, float zoom) {

    float A[] = {jp_x, jp_y,x,y,zoom};
    int B[] = {h, w};

    cl::CommandQueue queue(context, this->select_device[0]);

    queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(float) * 5 , A);
    queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(int) * 2   , B);

    cl::Kernel kernel_add = cl::Kernel(program, "frac");

    kernel_add.setArg(0, buffer_A);
    kernel_add.setArg(1, buffer_B);
    kernel_add.setArg(2, buffer_C);

    queue.enqueueNDRangeKernel(kernel_add, cl::NullRange, cl::NDRange(h * w),
                               cl::NullRange);
    queue.finish();

    unsigned int *C = new unsigned int[h * w];

    queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(int) * h * w, C);

    std::cout << "end calcul" << std::endl;
    return C;
  }

  void loadSource(std::string name_file_src, int max_x, int max_y) {

    auto default_device = this->select_device[0];
    context = cl::Context({default_device});

    this->max_x = max_x;
    this->max_y = max_y;

    std::ifstream file_src(name_file_src);
    std::string kernel_code = this->readAllFile(file_src);

    sources.push_back({kernel_code.c_str(), kernel_code.length()});

    program = cl::Program(context, sources);

    if (program.build({default_device}) != CL_SUCCESS) {
      std::cout << " Error building: "
                << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(
                       this->select_device[0])
                << "\n";
      exit(1);
    }

    std::cout <<"No build error =)"  << "\n";

    this->buffer_A = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(float) * 5);
    this->buffer_B = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * 2);
    this->buffer_C =
        cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * max_x * max_y);

  }

protected:
  bool reloadPlatformFound();

  bool Compile();

private:
  std::string readAllFile(std::ifstream &in) {
    std::stringstream sstr;
    sstr << in.rdbuf();
    return sstr.str();
  }
};
