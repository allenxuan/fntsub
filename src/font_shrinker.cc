//
// Created by Xuanyi Huang on 2022/9/13.
//
#include <font_shrinker.h>
#include <ctime>
#include <iostream>

#include <cstdio>
#include <cstdlib>

#include <locale>
#include <codecvt>
#include <string>
#include <map>
#include <utility>
#include <fstream>
#include <cstring>

#include "sfntly/font.h"
#include "subtly/character_predicate.h"
#include "subtly/stats.h"
#include "subtly/subsetter.h"
#include "subtly/utils.h"

using namespace subtly;

namespace font_shrinker {
    std::wstring StringToWstring(const char *utf8Bytes);

    //根据路径获取文件名
    std::string GetPathOrURLShortName(const std::string &strFullName);

    int shrink(const char *font_path, const char *output_font_path, const char *utf8Bytes) {
        std::wstring wstr = StringToWstring(utf8Bytes);

        FontPtr font;
        font.Attach(subtly::LoadFont(font_path));
        if (font->num_tables() == 0) {
            fprintf(stderr, "Could not load font %s.\n", font_path);
            exit(1);
        }

        auto charaters = new std::set<int32_t>;
        for (const auto &unicode: wstr) {
            charaters->insert((int32_t) unicode);
        }

        Ptr<CharacterPredicate> set_predicate =
                new AcceptSet(charaters);
        Ptr<Subsetter> subsetter = new Subsetter(font, set_predicate);
        Ptr<Font> new_font;
        new_font.Attach(subsetter->Subset());
        if (!new_font) {
            fprintf(stderr, "Cannot create subset.\n");
            exit(1);
        }

        auto file_name = GetPathOrURLShortName(font_path);
        bool success = subtly::SerializeFont(output_font_path, new_font);
        if (!success) {
            fprintf(stderr, "Cannot create font file.\n");
            exit(1);
        }
        return 0;
    }

    std::wstring StringToWstring(const char *utf8Bytes) {
        using convert_type = std::codecvt_utf8<typename std::wstring::value_type>;
        std::wstring_convert<convert_type, typename std::wstring::value_type> converter;
        return converter.from_bytes(utf8Bytes);
    }

    //根据路径获取文件名
    std::string GetPathOrURLShortName(const std::string &strFullName) {
        if (strFullName.empty()) {
            return "";
        }
        std::string::size_type iPos = strFullName.find_last_of('/') + 1;
        return strFullName.substr(iPos, strFullName.length() - iPos);
    }
}

