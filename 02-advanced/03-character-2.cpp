#include <iostream>
#include <string>

int main()
{
    std::string a = "abcdあいうえお";
    std::cout << a << "/" << a.length() << std::endl;

    std::u8string u8str = u8"abcdあいうえお";
    // std::cout << u8str << "/" << std::endl;

    std::wstring wa = L"abcdあいうえお";
    // std::wcout << wa << "/" << wa.length() << std::endl;

    std::u16string u16str = u"abcdあいうえお";

    // CP932(SJIS)
    {
        std::string s = "い"; // SJIS(82 A0), UTF8(E3 81 82)
        std::wstring ws = L"あいうえお";
        std::byte buff[2] = {};
        std::memcpy(buff, s.data(), 1);
        printf("%02x\n", std::to_integer<int>(buff[0]));
        std::cout << std::to_integer<int>(buff[0]) << std::endl;
    }
}