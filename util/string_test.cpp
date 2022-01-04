#include <gtest/gtest.h>
#include "string.h"

using namespace tbox;
using namespace tbox::util::string;

TEST(string, Split) {
    std::vector<std::string> str_vec;
    ASSERT_EQ(Split("A::BB:C::DD ", "::", str_vec), 3u);
    EXPECT_EQ(str_vec[0], std::string("A"));
    EXPECT_EQ(str_vec[1], std::string("BB:C"));
    EXPECT_EQ(str_vec[2], std::string("DD "));

    str_vec.clear();
    ASSERT_EQ(Split("A::BB:C::DD ", ":::", str_vec), 1u);
    EXPECT_EQ(str_vec[0], std::string("A::BB:C::DD "));

    str_vec.clear();
    ASSERT_EQ(Split("A::BB:C::DD ", ":", str_vec), 6u);
    EXPECT_EQ(str_vec[0], std::string("A"));
    EXPECT_EQ(str_vec[1], std::string(""));
    EXPECT_EQ(str_vec[2], std::string("BB"));
    EXPECT_EQ(str_vec[3], std::string("C"));
    EXPECT_EQ(str_vec[4], std::string(""));
    EXPECT_EQ(str_vec[5], std::string("DD "));
}

TEST(string, SplitBySpace) {
    std::vector<std::string> str_vec;
    ASSERT_EQ(SplitBySpace(" aa bb \t  cc  d ", str_vec), 4u);
    EXPECT_EQ(str_vec[0], "aa");
    EXPECT_EQ(str_vec[1], "bb");
    EXPECT_EQ(str_vec[2], "cc");
    EXPECT_EQ(str_vec[3], "d");

    str_vec.clear();
    ASSERT_EQ(SplitBySpace("aa b", str_vec), 2u);
    EXPECT_EQ(str_vec[0], "aa");
    EXPECT_EQ(str_vec[1], "b");

}

TEST(string, StripLeft) {
    EXPECT_EQ(StripLeft(" A "), "A ");
}

TEST(string, StripRight) {
    EXPECT_EQ(StripRight(" A "), " A");
}

TEST(string, Strip) {
    EXPECT_EQ(Strip(" A "), "A");
}

TEST(string, RawDataToHexStr) {
    uint8_t tmp[] = { 0x0e, 0x00, 0xa8 };
    EXPECT_EQ(RawDataToHexStr(tmp, sizeof(tmp), false, ""), "0e00a8");
    EXPECT_EQ(RawDataToHexStr(tmp, sizeof(tmp), false, " "), "0e 00 a8");
    EXPECT_EQ(RawDataToHexStr(tmp, sizeof(tmp), true, ""), "0E00A8");
    EXPECT_EQ(RawDataToHexStr(tmp, sizeof(tmp), true, " "), "0E 00 A8");
}

TEST(string, HexStrToRawData) {
    uint8_t tmp[100] = { 0 };
    EXPECT_EQ(HexStrToRawData("0123456789aBcDEf", tmp, 16), 8u);
    EXPECT_EQ(tmp[0], 0x01);
    EXPECT_EQ(tmp[1], 0x23);
    EXPECT_EQ(tmp[2], 0x45);
    EXPECT_EQ(tmp[3], 0x67);
    EXPECT_EQ(tmp[4], 0x89);
    EXPECT_EQ(tmp[5], 0xab);
    EXPECT_EQ(tmp[6], 0xcd);
    EXPECT_EQ(tmp[7], 0xef);
    EXPECT_EQ(tmp[8], 0x00);
    EXPECT_EQ(tmp[9], 0x00);

    //! 验证提供的输出缓冲不够的情况
    memset(tmp, 0, sizeof(tmp));
    EXPECT_EQ(HexStrToRawData("0123456789aBcDEf", tmp, 3), 3u);
    EXPECT_EQ(tmp[0], 0x01);
    EXPECT_EQ(tmp[1], 0x23);
    EXPECT_EQ(tmp[2], 0x45);
    EXPECT_EQ(tmp[3], 0x00);
    EXPECT_EQ(tmp[4], 0x00);

    //! 验证提供的HEX字串不全的情况
    memset(tmp, 0, sizeof(tmp));
    EXPECT_EQ(HexStrToRawData("01234", tmp, sizeof(tmp)), 2u);
    EXPECT_EQ(tmp[0], 0x01);
    EXPECT_EQ(tmp[1], 0x23);
    EXPECT_EQ(tmp[2], 0x00);
    EXPECT_EQ(tmp[3], 0x00);
}

TEST(string, Replace) {
    if (true) { //! 全替换
        std::string t("aa bbaa aab aaaa aab");
        std::string r;
        Replace(t, "aa", "cc");
        EXPECT_STREQ(t.c_str(), "cc bbcc ccb cccc ccb");
    }

    if (true) { //! 从某位置替换
        std::string t("aa bbaa aab aaaa aab");
        std::string r;
        Replace(t, "aa", "cc", 6);
        EXPECT_STREQ(t.c_str(), "aa bbaa ccb cccc ccb");
    }

    if (true) { //! 指定位置与替换次数
        std::string t("aa bbaa aab aaaa aab");
        std::string r;
        Replace(t, "aa", "cc", 6, 2);
        EXPECT_STREQ(t.c_str(), "aa bbaa ccb ccaa aab");
    }

    if (true) { //! 起始位置超出
        std::string t("aa bbaa aab aaaa aab");
        std::string r;
        Replace(t, "aa", "cc", t.size());
        EXPECT_STREQ(t.c_str(), "aa bbaa aab aaaa aab");
    }
}
