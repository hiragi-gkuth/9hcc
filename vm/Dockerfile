FROM alpine as cclinux

WORKDIR /root

RUN apk update && apk add gcc g++ make binutils git bash emacs

ENTRYPOINT ["/bin/bash"]
