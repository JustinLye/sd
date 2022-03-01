#pragma once

#include <memory>
#include <queue>
#include <vector>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "sd/framework/graphics/buffer_segment.h"
#include "sd/framework/interfaces/IComponent.h"
#include "sd/framework/interfaces/IDrawable.h"
#include "sd/framework/interfaces/mouse_button_change_event_handler.h"
#include "sd/framework/input/mouse_button_state.h"
#include "sd/framework/input/mouse_button_t.h"
#include "sd/gameplay/world.h"

namespace sd {
namespace gameplay {

    class Points :
        public sd::framework::interfaces::IComponent,
        public sd::framework::interfaces::IDrawable,
        public sd::framework::interfaces::IMouseButtonChangeEventHandler {
    public:
        Points(std::shared_ptr<World> world);

        virtual void Draw() override;

        void Initialize();

        const std::vector<glm::vec3>& GetPoints() const;
        virtual void Update(double dt) override;
        virtual void operator()(const std::vector<sd::framework::input::MouseButtonChangeEvent>& events) override;

    private:
        std::shared_ptr<World> m_World;
        std::vector<glm::vec3> m_Points;
        GLFWwindow* m_Window;
        bool m_Rebuffer;
        GLuint m_ProgramId;
        GLuint m_VAO;
        GLuint m_VBO;
        bool m_MouseIsDown;
        double m_LastXPos;
        double m_LastYPos;
        std::vector<std::shared_ptr<sd::framework::graphics::BufferSegment>> m_Segments;
        bool m_StartSegment;
        bool m_EndSegment;
        std::size_t m_CurrentSegmentSize;
        std::size_t m_CurrentSegmentStart;
        GLuint m_AltProgramId;
        GLuint m_CurrentSegmentProgram;
        std::queue<sd::framework::input::MouseButtonChangeEvent> m_ChangeEventQueue;
        

    };

}}