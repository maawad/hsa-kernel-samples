FROM ubuntu:20.04
# FROM ubuntu:22.04
LABEL maintainer="Muhammad Awad <mawad@duck.com>"

# Install dependencies
RUN apt-get update && apt-get -y install wget build-essential git

# Install ROCm
# ROCm 5.4
# RUN wget https://repo.radeon.com/amdgpu-install/5.4/ubuntu/jammy/amdgpu-install_5.4.50400-1_all.deb
# RUN apt-get -y install ./amdgpu-install_5.4.50400-1_all.deb
# RUN DEBIAN_FRONTEND=noninteractive  amdgpu-install --accept-eula -y --usecase=rocm,hiplibsdk --no-dkms


# ROCm 5.2
# ubuntu:20.04
RUN wget https://repo.radeon.com/amdgpu-install/22.20/ubuntu/focal/amdgpu-install_22.20.50200-1_all.deb
RUN apt-get -y install ./amdgpu-install_22.20.50200-1_all.deb
RUN DEBIAN_FRONTEND=noninteractive  amdgpu-install --accept-eula -y --usecase=rocm,hiplibsdk --no-dkms

# ROCm 5.1
# ubuntu:20.04
# RUN apt-get update  && wget https://repo.radeon.com/amdgpu-install/22.10/ubuntu/focal/amdgpu-install_22.10.50100-1_all.deb
# RUN apt-get -y install ./amdgpu-install_22.10.50100-1_all.deb
# RUN DEBIAN_FRONTEND=noninteractive  amdgpu-install --accept-eula -y --usecase=rocm,hiplibsdk --no-dkms


# # Import the HSA ROCr GPG key
# RUN apt-get update && \
#     apt-get install -y gnupg && \
#     gpg --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys 55B24B10B10B9E9D && \
#     gpg --export --armor 55B24B10B10B9E9D | apt-key add - && \
#     echo 'deb [arch=amd64] https://repo.radeon.com/rocm/apt/debian/ focal main' > /etc/apt/sources.list.d/rocm.list && \
#     apt-get clean && \
#     rm -rf /var/lib/apt/lists/*


# # Install the HSA runtime development package
# RUN apt-get update && \
#     apt-get install -y lib-hsa-runtime-dev && \
#     apt-get clean && \
#     rm -rf /var/lib/apt/lists/*

# Set environment variables
ENV PATH="$PATH:/opt/rocm/bin:/opt/rocm/rocprofiler/bin:/opt/rocm/hip/bin"
ENV LD_LIBRARY_PATH="/opt/rocm/lib:/opt/rocm/llvm/lib:/opt/rocm/hip/lib"

# Install CMake
ARG CMAKE_VERSION=3.25.1
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.sh &&\
    chmod +x cmake-${CMAKE_VERSION}-linux-x86_64.sh &&\
    mkdir /opt/cmake && \
    ./cmake-${CMAKE_VERSION}-linux-x86_64.sh  --skip-license --prefix=/opt/cmake  &&\
    ln -s /opt/cmake/bin/* /usr/local/bin/

RUN apt-get install -y libnuma-dev
RUN apt-get install -y gdb valgrind