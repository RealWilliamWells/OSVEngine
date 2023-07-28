//
// Created by william on 28/07/23.
//

#ifndef OSVENGINE_CUBE_H
#define OSVENGINE_CUBE_H

#include "OSV/rendering/Model.h"

namespace osv::model {
    class Cube;

    namespace cube {
        static const std::vector<Vertex> vertices {
                {.position = {-0.5f , -0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, .texCoords = {0.0f  ,0.0f }},
                {.position = {0.5f , -0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,0.0f }},
                {.position = {0.5f , 0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,1.0f }},
                {.position = {0.5f , 0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,1.0f }},
                {.position = {-0.5f , 0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,1.0f }},
                {.position = {-0.5f , -0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,0.0f }},
                {.position = {-0.5f , -0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,0.0f }},
                {.position = {0.5f , -0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,0.0f }},
                {.position = {0.5f , 0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,1.0f }},
                {.position = {0.5f , 0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,1.0f }},
                {.position = {-0.5f , 0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,1.0f }},
                {.position = {-0.5f , -0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,0.0f }},
                {.position = {-0.5f , 0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,0.0f }},
                {.position = {-0.5f , 0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,1.0f }},
                {.position = {-0.5f , -0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,1.0f }},
                {.position = {-0.5f , -0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,1.0f }},
                {.position = {-0.5f , -0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,0.0f }},
                {.position = {-0.5f , 0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,0.0f }},
                {.position = {0.5f , 0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,0.0f }},
                {.position = {0.5f , 0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,1.0f }},
                {.position = {0.5f , -0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,1.0f }},
                {.position = {0.5f , -0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,1.0f }},
                {.position = {0.5f , -0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,0.0f }},
                {.position = {0.5f , 0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,0.0f }},
                {.position = {-0.5f , -0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,1.0f }},
                {.position = {0.5f , -0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,1.0f }},
                {.position = {0.5f , -0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,0.0f }},
                {.position = {0.5f , -0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,0.0f }},
                {.position = {-0.5f , -0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,0.0f }},
                {.position = {-0.5f , -0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,1.0f }},
                {.position = {-0.5f , 0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,1.0f }},
                {.position = {0.5f , 0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,1.0f }},
                {.position = {0.5f , 0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,0.0f }},
                {.position = {0.5f , 0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {1.0f  ,0.0f }},
                {.position = {-0.5f , 0.5f , 0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,0.0f }},
                {.position = {-0.5f , 0.5f , -0.5f }, .normal = {0.f, 0.f, 0.f}, . texCoords = {0.0f  ,1.0f }},
        };
    }
}

class osv::model::Cube : public osv::Model {
private:
    void initCube(glm::vec4 color);

public:
    Cube(glm::vec4 color);
};

#endif //OSVENGINE_CUBE_H
