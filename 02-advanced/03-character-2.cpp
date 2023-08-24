#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <cstddef>
using namespace std::literals::string_literals;

template <class String>
void dump(std::string name, String s, int len)
{
    std::cout << name;
    std::cout << "\t:";

    std::byte *buff = (std::byte *)std::calloc(len, sizeof(std::byte));
    std::memcpy(buff, s.data(), len);
    for (int i = 0; i < len; i++)
    {
        std::cout << " " << std::setfill('0') << std::setw(2) << std::hex << std::to_integer<int>(buff[i]);
    }
    std::cout << std::endl;
    std::free(buff);
}

int main()
{
    // �\�[�X�t�@�C����/execution-charset�Ŏw�肵���G���R�[�f�B���O
    std::string s = "abcd����������";
    std::cout << "std::string     : size=" << s.size() << std::endl; // SJIS����14(4+(2*5)),utf-8����19(4+(3*5))
    dump("std::string"s, s, 16);
    std::cout << std::endl;

    // Windows����UTF-16
    std::wstring wa = L"abcd����������";
    std::cout << "std::wstring    : size=" << wa.size() << std::endl; // 9(������)
    dump("std::wstring"s, wa, 16);
    std::cout << std::endl;

    // UTF-8
    std::u8string u8s = u8"abcd����������";
    std::cout << "std::u8string   : size=" << u8s.size() << std::endl; // 13(4+9?)
    dump("std::u8string"s, u8s, 16);
    std::cout << std::endl;

    // UTF-16
    std::u16string u16s = u"abcd����������";
    std::cout << "std::u16string  : size=" << u16s.size() << std::endl; // 9(������)
    dump("std::u16string"s, u16s, 16);
    std::cout << std::endl;

    // UTF-32
    std::u32string u32s = U"abcd����������";
    std::cout << "std::u32string  : size=" << u32s.size() << std::endl; // 9(������)
    dump("std::u32string"s, u32s, 16);
    std::cout << std::endl;
}