#include "string.h"

#include <iomanip>
#include <sstream>

namespace tbox::util::string {

size_t Split(const std::string &src_str, const std::string sep, std::vector<std::string> &str_vec)
{
    size_t start_pos = 0;
    size_t end_pos = 0;
    str_vec.clear();
    while (true) {
        end_pos = src_str.find(sep, start_pos);
        const std::string &str_chip = src_str.substr(start_pos, end_pos - start_pos);
        str_vec.push_back(str_chip);
        if (end_pos == std::string::npos)
            break;
        start_pos = end_pos + sep.size();
    }
    return str_vec.size();
}

size_t SplitBySpace(const std::string &src_str, std::vector<std::string> &str_vec)
{
    size_t start_pos = 0;
    size_t end_pos = 0;
    str_vec.clear();
    while (true) {
        start_pos = src_str.find_first_not_of(" \t", end_pos);
        if (start_pos == std::string::npos)
            break;
        end_pos = src_str.find_first_of(" \t", start_pos);
        const std::string &str_chip = src_str.substr(start_pos, end_pos - start_pos);
        str_vec.push_back(str_chip);
        if (end_pos == std::string::npos)
            break;
    }
    return str_vec.size();
}

std::string StripLeft(const std::string &orig_str)
{
    size_t start_pos = orig_str.find_first_not_of(' ', 0);
    if (start_pos == std::string::npos)
        return std::string();
    return orig_str.substr(start_pos);
}

std::string StripRight(const std::string &orig_str)
{
    size_t end_pos = orig_str.find_last_not_of(' ', orig_str.size() - 1);
    if (end_pos == std::string::npos)
        return std::string();
    return orig_str.substr(0, end_pos + 1);
}

std::string Strip(const std::string &orig_str)
{
    return StripLeft(StripRight(orig_str));
}

std::string RawDataToHexStr(const void *data_ptr, uint16_t data_len, bool uppercase, const std::string &delimiter)
{
    if ((data_ptr == NULL) || (data_len == 0))
        return std::string();

    using namespace std;
    ostringstream oss;
    oss << hex << setfill('0');
    if (uppercase)
        oss << std::uppercase;

    const uint8_t *ptr = static_cast<const uint8_t*>(data_ptr);
    for (uint16_t i = 0; i < data_len; ++i) {
        oss << setw(2) << (int)ptr[i];
        if (i < (data_len - 1))
            oss << delimiter;
    }
    return oss.str();
}

namespace {
uint8_t hexCharToValue(char hex_char)
{
    if (('0' <= hex_char) && (hex_char <= '9'))
        return hex_char - '0';
    else if (('A' <= hex_char) && (hex_char <= 'F'))
        return hex_char - 'A' + 10;
    else if (('a' <= hex_char) && (hex_char <= 'f'))
        return hex_char - 'a' + 10;
    else
        return 0;
}

}

size_t HexStrToRawData(const std::string &hex_str, void *out_ptr, uint16_t out_len)
{
    if ((out_ptr == NULL) || (out_len == 0))
        return 0;

    uint8_t *p_data = (uint8_t*)out_ptr;
    size_t data_len = 0;
    for (size_t i = 0; (i < out_len) && ((i * 2 + 1) < hex_str.size()); ++i) {
        char h_char = hex_str[2 * i];
        char l_char = hex_str[2 * i + 1];
        p_data[i] = (hexCharToValue(h_char) << 4) | (hexCharToValue(l_char) & 0x0f);
        ++data_len;
    }
    return data_len;
}

void Replace(std::string &target_str, const std::string &pattern_str, const std::string &replace_str,
             std::string::size_type start, std::string::size_type count)
{
    if (count == 0)
        count = UINT32_MAX;

    std::string::size_type pos = start;
    std::string::size_type pattern_len = pattern_str.size();
    std::string::size_type replace_str_len = replace_str.size();

    while (count > 0 && (pos = target_str.find(pattern_str, pos)) != std::string::npos) {
        target_str.replace(pos, pattern_len, replace_str);
        pos += replace_str_len;
        --count;
    }
}

}


