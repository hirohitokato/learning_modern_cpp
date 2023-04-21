/// auto�ɂ��^�̎������_�Ǝ擾
/// * `auto`���g���ƕϐ��̌^�������I�Ɍ���ł���B

#include <map>
#include <iostream>
#include <memory>
#include <functional>
#include <vector>

using namespace std;

int foo() { return 0; }

struct X
{
    int a;

    X(int a) : a(a) {}
    ~X()
    {
        cout << a << ": �f�X�g���N�^���Ă΂�܂���" << endl;
    }
    X(const X &other)
    {
        a = other.a + 1;
        cout << a << ": �R�s�[�R���X�g���N�^���Ă΂�܂���" << endl;
    }
};

class Interface
{
    virtual void foo() = 0;
};

class Implement : public Interface
{
public:
    void foo()
    {
        cout << "foo" << endl;
    }
};

///////////////////////////////////////////////
int main()
{
    {
        auto x = foo(); // x��int�^
        auto fx = foo;  // x��int�^��Ԃ��֐��|�C���^ `int (*)()`
        auto y = 1.0;   // y��double�^
    }
    cout << "-------------------------------------" << endl;
    {
        // �w��q��t���ăq���g�ɂ��o����
        const auto cx = foo(); // cx��const int�^
        static auto y = 1.0;   // y�͐ÓI��double�^
        int i = 100;
        auto p_i1 = &i;  // p_i1��int*�^
        auto *p_i2 = &i; // p_i2��int*�^
        // ���̓G���[�ɂȂ�B�^���l/�|�C���^/�Q�Ƃ̂ǂ�Ȃ̂��ӎ����ď����ƃR���p�C���G���[�ɂ��Ă���ėǂ�
        // auto *p_i3 = i;
    }
    cout << "-------------------------------------" << endl;
    {
        auto a = make_unique<int>(5);
        auto b = make_unique<int>(7);

        // �����_���̓R���p�C�������^��񂪕�����Ȃ� �� auto�Ő��_����ϐ��ł��������ł��Ȃ�(*)
        auto some_func = [](const std::unique_ptr<int> &p1, const std::unique_ptr<int> &p2)
        {
            return *p1 < *p2;
        };
        cout << "lambda ver.: " << some_func(a, b) << endl;
        // (���m�ɂ�std::function�Ń��b�v�ł��邪������������X�s�[�h����������)
        std::function<bool(const std::unique_ptr<int> &p1, const std::unique_ptr<int> &p2)>
            some_func2 = [](const std::unique_ptr<int> &p1, const std::unique_ptr<int> &p2)
        {
            return *p1 < *p2;
        };
        cout << "std::function ver.: " << some_func2(a, b) << endl;
    }
    cout << "-------------------------------------" << endl;
    {
        using namespace std;

        // �����_���̉������ɂ�auto���g����(�W�F�l���b�N�����_)�B
        auto f2 = [](auto a, auto b)
        { cout << "a is " << a << ", b is " << b << endl; };
        f2(2.4, 3);   // a��double�^�Ab��int�^�Ƃ݂Ȃ��Ă����B�e���v���[�g�֐����ǂ��ɂȂ�
        f2(1, "abc"); // a��int�^�Ab��const char* const�^�Ƃ݂Ȃ��Ă����B�e���v���[�g�֐����ǂ��ɂȂ�
    }
    cout << "-------------------------------------" << endl;
    {
        // �C�e���[�^�Ɏg���ƕ֗������S�ɂȂ�(1)
        using namespace std;

        // �^�̐錾���ԈႦ��ƕs�v�ȃR�s�[�������������Ă��܂��̂ŐϋɓI��auto���g��
        map<string, int> m = {{"a", 1}, {"c", 3}}; // ��memo:�����ł�`auto m2 = {...};`�Ƃ͏����Ȃ��B�R���p�C�����������Ă���Ȃ�

        for (const auto &e : m) // ����auto�̎��̂�`const std::pair<const std::string, int>&`
        {
            cout << e.first << " : " << e.second << endl;
        }
        // �����auto���g�킸for (const std::pair<std::string, int>& &e : m) {...}`�Ə����ƕs�v�ȃR�s�[�����������Ă��܂��I�I�i���Ƃŕ⑫�j
    }
    cout << "-------------------------------------" << endl;
    {
        // �C�e���[�^�Ɏg���ƕ֗������S�ɂȂ�(2)
        // auto�������Ƃ��̍H�v
        vector<X> x_vector = {X(1), X(2), X(3)};

        cout << "----" << endl;
        cout << "�R�s�[�R���X�g���N�^���Ă΂�Ă��܂�������(1)" << endl;
        for (X x : x_vector)
        {
            cout << "X(" << x.a << ")" << endl;
        }
        cout << "----" << endl;

        cout << "�R�s�[�R���X�g���N�^���Ă΂�Ă��܂�������(2)" << endl;
        for (auto x : x_vector)
        {
            cout << "X(" << x.a << ")" << endl;
        }
        cout << "----" << endl;

        cout << "�R�s�[�R���X�g���N�^���Ă΂�Ȃ��ǂ�������" << endl;
        for (const auto &x : x_vector)
        {
            cout << "X(" << x.a << ")" << endl;
        }
        cout << "----" << endl;
        cout << "�R�s�[�R���X�g���N�^�͌Ă΂�Ȃ����ʓ|������������" << endl;
        for (vector<X>::iterator it = x_vector.begin(), end = x_vector.end(); it != end; ++it)
        {
            cout << "X(" << (*it).a << ")" << endl;
        }
        cout << "----" << endl;
    }
    cout << "-------------------------------------" << endl;
    {
        // auto���g���Ȃ���ʁA�A�ł͂Ȃ��g��Ȃ������ǂ����

        auto *obj1 = static_cast<Interface *>(new Implement()); // Interface*�^
        auto *obj2 = new Implement();                           // Implement*�^�B

        // �ȉ��̋L�q�͕s���m�Ȃ̂ō폜�B���ӓ_�� https://pc98.skr.jp/post/2020/0616/ ���Q�Ƃ��Ăق���
        // Interface &ref = *obj;
        // // Interface obj = Implement();
        // vector<Interface &> a{};
        // a.push_back(ref);
    }
}

// Q1: auto���g���ƃX�s�[�h/�������͗�����H
//  A1: No. ��ŋ�����std::function�̋L�q�́Astd::function�Ń��b�v����̂Œǉ��̃��\�[�X�������Ƃ����b
// Q2: auto�ŃX�s�[�h�������邱�Ƃ͂���H
//  A2: �R���p�C�����Ɍ��܂�̂ŕς��Ȃ��i�R���p�C�����Ԃ͐L�т�j
// Q3: �C�e���[�^�Ŕz��ɃA�N�Z�X����Ƃ��̓R���e�i�̗v�f���Q�Ƃ��Ă���̂ŃR�s�[�R���X�g���N�^������Ȃ��H
//  A3: Yes
// Q4: �R�s�[�R���X�g���N�^�͏���ɍ����H�����ď������ƂŃI�[�o�[���C�h���Ă����H
//  A4: Yes
