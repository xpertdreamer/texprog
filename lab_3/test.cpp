#include "parser.h"
#include "uploader.h"

#include <string>
#include <iostream>

int
main(void)
{
    std::string init = Uploader::upload_file("docs/html/cmd_8h.html");
    Format detection = Parser::detect(init);
    std::wcout << Parser::to_string(detection) << std::endl;
    return 0;
}
