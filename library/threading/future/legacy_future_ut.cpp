#include "legacy_future.h"

#include <library/unittest/registar.h>

namespace NThreading {

SIMPLE_UNIT_TEST_SUITE(TLegacyFutureTest) {
    int intf() {
        return 17;
    }

    SIMPLE_UNIT_TEST(TestIntFunction) {
        TLegacyFuture<int> f((&intf));
        UNIT_ASSERT_VALUES_EQUAL(17, f.Get());
    }

    static int r;

    void voidf() {
        r = 18;
    }

    SIMPLE_UNIT_TEST(TestVoidFunction) {
        r = 0;
        TLegacyFuture<> f((&voidf));
        f.Get();
        UNIT_ASSERT_VALUES_EQUAL(18, r);
    }

    struct TSampleClass {
        int mValue;

        TSampleClass(int value)
            : mValue(value)
        {
        }

        int Calc() {
            return mValue + 1;
        }
    };

    SIMPLE_UNIT_TEST(TestMethod) {
        TLegacyFuture<int> f11(std::bind(&TSampleClass::Calc, TSampleClass(3)));
        UNIT_ASSERT_VALUES_EQUAL(4, f11.Get());

        TLegacyFuture<int> f12(std::bind(&TSampleClass::Calc, TSampleClass(3)), SystemThreadPool());
        UNIT_ASSERT_VALUES_EQUAL(4, f12.Get());

        TSampleClass c(5);

        TLegacyFuture<int> f21(std::bind(&TSampleClass::Calc, std::ref(c)));
        UNIT_ASSERT_VALUES_EQUAL(6, f21.Get());

        TLegacyFuture<int> f22(std::bind(&TSampleClass::Calc, std::ref(c)), SystemThreadPool());
        UNIT_ASSERT_VALUES_EQUAL(6, f22.Get());
    }

    struct TSomeThreadPool: public IThreadPool {};

    SIMPLE_UNIT_TEST(TestFunction) {
        std::function<int()> f((&intf));

        UNIT_ASSERT_VALUES_EQUAL(17, TLegacyFuture<int>(f).Get());
        UNIT_ASSERT_VALUES_EQUAL(17, TLegacyFuture<int>(f, SystemThreadPool()).Get());

        if (false) {
            TSomeThreadPool* q = nullptr;
            TLegacyFuture<int>(f, q); // just check compiles, do not start
        }
    }
}

}