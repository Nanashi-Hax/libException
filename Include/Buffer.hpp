#pragma once

#include <cstdint>
#include <atomic>

namespace Library::Debug
{
    template<typename K, typename V, uint32_t Max>
    class Map
    {
    public:
        bool insert(const K& key, const V& value)
        {
            for (uint32_t i = 0; i < count; i++)
            {
                if (data[i].key == key)
                {
                    data[i].value = value;
                    return true;
                }
            }
            if (count >= Max) return false;
            data[count++] = { key, value };
            return true;
        }

        bool exist(const K& key) const
        {
            for (uint32_t i = 0; i < count; i++) if (data[i].key == key) return true;
            return false;
        }

        V* find(const K& key)
        {
            for (uint32_t i = 0; i < count; i++) if (data[i].key == key) return &data[i].value;
            return nullptr;
        }

        const V* find(const K& key) const
        {
            for (uint32_t i = 0; i < count; i++) if (data[i].key == key) return &data[i].value;
            return nullptr;
        }

        bool erase(const K& key)
        {
            for (uint32_t i = 0; i < count; i++)
            {
                if (data[i].key == key)
                {
                    data[i] = data[count - 1]; // swap remove
                    count--;
                    return true;
                }
            }
            return false;
        }

        uint32_t size() const { return count; }

    private:
        struct Entry
        {
            K key;
            V value;
        };

        Entry data[Max];
        uint32_t count = 0;
    };

    template<typename T, uint32_t Size>
    class RingBuffer
    {
        static_assert((Size & (Size - 1)) == 0, "Size must be power of two");
    
    public:
        bool push(const T& value)
        {
            uint32_t t = tail.load(std::memory_order_relaxed);
            uint32_t next = (t + 1) & (Size - 1);
            if (next == head.load(std::memory_order_acquire)) return false; // full
            buffer[t] = value;
            tail.store(next, std::memory_order_release);
            return true;
        }
    
        bool pop(T& out)
        {
            uint32_t h = head.load(std::memory_order_relaxed);
            if (h == tail.load(std::memory_order_acquire)) return false; // empty
            out = buffer[h];
            head.store((h + 1) & (Size - 1), std::memory_order_release);
            return true;
        }
    
        void clear()
        {
            head.store(0, std::memory_order_relaxed);
            tail.store(0, std::memory_order_relaxed);
        }
    
    private:
        alignas(64) std::atomic<uint32_t> head{0};
        alignas(64) std::atomic<uint32_t> tail{0};
        T buffer[Size];
    };
}