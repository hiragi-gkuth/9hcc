FROM alpine as cclinux

WORKDIR /root

RUN apk update && apk add gcc g++ make binutils git bash emacs
RUN git clone https://github.com/HiragiGKUTH/9hcc.git && cd 9hcc && \
    git config --global user.email "docker@hiragi.com" && \
    git config --global user.name "docker.hiragi"
    


ENTRYPOINT ["/bin/bash"]
