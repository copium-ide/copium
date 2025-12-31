import copium;
import copium.window;
import copium.render.vulkan;
import copium.time;
import copium.logging;

int
main()
{
	Copium::sayHi();

	Window window("window", 100, 100, 800, 600, {
        WindowControls::RESIZABLE,
        WindowControls::VULKAN,
        WindowControls::TRANSPARENT
    });
    window.open();

    VulkanRender renderer(&window);

    Time::Delay fps(Time::FPS(60).getDuration(), true);

	while (!(window.closed))
    {
        WindowControls::pollEvents();
        if (window.shouldClose())
        {
            window.close();
        }
        if (fps.shouldFire())
        {
            renderer.renderBlank(0xaaffaaff);
        }
        
    }
}