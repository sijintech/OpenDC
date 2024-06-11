---
title: Quick start
description: a quick start to use compile openDC project
---
## Function Overview
Further Additions

## How to compile
Here, we provide an example of compiling the OpenDC project, using CMake files to manage the build process.

### Prerequisites

* You must install the intel **oneAPI** [**basekit**](https://www.intel.cn/content/www/cn/zh/developer/tools/oneapi/toolkits.html#base-kit) and [**hpckit**](https://www.intel.cn/content/www/cn/zh/developer/tools/oneapi/toolkits.html#hpc-kit). For detailed installation instructions, please refer to the [official oneAPI installation guide](https://www.intel.com/content/www/us/en/developer/articles/guide/installation-guide-for-oneapi-toolkits.html).
* **CMake** with a version greater than 3.20
* **OpenCV4**. Here is a simple installation [guide](https://medium.com/@juancrrn/installing-opencv-4-with-cuda-in-ubuntu-20-04-fde6d6a0a367).
### Compile OpenDC
The command for a simple compilation is as follows:
```bash
git clone https://github.com/sijintech/OpenDC.git # download the OpenDC project
cd OpenDC
source /opt/intel/oneapi/setvars.sh # initialize the intel environment
cmake --preset="linux-Debug" -S "." # configure the OpenDC cmake project
cmake --build --preset="linux-Debug" # build the OpenDC cmake project
```

## How to use
Further Additions
