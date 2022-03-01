#include "sd/gameplay/points.h"

#include <iostream>

#include <GLFW/glfw3.h>

#include "sd/framework/graphics/shaders.h"
#include "sd/framework/input/mouse_button_state_t.h"

namespace sd {
namespace gameplay {

    Points::Points(GLFWwindow* window) :
        IComponent(),
        IDrawable(),
        IMouseButtonChangeEventHandler(),
        m_Points(),
        m_Window(window),
        m_Rebuffer(true),
        m_ProgramId(0),
        m_VAO(0),
        m_VBO(0),
        m_MouseIsDown(false),
        m_LastXPos(0),
        m_LastYPos(0),
        m_Segments(),
        m_StartSegment(true),
        m_EndSegment(false),
        m_CurrentSegmentSize(0),
        m_CurrentSegmentStart(0),
        m_AltProgramId(0),
        m_CurrentSegmentProgram(0) {}

    void Points::Draw() {
        if (m_Points.empty()) {
            return;
        }
        glBindVertexArray(m_VAO);
        for (const auto& segment : m_Segments) {
            glUseProgram(segment.first);
            if (segment.second.second > 1) {
                glDrawArrays(GL_LINE_STRIP, segment.second.first, segment.second.second);
            } else {
                glDrawArrays(GL_POINTS, segment.second.first, segment.second.second);
            }
            
        }
        if (m_CurrentSegmentSize == 1) {
            glUseProgram(m_CurrentSegmentProgram);
            glDrawArrays(GL_POINTS, m_CurrentSegmentStart, m_CurrentSegmentSize);
        } else if (m_CurrentSegmentSize > 1) {
            glUseProgram(m_CurrentSegmentProgram);
            glDrawArrays(GL_LINE_STRIP, m_CurrentSegmentStart, m_CurrentSegmentSize);
        }
        glBindVertexArray(m_VAO);
        glUseProgram(0);
    }

    void Points::Initialize() {
        std::vector<std::pair<GLenum, const GLchar* const>> shaders;
        shaders.push_back(std::make_pair(GL_VERTEX_SHADER, sd::framework::graphics::shaders::vertex::default_shader));
        shaders.push_back(std::make_pair(GL_FRAGMENT_SHADER, sd::framework::graphics::shaders::fragment::default_shader));
        m_ProgramId = sd::framework::graphics::shaders::create_program(shaders);
        shaders.pop_back();
        shaders.push_back(std::make_pair(GL_FRAGMENT_SHADER, sd::framework::graphics::shaders::fragment::red_pixel));
        m_AltProgramId = sd::framework::graphics::shaders::create_program(shaders);
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexArray(0);
        glfwGetCursorPos(m_Window, &m_LastXPos, &m_LastYPos);
        m_CurrentSegmentProgram = m_ProgramId;
    }

    const std::vector<glm::vec3>& Points::GetPoints() const {
        return m_Points;
    }

    void Points::Update(double dt) {
        if (m_MouseIsDown) {
            double xpos, ypos;
            glfwGetCursorPos(m_Window, &xpos, &ypos);
            if (std::abs(xpos - m_LastXPos) <= std::numeric_limits<double>::epsilon() && std::abs(ypos - m_LastYPos) <= std::numeric_limits<double>::epsilon()) {
                return;
            }
            int height, width;
            glfwGetFramebufferSize(m_Window, &width, &height);
            double xcoord = xpos / (static_cast<double>(width) / 2.0) - 1.0;
            double ycoord = ypos / (static_cast<double>(height) / 2.0) - 1.0;
            m_LastXPos = xcoord;
            m_LastYPos = ycoord;

            m_Points.push_back(glm::vec3(xcoord, -ycoord, 0.0f));
            std::cout << "adding point: (" << xcoord << ", " << -ycoord << ")" << std::endl;
            glBindVertexArray(m_VAO);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_Points.size(), m_Points.data(), GL_STATIC_DRAW);
            glBindVertexArray(0);
            if (m_StartSegment) {
                m_StartSegment = false;
                m_CurrentSegmentSize = 1;
            } else {
                ++m_CurrentSegmentSize;
            }
        } else if (m_EndSegment) {
            m_EndSegment = false;
            m_StartSegment = true;
            m_Segments.push_back(std::make_pair(m_CurrentSegmentProgram, std::make_pair(m_CurrentSegmentStart, m_CurrentSegmentSize)));
            m_CurrentSegmentStart += m_CurrentSegmentSize;
            m_CurrentSegmentSize = 0;
            m_CurrentSegmentProgram = m_CurrentSegmentProgram == m_ProgramId ? m_AltProgramId : m_ProgramId;
        }
    }

    void Points::operator()(const std::vector<sd::framework::input::MouseButtonChangeEvent>& events) {
        using mouse_button_t = sd::framework::input::mouse_button_t;
        double xcoord = 0;
        double ycoord = 0;
        for (const auto& change_event : events) {
            if (change_event.button == mouse_button_t::left) {
                if (change_event.new_state.state == sd::framework::input::mouse_button_state_t::clicked) {
                    m_MouseIsDown = true;
                    m_StartSegment = true;
                } else if (change_event.old_state.state == sd::framework::input::mouse_button_state_t::released) {
                    m_MouseIsDown = false;
                    m_EndSegment = true;
                }
                xcoord = change_event.xcoord;
                ycoord = change_event.ycoord;
                break;
            }
        }

    }

}}