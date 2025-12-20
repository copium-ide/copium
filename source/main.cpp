import copium;
import copium.window;
import copium.time;
import copium.logging;

int
main()
{
	Copium::sayHi();

	Window window("window", 100, 100, 800, 600, {});
    window.open();

    Time::Delay fps(Time::FPS(5).getDuration(), true);

	while (!(window.closed))
    {
        WindowControls::pollEvents();
        if (window.shouldClose())
        {
            window.close();
        }
        if (fps.shouldFire())
        {
            Logging::print("INFO", 0, "render");
        }
        
    }
}