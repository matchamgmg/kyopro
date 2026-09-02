FROM gcc:15

RUN DEBIAN_FRONTED=noninteractive apt-get update && \
    apt-get install -y --no-install-recommends \
        git \
        gdb \
        python3 \
        ca-certificates \
        clang-format \
    && rm -rf /var/lib/apt/lists/*

RUN git clone --depth 1 \
    https://github.com/atcoder/ac-library.git \
    /opt/ac-library

RUN echo 'add-auto-load-safe-path /usr/local/lib64' > /root/.gdbinit