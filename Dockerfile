FROM ubuntu:16.04
MAINTAINER asterix24@gmail.com

RUN apt-get update \
    && apt-get install -y \
        build-essential \
        make \
        bzip2 \
        curl \
        python \
        && apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

SHELL ["/bin/bash", "-c"]
ADD https://toolchain.develboard.com/develboard-glibc-x86_64-meta-toolchain-b2qt-embedded-qt5-sdk-cortexa5hf-vfp-neon-toolchain-1.8.1.sh /tmp/develboard-toolchain.sh
RUN chmod +x /tmp/develboard-toolchain.sh && /tmp/develboard-toolchain.sh && rm /tmp/develboard-toolchain.sh

# cannot build as root
RUN useradd -s /bin/bash --create-home develboard

USER develboard
WORKDIR /home/develboard


CMD ["/bin/bash -c", "source /opt/develboard/1.8.1/environment-setup-cortexa5hf-vfp-neon-develer-linux-gnueabi && qmake && make"]


