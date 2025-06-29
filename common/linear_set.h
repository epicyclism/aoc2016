#pragma once
#include <vector>
#include <algorithm>

template<typename T> struct linear_set : public std::vector<T>
{
    bool contains(T const& t) const noexcept
    {
        return std::find(this->begin(), this->end(), t) != this->end();
    }
    template<typename F> size_t add(F t) noexcept
    {
        auto i = std::find(this->begin(), this->end(), t);
        if(i == this->end())
        {
            this->emplace_back(std::move(t));
            return this->size() - 1;
        }
        return std::distance(this->begin(), i);
    }
};