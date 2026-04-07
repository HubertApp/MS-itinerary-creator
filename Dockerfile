FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential cmake git curl zip pkg-config ninja-build

WORKDIR /opt
RUN git clone https://github.com/microsoft/vcpkg.git && \
    ./vcpkg/bootstrap-vcpkg.sh

WORKDIR /app
COPY vcpkg.json .
COPY . .

RUN cmake -B build -S . \
    -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake \
    -G Ninja
RUN cmake --build build

FROM ubuntu:22.04
WORKDIR /app
COPY --from=builder /app/build/astar_server .
EXPOSE 50051
CMD ["."]