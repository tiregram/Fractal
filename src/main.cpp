

#include <fstream>
#include <iostream>
#include <string>

#include "Image.hpp"
#include "Parser.hpp"

// string readAllFile(ifstream &in) {
//   stringstream sstr;
//   sstr << in.rdbuf();
//   return sstr.str();
// }

int main(int argc, char *argv[]) {

  auto oc = OpenCLWrapper();

  oc.print_plat_dev();

  std::cout << "select platforms.device platforms.device default(0.0):"
            << "\n";
  std::string plat_dev;
  std::getline(std::cin,plat_dev);

  std::cout << plat_dev << "\n";
  oc.selectByString(plat_dev);
  oc.print_plat_dev();

  // cl::Context context({default_device});

  // cl::Program::Sources sources;

  // // kernel calculates for each element C=A+B
  // ifstream fie("ocl_src/frac.c");
  // std::string kernel_code = readAllFile(fie);

  // sources.push_back({kernel_code.c_str(), kernel_code.length()});

  // cl::Program program(context, sources);
  // if (program.build({default_device}) != CL_SUCCESS) {
  //   std::cout << " Error building: "
  //             << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)
  //             << "\n";
  //   exit(1);
  // }

  // int size = atoi(argv[1]);

  // // create buffers on the device
  // cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(float) * 2);
  // cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(int) * 1);
  // cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(int) * size * size);

  // float A[] = {atof(argv[2]), atof(argv[3])};
  // int B[] = {size};

  // cl::CommandQueue queue(context, default_device);

  // queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(float) * 2, A);
  // queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(int) * 1, B);

  // cl::Kernel kernel_add = cl::Kernel(program, "frac");

  // kernel_add.setArg(0, buffer_A);
  // kernel_add.setArg(1, buffer_B);
  // kernel_add.setArg(2, buffer_C);

  // queue.enqueueNDRangeKernel(kernel_add, cl::NullRange,
  //                            cl::NDRange(size * size), cl::NullRange);
  // queue.finish();

  // int *C = new int[size * size];
  // queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(int) * size * size,
  // C);
  // std::cout << "end calcul" << std::endl;

  // export_image(size, size, std::string(argv[4]), C);

  // delete[] C;

  // return 0;
}
