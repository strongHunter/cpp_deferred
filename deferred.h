#ifndef DEFERRED_H
#define DEFERRED_H

// Base interface for deferred storage management using CRTP
template <typename Impl>
struct storage_base {
    ~storage_base() noexcept
    {
        Impl::destructImpl(impl());
    }

    decltype(auto) get() noexcept
    {
        return impl()->getImpl();
    }

    decltype(auto) get() const noexcept
    {
        return impl()->getImpl();
    }

    void set(auto&& val)
    {
        impl()->setImpl(std::forward<decltype(val)>(val));
    }

private:
    constexpr Impl* impl() noexcept
    {
        return static_cast<Impl*>(this);
    }

    constexpr const Impl* impl() const noexcept
    {
        return static_cast<const Impl*>(this);
    }
};


// Storage CRTP implementation
template <typename T>
class union_storage : public storage_base<union_storage<T>> {
    union { T value; };

public:
    friend class storage_base<union_storage<T>>;
    union_storage() noexcept {}
    ~union_storage() noexcept {}

    // I'm not a move constructor
    union_storage(T&& t) noexcept
            : value{ std::move(t) }
    {}

private:
    T& getImpl() noexcept
    {
        return value;
    }

    T const& getImpl() const noexcept
    {
        return value;
    }

    void setImpl(T&& t)
    {
        value = std::move(t);
    }

    static void destructImpl(union_storage* self) noexcept
    {
        reinterpret_cast<T*>(&self->value)->~T();
    }
};

// Alternative storage CRTP implementation
//
//template <typename T>
//class buffer_storage : public storage_base<buffer_storage<T>> {
//    alignas(T) std::byte storage[sizeof(T)];
//
//public:
//    friend class storage_base<buffer_storage<T>>;
//    buffer_storage() noexcept {}
//    ~buffer_storage() noexcept {}
//
//    // I'm not a move constructor
//    buffer_storage(T&& t) noexcept
//    {
//        setImpl(std::move(t));
//    }
//
//private:
//    T& getImpl() noexcept
//    {
//        return *reinterpret_cast<T*>(&storage);
//    }
//
//    const T& getImpl() const noexcept
//    {
//        return *reinterpret_cast<const T*>(&storage);
//    }
//
//    void setImpl(T&& t)
//    {
//        new (&storage) T(std::move(t));
//    }
//
//    static void destructImpl(buffer_storage* self) noexcept
//    {
//        reinterpret_cast<T*>(self->storage)->~T();
//    }
//};


template <typename T, template <typename> class Storage = union_storage>
class deferred {
    Storage<T> storage;

public:
    deferred() noexcept {}
    ~deferred() noexcept = default;

    // I'm not a move constructor
    deferred(T&& t) noexcept
            : storage{ std::move(t) }
    {}

    deferred& operator=(T&& t) noexcept
    {
        storage.set(std::move(t));
        return *this;
    }

public:
    T& get() & { return storage.get(); }
    T& get() && = delete;
    T const& get() const & { return storage.get(); }
    T const& get() const && = delete;

    operator T&() { return get(); }
    operator T const&() const { return get(); }

    T& operator*() { return get(); }
    T* operator->() { return &get(); }
};

#endif // DEFERRED_H
