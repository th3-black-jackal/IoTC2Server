#pragma once
#include <optional>
#include <vector>

template<typename T>
class IRepository {
public:
    virtual ~IRepository() = default;
    virtual bool create(const T& model) = 0;
    virtual std::optional<T> getById(int id) = 0;
    virtual std::vector<T> getAll() = 0;
    virtual bool update(const T& model) = 0;
    virtual bool remove(int id) = 0;
};
