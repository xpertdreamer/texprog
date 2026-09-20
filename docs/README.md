## Build systems

If you wanna build on your local machine, you should have the following instruments:

  - **gcc** - compiler used to build the project
  - **make** - build automation tool 

Otherwise, if you have **Podman** on your machine, just run the container with the virtual environment inside:

``` shell
    podman build . -t env:latest
    podman run --rm -v .:/app env:latest [target to build]
```

The build script runs automatically inside the container to build the specified target. The list of all targets can be found in the [make](#make-commands) section. 

## Random generator

The project root includes the `random.sh` script. It works on almost every Linux machine and generates 150 numbers in the range from 1 to 10000. 

To use it, run:

``` shell
sh random.sh
```

or 

``` shell
chmod +x random.sh
./random.sh
```

## Person generator

The project root also includes the `person.sh` script. It works incredibly simply: it takes base tokens (such as last name, first name, etc.), combines them into a single string, and that's it — you have new person data on your screen.

Usage examples are shown in the [Random generator](#random-generator) section.

## How to Use the CLIs

After building, you can run your target:

```shell
./build/lab_*/lab* -flags
```

Every binary without flags, or with the -h flag, shows a help message with all the relevant information.

## Make commands

> [!NOTE]
> Make generates html-documentation on build (/docs/html/index.html) (provide DOX=1 with make command)

Build one target from list:

``` shell
    make -B <name from the following list>
```

Remove build directory:

``` shell
    make clean
```

List of targets:

  - lab1 - Laboratory Work №1
  - lab2 - Laboratory Work №2

## Laboratory Work 1
### References:
  - <https://www.geeksforgeeks.org/cpp/map-associative-containers-the-c-standard-template-library-stl/>
  - <https://stackoverflow.com/questions/132358/how-to-read-file-content-into-istringstream>
  - <https://docs.podman.io/en/v1.6.4/markdown/podman-create.1.html>
  - <https://man.archlinux.org/man/podman.1.en>
  - <https://intellipaat.com/blog/tokenizing-a-string-cpp/>
  - <https://stackoverflow.com/questions/10051679/c-tokenize-string>
  - <https://stackoverflow.com/questions/51624933/how-exactly-do-lookup-tables-work-and-how-to-implement-them>
  - <http://blog.kislenko.net/show.php?id=2045>
  - <https://en.wikipedia.org/wiki/Prime_number>
  - <https://github.com/TheAlgorithms/C/blob/master/sorting/shell_sort.c>
  - <https://unix.stackexchange.com/questions/140750/generate-random-numbers-in-specific-range>


## Laboratory Work 2
### References:
  - <https://regex101.com/>
  - <https://habr.com/ru/companies/otus/articles/532056/>
  - <https://stackoverflow.com/questions/12908534/retrieving-a-regex-search-in-c>
  - <https://docs.asciidoctor.org/asciidoc/latest>
  - <https://stackoverflow.com/questions/56710024/what-is-a-raw-string>
  - <https://gist.github.com/elfefe/ef08e583e276e7617cd316ba2382fc40>
  - <https://github.com/opendevise/asciidoc-samples/blob/main/demo.adoc>
  - <https://dev.to/thormeier/how-to-generate-thousands-of-usernames-and-halfway-decent-passwords-with-bash-43m9>
  - <https://www.baeldung.com/linux/bash-string-character-loop>
  - <https://stackoverflow.com/questions/6212219/passing-parameters-to-a-bash-function>
  - <https://stackoverflow.com/questions/2264428/how-to-convert-a-string-to-lower-case-in-bash>
  - <https://stackoverflow.com/questions/2573834/c-convert-string-or-char-to-wstring-or-wchar-t>
  - <https://cplusplus.com/reference/sstream/wstringstream/>
