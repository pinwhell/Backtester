#pragma once

#include <vector>

template<typename T>
class Listeners {
public:
    std::vector<T> mSubscribers;
    std::vector<T> mPendingSubscribe;
    std::vector<T> mPendingUnsubscribe;

    bool IsSuscribed(T subscriber)
    {
        return std::find(mSubscribers.begin(), mSubscribers.end(), subscriber) != std::end(mSubscribers);
    }

    void Subscribe(T subscriber)
    {
        if (IsSuscribed(subscriber))
            return;

        mPendingSubscribe.push_back(subscriber);
    }

    void Remove(T subscriber)
    {
        if (IsSuscribed(subscriber) == false)
            return;

        mPendingUnsubscribe.push_back(subscriber);
    }

    void CompleteSubscribe()
    {
        for (auto toSub : mPendingSubscribe)
            mSubscribers.push_back(toSub);

        mPendingSubscribe.clear();
    }

    void CompleteUnsubscribe()
    {
        for(auto toUnsub : mPendingUnsubscribe)
            mSubscribers.erase(std::find(mSubscribers.begin(), mSubscribers.end(), toUnsub));

        mPendingUnsubscribe.clear();
    }
};
