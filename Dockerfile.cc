ARG CONTAINERD_VERSION=ali-CCv0

FROM golang:1.17-bullseye AS golang-base

FROM golang-base AS containerd-dev
ARG CONTAINERD_VERSION
RUN apt-get update -y && apt-get install -y libbtrfs-dev libseccomp-dev && \
    git clone -b ${CONTAINERD_VERSION} --depth 1 \
             https://github.com/confidential-containers/containerd $GOPATH/src/github.com/containerd/containerd && \
    cd $GOPATH/src/github.com/containerd/containerd && \
    make && DESTDIR=/out/ PREFIX= make install

FROM kindest/node:v1.23.1

COPY --from=containerd-dev /out/bin/containerd /out/bin/containerd-shim-runc-v2 /usr/local/bin/

ENTRYPOINT [ "/usr/local/bin/entrypoint", "/sbin/init" ]
