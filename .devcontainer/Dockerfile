FROM debian:11-slim

RUN apt update && apt install -y \
      git \
      vim \
      build-essential \
      gcc-arm-none-eabi \
 && apt clean \
 && rm -rf /var/lib/apt/lists/*
