#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <thread>
#include <mutex>

class Renderer {
public:
    explicit Renderer(size_t height, size_t width, const std::string& title);
    ~Renderer();
    void wait();
    void close();
    void handle(const std::function<void(const sf::Event &)> &handler);
    void setPixel(size_t x, size_t y, sf::Color color = sf::Color(255, 255, 255));
    void clearPixel(size_t x, size_t y, sf::Color color = sf::Color(255, 255, 255));
    void clearScreen();
    void requestFrame();

    bool is_running();

private:
    size_t m_height;
    size_t m_width;
    std::thread m_eventLoop;
    std::thread m_render;
    sf::RenderWindow *m_renderWindow;
    sf::RectangleShape *m_shapes;
    bool *m_shouldRenderShape;
    sf::Event m_event;
    std::mutex m_renderMutex;
    std::mutex m_drawMutex;
    std::vector<std::function<void(const sf::Event&)>> m_eventHandlers;

    std::thread render();
    std::thread eventLoop();
};