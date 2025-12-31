module;
#include <bgfx/bgfx.h>
#include <vector>
#include <cstdint>
#include <cmath>
#include <mutex>
export module copium.render.pbr.mesh;

export namespace PBR {

constexpr float PI = 3.14159265359f;

struct Vertex
{
    float position[3];
    float normal[3];
    float tangent[4];
    float uv[2];

    static bgfx::VertexLayout
    getLayout()
    {
        bgfx::VertexLayout layout;
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Tangent, 4, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
        return layout;
    }
};

class Mesh
{
private:
    bgfx::VertexBufferHandle vertexBuffer = BGFX_INVALID_HANDLE;
    bgfx::IndexBufferHandle indexBuffer = BGFX_INVALID_HANDLE;
    uint32_t vertexCount = 0;
    uint32_t indexCount = 0;

public:
    Mesh() = default;

    ~Mesh()
    {
        release();
    }

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept
        : vertexBuffer(other.vertexBuffer), indexBuffer(other.indexBuffer),
          vertexCount(other.vertexCount), indexCount(other.indexCount)
    {
        other.vertexBuffer = BGFX_INVALID_HANDLE;
        other.indexBuffer = BGFX_INVALID_HANDLE;
        other.vertexCount = 0;
        other.indexCount = 0;
    }

    Mesh&
    operator=(Mesh&& other) noexcept
    {
        if (this != &other)
        {
            release();
            vertexBuffer = other.vertexBuffer;
            indexBuffer = other.indexBuffer;
            vertexCount = other.vertexCount;
            indexCount = other.indexCount;
            other.vertexBuffer = BGFX_INVALID_HANDLE;
            other.indexBuffer = BGFX_INVALID_HANDLE;
            other.vertexCount = 0;
            other.indexCount = 0;
        }
        return *this;
    }

    void
    create(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices)
    {
        release();

        vertexCount = static_cast<uint32_t>(vertices.size());
        indexCount = static_cast<uint32_t>(indices.size());

        if (vertexCount == 0) return;

        const bgfx::Memory* vertexMem = bgfx::copy(vertices.data(), vertexCount * sizeof(Vertex));
        vertexBuffer = bgfx::createVertexBuffer(vertexMem, Vertex::getLayout());

        if (indexCount > 0)
        {
            const bgfx::Memory* indexMem = bgfx::copy(indices.data(), indexCount * sizeof(uint16_t));
            indexBuffer = bgfx::createIndexBuffer(indexMem);
        }
    }

    void
    create32(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    {
        release();

        vertexCount = static_cast<uint32_t>(vertices.size());
        indexCount = static_cast<uint32_t>(indices.size());

        if (vertexCount == 0) return;

        const bgfx::Memory* vertexMem = bgfx::copy(vertices.data(), vertexCount * sizeof(Vertex));
        vertexBuffer = bgfx::createVertexBuffer(vertexMem, Vertex::getLayout());

        if (indexCount > 0)
        {
            const bgfx::Memory* indexMem = bgfx::copy(indices.data(), indexCount * sizeof(uint32_t));
            indexBuffer = bgfx::createIndexBuffer(indexMem, BGFX_BUFFER_INDEX32);
        }
    }

    void
    bind() const
    {
        if (bgfx::isValid(vertexBuffer))
        {
            bgfx::setVertexBuffer(0, vertexBuffer);
        }
        if (bgfx::isValid(indexBuffer))
        {
            bgfx::setIndexBuffer(indexBuffer);
        }
    }

    void
    release()
    {
        if (bgfx::isValid(vertexBuffer))
        {
            bgfx::destroy(vertexBuffer);
            vertexBuffer = BGFX_INVALID_HANDLE;
        }
        if (bgfx::isValid(indexBuffer))
        {
            bgfx::destroy(indexBuffer);
            indexBuffer = BGFX_INVALID_HANDLE;
        }
        vertexCount = 0;
        indexCount = 0;
    }

    bool
    isValid() const
    {
        return bgfx::isValid(vertexBuffer);
    }

    uint32_t
    getVertexCount() const
    {
        return vertexCount;
    }

    uint32_t
    getIndexCount() const
    {
        return indexCount;
    }

    const std::vector<Vertex>&
    getVertices() const = delete; // Not stored after upload

    const std::vector<uint16_t>&
    getIndices() const = delete; // Not stored after upload
};

class MeshBuilder
{
private:
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices16;
    std::vector<uint32_t> indices32;
    bool use32BitIndices = false;

    void
    migrateToIndices32()
    {
        if (!use32BitIndices && !indices16.empty())
        {
            indices32.reserve(indices16.size());
            for (uint16_t idx : indices16)
            {
                indices32.push_back(static_cast<uint32_t>(idx));
            }
            indices16.clear();
            use32BitIndices = true;
        }
    }

public:
    void
    clear()
    {
        vertices.clear();
        indices16.clear();
        indices32.clear();
        use32BitIndices = false;
    }

    void
    addVertex(const Vertex& vertex)
    {
        vertices.push_back(vertex);
    }

    void
    addIndex(uint32_t index)
    {
        if (index > 65535)
        {
            migrateToIndices32();
            indices32.push_back(index);
        }
        else
        {
            if (use32BitIndices)
            {
                indices32.push_back(index);
            }
            else
            {
                indices16.push_back(static_cast<uint16_t>(index));
            }
        }
    }

    void
    addTriangle(uint32_t i0, uint32_t i1, uint32_t i2)
    {
        addIndex(i0);
        addIndex(i1);
        addIndex(i2);
    }

    Mesh
    build()
    {
        Mesh mesh;
        if (use32BitIndices)
        {
            mesh.create32(vertices, indices32);
        }
        else
        {
            mesh.create(vertices, indices16);
        }
        return mesh;
    }

    const std::vector<Vertex>&
    getVertices() const
    {
        return vertices;
    }

    size_t
    getIndexCount() const
    {
        return use32BitIndices ? indices32.size() : indices16.size();
    }

    static Mesh
    createCube(float size = 1.0f)
    {
        MeshBuilder builder;
        float half = size * 0.5f;

        Vertex v[24];
        for (int i = 0; i < 24; ++i)
        {
            v[i] = Vertex{};
            v[i].uv[0] = 0.0f;
            v[i].uv[1] = 0.0f;
            v[i].tangent[3] = 1.0f;
        }

        // Front face (+Z)
        v[0].position[0] = -half; v[0].position[1] = -half; v[0].position[2] = half; v[0].normal[2] = 1.0f;
        v[1].position[0] = half; v[1].position[1] = -half; v[1].position[2] = half; v[1].normal[2] = 1.0f;
        v[2].position[0] = half; v[2].position[1] = half; v[2].position[2] = half; v[2].normal[2] = 1.0f;
        v[3].position[0] = -half; v[3].position[1] = half; v[3].position[2] = half; v[3].normal[2] = 1.0f;

        // Back face (-Z)
        v[4].position[0] = half; v[4].position[1] = -half; v[4].position[2] = -half; v[4].normal[2] = -1.0f;
        v[5].position[0] = -half; v[5].position[1] = -half; v[5].position[2] = -half; v[5].normal[2] = -1.0f;
        v[6].position[0] = -half; v[6].position[1] = half; v[6].position[2] = -half; v[6].normal[2] = -1.0f;
        v[7].position[0] = half; v[7].position[1] = half; v[7].position[2] = -half; v[7].normal[2] = -1.0f;

        // Top face (+Y)
        v[8].position[0] = -half; v[8].position[1] = half; v[8].position[2] = half; v[8].normal[1] = 1.0f;
        v[9].position[0] = half; v[9].position[1] = half; v[9].position[2] = half; v[9].normal[1] = 1.0f;
        v[10].position[0] = half; v[10].position[1] = half; v[10].position[2] = -half; v[10].normal[1] = 1.0f;
        v[11].position[0] = -half; v[11].position[1] = half; v[11].position[2] = -half; v[11].normal[1] = 1.0f;

        // Bottom face (-Y)
        v[12].position[0] = -half; v[12].position[1] = -half; v[12].position[2] = -half; v[12].normal[1] = -1.0f;
        v[13].position[0] = half; v[13].position[1] = -half; v[13].position[2] = -half; v[13].normal[1] = -1.0f;
        v[14].position[0] = half; v[14].position[1] = -half; v[14].position[2] = half; v[14].normal[1] = -1.0f;
        v[15].position[0] = -half; v[15].position[1] = -half; v[15].position[2] = half; v[15].normal[1] = -1.0f;

        // Right face (+X)
        v[16].position[0] = half; v[16].position[1] = -half; v[16].position[2] = half; v[16].normal[0] = 1.0f;
        v[17].position[0] = half; v[17].position[1] = -half; v[17].position[2] = -half; v[17].normal[0] = 1.0f;
        v[18].position[0] = half; v[18].position[1] = half; v[18].position[2] = -half; v[18].normal[0] = 1.0f;
        v[19].position[0] = half; v[19].position[1] = half; v[19].position[2] = half; v[19].normal[0] = 1.0f;

        // Left face (-X)
        v[20].position[0] = -half; v[20].position[1] = -half; v[20].position[2] = -half; v[20].normal[0] = -1.0f;
        v[21].position[0] = -half; v[21].position[1] = -half; v[21].position[2] = half; v[21].normal[0] = -1.0f;
        v[22].position[0] = -half; v[22].position[1] = half; v[22].position[2] = half; v[22].normal[0] = -1.0f;
        v[23].position[0] = -half; v[23].position[1] = half; v[23].position[2] = -half; v[23].normal[0] = -1.0f;

        // Front face (+Z) tangents
        v[0].tangent[0] = 1.0f; v[0].tangent[1] = 0.0f; v[0].tangent[2] = 0.0f;
        v[1].tangent[0] = 1.0f; v[1].tangent[1] = 0.0f; v[1].tangent[2] = 0.0f;
        v[2].tangent[0] = 1.0f; v[2].tangent[1] = 0.0f; v[2].tangent[2] = 0.0f;
        v[3].tangent[0] = 1.0f; v[3].tangent[1] = 0.0f; v[3].tangent[2] = 0.0f;

        // Back face (-Z) tangents
        v[4].tangent[0] = -1.0f; v[4].tangent[1] = 0.0f; v[4].tangent[2] = 0.0f;
        v[5].tangent[0] = -1.0f; v[5].tangent[1] = 0.0f; v[5].tangent[2] = 0.0f;
        v[6].tangent[0] = -1.0f; v[6].tangent[1] = 0.0f; v[6].tangent[2] = 0.0f;
        v[7].tangent[0] = -1.0f; v[7].tangent[1] = 0.0f; v[7].tangent[2] = 0.0f;

        // Top face (+Y) tangents
        v[8].tangent[0] = 1.0f; v[8].tangent[1] = 0.0f; v[8].tangent[2] = 0.0f;
        v[9].tangent[0] = 1.0f; v[9].tangent[1] = 0.0f; v[9].tangent[2] = 0.0f;
        v[10].tangent[0] = 1.0f; v[10].tangent[1] = 0.0f; v[10].tangent[2] = 0.0f;
        v[11].tangent[0] = 1.0f; v[11].tangent[1] = 0.0f; v[11].tangent[2] = 0.0f;

        // Bottom face (-Y) tangents
        v[12].tangent[0] = 1.0f; v[12].tangent[1] = 0.0f; v[12].tangent[2] = 0.0f;
        v[13].tangent[0] = 1.0f; v[13].tangent[1] = 0.0f; v[13].tangent[2] = 0.0f;
        v[14].tangent[0] = 1.0f; v[14].tangent[1] = 0.0f; v[14].tangent[2] = 0.0f;
        v[15].tangent[0] = 1.0f; v[15].tangent[1] = 0.0f; v[15].tangent[2] = 0.0f;

        // Right face (+X) tangents
        v[16].tangent[0] = 0.0f; v[16].tangent[1] = 0.0f; v[16].tangent[2] = -1.0f;
        v[17].tangent[0] = 0.0f; v[17].tangent[1] = 0.0f; v[17].tangent[2] = -1.0f;
        v[18].tangent[0] = 0.0f; v[18].tangent[1] = 0.0f; v[18].tangent[2] = -1.0f;
        v[19].tangent[0] = 0.0f; v[19].tangent[1] = 0.0f; v[19].tangent[2] = -1.0f;

        // Left face (-X) tangents
        v[20].tangent[0] = 0.0f; v[20].tangent[1] = 0.0f; v[20].tangent[2] = 1.0f;
        v[21].tangent[0] = 0.0f; v[21].tangent[1] = 0.0f; v[21].tangent[2] = 1.0f;
        v[22].tangent[0] = 0.0f; v[22].tangent[1] = 0.0f; v[22].tangent[2] = 1.0f;
        v[23].tangent[0] = 0.0f; v[23].tangent[1] = 0.0f; v[23].tangent[2] = 1.0f;

        for (int i = 0; i < 24; ++i) builder.addVertex(v[i]);

        uint16_t indices[] = {
            0, 1, 2, 0, 2, 3,       // Front
            4, 5, 6, 4, 6, 7,       // Back
            8, 9, 10, 8, 10, 11,    // Top
            12, 13, 14, 12, 14, 15, // Bottom
            16, 17, 18, 16, 18, 19, // Right
            20, 21, 22, 20, 22, 23  // Left
        };

        for (uint16_t idx : indices) builder.addIndex(idx);

        return builder.build();
    }

    static Mesh
    createSphere(float radius = 1.0f, uint32_t segments = 32, uint32_t rings = 16)
    {
        MeshBuilder builder;

        for (uint32_t ring = 0; ring <= rings; ++ring)
        {
            float theta = static_cast<float>(ring) / static_cast<float>(rings) * PI;
            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);

            for (uint32_t seg = 0; seg <= segments; ++seg)
            {
                float phi = static_cast<float>(seg) / static_cast<float>(segments) * 2.0f * PI;
                float sinPhi = std::sin(phi);
                float cosPhi = std::cos(phi);

                Vertex v{};
                v.position[0] = radius * sinTheta * cosPhi;
                v.position[1] = radius * cosTheta;
                v.position[2] = radius * sinTheta * sinPhi;
                v.normal[0] = sinTheta * cosPhi;
                v.normal[1] = cosTheta;
                v.normal[2] = sinTheta * sinPhi;
                v.uv[0] = static_cast<float>(seg) / static_cast<float>(segments);
                v.uv[1] = static_cast<float>(ring) / static_cast<float>(rings);
                v.tangent[0] = -sinPhi;
                v.tangent[1] = 0.0f;
                v.tangent[2] = cosPhi;
                v.tangent[3] = 1.0f;

                builder.addVertex(v);
            }
        }

        for (uint32_t ring = 0; ring < rings; ++ring)
        {
            for (uint32_t seg = 0; seg < segments; ++seg)
            {
                uint32_t current = ring * (segments + 1) + seg;
                uint32_t next = current + segments + 1;

                builder.addTriangle(current, next, current + 1);
                builder.addTriangle(current + 1, next, next + 1);
            }
        }

        return builder.build();
    }

    static Mesh
    createPlane(float width = 1.0f, float height = 1.0f, uint32_t segmentsX = 1, uint32_t segmentsY = 1)
    {
        MeshBuilder builder;

        float halfWidth = width * 0.5f;
        float halfHeight = height * 0.5f;

        for (uint32_t y = 0; y <= segmentsY; ++y)
        {
            for (uint32_t x = 0; x <= segmentsX; ++x)
            {
                Vertex v{};
                v.position[0] = -halfWidth + (static_cast<float>(x) / static_cast<float>(segmentsX)) * width;
                v.position[1] = 0.0f;
                v.position[2] = -halfHeight + (static_cast<float>(y) / static_cast<float>(segmentsY)) * height;
                v.normal[0] = 0.0f;
                v.normal[1] = 1.0f;
                v.normal[2] = 0.0f;
                v.uv[0] = static_cast<float>(x) / static_cast<float>(segmentsX);
                v.uv[1] = static_cast<float>(y) / static_cast<float>(segmentsY);
                v.tangent[0] = 1.0f;
                v.tangent[1] = 0.0f;
                v.tangent[2] = 0.0f;
                v.tangent[3] = 1.0f;

                builder.addVertex(v);
            }
        }

        for (uint32_t y = 0; y < segmentsY; ++y)
        {
            for (uint32_t x = 0; x < segmentsX; ++x)
            {
                uint32_t i0 = y * (segmentsX + 1) + x;
                uint32_t i1 = i0 + 1;
                uint32_t i2 = i0 + (segmentsX + 1);
                uint32_t i3 = i2 + 1;

                builder.addTriangle(i0, i2, i1);
                builder.addTriangle(i1, i2, i3);
            }
        }

        return builder.build();
    }
};

}