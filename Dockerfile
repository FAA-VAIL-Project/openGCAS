FROM ubuntu:focal

WORKDIR ./openGCAS/

COPY src/ ./src/
COPY include/ ./include/
COPY data/ ./data/
COPY CMakeLists.txt ./

ENV CPLUS_INCLUDE_PATH=/usr/include/gdal
ENV C_INCLUDE_PATH=/usr/include/gdal

RUN apt-get update \
    && apt-get install -y software-properties-common \
    && apt-get update \
    && add-apt-repository ppa:ubuntugis/ppa \
    && apt-get update \
    && apt-get install -y gdal-bin libgdal-dev build-essential cmake \
    && cmake build . \
    && cmake --build .

CMD [ "./openGCAS" ]
