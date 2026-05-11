FROM ubuntu:24.04 AS builder

RUN apt-get update && apt-get install -y \
    cmake \
    make \
    g++ \
    flex \
    bison \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY src/ ./src/
COPY CMakeLists.txt ./

RUN mkdir build && cd build && \
    cmake .. && \
    make

FROM ubuntu:24.04
RUN apt-get update && apt-get install -y libstdc++6 && rm -rf /var/lib/apt/lists/*
WORKDIR /root/
COPY --from=builder /app/build/compiler /usr/local/bin/compiler
ENTRYPOINT ["compiler"]