#ifndef INSTANCECOUNTER_HPP_
#define INSTANCECOUNTER_HPP_

template<typename T>
class InstanceCounter {
    static int s_count;
    static int s_maxCount;
protected:
    InstanceCounter();
    virtual ~InstanceCounter();
public:
    static int getInstanceCount();
    static int getMaxInstanceCount();
};

template<typename T>
int InstanceCounter<T>::s_count;
template<typename T>
int InstanceCounter<T>::s_maxCount;

template<typename T>
InstanceCounter<T>::InstanceCounter() {
    ++s_count;
    if(s_count > s_maxCount) {
        s_maxCount = s_count;
    }
}

template<typename T>
InstanceCounter<T>::~InstanceCounter() {
    --s_count;
}

template<typename T>
int InstanceCounter<T>::getInstanceCount() {
    return s_count;
}
template<typename T>
int InstanceCounter<T>::getMaxInstanceCount() {
    return s_maxCount;
}

#endif
