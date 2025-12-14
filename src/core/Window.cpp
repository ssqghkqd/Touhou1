module core.Window;
import glfw;
import spdlog;
import Config;
import opengl;
import utils.Time;

namespace th
{

Window::Window()
{
    init(window_width, window_height, window_title);
}


void Window::init(int width, int height, const char* title)
{
    if (m_inited)
    {
        spdlog::warn("window已经初始化");
        return;
    }

    if (!m_glfwInitialized)
    {
        if (!glfw::init())
        {
            spdlog::critical("GLFW初始化失败");
            throw;
        }
        m_glfwInitialized = true;
        spdlog::info("GLFW 初始化成功");
    }

    if (m_window)
    {
        spdlog::warn("窗口已经创建");
    }
    glfw::windowHint(glfw::scale_to_monitor, glfw::FALSE);  // 禁用系统缩放
    glfw::windowHint(glfw::scale_framebuffer, glfw::FALSE); // 禁用帧缓冲缩放
    glfw::windowHint(glfw::context_version_major, 3);
    glfw::windowHint(glfw::context_version_minor, 3);
    glfw::windowHint(glfw::opengl_profile, glfw::opengl_core_profile);
    glfw::windowHint(glfw::resizable, 0);
    m_window = glfw::createWindow(width * window_scale, height * window_scale, title, nullptr, nullptr);

    // 检查是否成功创建窗口
    if (!m_window)
    {
        spdlog::critical("窗口创建失败");
        throw;
    }

    spdlog::info("窗口创建成功");

    glfw::makeContextCurrent(m_window);

    if (!gl::loadGLLoader((gl::loadproc)glfw::getProcAddress))
    {
        spdlog::critical("GLAD初始化失败");
        throw;
    }
    spdlog::info("GLAD初始化成功");
    gl::viewport(0, 0, width * window_scale, height * window_scale);
    glfw::swapInterval(1); // 启用垂直同步（锁定显示器刷新率）
    spdlog::info("window初始化成功");
    m_inited = true;
}

Window::~Window()
{
    shutdown();
}

void Window::shutdown()
{
    if (m_window)
    {
        glfw::destroyWindow(m_window);
        m_window = nullptr;
    }
}

void Window::updateFPS()
{
    const double currentTime = Time::getTime();
    m_frameCount++;

    if (currentTime - m_lastTime >= stat_interval)
    {
        m_lastTime = currentTime;
        m_fps = m_frameCount;
        m_frameCount = 0;

        spdlog::info("FPS: {}, t: {:.2f}", m_fps, currentTime);
    }
}


void Window::close() const
{
    glfw::setWindowShouldClose(m_window, true);
}

bool Window::shouldClose() const
{
    return glfw::windowShouldClose(m_window);
}
// 交换缓冲区
void Window::swapBuffers() const
{
    glfw::swapBuffers(m_window);
}
// 轮询事件
void Window::pollEvents() const
{
    glfw::pollEvents();
}

// 判断按键函数
bool Window::isKeyPressed(int key) const
{

    return glfw::getKey(m_window, key) == glfw::press;
}

bool Window::isKeyRelease(int key) const
{
    return glfw::getKey(m_window, key) == glfw::release;
}
} // namespace th