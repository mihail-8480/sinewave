#include "Renderer.hpp"

#include <cmath>

class FunctionRenderer {
public:
    FunctionRenderer(size_t height, size_t width, const std::string& title) : m_renderer(height, width, title) {
        m_height = height;
        m_width = width;
        m_renderer.handle([&](sf::Event ev) {
            if (ev.type == sf::Event::EventType::Closed) {
                m_renderer.close();
                exit(0);
            }
        });
    }
    void plot(const std::function<float(float)> &y, float yMod = 1.0f, float xMod = 1.0f) {
        m_renderer.clearScreen();
        for(size_t x = 0; x < m_width; x++) {
            m_renderer.setPixel(x, m_height - (int)(y((float)x * xMod) * yMod), sf::Color(0,255,0));
        }
        m_renderer.requestFrame();
    }
    void wait() {
        m_renderer.wait();
    }

    bool is_running() {
        return m_renderer.is_running();
    }

private:
    size_t m_height;
    size_t m_width;
    Renderer m_renderer;
};


int main()
{
    FunctionRenderer plotter(512,512, "y = sin(x + t)");
    int offset = 0;
    while(plotter.is_running()) {
        using namespace std::chrono_literals;
        plotter.plot([offset](float x) {
            return (float)sin((double)((x + (float)offset) / 25)) * 256 + 256;
        });
        std::this_thread::sleep_for(10ms);
        offset++;
    }
    plotter.wait();
    return 0;
}