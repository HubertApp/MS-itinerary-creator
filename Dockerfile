FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        cmake \
        g++ \
        nasm \
        ninja-build \
        git \
        ca-certificates \
        libgrpc++-dev \
        libprotobuf-dev \
        protobuf-compiler \
        protobuf-compiler-grpc \
        libhiredis-dev \
        libgtest-dev \
        libbenchmark-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY CMakeLists.txt .
COPY proto/         proto/
COPY include/       include/
COPY src/           src/
COPY tests/         tests/

RUN cmake -G Ninja \
          -DCMAKE_BUILD_TYPE=Release \
          -DCMAKE_ASM_NASM_COMPILER=/usr/bin/nasm \
          -S . \
          -B build

RUN cmake --build build --parallel $(nproc)


RUN cd build && ctest --output-on-failure -L "unit|integration"

FROM ubuntu:24.04 AS runner

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        libstdc++6 \
        libgrpc++1.51t64 \
        libprotobuf32t64 \
        libhiredis1.1.0 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/build/astar_server       .
COPY --from=builder /app/build/unit_tests         .

COPY --from=builder /app/build/integration_tests  .


ENV ASTAR_LOG_LEVEL=INFO

EXPOSE 50051

CMD ["./astar_server"]
