//
// Created by william on 29/01/23.
//

#ifndef WIZENGINE3D_ASSET_H
#define WIZENGINE3D_ASSET_H

#include "AssetBase.h"

namespace wiz {
    template <typename T>
    class Asset;
}

template <typename T>
class wiz::Asset : public wiz::AssetBase {
public:
    void dispose(void* data) const override {
        delete (T*)data;
    }
};

#endif //WIZENGINE3D_ASSET_H
