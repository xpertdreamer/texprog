#include <dlfcn.h>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2) { std::cerr << "usage: " << argv[0] << " FILE\n"; return 1; }

    void* h = dlopen("./liblab2.so", RTLD_NOW); // RTLD_NOW (immedeatly) 0x00002
    if (!h) { std::cerr << dlerror() << "\n"; return 1; }

    auto run = reinterpret_cast<int(*)(const char*)>(dlsym(h, "lab2"));
    if (!run) { std::cerr << dlerror() << "\n"; dlclose(h); return 1; }

    int rc = run(argv[1]);
    dlclose(h);
    return rc;
}
