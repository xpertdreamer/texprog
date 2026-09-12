## Build systems

If you wanna build on your local machine, you should have the following instruments:

  - **gcc** - compiler used to build the project
  - **make** - build automation tool 

Otherwise, if you have **Podman** on your machine, just run the container with the virtual environment inside:

``` shell
    podman build -t env:latest
    podman run --rm -it -v .:/app env:latest
```

Then, run the [build script](#make-commands) inside.

## Make commands

Build one target from list:

``` shell
    make -B <name from the following list>
```

Remove build directory:

``` shell
    make clean
```

List of targets:

  - lab1 — Laboratory Work №1

> [!NOTE]
> Make generates html-documentation on build

## Laboratory Work 1
### References:
  - <https://www.geeksforgeeks.org/cpp/map-associative-containers-the-c-standard-template-library-stl/>
  - <https://stackoverflow.com/questions/132358/how-to-read-file-content-into-istringstream>
  - <https://docs.podman.io/en/v1.6.4/markdown/podman-create.1.html>
  - <https://man.archlinux.org/man/podman.1.en>
  - <https://intellipaat.com/blog/tokenizing-a-string-cpp/>
  - <https://stackoverflow.com/questions/10051679/c-tokenize-string>
  - <https://stackoverflow.com/questions/51624933/how-exactly-do-lookup-tables-work-and-how-to-implement-them>
