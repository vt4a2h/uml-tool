namespace memory
{
    template <class Value>
    class shared_pointer
    {
    public:
        explicit shared_pointer(Value *value = nullptr);
        ~shared_pointer();
        void reset(Value *other = nullptr);
        size_t use_count() const;
        const Value *get() const;
        Value *get();

    private:
        void swap(shared_pointer &src);
    };

    template<class Value>
    shared_pointer<Value>::shared_pointer(Value *value)
    {
    }

    template<class Value>
    shared_pointer<Value>::~shared_pointer()
    {
    }

    template<class Value>
    void shared_pointer<Value>::reset(Value *other)
    {
    }

    template<class Value>
    size_t shared_pointer<Value>::use_count() const
    {
    }

    template<class Value>
    const Value *shared_pointer<Value>::get() const
    {
    }

    template<class Value>
    Value *shared_pointer<Value>::get()
    {
    }

}