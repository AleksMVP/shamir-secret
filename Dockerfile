FROM ubuntu:latest as executor

ENV TZ=Europe/Minsk
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && \
    apt-get -y install --no-install-recommends \
    libboost-all-dev \
    g++

WORKDIR /app

COPY . /app

RUN g++ main.cpp -std=c++17 -o main.out -Wall -Werror -Wextra

ENTRYPOINT bash