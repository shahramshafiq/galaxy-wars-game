#pragma once
#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>
using namespace std;

namespace GalaxyWars {

    class Resource {
        int val;
    public:
        Resource(int v = 0) : val(v) {}

        Resource(const Resource& other) : val(other.val) {
            cout << "Resource copied: " << val << endl;
        }

        int getValue() const {
            return val;
        }

        void setValue(int v) {
            val = v;
        }

        Resource operator+(const Resource& other) const
        {
            return Resource(this->val + other.val);
        }

        friend ostream& operator<<(ostream& os, const Resource& r);
    };

    ostream& operator<<(ostream& os, const Resource& r) {
        os << "Resource Value: " << r.val;
        return os;
    }

}

#endif
