#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <type_traits>

#include <glm/ext/vector_int2.hpp>
#include <glad/gl.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtx/string_cast.hpp>

#include "SomeGraphics/Rendering/DepthFrameBuffer.hpp"
#include "SomeGraphics/Rendering/UniformBuffer.hpp"
#include "SomeGraphics/Rendering/Program.hpp"
#include "SomeGraphics/Rendering/Material.hpp"
#include "SomeGraphics/Scene.hpp"
#include "SomeGraphics/Node.hpp"
#include "SomeGraphics/Mesh.hpp"
#include "SomeGraphics/Skin.hpp"

namespace sg {

class Camera;
class Scene;
class Skybox;
class VertexArray;
class PostProcess;
class Texture;

template<typename T>
concept AnyMesh = std::is_same_v<T, Mesh> || std::is_same_v<T, Skin>;

class Renderer {
public:
    Renderer();
    Renderer(Renderer&& other) = default;
    Renderer(const Renderer& other) = delete;
    Renderer& operator=(Renderer&& other) = default;
    Renderer& operator=(const Renderer& other) = delete;
    ~Renderer() = default;

    void set_viewport(glm::ivec2 dimensions) const;
    void clear() const;
    void set_clear_color(float red, float green, float blue, float opacity) const;
    void render(const Scene& scene, const Camera& camera, glm::ivec2 viewport_dimensions) const;
    void render(const Skybox& skybox, const Camera& camera) const;
    void post_process(const PostProcess& post_process, const Texture& texture) const;
    void set_framebuffer_srbg(bool state) const;
private:
    constexpr static uint32_t MAX_LIGHTS_COUNT = 32;
    constexpr static uint32_t MAX_SHADOW_MAPS_COUNT = 8;
    enum class RenderPass {
        Shadow,
        Shading,
    };
    struct ShadowPassMapInfo {
        glm::vec3 light_position;
        std::array<glm::mat4, 6> view_projections;
    };
    std::array<std::unique_ptr<DepthFrameBuffer>, MAX_SHADOW_MAPS_COUNT>
        m_shadow_pass_frame_buffers;
    std::unique_ptr<Program> m_shadow_mapping_program;
    mutable uint8_t m_shadow_maps_count = 0;
    mutable std::array<ShadowPassMapInfo, MAX_SHADOW_MAPS_COUNT> m_shadow_map_infos = {};
    UniformBuffer m_globals_uniform_buffer;
    UniformBuffer m_mesh_info_uniform_buffer;

    template<RenderPass render_pass>
    void render(const Scene& scene) const
    {
        auto render_any_mesh = [this, &scene](auto entity, auto& any_mesh) {
            const glm::mat4& model_matrix = scene.registry().get<Node>(entity).model_matrix();
            if constexpr (render_pass == RenderPass::Shadow) {
                render_shadow(*any_mesh, model_matrix, scene);
            } else {
                render(*any_mesh, model_matrix, scene);
            }
        };
        scene.registry().view<std::shared_ptr<Mesh>>().each(render_any_mesh);
        scene.registry().view<std::shared_ptr<Skin>>().each(render_any_mesh);
    }

    template<AnyMesh AnyMesh>
    void render_shadow(const AnyMesh& any_mesh, const glm::mat4& model_matrix, const Scene& scene) const
    {
        send_mesh_info(any_mesh, model_matrix, scene);
        const VertexArray& vertex_array = *any_mesh.vertex_array();
        vertex_array.bind();
        GLenum index_buffer_format = vertex_array.index_buffer()->format();
        for (uint8_t i = 0; i < m_shadow_maps_count; i++) {
            const ShadowPassMapInfo& shadow_map_info = m_shadow_map_infos[i];
            glm::vec3 light_position = shadow_map_info.light_position;
            m_shadow_mapping_program->set_vec3("u_light_position", light_position);
            const DepthFrameBuffer& frame_buffer = *m_shadow_pass_frame_buffers[i];
            frame_buffer.bind();
            for (CubemapFace face = CubemapFace::Begin; face < CubemapFace::End;
                face = CubemapFace(face + 1)) {
                frame_buffer.attach_face(face);
                m_shadow_mapping_program->set_mat4("u_view_projection",
                    shadow_map_info.view_projections[face]);
                for (const SubMeshInfo& sub_mesh_info : any_mesh.sub_meshes_info()) {
                    render<RenderPass::Shadow>(sub_mesh_info, index_buffer_format);
                }
            }
        }
    }

    template<AnyMesh AnyMesh>
    void render(const AnyMesh& any_mesh, const glm::mat4& model_matrix, const Scene& scene) const
    {
        send_mesh_info(any_mesh, model_matrix, scene);
        const VertexArray& vertex_array = *any_mesh.vertex_array();
        vertex_array.bind();
        GLenum index_buffer_format = vertex_array.index_buffer()->format();
        for (const SubMeshInfo& sub_mesh_info : any_mesh.sub_meshes_info()) {
            render<RenderPass::Shading>(sub_mesh_info, index_buffer_format);
        }
    }

    template<AnyMesh AnyMesh>
    void send_mesh_info(const AnyMesh& any_mesh, const glm::mat4& model_matrix, const Scene& scene) const
    {
        auto mesh_info = std::make_unique<MeshInfoUniformBlockData>();
        mesh_info->model = model_matrix;
        mesh_info->normal_matrix = glm::mat3x4(glm::inverse(glm::transpose(glm::mat3(model_matrix))));
        if constexpr (std::is_same_v<AnyMesh, Skin>) {
            mesh_info->is_rigged = true;
            glm::mat4 model_matrix_inverse = glm::inverse(model_matrix);
            uint32_t i = 0;
            for (const Bone& bone : any_mesh.bones()) {
                const Node* node = scene.registry().try_get<const Node>(bone.entity);
                if (!node) {
                    mesh_info->bone_transforms[i] = glm::mat4(1.0f);
                    i++;
                    continue;
                }
                glm::mat4 bone_node_to_skin = model_matrix_inverse * node->model_matrix();
                mesh_info->bone_transforms[i] = bone_node_to_skin * bone.skin_to_bone;
                i++;
            }
        } else {
            mesh_info->is_rigged = false;
        }
        m_mesh_info_uniform_buffer.update_data(*mesh_info);
    }

    template<RenderPass render_pass>
    void render(const SubMeshInfo& sub_mesh_info, GLenum index_buffer_format) const
    {
        if constexpr (render_pass == RenderPass::Shading) {
            const std::shared_ptr<Program>& program = sub_mesh_info.material()->program();
            program->use();
            constexpr static std::array<int, MAX_SHADOW_MAPS_COUNT> SHADOW_MAP_UNITS
                = { 15, 14, 13, 12, 11, 10, 9, 8 };
            program->set_ints("u_shadow_maps", std::span(SHADOW_MAP_UNITS)
                .subspan(0, m_shadow_pass_frame_buffers.size()));
            sub_mesh_info.material()->set_program_data();
        }
        glDrawElementsBaseVertex(GL_TRIANGLES, sub_mesh_info.indices_count(), index_buffer_format,
            sub_mesh_info.index_buffer_offset(), sub_mesh_info.vertices_offset());
    }

    void render(const VertexArray& vertex_array) const;

	static void GLAPIENTRY gl_debug_message_callback(GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar* message, const void* user_param);

    #pragma pack(push, 1)
    struct LightUniformData {
        glm::vec3 position;
        uint32_t : 32;
        glm::vec3 hdr_color;
        uint32_t : 32;
    };
    #pragma pack(pop)
    static_assert(sizeof(LightUniformData) == 32);
    static_assert(offsetof(LightUniformData, position) == 0);
    static_assert(offsetof(LightUniformData, hdr_color) == 16);
    #pragma pack(push, 1)
    struct GlobalsUniformBlockData {
        glm::mat4 view_projection;
        glm::vec3 camera_position;
        unsigned lights_count;
        LightUniformData lights[MAX_LIGHTS_COUNT];
        unsigned shadow_maps_count;
        uint32_t : 32;
        uint64_t : 64;
    };
    #pragma pack(pop)
    static_assert(sizeof(GlobalsUniformBlockData) == 1120);
    static_assert(offsetof(GlobalsUniformBlockData, view_projection) == 0);
    static_assert(offsetof(GlobalsUniformBlockData, camera_position) == 64);
    static_assert(offsetof(GlobalsUniformBlockData, lights_count) == 76);
    static_assert(offsetof(GlobalsUniformBlockData, lights[0]) == 80);
    static_assert(offsetof(GlobalsUniformBlockData, lights[1]) == 112);
    static_assert(offsetof(GlobalsUniformBlockData, shadow_maps_count) == 1104);

    #pragma pack(push, 1)
    struct MeshInfoUniformBlockData {
        glm::mat4 model;
        glm::mat3x4 normal_matrix;
        unsigned is_rigged;
        uint32_t : 32;
        uint64_t : 64;
        glm::mat4 bone_transforms[Skin::MAX_BONES_COUNT];
    };
    #pragma pack(pop)
    static_assert(sizeof(MeshInfoUniformBlockData) == 16512);
    static_assert(offsetof(MeshInfoUniformBlockData, model) == 0);
    static_assert(offsetof(MeshInfoUniformBlockData, normal_matrix) == 64);
    static_assert(offsetof(MeshInfoUniformBlockData, is_rigged) == 112);
    static_assert(offsetof(MeshInfoUniformBlockData, bone_transforms) == 128);
};

}
