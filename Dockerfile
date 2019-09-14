FROM ubuntu:18.04
USER root

RUN apt-get update
RUN apt-get -y install libboost-all-dev && apt -y install g++ && apt-get -y install cmake

WORKDIR /server
COPY . /server

COPY ./httpd.conf /etc/httpd.conf
COPY ./httptest /var/www/html/httptest

RUN cmake CMakeLists.txt
RUN make

EXPOSE 80

CMD ./http-server

