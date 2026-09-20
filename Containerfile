FROM docker.io/library/archlinux:base

RUN pacman -Sy --noconfirm && \
    pacman -S --noconfirm \
    gcc \
    make \
    doxygen \
    && pacman -Scc --noconfirm

WORKDIR /app

CMD ["/bin/bash"]

ENTRYPOINT ["./bash/build.sh"]
