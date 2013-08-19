#ifndef INSTANCECOUNTER_HPP_
#define INSTANCECOUNTER_HPP_

template<typename T>
class InstanceCounter {
    static int s_count;
protected:
    InstanceCounter() {
        ++s_count;
    }
    virtual ~InstanceCounter() {
        --s_count;
    }
public:
    static int getInstanceCount() {
        return s_count;
    }
};

template<typename T>
extern int InstanceCounter<T>::s_count;

#endif
