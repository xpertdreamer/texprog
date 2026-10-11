#include "parser.h"
#include "uploader.h"

#include <string>

int
main(void)
{
    std::string init = Uploader::upload_file("docs/README.md");
    Format detection = Parser::detect(init);
    std::wcout << Parser::to_string(detection) << std::endl;
    return 0;
}
