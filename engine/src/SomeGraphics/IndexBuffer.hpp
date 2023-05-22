#pragma once

#include <sys/types.h>
#include <vector>

namespace sg {

class IndexBuffer {
public:
    IndexBuffer(const std::vector<uint>& indices);
    ~IndexBuffer();

    uint count() const;
private:
    uint m_id = 0;
    uint m_count = 0;
};

}
