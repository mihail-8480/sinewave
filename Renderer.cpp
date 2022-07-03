#include "Renderer.hpp"
#include <cstring>
void Renderer::close() {
    m_renderWindow->close();
}

void Renderer::handle(const std::function<void(const sf::Event&)> &handler) {
    m_eventHandlers.push_back(handler);
}

void Renderer::setPixel(size_t x, size_t y, sf::Color color) {
    if (x >= m_width || x < 0) {
        return;
    }
    if (y >= m_height || y < 0) {
        return;
    }
    m_drawMutex.lock();
    m_shapes[y * m_width + x] = sf::RectangleShape({1.0f,1.0f});
    m_shapes[y * m_width + x].setPosition((float)x, (float)y);
    m_shapes[y * m_width + x].setFillColor(color);
    m_shouldRenderShape[y * m_width + x] = true;
    m_drawMutex.unlock();
}


void Renderer::clearScreen() {
    m_drawMutex.lock();
    memset(m_shouldRenderShape, false, sizeof(bool) * m_height * m_width);
    m_drawMutex.unlock();
}

void Renderer::clearPixel(size_t x, size_t y, sf::Color color) {
    m_drawMutex.lock();
    m_shouldRenderShape[y * m_width + x] = false;
    m_drawMutex.unlock();
}

void Renderer::requestFrame() {
    m_renderMutex.unlock();
    m_drawMutex.lock();
}

void Renderer::wait() {
    m_eventLoop.join();
    m_render.join();
}

std::thread Renderer::eventLoop() {
    return std::thread([&]() {
        while (m_renderWindow->isOpen())
        {
            while (m_renderWindow->waitEvent(m_event))
            {
                for(const auto& handler : m_eventHandlers) {
                    handler(m_event);
                }
            }
        }
    });
}


std::thread Renderer::render() {
    return std::thread([&]() {
        while (m_renderWindow->isOpen()) {
            m_renderMutex.lock();
            m_renderWindow->clear();
            for(size_t y = 0; y < m_height; y++) {
                for(size_t x = 0; x < m_width; x++) {
                    if (m_shouldRenderShape[y * m_width + x]) {
                        m_renderWindow->draw(m_shapes[y * m_width + x]);
                    }
                }
            }
            m_renderWindow->display();
            m_drawMutex.unlock();
        }
    });
}
Renderer::~Renderer() {
    delete m_renderWindow;
    delete[] m_shouldRenderShape;
    delete[] m_shapes;
}

Renderer::Renderer(size_t height, size_t width, const std::string& title) : m_height(height), m_width(width), m_event(), m_renderMutex(), m_drawMutex() {
    m_renderWindow = new sf::RenderWindow(sf::VideoMode(height, width), title);
    m_shapes = new sf::RectangleShape[height * width];
    m_shouldRenderShape = new bool[height * width];
    m_eventLoop = eventLoop();
    m_render = render();
    m_renderMutex.lock();
}

bool Renderer::is_running() {
    return m_renderWindow->isOpen();
}
