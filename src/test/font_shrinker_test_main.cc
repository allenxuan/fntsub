//
// Created by bytedance on 2022/9/13.
//
#include <font_shrinker.h>
void PrintUsage(const char* program_name);

int main(int argc, const char* argv[]) {
    const char *program_name = argv[0];
    if (argc < 5) {
        PrintUsage(program_name);
        exit(1);
    }

    clock_t start,end;
    start = clock();

    const char* characters = nullptr;
    if (std::strcmp(argv[3], "-s") == 0) {
        characters = argv[4];
    } else {
        PrintUsage(program_name);
        exit(1);
    }

    const char* input_font_path = argv[1];
    const char* output_font_path = argv[2];
    font_shrinker::shrink(input_font_path, output_font_path, characters);

    end = clock();
    printf("转换耗时 %.2f 毫秒", (end - start)/(double)CLOCKS_PER_SEC*1000);

    return 0;
}

void PrintUsage(const char* program_name) {
    fprintf(stdout, "Usage:\n\t%s <input_font_path> <output_font_path>"
                    " -s <string>\n", program_name);
}