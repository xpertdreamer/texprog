FROM docker.io/library/archlinux:base

RUN pacman -Syu --noconfirm && \
    pacman -S --noconfirm \
    gcc \
    make \
    glibc \
    && pacman -Scc --noconfirm

WORKDIR /app

CMD ["/bin/bash"]
