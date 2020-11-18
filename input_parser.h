#pragma once

#include <string_view>
#include <optional>
#include <utility>


using std::string_view;
using std::pair;

pair<string_view, std::optional<string_view>> SplitTwoStrict(string_view s, string_view delimiter);

pair<string_view, string_view> SplitTwo(string_view s, string_view delimiter);

string_view ReadToken(string_view& s, string_view delimiter);

int ConvertToInt(string_view str);

double ConvertToDouble(string_view str);
