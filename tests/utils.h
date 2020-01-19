#ifndef UTILS_H
#define UTILS_H

#include <cassert>
#include <cstring>
#include <functional>
#include <stdexcept>

template <typename TException>
void AssertExceptions(std::function<void()> action, TException expectedException)
{
    try {
        action();
    }
    catch (TException ex)
    {
        assert(strcmp(expectedException.what(), ex.what()) == 0);
        return;
    }
    catch (exception)
    {
        assert(!"A wrong exception thrown");
    }
    assert(!"No exception thrown");
}

#endif