FROM ubuntu:20.04

#RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/mirrors.aliyun.com/g" /etc/apt/sources.list && \
#         sed -i "s/http:\/\/security.ubuntu.com/http:\/\/mirrors.aliyun.com/g" /etc/apt/sources.list

RUN set -ex \
    && apt-get update \
    && DEBIAN_FRONTEND="noninteractive" apt-get -y install tzdata ca-certificates wget qemu-system cloud-image-utils

RUN mkdir /home/chall

WORKDIR /home/chall

# old version ubuntu
RUN wget http://cloud-images-archive.ubuntu.com/releases/focal/release-20200423/ubuntu-20.04-server-cloudimg-amd64.img
	
COPY ./user-data /home/chall/

RUN cloud-localds user-data.img user-data && \
    qemu-img resize ubuntu-20.04-server-cloudimg-amd64.img +20G

EXPOSE 2222

CMD qemu-system-x86_64 \
  -drive "file=ubuntu-20.04-server-cloudimg-amd64.img,format=qcow2" \
  -drive "file=user-data.img,format=raw" \
  -m 2G \
  -nic user,model=virtio,hostfwd=tcp:0.0.0.0:2222-:22 \
  -smp $(nproc) \
  -nographic \
  -monitor null
