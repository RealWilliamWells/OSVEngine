//
// Created by william on 29/01/23.
//

#ifndef WIZENGINE3D_ASSETBASE_H
#define WIZENGINE3D_ASSETBASE_H

#include <string>

namespace wiz {
    class AssetBase;
}

class wiz::AssetBase {
public:
    virtual ~AssetBase() = default;

    virtual void* load() const = 0;

    virtual void dispose(void* data) const = 0;

    virtual const std::string& getName() const = 0;
};

#endif //WIZENGINE3D_ASSETBASE_H
