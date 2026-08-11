FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        cmake \
        g++ \
        nasm \
        ninja-build \
        git \
        ca-certificates \
        # libm-dev \
        libgrpc++-dev \
        libprotobuf-dev \
        protobuf-compiler \
        protobuf-compiler-grpc \
        libhiredis-dev \
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

FROM ubuntu:24.04 AS runner

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        libstdc++6 \
        libgrpc++1.51t64 \
        libprotobuf32t64 \
        libhiredis1.1.0 \
        # libm6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/build/astar_server .
COPY --from=builder /app/build/unit_tests   .

ENV REDIS_HOST=redis
ENV REDIS_PORT=6379

EXPOSE 50051

CMD ["./astar_server"]