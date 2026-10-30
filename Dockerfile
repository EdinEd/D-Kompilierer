FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    cmake \
    make \
    g++ \
    flex \
    bison \
    llvm-dev \
    zlib1g-dev \
    libzstd-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY src/ ./src/
COPY CMakeLists.txt ./

RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    make -j"$(nproc)"

FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    llvm \
    clang \
    g++ \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace
COPY --from=builder /app/build/compiler /usr/local/bin/compiler
COPY tests/ ./tests/

ENTRYPOINT ["compiler"]
